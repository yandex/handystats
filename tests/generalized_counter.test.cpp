#include <iostream>
#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include <handystats/generalized_counter.hpp>

TEST(GeneralizedCounter, GeneralizedDefaultIncrementCountsCorrectly) {
	handystats::generalized_counter<> counter;

	const int EVENTS_COUNT = 100000;

	for (int i = 0; i < EVENTS_COUNT; ++i) {
		counter.increment();
	}

	ASSERT_EQ(counter.get_value(), EVENTS_COUNT);
}

TEST(GeneralizedCounter, GeneralizedIncrementCountsCorrectly) {
	handystats::generalized_counter<int64_t> counter;

	const int64_t EVENTS_COUNT = 1000000;

	for (int i = 0; i < EVENTS_COUNT; ++i) {
		counter.increment(EVENTS_COUNT);
	}

	ASSERT_EQ(counter.get_value(), EVENTS_COUNT * EVENTS_COUNT);
}

TEST(GeneralizedCounter, GeneralizedResetsCorrectly) {
	handystats::generalized_counter<> counter;

	const int TESTS_COUNT = 10;

	for (int test = 0; test < TESTS_COUNT; ++test) {
		counter.reset();
		counter.increment(test + 1);
		ASSERT_EQ(counter.get_value(), test + 1);
	}
}

TEST(GeneralizedCounter, GeneralizedDecrementCountsCorrectly) {
	handystats::generalized_counter<int64_t> counter;

	const int EVENTS_COUNT = 1000;

	for (int event = 0; event < EVENTS_COUNT; ++event) {
		counter.increment();
	}
	counter.decrement(EVENTS_COUNT);

	ASSERT_EQ(counter.get_value(), 0);

	counter.increment(EVENTS_COUNT);
	for (int event = 0; event < EVENTS_COUNT; ++event) {
		counter.decrement();
	}

	ASSERT_EQ(counter.get_value(), 0);
}

TEST(GeneralizedCounter, GeneralizedSetValueWorksCorrectly) {
	handystats::generalized_counter<> counter;

	const int TESTS_COUNT = 10;

	for (int test = 0; test < TESTS_COUNT; ++test) {
		counter.set_value(test);
		ASSERT_EQ(counter.get_value(), test);
	}
}

TEST(GeneralizedCounter, GeneralizedWorksWithDoubleCorrectly) {
	handystats::generalized_counter<double> counter;

	const int TESTS_COUNT = 10;

	for (int test = 0; test < TESTS_COUNT; ++test) {
		counter.set_value(test);
		const int STEPS_DOWN = 13;
		const double dec_value = double(test) / STEPS_DOWN;
		for (int step = 0; step < STEPS_DOWN; ++step) {
			counter.decrement(dec_value);
		}
		ASSERT_NEAR(counter.get_value(), 0.0, 1e-9);
	}
}
