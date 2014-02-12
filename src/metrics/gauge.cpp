#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace metrics {

namespace default_parameters {
	std::vector<double> quantile_probs({0.5, 0.75, 0.90, 0.95});
	long double moving_average_alpha = 2.0 / (1 + 15);
}

void gauge::internal_stats::update_value(internal_stats& stats, value_type value, time_point) {
	stats.values(value);
}

gauge::gauge()
{
}

gauge::gauge(value_type value, time_point timestamp)
	: value(value)
	, timestamp(timestamp)
{
	internal_stats::update_value(stats, value, timestamp);
}


void gauge::set(value_type value, time_point timestamp) {
	this->value = value;
	this->timestamp = timestamp;

	internal_stats::update_value(stats, this->value, this->timestamp);
}

std::pair<gauge::value_type, gauge::time_point> gauge::get() const {
	return std::make_pair(value, timestamp);
}

gauge::internal_stats gauge::get_stats() const {
	return stats;
}

}} // namespace handystats::metrics
