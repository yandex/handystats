#include <chrono>
#include <thread>
#include <map>
#include <string>

#include <gtest/gtest.h>

#include <handystats/operation.hpp>
#include <handystats/configuration.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/json_dump.hpp>
#include <handystats/rapidjson/document.h>

#include "internal_metrics_impl.hpp"
#include "internal_metrics/internal_gauge_impl.hpp"
#include "internal_metrics/internal_counter_impl.hpp"
#include "internal_metrics/internal_timer_impl.hpp"
#include "configuration_impl.hpp"

#include "message_queue_helper.hpp"

namespace handystats { namespace internal {

extern std::map<std::string, internal_metric> internal_metrics;

}} // namespace handystats::internal


class HandyConfigurationTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};


TEST_F(HandyConfigurationTest, TimerConfigurationIdleTimeout) {
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"timer\": {\
						\"idle-timeout\": 10\
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

	ASSERT_EQ(
			boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["dead-timer"])
			->base_timer
			->values
			.count(),
			0);
	ASSERT_EQ(
			boost::get<handystats::internal::internal_timer*>(handystats::internal::internal_metrics["alive-timer"])
			->base_timer
			->values
			.count(),
			1);
}

TEST_F(HandyConfigurationTest, MessageQueueCorrectConfiguration) {
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"message-queue\": {\
						\"sleep-on-empty\": [1, 10, 15, 20]\
					}\
				}\
			}"
		);
	ASSERT_EQ(handystats::config::message_queue.sleep_on_empty.size(), 4);
	ASSERT_EQ(handystats::config::message_queue.sleep_on_empty[0], std::chrono::microseconds(1));
	ASSERT_EQ(handystats::config::message_queue.sleep_on_empty[1], std::chrono::microseconds(10));
	ASSERT_EQ(handystats::config::message_queue.sleep_on_empty[2], std::chrono::microseconds(15));
	ASSERT_EQ(handystats::config::message_queue.sleep_on_empty[3], std::chrono::microseconds(20));

	HANDY_INIT();
}

TEST_F(HandyConfigurationTest, MessageQueueIncorrectConfiguration) {
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"message-queue\": {\
						\"sleep-on-empty\": [1, -10, 15, 20]\
					}\
				}\
			}"
		);
	ASSERT_EQ(handystats::config::message_queue.sleep_on_empty, handystats::config::message_queue_parameters().sleep_on_empty);

	HANDY_INIT();
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
	ASSERT_EQ(handystats::config::json_dump.interval, handystats::config::json_dump_parameters().interval);
	ASSERT_EQ(handystats::config::metrics_dump.interval, handystats::config::metrics_dump_parameters().interval);
	ASSERT_EQ(handystats::config::message_queue.sleep_on_empty, handystats::config::message_queue_parameters().sleep_on_empty);

	HANDY_INIT();
}

TEST_F(HandyConfigurationTest, JsonDumpIntervalConfiguration) {
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"json-dump\": {\
						\"interval\": 100000\
					},\
					\"metrics-dump\": {\
						\"interval\": 500000\
					}\
				}\
			}"
		);
	ASSERT_EQ(handystats::config::json_dump.interval, std::chrono::milliseconds(100000));
	ASSERT_EQ(handystats::config::metrics_dump.interval, std::chrono::milliseconds(500000));

	HANDY_INIT();

	// TODO: test json-dump-interval configuration in action
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

	HANDY_INIT();
}
