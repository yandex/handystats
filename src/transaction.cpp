#include <handystats/transaction.hpp>

namespace handystats {

void transaction::internal_stats::initialize(internal_stats& stats) {
	stats.running_time = time_duration(0);
	stats.paused_time = time_duration(0);
	stats.blocked_time = time_duration(0);
}

void transaction::internal_stats::update(internal_stats& stats, time_duration interval_time, state prev_state) {
	switch (prev_state) {
		case NOT_STARTED: case STOPPED:
			break;
		case RUNNING:
			stats.running_time += interval_time;
			break;
		case PAUSED:
			stats.paused_time += interval_time;
			break;
		case BLOCKED:
			stats.blocked_time += interval_time;
			break;
	}
}


transaction::transaction() {
	internal_stats::initialize(stats);

	last_timestamp = time_point();
	last_state = state::NOT_STARTED;
	final_status = status::NOT_DEFINED;
}

transaction::transaction(time_point start_timestamp, state initial_state) {
	internal_stats::initialize(stats);

	last_timestamp = start_timestamp;
	last_state = initial_state;
	final_status = status::NOT_DEFINED;
}


void transaction::start(time_point timestamp) {
	internal_stats::update(stats, timestamp - last_timestamp, last_state);
	last_timestamp = timestamp;

	last_state = state::RUNNING;
}

void transaction::stop(time_point timestamp, status final_status) {
	internal_stats::update(stats, timestamp - last_timestamp, last_state);
	last_timestamp = timestamp;

	last_state = state::STOPPED;
	this->final_status = final_status;
}


void transaction::pause(time_point timestamp) {
	internal_stats::update(stats, timestamp - last_timestamp, last_state);
	last_timestamp = timestamp;

	if (last_state == state::RUNNING) {
		last_state = state::PAUSED;
	}
}

void transaction::resume(time_point timestamp) {
	internal_stats::update(stats, timestamp - last_timestamp, last_state);
	last_timestamp = timestamp;

	if (last_state == state::PAUSED) {
		last_state = state::RUNNING;
	}
}


void transaction::heartbeat(time_point timestamp) {
	internal_stats::update(stats, timestamp - last_timestamp, last_state);
	last_timestamp = timestamp;
}


void transaction::block(time_point timestamp) {
	internal_stats::update(stats, timestamp - last_timestamp, last_state);
	last_timestamp = timestamp;

	if (last_state == state::RUNNING) {
		last_state = state::BLOCKED;
	}
}

void transaction::unblock(time_point timestamp) {
	internal_stats::update(stats, timestamp - last_timestamp, last_state);
	last_timestamp = timestamp;

	if (last_state == state::BLOCKED) {
		last_state = state::RUNNING;
	}
}


transaction::status transaction::get_status() const {
	return final_status;
}

transaction::state transaction::get_state() const {
	return last_state;
}

transaction::internal_stats transaction::get_stats() const {
	return stats;
}

} // namespace handystats
