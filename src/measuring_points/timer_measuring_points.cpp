#include <memory>

#include "events/event_message_impl.hpp"

#include <handystats/measuring_points/timer_measuring_points.hpp>


namespace handystats { namespace message_queue {

void push_event_message(std::shared_ptr<events::event_message> message);

}} // namespace handystats::message_queue


namespace handystats { namespace events {

std::shared_ptr<event_message> timer_init_event(const std::string, const uint64_t, metrics::timer::time_point);
std::shared_ptr<event_message> timer_start_event(const std::string, const uint64_t, metrics::timer::time_point);
std::shared_ptr<event_message> timer_stop_event(const std::string, const uint64_t, metrics::timer::time_point);
std::shared_ptr<event_message> timer_discard_event(const std::string, const uint64_t, metrics::timer::time_point);
std::shared_ptr<event_message> timer_heartbeat_event(const std::string, const uint64_t, metrics::timer::time_point);

}} // namespace handystats::events


void HANDY_TIMER_INIT(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	auto message = handystats::events::timer_init_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

void HANDY_TIMER_START(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	auto message = handystats::events::timer_start_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

void HANDY_TIMER_STOP(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	auto message = handystats::events::timer_stop_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

void HANDY_TIMER_DISCARD(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	auto message = handystats::events::timer_discard_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}

void HANDY_TIMER_HEARTBEAT(
		const std::string timer_name,
		const uint64_t instance_id,
		handystats::metrics::timer::time_point timestamp
		)
{
	auto message = handystats::events::timer_heartbeat_event(timer_name, instance_id, timestamp);
	handystats::message_queue::push_event_message(message);
}


