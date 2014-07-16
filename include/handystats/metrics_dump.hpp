// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_METRICS_DUMP_H_
#define HANDYSTATS_METRICS_DUMP_H_

#include <memory>
#include <string>
#include <map>

#include <handystats/metrics.hpp>

const std::shared_ptr <
	const std::map <
		std::string, handystats::metrics::metric_variant
		>
	>
HANDY_METRICS_DUMP();

#endif // HANDYSTATS_METRICS_DUMP_H_
