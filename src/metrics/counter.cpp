// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/counter.hpp>


namespace handystats { namespace metrics {

counter::counter(const config::metrics::counter& opts)
	: m_values(opts.values)
	, m_value()
	, m_timestamp(chrono::nanoseconds(0), chrono::clock_type::SYSTEM)
{
}


void counter::init(const value_type& init_value, const time_point& timestamp) {
	m_value = init_value;
	m_timestamp = timestamp;

	m_values.reset();

	m_values.update(m_value, m_timestamp);
}

void counter::increment(const value_type& incr_value, const time_point& timestamp) {
	if (m_timestamp.time_since_epoch().count() == 0) {
		init(0, timestamp);
	}

	m_value += incr_value;
	if (m_timestamp < timestamp) {
		m_timestamp = timestamp;
	}

	m_values.update(m_value, m_timestamp);
}

void counter::decrement(const value_type& decr_value, const time_point& timestamp) {
	if (m_timestamp.time_since_epoch().count() == 0) {
		init(0, timestamp);
	}

	m_value -= decr_value;
	if (m_timestamp < timestamp) {
		m_timestamp = timestamp;
	}

	m_values.update(m_value, m_timestamp);
}

void counter::update_statistics(const time_point& timestamp) {
	m_values.update_time(timestamp);
}

const statistics& counter::values() const {
	return m_values;
}

}} // namespace handystats::metrics
