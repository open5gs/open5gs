/*
 * positioning_case.h
 *
 * 
 */

#ifndef _OpenAPI_positioning_case_H_
#define _OpenAPI_positioning_case_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_positioning_case_NULL = 0, OpenAPI_positioning_case_NG_RAN_ASSISTED_POSITIONING } OpenAPI_positioning_case_e;

char* OpenAPI_positioning_case_ToString(OpenAPI_positioning_case_e positioning_case);

OpenAPI_positioning_case_e OpenAPI_positioning_case_FromString(char* positioning_case);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_positioning_case_H_ */

