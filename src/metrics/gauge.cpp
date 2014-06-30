// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/gauge.hpp>


namespace handystats { namespace metrics {

gauge::gauge()
{
	value = value_type();
	timestamp = time_point();
}

gauge::gauge(const value_type& value, const time_point& timestamp)
	: value(value)
	, timestamp(timestamp)
{
	values(value, timestamp);
}


void gauge::set(const value_type& value, const time_point& timestamp) {
	this->value = value;
	this->timestamp = timestamp;

	values(this->value, this->timestamp);
}

}} // namespace handystats::metrics
