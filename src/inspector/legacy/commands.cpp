

#include "commands.h"

#include <QString>
#include <QBuffer>
#include <QJSValue>
#include <QJSValueIterator>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QMetaObject>
#include <QNetworkAccessManager>
#include <QPixmap>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickWindow>
#include <QScreen>
#include <QTest>
#include <functional>

#include "constants.h"
#include "feature.h"
#include "frontend/navigator.h"
#include "../tools/qquickinspector.h"
#include "../inspector.h"
#include "leakdetector.h"
#include "localizer.h"
#include "logger.h"
#include "loghandler.h"
#include "models/featuremodel.h"
#include "mzglean.h"
#include "networkmanager.h"
#include "qmlengineholder.h"
#include "settingsholder.h"
#include "task.h"
#include "urlopener.h"
#include "utils.h"

#ifdef MZ_WASM
#  include "platforms/wasm/wasminspector.h"
#else
#  include <QCoreApplication>
#endif


namespace {
Logger logger("CommandHandler");

QJsonValue qjsValueToQJsonValue(const QJSValue& qjsValue) {
  if (qjsValue.isUndefined() || qjsValue.isNull()) {
    return QJsonValue(QJsonValue::Undefined);
  } else if (qjsValue.isBool()) {
    return QJsonValue(qjsValue.toBool());
  } else if (qjsValue.isNumber()) {
    return QJsonValue(qjsValue.toNumber());
  } else if (qjsValue.isString()) {
    return QJsonValue(qjsValue.toString());
  } else if (qjsValue.isArray()) {
    QJsonArray jsonArray;
    quint32 length = qjsValue.property("length").toUInt();
    for (quint32 i = 0; i < length; ++i) {
      jsonArray.append(qjsValueToQJsonValue(qjsValue.property(i)));
    }
    return QJsonValue(jsonArray);
  } else if (qjsValue.isObject()) {
    QJsonObject jsonObject;
    QJSValueIterator it(qjsValue);
    while (it.hasNext()) {
      it.next();
      jsonObject.insert(it.name(), qjsValueToQJsonValue(it.value()));
    }
    return QJsonValue(jsonObject);
  }
  // Handle other types as needed
  return QJsonValue(QJsonValue::Undefined);
}

QString getObjectClass(const QObject* target) {
  if (target == nullptr) {
    return "Unknown";
  }
  auto metaObject = target->metaObject();
  return metaObject->className();
}

// static
QJsonObject serialize(QQuickItem* item) {
  QJsonObject out;
  if (item == nullptr) {
    return out;
  }
  out["__class__"] = getObjectClass(item);

  // Todo: Check QObject subelements for the Layout Element
  auto metaObject = item->metaObject();
  int propertyCount = metaObject->propertyCount();
  out["__propertyCount__"] = propertyCount;
  for (int i = 0; i < metaObject->propertyCount(); i++) {
    auto property = metaObject->property(i);
    if (!property.isValid()) {
      continue;
    }
    QJsonObject prop;
    auto name = property.name();
    auto value = property.read(item);
    if (value.canConvert<QJsonValue>()) {
      out[name] = value.toJsonValue();
    } else if (value.canConvert<QString>()) {
      out[name] = value.toString();
    } else if (value.canConvert<QStringList>()) {
      auto list = value.toStringList();
      QJsonArray somelist;
      for (const QString& s : list) {
        somelist.append(s);
      }
      out[name] = somelist;
    } else {
      out[name] = value.typeName();
    }
  }

  QJsonArray subView;
  auto children = item->childItems();
  for (auto& c : children) {
    subView.append(serialize(c));
  }
  out["subItems"] = subView;
  return out;
}

QJsonObject getViewTree() {
  QJsonObject out;
  out["type"] = "qml_tree";

  QQmlApplicationEngine* engine = qobject_cast<QQmlApplicationEngine*>(
      QmlEngineHolder::instance()->engine());
  if (!engine) {
    return out;
  }

  QJsonArray viewRoots;
  for (auto& root : engine->rootObjects()) {
    QQuickWindow* window = qobject_cast<QQuickWindow*>(root);
    if (window == nullptr) {
      continue;
    }
    QQuickItem* content = window->contentItem();
    viewRoots.append(serialize(content));
  }
  out["tree"] = viewRoots;
  return out;
}


}  // namespace

