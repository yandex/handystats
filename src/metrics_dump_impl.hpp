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

#ifndef HANDYSTATS_METRICS_DUMP_IMPL_HPP_
#define HANDYSTATS_METRICS_DUMP_IMPL_HPP_

#include <string>
#include <memory>
#include <map>

#include <handystats/chrono.hpp>
#include <handystats/metrics.hpp>
#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace metrics_dump {

extern chrono::time_point dump_timestamp;

void update(const chrono::time_point& system_time, const chrono::time_point& internal_time);

const std::shared_ptr<const std::map<std::string, metrics::metric_variant>> get_dump();

void initialize();
void finalize();


namespace stats {

extern metrics::gauge dump_time;

void update(const chrono::time_point&);

void initialize();
void finalize();

} // namespace stats


}} // namespace handystats::metrics_dump

#endif // HANDYSTATS_METRICS_DUMP_IMPL_HPP_
