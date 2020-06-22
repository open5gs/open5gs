/*
 * unavailable_access_indication.h
 *
 *
 */

#ifndef _OpenAPI_unavailable_access_indication_H_
#define _OpenAPI_unavailable_access_indication_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_unavailable_access_indication_NULL = 0, OpenAPI_unavailable_access_indication__3GA_UNAVAILABLE, OpenAPI_unavailable_access_indication_N3GA_UNAVAILABLE } OpenAPI_unavailable_access_indication_e;

char* OpenAPI_unavailable_access_indication_ToString(OpenAPI_unavailable_access_indication_e unavailable_access_indication);

OpenAPI_unavailable_access_indication_e OpenAPI_unavailable_access_indication_FromString(char* unavailable_access_indication);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_unavailable_access_indication_H_ */

