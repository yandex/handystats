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

#ifndef HANDYSTATS_MACROS_H_
#define HANDYSTATS_MACROS_H_

#include <stdio.h>

#include <boost/preprocessor/cat.hpp>
#include <boost/preprocessor/arithmetic/dec.hpp>
#include <boost/preprocessor/control/if.hpp>
#include <boost/preprocessor/tuple.hpp>
#include <boost/preprocessor/seq.hpp>
#include <boost/preprocessor/facilities/expand.hpp>
#include <boost/preprocessor/facilities/empty.hpp>
#include <boost/preprocessor/comma_if.hpp>

/*
 * HANDY_PP_VARIADIC_SIZE(...)
 */
#define HANDY_PP_VARIADIC_SIZE_I(e0, e1, e2, e3, e4, e5, e6, e7, e8, e9, e10, e11, e12, e13, e14, e15, e16, e17, e18, e19, e20, e21, e22, e23, e24, e25, e26, e27, e28, e29, e30, e31, e32, e33, e34, e35, e36, e37, e38, e39, e40, e41, e42, e43, e44, e45, e46, e47, e48, e49, e50, e51, e52, e53, e54, e55, e56, e57, e58, e59, e60, e61, e62, e63, size, ...) size

#define HANDY_PP_VARIADIC_SIZE(...) HANDY_PP_VARIADIC_SIZE_I(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1,)

/*
 * HANDY_PP_OVERLOAD(prefix, ...)
 */
#define HANDY_PP_OVERLOAD(prefix, ...) BOOST_PP_CAT(prefix, HANDY_PP_VARIADIC_SIZE(__VA_ARGS__))

/*
 * HANDY_PP_IS_TUPLE(x)
 */
#define HANDY_PP_DUMMY_CALL_RET_2(...) 0, 0

#define HANDY_PP_IS_TUPLE(x) \
	BOOST_PP_DEC( \
		HANDY_PP_VARIADIC_SIZE( \
			HANDY_PP_DUMMY_CALL_RET_2 x \
		) \
	)

/*
 * HANDY_PP_TUPLE_TO_SEQ(x)
 */
#define HANDY_PP_TUPLE_TO_SEQ(...) HANDY_PP_OVERLOAD(HANDY_PP_TUPLE_TO_SEQ_O_, __VA_ARGS__)(__VA_ARGS__)
#define HANDY_PP_TUPLE_TO_SEQ_O_1(tuple) BOOST_PP_CAT(BOOST_PP_TUPLE_TO_SEQ_, HANDY_PP_VARIADIC_SIZE tuple) tuple
#define HANDY_PP_TUPLE_TO_SEQ_O_2(size, tuple) HANDY_PP_TUPLE_TO_SEQ_O_1(tuple)

/*
 * HANDY_PP_TUPLE_FIRST_ELEM(x)
 */
#define HANDY_PP_TUPLE_FIRST_ELEM(x) \
	BOOST_PP_TUPLE_ELEM( \
		BOOST_PP_SEQ_SIZE(HANDY_PP_TUPLE_TO_SEQ(x)), \
		0, \
		x \
	)

/*
 * HANDY_PP_TUPLE_POP_FRONT(x)
 */
#define HANDY_PP_TUPLE_POP_FRONT(x) \
	BOOST_PP_IF( \
		BOOST_PP_DEC(HANDY_PP_VARIADIC_SIZE x), \
		BOOST_PP_SEQ_TO_TUPLE(BOOST_PP_SEQ_POP_FRONT(HANDY_PP_TUPLE_TO_SEQ(x))), \
		(0) \
	)

/*
 * HANDY_PP_TUPLE_REM(x)
 */
#define HANDY_PP_REM(...) __VA_ARGS__
#define HANDY_PP_TUPLE_REM(size) HANDY_PP_REM

/*
 * HANDY_PP_TUPLE_EAT(x)
 */
#define HANDY_PP_EAT(...)
#define HANDY_PP_TUPLE_EAT(size) HANDY_PP_EAT

/*
 * HANDY_PP_METRIC_NAME_PRINT_ARGS(...)
 * HANDY_PP_METRIC_NAME_BUFFER_VAR
 * HANDY_PP_METRIC_NAME_BUFFER_SET(...)
 */
#define HANDY_PP_METRIC_NAME_PRINT_ARGS(...) HANDY_PP_TUPLE_REM() HANDY_PP_TUPLE_FIRST_ELEM((__VA_ARGS__))
#define HANDY_PP_METRIC_NAME_BUFFER_VAR BOOST_PP_CAT(HANDY_METRIC_NAME_BUFFER_VAR_, __LINE__)
#define HANDY_PP_METRIC_NAME_BUFFER_SET(...) \
	char HANDY_PP_METRIC_NAME_BUFFER_VAR[256]; \
	snprintf(HANDY_PP_METRIC_NAME_BUFFER_VAR, 255, HANDY_PP_METRIC_NAME_PRINT_ARGS(__VA_ARGS__)); \

/*
 * HANDY_PP_MEASURING_POINT_WRAPPER
 */
#define HANDY_PP_MEASURING_POINT_WRAPPER(measuring_point_func, ...) \
	BOOST_PP_EXPAND ( HANDY_PP_TUPLE_REM() \
		BOOST_PP_IF( \
			HANDY_PP_IS_TUPLE(HANDY_PP_TUPLE_FIRST_ELEM((__VA_ARGS__))), \
			/* if printf-like format */ \
			( \
				HANDY_PP_METRIC_NAME_BUFFER_SET(__VA_ARGS__); \
				measuring_point_func( \
					HANDY_PP_METRIC_NAME_BUFFER_VAR \
					BOOST_PP_COMMA_IF(BOOST_PP_DEC(HANDY_PP_VARIADIC_SIZE(__VA_ARGS__))) \
					BOOST_PP_IF( \
						BOOST_PP_DEC(HANDY_PP_VARIADIC_SIZE(__VA_ARGS__)), \
						HANDY_PP_TUPLE_REM(), \
						HANDY_PP_TUPLE_EAT() \
					) HANDY_PP_TUPLE_POP_FRONT((__VA_ARGS__)) \
				) \
			), \
			/* else pass args as is */ \
			( \
				measuring_point_func(__VA_ARGS__) \
			) \
		) \
	)

#endif // HANDYSTATS_MACROS_H_
