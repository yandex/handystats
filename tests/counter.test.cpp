#include <gtest/gtest.h>

#include <handystats/rapidjson/allocators.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/counter.hpp>

#include <handystats/json/counter_json_writer.hpp>

using namespace handystats::metrics;

TEST(CounterTest, TestCounterConstruction) {
	counter sample_counter(10, handystats::chrono::default_clock::now());

	ASSERT_EQ(sample_counter.value, 10);

	std::cout << handystats::json::write_to_json_string<rapidjson::MemoryPoolAllocator<>>(&sample_counter) << std::endl;
}

TEST(CounterTest, TestCounterIncrementDecrement) {
	counter sample_counter;

	const int min_test_value = 1E3;
	const int max_test_value = 1E4;

	sample_counter.increment(min_test_value, handystats::chrono::default_clock::now());
	ASSERT_EQ(sample_counter.value, min_test_value);

	for (int step = 0; step < max_test_value - min_test_value; ++step) {
		sample_counter.increment(1, handystats::chrono::default_clock::now());
	}
	ASSERT_EQ(sample_counter.value, max_test_value);

	for (int step = 0; step < max_test_value; ++step) {
		sample_counter.decrement(1, handystats::chrono::default_clock::now());
	}
	ASSERT_EQ(sample_counter.value, 0);

	std::cout << handystats::json::write_to_json_string<rapidjson::MemoryPoolAllocator<>>(&sample_counter) << std::endl;
}

TEST(CounterTest, TestCounterInternalStats) {
	counter sample_counter;

	const int min_test_value = 1E3;
	const int max_test_value = 1E4;

	sample_counter.increment(min_test_value, handystats::chrono::default_clock::now());
	sample_counter.increment(max_test_value - min_test_value, handystats::chrono::default_clock::now());

	ASSERT_EQ(sample_counter.incr_deltas.count(), 2);
	ASSERT_EQ(sample_counter.values.min(), 0);
	ASSERT_EQ(sample_counter.values.max(), max_test_value);

	for (int step = 0; step < max_test_value; ++step) {
		sample_counter.decrement(1, handystats::chrono::default_clock::now());
	}

	ASSERT_EQ(sample_counter.deltas.count(), 2 + max_test_value);
	ASSERT_EQ(sample_counter.values.min(), 0);
	ASSERT_EQ(sample_counter.decr_deltas.max(), 1);
	ASSERT_EQ(sample_counter.values.max(), max_test_value);

	std::cout << handystats::json::write_to_json_string<rapidjson::MemoryPoolAllocator<>>(&sample_counter) << std::endl;
}
