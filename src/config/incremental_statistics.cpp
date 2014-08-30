#include <handystats/config/incremental_statistics.hpp>

namespace handystats { namespace config {

const double incremental_statistics::defaults::moving_average_alpha = 2.0 / 16;
const std::chrono::milliseconds incremental_statistics::defaults::moving_interval = std::chrono::seconds(1);

incremental_statistics::incremental_statistics()
	: moving_average_alpha(defaults::moving_average_alpha)
	, moving_interval(defaults::moving_interval)
{}

}} // namespace handystats::config
