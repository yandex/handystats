// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>
#include <fstream>
#include <string>
#include <iostream>

#include <handystats/config.hpp>
#include <handystats/config.h>

#include "core_impl.hpp"

#include "config_impl.hpp"

namespace handystats { namespace config {

incremental_statistics incremental_statistics_opts;
timer timer_opts;
metrics_dump metrics_dump_opts;

bool default_initialized = false;

void initialize() {
	incremental_statistics_opts = incremental_statistics();
	timer_opts = timer();
	metrics_dump_opts = metrics_dump();

	default_initialized = true;
}

void finalize() {
	incremental_statistics_opts = incremental_statistics();
	timer_opts = timer();
	metrics_dump_opts = metrics_dump();

	default_initialized = true;
}

}} // namespace handystats::config


namespace handystats {

void config_json(const rapidjson::Value& config) {
	std::lock_guard<std::mutex> lock(handystats::operation_mutex);
	if (handystats::is_enabled()) {
		return;
	}

	if (!config::default_initialized) {
		config::initialize();
	}

	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("incremental-statistics")) {
		const rapidjson::Value& incremental_statistics_config = config["incremental-statistics"];
		config::incremental_statistics_opts.configure(incremental_statistics_config);
	}

	if (config.HasMember("timer")) {
		const rapidjson::Value& timer_config = config["timer"];
		config::timer_opts.configure(timer_config);
	}

	if (config.HasMember("metrics-dump")) {
		const rapidjson::Value& metrics_dump_config = config["metrics-dump"];
		config::metrics_dump_opts.configure(metrics_dump_config);
	}
}

void config_json(const char* config_data) {
	rapidjson::Document config;
	config.Parse<0>(config_data);
	if (config.HasParseError()) {
		std::cerr << "Unable to parse configuration json: " << config.GetParseError() << std::endl;
		return;
	}

	config_json(config);
}

void config_file(const char* filename) {
	std::ifstream input(filename, std::ios::in | std::ios::binary);
	if (!input) {
		std::cerr << "Unable to open configuration file " << filename << std::endl;
		return;
	}

	std::string config_data;
	input.seekg(0, std::ios::end);
	config_data.resize(input.tellg());
	input.seekg(0, std::ios::beg);
	input.read(&config_data[0], config_data.size());
	input.close();

	config_json(config_data.c_str());
}

} // namespace handystats


extern "C" {

void handystats_config_file(const char* file) {
	handystats::config_file(file);
}

void handystats_config_json(const char* config_data) {
	handystats::config_json(config_data);
}

} // extern "C"
