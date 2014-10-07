// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_CORE_IMPL_HPP_
#define HANDYSTATS_CONFIG_CORE_IMPL_HPP_

#include "rapidjson/document.h"

namespace handystats { namespace config {

struct core {
	bool enable;

	core();
};

void apply(const rapidjson::Value& config, core& core_opts);

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_CORE_HPP_
