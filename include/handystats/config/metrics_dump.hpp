// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_METRICS_DUMP_HPP_
#define HANDYSTATS_CONFIG_METRICS_DUMP_HPP_

#include <chrono>

namespace handystats { namespace config {

struct metrics_dump {
	struct defaults {
		static const std::chrono::milliseconds interval;
	};

	std::chrono::milliseconds interval;

	metrics_dump();
};

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_METRICS_DUMP_HPP_
