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

#ifndef HANDYSTATS_METRICS_GAUGE_HPP_
#define HANDYSTATS_METRICS_GAUGE_HPP_

#include <utility>

#include <handystats/chrono.hpp>
#include <handystats/statistics.hpp>
#include <handystats/config/metrics/gauge.hpp>

namespace handystats { namespace metrics {

struct gauge
{
	typedef double value_type;
	typedef chrono::internal_clock clock;
	typedef chrono::time_point time_point;

	gauge(const config::metrics::gauge& opts = config::metrics::gauge());
	void reset();

	void set(const value_type& value, const time_point& timestamp = clock::now());

	void update_statistics(const time_point& timestamp = clock::now());

	const statistics& values() const;

private:
	statistics m_values;

}; // struct gauge

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_GAUGE_HPP_
