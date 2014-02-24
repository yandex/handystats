#ifndef HANDYSTATS_JSON_DUMP_H_
#define HANDYSTATS_JSON_DUMP_H_

#include <string>
#include <memory>
#include <mutex>

#include <handystats/chrono_impl.hpp>

namespace handystats { namespace json {

extern chrono::default_time_point dump_timestamp;
extern chrono::default_duration DUMP_INTERVAL;

std::shared_ptr<const std::string> get_json_dump();
std::shared_ptr<const std::string> create_json_dump();
void update_json_dump();

}} // namespace handystats::json

inline std::shared_ptr<const std::string> HANDY_JSON_DUMP() {
	return handystats::json::get_json_dump();
}

#endif // HANDYSTATS_JSON_DUMP_H_
