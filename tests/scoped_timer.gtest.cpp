/*
 * Copyright (c) YANDEX LLC. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
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

class HandyScopedTimerTest : public ::testing::Test {
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

TEST_F(HandyScopedTimerTest, TestSingleInstanceScopedTimer) {
	const int COUNT = 5;
	auto sleep_time = std::chrono::milliseconds(10);

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_SCOPE("sleep.time");
		std::this_thread::sleep_for(sleep_time);
	}

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_EQ(
			boost::get<handystats::metrics::gauge>(metrics_dump->at("handystats.internal.size"))
			.values()
			.get<handystats::statistics::tag::value>(),
			1
		);

	ASSERT_TRUE(metrics_dump->find("sleep.time") != metrics_dump->end());

//	ASSERT_TRUE(
//			boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
//			.instances
//			.empty()
//		);

	const auto& agg_stats =
		boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
		.values();

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), COUNT);
	ASSERT_GE(
			agg_stats.get<handystats::statistics::tag::min>(),
			handystats::chrono::duration::convert_to(handystats::metrics::timer::value_unit,
				handystats::chrono::duration(sleep_time.count(), handystats::chrono::time_unit::MSEC)).count()
		);
}

TEST_F(HandyScopedTimerTest, TestMultipleNestedScopes) {
	auto sleep_time = std::chrono::milliseconds(1);

	{
		HANDY_TIMER_SCOPE("sleep.time");
		std::this_thread::sleep_for(sleep_time);

		{
			HANDY_TIMER_SCOPE("sleep.time");
			std::this_thread::sleep_for(sleep_time);

			{
				HANDY_TIMER_SCOPE("sleep.time");
				std::this_thread::sleep_for(sleep_time);
			}
		}
	}

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_EQ(
			boost::get<handystats::metrics::gauge>(metrics_dump->at("handystats.internal.size"))
			.values()
			.get<handystats::statistics::tag::value>(),
			1
		);

	ASSERT_TRUE(metrics_dump->find("sleep.time") != metrics_dump->end());

//	ASSERT_TRUE(
//			boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
//			.instances
//			.empty()
//		);

	const auto& agg_stats =
		boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
		.values();

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), 3);
	ASSERT_GE(
			agg_stats.get<handystats::statistics::tag::min>(),
			handystats::chrono::duration::convert_to(handystats::metrics::timer::value_unit,
				handystats::chrono::duration(sleep_time.count(), handystats::chrono::time_unit::MSEC)).count()
		);
	ASSERT_GE(
			agg_stats.get<handystats::statistics::tag::max>(),
			handystats::chrono::duration::convert_to(handystats::metrics::timer::value_unit,
				handystats::chrono::duration(sleep_time.count(), handystats::chrono::time_unit::MSEC)).count() * 3
		);
}

TEST_F(HandyScopedTimerTest, TestSeveralScopedTimersInOneScope) {
	const int COUNT = 10;
	auto sleep_time = std::chrono::milliseconds(1);

	for (int step = 0; step < COUNT; ++step) {
		HANDY_TIMER_SCOPE("double.sleep.time");
		std::this_thread::sleep_for(sleep_time);
		HANDY_TIMER_SCOPE("sleep.time");
		std::this_thread::sleep_for(sleep_time);
	}

	handystats::message_queue::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_EQ(
			boost::get<handystats::metrics::gauge>(metrics_dump->at("handystats.internal.size"))
			.values()
			.get<handystats::statistics::tag::value>(),
			2
		);

	ASSERT_TRUE(metrics_dump->find("sleep.time") != metrics_dump->end());
	ASSERT_TRUE(metrics_dump->find("double.sleep.time") != metrics_dump->end());

//	ASSERT_TRUE(
//			boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
//			.instances
//			.empty()
//		);
//	ASSERT_TRUE(
//			boost::get<handystats::metrics::timer>(metrics_dump->at("double.sleep.time"))
//			.instances
//			.empty()
//		);

	const auto& agg_stats =
		boost::get<handystats::metrics::timer>(metrics_dump->at("sleep.time"))
		.values();

	const auto& double_agg_stats =
		boost::get<handystats::metrics::timer>(metrics_dump->at("double.sleep.time"))
		.values();

	ASSERT_EQ(agg_stats.get<handystats::statistics::tag::count>(), COUNT);
	ASSERT_GE(
			agg_stats.get<handystats::statistics::tag::min>(),
			handystats::chrono::duration::convert_to(handystats::metrics::timer::value_unit,
				handystats::chrono::duration(sleep_time.count(), handystats::chrono::time_unit::MSEC)).count()
		);

	ASSERT_EQ(double_agg_stats.get<handystats::statistics::tag::count>(), COUNT);
	ASSERT_GE(
			double_agg_stats.get<handystats::statistics::tag::min>(),
			handystats::chrono::duration::convert_to(handystats::metrics::timer::value_unit,
				handystats::chrono::duration(sleep_time.count(), handystats::chrono::time_unit::MSEC)).count() * 2
		);
}

