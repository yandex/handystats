// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/statistics.hpp>
#include <handystats/config/statistics.hpp>

namespace handystats { namespace config {

const chrono::clock::duration statistics::defaults::moving_interval =
		chrono::duration_cast<chrono::clock::duration>(
			std::chrono::seconds(1)
		);
const size_t statistics::defaults::histogram_bins = 0;
const int statistics::defaults::tags =
	handystats::statistics::tag::value |
	handystats::statistics::tag::min | handystats::statistics::tag::max |
	handystats::statistics::tag::count | handystats::statistics::tag::sum | handystats::statistics::tag::avg |
	handystats::statistics::tag::moving_count | handystats::statistics::tag::moving_sum | handystats::statistics::tag::moving_avg |
	handystats::statistics::tag::timestamp
	;

statistics::statistics()
	: moving_interval(defaults::moving_interval)
	, histogram_bins(defaults::histogram_bins)
	, tags(defaults::tags)
{}

void statistics::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("moving-interval")) {
		const rapidjson::Value& moving_interval = config["moving-interval"];
		if (moving_interval.IsUint64() && moving_interval.GetUint64() > 0) {
			this->moving_interval =
				chrono::duration_cast<chrono::clock::duration>(
					std::chrono::milliseconds(moving_interval.GetUint64())
				);
		}
	}

	if (config.HasMember("histogram-bins")) {
		const rapidjson::Value& histogram_bins = config["histogram-bins"];
		if (histogram_bins.IsUint64() && histogram_bins.GetUint64() > 0) {
			this->histogram_bins = histogram_bins.GetUint64();
		}
	}

	if (config.HasMember("tags")) {
		const rapidjson::Value& tags = config["tags"];

		if (tags.IsArray()) {
			this->tags = handystats::statistics::tag::empty;
			for (size_t index = 0; index < tags.Size(); ++index) {
				const rapidjson::Value& tag = tags[index];
				if (tag.IsString()) {
					this->tags |= handystats::statistics::tag::from_string(tag.GetString());
				}
			}
		}
	}
}

}} // namespace handystats::config
