#ifndef HANDYSTATS_INTERNAL_GAUGE_H_
#define HANDYSTATS_INTERNAL_GAUGE_H_

#include <string>

#include <handystats/metrics/gauge.hpp>
#include <handystats/events/event_message.hpp>

namespace handystats { namespace internal {

struct internal_gauge
{
	typedef typename metrics::gauge::clock clock;
	typedef typename clock::time_point time_point;

	metrics::gauge* base_gauge;
	time_point timestamp;

	internal_gauge() {
		base_gauge = nullptr;
	}

	~internal_gauge() {
		if (base_gauge) {
			delete base_gauge;
		}
	}

	void process_event_message(events::event_message* message);

	void process_init_event(events::event_message* message);
	void process_set_event(events::event_message* message);

	std::string to_string() const;
};

}} // namespace handystats::internal

#endif // HANDYSTATS_INTERNAL_GAUGE_H_
