#include <memory>

#include <gtest/gtest.h>

#include <handystats/metrics/counter.hpp>

#include "events/event_message_impl.hpp"
#include "events/counter_events_impl.hpp"


using namespace handystats::events::counter;

TEST(CounterEventsTest, TestCounterInitEvent) {
	const std::string counter_name = "queue.size";
	const handystats::metrics::counter::value_type init_value = 10;
	auto message = create_init_event(counter_name, init_value, handystats::metrics::counter::clock::now());

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::COUNTER);

	ASSERT_EQ(message->event_type, event_type::INIT);
	ASSERT_EQ(*static_cast<handystats::metrics::counter::value_type*>(message->event_data[0]), init_value);
}

TEST(CounterEventsTest, TestCounterIncrementEvent) {
	const std::string counter_name = "queue.size";
	const handystats::metrics::counter::value_type value = 2;
	auto message = create_increment_event(counter_name, value, handystats::metrics::counter::clock::now());

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::COUNTER);

	ASSERT_EQ(message->event_type, event_type::INCREMENT);
	ASSERT_EQ(*static_cast<handystats::metrics::counter::value_type*>(message->event_data[0]), value);
}

TEST(CounterEventsTest, TestCounterDecrementEvent) {
	const std::string counter_name = "queue.size";
	const handystats::metrics::counter::value_type value = -1;
	auto message = create_decrement_event(counter_name, value, handystats::metrics::counter::clock::now());

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::COUNTER);

	ASSERT_EQ(message->event_type, event_type::DECREMENT);
	ASSERT_EQ(*static_cast<handystats::metrics::counter::value_type*>(message->event_data[0]), value);
}

