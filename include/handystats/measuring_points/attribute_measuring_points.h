// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_ATTRIBUTE_MEASURING_POINTS_H_
#define HANDYSTATS_ATTRIBUTE_MEASURING_POINTS_H_

#include <stdlib.h>

#include <handystats/handystats.h>

HANDYSTATS_EXTERN_C
void handystats_attribute_set_bool(
		const char* attribute_name,
		const bool b
	);

HANDYSTATS_EXTERN_C
void handystats_attribute_set_int(
		const char* attribute_name,
		const int i
	);

HANDYSTATS_EXTERN_C
void handystats_attribute_set_uint(
		const char* attribute_name,
		const unsigned u
	);

HANDYSTATS_EXTERN_C
void handystats_attribute_set_int64(
		const char* attribute_name,
		const int64_t i64
	);

HANDYSTATS_EXTERN_C
void handystats_attribute_set_uint64(
		const char* attribute_name,
		const uint64_t u64
	);

HANDYSTATS_EXTERN_C
void handystats_attribute_set_double(
		const char* attribute_name,
		const double d
	);

HANDYSTATS_EXTERN_C
void handystats_attribute_set_string(
		const char* attribute_name,
		const char* s
	);


#ifndef __cplusplus
	#ifndef HANDYSTATS_DISABLE

		#define HANDY_ATTRIBUTE_SET_BOOL(...) handystats_attribute_set_bool(__VA_ARGS__)

		#define HANDY_ATTRIBUTE_SET_INT(...) handystats_attribute_set_int(__VA_ARGS__)

		#define HANDY_ATTRIBUTE_SET_UINT(...) handystats_attribute_set_uint(__VA_ARGS__)

		#define HANDY_ATTRIBUTE_SET_INT64(...) handystats_attribute_set_int64(__VA_ARGS__)

		#define HANDY_ATTRIBUTE_SET_UINT64(...) handystats_attribute_set_uint64(__VA_ARGS__)

		#define HANDY_ATTRIBUTE_SET_DOUBLE(...) handystats_attribute_set_double(__VA_ARGS__)

		#define HANDY_ATTRIBUTE_SET_STRING(...) handystats_attribute_set_string(__VA_ARGS__)

	#else

		#define HANDY_ATTRIBUTE_SET_BOOL(...)

		#define HANDY_ATTRIBUTE_SET_INT(...)

		#define HANDY_ATTRIBUTE_SET_UINT(...)

		#define HANDY_ATTRIBUTE_SET_INT64(...)

		#define HANDY_ATTRIBUTE_SET_UINT64(...)

		#define HANDY_ATTRIBUTE_SET_DOUBLE(...)

		#define HANDY_ATTRIBUTE_SET_STRING(...)

	#endif

#endif

#endif // HANDYSTATS_ATTRIBUTE_MEASURING_POINTS_H_
