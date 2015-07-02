/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDYSTATS_JSON_DUMP_HPP_
#define HANDYSTATS_JSON_DUMP_HPP_

#include <string>

#include <handystats/json/gauge_json_writer.hpp>
#include <handystats/json/counter_json_writer.hpp>
#include <handystats/json/timer_json_writer.hpp>
#include <handystats/json/attribute_json_writer.hpp>

#include <handystats/metrics_dump.hpp>

namespace handystats { namespace json {

template<typename Allocator>
void fill(
		rapidjson::Value& dump, Allocator& allocator,
		const std::map<std::string, handystats::metrics::metric_variant>& metrics_map
	)
{
	dump.SetObject();

	for (auto metric_iter = metrics_map.cbegin(); metric_iter != metrics_map.cend(); ++metric_iter) {
		rapidjson::Value metric_value;
		switch (metric_iter->second.which()) {
			case metrics::metric_index::GAUGE:
				json::write_to_json_value(&boost::get<metrics::gauge>(metric_iter->second), &metric_value, allocator);
				break;
			case metrics::metric_index::COUNTER:
				json::write_to_json_value(&boost::get<metrics::counter>(metric_iter->second), &metric_value, allocator);
				break;
			case metrics::metric_index::TIMER:
				json::write_to_json_value(&boost::get<metrics::timer>(metric_iter->second), &metric_value, allocator);
				break;
			case metrics::metric_index::ATTRIBUTE:
				json::write_to_json_value(&boost::get<metrics::attribute>(metric_iter->second), &metric_value, allocator);
				break;
		}

		dump.AddMember(metric_iter->first.c_str(), allocator, metric_value, allocator);
	}
}

std::string to_string(const std::map<std::string, handystats::metrics::metric_variant>&);

}} // namespace handystats::json

std::string HANDY_JSON_DUMP();

#endif // HANDYSTATS_JSON_DUMP_HPP_
