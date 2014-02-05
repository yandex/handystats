#include <chrono>
#include <thread>

#include <gtest/gtest.h>

#include "handystats/internal.hpp"
#include "handystats/events/counter_events.hpp"
#include "handystats/events/gauge_events.hpp"
#include "handystats/events/transaction_events.hpp"

size_t pop_all_events() {
	size_t popped_count = 0;
	while (!handystats::internal::event_message_queue.empty()) {
		handystats::events::event_message* element;
		handystats::internal::event_message_queue.try_pop(element);
		++popped_count;
	}

	return popped_count;
}

TEST(EventMessageQueue, SinglePushCorrectlyAddsMessage) {
	HANDY_COUNTER_INIT("counter.name", 10);
	ASSERT_EQ(pop_all_events(), 1);

	HANDY_COUNTER_INCREMENT("counter.name", 1);
	ASSERT_EQ(pop_all_events(), 1);

	HANDY_COUNTER_DECREMENT("counter.name", 11);
	ASSERT_EQ(pop_all_events(), 1);

	ASSERT_TRUE(handystats::internal::event_message_queue.empty());
}

TEST(EventMessageQueue, MultiplePushesCorrectlyAddMessages) {
	HANDY_COUNTER_INIT("counter.name", 10);
	HANDY_COUNTER_INCREMENT("counter.name", 1);
	HANDY_COUNTER_DECREMENT("counter.name", 11);

	ASSERT_EQ(pop_all_events(), 3);

	ASSERT_TRUE(handystats::internal::event_message_queue.empty());
}

TEST(EventMessageQueue, PushDifferentMessages) {
	HANDY_COUNTER_INIT("counter.name", 10);
	HANDY_GAUGE_INIT("gauge.name", 10);
	HANDY_TRANSACTION_INIT("trans.name");

	ASSERT_EQ(pop_all_events(), 3);

	ASSERT_TRUE(handystats::internal::event_message_queue.empty());
}
