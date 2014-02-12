#include <handystats/metrics/timer.hpp>

namespace handystats { namespace metrics {

timer::timer() {
	this->timestamp = time_point();
	this->duration = value_type();
}

timer::timer(time_point start_timestamp) {
	this->timestamp = start_timestamp;
	this->duration = value_type();
}


void timer::start(time_point timestamp) {
	this->timestamp = timestamp;
}

void timer::finish(time_point timestamp) {
	duration += std::chrono::duration_cast<value_type>(timestamp - this->timestamp);

	this->timestamp = timestamp;
}


std::pair<timer::value_type, timer::time_point> timer::get() const {
	return std::make_pair(this->duration, this->timestamp);
}

}} // namespace handystats::metrics
