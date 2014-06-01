#ifndef HANDYSTATS_CONFIGURATION_IMPL_H_
#define HANDYSTATS_CONFIGURATION_IMPL_H_

#include <vector>

#include <handystats/chrono.hpp>
#include <handystats/rapidjson/document.h>

namespace handystats { namespace config {

struct incremental_statistics_parameters {
	double moving_average_alpha;
	std::chrono::milliseconds moving_interval;

	incremental_statistics_parameters();
	void configure(const rapidjson::Value& incremental_statistics_config);
};

struct timer_parameters {
	std::chrono::milliseconds idle_timeout;

	timer_parameters();
	void configure(const rapidjson::Value& timer_config);
};

struct json_dump_parameters {
	std::chrono::milliseconds interval;

	json_dump_parameters();
	void configure(const rapidjson::Value& json_dump_config);
};

struct message_queue_parameters {
	std::vector<std::chrono::microseconds> sleep_on_empty;

	message_queue_parameters();
	void configure(const rapidjson::Value& message_queue_config);
};

extern incremental_statistics_parameters incremental_statistics;
extern timer_parameters timer;
extern json_dump_parameters json_dump;
extern message_queue_parameters message_queue;

void initialize();
void finalize();

}} // namespace handystats::config

#endif // HANDYSTATS_CONFIGURATION_IMPL_H_
