
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_identity_info.h"

OpenAPI_ue_identity_info_t *OpenAPI_ue_identity_info_create(
    char *gpsi,
    char *pei,
    char *supi
)
{
    OpenAPI_ue_identity_info_t *ue_identity_info_local_var = ogs_malloc(sizeof(OpenAPI_ue_identity_info_t));
    ogs_assert(ue_identity_info_local_var);

    ue_identity_info_local_var->gpsi = gpsi;
    ue_identity_info_local_var->pei = pei;
    ue_identity_info_local_var->supi = supi;

    return ue_identity_info_local_var;
}

void OpenAPI_ue_identity_info_free(OpenAPI_ue_identity_info_t *ue_identity_info)
{
    if (NULL == ue_identity_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(ue_identity_info->gpsi);
    ogs_free(ue_identity_info->pei);
    ogs_free(ue_identity_info->supi);
    ogs_free(ue_identity_info);
}

cJSON *OpenAPI_ue_identity_info_convertToJSON(OpenAPI_ue_identity_info_t *ue_identity_info)
{
    cJSON *item = NULL;

    if (ue_identity_info == NULL) {
        ogs_error("OpenAPI_ue_identity_info_convertToJSON() failed [UeIdentityInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_identity_info->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", ue_identity_info->gpsi) == NULL) {
        ogs_error("OpenAPI_ue_identity_info_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (ue_identity_info->pei) {
    if (cJSON_AddStringToObject(item, "pei", ue_identity_info->pei) == NULL) {
        ogs_error("OpenAPI_ue_identity_info_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (ue_identity_info->supi) {
    if (cJSON_AddStringToObject(item, "supi", ue_identity_info->supi) == NULL) {
        ogs_error("OpenAPI_ue_identity_info_convertToJSON() failed [supi]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_identity_info_t *OpenAPI_ue_identity_info_parseFromJSON(cJSON *ue_identity_infoJSON)
{
    OpenAPI_ue_identity_info_t *ue_identity_info_local_var = NULL;
    cJSON *gpsi = cJSON_GetObjectItemCaseSensitive(ue_identity_infoJSON, "gpsi");

    if (gpsi) {
    if (!cJSON_IsString(gpsi)) {
        ogs_error("OpenAPI_ue_identity_info_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    cJSON *pei = cJSON_GetObjectItemCaseSensitive(ue_identity_infoJSON, "pei");

    if (pei) {
    if (!cJSON_IsString(pei)) {
        ogs_error("OpenAPI_ue_identity_info_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(ue_identity_infoJSON, "supi");

    if (supi) {
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_ue_identity_info_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    ue_identity_info_local_var = OpenAPI_ue_identity_info_create (
        gpsi ? ogs_strdup(gpsi->valuestring) : NULL,
        pei ? ogs_strdup(pei->valuestring) : NULL,
        supi ? ogs_strdup(supi->valuestring) : NULL
    );

    return ue_identity_info_local_var;
end:
    return NULL;
}

OpenAPI_ue_identity_info_t *OpenAPI_ue_identity_info_copy(OpenAPI_ue_identity_info_t *dst, OpenAPI_ue_identity_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_identity_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_identity_info_convertToJSON() failed");
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

    OpenAPI_ue_identity_info_free(dst);
    dst = OpenAPI_ue_identity_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

