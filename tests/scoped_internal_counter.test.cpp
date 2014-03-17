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
#include "internal_metrics/internal_counter_impl.hpp"


namespace handystats { namespace internal {

extern std::map<std::string, internal_metric> internal_metrics;

}} // namespace handystats::internal


class HandyScopedCounterTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(HandyScopedCounterTest, TestSingleScope) {
	const int COUNT = 10;

	for (int step = 0; step < COUNT; ++step) {
		HANDY_COUNTER_SCOPE("test.counter", 1);
	}

	while (!handystats::message_queue::empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	auto agg_stats =
		boost::get<handystats::internal::internal_counter*>(handystats::internal::internal_metrics["test.counter"])
		->base_counter
		->stats
		.values
		.stats
		.values;

	ASSERT_EQ(boost::accumulators::count(agg_stats), 2 * COUNT + 1);
	ASSERT_EQ(boost::accumulators::min(agg_stats), 0);
	ASSERT_EQ(boost::accumulators::max(agg_stats), 1);

	std::cout << *HANDY_JSON_DUMP() << std::endl;
}

TEST_F(HandyScopedCounterTest, TestDoubleNestedScope) {
	const int COUNT = 10;

	for (int step = 0; step < COUNT; ++step) {
		HANDY_COUNTER_SCOPE("test.counter", 1);
		{
			HANDY_COUNTER_SCOPE("test.counter", -1);
		}
	}

	while (!handystats::message_queue::empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(100));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	auto agg_stats =
		boost::get<handystats::internal::internal_counter*>(handystats::internal::internal_metrics["test.counter"])
		->base_counter
		->stats
		.values
		.stats
		.values;

	ASSERT_EQ(boost::accumulators::count(agg_stats), 4 * COUNT + 1);
	ASSERT_EQ(boost::accumulators::min(agg_stats), 0);
	ASSERT_EQ(boost::accumulators::max(agg_stats), 1);

	std::cout << *HANDY_JSON_DUMP() << std::endl;
}
