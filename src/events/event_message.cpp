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

#include "events/gauge_impl.hpp"
#include "events/counter_impl.hpp"
#include "events/timer_impl.hpp"
#include "events/attribute_impl.hpp"

#include "events/event_message_impl.hpp"


namespace handystats { namespace events {

void delete_event_message(event_message* message) {
	if (!message) {
		return;
	}

	switch (message->destination_type) {
		case event_destination_type::COUNTER:
			counter::delete_event(message);
			break;
		case event_destination_type::GAUGE:
			gauge::delete_event(message);
			break;
		case event_destination_type::TIMER:
			timer::delete_event(message);
			break;
		case event_destination_type::ATTRIBUTE:
			attribute::delete_event(message);
			break;
		default:
			return;
	}
}

}} // namespace handystats::events
