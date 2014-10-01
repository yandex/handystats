#include "config/metrics_dump_impl.hpp"

namespace handystats { namespace config {

metrics_dump::metrics_dump()
	: interval(750, chrono::time_unit::MSEC)
{}

void metrics_dump::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("interval")) {
		const rapidjson::Value& interval = config["interval"];
		if (interval.IsUint64()) {
			this->interval = chrono::duration(interval.GetUint64(), chrono::time_unit::MSEC);
		}
	}
}

}} // namespace handystats::config
