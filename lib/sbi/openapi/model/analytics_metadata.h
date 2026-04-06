/*
 * analytics_metadata.h
 *
 * 
 */

#ifndef _OpenAPI_analytics_metadata_H_
#define _OpenAPI_analytics_metadata_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_analytics_metadata_NULL = 0, OpenAPI_analytics_metadata_NUM_OF_SAMPLES, OpenAPI_analytics_metadata_DATA_WINDOW, OpenAPI_analytics_metadata_DATA_STAT_PROPS, OpenAPI_analytics_metadata_STRATEGY, OpenAPI_analytics_metadata_ACCURACY, OpenAPI_analytics_metadata_DATA_SOURCES, OpenAPI_analytics_metadata_USED_PROC_INSTRUCT } OpenAPI_analytics_metadata_e;

char* OpenAPI_analytics_metadata_ToString(OpenAPI_analytics_metadata_e analytics_metadata);

OpenAPI_analytics_metadata_e OpenAPI_analytics_metadata_FromString(char* analytics_metadata);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_analytics_metadata_H_ */

