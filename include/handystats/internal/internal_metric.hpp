#ifndef HANDYSTATS_INTERNAL_METRIC_H_
#define HANDYSTATS_INTERNAL_METRIC_H_

#include <string>

#include "handystats/events/event_message.hpp"

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

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_METRIC_H_
