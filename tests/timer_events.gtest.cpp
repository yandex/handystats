/*
 * Copyright (c) YANDEX LLC. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 */

#include <memory>
#include <cstdint>

#include <gtest/gtest.h>

#include <handystats/metrics/timer.hpp>

#include "events/event_message_impl.hpp"
#include "events/timer_impl.hpp"


using namespace handystats::events::timer;

TEST(TimerEventsTest, TestTimerInitEventWithDefaultInstance) {
	const char* timer_name = "queue.push";
	auto message = create_init_event(timer_name, -1, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::TIMER);

	ASSERT_EQ(message->event_type, event_type::INIT);
	ASSERT_EQ(reinterpret_cast<handystats::metrics::timer::instance_id_type>(message->event_data), handystats::metrics::timer::instance_id_type(-1));

	delete_event_message(message);
}

TEST(TimerEventsTest, TestTimerInitEventWithSpecificInstance) {
	const char* timer_name = "queue.push";
	const handystats::metrics::timer::instance_id_type instance_id = 123;
	auto message = create_init_event(timer_name, instance_id, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::TIMER);

	ASSERT_EQ(message->event_type, event_type::INIT);
	ASSERT_EQ(reinterpret_cast<handystats::metrics::timer::instance_id_type>(message->event_data), instance_id);

	delete_event_message(message);
}

TEST(TimerEventsTest, TestTimerStartEvent) {
	const char* timer_name = "queue.push";
	const handystats::metrics::timer::instance_id_type instance_id = 1234567890;
	auto message = create_start_event(timer_name, instance_id, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::TIMER);

	ASSERT_EQ(message->event_type, event_type::START);
	ASSERT_EQ(reinterpret_cast<handystats::metrics::timer::instance_id_type>(message->event_data), instance_id);

	delete_event_message(message);
}

TEST(TimerEventsTest, TestTimerStopEvent) {
	const char* timer_name = "queue.push";
	const handystats::metrics::timer::instance_id_type instance_id = 1234567890;
	auto message = create_stop_event(timer_name, instance_id, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::TIMER);

	ASSERT_EQ(message->event_type, event_type::STOP);
	ASSERT_EQ(reinterpret_cast<handystats::metrics::timer::instance_id_type>(message->event_data), instance_id);

	delete_event_message(message);
}

TEST(TimerEventsTest, TestTimerDiscardEvent) {
	const char* timer_name = "queue.push";
	const handystats::metrics::timer::instance_id_type instance_id = 1234567890;
	auto message = create_discard_event(timer_name, instance_id, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::TIMER);

	ASSERT_EQ(message->event_type, event_type::DISCARD);
	ASSERT_EQ(reinterpret_cast<handystats::metrics::timer::instance_id_type>(message->event_data), instance_id);

	delete_event_message(message);
}

TEST(TimerEventsTest, TestTimerHeartbeatEvent) {
	const char* timer_name = "queue.push";
	const handystats::metrics::timer::instance_id_type instance_id = 1234567890;
	auto message = create_heartbeat_event(timer_name, instance_id, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::TIMER);

	ASSERT_EQ(message->event_type, event_type::HEARTBEAT);
	ASSERT_EQ(reinterpret_cast<handystats::metrics::timer::instance_id_type>(message->event_data), instance_id);

	delete_event_message(message);
}

TEST(TimerEventsTest, TestTimerSetEvent) {
	const char* timer_name = "queue.push";
	handystats::chrono::duration duration(123456, handystats::chrono::time_unit::NSEC);
	auto message = create_set_event(timer_name, duration, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::TIMER);

	ASSERT_EQ(message->event_type, event_type::SET);
	ASSERT_EQ(reinterpret_cast<int64_t>(message->event_data),
			handystats::chrono::duration::convert_to(
				handystats::metrics::timer::value_unit,
				duration
			).count()
		);

	delete_event_message(message);
}

