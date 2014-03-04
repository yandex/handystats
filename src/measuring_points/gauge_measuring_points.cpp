#include <memory>

#include "events/gauge_events_impl.hpp"
#include "message_queue_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/gauge_measuring_points.hpp>


void HANDY_GAUGE_INIT(
		const std::string gauge_name,
		handystats::metrics::gauge::value_type init_value,
		handystats::metrics::gauge::time_point timestamp
		)
{
	if (handystats::is_enabled()) {
		auto message = handystats::events::gauge_init_event(gauge_name, init_value, timestamp);
		handystats::message_queue::push_event_message(message);
	}
}

void HANDY_GAUGE_SET(
		const std::string gauge_name,
		handystats::metrics::gauge::value_type value,
		handystats::metrics::gauge::time_point timestamp
		)
{
	if (handystats::is_enabled()) {
		auto message = handystats::events::gauge_set_event(gauge_name, value, timestamp);
		handystats::message_queue::push_event_message(message);
	}
}

