/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDYSTATS_ATTRIBUTE_EVENT_IMPL_HPP_
#define HANDYSTATS_ATTRIBUTE_EVENT_IMPL_HPP_

#include <string>

#include <handystats/metrics/attribute.hpp>

#include "events/event_message_impl.hpp"


namespace handystats { namespace events { namespace attribute {

namespace event_type {
enum : char {
	SET = 0
};
} // namespace event_type

/*
 * Event creation functions
 */
event_message* create_set_event(
		std::string&& attribute_name,
		const metrics::attribute::value_type& value,
		const metrics::attribute::time_point& timestamp
	);

/*
 * Event destructor
 */
void delete_event(event_message* message);


/*
 * Event processing function
 */
void process_event(metrics::attribute& counter, const event_message& message);

}}} // namespace handystats::events::attribute


#endif // HANDYSTATS_ATTRIBUTE_EVENT_HPP_

