#ifndef HANDYSTATS_TIMER_EVENT_H_
#define HANDYSTATS_TIMER_EVENT_H_

namespace handystats { namespace events {

enum class timer_event {
	INIT,
	START,
	STOP,
	DISCARD,
	HEARTBEAT
};

}} // namespace handystats::events


#endif // HANDYSTATS_TIMER_EVENT_H_
