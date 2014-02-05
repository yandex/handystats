#ifndef HANDYSTATS_INTERNAL_COUNTER_H_
#define HANDYSTATS_INTERNAL_COUNTER_H_

#include "handystats/internal/internal_metric.hpp"
#include "handystats/metrics/counter.hpp"

namespace handystats { namespace internal {

struct internal_counter : internal_metric<internal_counter, metrics::counter> {
	void process_event_message(events::event_message* message);

	void process_init_event(events::event_message* message);
	void process_increment_event(events::event_message* message);
	void process_decrement_event(events::event_message* message);

	std::string to_string() const;
};

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_COUNTER_H_
