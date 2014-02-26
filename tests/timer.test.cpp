#include <thread>

#include <gtest/gtest.h>

#include <handystats/chrono.hpp>
#include <handystats/metrics/timer.hpp>

#include <handystats/json/timer_json_writer.hpp>

using namespace handystats::metrics;

TEST(TimerTest, CheckTimerCountsRunningTimeCorrectly) {
	timer inter;
	inter.start(timer::clock::now());

	std::this_thread::sleep_for(handystats::chrono::default_duration(100));

	inter.stop(timer::clock::now());

	ASSERT_GT(std::chrono::duration_cast<handystats::chrono::default_duration>(inter.value).count(), 100);

	std::cout << handystats::json::write_to_json_string(&inter) << std::endl;
}

TEST(Timer, CheckNotStartedTimerHasNoDuration) {
	timer inter;

	std::this_thread::sleep_for(handystats::chrono::default_duration(100));

	ASSERT_EQ(std::chrono::duration_cast<handystats::chrono::default_duration>(inter.value).count(), 0);

	std::cout << handystats::json::write_to_json_string(&inter) << std::endl;
}

TEST(TimerTest, CheckNotStoppedTimerHasNoDuration) {
	timer inter;
	inter.start(timer::clock::now());

	std::this_thread::sleep_for(handystats::chrono::default_duration(100));

	ASSERT_EQ(std::chrono::duration_cast<handystats::chrono::default_duration>(inter.value).count(), 0);

	std::cout << handystats::json::write_to_json_string(&inter) << std::endl;
}

