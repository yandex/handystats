#include "events/event_message_impl.hpp"
#include "events/timer_events_impl.hpp"

#include "configuration_impl.hpp"

#include "internal_metrics/internal_timer_impl.hpp"


namespace handystats { namespace internal {

void internal_timer::check_timeout(time_point timestamp, clock::duration idle_timeout) {
	if (!base_timer) {
		return;
	}

	if (std::chrono::duration_cast<internal_timer::time_duration>(timestamp - check_timeout_timestamp) <
			std::chrono::duration_cast<internal_timer::time_duration>(idle_timeout))
	{
		return;
	}

	base_timer->check_timeout(timestamp, idle_timeout);

	check_timeout_timestamp = timestamp;
}

void internal_timer::process_event_message(const events::event_message& message) {
	if (message.destination_type != events::event_destination_type::TIMER) {
		return;
	}

	check_timeout(message.timestamp, std::chrono::duration_cast<clock::duration>(config::timer.idle_timeout));

	switch (message.event_type) {
		case events::timer_event::INIT:
			process_init_event(message);
			break;
		case events::timer_event::START:
			process_start_event(message);
			break;
		case events::timer_event::STOP:
			process_stop_event(message);
			break;
		case events::timer_event::DISCARD:
			process_discard_event(message);
			break;
		case events::timer_event::HEARTBEAT:
			process_heartbeat_event(message);
			break;
		default:
			return;
	}

	timestamp = message.timestamp;
}

void internal_timer::process_init_event(const events::event_message& message) {
	if (base_timer) {
		return;
	}

	base_timer = new metrics::timer();
}

void internal_timer::process_start_event(const events::event_message& message) {
	if (!base_timer) {
		base_timer = new metrics::timer();
	}

	metrics::timer::instance_id_type instance_id = *static_cast<metrics::timer::instance_id_type*>(message.event_data[0]);
	base_timer->start(message.timestamp, instance_id);

}

void internal_timer::process_stop_event(const events::event_message& message) {
	if (!base_timer) {
		return;
	}

	metrics::timer::instance_id_type instance_id = *static_cast<metrics::timer::instance_id_type*>(message.event_data[0]);
	base_timer->stop(message.timestamp, instance_id);
}

void internal_timer::process_discard_event(const events::event_message& message) {
	if (!base_timer) {
		return;
	}

	metrics::timer::instance_id_type instance_id = *static_cast<metrics::timer::instance_id_type*>(message.event_data[0]);
	base_timer->discard(message.timestamp, instance_id);
}

void internal_timer::process_heartbeat_event(const events::event_message& message) {
	if (!base_timer) {
		return;
	}

	metrics::timer::instance_id_type instance_id = *static_cast<metrics::timer::instance_id_type*>(message.event_data[0]);
	base_timer->heartbeat(message.timestamp, instance_id);
}

}} // namespace handystats::internal

