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

#include <gtest/gtest.h>

#include <rapidjson/allocators.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/gauge.hpp>

#include <handystats/json/gauge_json_writer.hpp>

using namespace handystats::metrics;

TEST(GaugeTest, TestGaugeConstruction) {
	gauge sample_gauge;
	sample_gauge.set(-10);

	ASSERT_NEAR(sample_gauge.values().get<handystats::statistics::tag::value>(), -10, 1E-9);
}

TEST(GaugeTest, TestGaugeSetMethod) {
	gauge sample_gauge;

	for (int test_value = -1E3; test_value < 1E3; ++test_value) {
		sample_gauge.set(test_value);

		ASSERT_NEAR(sample_gauge.values().get<handystats::statistics::tag::value>(), test_value, 1E-9);
	}
}

TEST(GaugeTest, TestGaugeInternalStats) {
	gauge sample_gauge;

	const int min_test_value = -1E3;
	const int max_test_value = 1E3;

	for (int test_value = min_test_value; test_value <= max_test_value; ++test_value) {
		sample_gauge.set(test_value);
	}


	const auto& stats = sample_gauge.values();
	ASSERT_NEAR(stats.get<handystats::statistics::tag::min>(), min_test_value, 1E-9);
	ASSERT_NEAR(stats.get<handystats::statistics::tag::max>(), max_test_value, 1E-9);

	ASSERT_EQ(stats.get<handystats::statistics::tag::count>(), max_test_value - min_test_value + 1);
	ASSERT_NEAR(stats.get<handystats::statistics::tag::sum>(), 0, 1E-9);
	ASSERT_NEAR(stats.get<handystats::statistics::tag::avg>(), 0, 1E-9);
}
