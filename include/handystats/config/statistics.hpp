// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_INCREMENTAL_STATISTICS_HPP_
#define HANDYSTATS_CONFIG_INCREMENTAL_STATISTICS_HPP_

#include <handystats/chrono.hpp>
#include <handystats/rapidjson/document.h>

namespace handystats { namespace config {

struct statistics {
	chrono::duration moving_interval;
	size_t histogram_bins;
	int tags;
	chrono::time_unit rate_unit;

	statistics();
	void configure(const rapidjson::Value& config);
};

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_INCREMENTAL_STATISTICS_HPP_
