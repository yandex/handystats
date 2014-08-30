// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>
#include <fstream>
#include <string>
#include <iostream>

#include <rapidjson/document.h>

#include <handystats/core.hpp>
#include <handystats/core.h>

#include "core_impl.hpp"

#include "config_impl.hpp"

namespace handystats { namespace config {

incremental_statistics incremental_statistics_opts;
timer timer_opts;
metrics_dump metrics_dump_opts;
core core_opts;

bool default_initialized = false;

void initialize() {
	incremental_statistics_opts = incremental_statistics();
	timer_opts = timer();
	metrics_dump_opts = metrics_dump();
	core_opts = core();

	default_initialized = true;
}

void finalize() {
	incremental_statistics_opts = incremental_statistics();
	timer_opts = timer();
	metrics_dump_opts = metrics_dump();
	core_opts = core();

	default_initialized = true;
}

}} // namespace handystats::config


namespace {

/*
 * Configure global incremental_statistics opts with rapidjson::Value config
 */
void configure(handystats::config::incremental_statistics& opts, const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("moving-average-alpha")) {
		const rapidjson::Value& moving_average_alpha = config["moving-average-alpha"];
		if (moving_average_alpha.IsNumber()) {
			opts.moving_average_alpha = moving_average_alpha.GetDouble();
		}
	}

	if (config.HasMember("moving-interval")) {
		const rapidjson::Value& moving_interval = config["moving-interval"];
		if (moving_interval.IsUint64()) {
			opts.moving_interval = std::chrono::milliseconds(moving_interval.GetUint64());
		}
	}
}

/*
 * Configure global timer opts with rapidjson::Value config
 */
void configure(handystats::config::timer& opts, const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("idle-timeout")) {
		const rapidjson::Value& idle_timeout = config["idle-timeout"];
		if (idle_timeout.IsUint64()) {
			opts.idle_timeout = std::chrono::milliseconds(idle_timeout.GetUint64());
		}
	}
}

/*
 * Configure global metrics_dump opts with rapidjson::Value config
 */
void configure(handystats::config::metrics_dump& opts, const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("interval")) {
		const rapidjson::Value& interval = config["interval"];
		if (interval.IsUint64()) {
			opts.interval = std::chrono::milliseconds(interval.GetUint64());
		}
	}
}

/*
 * Configure global core opts with rapidjson::Value config
 */
void configure(handystats::config::core& opts, const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("enable")) {
		const rapidjson::Value& enable = config["enable"];
		if (enable.IsBool()) {
			opts.enable = enable.GetBool();
		}
	}
}

/*
 * Configure global opts with rapidjson::Value config
 */
void config_json(const rapidjson::Value& config) {
	std::lock_guard<std::mutex> lock(handystats::operation_mutex);
	if (handystats::is_enabled()) {
		return;
	}

	if (!handystats::config::default_initialized) {
		handystats::config::initialize();
	}

	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("incremental-statistics")) {
		const rapidjson::Value& incremental_statistics_config = config["incremental-statistics"];
		configure(handystats::config::incremental_statistics_opts, incremental_statistics_config);
	}

	if (config.HasMember("timer")) {
		const rapidjson::Value& timer_config = config["timer"];
		configure(handystats::config::timer_opts, timer_config);
	}

	if (config.HasMember("metrics-dump")) {
		const rapidjson::Value& metrics_dump_config = config["metrics-dump"];
		configure(handystats::config::metrics_dump_opts, metrics_dump_config);
	}

	if (config.HasMember("core")) {
		const rapidjson::Value& core_config = config["core"];
		configure(handystats::config::core_opts, core_config);
	}
}

} // unnamed namespace


namespace handystats {

void config_json(const char* config_data) {
	rapidjson::Document config;
	config.Parse<0>(config_data);
	if (config.HasParseError()) {
		std::cerr << "Unable to parse configuration json: " << config.GetParseError() << std::endl;
		return;
	}

	::config_json(config);
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
