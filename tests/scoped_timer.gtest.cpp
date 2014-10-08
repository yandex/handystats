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

#if _HAVE_HANDY_MODULE_TEST == 1
#define TEST_TIMER_SCOPE(...) HANDY_TIMER_SCOPE(__VA_ARGS__)
#define TEST_COUNTER_SCOPE(...) HANDY_COUNTER_SCOPE(__VA_ARGS__)
#else
#define TEST_TIMER_SCOPE(...)
#define TEST_COUNTER_SCOPE(...)
#endif

class HandyScopedTimerTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_CONFIG_JSON(
				"{\
					\"dump-interval\": 10\
				}"
			);

		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(HandyScopedTimerTest, TestSingleInstanceScopedTimer) {
	const int COUNT = 5;
	auto sleep_time = std::chrono::milliseconds(10);

	for (int step = 0; step < COUNT; ++step) {
		TEST_TIMER_SCOPE("sleep.time");
		std::this_thread::sleep_for(sleep_time);
	}

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find("sleep.time") != metrics_dump->first.end());

//	ASSERT_TRUE(
//			boost::get<handystats::metrics::timer>(metrics_dump->first.at("sleep.time"))
//			.instances
//			.empty()
//		);

	const auto& agg_stats = metrics_dump->first.at("sleep.time");

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), COUNT);
	ASSERT_GE(
			agg_stats.get<handystats::statistics::tag::min>(),
			handystats::chrono::duration::convert_to(handystats::metrics::timer::value_unit,
				handystats::chrono::duration(sleep_time.count(), handystats::chrono::time_unit::MSEC)).count()
		);
}

TEST_F(HandyScopedTimerTest, TestMultipleNestedScopes) {
	auto sleep_time = std::chrono::milliseconds(1);

	{
		TEST_TIMER_SCOPE("sleep.time");
		std::this_thread::sleep_for(sleep_time);

		{
			TEST_TIMER_SCOPE("sleep.time");
			std::this_thread::sleep_for(sleep_time);

			{
				TEST_TIMER_SCOPE("sleep.time");
				std::this_thread::sleep_for(sleep_time);
			}
		}
	}

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find("sleep.time") != metrics_dump->first.end());

//	ASSERT_TRUE(
//			boost::get<handystats::metrics::timer>(metrics_dump->first.at("sleep.time"))
//			.instances
//			.empty()
//		);

	const auto& agg_stats = metrics_dump->first.at("sleep.time");

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), 3);
	ASSERT_GE(
			agg_stats.get<handystats::statistics::tag::min>(),
			handystats::chrono::duration::convert_to(handystats::metrics::timer::value_unit,
				handystats::chrono::duration(sleep_time.count(), handystats::chrono::time_unit::MSEC)).count()
		);
	ASSERT_GE(
			agg_stats.get<handystats::statistics::tag::max>(),
			handystats::chrono::duration::convert_to(handystats::metrics::timer::value_unit,
				handystats::chrono::duration(sleep_time.count(), handystats::chrono::time_unit::MSEC)).count() * 3
		);
}

TEST_F(HandyScopedTimerTest, TestSeveralScopedTimersInOneScope) {
	const int COUNT = 10;
	auto sleep_time = std::chrono::milliseconds(1);

	for (int step = 0; step < COUNT; ++step) {
		TEST_TIMER_SCOPE("double.sleep.time");
		std::this_thread::sleep_for(sleep_time);
		TEST_TIMER_SCOPE("sleep.time");
		std::this_thread::sleep_for(sleep_time);
	}

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find("sleep.time") != metrics_dump->first.end());
	ASSERT_TRUE(metrics_dump->first.find("double.sleep.time") != metrics_dump->first.end());

	const auto& agg_stats = metrics_dump->first.at("sleep.time");
	const auto& double_agg_stats = metrics_dump->first.at("double.sleep.time");

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), COUNT);
	ASSERT_GE(
			agg_stats.get<handystats::statistics::tag::min>(),
			handystats::chrono::duration::convert_to(handystats::metrics::timer::value_unit,
				handystats::chrono::duration(sleep_time.count(), handystats::chrono::time_unit::MSEC)).count()
		);

	ASSERT_EQ(double_agg_stats.get<handystats::statistics::tag::count>(), COUNT);
	ASSERT_GE(
			double_agg_stats.get<handystats::statistics::tag::min>(),
			handystats::chrono::duration::convert_to(handystats::metrics::timer::value_unit,
				handystats::chrono::duration(sleep_time.count(), handystats::chrono::time_unit::MSEC)).count() * 2
		);
}
