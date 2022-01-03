
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sdm_subs_modification.h"

OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_create(
    char *expires,
    OpenAPI_list_t *monitored_resource_uris
)
{
    OpenAPI_sdm_subs_modification_t *sdm_subs_modification_local_var = ogs_malloc(sizeof(OpenAPI_sdm_subs_modification_t));
    ogs_assert(sdm_subs_modification_local_var);

    sdm_subs_modification_local_var->expires = expires;
    sdm_subs_modification_local_var->monitored_resource_uris = monitored_resource_uris;

    return sdm_subs_modification_local_var;
}

void OpenAPI_sdm_subs_modification_free(OpenAPI_sdm_subs_modification_t *sdm_subs_modification)
{
    if (NULL == sdm_subs_modification) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sdm_subs_modification->expires);
    OpenAPI_list_for_each(sdm_subs_modification->monitored_resource_uris, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(sdm_subs_modification->monitored_resource_uris);
    ogs_free(sdm_subs_modification);
}

cJSON *OpenAPI_sdm_subs_modification_convertToJSON(OpenAPI_sdm_subs_modification_t *sdm_subs_modification)
{
    cJSON *item = NULL;

    if (sdm_subs_modification == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [SdmSubsModification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sdm_subs_modification->expires) {
    if (cJSON_AddStringToObject(item, "expires", sdm_subs_modification->expires) == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [expires]");
        goto end;
    }
    }

    if (sdm_subs_modification->monitored_resource_uris) {
    cJSON *monitored_resource_uris = cJSON_AddArrayToObject(item, "monitoredResourceUris");
    if (monitored_resource_uris == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }

    OpenAPI_lnode_t *monitored_resource_uris_node;
    OpenAPI_list_for_each(sdm_subs_modification->monitored_resource_uris, monitored_resource_uris_node)  {
    if (cJSON_AddStringToObject(monitored_resource_uris, "", (char*)monitored_resource_uris_node->data) == NULL) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed [monitored_resource_uris]");
        goto end;
    }
                    }
    }

end:
    return item;
}

OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_parseFromJSON(cJSON *sdm_subs_modificationJSON)
{
    OpenAPI_sdm_subs_modification_t *sdm_subs_modification_local_var = NULL;
    cJSON *expires = cJSON_GetObjectItemCaseSensitive(sdm_subs_modificationJSON, "expires");

    if (expires) {
    if (!cJSON_IsString(expires)) {
        ogs_error("OpenAPI_sdm_subs_modification_parseFromJSON() failed [expires]");
        goto end;
    }
    }

    cJSON *monitored_resource_uris = cJSON_GetObjectItemCaseSensitive(sdm_subs_modificationJSON, "monitoredResourceUris");

    OpenAPI_list_t *monitored_resource_urisList;
    if (monitored_resource_uris) {
    cJSON *monitored_resource_uris_local;
    if (!cJSON_IsArray(monitored_resource_uris)) {
        ogs_error("OpenAPI_sdm_subs_modification_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
    monitored_resource_urisList = OpenAPI_list_create();

    cJSON_ArrayForEach(monitored_resource_uris_local, monitored_resource_uris) {
    if (!cJSON_IsString(monitored_resource_uris_local)) {
        ogs_error("OpenAPI_sdm_subs_modification_parseFromJSON() failed [monitored_resource_uris]");
        goto end;
    }
    OpenAPI_list_add(monitored_resource_urisList , ogs_strdup(monitored_resource_uris_local->valuestring));
    }
    }

    sdm_subs_modification_local_var = OpenAPI_sdm_subs_modification_create (
        expires ? ogs_strdup(expires->valuestring) : NULL,
        monitored_resource_uris ? monitored_resource_urisList : NULL
    );

    return sdm_subs_modification_local_var;
end:
    return NULL;
}

OpenAPI_sdm_subs_modification_t *OpenAPI_sdm_subs_modification_copy(OpenAPI_sdm_subs_modification_t *dst, OpenAPI_sdm_subs_modification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sdm_subs_modification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sdm_subs_modification_convertToJSON() failed");
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

    OpenAPI_sdm_subs_modification_free(dst);
    dst = OpenAPI_sdm_subs_modification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

