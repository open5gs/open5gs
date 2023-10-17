/*
 * partitioning_criteria.h
 *
 * 
 */

#ifndef _OpenAPI_partitioning_criteria_H_
#define _OpenAPI_partitioning_criteria_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_partitioning_criteria_NULL = 0, OpenAPI_partitioning_criteria_TAC, OpenAPI_partitioning_criteria_SUBPLMN, OpenAPI_partitioning_criteria_GEOAREA, OpenAPI_partitioning_criteria_SNSSAI, OpenAPI_partitioning_criteria_DNN } OpenAPI_partitioning_criteria_e;

char* OpenAPI_partitioning_criteria_ToString(OpenAPI_partitioning_criteria_e partitioning_criteria);

OpenAPI_partitioning_criteria_e OpenAPI_partitioning_criteria_FromString(char* partitioning_criteria);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_partitioning_criteria_H_ */

