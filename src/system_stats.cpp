// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace system_stats {

metrics::gauge message_queue_size;
metrics::gauge internal_metrics_size;

metrics::gauge message_process_time;
metrics::gauge message_pop_time;


void reset() {
	message_queue_size = metrics::gauge();
	internal_metrics_size = metrics::gauge();

	message_process_time = metrics::gauge();
	message_pop_time = metrics::gauge();
}

void initialize() {
	reset();
}

void finalize() {
	reset();
}

}} // namespace handystats::system_stats
