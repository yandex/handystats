#ifndef HANDYSTATS_INTERNAL_GAUGE_H_
#define HANDYSTATS_INTERNAL_GAUGE_H_

#include "handystats/internal/internal_metric.hpp"
#include "handystats/metrics/gauge.hpp"

namespace handystats { namespace internal {

struct internal_gauge : internal_metric<internal_gauge, metrics::gauge> {
	void process_event_message(events::event_message* message);

	void process_init_event(events::event_message* message);
	void process_set_event(events::event_message* message);

	std::string to_string() const;
};

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_GAUGE_H_
