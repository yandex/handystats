// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_UTILS_RAPIDJSON_WRITER_HPP_
#define HANDYSTATS_UTILS_RAPIDJSON_WRITER_HPP_

#include <cstdint>
#include <string>

#include <handystats/chrono.hpp>
#include <handystats/statistics.hpp>
#include <handystats/attribute.hpp>
#include <handystats/metrics_dump.hpp>

namespace handystats { namespace utils { namespace rapidjson {

template <typename JsonValue, typename Allocator>
inline
JsonValue& fill_value(JsonValue& json_value, const chrono::time_point& timestamp, Allocator&) {
	const auto& system_timestamp = chrono::time_point::convert_to(chrono::clock_type::SYSTEM_CLOCK, timestamp);

	json_value.SetUint64(system_timestamp.time_since_epoch().count(chrono::time_unit::MSEC));

	return json_value;
}


template <typename JsonValue, typename Allocator>
inline
JsonValue& fill_value(JsonValue& json_value, const statistics& stats, Allocator& allocator) {
	if (!json_value.IsObject()) {
		json_value.SetObject();
	}

	if (stats.tags() == statistics::tag::empty) {
		return json_value;
	}

	if (stats.enabled(statistics::tag::value)) {
		json_value.AddMember("value", stats.get<statistics::tag::value>(), allocator);
	}
	if (stats.enabled(statistics::tag::min)) {
		json_value.AddMember("min", stats.get<statistics::tag::min>(), allocator);
	}
	if (stats.enabled(statistics::tag::max)) {
		json_value.AddMember("max", stats.get<statistics::tag::max>(), allocator);
	}
	if (stats.enabled(statistics::tag::count)) {
		json_value.AddMember("count", stats.get<statistics::tag::count>(), allocator);
	}
	if (stats.enabled(statistics::tag::sum)) {
		json_value.AddMember("sum", stats.get<statistics::tag::sum>(), allocator);
	}
	if (stats.enabled(statistics::tag::avg)) {
		json_value.AddMember("avg", stats.get<statistics::tag::avg>(), allocator);
	}

	if (stats.enabled(statistics::tag::moving_count)) {
		json_value.AddMember("moving-count", stats.get<statistics::tag::moving_count>(), allocator);
	}
	if (stats.enabled(statistics::tag::moving_sum)) {
		json_value.AddMember("moving-sum", stats.get<statistics::tag::moving_sum>(), allocator);
	}
	if (stats.enabled(statistics::tag::moving_avg)) {
		json_value.AddMember("moving-avg", stats.get<statistics::tag::moving_avg>(), allocator);
	}
	if (stats.enabled(statistics::tag::histogram)) {
		const auto& histogram = stats.get<statistics::tag::histogram>();
		JsonValue histogram_value;
		histogram_value.SetArray();
		for (auto bin = histogram.begin(); bin != histogram.end(); ++bin) {
			histogram_value.PushBack(
					JsonValue().SetArray()
					.PushBack(std::get<statistics::BIN_CENTER>(*bin), allocator)
					.PushBack(std::get<statistics::BIN_COUNT>(*bin), allocator),
					allocator
				);
		}
		json_value.AddMember("histogram", histogram_value, allocator);
	}
	if (stats.enabled(statistics::tag::quantile)) {
		const auto& quantile = stats.get<statistics::tag::quantile>();
		json_value.AddMember("p25", quantile.at(0.25), allocator);
		json_value.AddMember("p50", quantile.at(0.50), allocator);
		json_value.AddMember("p75", quantile.at(0.75), allocator);
		json_value.AddMember("p90", quantile.at(0.90), allocator);
		json_value.AddMember("p95", quantile.at(0.95), allocator);
	}
	if (stats.enabled(statistics::tag::entropy)) {
		json_value.AddMember("entropy", stats.get<statistics::tag::entropy>(), allocator);
	}
	if (stats.enabled(statistics::tag::throughput)) {
		json_value.AddMember("throughput", stats.get<statistics::tag::throughput>(), allocator);
	}
	if (stats.enabled(statistics::tag::frequency)) {
		json_value.AddMember("frequency", stats.get<statistics::tag::frequency>(), allocator);
	}

	if (stats.enabled(statistics::tag::timestamp)) {
		JsonValue timestamp_value;
		fill_value(timestamp_value, stats.get<statistics::tag::timestamp>(), allocator);
		json_value.AddMember("timestamp", timestamp_value, allocator);
	}

	return json_value;
}


template <typename JsonValue, typename Allocator>
class attribute_visitor : public boost::static_visitor<JsonValue&>
{
public:
	attribute_visitor(JsonValue& json_value, Allocator& allocator)
		: json_value(json_value)
		, allocator(allocator)
	{}

