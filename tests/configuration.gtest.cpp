#include <chrono>
#include <thread>
#include <map>
#include <string>

#include <gtest/gtest.h>

#include <handystats/rapidjson/document.h>

#include <handystats/core.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/metrics_dump.hpp>
#include <handystats/json_dump.hpp>

#include "config_impl.hpp"
#include "metrics_dump_impl.hpp"

#include "message_queue_helper.hpp"


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
					\"interval\": 750,\
					\"to-json\": false\
				}\
			}"
		);

	ASSERT_NEAR(handystats::config::metrics_dump_opts.interval.count(),
			handystats::chrono::duration_cast<handystats::chrono::clock::duration>(
				std::chrono::milliseconds(750)
			)
			.count(),
			1E-6
		);
	ASSERT_EQ(handystats::config::metrics_dump_opts.to_json, false);
}

TEST_F(HandyConfigurationTest, MetricsDumpToJsonTrueCheck) {
	HANDY_CONFIG_JSON(
			"{\
				\"metrics-dump\": {\
					\"interval\": 2,\
					\"to-json\": true\
				}\
			}"
		);

	ASSERT_NEAR(handystats::config::metrics_dump_opts.interval.count(),
			handystats::chrono::duration_cast<handystats::chrono::clock::duration>(
				std::chrono::milliseconds(2)
			)
			.count(),
			1E-6
		);
	ASSERT_EQ(handystats::config::metrics_dump_opts.to_json, true);

	HANDY_INIT();

	HANDY_GAUGE_SET("gauge.test", 15);

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("gauge.test") != metrics_dump->end());

	auto json_dump = HANDY_JSON_DUMP();

	ASSERT_FALSE(json_dump->empty());
}

TEST_F(HandyConfigurationTest, MetricsDumpToJsonFalseCheck) {
	HANDY_CONFIG_JSON(
			"{\
				\"metrics-dump\": {\
					\"interval\": 2,\
					\"to-json\": false\
				}\
			}"
		);

	ASSERT_NEAR(handystats::config::metrics_dump_opts.interval.count(),
			handystats::chrono::duration_cast<handystats::chrono::clock::duration>(
				std::chrono::milliseconds(2)
			)
			.count(),
			1E-6
		);
	ASSERT_EQ(handystats::config::metrics_dump_opts.to_json, false);

	HANDY_INIT();

	HANDY_GAUGE_SET("gauge.test", 15);

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("gauge.test") != metrics_dump->end());

	auto json_dump = HANDY_JSON_DUMP();

	ASSERT_TRUE(json_dump->empty());
}

TEST_F(HandyConfigurationTest, NoMetricsDumpCheck) {
	HANDY_CONFIG_JSON(
			"{\
				\"metrics-dump\": {\
					\"interval\": 0,\
					\"to-json\": true\
				}\
			}"
		);

	ASSERT_NEAR(handystats::config::metrics_dump_opts.interval.count(),
			handystats::chrono::duration_cast<handystats::chrono::clock::duration>(
				std::chrono::milliseconds(0)
			)
			.count(),
			1E-6
		);
	ASSERT_EQ(handystats::config::metrics_dump_opts.to_json, true);

	HANDY_INIT();

	HANDY_GAUGE_SET("gauge.test", 15);

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->empty());

	auto json_dump = HANDY_JSON_DUMP();

	ASSERT_TRUE(json_dump->empty());
}

