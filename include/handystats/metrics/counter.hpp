/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDYSTATS_METRICS_COUNTER_HPP_
#define HANDYSTATS_METRICS_COUNTER_HPP_

#include <cstdint>

#include <utility>

#include <handystats/chrono.hpp>
#include <handystats/statistics.hpp>
#include <handystats/config/metrics/counter.hpp>

namespace handystats { namespace metrics {

struct counter
{
	typedef int64_t value_type;
	typedef chrono::tsc_clock clock;
	typedef chrono::time_point time_point;

	counter(const config::metrics::counter& opts = config::metrics::counter());

	void init(const value_type& value = 0, const time_point& timestamp = clock::now());
	void increment(const value_type& value = 1, const time_point& timestamp = clock::now());
	void decrement(const value_type& value = 1, const time_point& timestamp = clock::now());

	void update_statistics(const time_point& timestamp = clock::now());

	const statistics& values() const;

private:
	statistics m_values;

	value_type m_value;
	time_point m_timestamp;

}; // struct counter

}} // namespace handystats::metrics


#endif // HANDYSTATS_METRICS_COUNTER_HPP_
