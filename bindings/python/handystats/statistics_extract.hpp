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

#ifndef HANDYSTATS_BINDINGS_PYTHON_STATISTICS_EXTRACT_HPP_
#define HANDYSTATS_BINGINDS_PYTHON_STATISTICS_EXTRACT_HPP_

#include "handystats/statistics.hpp"

namespace handystats { namespace bindings { namespace python { namespace detail {

inline
double extract_value(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::value>(data);
}

inline
double extract_min(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::min>(data);
}

inline
double extract_max(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::max>(data);
}

inline
size_t extract_count(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::count>(data);
}

inline
double extract_sum(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::sum>(data);
}

inline
double extract_avg(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::avg>(data);
}

inline
double extract_moving_count(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::moving_count>(data);
}

inline
double extract_moving_sum(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::moving_sum>(data);
}

inline
double extract_moving_avg(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::moving_avg>(data);
}

inline
double extract_quantile(const statistics::data& data, const double& level) {
	return statistics::get_impl<statistics::tag::quantile>(data).at(level);
}

inline
double extract_entropy(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::entropy>(data);
}

inline
double extract_throughput(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::throughput>(data);
}

inline
double extract_frequency(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::frequency>(data);
}

inline
chrono::time_point extract_timestamp(const statistics::data& data) {
	return statistics::get_impl<statistics::tag::timestamp>(data);
}

}}}} // namespace handystats::bindings::python::detail

#endif // HANDYSTATS_BINDINGS_PYTHON_STATISTICS_EXTRACT_HPP_
