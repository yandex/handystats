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

#ifndef HANDYSTATS_ATTRIBUTE_EVENT_IMPL_HPP_
#define HANDYSTATS_ATTRIBUTE_EVENT_IMPL_HPP_

#include <string>

#include <handystats/attribute.hpp>

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
		const handystats::attribute::value_type& value,
		const handystats::attribute::time_point& timestamp
	);

/*
 * Event destructor
 */
void delete_event(event_message* message);


/*
 * Event processing function
 */
void process_event(handystats::attribute& attr, const event_message& message);

}}} // namespace handystats::events::attribute


#endif // HANDYSTATS_ATTRIBUTE_EVENT_HPP_
