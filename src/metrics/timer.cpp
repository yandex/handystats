#include <handystats/metrics/timer.hpp>

namespace handystats { namespace metrics {

void timer::internal_stats::initialize(internal_stats& stats) {
	stats.duration = time_duration(0);
}

void timer::internal_stats::update(internal_stats& stats, time_duration timer_time) {
	stats.duration += timer_time;
}


timer::timer() {
	internal_stats::initialize(stats);

	start_timestamp = time_point();
	finish_timestamp = time_point();
}

timer::timer(time_point start_timestamp) {
	internal_stats::initialize(stats);

	this->start_timestamp = start_timestamp;
	this->finish_timestamp = time_point();
}


void timer::start(time_point timestamp) {
	start_timestamp = timestamp;
}

void timer::finish(time_point timestamp) {
	internal_stats::update(stats, timestamp - start_timestamp);

	finish_timestamp = timestamp;
}


timer::internal_stats timer::get_stats() const {
	return stats;
}

}} // namespace handystats::metrics
