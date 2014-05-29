#include <mutex>
#include <fstream>
#include <string>
#include <iostream>

#include <handystats/operation.hpp>

#include <handystats/configuration.hpp>
#include "configuration_impl.hpp"

namespace handystats { namespace config {

gauge_parameters::gauge_parameters() {
	moving_average_alpha = 2.0 / 16;
	moving_interval = std::chrono::seconds(1);
}

void gauge_parameters::configure(const rapidjson::Value& gauge_config) {
	if (gauge_config.HasMember("moving-average-alpha")) {
		const rapidjson::Value& moving_average_alpha = gauge_config["moving-average-alpha"];
		if (moving_average_alpha.IsNumber()) {
			this->moving_average_alpha = moving_average_alpha.GetDouble();
		}
	}
	if (gauge_config.HasMember("moving-interval")) {
		const rapidjson::Value& moving_interval = gauge_config["moving-interval"];
		if (moving_interval.IsUint64()) {
			this->moving_interval = std::chrono::milliseconds(moving_interval.GetUint64());
		}
	}
}


timer_parameters::timer_parameters() {
	idle_timeout = std::chrono::milliseconds(10000);
}

void timer_parameters::configure(const rapidjson::Value& timer_config) {
	if (timer_config.HasMember("idle-timeout")) {
		const rapidjson::Value& idle_timeout = timer_config["idle-timeout"];
		if (idle_timeout.IsUint64()) {
			this->idle_timeout = std::chrono::milliseconds(idle_timeout.GetUint64());
		}
	}
}


json_dump_parameters::json_dump_parameters() {
	interval = std::chrono::milliseconds(500);
}

void json_dump_parameters::configure(const rapidjson::Value& json_dump_config) {
	if (json_dump_config.HasMember("interval")) {
		const rapidjson::Value& interval = json_dump_config["interval"];
		if (interval.IsUint64()) {
			this->interval = std::chrono::milliseconds(interval.GetUint64());
		}
	}
}


message_queue_parameters::message_queue_parameters() {
	sleep_on_empty = std::vector<std::chrono::microseconds>({
				std::chrono::microseconds(1),
				std::chrono::microseconds(5),
				std::chrono::microseconds(10),
				std::chrono::microseconds(50),
				std::chrono::microseconds(100),
				std::chrono::microseconds(500),
				std::chrono::microseconds(1000),
				std::chrono::microseconds(5000),
				std::chrono::microseconds(10000)
			});
}

void message_queue_parameters::configure(const rapidjson::Value& message_queue_config) {
	if (message_queue_config.HasMember("sleep-on-empty")) {
		const rapidjson::Value& sleep_on_empty = message_queue_config["sleep-on-empty"];
		if (sleep_on_empty.IsArray() && !sleep_on_empty.Empty()) {
			std::vector<std::chrono::microseconds> sleep_intervals;
			sleep_intervals.reserve(sleep_on_empty.Size());
			for (size_t i = 0; i < sleep_on_empty.Size(); ++i) {
				const rapidjson::Value& sleep_interval = sleep_on_empty[i];
				if (!sleep_interval.IsUint64()) {
					break;
				}
				sleep_intervals.emplace_back(sleep_interval.GetUint64());
			}
			if (sleep_intervals.size() == sleep_on_empty.Size()) {
				this->sleep_on_empty = sleep_intervals;
			}
		}
	}
}


gauge_parameters gauge;
timer_parameters timer;
json_dump_parameters json_dump;
message_queue_parameters message_queue;

void initialize() {
}

void finalize() {
	gauge = gauge_parameters();
	timer = timer_parameters();
	json_dump = json_dump_parameters();
	message_queue = message_queue_parameters();
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

	if (handystats_config.HasMember("gauge")) {
		const rapidjson::Value& gauge_config = handystats_config["gauge"];
		handystats::config::gauge.configure(gauge_config);
	}

	if (handystats_config.HasMember("timer")) {
		const rapidjson::Value& timer_config = handystats_config["timer"];
		handystats::config::timer.configure(timer_config);
	}

	if (handystats_config.HasMember("json-dump")) {
		const rapidjson::Value& json_dump_config = handystats_config["json-dump"];
		handystats::config::json_dump.configure(json_dump_config);
	}

	if (handystats_config.HasMember("message-queue")) {
		const rapidjson::Value& message_queue_config = handystats_config["message-queue"];
		handystats::config::message_queue.configure(message_queue_config);
	}
}
