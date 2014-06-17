// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include "events/gauge_events_impl.hpp"
#include "events/counter_events_impl.hpp"
#include "events/timer_events_impl.hpp"

#include "events/event_message_impl.hpp"


namespace handystats { namespace events {

void delete_event_message(event_message* message) {
	if (!message) {
		return;
	}

	switch (message->destination_type) {
		case event_destination_type::COUNTER:
			delete_counter_event(message);
			break;
		case event_destination_type::GAUGE:
			delete_gauge_event(message);
			break;
		case event_destination_type::TIMER:
			delete_timer_event(message);
			break;
		default:
			return;
	}
}

}} // namespace handystats::events
