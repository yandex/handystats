// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_INCREMENTAL_STATISTICS_HPP_
#define HANDYSTATS_CONFIG_INCREMENTAL_STATISTICS_HPP_

#include <chrono>

#include <handystats/rapidjson/document.h>

namespace handystats { namespace config {

struct statistics {
	struct defaults {
		static const std::chrono::milliseconds moving_interval;
		static const size_t histogram_bins;
		static const int tag_mask;
	};

	std::chrono::milliseconds moving_interval;
	size_t histogram_bins;
	int tag_mask;

	statistics();
	void configure(const rapidjson::Value& config);
};

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_INCREMENTAL_STATISTICS_HPP_
