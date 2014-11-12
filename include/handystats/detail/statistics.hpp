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

#ifndef HANDYSTATS_STATISTICS_IMPL_HPP_
#define HANDYSTATS_STATISTICS_IMPL_HPP_

#include <string>
#include <map>

#include <handystats/statistics.hpp>
#include <handystats/config/statistics.hpp>

namespace handystats {

struct statistics::data {
	data();
	data(const config::statistics& config);

	void reset();

	void update(const value_type& value, const time_point& timestamp = clock::now());
	void update_time(const time_point& timestamp = clock::now());

	void append(data);
	void merge(const data&);

	void truncate_before(const time_point& timestamp);
	void truncate_after(const time_point& timestamp);

	void fulfill_dependencies();

	bool computed(const tag::type& t) const HANDYSTATS_NOEXCEPT;

	value_type m_value;
	value_type m_min;
	value_type m_max;
	value_type m_sum;
	size_t m_count;
	value_type m_moving_count;
	value_type m_moving_sum;
	histogram_type m_histogram;
	time_point m_data_timestamp;

	time_point m_timestamp;

	// configuration
	chrono::duration m_moving_interval;
	size_t m_histogram_bins;
	int m_tags; // computed tags
};

typedef std::map<std::string, statistics::data> stats_data_map;

} // namespace handystats

#endif // HANDYSTATS_STATISTICS_IMPL_HPP_
