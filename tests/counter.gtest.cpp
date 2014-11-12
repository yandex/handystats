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

#include <gtest/gtest.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/counter.hpp>

using namespace handystats::metrics;

TEST(CounterTest, TestCounterConstruction) {
	counter sample_counter;
	sample_counter.init(10);

	ASSERT_EQ(sample_counter.values().get<handystats::statistics::tag::value>(), 10);
}

TEST(CounterTest, TestCounterIncrementDecrement) {
	counter sample_counter;

	const int min_test_value = 1E3;
	const int max_test_value = 1E4;

	sample_counter.increment(min_test_value);
	ASSERT_EQ(sample_counter.values().get<handystats::statistics::tag::value>(), min_test_value);

	for (int step = 0; step < max_test_value - min_test_value; ++step) {
		sample_counter.increment(1);
	}
	ASSERT_EQ(sample_counter.values().get<handystats::statistics::tag::value>(), max_test_value);

	for (int step = 0; step < max_test_value; ++step) {
		sample_counter.decrement(1);
	}
	ASSERT_EQ(sample_counter.values().get<handystats::statistics::tag::value>(), 0);
}

TEST(CounterTest, TestCounterInternalStats) {
	counter sample_counter;

	const int min_test_value = 1E3;
	const int max_test_value = 1E4;

	sample_counter.increment(min_test_value);
	sample_counter.increment(max_test_value - min_test_value);

	ASSERT_EQ(sample_counter.values().get<handystats::statistics::tag::min>(), 0);
	ASSERT_EQ(sample_counter.values().get<handystats::statistics::tag::max>(), max_test_value);

	for (int step = 0; step < max_test_value; ++step) {
		sample_counter.decrement(1);
	}

	ASSERT_EQ(sample_counter.values().get<handystats::statistics::tag::min>(), 0);
	ASSERT_EQ(sample_counter.values().get<handystats::statistics::tag::max>(), max_test_value);
}
