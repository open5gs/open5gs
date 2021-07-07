/*
 * status_change.h
 *
 * 
 */

#ifndef _OpenAPI_status_change_H_
#define _OpenAPI_status_change_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_status_change_NULL = 0, OpenAPI_status_change_AMF_UNAVAILABLE, OpenAPI_status_change_AMF_AVAILABLE } OpenAPI_status_change_e;

char* OpenAPI_status_change_ToString(OpenAPI_status_change_e status_change);

OpenAPI_status_change_e OpenAPI_status_change_FromString(char* status_change);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_status_change_H_ */

