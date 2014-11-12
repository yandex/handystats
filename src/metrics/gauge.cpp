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

#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace metrics {

gauge::gauge(const config::metrics::gauge& opts)
	: m_values(opts.values)
{
}

void gauge::reset() {
	m_values.reset();
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
