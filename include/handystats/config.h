// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_H_
#define HANDYSTATS_CONFIG_H_

#include <handystats/common.h>

/*
 * {
 *     "core": {
 *         "enable": <boolean value>
 *     },
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
void handystats_config_file(const char* filename);

HANDYSTATS_EXTERN_C
void handystats_config_json(const char* config);


#ifndef __cplusplus
	#ifndef HANDYSTATS_DISABLE

		#define HANDY_CONFIG_FILE(...) handystats_config_file(__VA_ARGS__)

		#define HANDY_CONFIG_JSON(...) handystats_config_json(__VA_ARGS__)

	#else

		#define HANDY_CONFIG_FILE(...)

		#define HANDY_CONFIG_JSON(...)

	#endif
#endif

#endif // HANDYSTATS_CONFIG_H_
