#include <chrono>
#include <thread>
#include <map>
#include <string>

#include <gtest/gtest.h>

#include <handystats/operation.hpp>
#include <handystats/configuration.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/json_dump.hpp>
#include <handystats/metrics_dump.hpp>
#include <handystats/rapidjson/document.h>

#include "configuration_impl.hpp"

class ZeroDumpIntervalTest : public ::testing::Test {
protected:
	virtual void SetUp() {
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(ZeroDumpIntervalTest, ZeroDumpIntervalsConfiguration) {
	HANDY_CONFIGURATION_JSON(
			"{\
				\"handystats\": {\
					\"json-dump\": {\
						\"interval\": 0\
					},\
					\"metrics-dump\": {\
						\"interval\": 0\
					}\
				}\
			}"
		);
	ASSERT_EQ(handystats::config::json_dump.interval, std::chrono::milliseconds(0));
	ASSERT_EQ(handystats::config::metrics_dump.interval, std::chrono::milliseconds(0));

	HANDY_INIT();

	const size_t COUNTERS_COUNT = 10;
	for (size_t i = 0; i < COUNTERS_COUNT; ++i) {
		HANDY_COUNTER_INCREMENT("counter" + std::to_string(i));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(50));

	ASSERT_TRUE(HANDY_JSON_DUMP()->empty());
	ASSERT_TRUE(HANDY_METRICS_DUMP()->empty());
}
