#include <handystats/config/core.hpp>

namespace handystats { namespace config {

const bool core::defaults::enable = true;

core::core()
	: enable(defaults::enable)
{}

void core::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("enable")) {
		const rapidjson::Value& enable = config["enable"];
		if (enable.IsBool()) {
			this->enable = enable.GetBool();
		}
	}
}

}} // namespace handystats::config
