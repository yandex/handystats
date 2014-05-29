#ifndef HANDYSTATS_CONFIGURATION_H_
#define HANDYSTATS_CONFIGURATION_H_

#include <handystats/rapidjson/document.h>

/*
 * {
 *    "handystats": {
 *        "gauge": {
 *            "moving-average-alpha": <double value>,
 *            "moving-interval": <value in msec>
 *        },
 *        "timer": {
 *           "idle-timeout": <value in msec>
 *        },
 *        "json-dump": {
 *            "interval": <value in msec>
 *        },
 *        "message-queue": {
 *           "sleep_on_empty": [<first sleep interval in usec>, <second sleep interval in usec>, ...]
 *        }
 *    }
 * }
 */

void HANDY_CONFIGURATION_FILE(const char* filename);
void HANDY_CONFIGURATION_JSON(const char* config);
void HANDY_CONFIGURATION_JSON(const rapidjson::Value& config);

#endif // HANDYSTATS_CONFIGURATION_H_
