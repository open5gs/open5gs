
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "chf_service_info.h"

OpenAPI_chf_service_info_t *OpenAPI_chf_service_info_create(
    char *primary_chf_service_instance,
    char *secondary_chf_service_instance
    )
{
    OpenAPI_chf_service_info_t *chf_service_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_chf_service_info_t));
    if (!chf_service_info_local_var) {
        return NULL;
    }
    chf_service_info_local_var->primary_chf_service_instance = primary_chf_service_instance;
    chf_service_info_local_var->secondary_chf_service_instance = secondary_chf_service_instance;

    return chf_service_info_local_var;
}

void OpenAPI_chf_service_info_free(OpenAPI_chf_service_info_t *chf_service_info)
{
    if (NULL == chf_service_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(chf_service_info->primary_chf_service_instance);
    ogs_free(chf_service_info->secondary_chf_service_instance);
    ogs_free(chf_service_info);
}

cJSON *OpenAPI_chf_service_info_convertToJSON(OpenAPI_chf_service_info_t *chf_service_info)
{
    cJSON *item = NULL;

    if (chf_service_info == NULL) {
        ogs_error("OpenAPI_chf_service_info_convertToJSON() failed [ChfServiceInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (chf_service_info->primary_chf_service_instance) {
        if (cJSON_AddStringToObject(item, "primaryChfServiceInstance", chf_service_info->primary_chf_service_instance) == NULL) {
            ogs_error("OpenAPI_chf_service_info_convertToJSON() failed [primary_chf_service_instance]");
            goto end;
        }
    }

    if (chf_service_info->secondary_chf_service_instance) {
        if (cJSON_AddStringToObject(item, "secondaryChfServiceInstance", chf_service_info->secondary_chf_service_instance) == NULL) {
            ogs_error("OpenAPI_chf_service_info_convertToJSON() failed [secondary_chf_service_instance]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_chf_service_info_t *OpenAPI_chf_service_info_parseFromJSON(cJSON *chf_service_infoJSON)
{
    OpenAPI_chf_service_info_t *chf_service_info_local_var = NULL;
    cJSON *primary_chf_service_instance = cJSON_GetObjectItemCaseSensitive(chf_service_infoJSON, "primaryChfServiceInstance");

    if (primary_chf_service_instance) {
        if (!cJSON_IsString(primary_chf_service_instance)) {
            ogs_error("OpenAPI_chf_service_info_parseFromJSON() failed [primary_chf_service_instance]");
            goto end;
        }
    }

    cJSON *secondary_chf_service_instance = cJSON_GetObjectItemCaseSensitive(chf_service_infoJSON, "secondaryChfServiceInstance");

    if (secondary_chf_service_instance) {
        if (!cJSON_IsString(secondary_chf_service_instance)) {
            ogs_error("OpenAPI_chf_service_info_parseFromJSON() failed [secondary_chf_service_instance]");
            goto end;
        }
    }

    chf_service_info_local_var = OpenAPI_chf_service_info_create (
        primary_chf_service_instance ? ogs_strdup(primary_chf_service_instance->valuestring) : NULL,
        secondary_chf_service_instance ? ogs_strdup(secondary_chf_service_instance->valuestring) : NULL
        );

    return chf_service_info_local_var;
end:
    return NULL;
}

OpenAPI_chf_service_info_t *OpenAPI_chf_service_info_copy(OpenAPI_chf_service_info_t *dst, OpenAPI_chf_service_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_chf_service_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_chf_service_info_convertToJSON() failed");
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

    OpenAPI_chf_service_info_free(dst);
    dst = OpenAPI_chf_service_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

