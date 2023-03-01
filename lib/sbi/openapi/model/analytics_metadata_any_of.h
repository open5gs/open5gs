/*
 * analytics_metadata_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_analytics_metadata_any_of_H_
#define _OpenAPI_analytics_metadata_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_analytics_metadata_any_of_NULL = 0, OpenAPI_analytics_metadata_any_of_NUM_OF_SAMPLES, OpenAPI_analytics_metadata_any_of_DATA_WINDOW, OpenAPI_analytics_metadata_any_of_DATA_STAT_PROPS, OpenAPI_analytics_metadata_any_of_STRATEGY, OpenAPI_analytics_metadata_any_of_ACCURACY } OpenAPI_analytics_metadata_any_of_e;

char* OpenAPI_analytics_metadata_any_of_ToString(OpenAPI_analytics_metadata_any_of_e analytics_metadata_any_of);

OpenAPI_analytics_metadata_any_of_e OpenAPI_analytics_metadata_any_of_FromString(char* analytics_metadata_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_analytics_metadata_any_of_H_ */

