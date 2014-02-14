#ifndef HANDYSTATS_CORE_H_
#define HANDYSTATS_CORE_H_

#include <thread>

#include "handystats/chrono.hpp"
#include "handystats/message_queue.hpp"
#include "handystats/internal.hpp"


namespace handystats {

extern metrics::gauge event_message_queue_size;
extern metrics::gauge monitors_size;
extern metrics::gauge message_processing_time;

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

#endif // HANDYSTATS_CORE_H_
