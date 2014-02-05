#include "handystats/internal.hpp"

namespace handystats { namespace internal {

void process_event_message(events::event_message* message) {
	if (!message) {
		return;
	}

	auto& monitor = monitors[message->destination_name];
	process_event_message(message, monitor);
}

void process_event_message(events::event_message* message, internal_monitor& monitor) {
	switch (monitor.which()) {
		case internal_monitor_index::INTERNAL_COUNTER:
			boost::get<internal_counter*>(monitor)->process_event_message(message);
			break;
		case internal_monitor_index::INTERNAL_GAUGE:
			boost::get<internal_gauge*>(monitor)->process_event_message(message);
			break;
		default:
			return;
	}
}


}} // namespace handystats::internal
