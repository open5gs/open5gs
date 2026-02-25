/*
 * lcs_priority_any_of.h
 *
 * 
 */

#ifndef _OpenAPI_lcs_priority_any_of_H_
#define _OpenAPI_lcs_priority_any_of_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_lcs_priority_any_of_NULL = 0, OpenAPI_lcs_priority_any_of_HIGHEST_PRIORITY, OpenAPI_lcs_priority_any_of_NORMAL_PRIORITY } OpenAPI_lcs_priority_any_of_e;

char* OpenAPI_lcs_priority_any_of_ToString(OpenAPI_lcs_priority_any_of_e lcs_priority_any_of);

OpenAPI_lcs_priority_any_of_e OpenAPI_lcs_priority_any_of_FromString(char* lcs_priority_any_of);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_lcs_priority_any_of_H_ */

