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

#ifndef HANDYSTATS_TIMER_MEASURING_POINTS_HPP_
#define HANDYSTATS_TIMER_MEASURING_POINTS_HPP_

#include <string>
#include <algorithm>
#include <cstdint>

#include <boost/preprocessor/list/cat.hpp>

#include <handystats/metrics/timer.hpp>
#include <handystats/macros.h>


namespace handystats { namespace measuring_points {

void timer_init(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id = metrics::timer::DEFAULT_INSTANCE_ID,
		const chrono::time_point& timestamp = chrono::internal_clock::now()
	);

void timer_start(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id = metrics::timer::DEFAULT_INSTANCE_ID,
		const chrono::time_point& timestamp = chrono::internal_clock::now()
	);

void timer_stop(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id = metrics::timer::DEFAULT_INSTANCE_ID,
		const chrono::time_point& timestamp = chrono::internal_clock::now()
	);

void timer_discard(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id = metrics::timer::DEFAULT_INSTANCE_ID,
		const chrono::time_point& timestamp = chrono::internal_clock::now()
	);

void timer_heartbeat(
		std::string&& timer_name,
		const metrics::timer::instance_id_type& instance_id = metrics::timer::DEFAULT_INSTANCE_ID,
		const chrono::time_point& timestamp = chrono::internal_clock::now()
	);

void timer_set(
		std::string&& timer_name,
		const metrics::timer::value_type& measurement,
		const chrono::time_point& timestamp = chrono::internal_clock::now()
	);

/*
 * Helper struct.
 * On construction HANDY_TIMER_START event is generated.
 * On destruction HANDY_TIMER_STOP event is generated.
 */
struct scoped_timer_helper {
	const std::string timer_name;
	const chrono::time_point start_time;

	scoped_timer_helper(std::string&& timer_name, const chrono::time_point& start_time)
		: timer_name(std::move(timer_name)), start_time(start_time)
	{
	}

	~scoped_timer_helper() {
		auto end_time = chrono::internal_clock::now();
		timer_set(timer_name.substr(), end_time - start_time);
	}
};

}} // namespace handystats::measuring_points


#ifndef HANDYSTATS_DISABLE

	#define HANDY_TIMER_INIT(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::timer_init, __VA_ARGS__)

	#define HANDY_TIMER_START(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::timer_start, __VA_ARGS__)

	#define HANDY_TIMER_STOP(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::timer_stop, __VA_ARGS__)

	#define HANDY_TIMER_DISCARD(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::timer_discard, __VA_ARGS__)

	#define HANDY_TIMER_HEARTBEAT(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::timer_heartbeat, __VA_ARGS__)

	#define HANDY_TIMER_SET(...) HANDY_PP_MEASURING_POINT_WRAPPER(handystats::measuring_points::timer_set, __VA_ARGS__)

#else

	#define HANDY_TIMER_INIT(...)

	#define HANDY_TIMER_START(...)

	#define HANDY_TIMER_STOP(...)

	#define HANDY_TIMER_DISCARD(...)

	#define HANDY_TIMER_HEARTBEAT(...)

	#define HANDY_TIMER_SET(...)

#endif


/*
 * Helper scope macros.
 */
#define UNIQUE_SCOPED_TIMER_NAME BOOST_PP_LIST_CAT((HANDY_SCOPED_TIMER_VAR_, (__LINE__, BOOST_PP_NIL)))

/*
 * HANDY_TIMER_SCOPE event constructs scoped_timer_helper named variable.
 */
#ifndef HANDYSTATS_DISABLE

	#define HANDY_TIMER_SCOPE(timer_name) \
		BOOST_PP_EXPAND( BOOST_PP_TUPLE_REM() \
			BOOST_PP_IF( \
				HANDY_PP_IS_TUPLE(timer_name), \
				( \
					HANDY_PP_METRIC_NAME_BUFFER_SET(counter_name); \
					handystats::measuring_points::scoped_timer_helper UNIQUE_SCOPED_TIMER_NAME \
						(HANDY_PP_METRIC_NAME_BUFFER_VAR, handystats::chrono::internal_clock::now()) \
				), \
				( \
					handystats::measuring_points::scoped_timer_helper UNIQUE_SCOPED_TIMER_NAME \
						(timer_name, handystats::chrono::internal_clock::now()) \
				) \
			) \
		)

#else

	#define HANDY_TIMER_SCOPE(...)

#endif

#endif // HANDYSTATS_TIMER_MEASURING_POINTS_HPP_
