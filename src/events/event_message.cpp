#include "handystats/events/event_message.hpp"

#include "handystats/events/counter_events.hpp"
#include "handystats/events/gauge_events.hpp"
#include "handystats/events/transaction_events.hpp"

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
		case event_destination_type::TRANSACTION:
			delete_transaction_event(message);
			break;
		default:
			return;
	}
}

}} // namespace handystats::events
