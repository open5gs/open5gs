/*
 * media_type.h
 *
 * 
 */

#ifndef _OpenAPI_media_type_H_
#define _OpenAPI_media_type_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum { OpenAPI_media_type_NULL = 0, OpenAPI_media_type_AUDIO, OpenAPI_media_type_VIDEO, OpenAPI_media_type_DATA, OpenAPI_media_type_APPLICATION, OpenAPI_media_type_CONTROL, OpenAPI_media_type_TEXT, OpenAPI_media_type_MESSAGE, OpenAPI_media_type_OTHER } OpenAPI_media_type_e;

char* OpenAPI_media_type_ToString(OpenAPI_media_type_e media_type);

OpenAPI_media_type_e OpenAPI_media_type_FromString(char* media_type);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_type_H_ */

