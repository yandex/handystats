#include <gtest/gtest.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/counter.hpp>

using namespace handystats::metrics;

TEST(CounterTest, TestCounterConstruction) {
	counter sample_counter(10, handystats::chrono::steady_clock<counter::time_duration>::now());

	ASSERT_EQ(sample_counter.get().first, 10);
}

TEST(CounterTest, TestCounterIncrementDecrement) {
	counter sample_counter;

	const int min_test_value = 1E3;
	const int max_test_value = 1E4;

	sample_counter.increment(min_test_value, handystats::chrono::steady_clock<counter::time_duration>::now());
	ASSERT_EQ(sample_counter.get().first, min_test_value);

	for (int step = 0; step < max_test_value - min_test_value; ++step) {
		sample_counter.increment(1, handystats::chrono::steady_clock<counter::time_duration>::now());
	}
	ASSERT_EQ(sample_counter.get().first, max_test_value);

	for (int step = 0; step < max_test_value; ++step) {
		sample_counter.decrement(1, handystats::chrono::steady_clock<counter::time_duration>::now());
	}
	ASSERT_EQ(sample_counter.get().first, 0);
}

TEST(CounterTest, TestCounterInternalStats) {
	counter sample_counter;

	const int min_test_value = 1E3;
	const int max_test_value = 1E4;

	sample_counter.increment(min_test_value, handystats::chrono::steady_clock<counter::time_duration>::now());
	sample_counter.increment(max_test_value - min_test_value, handystats::chrono::steady_clock<counter::time_duration>::now());
	ASSERT_EQ(sample_counter.get_stats().count, 2);
	ASSERT_EQ(sample_counter.get_stats().min_value, min_test_value);
	ASSERT_EQ(sample_counter.get_stats().max_value, max_test_value);

	for (int step = 0; step < max_test_value; ++step) {
		sample_counter.decrement(1, handystats::chrono::steady_clock<counter::time_duration>::now());
	}
	ASSERT_EQ(sample_counter.get_stats().count, 2 + max_test_value);
	ASSERT_EQ(sample_counter.get_stats().min_value, 0);
	ASSERT_EQ(sample_counter.get_stats().max_value, max_test_value);
}