TEST_F(HandyConfigurationTest, TimerConfigurationIdleTimeout) {
	HANDY_CONFIG_JSON(
			"{\
				\"metrics\": {\
					\"timer\": {\
						\"idle-timeout\": 10\
					}\
				},\
				\"metrics-dump\": {\
					\"interval\": 2\
				}\
			}"
		);

	ASSERT_NEAR(handystats::config::metrics::timer_opts.idle_timeout.count(),
			handystats::chrono::duration_cast<handystats::chrono::clock::duration>(
				std::chrono::milliseconds(10)
			)
			.count(),
			1E-6
		);

	HANDY_INIT();

	HANDY_TIMER_START("dead-timer");
	HANDY_TIMER_START("alive-timer");

	for (int cycle = 0; cycle < 10; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		HANDY_TIMER_HEARTBEAT("alive-timer");
	}

	HANDY_TIMER_STOP("dead-timer");
	HANDY_TIMER_STOP("alive-timer");

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	auto metrics_dump = HANDY_METRICS_DUMP();
	ASSERT_EQ(
			boost::get<handystats::metrics::timer>(metrics_dump->at("dead-timer"))
			.values()
			.get<handystats::statistics::tag::count>(),
			0
		);
	ASSERT_EQ(
			boost::get<handystats::metrics::timer>(metrics_dump->at("alive-timer"))
			.values()
			.get<handystats::statistics::tag::count>(),
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
	ASSERT_EQ(handystats::config::metrics_dump_opts.to_json, handystats::config::metrics_dump().to_json);
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
			handystats::chrono::duration_cast<handystats::chrono::clock::duration>(
				std::chrono::milliseconds(1234)
			)
			.count(),
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
					\"interval\": 1,\
					\"to-json\": true\
				}\
			}"
		);

	ASSERT_FALSE(handystats::config::core_opts.enable);

	HANDY_INIT();

	for (int i = 0; i < 10; ++i) {
		HANDY_GAUGE_SET("test.gauge", i);
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	ASSERT_TRUE(HANDY_METRICS_DUMP()->empty());
	ASSERT_TRUE(HANDY_JSON_DUMP()->empty());
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
	ASSERT_EQ(handystats::config::metrics::counter_opts.incr_deltas.histogram_bins, 25);

	ASSERT_EQ(handystats::config::metrics::gauge_opts.values.tag_mask, handystats::statistics::tag::histogram);
	ASSERT_EQ(handystats::config::metrics::counter_opts.incr_deltas.tag_mask, handystats::statistics::tag::histogram);

	HANDY_INIT();

	for (int i = 0; i < 10; ++i) {
		HANDY_GAUGE_SET("test.gauge", i);
	}

	for (int i = 0; i < 100; ++i) {
		HANDY_COUNTER_INCREMENT("test.counter", i);
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	ASSERT_FALSE(HANDY_METRICS_DUMP()->empty());
	auto metrics_dump = HANDY_METRICS_DUMP();

	auto gauge = boost::get<handystats::metrics::gauge>(metrics_dump->at("test.gauge"));

	ASSERT_TRUE(gauge.values().enabled(handystats::statistics::tag::histogram));
	ASSERT_EQ(gauge.values().get<handystats::statistics::tag::histogram>().size(), 10);

	auto counter = boost::get<handystats::metrics::counter>(metrics_dump->at("test.counter"));

	ASSERT_TRUE(counter.incr_deltas().enabled(handystats::statistics::tag::histogram));
	ASSERT_EQ(counter.incr_deltas().get<handystats::statistics::tag::histogram>().size(), 25);
}

TEST_F(HandyConfigurationTest, HistogramConfigOptionDisabled) {
	HANDY_CONFIG_JSON(
			"{\
				\"statistics\": {\
					\"histogram-bins\": 0,\
					\"tags\": \"histogram\",\
					\"moving-interval\": 1000\
				},\
				\"metrics-dump\": {\
					\"interval\": 1\
				}\
			}"
		);

	ASSERT_EQ(handystats::config::statistics_opts.histogram_bins, 0);

	HANDY_INIT();

	for (int i = 0; i < 10; ++i) {
		HANDY_GAUGE_SET("test.gauge", i);
	}

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(10));

	ASSERT_FALSE(HANDY_METRICS_DUMP()->empty());
	auto metrics_dump = HANDY_METRICS_DUMP();

	auto gauge = boost::get<handystats::metrics::gauge>(metrics_dump->at("test.gauge"));

	ASSERT_EQ(gauge.values().get<handystats::statistics::tag::histogram>().size(), 0);
}
