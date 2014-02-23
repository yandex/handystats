#include <vector>
#include <utility>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <queue>

#include <gtest/gtest.h>

#include "handystats/handystats.hpp"

#include <handystats/json/internal_timer_json_writer.hpp>

class HandyTimerTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_ENABLE();
	}
	virtual void TearDown() {
		HANDY_DISABLE();
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

	auto timer = boost::get<handystats::internal::internal_timer*>(handystats::internal::monitors["sleep.time"]);
	std::cout << handystats::json::write_to_json_string(timer) << std::endl;

	std::cout << "message-queue-size: " << handystats::json::write_to_json_string(&handystats::event_message_queue_size) << std::endl;
	std::cout << "monitors-size: " << handystats::json::write_to_json_string(&handystats::monitors_size) << std::endl;
	std::cout << "event-processing-time: " << handystats::json::write_to_json_string(&handystats::message_processing_time) << std::endl;
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

	auto timer = boost::get<handystats::internal::internal_timer*>(handystats::internal::monitors["sleep.time"]);
	std::cout << handystats::json::write_to_json_string(timer) << std::endl;

	std::cout << "message-queue-size: " << handystats::json::write_to_json_string(&handystats::event_message_queue_size) << std::endl;
	std::cout << "monitors-size: " << handystats::json::write_to_json_string(&handystats::monitors_size) << std::endl;
	std::cout << "event-processing-time: " << handystats::json::write_to_json_string(&handystats::message_processing_time) << std::endl;
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

	auto timer = boost::get<handystats::internal::internal_timer*>(handystats::internal::monitors["sleep.time"]);
	std::cout << handystats::json::write_to_json_string(timer) << std::endl;

	std::cout << "message-queue-size: " << handystats::json::write_to_json_string(&handystats::event_message_queue_size) << std::endl;
	std::cout << "monitors-size: " << handystats::json::write_to_json_string(&handystats::monitors_size) << std::endl;
	std::cout << "event-processing-time: " << handystats::json::write_to_json_string(&handystats::message_processing_time) << std::endl;
}
