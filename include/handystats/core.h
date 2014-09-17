// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CORE_H_
#define HANDYSTATS_CORE_H_

#include <handystats/common.h>

/*
 * {
 *     "core": {
 *         "enable": <boolean value>
 *     },
 *     "statistics": {
 *         "moving-interval": <value in msec>,
 *         "histogram-bins": <integer value>,
 *         "tags": ["<tag name>", "<tag name>", ...]
 *     },
 *     "metrics": {
 *         "gauge": {
 *             "values": {
 *                 <statistics opts>
 *             }
 *         },
 *         "counter": {
 *             "values": {
 *                 <statistics opts>
 *             },
 *             "incr-deltas": {
 *                 <statistics opts>
 *             },
 *             "decr-deltas": {
 *                 <statistics opts>
 *             },
 *             "deltas": {
 *                 <statistics opts>
 *             }
 *         },
 *         "timer": {
 *             "idle-timeout": <value in msec>,
 *             "values": {
 *                 <statistics opts>
 *             }
 *         }
 *     },
 *     "metrics-dump": {
 *         "interval": <value in msec>
 *     }
 * }
 */


HANDYSTATS_EXTERN_C
void handystats_initialize();

HANDYSTATS_EXTERN_C
void handystats_finalize();

HANDYSTATS_EXTERN_C
void handystats_config_file(const char* filename);

HANDYSTATS_EXTERN_C
void handystats_config_json(const char* config);


#ifndef __cplusplus
	#ifndef HANDYSTATS_DISABLE

		#define HANDY_INIT(...) handystats_initialize(__VA_ARGS__)

		#define HANDY_FINALIZE(...) handystats_finalize(__VA_ARGS__)

		#define HANDY_CONFIG_FILE(...) handystats_config_file(__VA_ARGS__)

		#define HANDY_CONFIG_JSON(...) handystats_config_json(__VA_ARGS__)

	#else

		#define HANDY_INIT(...)

		#define HANDY_FINALIZE(...)

		#define HANDY_CONFIG_FILE(...)

		#define HANDY_CONFIG_JSON(...)

	#endif
#endif

#endif // HANDYSTATS_CORE_H_
