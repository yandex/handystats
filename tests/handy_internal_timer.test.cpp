#include <vector>
#include <utility>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <queue>

#include <gtest/gtest.h>

#include "handystats/handystats.hpp"

TEST(HandyTimerTest, CommonTestSingleInstanceTimer) {
	HANDY_ENABLE();

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
			boost::get<handystats::internal::internal_timer*>(handystats::internal::monitors["sleep.time"])
			->instances.empty()
			);

	auto agg_stats =
		boost::get<handystats::internal::internal_timer*>(handystats::internal::monitors["sleep.time"])
		->aggregator
		.get_stats()
		.values;

	ASSERT_EQ(boost::accumulators::count(agg_stats), COUNT);
	ASSERT_GE(boost::accumulators::min(agg_stats), sleep_time.count());
	ASSERT_NEAR(boost::accumulators::mean(agg_stats), sleep_time.count(), sleep_time.count() * 0.25);

	/*
	std::cout << "0.5: " << boost::accumulators::quantile(agg_stats, boost::accumulators::quantile_probability = 0.5) << std::endl;
	std::cout << "0.75: " << boost::accumulators::quantile(agg_stats, boost::accumulators::quantile_probability = 0.75) << std::endl;
	std::cout << "0.95: " << boost::accumulators::quantile(agg_stats, boost::accumulators::quantile_probability = 0.95) << std::endl;
	*/

	HANDY_DISABLE();
}

TEST(HandyTimerTest, CommonTestMultiInstanceTimer) {
	HANDY_ENABLE();

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
			boost::get<handystats::internal::internal_timer*>(handystats::internal::monitors["sleep.time"])
			->instances.empty()
			);

	auto agg_stats =
		boost::get<handystats::internal::internal_timer*>(handystats::internal::monitors["sleep.time"])
		->aggregator
		.get_stats()
		.values;

	ASSERT_EQ(boost::accumulators::count(agg_stats), COUNT);
	ASSERT_GE(boost::accumulators::min(agg_stats), sleep_time.count());
	ASSERT_NEAR(boost::accumulators::mean(agg_stats), sleep_time.count(), sleep_time.count() * 0.25);

	/*
	std::cout << "0.5: " << boost::accumulators::quantile(agg_stats, boost::accumulators::quantile_probability = 0.5) << std::endl;
	std::cout << "0.75: " << boost::accumulators::quantile(agg_stats, boost::accumulators::quantile_probability = 0.75) << std::endl;
	std::cout << "0.95: " << boost::accumulators::quantile(agg_stats, boost::accumulators::quantile_probability = 0.95) << std::endl;
	*/

	HANDY_DISABLE();
}

TEST(HandyTimerTest, TestConcurrentlyMultiInstanceTimer) {
	HANDY_ENABLE();

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
			boost::get<handystats::internal::internal_timer*>(handystats::internal::monitors["sleep.time"])
			->instances.empty()
			);

	auto agg_stats =
		boost::get<handystats::internal::internal_timer*>(handystats::internal::monitors["sleep.time"])
		->aggregator
		.get_stats()
		.values;

	ASSERT_EQ(boost::accumulators::count(agg_stats), COUNT);
	ASSERT_GE(boost::accumulators::min(agg_stats), sleep_time.count());
	ASSERT_NEAR(boost::accumulators::mean(agg_stats), sleep_time.count(), sleep_time.count() * 0.5);

/*
	std::cout << "0.5: " << boost::accumulators::quantile(agg_stats, boost::accumulators::quantile_probability = 0.5) << std::endl;
	std::cout << "0.75: " << boost::accumulators::quantile(agg_stats, boost::accumulators::quantile_probability = 0.75) << std::endl;
	std::cout << "0.95: " << boost::accumulators::quantile(agg_stats, boost::accumulators::quantile_probability = 0.95) << std::endl;
*/

	HANDY_DISABLE();
}