	JsonValue& operator() (const bool& value) const {
		json_value.SetBool(value);
		return json_value;
	}

	JsonValue& operator() (const int& value) const {
		json_value.SetInt(value);
		return json_value;
	}

	JsonValue& operator() (const unsigned& value) const {
		json_value.SetUint(value);
		return json_value;
	}

	JsonValue& operator() (const int64_t& value) const {
		json_value.SetInt64(value);
		return json_value;
	}

	JsonValue& operator() (const uint64_t& value) const {
		json_value.SetUint64(value);
		return json_value;
	}

	JsonValue& operator() (const double& value) const {
		json_value.SetDouble(value);
		return json_value;
	}

	JsonValue& operator() (const std::string& value) const {
		json_value.SetString(value.c_str(), value.size(), allocator);
		return json_value;
	}

private:
	JsonValue& json_value;
	Allocator& allocator;
};

template <typename JsonValue, typename Allocator>
inline
JsonValue& fill_value(JsonValue& json_value, const attribute& attr, Allocator& allocator) {
	attribute_visitor<JsonValue, Allocator> visitor(json_value, allocator);
	boost::apply_visitor(visitor, attr.value());

	return json_value;
}


template <typename JsonValue, typename Allocator>
inline
JsonValue& fill_value(JsonValue& json_value, const metric_map& metrics, Allocator& allocator) {
	if (!json_value.IsObject()) {
		json_value.SetObject();
	}

	for (auto metric_iter = metrics.cbegin(); metric_iter != metrics.cend(); ++metric_iter) {
		JsonValue metric_value;
		fill_value(metric_value, metric_iter->second, allocator);

		JsonValue metric_name(metric_iter->first.c_str(), metric_iter->first.size(), allocator);
		json_value.AddMember(metric_name, metric_value, allocator);
	}

	return json_value;
}


template <typename JsonValue, typename Allocator>
inline
JsonValue& fill_value(JsonValue& json_value, const attribute_map& attributes, Allocator& allocator) {
	if (!json_value.IsObject()) {
		json_value.SetObject();
	}

	for (auto attr_iter = attributes.cbegin(); attr_iter != attributes.cend(); ++attr_iter) {
		JsonValue attr_value;
		fill_value(attr_value, attr_iter->second, allocator);

		JsonValue attr_name(attr_iter->first.c_str(), attr_iter->first.size(), allocator);
		json_value.AddMember(attr_name, attr_value, allocator);
	}

	return json_value;
}


template <typename JsonValue, typename Allocator>
inline
JsonValue& fill_value(JsonValue& json_value, const metrics_dump_type& metrics_dump, Allocator& allocator) {
	if (!json_value.IsObject()) {
		json_value.SetObject();
	}

	const auto& metrics = metrics_dump.first;
	fill_value(json_value, metrics, allocator);

	const auto& attributes = metrics_dump.second;
	fill_value(json_value, attributes, allocator);

	return json_value;
}


// Example usage:
//
// using namespace rapidjson;
// auto str = to_string<Value, StringBuffer, PrettyWriter<StringBuffer>>(json_value);
//
template <typename JsonValue, typename StringBuffer, typename Writer>
inline
std::string to_string(const JsonValue& json_value) {
	StringBuffer buffer;
	Writer writer(buffer);

	json_value.Accept(writer);

	return std::string(buffer.GetString(), buffer.GetSize());
}

}}} // namespace handystats::utils::rapidjson

#endif // HANDYSTATS_UTILS_RAPIDJSON_WRITER_HPP_
