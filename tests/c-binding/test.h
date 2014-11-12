/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#define TEST_GAUGE_NAME "test.gauge"
#define TEST_GAUGE_MIN -10
#define TEST_GAUGE_MAX 10

#define TEST_COUNTER_NAME "test.counter"
#define TEST_COUNTER_INIT_VALUE 0
#define TEST_COUNTER_INCR_DELTA 3
#define TEST_COUNTER_INCR_COUNT 10
#define TEST_COUNTER_DECR_DELTA 3
#define TEST_COUNTER_DECR_COUNT 15

#define TEST_SCOPED_COUNTER_NAME "test.scoped.counter"
#define TEST_SCOPED_COUNTER_DELTA 123
#define TEST_SCOPED_COUNTER_COUNT 15

#define TEST_TIMER_NAME "test.timer"
#define TEST_TIMER_NANOSLEEP_TIME 10000
#define TEST_TIMER_NANOSLEEP_COUNT 10

#define TEST_SCOPED_TIMER_NAME "test.scoped.timer"
#define TEST_SCOPED_TIMER_NANOSLEEP_TIME 20000
#define TEST_SCOPED_TIMER_NANOSLEEP_COUNT 5

#define TEST_BOOL_ATTR_NAME "test.bool.attr"
#define TEST_BOOL_ATTR_VALUE 1

#define TEST_DOUBLE_ATTR_NAME "test.double.attr"
#define TEST_DOUBLE_ATTR_VALUE 12345678.9

#define TEST_INT_ATTR_NAME "test.int.attr"
#define TEST_INT_ATTR_VALUE -123456

#define TEST_UINT_ATTR_NAME "test.uint.attr"
#define TEST_UINT_ATTR_VALUE 123456789

#define TEST_INT64_ATTR_NAME "test.int64.attr"
#define TEST_INT64_ATTR_VALUE -1234567890

#define TEST_UINT64_ATTR_NAME "test.uint64.attr"
#define TEST_UINT64_ATTR_VALUE 1234567890

#define TEST_STRING_ATTR_NAME "test.string.attr"
#define TEST_STRING_ATTR_VALUE "test.string"

#include <handystats/common.h>

HANDYSTATS_EXTERN_C void run_tests();
