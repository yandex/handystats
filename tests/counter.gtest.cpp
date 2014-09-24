#include <gtest/gtest.h>

#include <handystats/rapidjson/allocators.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/counter.hpp>

#include <handystats/json/counter_json_writer.hpp>

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
