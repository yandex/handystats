#include <memory>

#include <handystats/events/event_message.hpp>
#include <handystats/measuring_points/gauge_measuring_points.hpp>


namespace handystats { namespace message_queue {

void push_event_message(std::shared_ptr<events::event_message> message);

}} // namespace handystats::message_queue


namespace handystats { namespace events {

std::shared_ptr<event_message> gauge_init_event(const std::string, metrics::gauge::value_type, metrics::gauge::time_point);
std::shared_ptr<event_message> gauge_set_event(const std::string, metrics::gauge::value_type, metrics::gauge::time_point);

}} // namespace handystats::events


void HANDY_GAUGE_INIT(
		const std::string gauge_name,
		handystats::metrics::gauge::value_type init_value,
		handystats::metrics::gauge::time_point timestamp
		)
{
	auto message = handystats::events::gauge_init_event(gauge_name, init_value, timestamp);
	handystats::message_queue::push_event_message(message);
}

void HANDY_GAUGE_SET(
		const std::string gauge_name,
		handystats::metrics::gauge::value_type value,
		handystats::metrics::gauge::time_point timestamp
		)
{
	auto message = handystats::events::gauge_set_event(gauge_name, value, timestamp);
	handystats::message_queue::push_event_message(message);
}

