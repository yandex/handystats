#include <memory>

#include "events/counter_events_impl.hpp"
#include "message_queue_impl.hpp"
#include "core_impl.hpp"

#include <handystats/measuring_points/counter_measuring_points.hpp>


void HANDY_COUNTER_INIT(
		const std::string counter_name,
		handystats::metrics::counter::value_type init_value,
		handystats::metrics::counter::time_point timestamp
		)
{
	if (handystats::is_enabled()) {
		auto message = handystats::events::counter_init_event(counter_name, init_value, timestamp);
		handystats::message_queue::push_event_message(message);
	}
}

void HANDY_COUNTER_INCREMENT(
		const std::string counter_name,
		handystats::metrics::counter::value_type value,
		handystats::metrics::counter::time_point timestamp
		)
{
	if (handystats::is_enabled()) {
		auto message = handystats::events::counter_increment_event(counter_name, value, timestamp);
		handystats::message_queue::push_event_message(message);
	}
}

void HANDY_COUNTER_DECREMENT(
		const std::string counter_name,
		handystats::metrics::counter::value_type value,
		handystats::metrics::counter::time_point timestamp
		)
{
	if (handystats::is_enabled()) {
		auto message = handystats::events::counter_decrement_event(counter_name, value, timestamp);
		handystats::message_queue::push_event_message(message);
	}
}

