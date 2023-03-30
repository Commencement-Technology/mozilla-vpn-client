/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "glean/uuid.h"

#include <QDebug>

#if not(defined(__wasm__) || defined(BUILD_QMAKE))
#  include "qtglean.h"
#endif

UuidMetric::UuidMetric(int id) : m_id(id) {}

void UuidMetric::set(const QString& uuid) const {
#if not(defined(__wasm__) || defined(BUILD_QMAKE))
  glean_uuid_set(m_id, uuid.toLocal8Bit());
#else
  Q_UNUSED(uuid);
#endif
}

QString UuidMetric::generateAndSet() const {
#if not(defined(__wasm__) || defined(BUILD_QMAKE))
  return glean_uuid_generate_and_set(m_id);
#else
  return QString();
#endif
}

int32_t UuidMetric::testGetNumRecordedErrors(ErrorType errorType) const {
#if not(defined(__wasm__) || defined(BUILD_QMAKE))
  return glean_uuid_test_get_num_recorded_errors(m_id, errorType);
#else
  Q_UNUSED(errorType);
  return 0;
#endif
}

QString UuidMetric::testGetValue(const QString& pingName) const {
#if not(defined(__wasm__) || defined(BUILD_QMAKE))
  return glean_uuid_test_get_value(m_id, pingName.toLocal8Bit());
#else
  Q_UNUSED(pingName);
  return "";
#endif
}
