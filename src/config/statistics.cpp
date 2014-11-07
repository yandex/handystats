// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <stdexcept>

#include <handystats/statistics.hpp>

#include "config/statistics_impl.hpp"

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
{}

void apply(const rapidjson::Value& config, statistics& statistics_opts) {
	if (!config.IsObject()) {
		throw std::logic_error("Statistics config data must be JSON object");
	}

	if (config.HasMember("moving-interval")) {
		const rapidjson::Value& moving_interval = config["moving-interval"];
		if (moving_interval.IsUint64() && moving_interval.GetUint64() >= 0) {
			statistics_opts.moving_interval = chrono::milliseconds(moving_interval.GetUint64());
		}
		else {
			throw std::logic_error("'moving-interval' config option must be unsigned integer");
		}
	}

	if (config.HasMember("histogram-bins")) {
		const rapidjson::Value& histogram_bins = config["histogram-bins"];
		if (histogram_bins.IsUint64() && histogram_bins.GetUint64() >= 0) {
			statistics_opts.histogram_bins = histogram_bins.GetUint64();
		}
		else {
			throw std::logic_error("'histogram-bins' config option must be unsigned integer");
		}
	}

	if (config.HasMember("stats")) {
		const rapidjson::Value& tags = config["stats"];

		if (tags.IsArray()) {
			statistics_opts.tags = handystats::statistics::tag::empty;
			for (size_t index = 0; index < tags.Size(); ++index) {
				const rapidjson::Value& tag = tags[index];
				if (tag.IsString()) {
					try {
						statistics_opts.tags |= handystats::statistics::tag::from_string(tag.GetString());
					}
					catch (const handystats::statistics::invalid_tag_error& err) {
						throw std::logic_error("'stats' config option has invalid statistic name");
					}
				}
				else {
					throw std::logic_error("'stats' config option must be array of strings");
				}
			}
		}
		else {
			throw std::logic_error("'stats' config option must be array");
		}
	}
}

}} // namespace handystats::config
