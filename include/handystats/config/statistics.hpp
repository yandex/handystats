// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_INCREMENTAL_STATISTICS_HPP_
#define HANDYSTATS_CONFIG_INCREMENTAL_STATISTICS_HPP_

#include <handystats/chrono.hpp>

namespace handystats { namespace config {

struct statistics {
	chrono::duration moving_interval;
	size_t histogram_bins;
	int tags;

	statistics();
};

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_INCREMENTAL_STATISTICS_HPP_
