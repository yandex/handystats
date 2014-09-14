// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CORE_HPP_
#define HANDYSTATS_CORE_HPP_

#include <handystats/rapidjson/document.h>

/*
 * {
 *     "core": {
 *         "enable": <boolean value>
 *     },
 *     "statistics": {
 *         "moving-interval": <value in msec>,
 *         "histogram-bins": <integer value>,
 *         "tags": "<tag name>" | ["<tag name>", "<tag name>", ...]
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
 *         "interval": <value in msec>,
 *         "to-json": <boolean value>
 *     }
 * }
 */


namespace handystats {

void initialize();

void finalize();

void config_file(const char* filename);
void config_json(const char* config);
void config_json(const rapidjson::Value& config);

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
