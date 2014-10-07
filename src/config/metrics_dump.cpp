#include "config/metrics_dump_impl.hpp"

namespace handystats { namespace config {

metrics_dump::metrics_dump()
	: interval(750, chrono::time_unit::MSEC)
{}

void apply(const rapidjson::Value& config, metrics_dump& metrics_dump_opts) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("interval")) {
		const rapidjson::Value& interval = config["interval"];
		if (interval.IsUint64()) {
			metrics_dump_opts.interval = chrono::duration(interval.GetUint64(), chrono::time_unit::MSEC);
		}
	}
}

}} // namespace handystats::config
