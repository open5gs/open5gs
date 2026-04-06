/*
 * clock_quality_detail_level.h
 *
 * Indicates the Clock Quality Detail Level.
 */

#ifndef _OpenAPI_clock_quality_detail_level_H_
#define _OpenAPI_clock_quality_detail_level_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_clock_quality_detail_level_NULL = 0, OpenAPI_clock_quality_detail_level_CLOCK_QUALITY_METRICS, OpenAPI_clock_quality_detail_level_ACCEPT_INDICATION } OpenAPI_clock_quality_detail_level_e;

char* OpenAPI_clock_quality_detail_level_ToString(OpenAPI_clock_quality_detail_level_e clock_quality_detail_level);

OpenAPI_clock_quality_detail_level_e OpenAPI_clock_quality_detail_level_FromString(char* clock_quality_detail_level);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_clock_quality_detail_level_H_ */

