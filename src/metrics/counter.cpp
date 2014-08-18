// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/counter.hpp>


namespace handystats { namespace metrics {

counter::counter()
	: value()
	, timestamp()
{
}


void counter::init(const value_type& init_value, const time_point& timestamp) {
	this->value = init_value;
	this->timestamp = timestamp;

	values.clear();
	deltas.clear();
	incr_deltas.clear();
	decr_deltas.clear();

	values(this->value, this->timestamp);
}

void counter::increment(const value_type& incr_value, const time_point& timestamp) {
	if (this->timestamp == time_point()) {
		init(0, timestamp);
	}

	this->value += incr_value;
	this->timestamp = timestamp;

	values(this->value, this->timestamp);
	deltas(incr_value, this->timestamp);
	incr_deltas(incr_value, this->timestamp);
}

void counter::decrement(const value_type& decr_value, const time_point& timestamp) {
	if (this->timestamp == time_point()) {
		init(0, timestamp);
	}

	this->value -= decr_value;
	this->timestamp = timestamp;

	values(this->value, this->timestamp);
	deltas(-decr_value, this->timestamp);
	decr_deltas(decr_value, this->timestamp);
}

}} // namespace handystats::metrics
