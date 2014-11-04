#include <vector>
#include <utility>
#include <cstdlib>
#include <thread>
#include <queue>
#include <string>
#include <map>
#include <memory>
#include <chrono>

#include <gtest/gtest.h>

#include <handystats/chrono.hpp>
#include <handystats/core.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/metrics_dump.hpp>
#include <handystats/module.h>

#include "message_queue_helper.hpp"
#include "metrics_dump_helper.hpp"

#ifndef _HAVE_HANDY_MODULE_TEST
#define _HAVE_HANDY_MODULE_TEST 1
#endif
HANDY_MODULE(TEST)

class HandyTimerTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_CONFIG_JSON(
				"{\
					\"metrics\": {\
						\"timer\": {\
							\"idle-timeout\": 100\
						}\
					},\
					\"dump-interval\": 10\
				}"
			);

		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(HandyTimerTest, CommonTestSingleInstanceTimer) {
	const int COUNT = 5;
	auto sleep_time = std::chrono::milliseconds(10);

	for (int step = 0; step < COUNT; ++step) {
		TEST_TIMER_START("sleep.time");
		std::this_thread::sleep_for(sleep_time);
		TEST_TIMER_STOP("sleep.time");
	}

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	const auto& agg_stats = metrics_dump->first.at("sleep.time");

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), COUNT);
	ASSERT_GE(
			agg_stats.get<handystats::statistics::tag::min>(),
			handystats::chrono::milliseconds(sleep_time.count()).count(handystats::metrics::timer::value_unit)
		);
}

TEST_F(HandyTimerTest, CommonTestMultiInstanceTimer) {
	const int COUNT = 10;
	auto sleep_time = std::chrono::milliseconds(1);

	for (int step = 0; step < COUNT; ++step) {
		TEST_TIMER_START("sleep.time", step);
		std::this_thread::sleep_for(sleep_time);
		TEST_TIMER_STOP("sleep.time", step);
	}

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	const auto& agg_stats = metrics_dump->first.at("sleep.time");

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), COUNT);
	ASSERT_GE(
			agg_stats.get<handystats::statistics::tag::min>(),
			handystats::chrono::milliseconds(sleep_time.count()).count(handystats::metrics::timer::value_unit)
		);
}

TEST_F(HandyTimerTest, TestConcurrentlyMultiInstanceTimer) {
	const int COUNT = 10;
	auto sleep_time = std::chrono::milliseconds(1);

	for (int step = 0; step < COUNT; ++step) {
		TEST_TIMER_START("sleep.time", step);
	}

	std::this_thread::sleep_for(sleep_time);

	for (int step = 0; step < COUNT; ++step) {
		TEST_TIMER_STOP("sleep.time", step);
	}

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	const auto& agg_stats = metrics_dump->first.at("sleep.time");

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), COUNT);
	ASSERT_GE(
			agg_stats.get<handystats::statistics::tag::min>(),
			handystats::chrono::milliseconds(sleep_time.count()).count(handystats::metrics::timer::value_unit)
		);
}
