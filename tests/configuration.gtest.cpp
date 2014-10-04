#include <chrono>
#include <thread>
#include <map>
#include <string>

#include <gtest/gtest.h>

#include <handystats/rapidjson/document.h>

#include <handystats/core.hpp>
#include <handystats/module.h>
#include <handystats/metrics_dump.hpp>
#include <handystats/json_dump.hpp>

#include "config_impl.hpp"
#include "metrics_dump_impl.hpp"

#include "message_queue_helper.hpp"
#include "metrics_dump_helper.hpp"

#ifndef _HAVE_HANDY_MODULE_TEST
#define _HAVE_HANDY_MODULE_TEST 1
#endif
HANDY_MODULE(TEST)

class HandyConfigurationTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(HandyConfigurationTest, MetricsDumpConfiguration) {
	HANDY_CONFIG_JSON(
			"{\
				\"metrics-dump\": {\
					\"interval\": 750\
				}\
			}"
		);

	ASSERT_NEAR(handystats::config::metrics_dump_opts.interval.count(),
			handystats::chrono::duration(750, handystats::chrono::time_unit::MSEC).count(),
			1E-6
		);
}

TEST_F(HandyConfigurationTest, MetricsDumpToJsonTrueCheck) {
	HANDY_CONFIG_JSON(
			"{\
				\"metrics-dump\": {\
					\"interval\": 2\
				}\
			}"
		);

	ASSERT_NEAR(handystats::config::metrics_dump_opts.interval.count(),
			handystats::chrono::duration(2, handystats::chrono::time_unit::MSEC).count(),
			1E-6
		);

	HANDY_INIT();

	TEST_GAUGE_SET("gauge.test", 15);

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find("gauge.test") != metrics_dump->first.end());

	auto json_dump = HANDY_JSON_DUMP();

	ASSERT_FALSE(json_dump.empty());
}

TEST_F(HandyConfigurationTest, NoMetricsDumpCheck) {
	HANDY_CONFIG_JSON(
			"{\
				\"metrics-dump\": {\
					\"interval\": 0\
				}\
			}"
		);

	ASSERT_NEAR(handystats::config::metrics_dump_opts.interval.count(),
			handystats::chrono::duration(0, handystats::chrono::time_unit::MSEC).count(),
			1E-6
		);

	HANDY_INIT();

	TEST_GAUGE_SET("gauge.test", 15);

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.empty());

	auto json_dump = HANDY_JSON_DUMP();

	ASSERT_EQ(json_dump, "{}");
}

TEST_F(HandyConfigurationTest, TimerConfigurationIdleTimeout) {
	HANDY_CONFIG_JSON(
			"{\
				\"metrics\": {\
					\"timer\": {\
						\"idle-timeout\": 100\
					}\
				},\
				\"metrics-dump\": {\
					\"interval\": 2\
				}\
			}"
		);

	ASSERT_NEAR(handystats::config::metrics::timer_opts.idle_timeout.count(),
			handystats::chrono::duration(100, handystats::chrono::time_unit::MSEC).count(),
			1E-6
		);

	HANDY_INIT();

	TEST_TIMER_START("dead-timer");
	TEST_TIMER_START("alive-timer");

	for (int cycle = 0; cycle < 100; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		TEST_TIMER_HEARTBEAT("alive-timer");
	}

	TEST_TIMER_STOP("dead-timer");
	TEST_TIMER_STOP("alive-timer");

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();
	ASSERT_EQ(
			metrics_dump->first.at("dead-timer").get<handystats::statistics::tag::count>(),
			0
		);
	ASSERT_EQ(
			metrics_dump->first.at("alive-timer").get<handystats::statistics::tag::count>(),
			1
		);
}

TEST_F(HandyConfigurationTest, NoConfigurationUseDefaults) {
	HANDY_CONFIG_JSON(
			"{\
			}"
		);
	ASSERT_EQ(handystats::config::statistics_opts.moving_interval.count(), handystats::config::statistics().moving_interval.count());
	ASSERT_EQ(handystats::config::statistics_opts.histogram_bins, handystats::config::statistics().histogram_bins);
	ASSERT_EQ(handystats::config::metrics::timer_opts.idle_timeout.count(), handystats::config::metrics::timer().idle_timeout.count());
	ASSERT_EQ(handystats::config::metrics_dump_opts.interval.count(), handystats::config::metrics_dump().interval.count());
}

TEST_F(HandyConfigurationTest, IncrementalStatisticsConfiguration) {
	HANDY_CONFIG_JSON(
			"{\
				\"statistics\": {\
					\"moving-interval\": 1234,\
					\"histogram-bins\": 200\
				}\
			}"
		);

	ASSERT_NEAR(handystats::config::statistics_opts.moving_interval.count(),
			handystats::chrono::duration(1234, handystats::chrono::time_unit::MSEC).count(),
			1E-6
		);
	ASSERT_EQ(handystats::config::statistics_opts.histogram_bins, 200);
}

