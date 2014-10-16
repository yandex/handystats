// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <string>
#include <iostream>
#include <sstream>

#include <handystats/chrono.hpp>
#include <handystats/statistics.hpp>

#include "statistics_impl.hpp"
#include "serialization/rapidjson_impl.hpp"

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"

using namespace handystats;

int main() {
	stats_data_map agg_metrics;
	chrono::time_point start_timestamp(chrono::duration(0, chrono::time_unit::NSEC), chrono::clock_type::SYSTEM);
	chrono::time_point prev_dump_timestamp = start_timestamp;

	while (!std::cin.eof()) {
		std::string log_line;
		std::getline(std::cin, log_line);

		if (log_line.empty()) continue;

		std::stringstream log_line_stream(log_line, std::ios::in);

		chrono::time_point dump_timestamp;
		rapidjson::Document dump_value;

		// dump_timestamp
		{
			uint64_t timestamp_ms;
			log_line_stream >> timestamp_ms;
			dump_timestamp =
				chrono::time_point(
						chrono::duration(timestamp_ms, chrono::time_unit::MSEC),
						chrono::clock_type::SYSTEM
					);

			if (dump_timestamp <= prev_dump_timestamp) continue;
		}

		// dump_value
		{
			std::string dump_str;
			std::getline(log_line_stream, dump_str);

			dump_value.Parse<0>(dump_str.c_str());
			if (dump_value.HasParseError()) {
				continue;
			}
		}

		if (start_timestamp.time_since_epoch().count() == 0) {
			start_timestamp = dump_timestamp - chrono::duration(1, chrono::time_unit::SEC);
		}

		stats_data_map current_metrics;
		serialization::rapidjson::load_dump(current_metrics, dump_timestamp, static_cast<const rapidjson::Value&>(dump_value));

		for (auto metric_iter = current_metrics.begin(); metric_iter != current_metrics.end(); ++metric_iter) {
			auto agg_metric_iter = agg_metrics.find(metric_iter->first);
			if (agg_metric_iter == agg_metrics.end()) {
				agg_metric_iter = agg_metrics.insert(std::make_pair(metric_iter->first, statistics::data())).first;

				agg_metric_iter->second.m_timestamp = start_timestamp;
				agg_metric_iter->second.m_histogram_bins = 30;
			}

			agg_metric_iter->second.append(metric_iter->second);
		}

		prev_dump_timestamp = dump_timestamp;
	}

	rapidjson::Value json_agg_dump;
	rapidjson::MemoryPoolAllocator<> allocator;

	serialization::rapidjson::fill_value(json_agg_dump, agg_metrics, allocator);

	const auto& agg_dump_str =
		utils::rapidjson::to_string <
				rapidjson::Value, rapidjson::StringBuffer,
				rapidjson::Writer<rapidjson::StringBuffer>
			>
			(json_agg_dump);

	std::cout << agg_dump_str << std::endl;
}
