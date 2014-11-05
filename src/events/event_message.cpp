// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include "events/gauge_impl.hpp"
#include "events/counter_impl.hpp"
#include "events/timer_impl.hpp"
#include "events/attribute_impl.hpp"

#include "events/event_message_impl.hpp"


namespace handystats { namespace events {

void process_event_message(metrics::metric_ptr_variant& metric_ptr, const event_message& message) {
	switch (metric_ptr.which()) {
		case metrics::metric_index::COUNTER:
			counter::process_event(*boost::get<metrics::counter*>(metric_ptr), message);
			break;
		case metrics::metric_index::GAUGE:
			gauge::process_event(*boost::get<metrics::gauge*>(metric_ptr), message);
			break;
		case metrics::metric_index::TIMER:
			timer::process_event(*boost::get<metrics::timer*>(metric_ptr), message);
			break;
		default:
			return;
	}
}

void process_event_message(::handystats::attribute& attr, const event_message& message) {
	attribute::process_event(attr, message);
}


void delete_event_message(event_message* message) {
	if (!message) {
		return;
	}

	switch (message->destination_type) {
		case event_destination_type::COUNTER:
			counter::delete_event(message);
			break;
		case event_destination_type::GAUGE:
			gauge::delete_event(message);
			break;
		case event_destination_type::TIMER:
			timer::delete_event(message);
			break;
		case event_destination_type::ATTRIBUTE:
			attribute::delete_event(message);
			break;
		default:
			return;
	}
}

}} // namespace handystats::events
