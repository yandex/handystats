// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_IMPL_HPP_
#define HANDYSTATS_CONFIG_IMPL_HPP_

#include <vector>

#include <handystats/chrono.hpp>
#include <handystats/rapidjson/document.h>

namespace handystats { namespace config {

struct incremental_statistics_parameters {
	double moving_average_alpha;
	std::chrono::milliseconds moving_interval;

	incremental_statistics_parameters();
	void configure(const rapidjson::Value& incremental_statistics_config);
};

struct timer_parameters {
	std::chrono::milliseconds idle_timeout;

	timer_parameters();
	void configure(const rapidjson::Value& timer_config);
};

struct metrics_dump_parameters {
	std::chrono::milliseconds interval;
	bool to_json;

	metrics_dump_parameters();
	void configure(const rapidjson::Value& metrics_dump_config);
};

extern incremental_statistics_parameters incremental_statistics;
extern timer_parameters timer;
extern metrics_dump_parameters metrics_dump;

void initialize();
void finalize();

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_IMPL_HPP_
