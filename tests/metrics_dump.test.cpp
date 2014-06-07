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
#include <handystats/metrics_dump.hpp>
#include <handystats/configuration.hpp>

class MetricsDumpTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(MetricsDumpTest, SampleCounter) {
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"metrics-dump\": {\
						\"interval\": 10\
					}\
				}\
			}"
		);

	const size_t INCR_COUNT = 100;
	const size_t INCR_VALUE = 10;

	HANDY_INIT();

	for (size_t i = 0; i < INCR_COUNT; ++i) {
		HANDY_COUNTER_INCREMENT("counter", INCR_VALUE);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("counter") != metrics_dump->cend());

	auto& counter = boost::get<handystats::metrics::const_counter>(metrics_dump->find("counter")->second);
	ASSERT_EQ(counter.value, INCR_COUNT * INCR_VALUE);
	ASSERT_EQ(counter.incr_deltas.count(), INCR_COUNT);
	ASSERT_EQ(counter.incr_deltas.sum(), INCR_COUNT * INCR_VALUE);
	ASSERT_EQ(counter.incr_deltas.mean(), INCR_VALUE);
}

TEST_F(MetricsDumpTest, SampleTimer) {
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"metrics-dump\": {\
						\"interval\": 10\
					}\
				}\
			}"
		);

	const std::chrono::milliseconds sleep_interval(10);
	const size_t TIMER_INSTANCES = 10;

	HANDY_INIT();

	for (size_t i = 0; i < TIMER_INSTANCES; ++i) {
		HANDY_TIMER_START("timer", i);
	}

	std::this_thread::sleep_for(sleep_interval);

	for (size_t i = 0; i < TIMER_INSTANCES; ++i) {
		HANDY_TIMER_STOP("timer", i);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(30));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("timer") != metrics_dump->cend());

	auto& timer = boost::get<handystats::metrics::const_timer>(metrics_dump->find("timer")->second);
	ASSERT_EQ(timer.values.count(), TIMER_INSTANCES);
	ASSERT_TRUE(timer.values.min() >= std::chrono::duration_cast<handystats::chrono::default_duration>(sleep_interval).count());
}

TEST_F(MetricsDumpTest, SampleGauge) {
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"metrics-dump\": {\
						\"interval\": 10\
					}\
				}\
			}"
		);

	const size_t MIN_VALUE = 0;
	const size_t MAX_VALUE = 100;

	HANDY_INIT();

	for (size_t value = MIN_VALUE; value <= MAX_VALUE; ++value) {
		HANDY_GAUGE_SET("gauge", value);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(30));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("gauge") != metrics_dump->cend());

	auto& gauge = boost::get<handystats::metrics::const_gauge>(metrics_dump->find("gauge")->second);
	ASSERT_EQ(gauge.values.count(), MAX_VALUE - MIN_VALUE + 1);
	ASSERT_EQ(gauge.values.min(), MIN_VALUE);
	ASSERT_EQ(gauge.values.max(), MAX_VALUE);
	ASSERT_EQ(gauge.values.mean(), (MAX_VALUE + MIN_VALUE) / 2.0);
}
