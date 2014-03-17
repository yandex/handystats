#ifndef HANDYSTATS_TIMER_MEASURING_POINTS_H_
#define HANDYSTATS_TIMER_MEASURING_POINTS_H_

#include <string>
#include <cstdint>

#include <boost/preprocessor/list/cat.hpp>
#include <boost/preprocessor/facilities/overload.hpp>

#include <handystats/metrics/timer.hpp>

void HANDY_TIMER_INIT(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		);

void HANDY_TIMER_START(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		);

void HANDY_TIMER_STOP(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		);

void HANDY_TIMER_DISCARD(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		);

void HANDY_TIMER_HEARTBEAT(
		const std::string timer_name,
		const uint64_t instance_id = -1,
		handystats::metrics::timer::time_point timestamp = handystats::metrics::timer::clock::now()
		);


namespace handystats { namespace measuring_points {

/*
 * Helper struct.
 * On construction HANDY_TIMER_START event is generated.
 * On destruction HANDY_TIMER_STOP event is generated.
 */
struct scoped_timer_helper {
	const std::string timer_name;
	const uint64_t instance_id;

	scoped_timer_helper(const std::string& timer_name, const uint64_t instance_id)
		: timer_name(timer_name), instance_id(instance_id)
	{
		HANDY_TIMER_START(timer_name, instance_id);
	}

	~scoped_timer_helper() {
		HANDY_TIMER_STOP(timer_name, instance_id);
	}
};

}} // namespace handystats::measuring_points


/*
 * Helper macros.
 */
#define UNIQUE_SCOPED_TIMER_NAME BOOST_PP_LIST_CAT((HANDY_SCOPED_TIMER_VAR_, (__LINE__, BOOST_PP_NIL)))

#define HANDY_TIMER_SCOPE_1(timer_name) HANDY_TIMER_SCOPE_2(timer_name, -1)

#define HANDY_TIMER_SCOPE_2(timer_name, instance_id) \
	handystats::measuring_points::scoped_timer_helper UNIQUE_SCOPED_TIMER_NAME (timer_name, instance_id)

/*
 * HANDY_TIMER_SCOPE event constructs scoped_timer_helper named variable.
 */
#define HANDY_TIMER_SCOPE(...) BOOST_PP_OVERLOAD(HANDY_TIMER_SCOPE_,__VA_ARGS__)(__VA_ARGS__)

#endif // HANDYSTATS_TIMER_MEASURING_POINTS_H_
