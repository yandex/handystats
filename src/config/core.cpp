#include <handystats/config/core.hpp>

namespace handystats { namespace config {

const bool core::defaults::enable = true;

core::core()
	: enable(defaults::enable)
{}

}} // namespace handystats::config
