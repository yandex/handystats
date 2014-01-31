#include <handystats/metrics/gauge.hpp>

namespace handystats { namespace metrics {

void gauge::internal_stats::initialize(internal_stats& stats) {
	stats.count = 0;
}

void gauge::internal_stats::initialize(internal_stats& stats, value_type value, time_point timestamp) {
	stats.count = 1;

	stats.min_value = value;
	stats.min_value_timestamp = timestamp;

	stats.max_value = value;
	stats.max_value_timestamp = timestamp;
}

void gauge::internal_stats::update(internal_stats& stats, value_type value, time_point timestamp) {
	++stats.count;

	if (stats.min_value > value) {
		stats.min_value = value;
		stats.min_value_timestamp = timestamp;
	}

	if (stats.max_value < value) {
		stats.max_value = value;
		stats.max_value_timestamp = timestamp;
	}
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

	if (stats.count == 0) {
		internal_stats::initialize(stats, this->value, this->timestamp);
	}
	else {
		internal_stats::update(stats, this->value, this->timestamp);
	}
}

std::pair<gauge::value_type, gauge::time_point> gauge::get() const {
	return std::make_pair(value, timestamp);
}

gauge::internal_stats gauge::get_stats() const {
	return stats;
}

}} // namespace handystats::metrics
