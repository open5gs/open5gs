/*
 * media_resource_type.h
 *
 * Indicates the Media Resource type
 */

#ifndef _OpenAPI_media_resource_type_H_
#define _OpenAPI_media_resource_type_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_media_resource_type_NULL = 0, OpenAPI_media_resource_type_DC, OpenAPI_media_resource_type_AUDIO, OpenAPI_media_resource_type_VIDEO } OpenAPI_media_resource_type_e;

char* OpenAPI_media_resource_type_ToString(OpenAPI_media_resource_type_e media_resource_type);

OpenAPI_media_resource_type_e OpenAPI_media_resource_type_FromString(char* media_resource_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_resource_type_H_ */

