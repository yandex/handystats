// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <string>
#include <map>

#include <handystats/chrono.hpp>
#include <handystats/metrics.hpp>

#include "events/event_message_impl.hpp"
#include "events/counter_events_impl.hpp"
#include "events/gauge_events_impl.hpp"
#include "events/timer_events_impl.hpp"
#include "events/attribute_events_impl.hpp"
#include "config_impl.hpp"

#include "internal_impl.hpp"

namespace handystats { namespace internal {


namespace stats {

metrics::gauge size;
metrics::gauge process_time;

void initialize() {
	size = metrics::gauge();
	size.set(0);

	process_time = metrics::gauge();
}

void finalize() {
	size = metrics::gauge();
	size.set(0);

	process_time = metrics::gauge();
}

} // namespace stats


std::map<std::string, metrics::metric_ptr_variant> metrics_map;


size_t size() {
	return metrics_map.size();
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
	auto process_start_time = chrono::clock::now();

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
		switch (message.destination_type) {
			case events::event_destination_type::COUNTER:
				metric_ptr = new metrics::counter();
				break;
			case events::event_destination_type::GAUGE:
				metric_ptr = new metrics::gauge();
				break;
			case events::event_destination_type::TIMER:
				metric_ptr = new metrics::timer(config::timer_opts, config::incremental_statistics_opts);
				break;
			case events::event_destination_type::ATTRIBUTE:
				metric_ptr = new metrics::attribute();
				break;
		}
	}

	process_event_message(metric_ptr, message);

	auto process_end_time = chrono::clock::now();

	stats::process_time.set(
			chrono::duration_cast<chrono::time_duration>(process_end_time - process_start_time).count(),
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
