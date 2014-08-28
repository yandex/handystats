#include <vector>
#include <utility>
#include <cstdlib>
#include <thread>
#include <queue>
#include <string>
#include <map>
#include <memory>

#include <gtest/gtest.h>

#include <handystats/chrono.hpp>
#include <handystats/core.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/metrics_dump.hpp>


#include "message_queue_helper.hpp"


class HandyTimerTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_CONFIG_JSON(
				"{\
					\"timer\": {\
						\"idle-timeout\": 100\
					},\
					\"metrics-dump\": {\
						\"interval\": 10\
					}\
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
		HANDY_TIMER_START("sleep.time");
		std::this_thread::sleep_for(sleep_time);
		HANDY_TIMER_STOP("sleep.time");
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(
			boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
			.instances
			.empty()
		);

	auto agg_stats =
		boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
		.values;

	ASSERT_EQ(agg_stats.count(), COUNT);
	ASSERT_GE(agg_stats.min(), handystats::chrono::duration_cast<handystats::chrono::time_duration>(sleep_time).count());
}

TEST_F(HandyTimerTest, CommonTestMultiInstanceTimer) {
	const int COUNT = 10;
	auto sleep_time = std::chrono::milliseconds(1);

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_START("sleep.time", step);
		std::this_thread::sleep_for(sleep_time);
		HANDY_TIMER_STOP("sleep.time", step);
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(
			boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
			.instances
			.empty()
		);

	auto agg_stats =
		boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
		.values;

	ASSERT_EQ(agg_stats.count(), COUNT);
	ASSERT_GE(agg_stats.min(), handystats::chrono::duration_cast<handystats::chrono::time_duration>(sleep_time).count());
}

TEST_F(HandyTimerTest, TestConcurrentlyMultiInstanceTimer) {
	const int COUNT = 10;
	auto sleep_time = std::chrono::milliseconds(1);

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_START("sleep.time", step);
	}

	std::this_thread::sleep_for(sleep_time);

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_STOP("sleep.time", step);
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(
			boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
			.instances
			.empty()
		);

	auto agg_stats =
		boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
		.values;

	ASSERT_EQ(agg_stats.count(), COUNT);
	ASSERT_GE(agg_stats.min(), handystats::chrono::duration_cast<handystats::chrono::time_duration>(sleep_time).count());
}
