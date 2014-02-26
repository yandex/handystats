#ifndef HANDYSTATS_COUNTER_JSON_WRITER_H_
#define HANDYSTATS_COUNTER_JSON_WRITER_H_

#include <string>

#include <handystats/rapidjson/document.h>
#include <handystats/rapidjson/stringbuffer.h>
#include <handystats/rapidjson/prettywriter.h>

#include <handystats/json/allocators.hpp>
#include <handystats/json/timestamp.hpp>
#include <handystats/json/gauge_json_writer.hpp>
#include <handystats/metrics/counter.hpp>


namespace handystats { namespace json {

template<typename Allocator>
inline void write_to_json_value(metrics::counter* obj, rapidjson::Value* json_value, Allocator& allocator) {
	if (!obj) {
		json_value = new rapidjson::Value();
		return;
	}

	if (!json_value) {
		json_value = new rapidjson::Value(rapidjson::kObjectType);
	}
	else {
		json_value->SetObject();
	}

	json_value->AddMember("type", "counter", allocator);
	json_value->AddMember("value", obj->value, allocator);

	rapidjson::Value timestamp;
	write_to_json_value(obj->timestamp, &timestamp);
	json_value->AddMember("timestamp", timestamp, allocator);

	{
		rapidjson::Value values;
		auto values_stats = obj->stats.values;
		write_to_json_value(&values_stats, &values, allocator);
		json_value->AddMember("values", values, allocator);

		rapidjson::Value incr_deltas;
		auto incr_deltas_stats = obj->stats.incr_deltas;
		write_to_json_value(&incr_deltas_stats, &incr_deltas, allocator);
		json_value->AddMember("incr-deltas", incr_deltas, allocator);

		rapidjson::Value decr_deltas;
		auto decr_deltas_stats = obj->stats.decr_deltas;
		write_to_json_value(&decr_deltas_stats, &decr_deltas, allocator);
		json_value->AddMember("decr-deltas", decr_deltas, allocator);

		rapidjson::Value deltas;
		auto deltas_stats = obj->stats.deltas;
		write_to_json_value(&deltas_stats, &deltas, allocator);
		json_value->AddMember("deltas", deltas, allocator);
	}
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(metrics::counter* obj, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(obj, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

inline std::string write_to_json_string(metrics::counter* obj) {
	rapidjson::StringBuffer buffer;
	write_to_json_buffer(obj, &buffer, memoryPoolAllocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_COUNTER_JSON_WRITER_H_
