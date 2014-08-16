#include <memory>

#include <gtest/gtest.h>

#include <handystats/metrics/gauge.hpp>

#include "events/event_message_impl.hpp"
#include "events/gauge_events_impl.hpp"


using namespace handystats::events::gauge;

TEST(GaugeEventsTest, TestGaugeInitEvent) {
	const char* gauge_name = "proc.load";
	const double init_value = 0.75;
	auto message = create_init_event(gauge_name, init_value, handystats::metrics::gauge::clock::now());

	ASSERT_EQ(message->destination_name, gauge_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::GAUGE);

	ASSERT_EQ(message->event_type, event_type::INIT);
	ASSERT_NEAR(*static_cast<handystats::metrics::gauge::value_type*>(message->event_data[0]), init_value, 1E-6);
}

TEST(GaugeEventsTest, TestGaugeSetEvent) {
	const char* gauge_name = "proc.load";
	const double value = 1.5;
	auto message = create_set_event(gauge_name, value, handystats::metrics::gauge::clock::now());

	ASSERT_EQ(message->destination_name, gauge_name);
	ASSERT_EQ(message->destination_type, handystats::events::event_destination_type::GAUGE);

	ASSERT_EQ(message->event_type, event_type::SET);
	ASSERT_NEAR(*static_cast<handystats::metrics::gauge::value_type*>(message->event_data[0]), value, 1E-6);
}

