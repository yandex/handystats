// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>
#include <fstream>
#include <string>
#include <iostream>

#include <handystats/core.hpp>
#include <handystats/core.h>

#include "core_impl.hpp"
#include "rapidjson/document.h"

#include "config_impl.hpp"

namespace handystats { namespace config {

statistics statistics_opts;

namespace metrics {
	gauge gauge_opts;
	counter counter_opts;
	timer timer_opts;
}

metrics_dump metrics_dump_opts;
core core_opts;

static void reset() {
	statistics_opts = statistics();

	metrics::gauge_opts = metrics::gauge();
	metrics::counter_opts = metrics::counter();
	metrics::timer_opts = metrics::timer();

	metrics_dump_opts = metrics_dump();
	core_opts = core();
}

__attribute__((constructor(300)))
static void init_opts() {
	reset();
}

void initialize() {
	reset();
}

void finalize() {
	reset();
}

}} // namespace handystats::config


namespace handystats {

static
bool config_json(const rapidjson::Value& config) {
	if (core) {
		return false;
	}

	if (!config.IsObject()) {
		return false;
	}

	if (config.HasMember("defaults")) {
		const rapidjson::Value& statistics_config = config["defaults"];

		config::apply(statistics_config, config::statistics_opts);
		config::apply(statistics_config, config::metrics::gauge_opts.values);
		config::apply(statistics_config, config::metrics::counter_opts.values);
		config::apply(statistics_config, config::metrics::timer_opts.values);
	}

	if (config.HasMember("metrics")) {
		const rapidjson::Value& metrics_config = config["metrics"];

		if (metrics_config.HasMember("gauge")) {
			config::apply(metrics_config["gauge"], config::metrics::gauge_opts);
		}
		if (metrics_config.HasMember("counter")) {
			config::apply(metrics_config["counter"], config::metrics::counter_opts);
		}
		if (metrics_config.HasMember("timer")) {
			config::apply(metrics_config["timer"], config::metrics::timer_opts);
		}
	}

	config::apply(config, config::metrics_dump_opts);

	config::apply(config, config::core_opts);

	return true;
}

bool config_json(const char* config_data) {
	rapidjson::Document config;
	config.Parse<0>(config_data);
	if (config.HasParseError()) {
		std::cerr << "Unable to parse configuration json: " << config.GetParseError() << std::endl;
		return false;
	}

	return config_json(config);
}

bool config_file(const char* filename) {
	std::ifstream input(filename, std::ios::in | std::ios::binary);
	if (!input) {
		std::cerr << "Unable to open configuration file " << filename << std::endl;
		return false;
	}

	std::string config_data;
	input.seekg(0, std::ios::end);
	config_data.resize(input.tellg());
	input.seekg(0, std::ios::beg);
	input.read(&config_data[0], config_data.size());
	input.close();

	return config_json(config_data.c_str());
}

} // namespace handystats


extern "C" {

int handystats_config_file(const char* file) {
	return handystats::config_file(file);
}

int handystats_config_json(const char* config_data) {
	return handystats::config_json(config_data);
}

} // extern "C"
