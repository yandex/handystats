// Copyright (c) 2014 Yandex LLC. All rights reserved.

#ifndef HANDYSTATS_CONFIGURATION_HPP_
#define HANDYSTATS_CONFIGURATION_HPP_

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

void configuration_file(const char* filename);
void configuration_json(const char* config);
void configuration_json(const rapidjson::Value& config);

} // namespace handystats


#ifndef HANDYSTATS_DISABLE

	#define HANDY_CONFIGURATION_FILE(...) handystats::configuration_file(__VA_ARGS__)

	#define HANDY_CONFIGURATION_JSON(...) handystats::configuration_json(__VA_ARGS__)

#else

	#define HANDY_CONFIGURATION_FILE(...)

	#define HANDY_CONFIGURATION_JSON(...)

#endif

#endif // HANDYSTATS_CONFIGURATION_HPP_
