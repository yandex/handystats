#ifndef HANDYSTATS_INTERNAL_H_
#define HANDYSTATS_INTERNAL_H_

#include <string>
#include <unordered_map>

#include <boost/variant.hpp>

#include "handystats/events/event_message.hpp"
#include "handystats/internal/internal_counter.hpp"
#include "handystats/internal/internal_gauge.hpp"

namespace handystats { namespace internal {

typedef boost::variant <
		internal_counter*,
		internal_gauge*
	> internal_monitor;

enum internal_monitor_index {
	INTERNAL_COUNTER = 0,
	INTERNAL_GAUGE
};

static std::unordered_map<std::string, internal_monitor> monitors;

void process_event_message(events::event_message* message);
void process_event_message(events::event_message* message, internal_monitor& monitor);

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_H_
