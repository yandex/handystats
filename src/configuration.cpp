// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <mutex>
#include <fstream>
#include <string>
#include <iostream>

#include <handystats/operation.hpp>

#include <handystats/configuration.hpp>
#include <handystats/configuration/defaults.hpp>
#include "configuration_impl.hpp"

namespace handystats { namespace config { namespace defaults {

namespace incremental_statistics {

const double moving_average_alpha = 2.0 / 16;
const std::chrono::milliseconds moving_interval = std::chrono::seconds(1);

}

namespace timer {

const std::chrono::milliseconds idle_timeout = std::chrono::seconds(10);

}

namespace metrics_dump {

const std::chrono::milliseconds interval = std::chrono::milliseconds(500);
const bool to_json = true;

}

}}} // namespace handystats::config::defaults


namespace handystats { namespace config {

incremental_statistics_parameters::incremental_statistics_parameters() {
	moving_average_alpha = defaults::incremental_statistics::moving_average_alpha;
	moving_interval = defaults::incremental_statistics::moving_interval;
}

void incremental_statistics_parameters::configure(const rapidjson::Value& incremental_statistics_config) {
	if (incremental_statistics_config.HasMember("moving-average-alpha")) {
		const rapidjson::Value& moving_average_alpha = incremental_statistics_config["moving-average-alpha"];
		if (moving_average_alpha.IsNumber()) {
			this->moving_average_alpha = moving_average_alpha.GetDouble();
		}
	}
	if (incremental_statistics_config.HasMember("moving-interval")) {
		const rapidjson::Value& moving_interval = incremental_statistics_config["moving-interval"];
		if (moving_interval.IsUint64()) {
			this->moving_interval = std::chrono::milliseconds(moving_interval.GetUint64());
		}
	}
}


timer_parameters::timer_parameters() {
	idle_timeout = defaults::timer::idle_timeout;
}

void timer_parameters::configure(const rapidjson::Value& timer_config) {
	if (timer_config.HasMember("idle-timeout")) {
		const rapidjson::Value& idle_timeout = timer_config["idle-timeout"];
		if (idle_timeout.IsUint64()) {
			this->idle_timeout = std::chrono::milliseconds(idle_timeout.GetUint64());
		}
	}
}


metrics_dump_parameters::metrics_dump_parameters() {
	interval = defaults::metrics_dump::interval;
	to_json = defaults::metrics_dump::to_json;
}

void metrics_dump_parameters::configure(const rapidjson::Value& metrics_dump_config) {
	if (metrics_dump_config.HasMember("interval")) {
		const rapidjson::Value& interval = metrics_dump_config["interval"];
		if (interval.IsUint64()) {
			this->interval = std::chrono::milliseconds(interval.GetUint64());
		}
	}

	if (metrics_dump_config.HasMember("to-json")) {
		const rapidjson::Value& to_json = metrics_dump_config["to-json"];
		if (to_json.IsBool()) {
			this->to_json = to_json.GetBool();
		}
	}
}


incremental_statistics_parameters incremental_statistics;
timer_parameters timer;
metrics_dump_parameters metrics_dump;

void initialize() {
}

void finalize() {
	incremental_statistics = incremental_statistics_parameters();
	timer = timer_parameters();
	metrics_dump = metrics_dump_parameters();
}

}} // namespace handystats::config


namespace handystats {

extern std::mutex operation_mutex;
bool is_enabled();

} // namespace handystats


void HANDY_CONFIGURATION_FILE(const char* filename) {
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

	HANDY_CONFIGURATION_JSON(config_data.c_str());
}

void HANDY_CONFIGURATION_JSON(const char* config_data) {
	rapidjson::Document config;
	config.Parse<0>(config_data);
	if (config.HasParseError()) {
		std::cerr << "Unable to parse configuration json: " << config.GetParseError() << std::endl;
		return;
	}

	HANDY_CONFIGURATION_JSON(config);
}

void HANDY_CONFIGURATION_JSON(const rapidjson::Value& config) {
	std::lock_guard<std::mutex> lock(handystats::operation_mutex);
	if (handystats::is_enabled()) {
		return;
	}

	if (!config.IsObject()) {
		return;
	}
	if (!config.HasMember("handystats")) {
		return;
	}

	const rapidjson::Value& handystats_config = config["handystats"];

	if (handystats_config.HasMember("incremental-statistics")) {
		const rapidjson::Value& incremental_statistics_config = handystats_config["incremental-statistics"];
		handystats::config::incremental_statistics.configure(incremental_statistics_config);
	}

	if (handystats_config.HasMember("timer")) {
		const rapidjson::Value& timer_config = handystats_config["timer"];
		handystats::config::timer.configure(timer_config);
	}

	if (handystats_config.HasMember("metrics-dump")) {
		const rapidjson::Value& metrics_dump_config = handystats_config["metrics-dump"];
		handystats::config::metrics_dump.configure(metrics_dump_config);
	}
}
