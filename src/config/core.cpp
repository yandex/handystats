#include "config/core_impl.hpp"

namespace handystats { namespace config {

core::core()
	: enable(true)
{}

void apply(const rapidjson::Value& config, core& core_opts) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("enable")) {
		const rapidjson::Value& enable = config["enable"];
		if (enable.IsBool()) {
			core_opts.enable = enable.GetBool();
		}
	}
}

}} // namespace handystats::config
