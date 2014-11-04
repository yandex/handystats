#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include <handystats/core.hpp>
#include <handystats/metrics_dump.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/module.h>

#include "message_queue_helper.hpp"

#define _HAVE_HANDY_MODULE_TEST 0

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

class HandyDisabledModuleTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_CONFIG_JSON(
				"{\
					\"dump-interval\": 1\
				}"
			);

		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(HandyDisabledModuleTest, CheckGauge) {
	const size_t COUNT = 10;
	const char* gauge_name = "test.gauge";

	for (size_t i = 0; i < COUNT; ++i) {
		TEST_GAUGE_SET(gauge_name, i);
	}

	handystats::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find(gauge_name) == metrics_dump->first.end());
}

TEST_F(HandyDisabledModuleTest, CheckCounter) {
	const size_t COUNT = 10;
	const int64_t DELTA = 10;
	const char* counter_name = "test.counter";

	for (size_t i = 0; i < COUNT; ++i) {
		TEST_COUNTER_SCOPE(counter_name, DELTA);
		TEST_COUNTER_CHANGE(counter_name, DELTA);
	}

	handystats::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find(counter_name) == metrics_dump->first.end());
}

TEST_F(HandyDisabledModuleTest, CheckTimer) {
	const size_t COUNT = 10;
	const std::chrono::microseconds sleep_time(1);
	const char* timer_name = "test.timer";

	TEST_TIMER_START(timer_name);
	for (size_t i = 0; i < COUNT; ++i) {
		TEST_TIMER_SCOPE(timer_name);
		std::this_thread::sleep_for(sleep_time);
	}
	TEST_TIMER_STOP(timer_name);

	handystats::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find(timer_name) == metrics_dump->first.end());
}
