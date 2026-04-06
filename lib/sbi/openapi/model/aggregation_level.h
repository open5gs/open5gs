/*
 * aggregation_level.h
 *
 * 
 */

#ifndef _OpenAPI_aggregation_level_H_
#define _OpenAPI_aggregation_level_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_aggregation_level_NULL = 0, OpenAPI_aggregation_level_UE, OpenAPI_aggregation_level_AOI } OpenAPI_aggregation_level_e;

char* OpenAPI_aggregation_level_ToString(OpenAPI_aggregation_level_e aggregation_level);

OpenAPI_aggregation_level_e OpenAPI_aggregation_level_FromString(char* aggregation_level);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_aggregation_level_H_ */

