// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_METRICS_DUMP_IMPL_HPP_
#define HANDYSTATS_CONFIG_METRICS_DUMP_IMPL_HPP_

#include <handystats/chrono.hpp>
#include <handystats/rapidjson/document.h>

namespace handystats { namespace config {

struct metrics_dump {
	struct defaults {
		static const chrono::clock::duration interval;
		static const bool to_json;
	};

	chrono::clock::duration interval;
	bool to_json;

	metrics_dump();
	void configure(const rapidjson::Value& config);
};

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_METRICS_DUMP_HPP_
