#ifndef HANDYSTATS_INTERNAL_H_
#define HANDYSTATS_INTERNAL_H_

#include <string>

#include "handystats/events/event_message.hpp"

#include "handystats/metrics/counter.hpp"
#include "handystats/metrics/gauge.hpp"

namespace handystats { namespace internal {

template<class impl, class metric>
struct internal_metric {
	typedef metric metric_type;

	typedef typename metric_type::clock clock;
	typedef typename clock::time_point time_point;

	metric_type* base_metric;
	time_point timestamp;

	internal_metric();

	void process_event_message(events::event_message* message);
	std::string to_string() const;

	~internal_metric();
};


struct internal_counter : internal_metric<internal_counter, metrics::counter> {
	void process_event_message(events::event_message* message);

	void process_init_event(events::event_message* message);
	void process_increment_event(events::event_message* message);
	void process_decrement_event(events::event_message* message);

	std::string to_string() const;
};

struct internal_gauge : internal_metric<internal_gauge, metrics::gauge> {
	void process_event_message(events::event_message* message);

	void process_init_event(events::event_message* message);
	void process_set_event(events::event_message* message);

	std::string to_string() const;
};

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_H_
