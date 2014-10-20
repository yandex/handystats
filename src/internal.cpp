// Copyright (c) 2014 Yandex LLC. All rights reserved.

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

metrics::gauge process_time;

void update(const chrono::time_point& timestamp) {
	process_time.update_statistics(timestamp);
}

static void reset() {
	config::metrics::gauge process_time_opts;
	process_time_opts.values.tags = statistics::tag::moving_avg;
	process_time_opts.values.moving_interval = chrono::seconds(1);

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
std::map<std::string, attribute> attributes_map;

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
		}
	}
}

static
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
		default:
			return;
	}
}

static
void process_event_message(attribute& attr, const events::event_message& message) {
	events::attribute::process_event(attr, message);
}

void process_event_message(const events::event_message& message) {
	auto process_start_time = chrono::internal_clock::now();

	if (message.destination_type == events::event_destination_type::ATTRIBUTE) {
		auto& attr = attributes_map[message.destination_name];
		process_event_message(attr, message);
	}
	else {
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
		}

		if (empty_metric) {
			switch (message.destination_type) {
				case events::event_destination_type::COUNTER:
					metric_ptr = new metrics::counter(config::metrics::counter_opts);
					break;
				case events::event_destination_type::GAUGE:
					metric_ptr = new metrics::gauge(config::metrics::gauge_opts);
					break;
				case events::event_destination_type::TIMER:
					metric_ptr = new metrics::timer(config::metrics::timer_opts);
					break;
			}
		}

		process_event_message(metric_ptr, message);
	}

	auto process_end_time = chrono::internal_clock::now();

	stats::process_time.set(
			chrono::duration::convert_to(metrics::timer::value_unit, process_end_time - process_start_time).count(),
			process_end_time
		);
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
			default:
				break;
		}
	}

	metrics_map.clear();
	attributes_map.clear();

	stats::finalize();
}

}} // namespace handystats::internal
