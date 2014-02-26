#include <handystats/events/event_message.hpp>
#include <handystats/events/counter_events.hpp>

#include "internal_metrics/internal_counter_impl.hpp"


namespace handystats { namespace internal {

void internal_counter::process_event_message(const events::event_message& message) {
	if (message.destination_type != events::event_destination_type::COUNTER) {
		return;
	}

	switch (*static_cast<events::counter_event*>(message.event_type)) {
		case events::counter_event::INIT:
			process_init_event(message);
			break;
		case events::counter_event::INCREMENT:
			process_increment_event(message);
			break;
		case events::counter_event::DECREMENT:
			process_decrement_event(message);
			break;
		default:
			return;
	}

	timestamp = message.timestamp;
}


void internal_counter::process_init_event(const events::event_message& message) {
	if (base_counter) {
		return;
	}

	base_counter = new metrics::counter(*static_cast<metrics::counter::value_type*>(message.event_data[0]), message.timestamp);
}

void internal_counter::process_increment_event(const events::event_message& message) {
	if (!base_counter) {
		base_counter = new metrics::counter();
	}

	base_counter->increment(*static_cast<metrics::counter::value_type*>(message.event_data[0]), message.timestamp);
}

void internal_counter::process_decrement_event(const events::event_message& message) {
	if (!base_counter) {
		base_counter = new metrics::counter();
	}

	base_counter->decrement(*static_cast<metrics::counter::value_type*>(message.event_data[0]), message.timestamp);
}


}} // namespace handystats::internal
