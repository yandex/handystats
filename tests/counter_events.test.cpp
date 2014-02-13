#include <gtest/gtest.h>

#include "handystats/events/counter_events.hpp"

using namespace handystats::events;

TEST(CounterEventsTest, TestCounterInitEvent) {
	const std::string counter_name = "queue.size";
	const handystats::metrics::counter::value_type init_value = 10;
	event_message* message = counter_init_event(counter_name, init_value);

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, event_destination_type::COUNTER);

	ASSERT_EQ(*static_cast<counter_event*>(message->event_type), counter_event::INIT);
	ASSERT_EQ(*static_cast<handystats::metrics::counter::value_type*>(message->event_data[0]), init_value);

	delete_counter_event(message);
}

TEST(CounterEventsTest, TestCounterIncrementEvent) {
	const std::string counter_name = "queue.size";
	const handystats::metrics::counter::value_type value = 2;
	event_message* message = counter_increment_event(counter_name, value);

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, event_destination_type::COUNTER);

	ASSERT_EQ(*static_cast<counter_event*>(message->event_type), counter_event::INCREMENT);
	ASSERT_EQ(*static_cast<handystats::metrics::counter::value_type*>(message->event_data[0]), value);

	delete_counter_event(message);
}

TEST(CounterEventsTest, TestCounterDecrementEvent) {
	const std::string counter_name = "queue.size";
	const handystats::metrics::counter::value_type value = -1;
	event_message* message = counter_decrement_event(counter_name, value);

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, event_destination_type::COUNTER);

	ASSERT_EQ(*static_cast<counter_event*>(message->event_type), counter_event::DECREMENT);
	ASSERT_EQ(*static_cast<handystats::metrics::counter::value_type*>(message->event_data[0]), value);

	delete_counter_event(message);
}

