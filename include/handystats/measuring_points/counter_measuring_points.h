// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDY_COUNTER_MEASURING_POINTS_H_
#define HANDY_COUNTER_MEASURING_POINTS_H_

#include <stdint.h>

#include <boost/preprocessor/list/cat.hpp>

#include <handystats/common.h>

HANDYSTATS_EXTERN_C
void handystats_counter_init(
		const char* counter_name,
		const int64_t init_value
	);

HANDYSTATS_EXTERN_C
void handystats_counter_increment(
		const char* counter_name,
		const int64_t value
	);

HANDYSTATS_EXTERN_C
void handystats_counter_decrement(
		const char* counter_name,
		const int64_t value
	);

HANDYSTATS_EXTERN_C
void handystats_counter_change(
		const char* counter_name,
		const int64_t value
	);


#ifndef __cplusplus
	#ifndef HANDYSTATS_DISABLE

		#define HANDY_COUNTER_INIT(...) handystats_counter_init(__VA_ARGS__)

		#define HANDY_COUNTER_INCREMENT(...) handystats_counter_increment(__VA_ARGS__)

		#define HANDY_COUNTER_DECREMENT(...) handystats_counter_decrement(__VA_ARGS__)

		#define HANDY_COUNTER_CHANGE(...) handystats_counter_change(__VA_ARGS__)

	#else

		#define HANDY_COUNTER_INIT(...)

		#define HANDY_COUNTER_INCREMENT(...)

		#define HANDY_COUNTER_DECREMENT(...)

		#define HANDY_COUNTER_CHANGE(...)

	#endif

	struct handystats_scoped_counter_helper {
		const char* counter_name;
		const int64_t delta_value;
	};

	static inline void handystats_scoped_counter_cleanup(struct handystats_scoped_counter_helper* scoped_counter) {
		HANDY_COUNTER_DECREMENT(scoped_counter->counter_name, scoped_counter->delta_value);
	}

	#define C_UNIQUE_SCOPED_COUNTER_NAME BOOST_PP_LIST_CAT((C_HANDY_SCOPED_COUNTER_VAR_, (__LINE__, BOOST_PP_NIL)))

	#ifndef HANDYSTATS_DISABLE

		#define HANDY_COUNTER_SCOPE(...) \
			struct handystats_scoped_counter_helper C_UNIQUE_SCOPED_COUNTER_NAME __attribute__((cleanup(handystats_scoped_counter_cleanup))) = {__VA_ARGS__};\
			HANDY_COUNTER_INCREMENT(__VA_ARGS__)

	#else

		#define HANDY_COUNTER_SCOPE(...)

	#endif

#endif

#endif // HANDYSTATS_COUNTER_MEASURING_POINTS_H_
