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

#include <memory>

#include <gtest/gtest.h>

#include <handystats/metrics/gauge.hpp>
#include <handystats/core.hpp>

#include "events/event_message_impl.hpp"
#include "events/gauge_impl.hpp"

class GaugeEventsTest: public ::testing::Test {
protected:
	virtual void SetUp() {
		HANDY_INIT();
	}
	virtual void TearDown() {
		HANDY_FINALIZE();
	}
};

using namespace handystats::events::gauge;

TEST_F(GaugeEventsTest, TestGaugeInitEvent) {
	const char* gauge_name = "proc.load";
	const double init_value = 0.75;
	auto message = create_init_event(gauge_name, init_value, handystats::metrics::gauge::clock::now());

	ASSERT_EQ(message->destination_name, gauge_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::GAUGE);

	ASSERT_EQ(message->event_type, event_type::INIT);
	ASSERT_NEAR(*reinterpret_cast<handystats::metrics::gauge::value_type*>(&message->event_data), init_value, 1E-6);

	delete_event_message(message);
}

TEST_F(GaugeEventsTest, TestGaugeSetEvent) {
	const char* gauge_name = "proc.load";
	const double value = 1.5;
	auto message = create_set_event(gauge_name, value, handystats::metrics::gauge::clock::now());

	ASSERT_EQ(message->destination_name, gauge_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::GAUGE);

	ASSERT_EQ(message->event_type, event_type::SET);
	ASSERT_NEAR(*reinterpret_cast<handystats::metrics::gauge::value_type*>(&message->event_data), value, 1E-6);

	delete_event_message(message);
}

