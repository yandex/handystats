// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_TIMER_EVENT_H_
#define HANDYSTATS_TIMER_EVENT_H_

#include <string>

#include <handystats/metrics/timer.hpp>

#include "events/event_message_impl.hpp"


namespace handystats { namespace events { namespace timer {

namespace event_type {
enum {
	INIT = 0,
	START,
	STOP,
	DISCARD,
	HEARTBEAT
};
} // namespace event_type

/*
 * Event creation functions
 */
event_message_ptr create_init_event(
		const std::string& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	);

event_message_ptr create_start_event(
		const std::string& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	);

event_message_ptr create_stop_event(
		const std::string& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	);

event_message_ptr create_discard_event(
		const std::string& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	);

event_message_ptr create_heartbeat_event(
		const std::string& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	);


/*
 * Event destructor
 */
void delete_event(event_message* message);


/*
 * Event processing function
 */
void process_event(metrics::timer& timer, const event_message& message);

}}} // namespace handystats::events::timer


#endif // HANDYSTATS_TIMER_EVENT_H_
