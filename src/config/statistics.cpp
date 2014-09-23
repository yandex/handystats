// Copyright (c) 2014 Yandex LLC. All rights reserved.

#include <handystats/statistics.hpp>
#include <handystats/config/statistics.hpp>

namespace handystats { namespace config {

statistics::statistics()
	: moving_interval(
		chrono::duration_cast<chrono::clock::duration>(
			std::chrono::seconds(1)
		)
	)
	, histogram_bins(30)
	, tags(
		handystats::statistics::tag::value |
		handystats::statistics::tag::min | handystats::statistics::tag::max |
		handystats::statistics::tag::count | handystats::statistics::tag::sum | handystats::statistics::tag::avg |
		handystats::statistics::tag::moving_count | handystats::statistics::tag::moving_sum | handystats::statistics::tag::moving_avg |
		handystats::statistics::tag::timestamp
	)
	, rate_unit(
		chrono::duration_cast<chrono::clock::duration>(
			std::chrono::seconds(1)
		)
	)
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

	if (config.HasMember("rate-unit")) {
		const rapidjson::Value& rate_unit = config["rate-unit"];

		if (rate_unit.IsString()) {
			chrono::clock::duration rate_interval(0);
			if (strcmp(rate_unit.GetString(), "ns") == 0) {
				rate_interval =
					chrono::duration_cast<chrono::clock::duration>(
						std::chrono::nanoseconds(1)
					);
			}
			else if (strcmp(rate_unit.GetString(), "us") == 0) {
				rate_interval =
					chrono::duration_cast<chrono::clock::duration>(
						std::chrono::microseconds(1)
					);
			}
			else if (strcmp(rate_unit.GetString(), "ms") == 0) {
				rate_interval =
					chrono::duration_cast<chrono::clock::duration>(
						std::chrono::milliseconds(1)
					);
			}
			else if (strcmp(rate_unit.GetString(), "s") == 0) {
				rate_interval =
					chrono::duration_cast<chrono::clock::duration>(
						std::chrono::seconds(1)
					);
			}
			else if (strcmp(rate_unit.GetString(), "m") == 0) {
				rate_interval =
					chrono::duration_cast<chrono::clock::duration>(
						std::chrono::minutes(1)
					);
			}
			else if (strcmp(rate_unit.GetString(), "h") == 0) {
				rate_interval =
					chrono::duration_cast<chrono::clock::duration>(
						std::chrono::hours(1)
					);
			}

			if (rate_interval.count() > 0) {
				this->rate_unit = rate_interval;
			}
		}
	}
}

}} // namespace handystats::config
