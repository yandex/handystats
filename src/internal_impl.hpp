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

#ifndef HANDYSTATS_INTERNAL_IMPL_HPP_
#define HANDYSTATS_INTERNAL_IMPL_HPP_

#include <map>
#include <string>

#include <handystats/metrics.hpp>
#include <handystats/metrics/gauge.hpp>


namespace handystats { namespace events {

struct event_message;

}} // namespace handystats::events


namespace handystats { namespace internal {

extern std::map<std::string, metrics::metric_ptr_variant> metrics_map;

void update_metrics(const chrono::time_point&);

void process_event_message(const events::event_message&);

size_t size();

void initialize();
void finalize();


namespace stats {

extern metrics::gauge size;
extern metrics::gauge process_time;

void update(const chrono::time_point&);

void initialize();
void finalize();

} // namespace stats


}} // namespace handystats::internal


#endif // HANDYSTATS_INTERNAL_IMPL_HPP_
