#include <memory>

#include <gtest/gtest.h>

#include <handystats/metrics/gauge.hpp>

#include "events/event_message_impl.hpp"
#include "events/gauge_events_impl.hpp"


namespace handystats { namespace events {

std::shared_ptr<event_message> gauge_init_event(const std::string, metrics::gauge::value_type, metrics::gauge::time_point);
std::shared_ptr<event_message> gauge_set_event(const std::string, metrics::gauge::value_type, metrics::gauge::time_point);

}} // namespace handystats::events


using namespace handystats::events;

TEST(GaugeEventsTest, TestGaugeInitEvent) {
	const std::string gauge_name = "proc.load";
	const double init_value = 0.75;
	auto message = gauge_init_event(gauge_name, init_value, handystats::metrics::gauge::clock::now());

	ASSERT_EQ(message->destination_name, gauge_name);
	ASSERT_EQ(message->destination_type, event_destination_type::GAUGE);

	ASSERT_EQ(*static_cast<gauge_event*>(message->event_type), gauge_event::INIT);
	ASSERT_NEAR(*static_cast<handystats::metrics::gauge::value_type*>(message->event_data[0]), init_value, 1E-6);
}

TEST(GaugeEventsTest, TestGaugeSetEvent) {
	const std::string gauge_name = "proc.load";
	const double value = 1.5;
	auto message = gauge_set_event(gauge_name, value, handystats::metrics::gauge::clock::now());

	ASSERT_EQ(message->destination_name, gauge_name);
	ASSERT_EQ(message->destination_type, event_destination_type::GAUGE);

	ASSERT_EQ(*static_cast<gauge_event*>(message->event_type), gauge_event::SET);
	ASSERT_NEAR(*static_cast<handystats::metrics::gauge::value_type*>(message->event_data[0]), value, 1E-6);
}

