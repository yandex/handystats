#include <handystats/metrics/gauge.hpp>


namespace handystats { namespace metrics {

gauge::gauge()
{
}

gauge::gauge(value_type value, time_point timestamp)
	: value(value)
	, timestamp(timestamp)
{
	values(value, timestamp);
}


void gauge::set(value_type value, time_point timestamp) {
	this->value = value;
	this->timestamp = timestamp;

	values(this->value, this->timestamp);
}

}} // namespace handystats::metrics
