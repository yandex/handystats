#ifndef HANDYSTATS_GAUGE_EVENT_H_
#define HANDYSTATS_GAUGE_EVENT_H_

#include <memory>

#include "handystats/metrics/gauge.hpp"
#include "handystats/events/event_message.hpp"

namespace handystats { namespace events {

enum class gauge_event {
	INIT,
	SET
};


std::shared_ptr<event_message> gauge_init_event(
		const std::string gauge_name,
		metrics::gauge::value_type init_value,
		metrics::gauge::time_point timestamp = metrics::gauge::clock::now()
		);

std::shared_ptr<event_message> gauge_set_event(
		const std::string gauge_name,
		metrics::gauge::value_type value,
		metrics::gauge::time_point timestamp = metrics::gauge::clock::now()
		);


void delete_gauge_init_event(event_message*);
void delete_gauge_set_event(event_message*);

void delete_gauge_event(event_message*);


}} // namespace handystats::events

#endif // HANDYSTATS_GAUGE_EVENT_H_

