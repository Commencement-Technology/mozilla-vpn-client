/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "glean/quantity.h"

#include <QDebug>

#if not(defined(__wasm__) || defined(BUILD_QMAKE))
#  include "qtglean.h"
#endif

QuantityMetric::QuantityMetric(int id) : m_id(id) {}

void QuantityMetric::set(int value) const {
#if not(defined(__wasm__) || defined(BUILD_QMAKE))
  glean_quantity_set(m_id, value);
#else
  Q_UNUSED(value);
#endif
}

int32_t QuantityMetric::testGetNumRecordedErrors(ErrorType errorType) const {
#if not(defined(__wasm__) || defined(BUILD_QMAKE))
  return glean_quantity_test_get_num_recorded_errors(m_id, errorType);
#else
  Q_UNUSED(errorType);
  return 0;
#endif
}

int64_t QuantityMetric::testGetValue(const QString& pingName) const {
#if not(defined(__wasm__) || defined(BUILD_QMAKE))
  return glean_quantity_test_get_value(m_id, pingName.toLocal8Bit());
#else
  Q_UNUSED(pingName);
  return 0;
#endif
}
