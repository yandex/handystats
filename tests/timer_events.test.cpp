#include <memory>
#include <cstdint>

#include <gtest/gtest.h>

#include <handystats/metrics/timer.hpp>

#include "events/event_message_impl.hpp"
#include "events/timer_events_impl.hpp"


namespace handystats { namespace events {

std::shared_ptr<event_message> timer_init_event(const std::string, const uint64_t, metrics::timer::time_point);
std::shared_ptr<event_message> timer_start_event(const std::string, const uint64_t, metrics::timer::time_point);
std::shared_ptr<event_message> timer_stop_event(const std::string, const uint64_t, metrics::timer::time_point);
std::shared_ptr<event_message> timer_discard_event(const std::string, const uint64_t, metrics::timer::time_point);
std::shared_ptr<event_message> timer_heartbeat_event(const std::string, const uint64_t, metrics::timer::time_point);

}} // namespace handystats::events


using namespace handystats::events;

TEST(TimerEventsTest, TestTimerInitEventWithDefaultInstance) {
	const std::string timer_name = "queue.push";
	auto message = timer_init_event(timer_name, -1, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TIMER);

	ASSERT_EQ(*static_cast<timer_event*>(message->event_type), timer_event::INIT);
	ASSERT_EQ(*static_cast<uint64_t*>(message->event_data[0]), uint64_t(-1));
}

TEST(TimerEventsTest, TestTimerInitEventWithSpecificInstance) {
	const std::string timer_name = "queue.push";
	const uint64_t instance_id = 123;
	auto message = timer_init_event(timer_name, instance_id, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TIMER);

	ASSERT_EQ(*static_cast<timer_event*>(message->event_type), timer_event::INIT);
	ASSERT_EQ(*static_cast<uint64_t*>(message->event_data[0]), instance_id);
}

TEST(TimerEventsTest, TestTimerStartEvent) {
	const std::string timer_name = "queue.push";
	const uint64_t instance_id = 1234567890;
	auto message = timer_start_event(timer_name, instance_id, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TIMER);

	ASSERT_EQ(*static_cast<timer_event*>(message->event_type), timer_event::START);
	ASSERT_EQ(*static_cast<uint64_t*>(message->event_data[0]), instance_id);
}

TEST(TimerEventsTest, TestTimerStopEvent) {
	const std::string timer_name = "queue.push";
	const uint64_t instance_id = 1234567890;
	auto message = timer_stop_event(timer_name, instance_id, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TIMER);

	ASSERT_EQ(*static_cast<timer_event*>(message->event_type), timer_event::STOP);
	ASSERT_EQ(*static_cast<uint64_t*>(message->event_data[0]), instance_id);
}

TEST(TimerEventsTest, TestTimerDiscardEvent) {
	const std::string timer_name = "queue.push";
	const uint64_t instance_id = 1234567890;
	auto message = timer_discard_event(timer_name, instance_id, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TIMER);

	ASSERT_EQ(*static_cast<timer_event*>(message->event_type), timer_event::DISCARD);
	ASSERT_EQ(*static_cast<uint64_t*>(message->event_data[0]), instance_id);
}

TEST(TimerEventsTest, TestTimerHeartbeatEvent) {
	const std::string timer_name = "queue.push";
	const uint64_t instance_id = 1234567890;
	auto message = timer_heartbeat_event(timer_name, instance_id, handystats::metrics::timer::clock::now());

	ASSERT_EQ(message->destination_name, timer_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TIMER);

	ASSERT_EQ(*static_cast<timer_event*>(message->event_type), timer_event::HEARTBEAT);
	ASSERT_EQ(*static_cast<uint64_t*>(message->event_data[0]), instance_id);
}


