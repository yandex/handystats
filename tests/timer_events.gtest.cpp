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
	handystats::chrono::duration duration = handystats::chrono::nanoseconds(123456);
	auto message = create_set_event(timer_name, duration, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::TIMER);

	ASSERT_EQ(message->event_type, event_type::SET);
	ASSERT_EQ(reinterpret_cast<int64_t>(message->event_data), duration.count());

	delete_event_message(message);
}

