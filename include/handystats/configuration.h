// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIGURATION_H_
#define HANDYSTATS_CONFIGURATION_H_

#include <handystats/common.h>

/*
 * {
 *     "incremental-statistics": {
 *         "moving-average-alpha": <double value>,
 *         "moving-interval": <value in msec>
 *     },
 *     "timer": {
 *        "idle-timeout": <value in msec>
 *     },
 *     "metrics-dump": {
 *         "interval": <value in msec>,
 *         "to-json": <boolean value>
 *     }
 * }
 */

HANDYSTATS_EXTERN_C
void handystats_configuration_file(const char* filename);

HANDYSTATS_EXTERN_C
void handystats_configuration_json(const char* config);


#ifndef __cplusplus
	#ifndef HANDYSTATS_DISABLE

		#define HANDY_CONFIGURATION_FILE(...) handystats_configuration_file(__VA_ARGS__)

		#define HANDY_CONFIGURATION_JSON(...) handystats_configuration_json(__VA_ARGS__)

	#else

		#define HANDY_CONFIGURATION_FILE(...)

		#define HANDY_CONFIGURATION_JSON(...)

	#endif
#endif

#endif // HANDYSTATS_CONFIGURATION_H_
