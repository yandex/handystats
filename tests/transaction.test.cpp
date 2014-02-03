#include <thread>

#include <gtest/gtest.h>

#include <handystats/chrono.hpp>
#include <handystats/transaction.hpp>

using namespace handystats;

TEST(TransactionTest, NewTransactionWithoutInitialState) {
	transaction trans;

	ASSERT_EQ(trans.get_state(), transaction::state::NOT_STARTED);
}

TEST(TransactionTest, NewTransactionWithInitialState) {
	transaction running_trans(transaction::clock::now(), transaction::state::RUNNING);
	ASSERT_EQ(running_trans.get_state(), transaction::state::RUNNING);

	transaction paused_trans(transaction::clock::now(), transaction::state::PAUSED);
	ASSERT_EQ(paused_trans.get_state(), transaction::state::PAUSED);

	transaction blocked_trans(transaction::clock::now(), transaction::state::BLOCKED);
	ASSERT_EQ(blocked_trans.get_state(), transaction::state::BLOCKED);

	transaction stopped_trans(transaction::clock::now(), transaction::state::STOPPED);
	ASSERT_EQ(stopped_trans.get_state(), transaction::state::STOPPED);
}

TEST(TransactionTest, NotPausedTransHasNoPausedTime) {
	transaction trans;
	trans.start(transaction::clock::now());
	trans.block(transaction::clock::now());

	std::this_thread::sleep_for(std::chrono::nanoseconds(100));

	trans.unblock(transaction::clock::now());
	trans.stop(transaction::clock::now(), transaction::status::SUCCESS);

	ASSERT_EQ(trans.get_stats().paused_time.count(), 0);

	ASSERT_GT(trans.get_stats().running_time.count(), 0);
	ASSERT_GT(trans.get_stats().blocked_time.count(), 0);
}

TEST(TransactionTest, NotBlockedTransHasNoBlockedTime) {
	transaction trans;
	trans.start(transaction::clock::now());
	trans.pause(transaction::clock::now());

	std::this_thread::sleep_for(std::chrono::nanoseconds(100));

	trans.resume(transaction::clock::now());
	trans.stop(transaction::clock::now(), transaction::status::SUCCESS);

	ASSERT_EQ(trans.get_stats().blocked_time.count(), 0);

	ASSERT_GT(trans.get_stats().running_time.count(), 0);
	ASSERT_GT(trans.get_stats().paused_time.count(), 0);
}

TEST(TransactionTest, NotStartedTransHasNoTimes) {
	transaction trans;
//	trans.start(transaction::clock::now());
	trans.pause(transaction::clock::now());

	std::this_thread::sleep_for(std::chrono::nanoseconds(100));

	trans.resume(transaction::clock::now());
	trans.stop(transaction::clock::now(), transaction::status::SUCCESS);

	ASSERT_EQ(trans.get_stats().blocked_time.count(), 0);
	ASSERT_EQ(trans.get_stats().running_time.count(), 0);
	ASSERT_EQ(trans.get_stats().paused_time.count(), 0);
}

TEST(TransactionTest, FinalStatusNotDefinedByDefault) {
	transaction trans;

	ASSERT_EQ(trans.get_status(), transaction::status::NOT_DEFINED);
}

TEST(TransactionTest, TransStopsWithCorrectFinalStatus) {
	transaction success_trans;
	success_trans.stop(transaction::clock::now(), transaction::status::SUCCESS);
	ASSERT_EQ(success_trans.get_status(), transaction::status::SUCCESS);

	transaction fail_trans;
	fail_trans.stop(transaction::clock::now(), transaction::status::FAIL);
	ASSERT_EQ(fail_trans.get_status(), transaction::status::FAIL);
}
