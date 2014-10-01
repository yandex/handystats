#include <thread>
#include <chrono>

#include <gtest/gtest.h>

#include <handystats/rapidjson/allocators.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/timer.hpp>

#include <handystats/json/timer_json_writer.hpp>

using namespace handystats::metrics;

TEST(TimerTest, CheckTimerCountsRunningTimeCorrectly) {
	const auto& sleep_time = std::chrono::microseconds(100);
	timer inter;
	inter.start();

	std::this_thread::sleep_for(sleep_time);

	inter.stop();

	ASSERT_GT(inter.values().get<handystats::statistics::tag::value>(),
			handystats::chrono::duration::convert_to(handystats::metrics::timer::value_unit,
				handystats::chrono::duration(sleep_time.count(), handystats::chrono::time_unit::USEC)).count()
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

