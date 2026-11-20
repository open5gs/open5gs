
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "media_resource.h"

OpenAPI_media_resource_t *OpenAPI_media_resource_create(
    char *media_id,
    OpenAPI_media_resource_type_e media_resource_capability,
    OpenAPI_avatar_media_t *avatar_media
)
{
    OpenAPI_media_resource_t *media_resource_local_var = ogs_malloc(sizeof(OpenAPI_media_resource_t));
    ogs_assert(media_resource_local_var);

    media_resource_local_var->media_id = media_id;
    media_resource_local_var->media_resource_capability = media_resource_capability;
    media_resource_local_var->avatar_media = avatar_media;

    return media_resource_local_var;
}

void OpenAPI_media_resource_free(OpenAPI_media_resource_t *media_resource)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == media_resource) {
        return;
    }
    if (media_resource->media_id) {
        ogs_free(media_resource->media_id);
        media_resource->media_id = NULL;
    }
    if (media_resource->avatar_media) {
        OpenAPI_avatar_media_free(media_resource->avatar_media);
        media_resource->avatar_media = NULL;
    }
    ogs_free(media_resource);
}

cJSON *OpenAPI_media_resource_convertToJSON(OpenAPI_media_resource_t *media_resource)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (media_resource == NULL) {
        ogs_error("OpenAPI_media_resource_convertToJSON() failed [MediaResource]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!media_resource->media_id) {
        ogs_error("OpenAPI_media_resource_convertToJSON() failed [media_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "mediaID", media_resource->media_id) == NULL) {
        ogs_error("OpenAPI_media_resource_convertToJSON() failed [media_id]");
        goto end;
    }

    if (media_resource->media_resource_capability == OpenAPI_media_resource_type_NULL) {
        ogs_error("OpenAPI_media_resource_convertToJSON() failed [media_resource_capability]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "mediaResourceCapability", OpenAPI_media_resource_type_ToString(media_resource->media_resource_capability)) == NULL) {
        ogs_error("OpenAPI_media_resource_convertToJSON() failed [media_resource_capability]");
        goto end;
    }

    if (media_resource->avatar_media) {
    cJSON *avatar_media_local_JSON = OpenAPI_avatar_media_convertToJSON(media_resource->avatar_media);
    if (avatar_media_local_JSON == NULL) {
        ogs_error("OpenAPI_media_resource_convertToJSON() failed [avatar_media]");
        goto end;
    }
    cJSON_AddItemToObject(item, "avatarMedia", avatar_media_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_media_resource_convertToJSON() failed [avatar_media]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_media_resource_t *OpenAPI_media_resource_parseFromJSON(cJSON *media_resourceJSON)
{
    OpenAPI_media_resource_t *media_resource_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *media_id = NULL;
    cJSON *media_resource_capability = NULL;
    OpenAPI_media_resource_type_e media_resource_capabilityVariable = 0;
    cJSON *avatar_media = NULL;
    OpenAPI_avatar_media_t *avatar_media_local_nonprim = NULL;
    media_id = cJSON_GetObjectItemCaseSensitive(media_resourceJSON, "mediaID");
    if (!media_id) {
        ogs_error("OpenAPI_media_resource_parseFromJSON() failed [media_id]");
        goto end;
    }
    if (!cJSON_IsString(media_id)) {
        ogs_error("OpenAPI_media_resource_parseFromJSON() failed [media_id]");
        goto end;
    }

    media_resource_capability = cJSON_GetObjectItemCaseSensitive(media_resourceJSON, "mediaResourceCapability");
    if (!media_resource_capability) {
        ogs_error("OpenAPI_media_resource_parseFromJSON() failed [media_resource_capability]");
        goto end;
    }
    if (!cJSON_IsString(media_resource_capability)) {
        ogs_error("OpenAPI_media_resource_parseFromJSON() failed [media_resource_capability]");
        goto end;
    }
    media_resource_capabilityVariable = OpenAPI_media_resource_type_FromString(media_resource_capability->valuestring);

    avatar_media = cJSON_GetObjectItemCaseSensitive(media_resourceJSON, "avatarMedia");
    if (avatar_media) {
    avatar_media_local_nonprim = OpenAPI_avatar_media_parseFromJSON(avatar_media);
    if (!avatar_media_local_nonprim) {
        ogs_error("OpenAPI_avatar_media_parseFromJSON failed [avatar_media]");
        goto end;
    }
    }

    media_resource_local_var = OpenAPI_media_resource_create (
        ogs_strdup(media_id->valuestring),
        media_resource_capabilityVariable,
        avatar_media ? avatar_media_local_nonprim : NULL
    );

    return media_resource_local_var;
end:
    if (avatar_media_local_nonprim) {
        OpenAPI_avatar_media_free(avatar_media_local_nonprim);
        avatar_media_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_media_resource_t *OpenAPI_media_resource_copy(OpenAPI_media_resource_t *dst, OpenAPI_media_resource_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_media_resource_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_media_resource_convertToJSON() failed");
        return NULL;
    }

    content = cJSON_Print(item);
    cJSON_Delete(item);

    if (!content) {
        ogs_error("cJSON_Print() failed");
        return NULL;
    }

    item = cJSON_Parse(content);
    ogs_free(content);
    if (!item) {
        ogs_error("cJSON_Parse() failed");
        return NULL;
    }

    OpenAPI_media_resource_free(dst);
    dst = OpenAPI_media_resource_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

