#include "controller.h"
#include "controllerimpl.h"
#include "mozillavpn.h"
#include "server.h"
#include "timercontroller.h"

#ifdef __linux__
#include "platforms/linux/linuxcontroller.h"
#elif MACOS_INTEGRATION
#include "platforms/macos/macoscontroller.h"
#else
#include "platforms/dummy/dummycontroller.h"
#endif

#include <QDebug>
#include <QTimer>

Controller::Controller()
{
    m_impl.reset(new TimerController(
#ifdef __linux__
        new LinuxController()
#elif MACOS_INTEGRATION
        new MacOSController()
#else
        new DummyController()
#endif
            ));

    connect(m_impl.get(), &ControllerImpl::connected, this, &Controller::connected);
    connect(m_impl.get(), &ControllerImpl::disconnected, this, &Controller::disconnected);
    connect(m_impl.get(), &ControllerImpl::initialized, [this](bool status, State state, const QDateTime& connectionDate) {
        qDebug() << "Controller activated with status:" << status << "state:" << state << "connectionDate:" << connectionDate;

        Q_ASSERT(m_state == StateInitializing);

        //TODO: use status to inform when something is down.
        Q_UNUSED(status);

        if (processNextStep()) {
            return;
        }

        setState(state);

        // If we are connected already at startup time, we can trigger the connection sequence of tasks.
        if (state == StateOn) {
            m_connectionDate = connectionDate;
            connected();
        }
    });

    connect(&m_timer, &QTimer::timeout, this, &Controller::timeUpdated);
}

Controller::~Controller() = default;

void Controller::setVPN(MozillaVPN *vpn)
{
    Q_ASSERT(!m_vpn);
    m_vpn = vpn;
}

void Controller::initialize()
{
    qDebug() << "Initializing the controller";

    Q_ASSERT(m_vpn);
    Q_ASSERT(m_state == StateInitializing);

    const Device *device = m_vpn->deviceModel()->currentDevice();
    m_impl->initialize(device, m_vpn->keys());
}

void Controller::activate()
{
    qDebug() << "Activation" << m_state;

    if (m_state != StateOff && m_state != StateSwitching) {
        qDebug() << "Already disconnected";
        return;
    }

    QList<Server> servers = m_vpn->getServers();
    Q_ASSERT(!servers.isEmpty());

    m_currentServer = Server::weightChooser(servers);
    Q_ASSERT(m_currentServer.initialized());

    const Device *device = m_vpn->deviceModel()->currentDevice();

    if (m_state == StateOff) {
        setState(StateConnecting);
    }

    m_timer.stop();

    m_connectionDate = QDateTime::currentDateTime();

    m_impl->activate(m_currentServer, device, m_vpn->keys(), m_state == StateSwitching);
}

void Controller::deactivate()
{
    qDebug() << "Deactivation" << m_state;

    if (m_state != StateOn && m_state != StateSwitching) {
        qDebug() << "Already disconnected";
        return;
    }

    Q_ASSERT(m_state == StateOn || m_state == StateSwitching);

    if (m_state == StateOn) {
        setState(StateDisconnecting);
    }

    m_timer.stop();
    m_connectionHealth.stop();

    const Device *device = m_vpn->deviceModel()->currentDevice();

    Q_ASSERT(m_currentServer.initialized());
    m_impl->deactivate(m_currentServer, device, m_vpn->keys(), m_state == StateSwitching);
}

void Controller::connected()
{
    qDebug() << "Connected from state:" << m_state;

    // This is an unexpected connection. Let's use the Connecting state to animate the UI.
    if (m_state != StateConnecting && m_state != StateSwitching) {
        setState(StateConnecting);
        QTimer::singleShot(TIME_ACTIVATION, [this]() {
            if (m_state == StateConnecting) {
                connected();
            }
        });
        return;
    }

    setState(StateOn);
    emit timeChanged();

    if (m_nextStep != None) {
        disconnect();
        return;
    }

    if (m_timer.isActive()) {
        m_timer.stop();
    }
    m_timer.start(1000);

    m_connectionHealth.start(m_currentServer);
}

void Controller::disconnected() {
    qDebug() << "Disconnected from state:" << m_state;

    m_timer.stop();
    m_connectionHealth.stop();

    // This is an unexpected disconnection. Let's use the Disconnecting state to animate the UI.
    if (m_state != StateDisconnecting && m_state != StateSwitching) {
        setState(StateDisconnecting);
        QTimer::singleShot(TIME_DEACTIVATION, [this]() {
            if (m_state == StateDisconnecting) {
                disconnected();
            }
        });
        return;
    }

    NextStep nextStep = m_nextStep;
    m_nextStep = None;

    if (processNextStep()) {
        return;
    }

    if (nextStep == None && m_state == StateSwitching) {
        m_vpn->changeServer(m_switchingCountryCode, m_switchingCity);
        activate();
        return;
    }

    setState(StateOff);
}

void Controller::timeUpdated() {
    Q_ASSERT(m_state == StateOn);
    emit timeChanged();
}

void Controller::changeServer(const QString &countryCode, const QString &city)
{
    Q_ASSERT(m_state == StateOn || m_state == StateOff);

    if (m_vpn->currentServer()->countryCode() == countryCode
        && m_vpn->currentServer()->city() == city) {
        qDebug() << "No server change needed";
        return;
    }

    if (m_state == StateOff) {
        qDebug() << "Change server";
        m_vpn->changeServer(countryCode, city);
        return;
    }

    m_timer.stop();

    qDebug() << "Switching to a different server";

    m_currentCity = m_vpn->currentServer()->city();
    m_switchingCountryCode = countryCode;
    m_switchingCity = city;

    setState(StateSwitching);

    deactivate();
}

void Controller::quit()
{
    qDebug() << "Quitting";

    if (m_state == StateOff) {
        emit readyToQuit();
        return;
    }

    m_nextStep = Quit;

    if (m_state == StateOn) {
        deactivate();
        return;
    }
}

void Controller::updateRequired()
{
    qDebug() << "Update required";

    if (m_state == StateOff) {
        emit readyToUpdate();
        return;
    }

    m_nextStep = Update;

    if (m_state == StateOn) {
        deactivate();
        return;
    }
}

void Controller::logout()
{
    qDebug() << "Logout";

    m_vpn->logout();

    if (m_state == StateOff) {
        return;
    }

    m_nextStep = Disconnect;

    if (m_state == StateOn) {
        deactivate();
        return;
    }
}

void Controller::setDeviceLimit(bool deviceLimit)
{
    qDebug() << "Device limit mode:" << deviceLimit;

    if (!deviceLimit) {
        Q_ASSERT(m_state == StateDeviceLimit);
        setState(StateOff);
        return;
    }

    if (m_state == StateOff) {
        setState(StateDeviceLimit);
        return;
    }

    m_nextStep = DeviceLimit;

    if (m_state == StateOn) {
        deactivate();
        return;
    }
}

bool Controller::processNextStep()
{
    NextStep nextStep = m_nextStep;
    m_nextStep = None;

    if (nextStep == Quit) {
        emit readyToQuit();
        return true;
    }

    if (nextStep == Update) {
        emit readyToUpdate();
        return true;
    }

    if (nextStep == DeviceLimit) {
        setState(StateDeviceLimit);
        return true;
    }

    return false;
}

void Controller::setState(State state)
{
    m_state = state;
    emit stateChanged();
}

int Controller::time() const
{
    return m_connectionDate.msecsTo(QDateTime::currentDateTime()) / 1000;
}
