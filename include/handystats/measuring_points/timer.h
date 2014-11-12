/*
  Copyright (c) 2014 Yandex LLC. All rights reserved.

  This file is part of Handystats.

  Handystats is free software; you can redistribute it and/or modify
  it under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 3 of the License, or
  (at your option) any later version.

  Handystats is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef HANDY_TIMER_MEASURING_POINTS_H_
#define HANDY_TIMER_MEASURING_POINTS_H_

#include <stdint.h>

#include <boost/preprocessor/list/cat.hpp>

#include <handystats/common.h>
#include <handystats/chrono.h>

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

HANDYSTATS_EXTERN_C
void handystats_timer_set(
		const char* timer_name,
		const int64_t measurement
	);

#ifndef __cplusplus
	#ifndef HANDYSTATS_DISABLE

		#define HANDY_TIMER_INIT(...) handystats_timer_init(__VA_ARGS__)

		#define HANDY_TIMER_START(...) handystats_timer_start(__VA_ARGS__)

		#define HANDY_TIMER_STOP(...) handystats_timer_stop(__VA_ARGS__)

		#define HANDY_TIMER_DISCARD(...) handystats_timer_discard(__VA_ARGS__)

		#define HANDY_TIMER_HEARTBEAT(...) handystats_timer_heartbeat(__VA_ARGS__)

		#define HANDY_TIMER_SET(...) handystats_timer_set(__VA_ARGS__)

	#else

		#define HANDY_TIMER_INIT(...)

		#define HANDY_TIMER_START(...)

		#define HANDY_TIMER_STOP(...)

		#define HANDY_TIMER_DISCARD(...)

		#define HANDY_TIMER_HEARTBEAT(...)

		#define HANDY_TIMER_SET(...)

	#endif

	struct handystats_scoped_timer_helper {
		const char* timer_name;
		const int64_t start_time;
	};

	static inline void handystats_scoped_timer_cleanup(struct handystats_scoped_timer_helper* scoped_timer) {
		int64_t end_time = handystats_now();
		HANDY_TIMER_SET(scoped_timer->timer_name, end_time - scoped_timer->start_time);
	}

	#define C_UNIQUE_SCOPED_TIMER_NAME BOOST_PP_LIST_CAT((C_HANDY_SCOPED_TIMER_VAR_, (__LINE__, BOOST_PP_NIL)))

	#ifndef HANDYSTATS_DISABLE

		#define HANDY_TIMER_SCOPE(timer_name) \
			struct handystats_scoped_timer_helper\
			C_UNIQUE_SCOPED_TIMER_NAME __attribute__((cleanup(handystats_scoped_timer_cleanup))) =\
				{timer_name, handystats_now()}

	#else

		#define HANDY_TIMER_SCOPE(...)

	#endif

#endif

#endif // HANDYSTATS_TIMER_MEASURING_POINTS_H_
