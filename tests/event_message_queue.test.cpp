#include <chrono>
#include <thread>
#include <memory>
#include <handystats/atomic.hpp>

#include <gtest/gtest.h>

#include "events/event_message_impl.hpp"
#include "message_queue_impl.hpp"

#include <handystats/measuring_points.hpp>


namespace handystats {

extern std::atomic<bool> enabled_flag;

} // namespace handystats


class EventMessageQueueTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		handystats::message_queue::initialize();
		handystats::enabled_flag.store(true, std::memory_order_release);
	}
	virtual void TearDown() {
		handystats::message_queue::finalize();
		handystats::enabled_flag.store(false, std::memory_order_release);
	}
};


TEST_F(EventMessageQueueTest, SinglePushCorrectlyAddsMessage) {
	HANDY_COUNTER_INIT("counter.name", 10);
	ASSERT_EQ(handystats::message_queue::size(), 1);

	HANDY_COUNTER_INCREMENT("counter.name", 1);
	ASSERT_EQ(handystats::message_queue::size(), 2);

	HANDY_COUNTER_DECREMENT("counter.name", 11);
	ASSERT_EQ(handystats::message_queue::size(), 3);

	HANDY_COUNTER_CHANGE("counter.name", -10);
	ASSERT_EQ(handystats::message_queue::size(), 4);
}

TEST_F(EventMessageQueueTest, MultiplePushesCorrectlyAddMessages) {
	HANDY_COUNTER_INIT("counter.name", 10);
	HANDY_COUNTER_INCREMENT("counter.name", 1);
	HANDY_COUNTER_DECREMENT("counter.name", 11);
	HANDY_COUNTER_CHANGE("counter.name", 13);

	ASSERT_EQ(handystats::message_queue::size(), 4);
}

TEST_F(EventMessageQueueTest, PushDifferentMessages) {
	HANDY_COUNTER_INIT("counter.name", 10);
	HANDY_GAUGE_INIT("gauge.name", 10);
	HANDY_TIMER_INIT("timer.name");

	ASSERT_EQ(handystats::message_queue::size(), 3);
}

