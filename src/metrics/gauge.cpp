// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/gauge.hpp>


namespace handystats { namespace metrics {

gauge::gauge()
{
	value = value_type();
	timestamp = time_point();
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
