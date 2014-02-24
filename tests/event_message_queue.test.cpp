#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include <handystats/measuring_points.hpp>
#include <handystats/message_queue_impl.hpp>

TEST(EventMessageQueue, SinglePushCorrectlyAddsMessage) {
	handystats::message_queue::initialize();

	HANDY_COUNTER_INIT("counter.name", 10);
	ASSERT_EQ(handystats::message_queue::event_message_queue->unsafe_size(), 1);

	HANDY_COUNTER_INCREMENT("counter.name", 1);
	ASSERT_EQ(handystats::message_queue::event_message_queue->unsafe_size(), 2);

	HANDY_COUNTER_DECREMENT("counter.name", 11);
	ASSERT_EQ(handystats::message_queue::event_message_queue->unsafe_size(), 3);

	handystats::message_queue::finalize();
}

TEST(EventMessageQueue, MultiplePushesCorrectlyAddMessages) {
	handystats::message_queue::initialize();

	HANDY_COUNTER_INIT("counter.name", 10);
	HANDY_COUNTER_INCREMENT("counter.name", 1);
	HANDY_COUNTER_DECREMENT("counter.name", 11);

	ASSERT_EQ(handystats::message_queue::event_message_queue->unsafe_size(), 3);

	handystats::message_queue::finalize();
}

TEST(EventMessageQueue, PushDifferentMessages) {
	handystats::message_queue::initialize();

	HANDY_COUNTER_INIT("counter.name", 10);
	HANDY_GAUGE_INIT("gauge.name", 10);
	HANDY_TIMER_INIT("timer.name");

	ASSERT_EQ(handystats::message_queue::event_message_queue->unsafe_size(), 3);

	handystats::message_queue::finalize();
}

