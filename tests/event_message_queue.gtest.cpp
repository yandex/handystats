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

#include <gtest/gtest.h>

#include "events/event_message_impl.hpp"
#include "core_impl.hpp"

#include <handystats/module.h>

#ifndef _HAVE_HANDY_MODULE_TEST
#define _HAVE_HANDY_MODULE_TEST 1
#endif
HANDY_MODULE(TEST)

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
	TEST_COUNTER_INIT("counter.name", 10);
	ASSERT_EQ(handystats::core->m_channel.size(), 1);

	TEST_COUNTER_INCREMENT("counter.name", 1);
	ASSERT_EQ(handystats::core->m_channel.size(), 2);

	TEST_COUNTER_DECREMENT("counter.name", 11);
	ASSERT_EQ(handystats::core->m_channel.size(), 3);

	TEST_COUNTER_CHANGE("counter.name", -10);
	ASSERT_EQ(handystats::core->m_channel.size(), 4);
}

TEST_F(EventMessageQueueTest, MultiplePushesCorrectlyAddMessages) {
	TEST_COUNTER_INIT("counter.name", 10);
	TEST_COUNTER_INCREMENT("counter.name", 1);
	TEST_COUNTER_DECREMENT("counter.name", 11);
	TEST_COUNTER_CHANGE("counter.name", 13);

	ASSERT_EQ(handystats::core->m_channel.size(), 4);
}

TEST_F(EventMessageQueueTest, PushDifferentMessages) {
	TEST_COUNTER_INIT("counter.name", 10);
	TEST_GAUGE_INIT("gauge.name", 10);
	TEST_TIMER_INIT("timer.name");

	ASSERT_EQ(handystats::core->m_channel.size(), 3);
}

