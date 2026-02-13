/*
 * occurrence_info_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_occurrence_info_any_of_H_
#define _OpenAPI_occurrence_info_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_occurrence_info_any_of_NULL = 0, OpenAPI_occurrence_info_any_of_ONE_TIME_EVENT, OpenAPI_occurrence_info_any_of_MULTIPLE_TIME_EVENT } OpenAPI_occurrence_info_any_of_e;

char* OpenAPI_occurrence_info_any_of_ToString(OpenAPI_occurrence_info_any_of_e occurrence_info_any_of);

OpenAPI_occurrence_info_any_of_e OpenAPI_occurrence_info_any_of_FromString(char* occurrence_info_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_occurrence_info_any_of_H_ */

