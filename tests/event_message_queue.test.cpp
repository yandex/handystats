#include <chrono>
#include <thread>
#include <memory>

#include <gtest/gtest.h>

#include <tbb/concurrent_queue.h>

#include "events/event_message_impl.hpp"

#include <handystats/measuring_points.hpp>


namespace handystats { namespace message_queue {

extern tbb::concurrent_queue<std::shared_ptr<events::event_message>>* event_message_queue;

void initialize();
void finalize();

}} // namespace handystats::message_queue


class EventMessageQueueTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		handystats::message_queue::initialize();
	}
	virtual void TearDown() {
		handystats::message_queue::finalize();
	}
};


TEST_F(EventMessageQueueTest, SinglePushCorrectlyAddsMessage) {
	HANDY_COUNTER_INIT("counter.name", 10);
	ASSERT_EQ(handystats::message_queue::event_message_queue->unsafe_size(), 1);

	HANDY_COUNTER_INCREMENT("counter.name", 1);
	ASSERT_EQ(handystats::message_queue::event_message_queue->unsafe_size(), 2);

	HANDY_COUNTER_DECREMENT("counter.name", 11);
	ASSERT_EQ(handystats::message_queue::event_message_queue->unsafe_size(), 3);
}

TEST_F(EventMessageQueueTest, MultiplePushesCorrectlyAddMessages) {
	HANDY_COUNTER_INIT("counter.name", 10);
	HANDY_COUNTER_INCREMENT("counter.name", 1);
	HANDY_COUNTER_DECREMENT("counter.name", 11);

	ASSERT_EQ(handystats::message_queue::event_message_queue->unsafe_size(), 3);
}

TEST_F(EventMessageQueueTest, PushDifferentMessages) {
	HANDY_COUNTER_INIT("counter.name", 10);
	HANDY_GAUGE_INIT("gauge.name", 10);
	HANDY_TIMER_INIT("timer.name");

	ASSERT_EQ(handystats::message_queue::event_message_queue->unsafe_size(), 3);
}

