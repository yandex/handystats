#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/timer.hpp>

using namespace handystats::metrics;

TEST(TimerTest, CheckTimerCountsRunningTimeCorrectly) {
	const auto& sleep_time = std::chrono::microseconds(100);
	timer inter;
	inter.start();

	std::this_thread::sleep_for(sleep_time);

	inter.stop();

	ASSERT_GT(inter.values().get<handystats::statistics::tag::value>(),
			handystats::chrono::microseconds(sleep_time.count()).count(handystats::metrics::timer::value_unit)
		);
}

TEST(Timer, CheckNotStartedTimerHasNoDuration) {
	const auto& sleep_time = std::chrono::microseconds(100);
	timer inter;

	std::this_thread::sleep_for(sleep_time);

	ASSERT_EQ(inter.values().get<handystats::statistics::tag::value>(), 0);
}

TEST(TimerTest, CheckNotStoppedTimerHasNoDuration) {
	const auto& sleep_time = std::chrono::microseconds(100);
	timer inter;
	inter.start();

	std::this_thread::sleep_for(sleep_time);

	ASSERT_EQ(inter.values().get<handystats::statistics::tag::value>(), 0);
}

