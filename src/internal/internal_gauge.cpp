#include <handystats/internal/internal_gauge.hpp>
#include <handystats/events/gauge_events.hpp>

namespace handystats { namespace internal {

void internal_gauge::process_event_message(events::event_message* message) {
	if (!message) {
		return;
	}

	if (message->destination_type != events::event_destination_type::GAUGE) {
		return;
	}

	switch (*static_cast<events::gauge_event*>(message->event_type)) {
		case events::gauge_event::INIT:
			process_init_event(message);
			break;
		case events::gauge_event::SET:
			process_set_event(message);
			break;
		default:
			return;
	}

	timestamp = message->timestamp;
}


void internal_gauge::process_init_event(events::event_message* message) {
	if (base_gauge) {
		return;
	}

	base_gauge = new metrics::gauge(*static_cast<metrics::gauge::value_type*>(message->event_data[0]), message->timestamp);
}

void internal_gauge::process_set_event(events::event_message* message) {
	if (!base_gauge) {
		base_gauge = new metrics::gauge();
	}

	base_gauge->set(*static_cast<metrics::gauge::value_type*>(message->event_data[0]), message->timestamp);
}


}} // namespace handystats::internal
