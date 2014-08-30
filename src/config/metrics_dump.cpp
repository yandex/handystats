#include <handystats/config/metrics_dump.hpp>

namespace handystats { namespace config {

const std::chrono::milliseconds metrics_dump::defaults::interval = std::chrono::milliseconds(750);

metrics_dump::metrics_dump()
	: interval(defaults::interval)
{}

}} // namespace handystats::config
