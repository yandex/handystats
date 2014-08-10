// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDY_COUNTER_MEASURING_POINTS_HPP_
#define HANDY_COUNTER_MEASURING_POINTS_HPP_

#include <string>

#include <boost/preprocessor/list/cat.hpp>

#include <handystats/metrics/counter.hpp>
#include <handystats/macro_overload.hpp>

void HANDY_COUNTER_INIT(
		const std::string& counter_name,
		const handystats::metrics::counter::value_type& init_value = handystats::metrics::counter::value_type(),
		const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now()
		);

void HANDY_COUNTER_INCREMENT(
		const std::string& counter_name,
		const handystats::metrics::counter::value_type& value = 1,
		const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now()
		);

void HANDY_COUNTER_DECREMENT(
		const std::string& counter_name,
		const handystats::metrics::counter::value_type& value = 1,
		const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now()
		);

void HANDY_COUNTER_CHANGE(
		const std::string& counter_name,
		const handystats::metrics::counter::value_type& value,
		const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now()
		);


namespace handystats { namespace measuring_points {

/*
 * Helper struct.
 * On construction HANDY_COUNTER_CHANGE event with +delta value is generated.
 * On destruction HANDY_COUNTER_CHANGE event with -delta value is generated.
 */
struct scoped_counter_helper {
	const std::string counter_name;
	const handystats::metrics::counter::value_type delta_value;

	scoped_counter_helper(const std::string& counter_name, const handystats::metrics::counter::value_type& delta_value)
		: counter_name(counter_name), delta_value(delta_value)
	{
		HANDY_COUNTER_CHANGE(counter_name, delta_value);
	}

	~scoped_counter_helper() {
		HANDY_COUNTER_CHANGE(counter_name, -delta_value);
	}
};

}} // namespace handystats::measuring_points


/*
 * Helper macros.
 */
#define UNIQUE_SCOPED_COUNTER_NAME BOOST_PP_LIST_CAT((HANDY_SCOPED_COUNTER_VAR_, (__LINE__, BOOST_PP_NIL)))

#define HANDY_COUNTER_SCOPE_1(counter_name) HANDY_COUNTER_SCOPE_2(counter_name, 1)

#define HANDY_COUNTER_SCOPE_2(counter_name, delta_value) \
	handystats::measuring_points::scoped_counter_helper UNIQUE_SCOPED_COUNTER_NAME (counter_name, delta_value)

/*
 * HANDY_COUNTER_SCOPE event constructs scoped_counter_helper named variable.
 */
#define HANDY_COUNTER_SCOPE(...) HANDY_PP_OVERLOAD(HANDY_COUNTER_SCOPE_,__VA_ARGS__)(__VA_ARGS__)

#endif // HANDYSTATS_COUNTER_MEASURING_POINTS_HPP_
