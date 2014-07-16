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
#include <handystats/operation.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/metrics_dump.hpp>
#include <handystats/configuration.hpp>

#include "message_queue_helper.hpp"

class MetricsDumpTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_CONFIGURATION_JSON(
				"{\
					\"handystats\": {\
						\"metrics-dump\": {\
							\"interval\": 10\
						}\
					}\
				}"
			);

		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(MetricsDumpTest, SampleCounter) {
	const size_t INCR_COUNT = 100;
	const size_t INCR_VALUE = 10;

	for (size_t i = 0; i < INCR_COUNT; ++i) {
		HANDY_COUNTER_INCREMENT("counter", INCR_VALUE);
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("counter") != metrics_dump->end());

	auto& counter = boost::get<handystats::metrics::counter>(metrics_dump->at("counter"));
	ASSERT_EQ(counter.value, INCR_COUNT * INCR_VALUE);
	ASSERT_EQ(counter.incr_deltas.count(), INCR_COUNT);
	ASSERT_EQ(counter.incr_deltas.sum(), INCR_COUNT * INCR_VALUE);
	ASSERT_EQ(counter.incr_deltas.mean(), INCR_VALUE);
}

TEST_F(MetricsDumpTest, SampleTimer) {
	const std::chrono::milliseconds sleep_interval(10);
	const size_t TIMER_INSTANCES = 10;

	for (size_t i = 0; i < TIMER_INSTANCES; ++i) {
		HANDY_TIMER_START("timer", i);
	}

	std::this_thread::sleep_for(sleep_interval);

	for (size_t i = 0; i < TIMER_INSTANCES; ++i) {
		HANDY_TIMER_STOP("timer", i);
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("timer") != metrics_dump->end());

	auto& timer = boost::get<handystats::metrics::timer>(metrics_dump->at("timer"));
	ASSERT_EQ(timer.values.count(), TIMER_INSTANCES);
	ASSERT_TRUE(timer.values.min() >= handystats::chrono::duration_cast<handystats::chrono::time_duration>(sleep_interval).count());
}

TEST_F(MetricsDumpTest, SampleGauge) {
	const size_t MIN_VALUE = 0;
	const size_t MAX_VALUE = 100;

	for (size_t value = MIN_VALUE; value <= MAX_VALUE; ++value) {
		HANDY_GAUGE_SET("gauge", value);
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("gauge") != metrics_dump->end());

	auto& gauge = boost::get<handystats::metrics::gauge>(metrics_dump->at("gauge"));
	ASSERT_EQ(gauge.values.count(), MAX_VALUE - MIN_VALUE + 1);
	ASSERT_EQ(gauge.values.min(), MIN_VALUE);
	ASSERT_EQ(gauge.values.max(), MAX_VALUE);
	ASSERT_EQ(gauge.values.mean(), (MAX_VALUE + MIN_VALUE) / 2.0);
}
