// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_METRICS_DUMP_HPP_
#define HANDYSTATS_CONFIG_METRICS_DUMP_HPP_

#include <chrono>

#include <handystats/rapidjson/document.h>

namespace handystats { namespace config {

struct metrics_dump {
	struct defaults {
		static const std::chrono::milliseconds interval;
		static const bool to_json;
	};

	std::chrono::milliseconds interval;
	bool to_json;

	metrics_dump();
	void configure(const rapidjson::Value& config);
};

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIG_METRICS_DUMP_HPP_
