// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_SERIALIZATION_RAPIDJSON_IMPL_HPP_
#define HANDYSTATS_SERIALIZATION_RAPIDJSON_IMPL_HPP_

#include <utility>

#include <handystats/common.h>
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


template <typename JsonValue>
inline
statistics::data& load_value(statistics::data& data, const JsonValue& json_value) {
	if (!json_value.IsObject()) {
		return data;
	}

	{
		typename JsonValue::ConstMemberIterator timestamp_iter = json_value.FindMember("ts");
		if (timestamp_iter) {
			const auto& timestamp_json = timestamp_iter->value;
			if (timestamp_json.IsUint64()) {
				data.m_timestamp =
					chrono::time_point(
							chrono::milliseconds(timestamp_json.GetUint64()),
							chrono::clock_type::SYSTEM_CLOCK
						);
				data.m_tags |= statistics::tag::timestamp;
			}
		}
	}
	{
		typename JsonValue::ConstMemberIterator data_timestamp_iter = json_value.FindMember("data-ts");
		if (data_timestamp_iter) {
			const auto& data_timestamp_json = data_timestamp_iter->value;
			if (data_timestamp_json.IsUint64()) {
				data.m_data_timestamp =
					chrono::time_point(
							chrono::milliseconds(data_timestamp_json.GetUint64()),
							chrono::clock_type::SYSTEM_CLOCK
						);
				if (!(data.m_tags & statistics::tag::timestamp)) {
					data.m_timestamp = data.m_data_timestamp;
					data.m_tags |= statistics::tag::timestamp;
				}
			}
		}
		else if (data.m_tags & statistics::tag::timestamp) {
			data.m_data_timestamp = data.m_timestamp;
		}
	}
	{
		typename JsonValue::ConstMemberIterator mint_iter = json_value.FindMember("m-int");
		if (mint_iter) {
			const auto& mint_json = mint_iter->value;
			if (mint_json.IsUint64()) {
				data.m_moving_interval = chrono::milliseconds(mint_json.GetUint64());
			}
		}
	}
	{
		typename JsonValue::ConstMemberIterator value_iter = json_value.FindMember("val");
		if (value_iter) {
			const auto& value_json = value_iter->value;
			if (value_json.IsNumber()) {
				data.m_value = (statistics::value_type)value_json.GetDouble();
				data.m_tags |= statistics::tag::value;
			}
		}
	}
	{
		typename JsonValue::ConstMemberIterator min_iter = json_value.FindMember("min");
		if (min_iter) {
			const auto& min_json = min_iter->value;
			if (min_json.IsNumber()) {
				data.m_min = (statistics::value_type)min_json.GetDouble();
				data.m_tags |= statistics::tag::min;
			}
		}
	}
	{
		typename JsonValue::ConstMemberIterator max_iter = json_value.FindMember("max");
		if (max_iter) {
			const auto& max_json = max_iter->value;
			if (max_json.IsNumber()) {
				data.m_max = (statistics::value_type)max_json.GetDouble();
				data.m_tags |= statistics::tag::max;
			}
		}
	}
	{
		typename JsonValue::ConstMemberIterator cnt_iter = json_value.FindMember("cnt");
		if (cnt_iter) {
			const auto& cnt_json = cnt_iter->value;
			if (cnt_json.IsUint64()) {
				data.m_count = cnt_json.GetUint64();
				data.m_tags |= statistics::tag::count;
			}
		}
	}
	{
		typename JsonValue::ConstMemberIterator sum_iter = json_value.FindMember("sum");
		if (sum_iter) {
			const auto& sum_json = sum_iter->value;
			if (sum_json.IsNumber()) {
				data.m_sum = (statistics::value_type)sum_json.GetDouble();
				data.m_tags |= statistics::tag::sum;
			}
		}
	}
	{
		typename JsonValue::ConstMemberIterator mcnt_iter = json_value.FindMember("m-cnt");
		if (mcnt_iter) {
			const auto& mcnt_json = mcnt_iter->value;
			if (mcnt_json.IsNumber()) {
				data.m_moving_count = mcnt_json.GetDouble();
				data.m_tags |= statistics::tag::moving_count;
			}
		}
	}
	{
		typename JsonValue::ConstMemberIterator msum_iter = json_value.FindMember("m-sum");
		if (msum_iter) {
			const auto& msum_json = msum_iter->value;
			if (msum_json.IsNumber()) {
				data.m_moving_sum = msum_json.GetDouble();
				data.m_tags |= statistics::tag::moving_sum;
			}
		}
	}
	{
		typename JsonValue::ConstMemberIterator hist_iter = json_value.FindMember("hist");
		if (hist_iter) {
			const auto& hist_json = hist_iter->value;
			if (hist_json.IsArray()) {
				statistics::histogram_type hist;
				hist.reserve(hist_json.Size());

				for (size_t index = 0; index < hist_json.Size(); ++index) {
					const auto& hist_bin_json = hist_json[index];
					if (hist_bin_json.IsArray()) {
						statistics::bin_type hist_bin(
									0,
									0,
									chrono::time_point(
										chrono::nanoseconds(0),
										chrono::clock_type::SYSTEM_CLOCK
									)
								);
						if (hist_bin_json.Size() > 0) {
							const auto& bin_center_json = hist_bin_json[0u];
							if (bin_center_json.IsNumber()) {
								std::get<statistics::BIN_CENTER>(hist_bin) = (statistics::value_type)bin_center_json.GetDouble();
							}
						}
						if (hist_bin_json.Size() > 1) {
							const auto& bin_count_json = hist_bin_json[1u];
							if (bin_count_json.IsNumber()) {
								std::get<statistics::BIN_COUNT>(hist_bin) = bin_count_json.GetDouble();
							}
						}
						if (hist_bin_json.Size() > 2) {
							const auto& bin_ts_json = hist_bin_json[2u];
							if (bin_ts_json.IsUint64()) {
								std::get<statistics::BIN_TIMESTAMP>(hist_bin) =
									chrono::time_point(
											chrono::milliseconds(bin_ts_json.GetUint64()),
											chrono::clock_type::SYSTEM_CLOCK
										);
							}
						}

						if (std::get<statistics::BIN_TIMESTAMP>(hist_bin).time_since_epoch().count() == 0) {
							if (data.m_tags & statistics::tag::timestamp) {
								std::get<statistics::BIN_TIMESTAMP>(hist_bin) = data.m_timestamp;
							}
						}

						hist.push_back(hist_bin);
					}
				}

				data.m_histogram = hist;
				data.m_histogram_bins = hist.size();
				data.m_tags |= statistics::tag::histogram;
			}
		}
	}
	{
		typename JsonValue::ConstMemberIterator rate_iter = json_value.FindMember("rate");
		if (rate_iter) {
			const auto& rate_json = rate_iter->value;
			if (rate_json.IsNumber()) {
				data.m_rate = rate_json.GetDouble();
				data.m_tags |= statistics::tag::rate;
			}
		}
	}

	data.fulfill_dependencies();

	return data;
}

template <typename JsonValue>
inline
stats_data_map& load_dump(stats_data_map& metrics, const chrono::time_point& dump_timestamp, const JsonValue& dump_value) {
	if (!dump_value.IsObject()) {
		return metrics;
	}

	config::statistics empty_config;
	empty_config.moving_interval = chrono::nanoseconds(0);
	empty_config.histogram_bins = 0;
	empty_config.tags = statistics::tag::empty;
	empty_config.rate_unit = chrono::time_unit::SEC;

	for (auto member_iter = dump_value.MemberBegin(); member_iter != dump_value.MemberEnd(); ++member_iter) {
		statistics::data& data =
			metrics.insert(
				std::make_pair(std::string(member_iter->name.GetString()), statistics::data(empty_config))
			)
			.first
			->second;

		data.m_timestamp = dump_timestamp;
		data.m_data_timestamp = dump_timestamp;
		data.m_tags |= statistics::tag::timestamp;

		load_value(data, member_iter->value);
	}

	return metrics;
}

}}} // namespace handystats::serialization::rapidjson

#endif // HANDYSTATS_SERIALIZATION_RAPIDJSON_IMPL_HPP_
