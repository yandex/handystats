#include <time.h>

#include <handystats/core.h>
#include <handystats/measuring_points.h>

#include "test.h"

void run_test_gauge(void) {
	int gauge_value;
	for(gauge_value = TEST_GAUGE_MIN; gauge_value <= TEST_GAUGE_MAX; ++gauge_value) {
		HANDY_GAUGE_SET(TEST_GAUGE_NAME, gauge_value);
	}
}

void run_test_counter(void) {
	HANDY_COUNTER_INIT(TEST_COUNTER_NAME, TEST_COUNTER_INIT_VALUE);

	int incr_step;
	for (incr_step = 0; incr_step < TEST_COUNTER_INCR_COUNT; ++incr_step) {
		HANDY_COUNTER_INCREMENT(TEST_COUNTER_NAME, TEST_COUNTER_INCR_DELTA);
	}

	int decr_step;
	for (decr_step = 0; decr_step < TEST_COUNTER_DECR_COUNT; ++decr_step) {
		HANDY_COUNTER_DECREMENT(TEST_COUNTER_NAME, TEST_COUNTER_DECR_DELTA);
	}
}

void run_test_scoped_counter(void) {
	int cycle;
	for (cycle = 0; cycle < TEST_SCOPED_COUNTER_COUNT; ++cycle) {
		HANDY_COUNTER_SCOPE(TEST_SCOPED_COUNTER_NAME, TEST_SCOPED_COUNTER_DELTA);
	}
}

void run_test_timer(void) {
	struct timespec rqtp;
	rqtp.tv_sec = 0;
	rqtp.tv_nsec = TEST_TIMER_NANOSLEEP_TIME;

	int cycle;

	for (cycle = 0; cycle < TEST_TIMER_NANOSLEEP_COUNT; ++cycle) {
		HANDY_TIMER_START(TEST_TIMER_NAME, cycle);
	}

	nanosleep(&rqtp, NULL);

	for (cycle = 0; cycle < TEST_TIMER_NANOSLEEP_COUNT; ++cycle) {
		HANDY_TIMER_STOP(TEST_TIMER_NAME, cycle);
	}
}

void run_test_scoped_timer(void) {
	struct timespec rqtp;
	rqtp.tv_sec = 0;
	rqtp.tv_nsec = TEST_SCOPED_TIMER_NANOSLEEP_TIME;

	int cycle;

	for (cycle = 0; cycle < TEST_SCOPED_TIMER_NANOSLEEP_COUNT; ++cycle) {
		HANDY_TIMER_SCOPE(TEST_SCOPED_TIMER_NAME);
		nanosleep(&rqtp, NULL);
	}
}

void run_test_bool_attr(void) {
	HANDY_ATTRIBUTE_SET_BOOL(TEST_BOOL_ATTR_NAME, TEST_BOOL_ATTR_VALUE);
}

void run_test_double_attr(void) {
	HANDY_ATTRIBUTE_SET_DOUBLE(TEST_DOUBLE_ATTR_NAME, TEST_DOUBLE_ATTR_VALUE);
}

void run_test_int_attr(void) {
	HANDY_ATTRIBUTE_SET_INT(TEST_INT_ATTR_NAME, TEST_INT_ATTR_VALUE);
}

void run_test_uint_attr(void) {
	HANDY_ATTRIBUTE_SET_UINT(TEST_UINT_ATTR_NAME, TEST_UINT_ATTR_VALUE);
}

void run_test_int64_attr(void) {
	HANDY_ATTRIBUTE_SET_INT64(TEST_INT64_ATTR_NAME, TEST_INT64_ATTR_VALUE);
}

void run_test_uint64_attr(void) {
	HANDY_ATTRIBUTE_SET_UINT64(TEST_UINT64_ATTR_NAME, TEST_UINT64_ATTR_VALUE);
}

void run_test_string_attr(void) {
	HANDY_ATTRIBUTE_SET_STRING(TEST_STRING_ATTR_NAME, TEST_STRING_ATTR_VALUE);
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

int main(int argc, char** argv) {
	HANDY_CONFIG_JSON("{\"metrics-dump\":{\"interval\": 20}}");
	HANDY_INIT();

	run_test_gauge();
	run_test_counter();
	run_test_scoped_counter();
	run_test_timer();
	run_test_scoped_timer();
	run_test_attr();

	int ret = check_tests(argc, argv);

	HANDY_FINALIZE();

	return ret;
}
