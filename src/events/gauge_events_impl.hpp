#ifndef HANDYSTATS_GAUGE_EVENT_IMPL_H_
#define HANDYSTATS_GAUGE_EVENT_IMPL_H_

#include <string>
#include <memory>

#include <handystats/metrics/gauge.hpp>

#include "events/event_message_impl.hpp"


namespace handystats { namespace events {

enum class gauge_event {
	INIT,
	SET
};


std::shared_ptr<event_message> gauge_init_event(
		const std::string gauge_name,
		metrics::gauge::value_type init_value,
		metrics::gauge::time_point timestamp
		);

std::shared_ptr<event_message> gauge_set_event(
		const std::string gauge_name,
		metrics::gauge::value_type value,
		metrics::gauge::time_point timestamp
		);


void delete_gauge_event(event_message* message);

}} // namespace handystats::events


#endif // HANDYSTATS_GAUGE_EVENT_H_

