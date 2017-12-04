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

#include <handystats/statistics.hpp>
#include <handystats/config/statistics.hpp>

#include "config_impl.hpp"

namespace handystats { namespace config {

statistics::statistics()
	: moving_interval(1, chrono::time_unit::SEC)
	, histogram_bins(30)
	, tags(
		handystats::statistics::tag::value |
		handystats::statistics::tag::min | handystats::statistics::tag::max |
		handystats::statistics::tag::count | handystats::statistics::tag::sum | handystats::statistics::tag::avg |
		handystats::statistics::tag::moving_count | handystats::statistics::tag::moving_sum | handystats::statistics::tag::moving_avg |
		handystats::statistics::tag::timestamp
	)
	, rate_unit(chrono::time_unit::SEC)
{}

void configure(statistics& obj, const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("moving-interval")) {
		const rapidjson::Value& moving_interval = config["moving-interval"];
		if (moving_interval.IsUint64() && moving_interval.GetUint64() > 0) {
			obj.moving_interval = chrono::duration(moving_interval.GetUint64(), chrono::time_unit::MSEC);
		}
	}

	if (config.HasMember("histogram-bins")) {
		const rapidjson::Value& histogram_bins = config["histogram-bins"];
		if (histogram_bins.IsUint64() && histogram_bins.GetUint64() > 0) {
			obj.histogram_bins = histogram_bins.GetUint64();
		}
	}

	if (config.HasMember("tags")) {
		const rapidjson::Value& tags = config["tags"];

		if (tags.IsArray()) {
			obj.tags = handystats::statistics::tag::empty;
			for (size_t index = 0; index < tags.Size(); ++index) {
				const rapidjson::Value& tag = tags[index];
				if (tag.IsString()) {
					obj.tags |= handystats::statistics::tag::from_string(tag.GetString());
				}
			}
		}
	}

	if (config.HasMember("rate-unit")) {
		const rapidjson::Value& rate_unit = config["rate-unit"];

		if (rate_unit.IsString()) {
			chrono::time_unit unit = chrono::time_unit::TICK;
			if (strcmp(rate_unit.GetString(), "ns") == 0) {
				unit = chrono::time_unit::NSEC;
			}
			else if (strcmp(rate_unit.GetString(), "us") == 0) {
				unit = chrono::time_unit::USEC;
			}
			else if (strcmp(rate_unit.GetString(), "ms") == 0) {
				unit = chrono::time_unit::MSEC;
			}
			else if (strcmp(rate_unit.GetString(), "s") == 0) {
				unit = chrono::time_unit::SEC;
			}
			else if (strcmp(rate_unit.GetString(), "m") == 0) {
				unit = chrono::time_unit::MIN;
			}
			else if (strcmp(rate_unit.GetString(), "h") == 0) {
				unit = chrono::time_unit::HOUR;
			}

			if (unit != chrono::time_unit::TICK) {
				obj.rate_unit = unit;
			}
		}
	}
}

}} // namespace handystats::config
