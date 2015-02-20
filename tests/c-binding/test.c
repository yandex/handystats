#include <time.h>

#include <handystats/core.h>
#include <handystats/measuring_points.h>

#include "test.h"

void run_test_gauge(void) {
	int gauge_value;
	for(gauge_value = TEST_GAUGE_MIN; gauge_value <= TEST_GAUGE_MAX; ++gauge_value) {
		HANDY_GAUGE_SET(("%s." TEST_GAUGE_NAME, "test"), gauge_value);
	}
}

void run_test_counter(void) {
	HANDY_COUNTER_INIT(("%s." TEST_COUNTER_NAME, "test"), TEST_COUNTER_INIT_VALUE);

	int incr_step;
	for (incr_step = 0; incr_step < TEST_COUNTER_INCR_COUNT; ++incr_step) {
		HANDY_COUNTER_INCREMENT(("%s." TEST_COUNTER_NAME, "test"), TEST_COUNTER_INCR_DELTA);
	}

	int decr_step;
	for (decr_step = 0; decr_step < TEST_COUNTER_DECR_COUNT; ++decr_step) {
		HANDY_COUNTER_DECREMENT(("%s." TEST_COUNTER_NAME, "test"), TEST_COUNTER_DECR_DELTA);
	}
}

void run_test_scoped_counter(void) {
	int cycle;
	for (cycle = 0; cycle < TEST_SCOPED_COUNTER_COUNT; ++cycle) {
		HANDY_COUNTER_SCOPE(("%s." TEST_SCOPED_COUNTER_NAME, "test"), TEST_SCOPED_COUNTER_DELTA);
	}
}

void run_test_timer(void) {
	struct timespec rqtp;
	rqtp.tv_sec = 0;
	rqtp.tv_nsec = TEST_TIMER_NANOSLEEP_TIME;

	int cycle;

	for (cycle = 0; cycle < TEST_TIMER_NANOSLEEP_COUNT; ++cycle) {
		HANDY_TIMER_START(("%s." TEST_TIMER_NAME, "test"), cycle);
	}

	nanosleep(&rqtp, NULL);

	for (cycle = 0; cycle < TEST_TIMER_NANOSLEEP_COUNT; ++cycle) {
		HANDY_TIMER_STOP(("%s." TEST_TIMER_NAME, "test"), cycle);
	}
}

void run_test_scoped_timer(void) {
	struct timespec rqtp;
	rqtp.tv_sec = 0;
	rqtp.tv_nsec = TEST_SCOPED_TIMER_NANOSLEEP_TIME;

	int cycle;

	for (cycle = 0; cycle < TEST_SCOPED_TIMER_NANOSLEEP_COUNT; ++cycle) {
		HANDY_TIMER_SCOPE(("%s." TEST_SCOPED_TIMER_NAME, "test"));
		nanosleep(&rqtp, NULL);
	}
}

void run_test_bool_attr(void) {
	HANDY_ATTRIBUTE_SET_BOOL(("%s." TEST_BOOL_ATTR_NAME, "test"), TEST_BOOL_ATTR_VALUE);
}

void run_test_double_attr(void) {
	HANDY_ATTRIBUTE_SET_DOUBLE(("%s." TEST_DOUBLE_ATTR_NAME, "test"), TEST_DOUBLE_ATTR_VALUE);
}

void run_test_int_attr(void) {
	HANDY_ATTRIBUTE_SET_INT(("%s." TEST_INT_ATTR_NAME, "test"), TEST_INT_ATTR_VALUE);
}

void run_test_uint_attr(void) {
	HANDY_ATTRIBUTE_SET_UINT(("%s." TEST_UINT_ATTR_NAME, "test"), TEST_UINT_ATTR_VALUE);
}

void run_test_int64_attr(void) {
	HANDY_ATTRIBUTE_SET_INT64(("%s." TEST_INT64_ATTR_NAME, "test"), TEST_INT64_ATTR_VALUE);
}

void run_test_uint64_attr(void) {
	HANDY_ATTRIBUTE_SET_UINT64(("%s." TEST_UINT64_ATTR_NAME, "test"), TEST_UINT64_ATTR_VALUE);
}

void run_test_string_attr(void) {
	HANDY_ATTRIBUTE_SET_STRING(("%s." TEST_STRING_ATTR_NAME, "test"), TEST_STRING_ATTR_VALUE);
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
