# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

SOURCES += \
        apps/vpn/addons/addon.cpp \
        apps/vpn/addons/addonapi.cpp \
        apps/vpn/addons/addonguide.cpp \
        apps/vpn/addons/addoni18n.cpp \
        apps/vpn/addons/addonmessage.cpp \
        apps/vpn/addons/addonproperty.cpp \
        apps/vpn/addons/addonpropertylist.cpp \
        apps/vpn/addons/addontutorial.cpp \
        apps/vpn/addons/conditionwatchers/addonconditionwatcher.cpp \
        apps/vpn/addons/conditionwatchers/addonconditionwatcherfeaturesenabled.cpp \
        apps/vpn/addons/conditionwatchers/addonconditionwatchergroup.cpp \
        apps/vpn/addons/conditionwatchers/addonconditionwatcherjavascript.cpp \
        apps/vpn/addons/conditionwatchers/addonconditionwatcherlocales.cpp \
        apps/vpn/addons/conditionwatchers/addonconditionwatchertime.cpp \
        apps/vpn/addons/conditionwatchers/addonconditionwatchertranslationthreshold.cpp \
        apps/vpn/addons/conditionwatchers/addonconditionwatchertriggertimesecs.cpp \
        apps/vpn/addons/manager/addondirectory.cpp \
        apps/vpn/addons/manager/addonindex.cpp \
        apps/vpn/addons/manager/addonmanager.cpp \
        apps/vpn/addons/state/addonsessionstate.cpp \
        apps/vpn/addons/state/addonstatebase.cpp \
        apps/vpn/appconstants.cpp \
        apps/vpn/apppermission.cpp \
        apps/vpn/captiveportal/captiveportal.cpp \
        apps/vpn/captiveportal/captiveportaldetection.cpp \
        apps/vpn/captiveportal/captiveportaldetectionimpl.cpp \
        apps/vpn/captiveportal/captiveportalmonitor.cpp \
        apps/vpn/captiveportal/captiveportalnotifier.cpp \
        apps/vpn/captiveportal/captiveportalrequest.cpp \
        apps/vpn/captiveportal/captiveportalrequesttask.cpp \
        apps/vpn/command.cpp \
        apps/vpn/commandlineparser.cpp \
        apps/vpn/commands/commandactivate.cpp \
        apps/vpn/commands/commanddeactivate.cpp \
        apps/vpn/commands/commanddevice.cpp \
        apps/vpn/commands/commandlogin.cpp \
        apps/vpn/commands/commandlogout.cpp \
        apps/vpn/commands/commandselect.cpp \
        apps/vpn/commands/commandservers.cpp \
        apps/vpn/commands/commandstatus.cpp \
        apps/vpn/commands/commandui.cpp \
        apps/vpn/composer/composer.cpp \
        apps/vpn/composer/composerblock.cpp \
        apps/vpn/composer/composerblockbutton.cpp \
        apps/vpn/composer/composerblocktext.cpp \
        apps/vpn/composer/composerblocktitle.cpp \
        apps/vpn/composer/composerblockorderedlist.cpp \
        apps/vpn/composer/composerblockunorderedlist.cpp \
        apps/vpn/connectionbenchmark/benchmarktask.cpp \
        apps/vpn/connectionbenchmark/benchmarktaskping.cpp \
        apps/vpn/connectionbenchmark/benchmarktasktransfer.cpp \
        apps/vpn/connectionbenchmark/connectionbenchmark.cpp \
        apps/vpn/connectionbenchmark/uploaddatagenerator.cpp \
        apps/vpn/connectionhealth.cpp \
        apps/vpn/controller.cpp \
        apps/vpn/dnshelper.cpp \
        apps/vpn/dnspingsender.cpp \
        apps/vpn/externalophandler.cpp \
        apps/vpn/frontend/navigator.cpp \
        apps/vpn/frontend/navigatorreloader.cpp \
        apps/vpn/keyregenerator.cpp \
        apps/vpn/imageproviderfactory.cpp \
        apps/vpn/inspector/inspectorhandler.cpp \
        apps/vpn/inspector/inspectoritempicker.cpp \
        apps/vpn/inspector/inspectorutils.cpp \
        apps/vpn/inspector/inspectorwebsocketconnection.cpp \
        apps/vpn/inspector/inspectorwebsocketserver.cpp \
        apps/vpn/ipaddresslookup.cpp \
        apps/vpn/logoutobserver.cpp \
        apps/vpn/main.cpp \
        apps/vpn/models/device.cpp \
        apps/vpn/models/devicemodel.cpp \
        apps/vpn/models/featuremodel.cpp \
        apps/vpn/models/feedbackcategorymodel.cpp \
        apps/vpn/models/keys.cpp \
        apps/vpn/models/location.cpp \
        apps/vpn/models/recentconnections.cpp \
        apps/vpn/models/server.cpp \
        apps/vpn/models/servercity.cpp \
        apps/vpn/models/servercountry.cpp \
        apps/vpn/models/servercountrymodel.cpp \
        apps/vpn/models/serverdata.cpp \
        apps/vpn/models/subscriptiondata.cpp \
        apps/vpn/models/supportcategorymodel.cpp \
        apps/vpn/models/user.cpp \
        apps/vpn/mozillavpn.cpp \
        apps/vpn/mozillavpn_p.cpp \
        apps/vpn/networkwatcher.cpp \
        apps/vpn/notificationhandler.cpp \
        apps/vpn/pinghelper.cpp \
        apps/vpn/pingsender.cpp \
        apps/vpn/pingsenderfactory.cpp \
        apps/vpn/platforms/dummy/dummyapplistprovider.cpp \
        apps/vpn/platforms/dummy/dummynetworkwatcher.cpp \
        apps/vpn/platforms/dummy/dummypingsender.cpp \
        apps/vpn/productshandler.cpp \
        apps/vpn/purchasehandler.cpp \
        apps/vpn/purchaseiaphandler.cpp \
        apps/vpn/purchasewebhandler.cpp \
        apps/vpn/profileflow.cpp \
        apps/vpn/releasemonitor.cpp \
        apps/vpn/serveri18n.cpp \
        apps/vpn/serverlatency.cpp \
        apps/vpn/settingswatcher.cpp \
        apps/vpn/statusicon.cpp \
        apps/vpn/tasks/account/taskaccount.cpp \
        apps/vpn/tasks/adddevice/taskadddevice.cpp \
        apps/vpn/tasks/addon/taskaddon.cpp \
        apps/vpn/tasks/addonindex/taskaddonindex.cpp \
        apps/vpn/tasks/captiveportallookup/taskcaptiveportallookup.cpp \
        apps/vpn/tasks/getfeaturelist/taskgetfeaturelist.cpp \
        apps/vpn/tasks/getlocation/taskgetlocation.cpp \
        apps/vpn/tasks/getsubscriptiondetails/taskgetsubscriptiondetails.cpp \
        apps/vpn/tasks/controlleraction/taskcontrolleraction.cpp \
        apps/vpn/tasks/createsupportticket/taskcreatesupportticket.cpp \
        apps/vpn/tasks/heartbeat/taskheartbeat.cpp \
        apps/vpn/tasks/ipfinder/taskipfinder.cpp \
        apps/vpn/tasks/products/taskproducts.cpp \
        apps/vpn/tasks/release/taskrelease.cpp \
        apps/vpn/tasks/removedevice/taskremovedevice.cpp \
        apps/vpn/tasks/sendfeedback/tasksendfeedback.cpp \
        apps/vpn/tasks/servers/taskservers.cpp \
        apps/vpn/telemetry.cpp \
        apps/vpn/tutorial/tutorial.cpp \
        apps/vpn/tutorial/tutorialstep.cpp \
        apps/vpn/tutorial/tutorialstepbefore.cpp \
        apps/vpn/tutorial/tutorialstepnext.cpp \
        apps/vpn/update/updater.cpp \
        apps/vpn/update/versionapi.cpp \
        apps/vpn/update/webupdater.cpp \
        apps/vpn/websocket/exponentialbackoffstrategy.cpp \
        apps/vpn/websocket/pushmessage.cpp \
        apps/vpn/websocket/websockethandler.cpp

