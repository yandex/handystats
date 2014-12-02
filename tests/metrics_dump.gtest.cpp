/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <vector>
#include <utility>
#include <cstdlib>
#include <thread>
#include <queue>
#include <string>
#include <map>
#include <memory>
#include <chrono>

#include <gtest/gtest.h>

#include <handystats/chrono.hpp>
#include <handystats/core.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/metrics_dump.hpp>

#include "message_queue_helper.hpp"
#include "metrics_dump_helper.hpp"

class MetricsDumpTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_CONFIG_JSON(
				"{\
					\"dump-interval\": 10\
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

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find("counter") != metrics_dump->first.end());

	auto& counter_values = metrics_dump->first.at("counter");
	ASSERT_EQ(counter_values.get<handystats::statistics::tag::value>(), INCR_COUNT * INCR_VALUE);
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

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find("timer") != metrics_dump->first.end());

	auto& timer_values = metrics_dump->first.at("timer");
	ASSERT_EQ(timer_values.get<handystats::statistics::tag::count>(), TIMER_INSTANCES);
	ASSERT_TRUE(
			timer_values.get<handystats::statistics::tag::min>() >=
				handystats::chrono::milliseconds(sleep_interval.count()).count(handystats::metrics::timer::value_unit)
		);
}

TEST_F(MetricsDumpTest, SampleGauge) {
	const size_t MIN_VALUE = 0;
	const size_t MAX_VALUE = 100;

	for (size_t value = MIN_VALUE; value <= MAX_VALUE; ++value) {
		HANDY_GAUGE_SET("gauge", value);
	}

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find("gauge") != metrics_dump->first.end());

	auto& gauge_values = metrics_dump->first.at("gauge");
	ASSERT_EQ(gauge_values.get<handystats::statistics::tag::count>(), MAX_VALUE - MIN_VALUE + 1);
	ASSERT_EQ(gauge_values.get<handystats::statistics::tag::min>(), MIN_VALUE);
	ASSERT_EQ(gauge_values.get<handystats::statistics::tag::max>(), MAX_VALUE);
	ASSERT_EQ(gauge_values.get<handystats::statistics::tag::avg>(), (MAX_VALUE + MIN_VALUE) / 2.0);
}
