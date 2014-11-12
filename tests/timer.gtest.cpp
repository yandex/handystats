/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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

