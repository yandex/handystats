/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <handystats/metrics/counter.hpp>


namespace handystats { namespace metrics {

counter::counter(const config::metrics::counter& opts)
	: m_values(opts.values)
	, m_value()
	, m_timestamp(chrono::nanoseconds(0), chrono::clock_type::SYSTEM_CLOCK)
{
}

void counter::reset() {
	m_value = value_type();
	m_timestamp = chrono::time_point();

	m_values.reset();
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
