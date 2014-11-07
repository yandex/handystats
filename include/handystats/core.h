// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CORE_H_
#define HANDYSTATS_CORE_H_

#include <handystats/common.h>

/*
 * {
 *     "enable": <boolean value>,
 *     "dump-interval": <value in msec>,
 *     "defaults": {
 *         "moving-interval": <value in msec>,
 *         "histogram-bins": <integer value>,
 *         "stats": ["<statistic name>", "<statistic name>", ...]
 *     },
 *     "gauge": {
 *         <statistics opts>
 *     },
 *     "counter": {
 *         <statistics opts>
 *     },
 *     "timer": {
 *         "idle-timeout": <value in msec>,
 *         <statistics opts>
 *     },
 *     "<pattern>": {
 *         <statistics opts>
 *     }
 * }
 */

HANDYSTATS_EXTERN_C
void handystats_initialize();

HANDYSTATS_EXTERN_C
void handystats_finalize();

HANDYSTATS_EXTERN_C
int handystats_config_file(const char* filename);

HANDYSTATS_EXTERN_C
int handystats_config_json(const char* config);

HANDYSTATS_EXTERN_C
const char* handystats_config_error();

#ifndef __cplusplus
	#ifndef HANDYSTATS_DISABLE

		#define HANDY_INIT(...) handystats_initialize(__VA_ARGS__)

		#define HANDY_FINALIZE(...) handystats_finalize(__VA_ARGS__)

		#define HANDY_CONFIG_FILE(...) handystats_config_file(__VA_ARGS__)

		#define HANDY_CONFIG_JSON(...) handystats_config_json(__VA_ARGS__)

		#define HANDY_CONFIG_ERROR(...) handystats_config_error(__VA_ARGS__)

	#else

		#define HANDY_INIT(...)

		#define HANDY_FINALIZE(...)

		#define HANDY_CONFIG_FILE(...) 0

		#define HANDY_CONFIG_JSON(...) 0

		#define HANDY_CONFIG_ERROR(...) "Handystats is not supported. Recompile with appropriate falgs."

	#endif
#endif

#endif // HANDYSTATS_CORE_H_
