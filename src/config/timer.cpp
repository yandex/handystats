#include <handystats/config/timer.hpp>

namespace handystats { namespace config {

const std::chrono::milliseconds timer::defaults::idle_timeout = std::chrono::seconds(10);

timer::timer()
	: idle_timeout(timer::defaults::idle_timeout)
{}

}} // namespace handystats::config
