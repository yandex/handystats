// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include "events/gauge_events_impl.hpp"
#include "events/counter_events_impl.hpp"
#include "events/timer_events_impl.hpp"
#include "events/attribute_events_impl.hpp"

#include "events/event_message_impl.hpp"


namespace handystats { namespace events {

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
