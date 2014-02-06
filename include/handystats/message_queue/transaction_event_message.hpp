#ifndef HANDYSTATS_TRANSACTION_EVENT_MESSAGE_H_
#define HANDYSTATS_TRANSACTION_EVENT_MESSAGE_H_

#include "handystats/message_queue.hpp"
#include "handystats/events/transaction_events.hpp"

inline void HANDY_TRANSACTION_INIT(
		const std::string transaction_name,
		handystats::transaction::state init_state = handystats::transaction::state::NOT_STARTED,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now() = handystats::transaction::clock::now()
		)
{
	auto message = handystats::events::transaction_init_event(transaction_name, init_state, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_TRANSACTION_START(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		)
{
	auto message = handystats::events::transaction_start_event(transaction_name, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_TRANSACTION_STOP(
		const std::string transaction_name,
		handystats::transaction::status final_status = handystats::transaction::status::SUCCESS,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		)
{
	auto message = handystats::events::transaction_stop_event(transaction_name, final_status, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_TRANSACTION_PAUSE(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		)
{
	auto message = handystats::events::transaction_pause_event(transaction_name, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_TRANSACTION_RESUME(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		)
{
	auto message = handystats::events::transaction_resume_event(transaction_name, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_TRANSACTION_BLOCK(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		)
{
	auto message = handystats::events::transaction_block_event(transaction_name, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_TRANSACTION_UNBLOCK(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		)
{
	auto message = handystats::events::transaction_unblock_event(transaction_name, timestamp);
	handystats::message_queue::push_event_message(message);
}

inline void HANDY_TRANSACTION_HEARTBEAT(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		)
{
	auto message = handystats::events::transaction_heartbeat_event(transaction_name, timestamp);
	handystats::message_queue::push_event_message(message);
}

#endif // HANDYSTATS_TRANSACTION_EVENT_MESSAGE_H_

