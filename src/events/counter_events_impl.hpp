// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_COUNTER_EVENT_IMPL_HPP_
#define HANDYSTATS_COUNTER_EVENT_IMPL_HPP_

#include <string>

#include <handystats/metrics/counter.hpp>

#include "events/event_message_impl.hpp"


namespace handystats { namespace events { namespace counter {

namespace event_type {
enum {
	INIT = 0,
	INCREMENT,
	DECREMENT
};
} // namespace event_type

/*
 * Event creation functions
 */
event_message* create_init_event(
		std::string&& counter_name,
		const metrics::counter::value_type& init_value,
		const metrics::counter::time_point& timestamp
	);

event_message* create_increment_event(
		std::string&& counter_name,
		const metrics::counter::value_type& value,
		const metrics::counter::time_point& timestamp
	);

event_message* create_decrement_event(
		std::string&& counter_name,
		const metrics::counter::value_type& value,
		const metrics::counter::time_point& timestamp
	);


/*
 * Event destructor
 */
void delete_event(event_message* message);


/*
 * Event processing function
 */
void process_event(metrics::counter& counter, const event_message& message);

}}} // namespace handystats::events::counter


#endif // HANDYSTATS_COUNTER_EVENT_HPP_
