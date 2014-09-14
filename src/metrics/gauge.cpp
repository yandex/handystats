// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace metrics {

gauge::gauge(const config::metrics::gauge& opts)
	: m_values(opts.values)
{
}

void gauge::set(const value_type& value, const time_point& timestamp) {
	m_values.update(value, timestamp);
}

void gauge::update_statistics(const time_point& timestamp) {
	m_values.update_time(timestamp);
}

const statistics& gauge::values() const {
	return m_values;
}

}} // namespace handystats::metrics
