// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/counter.hpp>


namespace handystats { namespace metrics {

counter::counter(value_type value, time_point timestamp)
	: value(value)
	, timestamp(timestamp)
{
	values(this->value, this->timestamp);
}


void counter::increment(value_type incr_value, time_point timestamp) {
	this->value += incr_value;
	this->timestamp = timestamp;

	values(this->value, this->timestamp);
	deltas(incr_value, this->timestamp);
	incr_deltas(incr_value, this->timestamp);
}

void counter::decrement(value_type decr_value, time_point timestamp) {
	this->value -= decr_value;
	this->timestamp = timestamp;

	values(this->value, this->timestamp);
	deltas(decr_value, this->timestamp);
	decr_deltas(decr_value, this->timestamp);
}

}} // namespace handystats::metrics
