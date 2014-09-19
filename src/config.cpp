// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>
#include <fstream>
#include <string>
#include <iostream>

#include <handystats/core.hpp>
#include <handystats/core.h>

#include "core_impl.hpp"

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

bool config_json(const rapidjson::Value& config) {
	std::lock_guard<std::mutex> lock(handystats::operation_mutex);
	if (handystats::is_enabled()) {
		return true;
	}

	if (!config.IsObject()) {
		return false;
	}

	if (config.HasMember("statistics")) {
		const rapidjson::Value& statistics_config = config["statistics"];

		config::statistics_opts.configure(statistics_config);

		config::metrics::gauge_opts.values.configure(statistics_config);

		config::metrics::counter_opts.values.configure(statistics_config);
		config::metrics::counter_opts.incr_deltas.configure(statistics_config);
		config::metrics::counter_opts.decr_deltas.configure(statistics_config);
		config::metrics::counter_opts.deltas.configure(statistics_config);

		config::metrics::timer_opts.values.configure(statistics_config);
	}

	if (config.HasMember("metrics")) {
		const rapidjson::Value& metrics_config = config["metrics"];

		if (metrics_config.HasMember("gauge")) {
			config::metrics::gauge_opts.configure(metrics_config["gauge"]);
		}
		if (metrics_config.HasMember("counter")) {
			config::metrics::counter_opts.configure(metrics_config["counter"]);
		}
		if (metrics_config.HasMember("timer")) {
			config::metrics::timer_opts.configure(metrics_config["timer"]);
		}
	}

	if (config.HasMember("metrics-dump")) {
		const rapidjson::Value& metrics_dump_config = config["metrics-dump"];
		config::metrics_dump_opts.configure(metrics_dump_config);
	}

	if (config.HasMember("core")) {
		const rapidjson::Value& core_config = config["core"];
		config::core_opts.configure(core_config);
	}

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
