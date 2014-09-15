// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_CORE_IMPL_HPP_
#define HANDYSTATS_CONFIG_CORE_IMPL_HPP_

#include <handystats/rapidjson/document.h>

namespace handystats { namespace config {

struct core {
	bool enable;

	core();
	void configure(const rapidjson::Value& config);
};

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_CORE_HPP_
