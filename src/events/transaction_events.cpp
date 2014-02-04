#include "handystats/events/transaction_events.hpp"

namespace handystats { namespace events {

event_message* transaction_init_event(
		const std::string transaction_name,
		transaction::state init_state,
		transaction::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = transaction_name;
	message->destination_type = event_destination_type::TRANSACTION;

	message->timestamp = timestamp;

	message->event_type = new transaction_event(transaction_event::INIT);
	message->event_data.push_back(new transaction::state(init_state));

	return message;
}

void delete_transaction_init_event(event_message* message) {
	delete static_cast<transaction_event*>(message->event_type);
	delete static_cast<transaction::state*>(message->event_data[0]);

	delete message;
}


event_message* transaction_start_event(
		const std::string transaction_name,
		transaction::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = transaction_name;
	message->destination_type = event_destination_type::TRANSACTION;

	message->timestamp = timestamp;

	message->event_type = new transaction_event(transaction_event::START);

	return message;
}

void delete_transaction_start_event(event_message* message) {
	delete static_cast<transaction_event*>(message->event_type);

	delete message;
}


event_message* transaction_stop_event(
		const std::string transaction_name,
		transaction::status final_status,
		transaction::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = transaction_name;
	message->destination_type = event_destination_type::TRANSACTION;

	message->timestamp = timestamp;

	message->event_type = new transaction_event(transaction_event::STOP);
	message->event_data.push_back(new transaction::status(final_status));

	return message;
}

void delete_transaction_stop_event(event_message* message) {
	delete static_cast<transaction_event*>(message->event_type);
	delete static_cast<transaction::status*>(message->event_data[0]);

	delete message;
}


event_message* transaction_pause_event(
		const std::string transaction_name,
		transaction::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = transaction_name;
	message->destination_type = event_destination_type::TRANSACTION;

	message->timestamp = timestamp;

	message->event_type = new transaction_event(transaction_event::PAUSE);

	return message;
}

void delete_transaction_pause_event(event_message* message) {
	delete static_cast<transaction_event*>(message->event_type);

	delete message;
}


event_message* transaction_resume_event(
		const std::string transaction_name,
		transaction::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = transaction_name;
	message->destination_type = event_destination_type::TRANSACTION;

	message->timestamp = timestamp;

	message->event_type = new transaction_event(transaction_event::RESUME);

	return message;
}

void delete_transaction_resume_event(event_message* message) {
	delete static_cast<transaction_event*>(message->event_type);

	delete message;
}


event_message* transaction_block_event(
		const std::string transaction_name,
		transaction::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = transaction_name;
	message->destination_type = event_destination_type::TRANSACTION;

	message->timestamp = timestamp;

	message->event_type = new transaction_event(transaction_event::BLOCK);

	return message;
}

void delete_transaction_block_event(event_message* message) {
	delete static_cast<transaction_event*>(message->event_type);

	delete message;
}


event_message* transaction_unblock_event(
		const std::string transaction_name,
		transaction::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = transaction_name;
	message->destination_type = event_destination_type::TRANSACTION;

	message->timestamp = timestamp;

	message->event_type = new transaction_event(transaction_event::UNBLOCK);

	return message;
}

void delete_transaction_unblock_event(event_message* message) {
	delete static_cast<transaction_event*>(message->event_type);

	delete message;
}


event_message* transaction_heartbeat_event(
		const std::string transaction_name,
		transaction::time_point timestamp
		)
{
	event_message* message = new event_message;

	message->destination_name = transaction_name;
	message->destination_type = event_destination_type::TRANSACTION;

	message->timestamp = timestamp;

	message->event_type = new transaction_event(transaction_event::HEARTBEAT);

	return message;
}

void delete_transaction_heartbeat_event(event_message* message) {
	delete static_cast<transaction_event*>(message->event_type);

	delete message;
}


void delete_transaction_event(event_message* message) {
	switch (*static_cast<transaction_event*>(message->event_type)) {
		case transaction_event::INIT:
			delete_transaction_init_event(message);
			break;
		case transaction_event::START:
			delete_transaction_start_event(message);
			break;
		case transaction_event::STOP:
			delete_transaction_stop_event(message);
			break;
		case transaction_event::PAUSE:
			delete_transaction_pause_event(message);
			break;
		case transaction_event::RESUME:
			delete_transaction_resume_event(message);
			break;
		case transaction_event::BLOCK:
			delete_transaction_block_event(message);
			break;
		case transaction_event::UNBLOCK:
			delete_transaction_unblock_event(message);
			break;
		case transaction_event::HEARTBEAT:
			delete_transaction_heartbeat_event(message);
			break;
	}
}


}} // namespace handystats::events


inline void HANDY_TRANSACTION_INIT(
		const std::string transaction_name,
		handystats::transaction::state init_state,
		handystats::transaction::time_point timestamp
		)
{
	auto message = handystats::events::transaction_init_event(transaction_name, init_state, timestamp);
	handystats::event_message_queue.push(message);
}

inline void HANDY_TRANSACTION_START(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp
		)
{
	auto message = handystats::events::transaction_start_event(transaction_name, timestamp);
	handystats::event_message_queue.push(message);
}

inline void HANDY_TRANSACTION_STOP(
		const std::string transaction_name,
		handystats::transaction::status final_status,
		handystats::transaction::time_point timestamp
		)
{
	auto message = handystats::events::transaction_stop_event(transaction_name, final_status, timestamp);
	handystats::event_message_queue.push(message);
}

inline void HANDY_TRANSACTION_PAUSE(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp
		)
{
	auto message = handystats::events::transaction_pause_event(transaction_name, timestamp);
	handystats::event_message_queue.push(message);
}

inline void HANDY_TRANSACTION_RESUME(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp
		)
{
	auto message = handystats::events::transaction_resume_event(transaction_name, timestamp);
	handystats::event_message_queue.push(message);
}

inline void HANDY_TRANSACTION_BLOCK(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp
		)
{
	auto message = handystats::events::transaction_block_event(transaction_name, timestamp);
	handystats::event_message_queue.push(message);
}

inline void HANDY_TRANSACTION_UNBLOCK(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp
		)
{
	auto message = handystats::events::transaction_unblock_event(transaction_name, timestamp);
	handystats::event_message_queue.push(message);
}

inline void HANDY_TRANSACTION_HEARTBEAT(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp
		)
{
	auto message = handystats::events::transaction_heartbeat_event(transaction_name, timestamp);
	handystats::event_message_queue.push(message);
}

