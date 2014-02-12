#include <handystats/metrics/counter.hpp>

namespace handystats { namespace metrics {

void counter::internal_stats::update_value(value_type value, time_point timestamp) {
	values.set(value, timestamp);
}

void counter::internal_stats::update_increment(value_type delta, time_point timestamp) {
	incr_deltas.set(delta, timestamp);
	deltas.set(delta, timestamp);
}

void counter::internal_stats::update_decrement(value_type delta, time_point timestamp) {
	decr_deltas.set(delta, timestamp);
	deltas.set(-delta, timestamp);
}

counter::counter()
	: value()
	, timestamp(chrono::default_clock::now())
{
	stats.values.set(this->value, this->timestamp);
}

counter::counter(value_type value, time_point timestamp)
	: value(value)
	, timestamp(timestamp)
{
	stats.values.set(this->value, this->timestamp);
}


void counter::increment(value_type incr_value, time_point timestamp) {
	this->value += incr_value;
	this->timestamp = timestamp;

	stats.update_value(this->value, this->timestamp);
	stats.update_increment(incr_value, this->timestamp);
}

void counter::decrement(value_type decr_value, time_point timestamp) {
	this->value -= decr_value;
	this->timestamp = timestamp;

	stats.update_value(this->value, this->timestamp);
	stats.update_decrement(decr_value, this->timestamp);
}


std::pair<counter::value_type, counter::time_point> counter::get() const {
	return std::make_pair(value, timestamp);
}

counter::internal_stats counter::get_stats() const {
	return stats;
}

}} // namespace handystats::metrics
