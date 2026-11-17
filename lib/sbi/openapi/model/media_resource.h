/*
 * media_resource.h
 *
 * 
 */

#ifndef _OpenAPI_media_resource_H_
#define _OpenAPI_media_resource_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_media_resource_s OpenAPI_media_resource_t;
#include "avatar_media.h"
#include "media_resource_type.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_media_resource_s {
    char *media_id;
    OpenAPI_media_resource_type_e media_resource_capability;
    struct OpenAPI_avatar_media_s *avatar_media;
};

OpenAPI_media_resource_t *OpenAPI_media_resource_create(
    char *media_id,
    OpenAPI_media_resource_type_e media_resource_capability,
    OpenAPI_avatar_media_t *avatar_media
);
void OpenAPI_media_resource_free(OpenAPI_media_resource_t *media_resource);
OpenAPI_media_resource_t *OpenAPI_media_resource_parseFromJSON(cJSON *media_resourceJSON);
cJSON *OpenAPI_media_resource_convertToJSON(OpenAPI_media_resource_t *media_resource);
OpenAPI_media_resource_t *OpenAPI_media_resource_copy(OpenAPI_media_resource_t *dst, OpenAPI_media_resource_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_media_resource_H_ */

