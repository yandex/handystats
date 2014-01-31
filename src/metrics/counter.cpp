#include <handystats/metrics/counter.hpp>

namespace handystats { namespace metrics {

void counter::internal_stats::initialize(internal_stats& stats) {
	stats.count = 0;
}

void counter::internal_stats::initialize(internal_stats& stats, value_type value, time_point timestamp) {
	stats.count = 1;

	stats.min_value = value;
	stats.min_value_timestamp = timestamp;

	stats.max_value = value;
	stats.max_value_timestamp = timestamp;
}

void counter::internal_stats::update(internal_stats& stats, value_type value, time_point timestamp) {
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

counter::counter()
	: value(0)
{
	internal_stats::initialize(stats);
}

counter::counter(value_type value, time_point timestamp)
	: value(value)
	, timestamp(timestamp)
{
	internal_stats::initialize(stats, value, timestamp);
}


void counter::increment(value_type value, time_point timestamp) {
	this->value += value;
	this->timestamp = timestamp;

	if (stats.count == 0) {
		internal_stats::initialize(stats, this->value, this->timestamp);
	}
	else {
		internal_stats::update(stats, this->value, this->timestamp);
	}
}

void counter::decrement(value_type value, time_point timestamp) {
	this->value -= value;
	this->timestamp = timestamp;

	if (stats.count == 0) {
		internal_stats::initialize(stats, this->value, this->timestamp);
	}
	else {
		internal_stats::update(stats, this->value, this->timestamp);
	}
}

std::pair<counter::value_type, counter::time_point> counter::get() const {
	return std::make_pair(value, timestamp);
}

counter::internal_stats counter::get_stats() const {
	return stats;
}

}} // namespace handystats::metrics
