#include "handystats/events/event_message.hpp"

namespace handystats { namespace events {

void delete_gauge_event(event_message*);
void delete_counter_event(event_message*);
void delete_timer_event(event_message*);

}} // namespace handystats::events


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
