#ifndef HANDYSTATS_TRANSACTION_EVENT_H_
#define HANDYSTATS_TRANSACTION_EVENT_H_

#include "handystats/events/event_message.hpp"
#include "handystats/transaction.hpp"

namespace handystats { namespace events {

enum class transaction_event {
	INIT,
	START,
	STOP,
	PAUSE,
	RESUME,
	BLOCK,
	UNBLOCK,
	HEARTBEAT
};


event_message* transaction_init_event(
		const std::string transaction_name,
		transaction::state init_state = transaction::state::NOT_STARTED,
		transaction::time_point timestamp = transaction::clock::now()
		);

event_message* transaction_start_event(
		const std::string transaction_name,
		transaction::time_point timestamp = transaction::clock::now()
		);

event_message* transaction_stop_event(
		const std::string transaction_name,
		transaction::status final_status = transaction::status::SUCCESS,
		transaction::time_point timestamp = transaction::clock::now()
		);

event_message* transaction_pause_event(
		const std::string transaction_name,
		transaction::time_point timestamp = transaction::clock::now()
		);

event_message* transaction_resume_event(
		const std::string transaction_name,
		transaction::time_point timestamp = transaction::clock::now()
		);

event_message* transaction_block_event(
		const std::string transaction_name,
		transaction::time_point timestamp = transaction::clock::now()
		);

event_message* transaction_unblock_event(
		const std::string transaction_name,
		transaction::time_point timestamp = transaction::clock::now()
		);

event_message* transaction_heartbeat_event(
		const std::string transaction_name,
		transaction::time_point timestamp = transaction::clock::now()
		);


void delete_transaction_init_event(event_message*);
void delete_transaction_start_event(event_message*);
void delete_transaction_stop_event(event_message*);
void delete_transaction_pause_event(event_message*);
void delete_transaction_resume_event(event_message*);
void delete_transaction_block_event(event_message*);
void delete_transaction_unblock_event(event_message*);
void delete_transaction_heartbeat_event(event_message*);

void delete_transaction_event(event_message*);


}} // namespace handystats::events


void HANDY_TRANSACTION_INIT(
		const std::string transaction_name,
		handystats::transaction::state init_state = handystats::transaction::state::NOT_STARTED,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		);

void HANDY_TRANSACTION_START(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		);

void HANDY_TRANSACTION_STOP(
		const std::string transaction_name,
		handystats::transaction::status final_status = handystats::transaction::status::SUCCESS,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		);

void HANDY_TRANSACTION_PAUSE(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		);

void HANDY_TRANSACTION_RESUME(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		);

void HANDY_TRANSACTION_BLOCK(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		);

void HANDY_TRANSACTION_UNBLOCK(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		);

void HANDY_TRANSACTION_HEARTBEAT(
		const std::string transaction_name,
		handystats::transaction::time_point timestamp = handystats::transaction::clock::now()
		);


#endif // HANDYSTATS_TRANSACTION_EVENT_H_
