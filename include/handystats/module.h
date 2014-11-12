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

#ifndef HANDYSTATS_MODULE_H_
#define HANDYSTATS_MODULE_H_

/*
 * Usage example:
 *
 * -------------------------------------------------------------------
 * #include <handystats/module.h>
 *
 * #ifndef _HAVE_HANDY_MODULE_CACHE
 * #define _HAVE_HANDY_MODULE_CACHE 1
 * #endif
 *
 * HANDY_MODULE(CACHE)
 *
 * #if _HAVE_HANDY_MODULE_CACHE == 1
 * #define CACHE_TIMER_SCOPE(...) HANDY_TIMER_SCOPE(__VA_ARGS__)
 * #define CACHE_COUNTER_SCOPE(...) HANDY_COUNTER_SCOPE(__VA_ARGS__)
 * #else
 * #define CACHE_TIMER_SCOPE(...)
 * #define CACHE_COUNTER_SCOPE(...)
 * #endif
 * --------------------------------------------------------------------
 *
 * To disable measuring points for specific module (e.g. CACHE) compile with -D_HAVE_HANDY_MODULE_CACHE=0
 */

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/seq/cat.hpp>
#include <boost/preprocessor/facilities/expand.hpp>

#include <handystats/common.h>

#ifndef __cplusplus
	#include <handystats/measuring_points.h>
#else
	#include <utility>

	#include <handystats/measuring_points.hpp>
#endif

#define HANDY_MODULE_METRIC_OP(LANG, MODULE, METRIC, OP, PARAMS, FORWARD_PARAMS) \
	/* LANG_HANDY_MODULE_METRIC_OP_0 */ \
	inline void \
	BOOST_PP_SEQ_CAT((LANG)(_HANDY_)(MODULE)(_)(METRIC)(_)(OP)(_0)) PARAMS {} \
	/* LANG_HANDY_MODULE_METRIC_OP_1 */ \
	inline void \
	BOOST_PP_SEQ_CAT((LANG)(_HANDY_)(MODULE)(_)(METRIC)(_)(OP)(_1)) PARAMS { \
		BOOST_PP_EXPAND(BOOST_PP_SEQ_CAT((HANDY_)(METRIC)(_)(OP)) FORWARD_PARAMS); \
	} \
	/* MODULE_METRIC_OP */ \
	inline void \
	BOOST_PP_SEQ_CAT((MODULE)(_)(METRIC)(_)(OP)) PARAMS { \
		BOOST_PP_EXPAND( \
			BOOST_PP_SEQ_CAT((LANG)(_HANDY_)(MODULE)(_)(METRIC)(_)(OP)(_)(BOOST_PP_CAT(_HAVE_HANDY_MODULE_, MODULE))) \
			FORWARD_PARAMS \
		); \
	} \
/**/

#ifndef __cplusplus

