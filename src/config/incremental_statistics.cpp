#include <handystats/config/incremental_statistics.hpp>

namespace handystats { namespace config {

const double incremental_statistics::defaults::moving_average_alpha = 2.0 / 16;
const std::chrono::milliseconds incremental_statistics::defaults::moving_interval = std::chrono::seconds(1);

incremental_statistics::incremental_statistics()
	: moving_average_alpha(defaults::moving_average_alpha)
	, moving_interval(defaults::moving_interval)
{}

void incremental_statistics::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("moving-average-alpha")) {
		const rapidjson::Value& moving_average_alpha = config["moving-average-alpha"];
		if (moving_average_alpha.IsNumber()) {
			this->moving_average_alpha = moving_average_alpha.GetDouble();
		}
	}

	if (config.HasMember("moving-interval")) {
		const rapidjson::Value& moving_interval = config["moving-interval"];
		if (moving_interval.IsUint64()) {
			this->moving_interval = std::chrono::milliseconds(moving_interval.GetUint64());
		}
	}
}

}} // namespace handystats::config
