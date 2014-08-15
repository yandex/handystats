// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_TIMER_MEASURING_POINTS_HPP_
#define HANDYSTATS_TIMER_MEASURING_POINTS_HPP_

#include <string>
#include <cstdint>

#include <boost/preprocessor/list/cat.hpp>

#include <handystats/metrics/timer.hpp>
#include <handystats/macro_overload.hpp>


namespace handystats { namespace measuring_points {

void timer_init(
		const std::string& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id = handystats::metrics::timer::DEFAULT_INSTANCE_ID,
		const handystats::metrics::timer::time_point& timestamp = handystats::metrics::timer::clock::now()
	);

void timer_start(
		const std::string& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id = handystats::metrics::timer::DEFAULT_INSTANCE_ID,
		const handystats::metrics::timer::time_point& timestamp = handystats::metrics::timer::clock::now()
	);

void timer_stop(
		const std::string& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id = handystats::metrics::timer::DEFAULT_INSTANCE_ID,
		const handystats::metrics::timer::time_point& timestamp = handystats::metrics::timer::clock::now()
	);

void timer_discard(
		const std::string& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id = handystats::metrics::timer::DEFAULT_INSTANCE_ID,
		const handystats::metrics::timer::time_point& timestamp = handystats::metrics::timer::clock::now()
	);

void timer_heartbeat(
		const std::string& timer_name,
		const handystats::metrics::timer::instance_id_type& instance_id = handystats::metrics::timer::DEFAULT_INSTANCE_ID,
		const handystats::metrics::timer::time_point& timestamp = handystats::metrics::timer::clock::now()
	);

/*
 * Helper struct.
 * On construction HANDY_TIMER_START event is generated.
 * On destruction HANDY_TIMER_STOP event is generated.
 */
struct scoped_timer_helper {
	const std::string timer_name;
	const handystats::metrics::timer::instance_id_type instance_id;

	scoped_timer_helper(const std::string& timer_name, const handystats::metrics::timer::instance_id_type& instance_id)
		: timer_name(timer_name), instance_id(instance_id)
	{
		timer_start(timer_name, instance_id);
	}

	~scoped_timer_helper() {
		timer_stop(timer_name, instance_id);
	}
};

}} // namespace handystats::measuring_points


#ifndef HANDYSTATS_DISABLE

	#define HANDY_TIMER_INIT(...) handystats::measuring_points::timer_init(__VA_ARGS__)

	#define HANDY_TIMER_START(...) handystats::measuring_points::timer_start(__VA_ARGS__)

	#define HANDY_TIMER_STOP(...) handystats::measuring_points::timer_stop(__VA_ARGS__)

	#define HANDY_TIMER_DISCARD(...) handystats::measuring_points::timer_discard(__VA_ARGS__)

	#define HANDY_TIMER_HEARTBEAT(...) handystats::measuring_points::timer_heartbeat(__VA_ARGS__)

#else

	#define HANDY_TIMER_INIT(...)

	#define HANDY_TIMER_START(...)

	#define HANDY_TIMER_STOP(...)

	#define HANDY_TIMER_DISCARD(...)

	#define HANDY_TIMER_HEARTBEAT(...)

#endif


/*
 * Helper scope macros.
 */
#define UNIQUE_SCOPED_TIMER_NAME BOOST_PP_LIST_CAT((HANDY_SCOPED_TIMER_VAR_, (__LINE__, BOOST_PP_NIL)))

#define HANDY_TIMER_SCOPE_1(timer_name) HANDY_TIMER_SCOPE_2(timer_name, handystats::metrics::timer::DEFAULT_INSTANCE_ID)

#define HANDY_TIMER_SCOPE_2(timer_name, instance_id) \
	handystats::measuring_points::scoped_timer_helper UNIQUE_SCOPED_TIMER_NAME (timer_name, instance_id)

/*
 * HANDY_TIMER_SCOPE event constructs scoped_timer_helper named variable.
 */
#ifndef HANDYSTATS_DISABLE

	#define HANDY_TIMER_SCOPE(...) HANDY_PP_OVERLOAD(HANDY_TIMER_SCOPE_,__VA_ARGS__)(__VA_ARGS__)

#else

	#define HANDY_TIMER_SCOPE(...)

#endif

#endif // HANDYSTATS_TIMER_MEASURING_POINTS_HPP_
