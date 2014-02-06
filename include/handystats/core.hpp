#ifndef HANDYSTATS_CORE_H_
#define HANDYSTATS_CORE_H_

#include <thread>

#include "handystats/chrono.hpp"

namespace handystats {

typedef std::chrono::nanoseconds time_duration;
typedef chrono::steady_clock<time_duration> clock;
typedef clock::time_point time_point;


extern bool handy_enabled;

inline bool is_handy_enabled() {
	return handy_enabled;
}

void enable_handy();
void disable_handy();


extern std::thread* processor_thread;
void process_message_queue();

void initialize();
void clean_up();

} // namespace handystats

inline void HANDY_ENABLE() {
	if (!handystats::is_handy_enabled()) {
		handystats::enable_handy();
	}
}

inline void HANDY_DISABLE() {
	if (handystats::is_handy_enabled()) {
		handystats::disable_handy();
	}
}

#endif // HANDYSTATS_MESSAGE_QUEUE_PROCESSOR_H_
