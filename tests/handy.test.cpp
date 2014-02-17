#include <vector>
#include <utility>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <queue>

#include <gtest/gtest.h>

#include "handystats/handystats.hpp"

#include <handystats/json_dump.hpp>

TEST(HandyCounterTest, HandyBubbleSortMonitoring) {
	HANDY_ENABLE();

	const int SIZE = 50;
	std::vector<int> data(SIZE);
	for (auto& element : data) {
		element = rand();
	}

	int swaps_count = 0;
	for (int step = 0; step < SIZE - 1; ++step) {
		bool sorted = true;
		for (int i = 0; i < SIZE - 1; ++i) {
			if (data[i] > data[i + 1]) {
				std::swap(data[i], data[i + 1]);
				sorted = false;
				swaps_count++;

				HANDY_COUNTER_INCREMENT("swaps.count", 1);
			}
		}

		if (sorted) {
			break;
		}
	}

	while (!handystats::message_queue::event_message_queue->empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_count =
		boost::get<handystats::internal::internal_counter*>(handystats::internal::monitors["swaps.count"])
		->base_counter
		->get().first;

	ASSERT_EQ(handy_count, swaps_count);

	auto counter = boost::get<handystats::internal::internal_counter*>(handystats::internal::monitors["swaps.count"]);
	std::cout << handystats::json::write_to_json_string(counter) << std::endl;

	std::cout << *HANDY_JSON_DUMP() << std::endl;

	HANDY_DISABLE();
}

TEST(HandyGaugeTest, HandyQueueSizeMonitoring) {
	HANDY_ENABLE();

	const int OPER_COUNT = 5000;
	std::queue<int> data;

	int max_queue_size = 0;
	for (int oper = 0; oper < OPER_COUNT; ++oper) {
		if (rand() & 1) {
			data.push(rand());
		}
		else {
			if (!data.empty()) {
				data.pop();
			}
		}
		HANDY_GAUGE_SET("queue.size", data.size());

		if (data.size() > max_queue_size) {
			max_queue_size = data.size();
		}
	}

	while (!handystats::message_queue::event_message_queue->empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_max_size =
		boost::accumulators::max(
			boost::get<handystats::internal::internal_gauge*>(handystats::internal::monitors["queue.size"])
			->base_gauge
			->get_stats()
			.values
		);

	ASSERT_EQ(handy_max_size, max_queue_size);

	auto gauge = boost::get<handystats::internal::internal_gauge*>(handystats::internal::monitors["queue.size"]);
	std::cout << handystats::json::write_to_json_string(gauge) << std::endl;

	std::cout << *HANDY_JSON_DUMP() << std::endl;

	HANDY_DISABLE();
}

TEST(HandyInternalTest, CheckEqualCounterNamesOnRestart) {
	HANDY_ENABLE();

	const int value_1 = 10;
	HANDY_COUNTER_INCREMENT("test.counter", value_1);

	while (!handystats::message_queue::event_message_queue->empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_value_1 =
		boost::get<handystats::internal::internal_counter*>(handystats::internal::monitors["test.counter"])
		->base_counter
		->get().first;

	ASSERT_EQ(handy_value_1, value_1);

	HANDY_DISABLE();

	HANDY_ENABLE();

	const int value_2 = 100;
	HANDY_COUNTER_INCREMENT("test.counter", value_2);

	while (!handystats::message_queue::event_message_queue->empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_value_2 =
		boost::get<handystats::internal::internal_counter*>(handystats::internal::monitors["test.counter"])
		->base_counter
		->get().first;

	ASSERT_EQ(handy_value_2, value_2);

	std::cout << *HANDY_JSON_DUMP() << std::endl;

	HANDY_DISABLE();
}

TEST(HandyInternalTest, CheckEqualMetricNamesOnRestart) {
	HANDY_ENABLE();

	const int value_1 = 10;
	HANDY_COUNTER_INCREMENT("test.metric", value_1);

	while (!handystats::message_queue::event_message_queue->empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_value_1 =
		boost::get<handystats::internal::internal_counter*>(handystats::internal::monitors["test.metric"])
		->base_counter
		->get().first;

	ASSERT_EQ(handy_value_1, value_1);

	HANDY_DISABLE();

	HANDY_ENABLE();

	const int value_2 = 100;
	HANDY_GAUGE_SET("test.metric", value_2);

	while (!handystats::message_queue::event_message_queue->empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_value_2 =
		boost::get<handystats::internal::internal_gauge*>(handystats::internal::monitors["test.metric"])
		->base_gauge
		->get().first;

	ASSERT_EQ(handy_value_2, value_2);

	std::cout << *HANDY_JSON_DUMP() << std::endl;

	HANDY_DISABLE();
}
