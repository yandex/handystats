#include <iostream>
#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include <handystats/monotonic_counter.hpp>

TEST(MonotonicCounter, MonotonicIncidentCountsCorrectly) {
	handystats::monotonic_counter<> counter;

	const int EVENTS_COUNT = 100000;

	for (int i = 0; i < EVENTS_COUNT; ++i) {
		counter.incident();
	}

	ASSERT_EQ(counter.get_value(), EVENTS_COUNT);
}

TEST(MonotonicCounter, MonotonicIncrementCountsCorrectly) {
	handystats::monotonic_counter<int64_t> counter;

	const int64_t EVENTS_COUNT = 1000000;

	for (int i = 0; i < EVENTS_COUNT; ++i) {
		counter.increment(EVENTS_COUNT);
	}

	ASSERT_EQ(counter.get_value(), EVENTS_COUNT * EVENTS_COUNT);
}

TEST(MonotonicCounter, MonotonicResetsCorrectly) {
	handystats::monotonic_counter<> counter;

	const int TESTS_COUNT = 100;

	for (int test = 0; test < TESTS_COUNT; ++test) {
		counter.reset();

		const int EVENTS_COUNT = test + 1;
		for (int event = 0; event < EVENTS_COUNT; ++event) {
			counter.incident();
		}

		ASSERT_EQ(counter.get_value(), EVENTS_COUNT);
	}
}

TEST(MonotonicCounter, MonotonicActiveTimeWorksAtLeastCorrectly) {
	handystats::monotonic_counter<int, std::chrono::milliseconds> counter;

	const int SLEEP_MILLIS = 100;

	counter.reset();
	std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MILLIS));
	counter.incident();

	ASSERT_GE(handystats::chrono::to_milliseconds(counter.get_active_time()).count(),
			SLEEP_MILLIS);
}
