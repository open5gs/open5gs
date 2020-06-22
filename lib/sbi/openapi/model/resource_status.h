/*
 * resource_status.h
 *
 *
 */

#ifndef _OpenAPI_resource_status_H_
#define _OpenAPI_resource_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_resource_status_NULL = 0, OpenAPI_resource_status_RELEASED, OpenAPI_resource_status_UNCHANGED, OpenAPI_resource_status_TRANSFERRED, OpenAPI_resource_status_UPDATED } OpenAPI_resource_status_e;

char* OpenAPI_resource_status_ToString(OpenAPI_resource_status_e resource_status);

OpenAPI_resource_status_e OpenAPI_resource_status_FromString(char* resource_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_resource_status_H_ */

