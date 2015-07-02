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

#include <gtest/gtest.h>

#include <handystats/metrics/counter.hpp>

#include "events/event_message_impl.hpp"
#include "events/counter_impl.hpp"


using namespace handystats::events::counter;

TEST(CounterEventsTest, TestCounterInitEvent) {
	const char* counter_name = "queue.size";
	const handystats::metrics::counter::value_type init_value = 10;
	auto message = create_init_event(counter_name, init_value, handystats::metrics::counter::clock::now());

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::COUNTER);

	ASSERT_EQ(message->event_type, event_type::INIT);
	ASSERT_EQ(reinterpret_cast<handystats::metrics::counter::value_type>(message->event_data), init_value);

	delete_event_message(message);
}

TEST(CounterEventsTest, TestCounterIncrementEvent) {
	const char* counter_name = "queue.size";
	const handystats::metrics::counter::value_type value = 2;
	auto message = create_increment_event(counter_name, value, handystats::metrics::counter::clock::now());

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::COUNTER);

	ASSERT_EQ(message->event_type, event_type::INCREMENT);
	ASSERT_EQ(reinterpret_cast<handystats::metrics::counter::value_type>(message->event_data), value);

	delete_event_message(message);
}

TEST(CounterEventsTest, TestCounterDecrementEvent) {
	const char* counter_name = "queue.size";
	const handystats::metrics::counter::value_type value = -1;
	auto message = create_decrement_event(counter_name, value, handystats::metrics::counter::clock::now());

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::COUNTER);

	ASSERT_EQ(message->event_type, event_type::DECREMENT);
	ASSERT_EQ(reinterpret_cast<handystats::metrics::counter::value_type>(message->event_data), value);

	delete_event_message(message);
}

