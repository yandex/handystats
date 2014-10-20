#include "config/metrics_dump_impl.hpp"

namespace handystats { namespace config {

metrics_dump::metrics_dump()
	: interval(750, chrono::time_unit::MSEC)
{}

void apply(const rapidjson::Value& config, metrics_dump& metrics_dump_opts) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("dump-interval")) {
		const rapidjson::Value& interval = config["dump-interval"];
		if (interval.IsUint64()) {
			metrics_dump_opts.interval = chrono::milliseconds(interval.GetUint64());
		}
	}
}

}} // namespace handystats::config
