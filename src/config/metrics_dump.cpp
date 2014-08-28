#include <handystats/config/metrics_dump.hpp>

namespace handystats { namespace config {

const std::chrono::milliseconds metrics_dump::defaults::interval = std::chrono::milliseconds(750);
const bool metrics_dump::defaults::to_json = false;

metrics_dump::metrics_dump()
	: interval(defaults::interval)
	, to_json(defaults::to_json)
{}

void metrics_dump::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("interval")) {
		const rapidjson::Value& interval = config["interval"];
		if (interval.IsUint64()) {
			this->interval = std::chrono::milliseconds(interval.GetUint64());
		}
	}

	if (config.HasMember("to-json")) {
		const rapidjson::Value& to_json = config["to-json"];
		if (to_json.IsBool()) {
			this->to_json = to_json.GetBool();
		}
	}
}

}} // namespace handystats::config
