// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_STATISTICS_IMPL_HPP_
#define HANDYSTATS_CONFIG_STATISTICS_IMPL_HPP_

#include <handystats/config/statistics.hpp>

#include "rapidjson/document.h"

namespace handystats { namespace config {

void apply(const rapidjson::Value& config, statistics& statistics_opts);

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_STATISTICS_IMPL_HPP_
