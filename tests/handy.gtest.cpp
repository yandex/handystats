/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

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

#include <handystats/core.hpp>
#include <handystats/measuring_points.hpp>
#include <handystats/metrics_dump.hpp>

#include "message_queue_helper.hpp"
#include "metrics_dump_helper.hpp"

class HandyCounterTest : public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_CONFIG_JSON(
				"{\
					\"dump-interval\": 10\
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
					\"dump-interval\": 10\
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

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find("swaps.count") != metrics_dump->first.end());

	int handy_count = metrics_dump->first.at("swaps.count").get<handystats::statistics::tag::value>();

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

	handystats::wait_until_empty();
	handystats::metrics_dump::wait_until(handystats::chrono::system_clock::now());

	auto metrics_dump = HANDY_METRICS_DUMP();

	ASSERT_TRUE(metrics_dump->first.find("queue.size") != metrics_dump->first.end());

	int handy_max_size = metrics_dump->first.at("queue.size").get<handystats::statistics::tag::max>();

	ASSERT_EQ(handy_max_size, max_queue_size);
}
