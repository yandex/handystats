// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/counter.hpp>


namespace handystats { namespace metrics {

counter::counter(const config::statistics& opts)
	: m_values(opts)
	, m_incr_deltas(opts)
	, m_decr_deltas(opts)
	, m_deltas(opts)
	, m_value()
	, m_timestamp()
{
}


void counter::init(const value_type& init_value, const time_point& timestamp) {
	m_value = init_value;
	m_timestamp = timestamp;

	m_values.reset();
	m_incr_deltas.reset();
	m_decr_deltas.reset();
	m_deltas.reset();

	m_values.update(m_value, m_timestamp);
}

void counter::increment(const value_type& incr_value, const time_point& timestamp) {
	if (m_timestamp == time_point()) {
		init(0, timestamp);
	}

	m_value += incr_value;
	if (m_timestamp < timestamp) {
		m_timestamp = timestamp;
	}

	m_values.update(m_value, m_timestamp);
	m_incr_deltas.update(incr_value, m_timestamp);
	m_deltas.update(incr_value, m_timestamp);
}

void counter::decrement(const value_type& decr_value, const time_point& timestamp) {
	if (m_timestamp == time_point()) {
		init(0, timestamp);
	}

	m_value -= decr_value;
	if (m_timestamp < timestamp) {
		m_timestamp = timestamp;
	}

	m_values.update(m_value, m_timestamp);
	m_decr_deltas.update(decr_value, m_timestamp);
	m_deltas.update(-decr_value, m_timestamp);
}

void counter::update_statistics(const time_point& timestamp) {
	m_values.update_time(timestamp);
	m_incr_deltas.update_time(timestamp);
	m_decr_deltas.update_time(timestamp);
	m_deltas.update_time(timestamp);
}

const statistics& counter::values() const {
	return m_values;
}

const statistics& counter::incr_deltas() const {
	return m_incr_deltas;
}

const statistics& counter::decr_deltas() const {
	return m_decr_deltas;
}

const statistics& counter::deltas() const {
	return m_deltas;
}

}} // namespace handystats::metrics
