// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_GAUGE_EVENT_IMPL_HPP_
#define HANDYSTATS_GAUGE_EVENT_IMPL_HPP_

#include <string>

#include <handystats/metrics/gauge.hpp>

#include "events/event_message_impl.hpp"


namespace handystats { namespace events { namespace gauge {

namespace event_type {
enum : char {
	INIT = 0,
	SET
};
} // namespace event_type

/*
 * Event creation functions
 */
event_message* create_init_event(
		std::string&& gauge_name,
		const metrics::gauge::value_type& init_value,
		const metrics::gauge::time_point& timestamp
	);

event_message* create_set_event(
		std::string&& gauge_name,
		const metrics::gauge::value_type& value,
		const metrics::gauge::time_point& timestamp
	);


/*
 * Event destructor
 */
void delete_event(event_message* message);


/*
 * Event processing function
 */
void process_event(metrics::gauge& counter, const event_message& message);

}}} // namespace handystats::events::gauge


#endif // HANDYSTATS_GAUGE_EVENT_HPP_

