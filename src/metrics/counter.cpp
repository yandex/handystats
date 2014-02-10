#include <handystats/metrics/counter.hpp>

namespace handystats { namespace metrics {

void counter::internal_stats::initialize(internal_stats& stats) {
}

void counter::internal_stats::initialize(internal_stats& stats, value_type value, time_point timestamp) {
	stats.values(value);
}

void counter::internal_stats::update_value(internal_stats& stats, value_type value, time_point timestamp) {
	stats.values(value);
}

void counter::internal_stats::update_increment(internal_stats& stats, value_type delta, time_point timestamp) {
	stats.deltas(delta);
	stats.incr_deltas(delta);
}

void counter::internal_stats::update_decrement(internal_stats& stats, value_type delta, time_point timestamp) {
	stats.deltas(delta);
	stats.decr_deltas(delta);
}

counter::counter()
	: value(0)
{
	internal_stats::initialize(stats, value, chrono::default_clock::now());
}

counter::counter(value_type value, time_point timestamp)
	: value(value)
	, timestamp(timestamp)
{
	internal_stats::initialize(stats, value, timestamp);
}


void counter::increment(value_type incr_value, time_point timestamp) {
	this->value += incr_value;
	this->timestamp = timestamp;

	internal_stats::update_value(stats, this->value, this->timestamp);
	internal_stats::update_increment(stats, incr_value, this->timestamp);
}

void counter::decrement(value_type decr_value, time_point timestamp) {
	this->value -= decr_value;
	this->timestamp = timestamp;

	internal_stats::update_value(stats, this->value, this->timestamp);
	internal_stats::update_decrement(stats, decr_value, this->timestamp);
}


std::pair<counter::value_type, counter::time_point> counter::get() const {
	return std::make_pair(value, timestamp);
}

counter::internal_stats counter::get_stats() const {
	return stats;
}

}} // namespace handystats::metrics
