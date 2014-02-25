#include <vector>
#include <utility>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <queue>

#include <gtest/gtest.h>

#include <handystats/operation.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/json_dump.hpp>

#include <handystats/internal_metrics_impl.hpp>
#include <handystats/message_queue_impl.hpp>

class HandyTimerTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(HandyTimerTest, CommonTestSingleInstanceTimer) {
	std::cout << "Cycles per nanosec: " << handystats::chrono::cycles_per_nanosec << std::endl;

	const int COUNT = 5;
	auto sleep_time = std::chrono::duration_cast<handystats::metrics::timer::time_duration>(std::chrono::milliseconds(10));

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_START("sleep.time");
		std::this_thread::sleep_for(sleep_time);
		HANDY_TIMER_STOP("sleep.time");
	}

	while (!handystats::message_queue::event_message_queue->empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	ASSERT_TRUE(
			boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["sleep.time"])
			->instances.empty()
			);

	auto agg_stats =
		boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["sleep.time"])
		->aggregator
		.get_stats()
		.values;

	ASSERT_EQ(boost::accumulators::count(agg_stats), COUNT);
	ASSERT_GE(boost::accumulators::min(agg_stats), sleep_time.count());

	std::cout << *HANDY_JSON_DUMP() << std::endl;
}

TEST_F(HandyTimerTest, CommonTestMultiInstanceTimer) {
	const int COUNT = 10;
	auto sleep_time = std::chrono::duration_cast<handystats::metrics::timer::time_duration>(std::chrono::milliseconds(1));

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_START("sleep.time", step);
		std::this_thread::sleep_for(sleep_time);
		HANDY_TIMER_STOP("sleep.time", step);
	}

	while (!handystats::message_queue::event_message_queue->empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	ASSERT_TRUE(
			boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["sleep.time"])
			->instances.empty()
			);

	auto agg_stats =
		boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["sleep.time"])
		->aggregator
		.get_stats()
		.values;

	ASSERT_EQ(boost::accumulators::count(agg_stats), COUNT);
	ASSERT_GE(boost::accumulators::min(agg_stats), sleep_time.count());

	std::cout << *HANDY_JSON_DUMP() << std::endl;
}

TEST_F(HandyTimerTest, TestConcurrentlyMultiInstanceTimer) {
	const int COUNT = 10;
	auto sleep_time = std::chrono::duration_cast<handystats::metrics::timer::time_duration>(std::chrono::milliseconds(1));

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_START("sleep.time", step);
	}

	std::this_thread::sleep_for(sleep_time);

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_STOP("sleep.time", step);
	}

	while (!handystats::message_queue::event_message_queue->empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	ASSERT_TRUE(
			boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["sleep.time"])
			->instances.empty()
			);

	auto agg_stats =
		boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["sleep.time"])
		->aggregator
		.get_stats()
		.values;

	ASSERT_EQ(boost::accumulators::count(agg_stats), COUNT);
	ASSERT_GE(boost::accumulators::min(agg_stats), sleep_time.count());

	std::cout << *HANDY_JSON_DUMP() << std::endl;
}
