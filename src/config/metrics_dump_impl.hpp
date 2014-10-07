// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_METRICS_DUMP_IMPL_HPP_
#define HANDYSTATS_CONFIG_METRICS_DUMP_IMPL_HPP_

#include <handystats/chrono.hpp>

#include "rapidjson/document.h"

namespace handystats { namespace config {

struct metrics_dump {
	chrono::duration interval;

	metrics_dump();
};

void apply(const rapidjson::Value& config, metrics_dump& metrics_dump_opts);

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_METRICS_DUMP_HPP_
