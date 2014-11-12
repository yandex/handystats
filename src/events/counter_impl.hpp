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

#ifndef HANDYSTATS_COUNTER_EVENT_IMPL_HPP_
#define HANDYSTATS_COUNTER_EVENT_IMPL_HPP_

#include <string>

#include <handystats/metrics/counter.hpp>

#include "events/event_message_impl.hpp"


namespace handystats { namespace events { namespace counter {

namespace event_type {
enum : char {
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
