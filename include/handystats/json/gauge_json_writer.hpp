#ifndef HANDYSTATS_GAUGE_JSON_WRITER_H_
#define HANDYSTATS_GAUGE_JSON_WRITER_H_

#include <string>

#include <handystats/rapidjson/document.h>
#include <handystats/rapidjson/stringbuffer.h>
#include <handystats/rapidjson/prettywriter.h>

#include <handystats/json/allocators.hpp>
#include <handystats/json/timestamp.hpp>
#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace json {

template<typename Allocator>
inline void write_to_json_value(metrics::gauge* obj, rapidjson::Value* json_value, Allocator& allocator) {
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

	json_value->AddMember("type", "gauge", allocator);
	json_value->AddMember("value", obj->value, allocator);

	rapidjson::Value timestamp;
	write_to_json_value(obj->timestamp, &timestamp);
	json_value->AddMember("timestamp", timestamp, allocator);

	{
		const auto& gauge_stats = obj->stats.values;

		json_value->AddMember("min", boost::accumulators::min(gauge_stats), allocator);
		json_value->AddMember("max", boost::accumulators::max(gauge_stats), allocator);
		json_value->AddMember("sum", boost::accumulators::sum(gauge_stats), allocator);
		json_value->AddMember("count", boost::accumulators::count(gauge_stats), allocator);
		json_value->AddMember("mean", boost::accumulators::count(gauge_stats) == 0 ? 0.0 : boost::accumulators::mean(gauge_stats), allocator);
		json_value->AddMember("moving-avg", boost::accumulators::moving_average(gauge_stats), allocator);
		json_value->AddMember("interval-count", boost::accumulators::interval_count(gauge_stats), allocator);
		json_value->AddMember("interval-sum", boost::accumulators::interval_sum(gauge_stats), allocator);
		json_value->AddMember("interval-mean", boost::accumulators::interval_mean(gauge_stats), allocator);
	}
}

template<typename StringBuffer, typename Allocator>
inline void write_to_json_buffer(metrics::gauge* obj, StringBuffer* buffer, Allocator& allocator) {
	rapidjson::Value json_value;
	write_to_json_value(obj, &json_value, allocator);

	if (!buffer) {
		buffer = new StringBuffer();
	}

	rapidjson::PrettyWriter<StringBuffer> writer(*buffer);
	json_value.Accept(writer);
}

inline std::string write_to_json_string(metrics::gauge* obj) {
	rapidjson::StringBuffer buffer;
	write_to_json_buffer(obj, &buffer, memoryPoolAllocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_GAUGE_JSON_WRITER_H_
