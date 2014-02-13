#include <gtest/gtest.h>

#include "handystats/events/gauge_events.hpp"

using namespace handystats::events;

TEST(GaugeEventsTest, TestGaugeInitEvent) {
	const std::string gauge_name = "proc.load";
	const double init_value = 0.75;
	event_message* message = gauge_init_event(gauge_name, init_value);

	ASSERT_EQ(message->destination_name, gauge_name);
	ASSERT_EQ(message->destination_type, event_destination_type::GAUGE);

	ASSERT_EQ(*static_cast<gauge_event*>(message->event_type), gauge_event::INIT);
	ASSERT_NEAR(*static_cast<handystats::metrics::gauge::value_type*>(message->event_data[0]), init_value, 1E-6);

	delete_gauge_event(message);
}

TEST(GaugeEventsTest, TestGaugeSetEvent) {
	const std::string gauge_name = "proc.load";
	const double value = 1.5;
	event_message* message = gauge_set_event(gauge_name, value);

	ASSERT_EQ(message->destination_name, gauge_name);
	ASSERT_EQ(message->destination_type, event_destination_type::GAUGE);

	ASSERT_EQ(*static_cast<gauge_event*>(message->event_type), gauge_event::SET);
	ASSERT_NEAR(*static_cast<handystats::metrics::gauge::value_type*>(message->event_data[0]), value, 1E-6);

	delete_gauge_event(message);
}

