#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace metrics {

void gauge::internal_stats::initialize(internal_stats& stats) {
}

void gauge::internal_stats::initialize(internal_stats& stats, value_type value, time_point timestamp) {
	stats.values(value);
}

void gauge::internal_stats::update_value(internal_stats& stats, value_type value, time_point timestamp) {
	stats.values(value);
}

gauge::gauge()
{
	internal_stats::initialize(stats);
}

gauge::gauge(value_type value, time_point timestamp)
	: value(value)
	, timestamp(timestamp)
{
	internal_stats::initialize(stats, value, timestamp);
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
