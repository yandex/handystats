/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HANDYSTATS_TIMER_EVENT_HPP_
#define HANDYSTATS_TIMER_EVENT_HPP_

#include <string>

#include <handystats/metrics/timer.hpp>

#include "events/event_message_impl.hpp"


namespace handystats { namespace events { namespace timer {

namespace event_type {
enum : char {
	INIT = 0,
	START,
	STOP,
	DISCARD,
	HEARTBEAT,
	SET
};
} // namespace event_type

/*
 * Event creation functions
 */
event_message* create_init_event(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	);

event_message* create_start_event(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	);

event_message* create_stop_event(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	);

event_message* create_discard_event(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	);

event_message* create_heartbeat_event(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id,
		const metrics::timer::time_point& timestamp
	);

event_message* create_set_event(
		std::string&& timer_name,
		const metrics::timer::value_type& measurement,
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


#endif // HANDYSTATS_TIMER_EVENT_HPP_
