/*
 * access_right_status.h
 *
 * 
 */

#ifndef _OpenAPI_access_right_status_H_
#define _OpenAPI_access_right_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_access_right_status_NULL = 0, OpenAPI_access_right_status_FULLY_ALLOWED, OpenAPI_access_right_status_PREVIEW_ALLOWED, OpenAPI_access_right_status_NO_ALLOWED } OpenAPI_access_right_status_e;

char* OpenAPI_access_right_status_ToString(OpenAPI_access_right_status_e access_right_status);

OpenAPI_access_right_status_e OpenAPI_access_right_status_FromString(char* access_right_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_access_right_status_H_ */

