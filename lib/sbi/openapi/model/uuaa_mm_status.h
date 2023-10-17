/*
 * uuaa_mm_status.h
 *
 * 
 */

#ifndef _OpenAPI_uuaa_mm_status_H_
#define _OpenAPI_uuaa_mm_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_uuaa_mm_status_NULL = 0, OpenAPI_uuaa_mm_status_SUCCESS, OpenAPI_uuaa_mm_status_PENDING, OpenAPI_uuaa_mm_status_FAILED } OpenAPI_uuaa_mm_status_e;

char* OpenAPI_uuaa_mm_status_ToString(OpenAPI_uuaa_mm_status_e uuaa_mm_status);

OpenAPI_uuaa_mm_status_e OpenAPI_uuaa_mm_status_FromString(char* uuaa_mm_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_uuaa_mm_status_H_ */

