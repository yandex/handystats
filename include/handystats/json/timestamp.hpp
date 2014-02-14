#ifndef HANDYSTATS_JSON_TIMESTAMP_H_
#define HANDYSTATS_JSON_TIMESTAMP_H_

#include <handystats/chrono.hpp>
#include <handystats/rapidjson/document.h>

namespace handystats { namespace json {

typedef std::chrono::milliseconds time_duration;

inline void write_to_json_value(chrono::default_time_point timestamp, rapidjson::Value* json_value) {
	if (!json_value) {
		json_value = new rapidjson::Value();
	}

	time_duration delta = std::chrono::duration_cast<time_duration>(chrono::default_clock::now() - timestamp);
	std::chrono::system_clock::time_point system_timestamp = std::chrono::system_clock::now() - delta;

	json_value->SetUint64(std::chrono::duration_cast<time_duration>(system_timestamp.time_since_epoch()).count());
}

}} // namespace handystats::json

#endif // HANDYSTATS_JSON_TIMESTAMP_H_
