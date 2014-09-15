#include "config/core_impl.hpp"

namespace handystats { namespace config {

core::core()
	: enable(true)
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
