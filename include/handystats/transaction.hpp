#ifndef HANDYSTATS_TRANSACTION_H_
#define HANDYSTATS_TRANSACTION_H_

#include <handystats/chrono.hpp>

namespace handystats {

class transaction {
public:

	typedef std::chrono::nanoseconds time_duration;
	typedef typename chrono::steady_clock<time_duration> clock;
	typedef typename clock::time_point time_point;

	enum status {
		NOT_DEFINED = -1,
		SUCCESS,
		FAIL
	};

	enum state {
		NOT_STARTED = -1,
		RUNNING,
		PAUSED,
		BLOCKED,
		STOPPED
	};

	struct internal_stats {
		time_duration running_time;
		time_duration paused_time;
		time_duration blocked_time;

		static void initialize(internal_stats& stats);
		static void update(internal_stats& stats, time_duration interval_time, state prev_state);
	};

	transaction();
	transaction(time_point start_timestamp, state initial_state = state::NOT_STARTED);

	void start(time_point timestamp);
	void stop(time_point timestamp, status final_status);

	void pause(time_point timestamp);
	void resume(time_point timestamp);

	void heartbeat(time_point timestamp);

	void block(time_point timestamp);
	void unblock(time_point timestamp);

	status get_status() const;
	state get_state() const;
	internal_stats get_stats() const;

private:
	time_point last_timestamp;
	state last_state;

	status final_status;

	internal_stats stats;

}; // class transaction

} // namespace handystats

#endif // HANDYSTATS_TRANSACTION_H_
