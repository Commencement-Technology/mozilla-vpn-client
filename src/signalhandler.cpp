/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "signalhandler.h"
#include "logger.h"
#include "signal.h"

#include <unistd.h>

namespace {

Logger logger(LOG_MAIN, "SignalHandler");

int s_signalpipe = -1;

}  // namespace

SignalHandler::SignalHandler() {
  Q_ASSERT(s_signalpipe < 0);

  int quitSignals[] = {SIGQUIT, SIGINT, SIGTERM, SIGHUP};

  sigset_t mask;
  sigemptyset(&mask);
  for (auto sig : quitSignals) {
    sigaddset(&mask, sig);
  }

  if (pipe(m_pipefds) != 0) {
    logger.log() << "Unable to create signal wakeup pipe";
    return;
  }
  s_signalpipe = m_pipefds[1];
  m_notifier = new QSocketNotifier(m_pipefds[0], QSocketNotifier::Read, this);
  connect(m_notifier,
          SIGNAL(activated(QSocketDescriptor, QSocketNotifier::Type)),
          SLOT(pipeReadReady()));

  struct sigaction sa;
  sa.sa_handler = SignalHandler::saHandler;
  sa.sa_mask = mask;
  sa.sa_flags = 0;

  for (auto sig : quitSignals) {
    sigaction(sig, &sa, nullptr);
  }
}

SignalHandler::~SignalHandler() {
  s_signalpipe = -1;
  if (m_pipefds[0] >= 0) {
    close(m_pipefds[0]);
  }
  if (m_pipefds[1] >= 1) {
    close(m_pipefds[1]);
  }
}

void SignalHandler::pipeReadReady() {
  int signal;
  if (read(m_pipefds[0], &signal, sizeof(signal)) == sizeof(signal)) {
    logger.log() << "Signal" << signal;
    emit quitRequested();
  }
}

void SignalHandler::saHandler(int signal) {
  if (s_signalpipe >= 0) {
    write(s_signalpipe, &signal, sizeof(signal));
  }
}
