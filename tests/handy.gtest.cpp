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
#include <handystats/metrics_dump.hpp>
#include <handystats/config.hpp>

#include "message_queue_helper.hpp"

class HandyCounterTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_CONFIG_JSON(
				"{\
					\"metrics-dump\": {\
						\"interval\": 10\
					}\
				}"
			);

		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

class HandyGaugeTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_CONFIG_JSON(
				"{\
					\"metrics-dump\": {\
						\"interval\": 10\
					}\
				}"
			);

		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

TEST_F(HandyCounterTest, HandyBubbleSortMonitoring) {
	const int SIZE = 50;
	std::vector<int> data(SIZE);
	for (auto element = data.begin(); element != data.end(); ++element) {
		*element = rand();
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

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("swaps.count") != metrics_dump->end());

	int handy_count =
		boost::get<handystats::metrics::counter>(metrics_dump->at("swaps.count"))
		.value;

	ASSERT_EQ(handy_count, swaps_count);
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

	handystats::message_queue::wait_until_empty();
	std::this_thread::sleep_for(std::chrono::milliseconds(20));

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->find("queue.size") != metrics_dump->end());

	int handy_max_size =
			boost::get<handystats::metrics::gauge>(metrics_dump->at("queue.size"))
			.values
			.max();

	ASSERT_EQ(handy_max_size, max_queue_size);
}

