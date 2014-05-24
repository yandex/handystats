#include <handystats/metrics/timer.hpp>


namespace handystats { namespace metrics {

timer::timer() {
	this->timestamp = time_point();
	this->value = value_type();
}

timer::timer(time_point start_timestamp) {
	this->timestamp = start_timestamp;
	this->value = value_type();
}


void timer::start(time_point timestamp) {
	this->timestamp = timestamp;
}

void timer::stop(time_point timestamp) {
	if (timestamp >= this->timestamp) {
		this->value += timestamp - this->timestamp;
		this->timestamp = timestamp;
	}
}

void timer::reset() {
	*this = timer();
}

}} // namespace handystats::metrics