HEADERS += \
        apps/vpn/addons/addon.h \
        apps/vpn/addons/addonapi.h \
        apps/vpn/addons/addonguide.h \
        apps/vpn/addons/addoni18n.h \
        apps/vpn/addons/addonmessage.h \
        apps/vpn/addons/addonproperty.h \
        apps/vpn/addons/addonpropertylist.h \
        apps/vpn/addons/addontutorial.h \
        apps/vpn/addons/conditionwatchers/addonconditionwatcher.h \
        apps/vpn/addons/conditionwatchers/addonconditionwatcherfeaturesenabled.h \
        apps/vpn/addons/conditionwatchers/addonconditionwatchergroup.h \
        apps/vpn/addons/conditionwatchers/addonconditionwatcherjavascript.h \
        apps/vpn/addons/conditionwatchers/addonconditionwatcherlocales.h \
        apps/vpn/addons/conditionwatchers/addonconditionwatchertime.h \
        apps/vpn/addons/conditionwatchers/addonconditionwatchertimeend.h \
        apps/vpn/addons/conditionwatchers/addonconditionwatchertimestart.h \
        apps/vpn/addons/conditionwatchers/addonconditionwatchertranslationthreshold.h \
        apps/vpn/addons/conditionwatchers/addonconditionwatchertriggertimesecs.h \
        apps/vpn/addons/manager/addondirectory.h \
        apps/vpn/addons/manager/addonindex.h \
        apps/vpn/addons/manager/addonmanager.h \
        apps/vpn/addons/state/addonsessionstate.h \
        apps/vpn/addons/state/addonstatebase.h \
        apps/vpn/addons/state/addonstate.h \
        apps/vpn/appconstants.h \
        apps/vpn/appimageprovider.h \
        apps/vpn/apppermission.h \
        apps/vpn/applistprovider.h \
        apps/vpn/captiveportal/captiveportal.h \
        apps/vpn/captiveportal/captiveportaldetection.h \
        apps/vpn/captiveportal/captiveportaldetectionimpl.h \
        apps/vpn/captiveportal/captiveportalmonitor.h \
        apps/vpn/captiveportal/captiveportalnotifier.h \
        apps/vpn/captiveportal/captiveportalrequest.h \
        apps/vpn/captiveportal/captiveportalrequesttask.h \
        apps/vpn/command.h \
        apps/vpn/commandlineparser.h \
        apps/vpn/commands/commandactivate.h \
        apps/vpn/commands/commanddeactivate.h \
        apps/vpn/commands/commanddevice.h \
        apps/vpn/commands/commandlogin.h \
        apps/vpn/commands/commandlogout.h \
        apps/vpn/commands/commandselect.h \
        apps/vpn/commands/commandservers.h \
        apps/vpn/commands/commandstatus.h \
        apps/vpn/commands/commandui.h \
        apps/vpn/composer/composer.h \
        apps/vpn/composer/composerblock.h \
        apps/vpn/composer/composerblockbutton.h \
        apps/vpn/composer/composerblocktext.h \
        apps/vpn/composer/composerblocktitle.h \
        apps/vpn/composer/composerblockorderedlist.h \
        apps/vpn/composer/composerblockunorderedlist.h \
        apps/vpn/connectionbenchmark/benchmarktask.h \
        apps/vpn/connectionbenchmark/benchmarktaskping.h \
        apps/vpn/connectionbenchmark/benchmarktasksentinel.h \
        apps/vpn/connectionbenchmark/benchmarktasktransfer.h \
        apps/vpn/connectionbenchmark/connectionbenchmark.h \
        apps/vpn/connectionbenchmark/uploaddatagenerator.h \
        apps/vpn/connectionhealth.h \
        apps/vpn/controller.h \
        apps/vpn/controllerimpl.h \
        apps/vpn/dnshelper.h \
        apps/vpn/dnspingsender.h \
        apps/vpn/externalophandler.h \
        apps/vpn/frontend/navigator.h \
        apps/vpn/frontend/navigatorreloader.h \
        apps/vpn/keyregenerator.h \
        apps/vpn/imageproviderfactory.h \
        apps/vpn/inspector/inspectorhandler.h \
        apps/vpn/inspector/inspectoritempicker.h \
        apps/vpn/inspector/inspectorutils.h \
        apps/vpn/inspector/inspectorwebsocketconnection.h \
        apps/vpn/inspector/inspectorwebsocketserver.h \
        apps/vpn/ipaddresslookup.h \
        apps/vpn/localizer.h \
        apps/vpn/logoutobserver.h \
        apps/vpn/models/device.h \
        apps/vpn/models/devicemodel.h \
        apps/vpn/models/featuremodel.h \
        apps/vpn/models/feedbackcategorymodel.h \
        apps/vpn/models/keys.h \
        apps/vpn/models/location.h \
        apps/vpn/models/recentconnections.h \
        apps/vpn/models/server.h \
        apps/vpn/models/servercity.h \
        apps/vpn/models/servercountry.h \
        apps/vpn/models/servercountrymodel.h \
        apps/vpn/models/serverdata.h \
        apps/vpn/models/subscriptiondata.h \
        apps/vpn/models/supportcategorymodel.h \
        apps/vpn/models/user.h \
        apps/vpn/mozillavpn.h \
        apps/vpn/mozillavpn_p.h \
        apps/vpn/networkwatcher.h \
        apps/vpn/networkwatcherimpl.h \
        apps/vpn/notificationhandler.h \
        apps/vpn/pinghelper.h \
        apps/vpn/pingsender.h \
        apps/vpn/pingsenderfactory.h \
        apps/vpn/platforms/dummy/dummyapplistprovider.h \
        apps/vpn/platforms/dummy/dummynetworkwatcher.h \
        apps/vpn/platforms/dummy/dummypingsender.h \
        apps/vpn/productshandler.h \
        apps/vpn/profileflow.h \
        apps/vpn/purchasehandler.h \
        apps/vpn/purchaseiaphandler.h \
        apps/vpn/purchasewebhandler.h \
        apps/vpn/releasemonitor.h \
        apps/vpn/serveri18n.h \
        apps/vpn/serverlatency.h \
        apps/vpn/settingswatcher.h \
        apps/vpn/statusicon.h \
        apps/vpn/tasks/account/taskaccount.h \
        apps/vpn/tasks/adddevice/taskadddevice.h \
        apps/vpn/tasks/addon/taskaddon.h \
        apps/vpn/tasks/addonindex/taskaddonindex.h \
        apps/vpn/tasks/captiveportallookup/taskcaptiveportallookup.h \
        apps/vpn/tasks/getfeaturelist/taskgetfeaturelist.h \
        apps/vpn/tasks/getlocation/taskgetlocation.h \
        apps/vpn/tasks/getsubscriptiondetails/taskgetsubscriptiondetails.h \
        apps/vpn/tasks/controlleraction/taskcontrolleraction.h \
        apps/vpn/tasks/createsupportticket/taskcreatesupportticket.h \
        apps/vpn/tasks/heartbeat/taskheartbeat.h \
        apps/vpn/tasks/ipfinder/taskipfinder.h \
        apps/vpn/tasks/products/taskproducts.h \
        apps/vpn/tasks/release/taskrelease.h \
        apps/vpn/tasks/removedevice/taskremovedevice.h \
        apps/vpn/tasks/sendfeedback/tasksendfeedback.h \
        apps/vpn/tasks/servers/taskservers.h \
        apps/vpn/telemetry.h \
        apps/vpn/tutorial/tutorial.h \
        apps/vpn/tutorial/tutorialstep.h \
        apps/vpn/tutorial/tutorialstepbefore.h \
        apps/vpn/tutorial/tutorialstepnext.h \
        apps/vpn/update/updater.h \
        apps/vpn/update/versionapi.h \
        apps/vpn/update/webupdater.h \
        apps/vpn/websocket/exponentialbackoffstrategy.h \
        apps/vpn/websocket/pushmessage.h \
        apps/vpn/websocket/websockethandler.h


RESOURCES += apps/vpn/ui/resources.qrc
RESOURCES += apps/vpn/ui/ui.qrc
RESOURCES += apps/vpn/resources/certs/certs.qrc
RESOURCES += apps/vpn/resources/public_keys/public_keys.qrc

CONFIG += qmltypes
QML_IMPORT_NAME = Mozilla.VPN.qmlcomponents
QML_IMPORT_MAJOR_VERSION = 1.0