TEST_F(HandyConfigurationTest, EnableFalseConfigOption) {
	HANDY_CONFIG_JSON(
			"{\
				\"core\": {\
					\"enable\": false\
				},\
				\"metrics-dump\": {\
					\"interval\": 1\
				}\
			}"
		);

	ASSERT_FALSE(handystats::config::core_opts.enable);

	HANDY_INIT();

	for (int i = 0; i < 10; ++i) {
		TEST_GAUGE_SET("test.gauge", i);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	ASSERT_TRUE(HANDY_METRICS_DUMP()->first.empty() && HANDY_METRICS_DUMP()->second.empty());
	ASSERT_EQ(HANDY_JSON_DUMP(), "{}");
}

TEST_F(HandyConfigurationTest, HistogramConfigOptionEnabled) {
	HANDY_CONFIG_JSON(
			"{\
				\"statistics\": {\
					\"histogram-bins\": 25,\
					\"moving-interval\": 1000,\
					\"tags\": [\"histogram\"]\
				},\
				\"metrics-dump\": {\
					\"interval\": 1\
				}\
			}"
		);

	ASSERT_EQ(handystats::config::statistics_opts.histogram_bins, 25);

	ASSERT_EQ(handystats::config::metrics::gauge_opts.values.histogram_bins, 25);

	ASSERT_EQ(handystats::config::metrics::gauge_opts.values.tags, handystats::statistics::tag::histogram);

	HANDY_INIT();

	for (int i = 0; i < 10; ++i) {
		TEST_GAUGE_SET("test.gauge", i);
	}

	for (int i = 0; i < 100; ++i) {
		TEST_COUNTER_INCREMENT("test.counter", i);
	}

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	ASSERT_FALSE(HANDY_METRICS_DUMP()->first.empty() || HANDY_METRICS_DUMP()->second.empty());
	auto metrics_dump = HANDY_METRICS_DUMP();

	auto gauge_values = metrics_dump->first.at("test.gauge");

	ASSERT_TRUE(gauge_values.enabled(handystats::statistics::tag::histogram));
	ASSERT_EQ(gauge_values.get<handystats::statistics::tag::histogram>().size(), 10);

	auto counter_values = metrics_dump->first.at("test.counter");

	ASSERT_TRUE(counter_values.enabled(handystats::statistics::tag::histogram));
	ASSERT_EQ(counter_values.get<handystats::statistics::tag::histogram>().size(), 25);
}

TEST_F(HandyConfigurationTest, HistogramConfigOptionDisabled) {
	HANDY_CONFIG_JSON(
			"{\
				\"statistics\": {\
					\"histogram-bins\": 20,\
					\"tags\": [],\
					\"moving-interval\": 1000\
				},\
				\"metrics-dump\": {\
					\"interval\": 1\
				}\
			}"
		);

	ASSERT_EQ(handystats::config::statistics_opts.histogram_bins, 20);

	HANDY_INIT();

	for (int i = 0; i < 10; ++i) {
		TEST_GAUGE_SET("test.gauge", i);
	}

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	ASSERT_FALSE(HANDY_METRICS_DUMP()->first.empty() && HANDY_METRICS_DUMP()->second.empty());
	auto metrics_dump = HANDY_METRICS_DUMP();

	auto gauge_values = metrics_dump->first.at("test.gauge");

	ASSERT_FALSE(gauge_values.computed(handystats::statistics::tag::histogram));
}

TEST_F(HandyConfigurationTest, MetricsConfigOverwritesStatistcs) {
	HANDY_CONFIG_JSON(
			"{\
				\"statistics\": {\
					\"histogram-bins\": 50,\
					\"tags\": [\"histogram\"],\
					\"moving-interval\": 1000\
				},\
				\"metrics\": {\
					\"gauge\": {\
						\"tags\": []\
					}\
				},\
				\"metrics-dump\": {\
					\"interval\": 1\
				}\
			}"
		);

	ASSERT_EQ(handystats::config::statistics_opts.histogram_bins, 50);

	HANDY_INIT();

	for (int i = 0; i < 10; ++i) {
		TEST_GAUGE_SET("test.gauge", i);
	}

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	ASSERT_FALSE(HANDY_METRICS_DUMP()->first.empty() && HANDY_METRICS_DUMP()->second.empty());
	auto metrics_dump = HANDY_METRICS_DUMP();

	auto gauge_values = metrics_dump->first.at("test.gauge");

	ASSERT_FALSE(gauge_values.computed(handystats::statistics::tag::histogram));
}
