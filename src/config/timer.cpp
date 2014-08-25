#include <handystats/config/timer.hpp>

namespace handystats { namespace config {

const std::chrono::milliseconds timer::defaults::idle_timeout = std::chrono::seconds(10);

timer::timer()
	: idle_timeout(timer::defaults::idle_timeout)
{
}

void timer::configure(const rapidjson::Value& config) {
	if (!config.IsObject()) {
		return;
	}

	if (config.HasMember("idle-timeout")) {
		const rapidjson::Value& idle_timeout = config["idle-timeout"];
		if (idle_timeout.IsUint64()) {
			this->idle_timeout = std::chrono::milliseconds(idle_timeout.GetUint64());
		}
	}
}

}} // namespace handystats::config
