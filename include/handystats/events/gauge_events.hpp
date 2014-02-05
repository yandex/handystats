#ifndef HANDYSTATS_GAUGE_EVENT_H_
#define HANDYSTATS_GAUGE_EVENT_H_

#include "handystats/metrics/gauge.hpp"
#include "handystats/events/event_message.hpp"

namespace handystats { namespace events {

enum class gauge_event {
	INIT,
	SET
};


event_message* gauge_init_event(
		const std::string gauge_name,
		metrics::gauge::value_type init_value,
		metrics::gauge::time_point timestamp = metrics::gauge::clock::now()
		);

event_message* gauge_set_event(
		const std::string gauge_name,
		metrics::gauge::value_type value,
		metrics::gauge::time_point timestamp = metrics::gauge::clock::now()
		);


void delete_gauge_init_event(event_message*);
void delete_gauge_set_event(event_message*);

void delete_gauge_event(event_message*);


}} // namespace handystats::events


void HANDY_GAUGE_INIT(
		const std::string gauge_name,
		handystats::metrics::gauge::value_type init_value,
		handystats::metrics::gauge::time_point timestamp = handystats::metrics::gauge::clock::now()
		);

void HANDY_GAUGE_SET(
		const std::string gauge_name,
		handystats::metrics::gauge::value_type value,
		handystats::metrics::gauge::time_point timestamp = handystats::metrics::gauge::clock::now()
		);


#endif // HANDYSTATS_GAUGE_EVENT_H_
