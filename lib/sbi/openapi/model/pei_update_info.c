
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pei_update_info.h"

OpenAPI_pei_update_info_t *OpenAPI_pei_update_info_create(
    char *pei
    )
{
    OpenAPI_pei_update_info_t *pei_update_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_pei_update_info_t));
    if (!pei_update_info_local_var) {
        return NULL;
    }
    pei_update_info_local_var->pei = pei;

    return pei_update_info_local_var;
}

void OpenAPI_pei_update_info_free(OpenAPI_pei_update_info_t *pei_update_info)
{
    if (NULL == pei_update_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(pei_update_info->pei);
    ogs_free(pei_update_info);
}

cJSON *OpenAPI_pei_update_info_convertToJSON(OpenAPI_pei_update_info_t *pei_update_info)
{
    cJSON *item = NULL;

    if (pei_update_info == NULL) {
        ogs_error("OpenAPI_pei_update_info_convertToJSON() failed [PeiUpdateInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pei_update_info->pei) {
        ogs_error("OpenAPI_pei_update_info_convertToJSON() failed [pei]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "pei", pei_update_info->pei) == NULL) {
        ogs_error("OpenAPI_pei_update_info_convertToJSON() failed [pei]");
        goto end;
    }

end:
    return item;
}

OpenAPI_pei_update_info_t *OpenAPI_pei_update_info_parseFromJSON(cJSON *pei_update_infoJSON)
{
    OpenAPI_pei_update_info_t *pei_update_info_local_var = NULL;
    cJSON *pei = cJSON_GetObjectItemCaseSensitive(pei_update_infoJSON, "pei");
    if (!pei) {
        ogs_error("OpenAPI_pei_update_info_parseFromJSON() failed [pei]");
        goto end;
    }


    if (!cJSON_IsString(pei)) {
        ogs_error("OpenAPI_pei_update_info_parseFromJSON() failed [pei]");
        goto end;
    }

    pei_update_info_local_var = OpenAPI_pei_update_info_create (
        ogs_strdup(pei->valuestring)
        );

    return pei_update_info_local_var;
end:
    return NULL;
}

OpenAPI_pei_update_info_t *OpenAPI_pei_update_info_copy(OpenAPI_pei_update_info_t *dst, OpenAPI_pei_update_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pei_update_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pei_update_info_convertToJSON() failed");
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

    OpenAPI_pei_update_info_free(dst);
    dst = OpenAPI_pei_update_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

