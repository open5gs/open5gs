/*
 * avatar_media.h
 *
 * 
 */

#ifndef _OpenAPI_avatar_media_H_
#define _OpenAPI_avatar_media_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_avatar_media_s OpenAPI_avatar_media_t;

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_avatar_media_s {
    char *resource_url;
    char *media_processing_specification;
};

OpenAPI_avatar_media_t *OpenAPI_avatar_media_create(
    char *resource_url,
    char *media_processing_specification
);
void OpenAPI_avatar_media_free(OpenAPI_avatar_media_t *avatar_media);
OpenAPI_avatar_media_t *OpenAPI_avatar_media_parseFromJSON(cJSON *avatar_mediaJSON);
cJSON *OpenAPI_avatar_media_convertToJSON(OpenAPI_avatar_media_t *avatar_media);
OpenAPI_avatar_media_t *OpenAPI_avatar_media_copy(OpenAPI_avatar_media_t *dst, OpenAPI_avatar_media_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_avatar_media_H_ */

