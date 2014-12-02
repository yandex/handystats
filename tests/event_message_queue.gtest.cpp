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

#include <chrono>
#include <thread>
#include <memory>
#include <handystats/atomic.hpp>

#include <handystats/measuring_points.hpp>

#include <gtest/gtest.h>

#include "events/event_message_impl.hpp"
#include "core_impl.hpp"

class EventMessageQueueTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		handystats::core.reset(new handystats::core_t());
	}
	virtual void TearDown() {
		handystats::core.reset();
	}
};


TEST_F(EventMessageQueueTest, SinglePushCorrectlyAddsMessage) {
	HANDY_COUNTER_INIT("counter.name", 10);
	ASSERT_EQ(handystats::core->m_channel.size(), 1);

	HANDY_COUNTER_INCREMENT("counter.name", 1);
	ASSERT_EQ(handystats::core->m_channel.size(), 2);

	HANDY_COUNTER_DECREMENT("counter.name", 11);
	ASSERT_EQ(handystats::core->m_channel.size(), 3);

	HANDY_COUNTER_CHANGE("counter.name", -10);
	ASSERT_EQ(handystats::core->m_channel.size(), 4);
}

TEST_F(EventMessageQueueTest, MultiplePushesCorrectlyAddMessages) {
	HANDY_COUNTER_INIT("counter.name", 10);
	HANDY_COUNTER_INCREMENT("counter.name", 1);
	HANDY_COUNTER_DECREMENT("counter.name", 11);
	HANDY_COUNTER_CHANGE("counter.name", 13);

	ASSERT_EQ(handystats::core->m_channel.size(), 4);
}

TEST_F(EventMessageQueueTest, PushDifferentMessages) {
	HANDY_COUNTER_INIT("counter.name", 10);
	HANDY_GAUGE_INIT("gauge.name", 10);
	HANDY_TIMER_INIT("timer.name");

	ASSERT_EQ(handystats::core->m_channel.size(), 3);
}

