#include <vector>
#include <utility>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <queue>
#include <string>
#include <map>
#include <memory>

#include <gtest/gtest.h>

#include <handystats/operation.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/json_dump.hpp>

#include "events/event_message_impl.hpp"
#include "message_queue_impl.hpp"
#include "internal_metrics_impl.hpp"
#include "internal_metrics/internal_gauge_impl.hpp"
#include "internal_metrics/internal_counter_impl.hpp"


namespace handystats { namespace internal {

extern std::map<std::string, internal_metric> internal_metrics;

}} // namespace handystats::internal


class HandyCounterTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

class HandyGaugeTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(HandyCounterTest, HandyBubbleSortMonitoring) {
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

	while (!handystats::message_queue::empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_count =
		boost::get<handystats::internal::internal_counter*>(handystats::internal::internal_metrics["swaps.count"])
		->base_counter
		->value;

	ASSERT_EQ(handy_count, swaps_count);

	std::cout << *HANDY_JSON_DUMP() << std::endl;
}

TEST_F(HandyGaugeTest, HandyQueueSizeMonitoring) {
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

	while (!handystats::message_queue::empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_max_size =
		boost::accumulators::max(
			boost::get<handystats::internal::internal_gauge*>(handystats::internal::internal_metrics["queue.size"])
			->base_gauge
			->stats
			.values
		);

	ASSERT_EQ(handy_max_size, max_queue_size);

	std::cout << *HANDY_JSON_DUMP() << std::endl;
}

TEST(HandyInternalTest, CheckEqualCounterNamesOnRestart) {
	HANDY_INIT();

	const int value_1 = 10;
	HANDY_COUNTER_INCREMENT("test.counter", value_1);

	while (!handystats::message_queue::empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_value_1 =
		boost::get<handystats::internal::internal_counter*>(handystats::internal::internal_metrics["test.counter"])
		->base_counter
		->value;

	ASSERT_EQ(handy_value_1, value_1);

	HANDY_FINALIZE();

	HANDY_INIT();

	const int value_2 = 100;
	HANDY_COUNTER_INCREMENT("test.counter", value_2);

	while (!handystats::message_queue::empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_value_2 =
		boost::get<handystats::internal::internal_counter*>(handystats::internal::internal_metrics["test.counter"])
		->base_counter
		->value;

	ASSERT_EQ(handy_value_2, value_2);

	std::cout << *HANDY_JSON_DUMP() << std::endl;

	HANDY_FINALIZE();
}

TEST(HandyInternalTest, CheckEqualMetricNamesOnRestart) {
	HANDY_INIT();

	const int value_1 = 10;
	HANDY_COUNTER_INCREMENT("test.metric", value_1);

	while (!handystats::message_queue::empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_value_1 =
		boost::get<handystats::internal::internal_counter*>(handystats::internal::internal_metrics["test.metric"])
		->base_counter
		->value;

	ASSERT_EQ(handy_value_1, value_1);

	HANDY_FINALIZE();

	HANDY_INIT();

	const int value_2 = 100;
	HANDY_GAUGE_SET("test.metric", value_2);

	while (!handystats::message_queue::empty()) {
		std::this_thread::sleep_for(std::chrono::microseconds(10));
	}

	std::this_thread::sleep_for(std::chrono::milliseconds(10));
	int handy_value_2 =
		boost::get<handystats::internal::internal_gauge*>(handystats::internal::internal_metrics["test.metric"])
		->base_gauge
		->value;

	ASSERT_EQ(handy_value_2, value_2);

	std::cout << *HANDY_JSON_DUMP() << std::endl;

	HANDY_FINALIZE();
}
