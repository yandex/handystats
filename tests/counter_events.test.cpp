#include <memory>

#include <gtest/gtest.h>

#include <handystats/metrics/counter.hpp>
#include <handystats/events/event_message.hpp>
#include <handystats/events/counter_events.hpp>

namespace handystats { namespace events {

std::shared_ptr<event_message> counter_init_event(const std::string, metrics::counter::value_type, metrics::counter::time_point);
std::shared_ptr<event_message> counter_increment_event(const std::string, metrics::counter::value_type, metrics::counter::time_point);
std::shared_ptr<event_message> counter_decrement_event(const std::string, metrics::counter::value_type, metrics::counter::time_point);

}} // namespace handystats::events


using namespace handystats::events;

TEST(CounterEventsTest, TestCounterInitEvent) {
	const std::string counter_name = "queue.size";
	const handystats::metrics::counter::value_type init_value = 10;
	auto message = counter_init_event(counter_name, init_value, handystats::metrics::counter::clock::now());

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, event_destination_type::COUNTER);

	ASSERT_EQ(*static_cast<counter_event*>(message->event_type), counter_event::INIT);
	ASSERT_EQ(*static_cast<handystats::metrics::counter::value_type*>(message->event_data[0]), init_value);
}

TEST(CounterEventsTest, TestCounterIncrementEvent) {
	const std::string counter_name = "queue.size";
	const handystats::metrics::counter::value_type value = 2;
	auto message = counter_increment_event(counter_name, value, handystats::metrics::counter::clock::now());

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, event_destination_type::COUNTER);

	ASSERT_EQ(*static_cast<counter_event*>(message->event_type), counter_event::INCREMENT);
	ASSERT_EQ(*static_cast<handystats::metrics::counter::value_type*>(message->event_data[0]), value);
}

TEST(CounterEventsTest, TestCounterDecrementEvent) {
	const std::string counter_name = "queue.size";
	const handystats::metrics::counter::value_type value = -1;
	auto message = counter_decrement_event(counter_name, value, handystats::metrics::counter::clock::now());

	ASSERT_EQ(message->destination_name, counter_name);
	ASSERT_EQ(message->destination_type, event_destination_type::COUNTER);

	ASSERT_EQ(*static_cast<counter_event*>(message->event_type), counter_event::DECREMENT);
	ASSERT_EQ(*static_cast<handystats::metrics::counter::value_type*>(message->event_data[0]), value);
}

