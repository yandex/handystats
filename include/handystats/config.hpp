// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIG_HPP_
#define HANDYSTATS_CONFIG_HPP_

#include <handystats/rapidjson/document.h>

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

namespace handystats {

void config_file(const char* filename);
void config_json(const char* config);
void config_json(const rapidjson::Value& config);

} // namespace handystats


#ifndef HANDYSTATS_DISABLE

	#define HANDY_CONFIG_FILE(...) handystats::config_file(__VA_ARGS__)

	#define HANDY_CONFIG_JSON(...) handystats::config_json(__VA_ARGS__)

#else

	#define HANDY_CONFIG_FILE(...)

	#define HANDY_CONFIG_JSON(...)

#endif

#endif // HANDYSTATS_CONFIG_HPP_
