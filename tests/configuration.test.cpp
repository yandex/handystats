#include <chrono>
#include <thread>
#include <map>
#include <string>

#include <gtest/gtest.h>

#include <handystats/rapidjson/document.h>

#include <handystats/operation.hpp>
#include <handystats/configuration.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/configuration.hpp>
#include <handystats/metrics_dump.hpp>
#include <handystats/json_dump.hpp>

#include "configuration_impl.hpp"
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
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"metrics-dump\": {\
						\"interval\": 750,\
						\"to-json\": false\
					}\
				}\
			}"
		);

	ASSERT_EQ(handystats::config::metrics_dump.interval, std::chrono::milliseconds(750));
	ASSERT_EQ(handystats::config::metrics_dump.to_json, false);
}

TEST_F(HandyConfigurationTest, MetricsDumpToJsonTrueCheck) {
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"metrics-dump\": {\
						\"interval\": 2,\
						\"to-json\": true\
					}\
				}\
			}"
		);

	ASSERT_EQ(handystats::config::metrics_dump.interval, std::chrono::milliseconds(2));
	ASSERT_EQ(handystats::config::metrics_dump.to_json, true);

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
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"metrics-dump\": {\
						\"interval\": 2,\
						\"to-json\": false\
					}\
				}\
			}"
		);

	ASSERT_EQ(handystats::config::metrics_dump.interval, std::chrono::milliseconds(2));
	ASSERT_EQ(handystats::config::metrics_dump.to_json, false);

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
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"metrics-dump\": {\
						\"interval\": 0,\
						\"to-json\": true\
					}\
				}\
			}"
		);

	ASSERT_EQ(handystats::config::metrics_dump.interval, std::chrono::milliseconds(0));
	ASSERT_EQ(handystats::config::metrics_dump.to_json, true);

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
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"timer\": {\
						\"idle-timeout\": 10\
					},\
					\"metrics-dump\": {\
						\"interval\": 2\
					}\
				}\
			}"
		);

	ASSERT_EQ(handystats::config::timer.idle_timeout, std::chrono::milliseconds(10));

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
			.values
			.count(),
			0
		);
	ASSERT_EQ(
			boost::get<handystats::metrics::timer>(metrics_dump->at("alive-timer"))
			.values
			.count(),
			1
		);
}

TEST_F(HandyConfigurationTest, NoConfigurationUseDefaults) {
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
				}\
			}"
		);
	ASSERT_EQ(handystats::config::incremental_statistics.moving_average_alpha, handystats::config::incremental_statistics_parameters().moving_average_alpha);
	ASSERT_EQ(handystats::config::incremental_statistics.moving_interval, handystats::config::incremental_statistics_parameters().moving_interval);
	ASSERT_EQ(handystats::config::timer.idle_timeout, handystats::config::timer_parameters().idle_timeout);
	ASSERT_EQ(handystats::config::metrics_dump.interval, handystats::config::metrics_dump_parameters().interval);
	ASSERT_EQ(handystats::config::metrics_dump.to_json, handystats::config::metrics_dump_parameters().to_json);
}

TEST_F(HandyConfigurationTest, IncrementalStatisticsConfiguration) {
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"incremental-statistics\": {\
						\"moving-interval\": 1234,\
						\"moving-average-alpha\": 1.5\
					}\
				}\
			}"
		);

	ASSERT_EQ(handystats::config::incremental_statistics.moving_interval, std::chrono::milliseconds(1234));
	ASSERT_EQ(handystats::config::incremental_statistics.moving_average_alpha, 1.5);
}
