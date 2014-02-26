#include "events/event_message_impl.hpp"
#include "events/timer_events_impl.hpp"

#include "internal_metrics/internal_timer_impl.hpp"


namespace handystats { namespace internal {

const internal_timer::time_duration internal_timer::TIMEOUT = std::chrono::duration_cast<internal_timer::time_duration>(std::chrono::seconds(5));

void internal_timer::check_on_timeout(time_point timestamp) {
	for (auto instance_iter = instances.begin(); instance_iter != instances.end();) {
		if (std::chrono::duration_cast<internal_timer::time_duration>(timestamp - instance_iter->second.timestamp) > TIMEOUT) {
			instance_iter = instances.erase(instance_iter);
		}
		else {
			++instance_iter;
		}
	}

	check_timestamp = timestamp;
}

void internal_timer::process_event_message(const events::event_message& message) {
	if (message.destination_type != events::event_destination_type::TIMER) {
		return;
	}

	switch (*static_cast<events::timer_event*>(message.event_type)) {
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

	if (std::chrono::duration_cast<internal_timer::time_duration>(timestamp - check_timestamp) > TIMEOUT) {
		check_on_timeout(timestamp);
	}
}

void internal_timer::process_init_event(const events::event_message& message) {
	uint64_t instance_id = *static_cast<uint64_t*>(message.event_data[0]);

	auto& instance = instances[instance_id];
	if (!instance.timer) {
		instance.timer = new metrics::timer();
		instance.timestamp = message.timestamp;
	}
}

void internal_timer::process_start_event(const events::event_message& message) {
	uint64_t instance_id = *static_cast<uint64_t*>(message.event_data[0]);

	auto& instance = instances[instance_id];
	if (!instance.timer) {
		instance.timer = new metrics::timer(message.timestamp);
		instance.timestamp = message.timestamp;
	}
	else {
		instance.timer->start(message.timestamp);
		instance.timestamp = message.timestamp;
	}
}

void internal_timer::process_stop_event(const events::event_message& message) {
	uint64_t instance_id = *static_cast<uint64_t*>(message.event_data[0]);

	auto instance_iter = instances.find(instance_id);
	if (instance_iter == instances.end()) {
		return;
	}
	if (!instance_iter->second.timer) {
		return;
	}

	auto& instance = instance_iter->second;
	instance.timer->stop(message.timestamp);

	double timer_duration = instance.timer->get().first.count();
	aggregator.set(timer_duration, message.timestamp);

	instances.erase(instance_iter);
}

void internal_timer::process_discard_event(const events::event_message& message) {
	uint64_t instance_id = *static_cast<uint64_t*>(message.event_data[0]);
	instances.erase(instance_id);
}

void internal_timer::process_heartbeat_event(const events::event_message& message) {
	uint64_t instance_id = *static_cast<uint64_t*>(message.event_data[0]);

	auto instance_iter = instances.find(instance_id);
	if (instance_iter == instances.end()) {
		return;
	}
	else {
		instance_iter->second.timestamp = message.timestamp;
	}
}


}} // namespace handystats::internal