#define HANDY_MODULE(MODULE) \
	/* GAUGE */ \
		/* INIT */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, GAUGE, INIT, \
				(const char* gauge_name, const double init_value), \
				(gauge_name, init_value) \
			) \
		/* SET */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, GAUGE, SET, \
				(const char* gauge_name, const double value), \
				(gauge_name, value) \
			) \
	/* ATTRIBUTE */ \
		/* SET_BOOL */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, ATTRIBUTE, SET_BOOL, \
				(const char* attribute_name, const char b), \
				(attribute_name, b) \
			) \
		/* SET_INT */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, ATTRIBUTE, SET_INT, \
				(const char* attribute_name, const int i), \
				(attribute_name, i) \
			) \
		/* SET_UINT */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, ATTRIBUTE, SET_UINT, \
				(const char* attribute_name, const unsigned u), \
				(attribute_name, u) \
			) \
		/* SET_INT64 */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, ATTRIBUTE, SET_INT64, \
				(const char* attribute_name, const int64_t i64), \
				(attribute_name, i64) \
			) \
		/* SET_UINT64 */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, ATTRIBUTE, SET_UINT64, \
				(const char* attribute_name, const uint64_t u64), \
				(attribute_name, u64) \
			) \
		/* SET_DOUBLE */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, ATTRIBUTE, SET_DOUBLE, \
				(const char* attribute_name, const double d), \
				(attribute_name, d) \
			) \
		/* SET_STRING */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, ATTRIBUTE, SET_STRING, \
				(const char* attribute_name, const char* s), \
				(attribute_name, s) \
			) \
	/* COUNTER */ \
		/* INIT */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, COUNTER, INIT, \
				(const char* counter_name, const int64_t init_value), \
				(counter_name, init_value) \
			) \
		/* INCREMENT */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, COUNTER, INCREMENT, \
				(const char* counter_name, const int64_t value), \
				(counter_name, value) \
			) \
		/* DECREMENT */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, COUNTER, DECREMENT, \
				(const char* counter_name, const int64_t value), \
				(counter_name, value) \
			) \
		/* CHANGE */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, COUNTER, CHANGE, \
				(const char* counter_name, const int64_t value), \
				(counter_name, value) \
			) \
	/* TIMER */ \
		/* INIT */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, TIMER, INIT, \
				(const char* timer_name, const uint64_t instance_id), \
				(timer_name, instance_id) \
			) \
		/* START */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, TIMER, START, \
				(const char* timer_name, const uint64_t instance_id), \
				(timer_name, instance_id) \
			) \
		/* STOP */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, TIMER, STOP, \
				(const char* timer_name, const uint64_t instance_id), \
				(timer_name, instance_id) \
			) \
		/* DISCARD */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, TIMER, DISCARD, \
				(const char* timer_name, const uint64_t instance_id), \
				(timer_name, instance_id) \
			) \
		/* HEARTBEAT */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, TIMER, HEARTBEAT, \
				(const char* timer_name, const uint64_t instance_id), \
				(timer_name, instance_id) \
			) \
		/* SET */ \
		HANDY_MODULE_METRIC_OP(C, MODULE, TIMER, SET, \
				(const char* timer_name, const int64_t measurement), \
				(timer_name, measurement) \
			) \
/**/

#else // defined(__cplusplus)

