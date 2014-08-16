// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_COUNTER_MEASURING_POINTS_HPP_
#define HANDYSTATS_COUNTER_MEASURING_POINTS_HPP_

#include <string>
#include <algorithm>

#include <boost/preprocessor/list/cat.hpp>

#include <handystats/metrics/counter.hpp>
#include <handystats/macro_overload.hpp>


namespace handystats { namespace measuring_points {

void counter_init(
		std::string&& counter_name,
		const handystats::metrics::counter::value_type& init_value = handystats::metrics::counter::value_type(),
		const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now()
		);

void counter_increment(
		std::string&& counter_name,
		const handystats::metrics::counter::value_type& value = 1,
		const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now()
		);

void counter_decrement(
		std::string&& counter_name,
		const handystats::metrics::counter::value_type& value = 1,
		const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now()
		);

void counter_change(
		std::string&& counter_name,
		const handystats::metrics::counter::value_type& value,
		const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now()
		);

/*
 * Helper struct.
 * On construction HANDY_COUNTER_CHANGE event with +delta value is generated.
 * On destruction HANDY_COUNTER_CHANGE event with -delta value is generated.
 */
struct scoped_counter_helper {
	const std::string counter_name;
	const handystats::metrics::counter::value_type delta_value;

	scoped_counter_helper(std::string&& counter_name, const handystats::metrics::counter::value_type& delta_value)
		: counter_name(std::move(counter_name)), delta_value(delta_value)
	{
		counter_change(this->counter_name.substr(), delta_value);
	}

	~scoped_counter_helper() {
		counter_change(counter_name.substr(), -delta_value);
	}
};

}} // namespace handystats::measuring_points


#ifndef HANDYSTATS_DISABLE

	#define HANDY_COUNTER_INIT(...) handystats::measuring_points::counter_init(__VA_ARGS__)

	#define HANDY_COUNTER_INCREMENT(...) handystats::measuring_points::counter_increment(__VA_ARGS__)

	#define HANDY_COUNTER_DECREMENT(...) handystats::measuring_points::counter_decrement(__VA_ARGS__)

	#define HANDY_COUNTER_CHANGE(...) handystats::measuring_points::counter_change(__VA_ARGS__)

#else

	#define HANDY_COUNTER_INIT(...)

	#define HANDY_COUNTER_INCREMENT(...)

	#define HANDY_COUNTER_DECREMENT(...)

	#define HANDY_COUNTER_CHANGE(...)

#endif


/*
 * Helper scope macros.
 */
#define UNIQUE_SCOPED_COUNTER_NAME BOOST_PP_LIST_CAT((HANDY_SCOPED_COUNTER_VAR_, (__LINE__, BOOST_PP_NIL)))

#define HANDY_COUNTER_SCOPE_1(counter_name) HANDY_COUNTER_SCOPE_2(counter_name, 1)

#define HANDY_COUNTER_SCOPE_2(counter_name, delta_value) \
	handystats::measuring_points::scoped_counter_helper UNIQUE_SCOPED_COUNTER_NAME (counter_name, delta_value)

/*
 * HANDY_COUNTER_SCOPE event constructs scoped_counter_helper named variable.
 */
#ifndef HANDYSTATS_DISABLE

	#define HANDY_COUNTER_SCOPE(...) HANDY_PP_OVERLOAD(HANDY_COUNTER_SCOPE_,__VA_ARGS__)(__VA_ARGS__)

#else

	#define HANDY_COUNTER_SCOPE(...)

#endif

#endif // HANDYSTATS_COUNTER_MEASURING_POINTS_HPP_