struct InspectorSettingCommand {
  QString m_settingName;

  enum {
    Boolean,
    String,
  } m_type;

  std::function<void(const QByteArray&)> m_set;
  std::function<QJsonValue()> m_get;
};

static QList<InspectorTools::Command> s_commands{
    InspectorTools::Command{
        "help", "The help menu", 0,
        [](const QList<QByteArray>&) {
          QJsonObject obj;

          QString value;

          {
            QTextStream out(&value);
            for (const InspectorTools::Command& command : s_commands) {
              out << command.m_commandName << '\t'
                  << command.m_commandDescription << Qt::endl;
            }
          }

          obj["value"] = value;
          return obj;
        }},
    InspectorTools::Command{"flip_off_feature", "Flip Off a feature", 1,
                            [](const QList<QByteArray>& arguments) {
                              QString featureName = arguments[1];
                              const Feature* feature =
                                  Feature::getOrNull(featureName);
                              if (!feature) {
                                QJsonObject obj;
                                obj["error"] = "Feature does not exist";
                                return obj;
                              }

                              if (feature->isSupported()) {
                                FeatureModel::instance()->toggle(arguments[1]);
                              }
                              return QJsonObject();
                            }},

    InspectorTools::Command{"flip_on_feature", "Flip On a feature", 1,
                            [](const QList<QByteArray>& arguments) {
                              QString featureName = arguments[1];
                              const Feature* feature =
                                  Feature::getOrNull(featureName);
                              if (!feature) {
                                QJsonObject obj;
                                obj["error"] = "Feature does not exist";
                                return obj;
                              }

                              if (!feature->isSupported()) {
                                FeatureModel::instance()->toggle(arguments[1]);
                              }
                              return QJsonObject();
                            }},

    InspectorTools::Command{
        "view_tree", "Sends a view tree", 0,
        [](const QList<QByteArray>&) { return getViewTree(); }},
    InspectorTools::Command{
        "query", "Query the tree", 1,
        [](const QList<QByteArray>& arguments) {
          QJsonObject obj;
          QQmlApplicationEngine* engine = qobject_cast<QQmlApplicationEngine*>(
              QmlEngineHolder::instance()->engine());
          obj["value"] = !!InspectorTools::QQuickInspector::queryObject(
              arguments[1], engine);
          return obj;
        }},

    InspectorTools::Command{
        "query_property", "Retrieve a property value from an object", 2,
        [](const QList<QByteArray>& arguments) {
          QJsonObject obj;
          QQmlApplicationEngine* engine = qobject_cast<QQmlApplicationEngine*>(
              QmlEngineHolder::instance()->engine());
          QObject* item = InspectorTools::QQuickInspector::queryObject(
              arguments[1], engine);
          if (!item) {
            obj["error"] = "Object not found";
            return obj;
          }

          QVariant property = item->property(arguments[2]);
          if (!property.isValid()) {
            obj["error"] = "Property is invalid";
            return obj;
          }

          obj["value"] = property.toString();
          return obj;
        }},

    InspectorTools::Command{
        "set_query_property", "Set a property value to an object", 3,
        [](const QList<QByteArray>& arguments) {
          QJsonObject obj;
          QQmlApplicationEngine* engine = qobject_cast<QQmlApplicationEngine*>(
              QmlEngineHolder::instance()->engine());
          QObject* item = InspectorTools::QQuickInspector::queryObject(
              arguments[1], engine);
          if (!item) {
            obj["error"] = "Object not found";
            return obj;
          }

          const QMetaObject* metaObject = item->metaObject();
          int propertyId = metaObject->indexOfProperty(arguments[2]);
          if (propertyId < 0) {
            obj["error"] = "Invalid property";
            return obj;
          }

          QMetaProperty property = metaObject->property(propertyId);
          Q_ASSERT(property.isValid());

          QVariant value = QVariant::fromValue(arguments[3]);
          if (!value.canConvert(property.type())) {
            obj["error"] = "Property value is invalid";
            return obj;
          }

          property.write(item, value);
          return obj;
        }},

    InspectorTools::Command{
        "property", "Retrieve a property value from a Mozilla object", 3,
        [](const QList<QByteArray>& arguments) {
          QJsonObject obj;

          int id = qmlTypeId(qPrintable(arguments[1]), 1, 0,
                             qPrintable(arguments[2]));

          QQmlApplicationEngine* engine = qobject_cast<QQmlApplicationEngine*>(
              QmlEngineHolder::instance()->engine());
          QObject* item = engine->singletonInstance<QObject*>(id);
          if (!item) {
            obj["error"] = "Object not found";
            return obj;
          }

          QVariant property = item->property(arguments[3]);
          if (!property.isValid()) {
            obj["error"] = "Property is invalid";
            return obj;
          }

          obj["value"] = property.toString();
          return obj;
        }},

    InspectorTools::Command{
        "set_property", "Set a property value to a Mozilla object", 4,
        [](const QList<QByteArray>& arguments) {
          QJsonObject obj;

          int id = qmlTypeId(qPrintable(arguments[1]), 1, 0,
                             qPrintable(arguments[2]));

          QQmlApplicationEngine* engine = qobject_cast<QQmlApplicationEngine*>(
              QmlEngineHolder::instance()->engine());
          QObject* item = engine->singletonInstance<QObject*>(id);
          if (!item) {
            obj["error"] = "Object not found";
            return obj;
          }

          const QMetaObject* metaObject = item->metaObject();
          int propertyId = metaObject->indexOfProperty(arguments[3]);
          if (propertyId < 0) {
            obj["error"] = "Invalid property";
            return obj;
          }

          QMetaProperty property = metaObject->property(propertyId);
          Q_ASSERT(property.isValid());

          QVariant value = QVariant::fromValue(arguments[4]);
          if (!value.canConvert(property.type())) {
            obj["error"] = "Property value is invalid";
            return obj;
          }

          property.write(item, value);
          return obj;
        }},

    InspectorTools::Command{
        "click", "Click on an object", 1,
        [](const QList<QByteArray>& arguments) {
          QJsonObject obj;
          QQmlApplicationEngine* engine = qobject_cast<QQmlApplicationEngine*>(
              QmlEngineHolder::instance()->engine());
          QObject* qmlobj = InspectorTools::QQuickInspector::queryObject(
              arguments[1], engine);
          if (!qmlobj) {
            logger.error() << "Did not find object to click on";
            obj["error"] = "Object not found";
            return obj;
          }
          QQuickItem* item = qobject_cast<QQuickItem*>(qmlobj);
          if (!item) {
            logger.error() << "Object is not clickable";
            obj["error"] = "Object is not clickable";
            return obj;
          }

          // It seems that in QT/QML there is a race-condition bug
          // between the rendering thread and the main one when
          // simulating clicks using QTest. At this point, all the
          // properties are synchronized, the animation is
          // probably already completed (otherwise it's a bug in
          // the test!) but it could be that he following
          // `mouse`Click` is ignored.
          // The horrible/temporary solution is to wait a bit more
          // and to add a delay (VPN-3697)
          QTest::qWait(150);

          QPointF pointF = item->mapToScene(QPoint(0, 0));
          QPoint point = pointF.toPoint();

          QTest::mouseClick(item->window(), Qt::LeftButton, Qt::NoModifier,
                            point);

          return obj;
        }},

    InspectorTools::Command{
        "stealurls",
        "Do not open the URLs in browser and expose them via inspector", 0,
        [](const QList<QByteArray>&) {
          UrlOpener::instance()->setStealUrls();
          return QJsonObject();
        }},

    InspectorTools::Command{"lasturl", "Retrieve the last opened URL", 0,
                            [](const QList<QByteArray>&) {
                              QJsonObject obj;
                              obj["value"] = UrlOpener::instance()->lastUrl();
                              return obj;
                            }},

    InspectorTools::Command{
        "set_setting", "Set a setting (setting, value)", 2,
        [](const QList<QByteArray>& arguments) {
          QJsonObject obj;

          SettingsHolder* settingsHolder = SettingsHolder::instance();
          const QMetaObject* metaObject = settingsHolder->metaObject();
          int propertyId = metaObject->indexOfProperty(arguments[1]);

          if (propertyId < 0) {
            obj["error"] =
                QString("Property %1 is not exposed by SettingsHolder")
                    .arg(QString(arguments[1]));
            return obj;
          }

          QMetaProperty property = metaObject->property(propertyId);
          Q_ASSERT(property.isValid());

          QVariant value = QVariant::fromValue(arguments[2]);
          if (value.canConvert(property.type())) {
            property.write(settingsHolder, value);
            return obj;
          }

          obj["error"] = QString("Property %1 has a non-supported type: %2")
                             .arg(arguments[1], property.type());
          return obj;
        }},

    InspectorTools::Command{
        "setting", "Get a setting value", 1,
        [](const QList<QByteArray>& arguments) {
          QJsonObject obj;

          SettingsHolder* settingsHolder = SettingsHolder::instance();
          const QMetaObject* metaObject = settingsHolder->metaObject();
          int propertyId = metaObject->indexOfProperty(arguments[1]);

          if (propertyId < 0) {
            obj["error"] =
                QString("Property %1 is not exposed by SettingsHolder")
                    .arg(QString(arguments[1]));
            return obj;
          }

          QMetaProperty property = metaObject->property(propertyId);
          Q_ASSERT(property.isValid());

          QVariant prop_value = property.read(settingsHolder);
          if (prop_value.canConvert<QJsonValue>()) {
            obj["value"] = prop_value.toJsonValue();
            return obj;
          }

          if (prop_value.canConvert<QDateTime>()) {
            obj["value"] = prop_value.toDateTime().toString();
            return obj;
          }

          if (prop_value.canConvert<QByteArray>()) {
            obj["value"] = prop_value.toByteArray().data();
            return obj;
          }

          obj["error"] = QString("Property %1 has a non-supported type: %2")
                             .arg(arguments[1], property.type());
          return obj;
        }},

    InspectorTools::Command{
        "settings_filename", "Get the setting filename", 0,
        [](const QList<QByteArray>&) {
          QJsonObject obj;
          obj["value"] = SettingsHolder::instance()->settingsFileName();
          return obj;
        }},

    InspectorTools::Command{"languages", "Returns a list of languages", 0,
                            [](const QList<QByteArray>&) {
                              QJsonObject obj;

                              Localizer* localizer = Localizer::instance();
                              Q_ASSERT(localizer);

                              QJsonArray languages;
                              for (const QString& language :
                                   localizer->languages()) {
                                languages.append(language);
                              }

                              obj["value"] = languages;
                              return obj;
                            }},

    InspectorTools::Command{"translate", "Translate a string", 1,
                            [](const QList<QByteArray>& arguments) {
                              QJsonObject obj;
                              obj["value"] = qtTrId(arguments[1]);
                              return obj;
                            }},

    InspectorTools::Command{
        "screen_capture", "Take a screen capture", 0,
        [](const QList<QByteArray>&) {
          QJsonObject obj;

          QWindow* window = QmlEngineHolder::instance()->window();
          if (!window) {
            obj["error"] = "Unable to identify the window";
            return obj;
          }

          QScreen* screen = window->screen();
          if (!screen) {
            obj["error"] = "Unable to identify the screen";
            return obj;
          }

          QPixmap pixmap = screen->grabWindow(window->winId());
          if (pixmap.isNull()) {
            obj["error"] = "Unable to grab the window";
            return obj;
          }

          QByteArray data;
          {
            QBuffer buffer(&data);
            buffer.open(QIODevice::WriteOnly);
            if (!pixmap.save(&buffer, "PNG")) {
              obj["error"] = "Unable to save the pixmap";
              return obj;
            }
          }
          obj["type"] = "screen";
          obj["value"] = QString(data.toBase64(QByteArray::Base64Encoding));
          return obj;
        }},

    InspectorTools::Command{"is_feature_flipped_on",
                            "Check if a feature is flipped on", 1,
                            [](const QList<QByteArray>& arguments) {
                              QString featureName = arguments[1];
                              auto const settings = SettingsHolder::instance();
                              QStringList flags = settings->featuresFlippedOn();

                              QJsonObject obj;
                              obj["value"] = flags.contains(featureName);
                              return obj;
                            }},

    InspectorTools::Command{
        "is_feature_flipped_off", "Check if a feature is flipped off", 1,
        [](const QList<QByteArray>& arguments) {
          QString featureName = arguments[1];
          auto const settings = SettingsHolder::instance();
          QStringList flags = settings->featuresFlippedOff();

          QJsonObject obj;
          obj["value"] = flags.contains(featureName);
          return obj;
        }},

    InspectorTools::Command{"back_button_clicked",
                            "Simulate an android back-button clicked", 0,
                            [](const QList<QByteArray>&) {
                              Navigator::instance()->eventHandled();
                              return QJsonObject();
                            }},

    InspectorTools::Command{"copy_to_clipboard", "Copy text to clipboard", 1,
                            [](const QList<QByteArray>& arguments) {
                              QString copiedText = arguments[1];
                              Utils::storeInClipboard(copiedText);
                              return QJsonObject();
                            }},

    InspectorTools::Command{"set_installation_time",
                            "Set the installation time", 1,
                            [](const QList<QByteArray>& arguments) {
                              qint64 epoch = arguments[1].toLongLong();
                              SettingsHolder::instance()->setInstallationTime(
                                  QDateTime::fromSecsSinceEpoch(epoch));
                              return QJsonObject();
                            }},

    InspectorTools::Command{
        "force_rtl", "Force RTL layout", 0,
        [](const QList<QByteArray>&) {
          Localizer::instance()->forceRTL();
          emit SettingsHolder::instance()->languageCodeChanged();
          return QJsonObject();
        }},

    InspectorTools::Command{"set_version_override",
                            "Override the version string", 1,
                            [](const QList<QByteArray>& arguments) {
                              QString versionOverride = QString(arguments[1]);
                              Constants::setVersionOverride(versionOverride);
                              return QJsonObject();
                            }},

    InspectorTools::Command{
        "glean_test_get_value", "Get value of a Glean metric", 3,
        [](const QList<QByteArray>& arguments) {
          QString metricCategory = QString(arguments[1]);
          QString metricName = QString(arguments[2]);
          QString ping = QString(arguments[3]);

          // Hack: let's run the code on the QML side,
          // because I (Bea) could not figure out a nice way
          // to access C++ namespaces and namespace properties
          // with runtime strings.
          QJSEngine* engine = QmlEngineHolder::instance()->engine();
          QJSValue function = QmlEngineHolder::instance()->engine()->evaluate(
              QString("(Glean) => Glean.%1.%2.testGetValue(\"%3\")")
                  .arg(metricCategory)
                  .arg(metricName)
                  .arg(ping));

          QJSValue result;  // TODO: I broke it, now remove it.
          QJsonObject obj;
          if (result.isError()) {
            obj["error"] = result.toString();
          } else {
            obj["value"] = qjsValueToQJsonValue(result);
          }

          return obj;
        }},

    InspectorTools::Command{"glean_test_reset", "Resets Glean for testing", 0,
                            [](const QList<QByteArray>&) {
                              MZGlean::initialize();
                              return QJsonObject();
                            }}

};




// Those Commands have grown to become a lovely ball of spagetti.
// I'm not going to bother untangling this.
// Please do not consider adding new ones here,
// Add them wherever the Commands are actually used.
void InspectorLegacyCommands::registerCommands(Inspector* i) {
  for (auto const& command : s_commands) {
    i->registerCommand(command);
    }
  }

