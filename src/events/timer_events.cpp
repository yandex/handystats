#include <handystats/metrics/timer.hpp>

#include "events/event_message_impl.hpp"
#include "events/timer_events_impl.hpp"


namespace handystats { namespace events {

std::shared_ptr<event_message> timer_init_event(
		const std::string timer_name,
		const metrics::timer::instance_id_type instance_id,
		metrics::timer::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = timer_name;
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = timer_event::INIT;
	message->event_data.push_back(new metrics::timer::instance_id_type(instance_id));

	return std::shared_ptr<event_message>(message, delete_event_message);
}

void delete_timer_init_event(event_message* message) {
	delete static_cast<metrics::timer::instance_id_type*>(message->event_data[0]);

	delete message;
}


std::shared_ptr<event_message> timer_start_event(
		const std::string timer_name,
		const metrics::timer::instance_id_type instance_id,
		metrics::timer::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = timer_name;
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = timer_event::START;
	message->event_data.push_back(new metrics::timer::instance_id_type(instance_id));

	return std::shared_ptr<event_message>(message, delete_event_message);
}

void delete_timer_start_event(event_message* message) {
	delete static_cast<metrics::timer::instance_id_type*>(message->event_data[0]);

	delete message;
}


std::shared_ptr<event_message> timer_stop_event(
		const std::string timer_name,
		const metrics::timer::instance_id_type instance_id,
		metrics::timer::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = timer_name;
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = timer_event::STOP;
	message->event_data.push_back(new metrics::timer::instance_id_type(instance_id));

	return std::shared_ptr<event_message>(message, delete_event_message);
}

void delete_timer_stop_event(event_message* message) {
	delete static_cast<metrics::timer::instance_id_type*>(message->event_data[0]);

	delete message;
}


std::shared_ptr<event_message> timer_discard_event(
		const std::string timer_name,
		const metrics::timer::instance_id_type instance_id,
		metrics::timer::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = timer_name;
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = timer_event::DISCARD;
	message->event_data.push_back(new metrics::timer::instance_id_type(instance_id));

	return std::shared_ptr<event_message>(message, delete_event_message);
}

void delete_timer_discard_event(event_message* message) {
	delete static_cast<metrics::timer::instance_id_type*>(message->event_data[0]);

	delete message;
}


std::shared_ptr<event_message> timer_heartbeat_event(
		const std::string timer_name,
		const metrics::timer::instance_id_type instance_id,
		metrics::timer::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = timer_name;
	message->destination_type = event_destination_type::TIMER;

	message->timestamp = timestamp;

	message->event_type = timer_event::HEARTBEAT;
	message->event_data.push_back(new metrics::timer::instance_id_type(instance_id));

	return std::shared_ptr<event_message>(message, delete_event_message);
}

void delete_timer_heartbeat_event(event_message* message) {
	delete static_cast<metrics::timer::instance_id_type*>(message->event_data[0]);

	delete message;
}


void delete_timer_event(event_message* message) {
	switch (message->event_type) {
		case timer_event::INIT:
			delete_timer_init_event(message);
			break;
		case timer_event::START:
			delete_timer_start_event(message);
			break;
		case timer_event::STOP:
			delete_timer_stop_event(message);
			break;
		case timer_event::DISCARD:
			delete_timer_discard_event(message);
			break;
		case timer_event::HEARTBEAT:
			delete_timer_heartbeat_event(message);
			break;
	}
}


}} // namespace handystats::events


