/*
* Copyright (c) YANDEX LLC. All rights reserved.
*
* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public
* License as published by the Free Software Foundation; either
* version 3.0 of the License, or (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this library.
*/

#ifndef HANDYSTATS_CORE_HPP_
#define HANDYSTATS_CORE_HPP_

#include <rapidjson/document.h>

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
