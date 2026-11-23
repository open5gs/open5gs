
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "avatar_media.h"

OpenAPI_avatar_media_t *OpenAPI_avatar_media_create(
    char *resource_url,
    char *media_processing_specification
)
{
    OpenAPI_avatar_media_t *avatar_media_local_var = ogs_malloc(sizeof(OpenAPI_avatar_media_t));
    ogs_assert(avatar_media_local_var);

    avatar_media_local_var->resource_url = resource_url;
    avatar_media_local_var->media_processing_specification = media_processing_specification;

    return avatar_media_local_var;
}

void OpenAPI_avatar_media_free(OpenAPI_avatar_media_t *avatar_media)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == avatar_media) {
        return;
    }
    if (avatar_media->resource_url) {
        ogs_free(avatar_media->resource_url);
        avatar_media->resource_url = NULL;
    }
    if (avatar_media->media_processing_specification) {
        ogs_free(avatar_media->media_processing_specification);
        avatar_media->media_processing_specification = NULL;
    }
    ogs_free(avatar_media);
}

cJSON *OpenAPI_avatar_media_convertToJSON(OpenAPI_avatar_media_t *avatar_media)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (avatar_media == NULL) {
        ogs_error("OpenAPI_avatar_media_convertToJSON() failed [AvatarMedia]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (avatar_media->resource_url) {
    if (cJSON_AddStringToObject(item, "resourceURL", avatar_media->resource_url) == NULL) {
        ogs_error("OpenAPI_avatar_media_convertToJSON() failed [resource_url]");
        goto end;
    }
    }

    if (avatar_media->media_processing_specification) {
    if (cJSON_AddStringToObject(item, "mediaProcessingSpecification", avatar_media->media_processing_specification) == NULL) {
        ogs_error("OpenAPI_avatar_media_convertToJSON() failed [media_processing_specification]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_avatar_media_t *OpenAPI_avatar_media_parseFromJSON(cJSON *avatar_mediaJSON)
{
    OpenAPI_avatar_media_t *avatar_media_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *resource_url = NULL;
    cJSON *media_processing_specification = NULL;
    resource_url = cJSON_GetObjectItemCaseSensitive(avatar_mediaJSON, "resourceURL");
    if (resource_url) {
    if (!cJSON_IsString(resource_url) && !cJSON_IsNull(resource_url)) {
        ogs_error("OpenAPI_avatar_media_parseFromJSON() failed [resource_url]");
        goto end;
    }
    }

    media_processing_specification = cJSON_GetObjectItemCaseSensitive(avatar_mediaJSON, "mediaProcessingSpecification");
    if (media_processing_specification) {
    if (!cJSON_IsString(media_processing_specification) && !cJSON_IsNull(media_processing_specification)) {
        ogs_error("OpenAPI_avatar_media_parseFromJSON() failed [media_processing_specification]");
        goto end;
    }
    }

    avatar_media_local_var = OpenAPI_avatar_media_create (
        resource_url && !cJSON_IsNull(resource_url) ? ogs_strdup(resource_url->valuestring) : NULL,
        media_processing_specification && !cJSON_IsNull(media_processing_specification) ? ogs_strdup(media_processing_specification->valuestring) : NULL
    );

    return avatar_media_local_var;
end:
    return NULL;
}

OpenAPI_avatar_media_t *OpenAPI_avatar_media_copy(OpenAPI_avatar_media_t *dst, OpenAPI_avatar_media_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_avatar_media_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_avatar_media_convertToJSON() failed");
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

    OpenAPI_avatar_media_free(dst);
    dst = OpenAPI_avatar_media_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

