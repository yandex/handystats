#ifndef HANDYSTATS_COUNTER_EVENT_H_
#define HANDYSTATS_COUNTER_EVENT_H_

namespace handystats { namespace events {

enum class counter_event {
	INIT,
	INCREMENT,
	DECREMENT
};

}} // namespace handystats::events

#endif // HANDYSTATS_COUNTER_EVENT_H_
