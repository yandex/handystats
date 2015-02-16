// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CORE_HPP_
#define HANDYSTATS_CORE_HPP_

#include <handystats/rapidjson/document.h>

/*
 * Old configuration format
 * {
 *     "core": {
 *         "enable": <boolean value>
 *     },
 *     "statistics": {
 *         "moving-interval": <value in msec>,
 *         "histogram-bins": <integer value>,
 *         "tags": ["<tag name>", "<tag name>", ...],
 *         "rate-unit": <"ns" | "us" | "ms" | "s" | "m" | "h">
 *     },
 *     "metrics": {
 *         "gauge": {
 *             <statistics opts>
 *         },
 *         "counter": {
 *             <statistics opts>
 *         },
 *         "timer": {
 *             "idle-timeout": <value in msec>,
 *             <statistics opts>
 *         }
 *     },
 *     "metrics-dump": {
 *         "interval": <value in msec>
 *     },
 *     "<pattern>": {
 *         <statistics opts>
 *     }
 * }
 */

/*
 * New configuration format
 * {
 *     "enable": <boolean value>,
 *     "dump-interval": <value in msec>,
 *     "defaults": {
 *         "moving-interval": <value in msec>,
 *         "histogram-bins": <integer value>,
 *         "tags": ["<tag name>", "<tag name>", ...],
 *         "rate-unit": <"ns" | "us" | "ms" | "s" | "m" | "h">
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

namespace handystats {

void initialize();

void finalize();

bool config_file(const char* filename);
bool config_json(const char* config);
bool config_json(const rapidjson::Value& config);

} // namespace handystats


#ifndef HANDYSTATS_DISABLE

	#define HANDY_INIT(...) handystats::initialize(__VA_ARGS__)

	#define HANDY_FINALIZE(...) handystats::finalize(__VA_ARGS__)

	#define HANDY_CONFIG_FILE(...) handystats::config_file(__VA_ARGS__)

	#define HANDY_CONFIG_JSON(...) handystats::config_json(__VA_ARGS__)

#else

	#define HANDY_INIT(...)

	#define HANDY_FINALIZE(...)

	#define HANDY_CONFIG_FILE(...)

	#define HANDY_CONFIG_JSON(...)

#endif

#endif // HANDYSTATS_CORE_HPP_
