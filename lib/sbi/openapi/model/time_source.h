/*
 * time_source.h
 *
 * Indicates the Time Source.
 */

#ifndef _OpenAPI_time_source_H_
#define _OpenAPI_time_source_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_time_source_NULL = 0, OpenAPI_time_source_SYNC_E, OpenAPI_time_source_PTP, OpenAPI_time_source_GNSS, OpenAPI_time_source_ATOMIC_CLOCK, OpenAPI_time_source_TERRESTRIAL_RADIO, OpenAPI_time_source_SERIAL_TIME_CODE, OpenAPI_time_source_NTP, OpenAPI_time_source_HAND_SET, OpenAPI_time_source_OTHER } OpenAPI_time_source_e;

char* OpenAPI_time_source_ToString(OpenAPI_time_source_e time_source);

OpenAPI_time_source_e OpenAPI_time_source_FromString(char* time_source);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_time_source_H_ */

