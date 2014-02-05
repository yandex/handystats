#ifndef HANDYSTATS_INTERNAL_H_
#define HANDYSTATS_INTERNAL_H_

#include <string>
#include <unordered_map>

#include <boost/variant.hpp>
#include <tbb/concurrent_queue.h>

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

extern std::unordered_map<std::string, internal_monitor> monitors;

extern tbb::concurrent_queue<events::event_message*> event_message_queue;

void process_event_message(events::event_message* message);
void process_event_message(events::event_message* message, internal_monitor& monitor);

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_H_
