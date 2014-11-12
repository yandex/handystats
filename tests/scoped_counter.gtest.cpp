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
#include <chrono>
#include <queue>
#include <string>
#include <map>
#include <memory>

#include <gtest/gtest.h>

#include <handystats/core.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/metrics_dump.hpp>
#include <handystats/module.h>

#include "message_queue_helper.hpp"
#include "metrics_dump_helper.hpp"

#ifndef _HAVE_HANDY_MODULE_TEST
#define _HAVE_HANDY_MODULE_TEST 1
#endif

HANDY_MODULE(TEST)

#if _HAVE_HANDY_MODULE_TEST == 1
#define TEST_TIMER_SCOPE(...) HANDY_TIMER_SCOPE(__VA_ARGS__)
#define TEST_COUNTER_SCOPE(...) HANDY_COUNTER_SCOPE(__VA_ARGS__)
#else
#define TEST_TIMER_SCOPE(...)
#define TEST_COUNTER_SCOPE(...)
#endif

class HandyScopedCounterTest : public ::testing::Test {
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

TEST_F(HandyScopedCounterTest, TestSingleScope) {
	const int COUNT = 10;

	for (int step = 0; step < COUNT; ++step) {
		TEST_COUNTER_SCOPE("test.counter", 1);
	}

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find("test.counter") != metrics_dump->first.end());

	const auto& agg_stats = metrics_dump->first.at("test.counter");

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), 2 * COUNT + 1);
	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::min>(), 0);
	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::max>(), 1);
}

TEST_F(HandyScopedCounterTest, TestDoubleNestedScope) {
	const int COUNT = 10;

	for (int step = 0; step < COUNT; ++step) {
		TEST_COUNTER_SCOPE("test.counter", 1);
		{
			TEST_COUNTER_SCOPE("test.counter", -1);
		}
	}

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find("test.counter") != metrics_dump->first.end());

	const auto& agg_stats = metrics_dump->first.at("test.counter");

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), 4 * COUNT + 1);
	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::min>(), 0);
	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::max>(), 1);
}
