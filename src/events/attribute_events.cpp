// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include "events/attribute_events_impl.hpp"


namespace handystats { namespace events { namespace attribute {

event_message* create_set_event(
		const std::string& attribute_name,
		const metrics::attribute::value_type& value,
		const metrics::attribute::time_point& timestamp
	)
{
	event_message* message = new event_message;

	message->destination_name = attribute_name;
	message->destination_type = event_destination_type::ATTRIBUTE;

	message->timestamp = timestamp;

	message->event_type = event_type::SET;
	message->event_data.push_back(new metrics::attribute::value_type(value));

	return message;
}

event_message* create_set_event(
		const std::string& attribute_name,
		const bool& b,
		const metrics::attribute::time_point& timestamp
	)
{
	return create_set_event(attribute_name, metrics::attribute::value_type(b), timestamp);
}

event_message* create_set_event(
		const std::string& attribute_name,
		const int& i,
		const metrics::attribute::time_point& timestamp
	)
{
	return create_set_event(attribute_name, metrics::attribute::value_type(i), timestamp);
}

event_message* create_set_event(
		const std::string& attribute_name,
		const unsigned& u,
		const metrics::attribute::time_point& timestamp
	)
{
	return create_set_event(attribute_name, metrics::attribute::value_type(u), timestamp);
}

event_message* create_set_event(
		const std::string& attribute_name,
		const int64_t& i64,
		const metrics::attribute::time_point& timestamp
	)
{
	return create_set_event(attribute_name, metrics::attribute::value_type(i64), timestamp);
}

event_message* create_set_event(
		const std::string& attribute_name,
		const uint64_t& u64,
		const metrics::attribute::time_point& timestamp
	)
{
	return create_set_event(attribute_name, metrics::attribute::value_type(u64), timestamp);
}

event_message* create_set_event(
		const std::string& attribute_name,
		const double& d,
		const metrics::attribute::time_point& timestamp
	)
{
	return create_set_event(attribute_name, metrics::attribute::value_type(d), timestamp);
}

event_message* create_set_event(
		const std::string& attribute_name,
		const char* s,
		const metrics::attribute::time_point& timestamp
	)
{
	return create_set_event(attribute_name, metrics::attribute::value_type(std::string(s)), timestamp);
}

event_message* create_set_event(
		const std::string& attribute_name,
		const std::string& s,
		const metrics::attribute::time_point& timestamp
	)
{
	return create_set_event(attribute_name, metrics::attribute::value_type(s), timestamp);
}

void delete_set_event(event_message* message) {
	delete static_cast<metrics::attribute::value_type*>(message->event_data[0]);

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
	const auto& value = *static_cast<metrics::attribute::value_type*>(message.event_data[0]);
	attribute.set(value, message.timestamp);
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
