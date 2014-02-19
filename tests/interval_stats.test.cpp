#include <thread>

#include <gtest/gtest.h>

#include <handystats/accumulators.hpp>
#include <handystats/chrono.hpp>

using namespace boost::accumulators;

TEST(IntervalStatsTest, TestIntervalCount) {
	accumulator_set<double, features<tag::interval_count>> acc(
			time_interval = std::chrono::milliseconds(1)
			);

	ASSERT_NEAR(interval_count(acc), 0.0, 1E-8);

	const int STEP_COUNT = 50;

	for (int cycle = 0; cycle < 10; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		for (int step = 0; step < STEP_COUNT; ++step) {
			acc(step, timestamp = handystats::chrono::default_clock::now());
		}

		ASSERT_NEAR(interval_count(acc), STEP_COUNT, 0.05 * STEP_COUNT);
	}
}

TEST(IntervalStatsTest, TestIntervalSum) {
	accumulator_set<double, features<tag::interval_sum>> acc(
			time_interval = std::chrono::milliseconds(1)
			);

	ASSERT_NEAR(interval_sum(acc), 0.0, 1E-8);

	const int STEP_COUNT = 50;

	for (int cycle = 0; cycle < 10; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		for (int step = 0; step < STEP_COUNT; ++step) {
			acc(1, timestamp = handystats::chrono::default_clock::now());
		}

		ASSERT_NEAR(interval_sum(acc), STEP_COUNT, 0.05 * STEP_COUNT);
	}
}

TEST(IntervalStatsTest, TestIntervalMean) {
	accumulator_set<double, features<tag::interval_mean>> acc(
			time_interval = std::chrono::milliseconds(1)
			);

	ASSERT_NEAR(interval_mean(acc), 0.0, 1E-8);

	const int STEP_COUNT = 50;

	for (int cycle = 0; cycle < 10; ++cycle) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		accumulator_set<double, features<tag::mean>> mean_acc;

		for (int step = 0; step < STEP_COUNT; ++step) {
			auto value = rand() % 1000;
			acc(value, timestamp = handystats::chrono::default_clock::now());
			mean_acc(value);
		}

		ASSERT_NEAR(interval_mean(acc), mean(mean_acc), 0.05 * mean(mean_acc));
	}
}