#define HANDY_MODULE(MODULE) \
	/* GAUGE */ \
		/* INIT */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, GAUGE, INIT, \
				( \
					std::string&& gauge_name, \
					const handystats::metrics::gauge::value_type& init_value, \
					const handystats::metrics::gauge::time_point& timestamp = handystats::metrics::gauge::clock::now() \
				), \
				(std::move(gauge_name), init_value, timestamp) \
			) \
		/* SET */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, GAUGE, SET, \
				( \
					std::string&& gauge_name, \
					const handystats::metrics::gauge::value_type& value, \
					const handystats::metrics::gauge::time_point& timestamp = handystats::metrics::gauge::clock::now() \
				), \
				(std::move(gauge_name), value, timestamp) \
			) \
	/* ATTRIBUTE */ \
		/* SET */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, ATTRIBUTE, SET, \
				( \
					std::string&& attribute_name, \
					const handystats::attribute::value_type& value, \
					const handystats::attribute::time_point& timestamp = handystats::attribute::clock::now() \
				), \
				(std::move(attribute_name), value, timestamp) \
			) \
		/* SET_BOOL */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, ATTRIBUTE, SET_BOOL, \
				( \
					std::string&& attribute_name, \
					const bool& b, \
					const handystats::attribute::time_point& timestamp = handystats::attribute::clock::now() \
				), \
				(std::move(attribute_name), b, timestamp) \
			) \
		/* SET_INT */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, ATTRIBUTE, SET_INT, \
				( \
					std::string&& attribute_name, \
					const int& i, \
					const handystats::attribute::time_point& timestamp = handystats::attribute::clock::now() \
				), \
				(std::move(attribute_name), i, timestamp) \
			) \
		/* SET_UINT */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, ATTRIBUTE, SET_UINT, \
				( \
					std::string&& attribute_name, \
					const unsigned& u, \
					const handystats::attribute::time_point& timestamp = handystats::attribute::clock::now() \
				), \
				(std::move(attribute_name), u, timestamp) \
			) \
		/* SET_INT64 */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, ATTRIBUTE, SET_INT64, \
				( \
					std::string&& attribute_name, \
					const int64_t& i64, \
					const handystats::attribute::time_point& timestamp = handystats::attribute::clock::now() \
				), \
				(std::move(attribute_name), i64, timestamp) \
			) \
		/* SET_UINT64 */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, ATTRIBUTE, SET_UINT64, \
				( \
					std::string&& attribute_name, \
					const uint64_t& u64, \
					const handystats::attribute::time_point& timestamp = handystats::attribute::clock::now() \
				), \
				(std::move(attribute_name), u64, timestamp) \
			) \
		/* SET_DOUBLE */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, ATTRIBUTE, SET_DOUBLE, \
				( \
					std::string&& attribute_name, \
					const double& d, \
					const handystats::attribute::time_point& timestamp = handystats::attribute::clock::now() \
				), \
				(std::move(attribute_name), d, timestamp) \
			) \
		/* SET_STRING */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, ATTRIBUTE, SET_STRING, \
				( \
					std::string&& attribute_name, \
					const std::string& s, \
					const handystats::attribute::time_point& timestamp = handystats::attribute::clock::now() \
				), \
				(std::move(attribute_name), s, timestamp) \
			) \
	/* COUNTER */ \
		/* INIT */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, COUNTER, INIT, \
				( \
					std::string&& counter_name, \
					const handystats::metrics::counter::value_type& init_value = handystats::metrics::counter::value_type(), \
					const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now() \
				), \
				(std::move(counter_name), init_value, timestamp) \
			) \
		/* INCREMENT */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, COUNTER, INCREMENT, \
				( \
					std::string&& counter_name, \
					const handystats::metrics::counter::value_type& value = 1, \
					const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now() \
				), \
				(std::move(counter_name), value, timestamp) \
			) \
		/* DECREMENT */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, COUNTER, DECREMENT, \
				( \
					std::string&& counter_name, \
					const handystats::metrics::counter::value_type& value = 1, \
					const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now() \
				), \
				(std::move(counter_name), value, timestamp) \
			) \
		/* CHANGE */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, COUNTER, CHANGE, \
				( \
					std::string&& counter_name, \
					const handystats::metrics::counter::value_type& value, \
					const handystats::metrics::counter::time_point& timestamp = handystats::metrics::counter::clock::now() \
				), \
				(std::move(counter_name), value, timestamp) \
			) \
	/* TIMER */ \
		/* INIT */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, TIMER, INIT, \
				( \
					std::string&& timer_name, \
					const handystats::metrics::timer::instance_id_type& instance_id = handystats::metrics::timer::DEFAULT_INSTANCE_ID, \
					const handystats::metrics::timer::time_point& timestamp = handystats::metrics::timer::clock::now() \
				), \
				(std::move(timer_name), instance_id, timestamp) \
			) \
		/* START */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, TIMER, START, \
				( \
					std::string&& timer_name, \
					const handystats::metrics::timer::instance_id_type& instance_id = handystats::metrics::timer::DEFAULT_INSTANCE_ID, \
					const handystats::metrics::timer::time_point& timestamp = handystats::metrics::timer::clock::now() \
				), \
				(std::move(timer_name), instance_id, timestamp) \
			) \
		/* STOP */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, TIMER, STOP, \
				( \
					std::string&& timer_name, \
					const handystats::metrics::timer::instance_id_type& instance_id = handystats::metrics::timer::DEFAULT_INSTANCE_ID, \
					const handystats::metrics::timer::time_point& timestamp = handystats::metrics::timer::clock::now() \
				), \
				(std::move(timer_name), instance_id, timestamp) \
			) \
		/* DISCARD */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, TIMER, DISCARD, \
				( \
					std::string&& timer_name, \
					const handystats::metrics::timer::instance_id_type& instance_id = handystats::metrics::timer::DEFAULT_INSTANCE_ID, \
					const handystats::metrics::timer::time_point& timestamp = handystats::metrics::timer::clock::now() \
				), \
				(std::move(timer_name), instance_id, timestamp) \
			) \
		/* HEARTBEAT */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, TIMER, HEARTBEAT, \
				( \
					std::string&& timer_name, \
					const handystats::metrics::timer::instance_id_type& instance_id = handystats::metrics::timer::DEFAULT_INSTANCE_ID, \
					const handystats::metrics::timer::time_point& timestamp = handystats::metrics::timer::clock::now() \
				), \
				(std::move(timer_name), instance_id, timestamp) \
			) \
		/* SET */ \
		HANDY_MODULE_METRIC_OP(CXX, MODULE, TIMER, SET, \
				( \
					std::string&& timer_name, \
					const handystats::metrics::timer::value_type& measurement, \
					const handystats::metrics::timer::time_point& timestamp = handystats::metrics::timer::clock::now() \
				), \
				(std::move(timer_name), measurement, timestamp) \
			) \
/**/

#endif

#endif // HANDYSTATS_MODULE_H_
