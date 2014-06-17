// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>

#include <handystats/chrono.hpp>
#include <handystats/metrics_dump.hpp>

#include "internal_metrics_impl.hpp"
#include "internal_metrics/internal_counter_impl.hpp"
#include "internal_metrics/internal_gauge_impl.hpp"
#include "internal_metrics/internal_timer_impl.hpp"

#include "configuration_impl.hpp"

#include "metrics_dump_impl.hpp"

namespace handystats { namespace internal {

extern std::map<std::string, internal_metric> internal_metrics;

}} // namespace handystats::internal


namespace handystats { namespace metrics {

chrono::default_time_point metrics_dump_timestamp;
std::mutex metrics_dump_mutex;

std::shared_ptr<const const_metrics_dump> metrics_dump(new const_metrics_dump());

std::shared_ptr<const const_metrics_dump> get_metrics_dump() {
	std::lock_guard<std::mutex> lock(metrics_dump_mutex);
	return metrics_dump;
}

std::shared_ptr<const const_metrics_dump> create_metrics_dump() {
	std::shared_ptr<const_metrics_dump> dump(new const_metrics_dump());

	for (auto metric_entry : internal::internal_metrics) {
		switch (metric_entry.second.which()) {
			case internal::internal_metric_index::INTERNAL_GAUGE:
				dump->insert(
						std::pair<std::string, const_metric>(
							metric_entry.first,
							(const_gauge)*boost::get<internal::internal_gauge*>(metric_entry.second)->base_gauge
						)
					);
				break;
			case internal::internal_metric_index::INTERNAL_COUNTER:
				dump->insert(
						std::pair<std::string, const_metric>(
							metric_entry.first,
							(const_counter)*boost::get<internal::internal_counter*>(metric_entry.second)->base_counter
						)
					);
				break;
			case internal::internal_metric_index::INTERNAL_TIMER:
				dump->insert(
						std::pair<std::string, const_metric>(
							metric_entry.first,
							(const_timer)*boost::get<internal::internal_timer*>(metric_entry.second)->base_timer
						)
					);
				break;
		}
	}

	metrics_dump_timestamp = chrono::default_clock::now();

	return std::const_pointer_cast<const const_metrics_dump>(dump);
}

void update_metrics_dump() {
	if (config::metrics_dump.interval.count() == 0) {
		return;
	}
	if (std::chrono::duration_cast<chrono::default_duration>(chrono::default_clock::now() - metrics_dump_timestamp) > config::metrics_dump.interval) {
		auto new_metrics_dump = create_metrics_dump();
		{
			std::lock_guard<std::mutex> lock(metrics_dump_mutex);
			metrics_dump = new_metrics_dump;
		}
	}
}

}} // namespace handystats::metrics_dump

const std::shared_ptr<const handystats::metrics::const_metrics_dump> HANDY_METRICS_DUMP() {
	return handystats::metrics::get_metrics_dump();
}
