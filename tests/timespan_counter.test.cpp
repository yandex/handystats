#include <iostream>
#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include <handystats/timespan_counter.hpp>

TEST(TimespanCounter, TimespanInitializedWithZero) {
	typedef handystats::timespan_counter<> timespan_counter;
	timespan_counter counter;

	EXPECT_EQ(counter.get_value().count(), 0);
	EXPECT_EQ(counter.get_running_time().count(), 0);
}

TEST(TimespanCounter, TimespanCountsAtLeastCorrectly) {
	typedef handystats::timespan_counter<std::chrono::milliseconds> timespan_counter;
	timespan_counter counter;

	const int SLEEP_MILLIS = 100;

	counter.start();
	std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_MILLIS));
	counter.stop();

	ASSERT_GE(counter.get_value().count(), SLEEP_MILLIS);
}

TEST(TimespanCounter, TimespanIsRunningWorksCorrectly) {
	handystats::timespan_counter<> counter;

	for (int step = 0; step < 3; ++step) {
		ASSERT_FALSE(counter.is_running());
		counter.start();
		ASSERT_TRUE(counter.is_running());
		counter.stop();
		ASSERT_FALSE(counter.is_running());
	}
}

TEST(TimespanCounter, TimespanLastSpanDurationCountsCorrectly) {
	typedef handystats::timespan_counter<std::chrono::milliseconds> timespan_counter;
	timespan_counter counter;

	const int MAX_SLEEP_MILLIS = 10;
	int summary_sleep = 0;

	counter.start();

	for (int sleep = 1; sleep < MAX_SLEEP_MILLIS; ++sleep) {
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
		summary_sleep += sleep;
		ASSERT_GE(counter.get_last_span_duration().count(), summary_sleep);
	}
}

TEST(TimespanCounter, TimespanAccumulatesSpans) {
	typedef handystats::timespan_counter<std::chrono::milliseconds> timespan_counter;
	timespan_counter counter;

	const int MAX_SLEEP_MILLIS = 10;
	int summary_sleep = 0;

	for (int sleep = 1; sleep < MAX_SLEEP_MILLIS; ++sleep) {
		counter.start();
		std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
		counter.stop();

		ASSERT_GE(counter.get_last_span_duration().count(), sleep);

		summary_sleep += sleep;
	}

	ASSERT_GE(counter.get_value().count(), summary_sleep);
}

TEST(TimespanCounter, TimespanTotalRunningTimeCollectsCorrectly) {
	typedef handystats::timespan_counter<std::chrono::milliseconds> timespan_counter;
	timespan_counter counter;

	const int MAX_STEPS = 5;

	int summary_sleep = 0;

	for (int step = 0; step < MAX_STEPS; ++step) {
		counter.start();

		ASSERT_GE(counter.get_running_time().count(), summary_sleep);

		const int MAX_SLEEP_MILLIS = 5;
		for (int sleep = 1; sleep < MAX_SLEEP_MILLIS; ++sleep) {
			std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
			summary_sleep += sleep;

			ASSERT_GE(counter.get_running_time().count(), summary_sleep);
		}

		counter.stop();
	}
}

