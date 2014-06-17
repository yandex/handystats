// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include "events/event_message_impl.hpp"
#include "events/gauge_events_impl.hpp"

#include "internal_metrics/internal_gauge_impl.hpp"


namespace handystats { namespace internal {

void internal_gauge::process_event_message(const events::event_message& message) {
	if (message.destination_type != events::event_destination_type::GAUGE) {
		return;
	}

	switch (message.event_type) {
		case events::gauge_event::INIT:
			process_init_event(message);
			break;
		case events::gauge_event::SET:
			process_set_event(message);
			break;
		default:
			return;
	}

	timestamp = message.timestamp;
}


void internal_gauge::process_init_event(const events::event_message& message) {
	if (base_gauge) {
		return;
	}

	base_gauge = new metrics::gauge(*static_cast<metrics::gauge::value_type*>(message.event_data[0]), message.timestamp);
}

void internal_gauge::process_set_event(const events::event_message& message) {
	if (!base_gauge) {
		base_gauge = new metrics::gauge();
	}

	base_gauge->set(*static_cast<metrics::gauge::value_type*>(message.event_data[0]), message.timestamp);
}


}} // namespace handystats::internal
