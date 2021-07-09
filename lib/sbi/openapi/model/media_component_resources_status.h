/*
 * media_component_resources_status.h
 *
 * 
 */

#ifndef _OpenAPI_media_component_resources_status_H_
#define _OpenAPI_media_component_resources_status_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_media_component_resources_status_NULL = 0, OpenAPI_media_component_resources_status_ACTIVE, OpenAPI_media_component_resources_status_INACTIVE } OpenAPI_media_component_resources_status_e;

char* OpenAPI_media_component_resources_status_ToString(OpenAPI_media_component_resources_status_e media_component_resources_status);

OpenAPI_media_component_resources_status_e OpenAPI_media_component_resources_status_FromString(char* media_component_resources_status);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_component_resources_status_H_ */

