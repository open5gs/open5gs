
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pei_update_info_1.h"

OpenAPI_pei_update_info_1_t *OpenAPI_pei_update_info_1_create(
    char *pei
)
{
    OpenAPI_pei_update_info_1_t *pei_update_info_1_local_var = ogs_malloc(sizeof(OpenAPI_pei_update_info_1_t));
    ogs_assert(pei_update_info_1_local_var);

    pei_update_info_1_local_var->pei = pei;

    return pei_update_info_1_local_var;
}

void OpenAPI_pei_update_info_1_free(OpenAPI_pei_update_info_1_t *pei_update_info_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pei_update_info_1) {
        return;
    }
    if (pei_update_info_1->pei) {
        ogs_free(pei_update_info_1->pei);
        pei_update_info_1->pei = NULL;
    }
    ogs_free(pei_update_info_1);
}

cJSON *OpenAPI_pei_update_info_1_convertToJSON(OpenAPI_pei_update_info_1_t *pei_update_info_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pei_update_info_1 == NULL) {
        ogs_error("OpenAPI_pei_update_info_1_convertToJSON() failed [PeiUpdateInfo_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!pei_update_info_1->pei) {
        ogs_error("OpenAPI_pei_update_info_1_convertToJSON() failed [pei]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "pei", pei_update_info_1->pei) == NULL) {
        ogs_error("OpenAPI_pei_update_info_1_convertToJSON() failed [pei]");
        goto end;
    }

end:
    return item;
}

OpenAPI_pei_update_info_1_t *OpenAPI_pei_update_info_1_parseFromJSON(cJSON *pei_update_info_1JSON)
{
    OpenAPI_pei_update_info_1_t *pei_update_info_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pei = NULL;
    pei = cJSON_GetObjectItemCaseSensitive(pei_update_info_1JSON, "pei");
    if (!pei) {
        ogs_error("OpenAPI_pei_update_info_1_parseFromJSON() failed [pei]");
        goto end;
    }
    if (!cJSON_IsString(pei)) {
        ogs_error("OpenAPI_pei_update_info_1_parseFromJSON() failed [pei]");
        goto end;
    }

    pei_update_info_1_local_var = OpenAPI_pei_update_info_1_create (
        ogs_strdup(pei->valuestring)
    );

    return pei_update_info_1_local_var;
end:
    return NULL;
}

OpenAPI_pei_update_info_1_t *OpenAPI_pei_update_info_1_copy(OpenAPI_pei_update_info_1_t *dst, OpenAPI_pei_update_info_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pei_update_info_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pei_update_info_1_convertToJSON() failed");
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

    OpenAPI_pei_update_info_1_free(dst);
    dst = OpenAPI_pei_update_info_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

