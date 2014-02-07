#include "handystats/events/event_message.hpp"

#include "handystats/events/counter_events.hpp"
#include "handystats/events/gauge_events.hpp"
#include "handystats/events/timer_events.hpp"

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
