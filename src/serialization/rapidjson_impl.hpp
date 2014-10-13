// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_SERIALIZATION_RAPIDJSON_IMPL_HPP_
#define HANDYSTATS_SERIALIZATION_RAPIDJSON_IMPL_HPP_

#include <handystats/statistics.hpp>
#include <handystats/utils/rapidjson_writer.hpp>
#include <handystats/metrics_dump.hpp>

#include "statistics_impl.hpp"

namespace handystats { namespace serialization { namespace rapidjson {

template <typename JsonValue, typename Allocator>
inline
JsonValue& fill_value(JsonValue& json_value, const statistics::data& stats_data, Allocator& allocator) {
	if (!json_value.IsObject()) {
		json_value.SetObject();
	}

	if (stats_data.m_tags == statistics::tag::empty) {
		return json_value;
	}

	if (stats_data.m_tags & statistics::tag::value) {
		json_value.AddMember("val", stats_data.m_value, allocator);
	}
	if (stats_data.m_tags & statistics::tag::min) {
		json_value.AddMember("min", stats_data.m_min, allocator);
	}
	if (stats_data.m_tags & statistics::tag::max) {
		json_value.AddMember("max", stats_data.m_max, allocator);
	}
	if (stats_data.m_tags & statistics::tag::count) {
		json_value.AddMember("cnt", stats_data.m_count, allocator);
	}
	if (stats_data.m_tags & statistics::tag::sum) {
		json_value.AddMember("sum", stats_data.m_sum, allocator);
	}
	if (stats_data.m_tags & statistics::tag::moving_count) {
		json_value.AddMember("m-cnt", stats_data.m_moving_count, allocator);
	}
	if (stats_data.m_tags & statistics::tag::moving_sum) {
		json_value.AddMember("m-sum", stats_data.m_moving_sum, allocator);
	}
	if (stats_data.m_tags & statistics::tag::histogram) {
		const auto& histogram = stats_data.m_histogram;
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
		json_value.AddMember("hist", histogram_value, allocator);
	}
	if (stats_data.m_tags & statistics::tag::rate) {
		json_value.AddMember("rate", stats_data.m_rate, allocator);
	}
	if (stats_data.m_tags & statistics::tag::timestamp) {
		JsonValue timestamp_value;
		utils::rapidjson::fill_value(timestamp_value, stats_data.m_timestamp, allocator);
		json_value.AddMember("ts", timestamp_value, allocator);

		JsonValue data_timestamp_value;
		utils::rapidjson::fill_value(data_timestamp_value, stats_data.m_data_timestamp, allocator);
		json_value.AddMember("data-ts", data_timestamp_value, allocator);
	}

	// moving_interval config
	const statistics::tag::type& moving_stats =
		statistics::tag::moving_count | statistics::tag::moving_sum |
		statistics::tag::histogram |
		statistics::tag::rate;

	if (stats_data.m_tags & moving_stats) {
		json_value.AddMember(
				"m-int",
				chrono::duration::convert_to(
						chrono::time_unit::MSEC,
						stats_data.m_moving_interval
					)
					.count(),
				allocator
			);
	}

	return json_value;
}

template <typename JsonValue, typename Allocator>
inline
JsonValue& fill_value(JsonValue& json_value, const stats_data_map& metrics, Allocator& allocator) {
	if (!json_value.IsObject()) {
		json_value.SetObject();
	}

	for (auto metric_iter = metrics.cbegin(); metric_iter != metrics.cend(); ++metric_iter) {
		JsonValue stats_data_value;
		fill_value(stats_data_value, metric_iter->second, allocator);

		JsonValue metric_name(metric_iter->first.c_str(), metric_iter->first.size(), allocator);

		json_value.AddMember(metric_name, stats_data_value, allocator);
	}

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
		fill_value(metric_value, *metric_iter->second.m_data, allocator);

		JsonValue metric_name(metric_iter->first.c_str(), metric_iter->first.size(), allocator);

		json_value.AddMember(metric_name, metric_value, allocator);
	}

	return json_value;
}

}}} // namespace handystats::serialization::rapidjson

#endif // HANDYSTATS_SERIALIZATION_RAPIDJSON_IMPL_HPP_
