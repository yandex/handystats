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

#include <time.h>

#include <handystats/core.h>
#include <handystats/measuring_points.h>

#include <handystats/module.h>

#include "test.h"

#ifndef _HAVE_HANDY_MODULE_TEST
#define _HAVE_HANDY_MODULE_TEST 1
#endif
HANDY_MODULE(TEST)

#if _HAVE_HANDY_MODULE_TEST == 1
#define TEST_TIMER_SCOPE(...) HANDY_TIMER_SCOPE(__VA_ARGS__)
#define TEST_COUNTER_SCOPE(...) HANDY_COUNTER_SCOPE(__VA_ARGS__)
#else
#define TEST_TIMER_SCOPE(...)
#define TEST_COUNTER_SCOPE(...)
#endif

void run_test_gauge(void) {
	int gauge_value;
	for(gauge_value = TEST_GAUGE_MIN; gauge_value <= TEST_GAUGE_MAX; ++gauge_value) {
		TEST_GAUGE_SET(TEST_GAUGE_NAME, gauge_value);
	}
}

void run_test_counter(void) {
	TEST_COUNTER_INIT(TEST_COUNTER_NAME, TEST_COUNTER_INIT_VALUE);

	int incr_step;
	for (incr_step = 0; incr_step < TEST_COUNTER_INCR_COUNT; ++incr_step) {
		TEST_COUNTER_INCREMENT(TEST_COUNTER_NAME, TEST_COUNTER_INCR_DELTA);
	}

	int decr_step;
	for (decr_step = 0; decr_step < TEST_COUNTER_DECR_COUNT; ++decr_step) {
		TEST_COUNTER_DECREMENT(TEST_COUNTER_NAME, TEST_COUNTER_DECR_DELTA);
	}
}

void run_test_scoped_counter(void) {
	int cycle;
	for (cycle = 0; cycle < TEST_SCOPED_COUNTER_COUNT; ++cycle) {
		TEST_COUNTER_SCOPE(TEST_SCOPED_COUNTER_NAME, TEST_SCOPED_COUNTER_DELTA);
	}
}

void run_test_timer(void) {
	struct timespec rqtp;
	rqtp.tv_sec = 0;
	rqtp.tv_nsec = TEST_TIMER_NANOSLEEP_TIME;

	int cycle;

	for (cycle = 0; cycle < TEST_TIMER_NANOSLEEP_COUNT; ++cycle) {
		TEST_TIMER_START(TEST_TIMER_NAME, cycle);
	}

	nanosleep(&rqtp, NULL);

	for (cycle = 0; cycle < TEST_TIMER_NANOSLEEP_COUNT; ++cycle) {
		TEST_TIMER_STOP(TEST_TIMER_NAME, cycle);
	}
}

void run_test_scoped_timer(void) {
	struct timespec rqtp;
	rqtp.tv_sec = 0;
	rqtp.tv_nsec = TEST_SCOPED_TIMER_NANOSLEEP_TIME;

	int cycle;

	for (cycle = 0; cycle < TEST_SCOPED_TIMER_NANOSLEEP_COUNT; ++cycle) {
		TEST_TIMER_SCOPE(TEST_SCOPED_TIMER_NAME);
		nanosleep(&rqtp, NULL);
	}
}

void run_test_bool_attr(void) {
	TEST_ATTRIBUTE_SET_BOOL(TEST_BOOL_ATTR_NAME, TEST_BOOL_ATTR_VALUE);
}

void run_test_double_attr(void) {
	TEST_ATTRIBUTE_SET_DOUBLE(TEST_DOUBLE_ATTR_NAME, TEST_DOUBLE_ATTR_VALUE);
}

void run_test_int_attr(void) {
	TEST_ATTRIBUTE_SET_INT(TEST_INT_ATTR_NAME, TEST_INT_ATTR_VALUE);
}

void run_test_uint_attr(void) {
	TEST_ATTRIBUTE_SET_UINT(TEST_UINT_ATTR_NAME, TEST_UINT_ATTR_VALUE);
}

void run_test_int64_attr(void) {
	TEST_ATTRIBUTE_SET_INT64(TEST_INT64_ATTR_NAME, TEST_INT64_ATTR_VALUE);
}

void run_test_uint64_attr(void) {
	TEST_ATTRIBUTE_SET_UINT64(TEST_UINT64_ATTR_NAME, TEST_UINT64_ATTR_VALUE);
}

void run_test_string_attr(void) {
	TEST_ATTRIBUTE_SET_STRING(TEST_STRING_ATTR_NAME, TEST_STRING_ATTR_VALUE);
}

void run_test_attr(void) {
	run_test_bool_attr();
	run_test_double_attr();
	run_test_int_attr();
	run_test_uint_attr();
	run_test_int64_attr();
	run_test_uint64_attr();
	run_test_string_attr();
}

void run_tests() {
	HANDY_CONFIG_JSON("{\"dump-interval\": 20}");
	HANDY_INIT();

	run_test_gauge();
	run_test_counter();
	run_test_scoped_counter();
	run_test_timer();
	run_test_scoped_timer();
	run_test_attr();
}
