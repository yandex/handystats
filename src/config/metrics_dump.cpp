#include "config/metrics_dump_impl.hpp"

namespace handystats { namespace config {

metrics_dump::metrics_dump()
	: interval(
		chrono::duration_cast<chrono::clock::duration>(
			std::chrono::milliseconds(750)
		)
	)
{}

void metrics_dump::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("interval")) {
		const rapidjson::Value& interval = config["interval"];
		if (interval.IsUint64()) {
			this->interval =
				chrono::duration_cast<chrono::clock::duration>(std::chrono::milliseconds(interval.GetUint64()));
		}
	}
}

}} // namespace handystats::config
