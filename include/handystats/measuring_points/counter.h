/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDY_COUNTER_MEASURING_POINTS_H_
#define HANDY_COUNTER_MEASURING_POINTS_H_

#include <stdint.h>

#include <boost/preprocessor/list/cat.hpp>

#include <handystats/macros.h>
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

		#define HANDY_COUNTER_INIT(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats_counter_init, __VA_ARGS__)

		#define HANDY_COUNTER_INCREMENT(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats_counter_increment, __VA_ARGS__)

		#define HANDY_COUNTER_DECREMENT(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats_counter_decrement, __VA_ARGS__)

		#define HANDY_COUNTER_CHANGE(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats_counter_change, __VA_ARGS__)

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

		#define HANDY_COUNTER_SCOPE(counter_name, delta_value) \
			BOOST_PP_EXPAND( HANDY_PP_TUPLE_REM() \
				BOOST_PP_IF( \
					HANDY_PP_IS_TUPLE(counter_name), \
					( \
						HANDY_PP_METRIC_NAME_BUFFER_SET(counter_name); \
						struct handystats_scoped_counter_helper C_UNIQUE_SCOPED_COUNTER_NAME __attribute__((cleanup(handystats_scoped_counter_cleanup))) = \
							{HANDY_PP_METRIC_NAME_BUFFER_VAR, delta_value}; \
						HANDY_COUNTER_INCREMENT(HANDY_PP_METRIC_NAME_BUFFER_VAR, delta_value) \
					), \
					( \
						struct handystats_scoped_counter_helper C_UNIQUE_SCOPED_COUNTER_NAME __attribute__((cleanup(handystats_scoped_counter_cleanup))) = \
							{counter_name, delta_value}; \
						HANDY_COUNTER_INCREMENT(counter_name, delta_value) \
					) \
				) \
			)

	#else

		#define HANDY_COUNTER_SCOPE(...)

	#endif

#endif

#endif // HANDYSTATS_COUNTER_MEASURING_POINTS_H_
