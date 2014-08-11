// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDY_GAUGE_MEASURING_POINTS_H_
#define HANDY_GAUGE_MEASURING_POINTS_H_

#include <handystats/handystats.h>

HANDYSTATS_EXTERN_C
void handystats_gauge_init(
		const char* gauge_name,
		const double init_value
	);

HANDYSTATS_EXTERN_C
void handystats_gauge_set(
		const char* gauge_name,
		const double value
	);


#ifndef __cplusplus
	#ifndef HANDYSTATS_DISABLE

		#define HANDY_GAUGE_INIT(...) handystats_gauge_init(__VA_ARGS__)

		#define HANDY_GAUGE_SET(...) handystats_gauge_set(__VA_ARGS__)

	#else

		#define HANDY_GAUGE_INIT(...)

		#define HANDY_GAUGE_SET(...)

	#endif

#endif

#endif // HANDYSTATS_GAUGE_MEASURING_POINTS_H_
