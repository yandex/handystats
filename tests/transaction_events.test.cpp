#include <gtest/gtest.h>

#include "handystats/events/transaction_events.hpp"

using namespace handystats::events;

TEST(TransactionEventsTest, TestTransactionInitEvent) {
	const std::string transaction_name = "queue.push";
	handystats::transaction::state init_state = handystats::transaction::state::RUNNING;
	event_message* message = transaction_init_event(transaction_name, init_state);

	ASSERT_EQ(message->destination_name, transaction_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TRANSACTION);

	ASSERT_EQ(*static_cast<transaction_event*>(message->event_type), transaction_event::INIT);
	ASSERT_EQ(*static_cast<handystats::transaction::state*>(message->event_data[0]), init_state);

	delete_transaction_event(message);
}

TEST(TransactionEventsTest, TestTransactionStartEvent) {
	const std::string transaction_name = "queue.push";
	event_message* message = transaction_start_event(transaction_name);

	ASSERT_EQ(message->destination_name, transaction_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TRANSACTION);

	ASSERT_EQ(*static_cast<transaction_event*>(message->event_type), transaction_event::START);

	delete_transaction_event(message);
}

TEST(TransactionEventsTest, TestTransactionStopEvent) {
	const std::string transaction_name = "queue.push";
	handystats::transaction::status final_status = handystats::transaction::status::FAIL;
	event_message* message = transaction_stop_event(transaction_name, final_status);

	ASSERT_EQ(message->destination_name, transaction_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TRANSACTION);

	ASSERT_EQ(*static_cast<transaction_event*>(message->event_type), transaction_event::STOP);
	ASSERT_EQ(*static_cast<handystats::transaction::status*>(message->event_data[0]), final_status);

	delete_transaction_event(message);
}

TEST(TransactionEventsTest, TestTransactionPauseEvent) {
	const std::string transaction_name = "queue.push";
	event_message* message = transaction_pause_event(transaction_name);

	ASSERT_EQ(message->destination_name, transaction_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TRANSACTION);

	ASSERT_EQ(*static_cast<transaction_event*>(message->event_type), transaction_event::PAUSE);

	delete_transaction_event(message);
}

TEST(TransactionEventsTest, TestTransactionResumeEvent) {
	const std::string transaction_name = "queue.push";
	event_message* message = transaction_resume_event(transaction_name);

	ASSERT_EQ(message->destination_name, transaction_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TRANSACTION);

	ASSERT_EQ(*static_cast<transaction_event*>(message->event_type), transaction_event::RESUME);

	delete_transaction_event(message);
}

TEST(TransactionEventsTest, TestTransactionBlockEvent) {
	const std::string transaction_name = "queue.push";
	event_message* message = transaction_block_event(transaction_name);

	ASSERT_EQ(message->destination_name, transaction_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TRANSACTION);

	ASSERT_EQ(*static_cast<transaction_event*>(message->event_type), transaction_event::BLOCK);

	delete_transaction_event(message);
}

TEST(TransactionEventsTest, TestTransactionUnblockEvent) {
	const std::string transaction_name = "queue.push";
	event_message* message = transaction_unblock_event(transaction_name);

	ASSERT_EQ(message->destination_name, transaction_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TRANSACTION);

	ASSERT_EQ(*static_cast<transaction_event*>(message->event_type), transaction_event::UNBLOCK);

	delete_transaction_event(message);
}

TEST(TransactionEventsTest, TestTransactionHeartbeatEvent) {
	const std::string transaction_name = "queue.push";
	event_message* message = transaction_heartbeat_event(transaction_name);

	ASSERT_EQ(message->destination_name, transaction_name);
	ASSERT_EQ(message->destination_type, event_destination_type::TRANSACTION);

	ASSERT_EQ(*static_cast<transaction_event*>(message->event_type), transaction_event::HEARTBEAT);

	delete_transaction_event(message);
}

