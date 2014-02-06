#include "handystats/internal.hpp"

namespace handystats { namespace internal {

std::unordered_map<std::string, internal_monitor> monitors;


void process_event_message(events::event_message* message) {
	if (!message) {
		return;
	}

	if (monitors.find(message->destination_name) == monitors.end()) {
		switch (message->destination_type) {
			case events::event_destination_type::COUNTER:
				monitors.emplace(message->destination_name, new internal_counter());
				break;
			case events::event_destination_type::GAUGE:
				monitors.emplace(message->destination_name, new internal_gauge());
				break;
			default:
				return;
		}
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


void initialize() {
}

void clean_up() {
	monitors.clear();
}

}} // namespace handystats::internal
