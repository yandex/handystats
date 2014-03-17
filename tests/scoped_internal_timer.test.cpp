#include <vector>
#include <utility>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <queue>
#include <string>
#include <map>
#include <memory>

#include <gtest/gtest.h>

#include <handystats/operation.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/json_dump.hpp>

#include "events/event_message_impl.hpp"
#include "message_queue_impl.hpp"
#include "internal_metrics_impl.hpp"
#include "internal_metrics/internal_timer_impl.hpp"


namespace handystats { namespace internal {

extern std::map<std::string, internal_metric> internal_metrics;

}} // namespace handystats::internal


class HandyScopedTimerTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(HandyScopedTimerTest, TestSingleInstanceScopedTimer) {
	std::cout << "Cycles per nanosec: " << handystats::chrono::cycles_per_nanosec << std::endl;

	const int COUNT = 5;
	auto sleep_time = std::chrono::milliseconds(10);

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_SCOPE("sleep.time");
		std::this_thread::sleep_for(sleep_time);
	}

	while (!handystats::message_queue::empty()) {
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
		.stats
		.values;

	ASSERT_EQ(boost::accumulators::count(agg_stats), COUNT);
	ASSERT_GE(boost::accumulators::min(agg_stats),
			std::chrono::duration_cast<handystats::chrono::default_duration>(sleep_time).count());

	std::cout << *HANDY_JSON_DUMP() << std::endl;
}

TEST_F(HandyScopedTimerTest, TestMultiInstanceScopedTimer) {
	const int COUNT = 10;
	auto sleep_time = std::chrono::milliseconds(1);

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_SCOPE("sleep.time", step);
		std::this_thread::sleep_for(sleep_time);
	}

	while (!handystats::message_queue::empty()) {
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
		.stats
		.values;

	ASSERT_EQ(boost::accumulators::count(agg_stats), COUNT);
	ASSERT_GE(boost::accumulators::min(agg_stats),
			std::chrono::duration_cast<handystats::chrono::default_duration>(sleep_time).count());

	std::cout << *HANDY_JSON_DUMP() << std::endl;
}

TEST_F(HandyScopedTimerTest, TestSeveralScopedTimersInOneScope) {
	const int COUNT = 10;
	auto sleep_time = std::chrono::milliseconds(1);

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_SCOPE("double.sleep.time", step);
		std::this_thread::sleep_for(sleep_time);
		HANDY_TIMER_SCOPE("sleep.time", step);
		std::this_thread::sleep_for(sleep_time);
	}

	while (!handystats::message_queue::empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	ASSERT_TRUE(
			boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["sleep.time"])
			->instances.empty()
			);

	ASSERT_TRUE(
			boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["double.sleep.time"])
			->instances.empty()
			);

	auto agg_stats =
		boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["sleep.time"])
		->aggregator
		.stats
		.values;

	ASSERT_EQ(boost::accumulators::count(agg_stats), COUNT);
	ASSERT_GE(boost::accumulators::min(agg_stats),
			std::chrono::duration_cast<handystats::chrono::default_duration>(sleep_time).count());

	auto double_agg_stats =
		boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["double.sleep.time"])
		->aggregator
		.stats
		.values;

	ASSERT_EQ(boost::accumulators::count(double_agg_stats), COUNT);
	ASSERT_GE(boost::accumulators::min(double_agg_stats),
			std::chrono::duration_cast<handystats::chrono::default_duration>(sleep_time).count() * 2);

	std::cout << *HANDY_JSON_DUMP() << std::endl;
}

