#ifndef HANDYSTATS_GAUGE_JSON_WRITER_H_
#define HANDYSTATS_GAUGE_JSON_WRITER_H_

#include <string>

#include <handystats/rapidjson/document.h>
#include <handystats/rapidjson/stringbuffer.h>
#include <handystats/rapidjson/prettywriter.h>

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
		const auto& gauge_stats = obj->stats;

		json_value->AddMember("min", gauge_stats.min(), allocator);
		json_value->AddMember("max", gauge_stats.max(), allocator);
		json_value->AddMember("sum", gauge_stats.sum(), allocator);
		json_value->AddMember("count", gauge_stats.count(), allocator);
		json_value->AddMember("mean", gauge_stats.count() == 0 ? 0.0 : gauge_stats.mean(), allocator);
		json_value->AddMember("moving-avg", gauge_stats.moving_average(), allocator);
		json_value->AddMember("interval-count", gauge_stats.interval_count(), allocator);
		json_value->AddMember("interval-sum", gauge_stats.interval_sum(), allocator);
		json_value->AddMember("interval-mean", gauge_stats.interval_mean(), allocator);
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

template<typename Allocator>
inline std::string write_to_json_string(metrics::gauge* obj, Allocator&& allocator = Allocator()) {
	rapidjson::GenericStringBuffer<rapidjson::UTF8<>, Allocator> buffer(&allocator);
	write_to_json_buffer(obj, &buffer, allocator);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}} // namespace handystats::json

#endif // HANDYSTATS_GAUGE_JSON_WRITER_H_
