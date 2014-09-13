// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <algorithm>

#include "events/attribute_impl.hpp"


namespace handystats { namespace events { namespace attribute {

event_message* create_set_event(
		std::string&& attribute_name,
		const metrics::attribute::value_type& value,
		const metrics::attribute::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name.swap(attribute_name);
	message->destination_type = event_destination_type::ATTRIBUTE;

	message->timestamp = timestamp;

	message->event_type = event_type::SET;
	message->event_data = new metrics::attribute::value_type(value);

	return message;
}

void delete_set_event(event_message* message) {
	delete static_cast<metrics::attribute::value_type*>(message->event_data);

	delete message;
}

void delete_event(event_message* message) {
	switch (message->event_type) {
		case event_type::SET:
			delete_set_event(message);
			break;
	}
}

void process_set_event(metrics::attribute& attribute, const event_message& message) {
	const auto& value = *reinterpret_cast<metrics::attribute::value_type*>(message.event_data);
	attribute.set(value);
}

void process_event(metrics::attribute& attribute, const event_message& message) {
	switch (message.event_type) {
		case event_type::SET:
			process_set_event(attribute, message);
			break;
		default:
			return;
	}
}


}}} // namespace handystats::events::attribute
