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

#include <handystats/core.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/metrics_dump.hpp>

#include "message_queue_helper.hpp"

class HandyScopedCounterTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_CONFIG_JSON(
				"{\
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

TEST_F(HandyScopedCounterTest, TestSingleScope) {
	const int COUNT = 10;

	for (int step = 0; step < COUNT; ++step) {
		HANDY_COUNTER_SCOPE("test.counter", 1);
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("test.counter") != metrics_dump->end());

	const auto& agg_stats =
		boost::get<handystats::metrics::counter>(metrics_dump->at("test.counter"))
		.values();

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), 2 * COUNT + 1);
	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::min>(), 0);
	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::max>(), 1);
}

TEST_F(HandyScopedCounterTest, TestDoubleNestedScope) {
	const int COUNT = 10;

	for (int step = 0; step < COUNT; ++step) {
		HANDY_COUNTER_SCOPE("test.counter", 1);
		{
			HANDY_COUNTER_SCOPE("test.counter", -1);
		}
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("test.counter") != metrics_dump->end());

	const auto& agg_stats =
		boost::get<handystats::metrics::counter>(metrics_dump->at("test.counter"))
		.values();

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), 4 * COUNT + 1);
	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::min>(), 0);
	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::max>(), 1);
}
