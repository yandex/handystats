// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDY_TIMER_MEASURING_POINTS_H_
#define HANDY_TIMER_MEASURING_POINTS_H_

#include <stdint.h>

#include <boost/preprocessor/list/cat.hpp>

#include <handystats/handystats.h>

HANDYSTATS_EXTERN_C
void handystats_timer_init(
		const char* timer_name,
		const uint64_t instance_id
	);

HANDYSTATS_EXTERN_C
void handystats_timer_start(
		const char* timer_name,
		const uint64_t instance_id
	);

HANDYSTATS_EXTERN_C
void handystats_timer_stop(
		const char* timer_name,
		const uint64_t instance_id
	);

HANDYSTATS_EXTERN_C
void handystats_timer_discard(
		const char* timer_name,
		const uint64_t instance_id
	);

HANDYSTATS_EXTERN_C
void handystats_timer_heartbeat(
		const char* timer_name,
		const uint64_t instance_id
	);


#ifndef __cplusplus
	#ifndef HANDYSTATS_DISABLE

		#define HANDY_TIMER_INIT(...) handystats_timer_init(__VA_ARGS__)

		#define HANDY_TIMER_START(...) handystats_timer_start(__VA_ARGS__)

		#define HANDY_TIMER_STOP(...) handystats_timer_stop(__VA_ARGS__)

		#define HANDY_TIMER_DISCARD(...) handystats_timer_discard(__VA_ARGS__)

		#define HANDY_TIMER_HEARTBEAT(...) handystats_timer_heartbeat(__VA_ARGS__)

	#else

		#define HANDY_TIMER_INIT(...)

		#define HANDY_TIMER_START(...)

		#define HANDY_TIMER_STOP(...)

		#define HANDY_TIMER_DISCARD(...)

		#define HANDY_TIMER_HEARTBEAT(...)

	#endif

	struct handystats_scoped_timer_helper {
		const char* timer_name;
		const uint64_t instance_id;
	};

	inline void handystats_scoped_timer_cleanup(struct handystats_scoped_timer_helper* scoped_timer) {
		HANDY_TIMER_STOP(scoped_timer->timer_name, scoped_timer->instance_id);
	}

	#define C_UNIQUE_SCOPED_TIMER_NAME BOOST_PP_LIST_CAT((C_HANDY_SCOPED_TIMER_VAR_, (__LINE__, BOOST_PP_NIL)))

	#ifndef HANDYSTATS_DISABLE

		#define HANDY_TIMER_SCOPE(...) \
			struct handystats_scoped_timer_helper C_UNIQUE_SCOPED_TIMER_NAME __attribute__((cleanup(handystats_scoped_timer_cleanup))) = {__VA_ARGS__};\
			HANDY_TIMER_START(__VA_ARGS__)

	#else

		#define HANDY_TIMER_SCOPE(...)

	#endif

#endif

#endif // HANDYSTATS_TIMER_MEASURING_POINTS_H_
