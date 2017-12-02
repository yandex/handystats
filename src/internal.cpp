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

#include <string>
#include <map>

#include <handystats/chrono.hpp>
#include <handystats/metrics.hpp>

#include "events/event_message_impl.hpp"
#include "events/counter_impl.hpp"
#include "events/gauge_impl.hpp"
#include "events/timer_impl.hpp"
#include "events/attribute_impl.hpp"
#include "config_impl.hpp"

#include "internal_impl.hpp"

namespace handystats { namespace internal {


namespace stats {

metrics::gauge size;
metrics::gauge process_time;

void update(const chrono::time_point& timestamp) {
	size.update_statistics(timestamp);
	process_time.update_statistics(timestamp);
}

static void reset() {
	config::metrics::gauge size_opts;
	size_opts.values.tags = statistics::tag::value;

	size = metrics::gauge(size_opts);
	size.set(0);

	config::metrics::gauge process_time_opts;
	process_time_opts.values.tags = statistics::tag::moving_avg;
	process_time_opts.values.moving_interval = chrono::duration(1, chrono::time_unit::SEC);

	process_time = metrics::gauge(process_time_opts);
}

void initialize() {
	reset();
}

void finalize() {
	reset();
}

} // namespace stats


std::map<std::string, metrics::metric_ptr_variant> metrics_map;

size_t size() {
	return metrics_map.size();
}

void update_metrics(const chrono::time_point& timestamp) {
	for (auto metric_iter = metrics_map.begin(); metric_iter != metrics_map.end(); ++metric_iter) {
		switch (metric_iter->second.which()) {
			case metrics::metric_index::GAUGE:
			{
				auto* gauge = boost::get<metrics::gauge*>(metric_iter->second);
				gauge->update_statistics(timestamp);
				break;
			}
			case metrics::metric_index::COUNTER:
			{
				auto* counter = boost::get<metrics::counter*>(metric_iter->second);
				counter->update_statistics(timestamp);
				break;
			}
			case metrics::metric_index::TIMER:
			{
				auto* timer = boost::get<metrics::timer*>(metric_iter->second);
				timer->update_statistics(timestamp);
				break;
			}
			case metrics::metric_index::ATTRIBUTE:
				break;
		}
	}
}

void process_event_message(metrics::metric_ptr_variant& metric_ptr, const events::event_message& message) {
	switch (metric_ptr.which()) {
		case metrics::metric_index::COUNTER:
			events::counter::process_event(*boost::get<metrics::counter*>(metric_ptr), message);
			break;
		case metrics::metric_index::GAUGE:
			events::gauge::process_event(*boost::get<metrics::gauge*>(metric_ptr), message);
			break;
		case metrics::metric_index::TIMER:
			events::timer::process_event(*boost::get<metrics::timer*>(metric_ptr), message);
			break;
		case metrics::metric_index::ATTRIBUTE:
			events::attribute::process_event(*boost::get<metrics::attribute*>(metric_ptr), message);
			break;
		default:
			return;
	}
}

void process_event_message(const events::event_message& message) {
	auto process_start_time = chrono::tsc_clock::now();

	auto& metric_ptr = metrics_map[message.destination_name];

	bool empty_metric = false;

	switch (metric_ptr.which()) {
		case metrics::metric_index::COUNTER:
			if (boost::get<metrics::counter*>(metric_ptr) == 0) {
				empty_metric = true;
			}
			break;
		case metrics::metric_index::GAUGE:
			if (boost::get<metrics::gauge*>(metric_ptr) == 0) {
				empty_metric = true;
			}
			break;
		case metrics::metric_index::TIMER:
			if (boost::get<metrics::timer*>(metric_ptr) == 0) {
				empty_metric = true;
			}
			break;
		case metrics::metric_index::ATTRIBUTE:
			if (boost::get<metrics::attribute*>(metric_ptr) == 0) {
				empty_metric = true;
			}
			break;
	}

	if (empty_metric) {
		rapidjson::Value* pattern_cfg = config::select_pattern(message.destination_name);

		switch (message.destination_type) {
			case events::event_destination_type::COUNTER:
				{
					auto counter_opts = config::metrics::counter_opts;
					if (pattern_cfg) {
						configure(counter_opts, *pattern_cfg);
					}
					metric_ptr = new metrics::counter(counter_opts);
					break;
				}
			case events::event_destination_type::GAUGE:
				{
					auto gauge_opts = config::metrics::gauge_opts;
					if (pattern_cfg) {
						configure(gauge_opts, *pattern_cfg);
					}
					metric_ptr = new metrics::gauge(gauge_opts);
					break;
				}
			case events::event_destination_type::TIMER:
				{
					auto timer_opts = config::metrics::timer_opts;
					if (pattern_cfg) {
						configure(timer_opts, *pattern_cfg);
					}
					metric_ptr = new metrics::timer(timer_opts);
					break;
				}
			case events::event_destination_type::ATTRIBUTE:
				{
					metric_ptr = new metrics::attribute();
					break;
				}
		}
	}

	process_event_message(metric_ptr, message);

	auto process_end_time = chrono::tsc_clock::now();

	stats::process_time.set(
			chrono::duration::convert_to(metrics::timer::value_unit, process_end_time - process_start_time).count(),
			process_end_time
		);

	stats::size.set(size(), process_end_time);
}


void initialize() {
	stats::initialize();
}

void finalize() {
	for (auto metric_iter = metrics_map.begin(); metric_iter != metrics_map.end(); ++metric_iter) {
		switch (metric_iter->second.which()) {
			case metrics::metric_index::COUNTER:
				delete boost::get<metrics::counter*>(metric_iter->second);
				break;
			case metrics::metric_index::GAUGE:
				delete boost::get<metrics::gauge*>(metric_iter->second);
				break;
			case metrics::metric_index::TIMER:
				delete boost::get<metrics::timer*>(metric_iter->second);
				break;
			case metrics::metric_index::ATTRIBUTE:
				delete boost::get<metrics::attribute*>(metric_iter->second);
				break;
			default:
				break;
		}
	}

	metrics_map.clear();

	stats::finalize();
}

}} // namespace handystats::internal
