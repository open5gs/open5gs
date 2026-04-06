
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pei_update_info_ext.h"

OpenAPI_pei_update_info_ext_t *OpenAPI_pei_update_info_ext_create(
    char *last_pei_change_timestamp,
    char *last_imei_change_timestamp,
    char *previous_pei,
    char *previous_pei_timestamp
)
{
    OpenAPI_pei_update_info_ext_t *pei_update_info_ext_local_var = ogs_malloc(sizeof(OpenAPI_pei_update_info_ext_t));
    ogs_assert(pei_update_info_ext_local_var);

    pei_update_info_ext_local_var->last_pei_change_timestamp = last_pei_change_timestamp;
    pei_update_info_ext_local_var->last_imei_change_timestamp = last_imei_change_timestamp;
    pei_update_info_ext_local_var->previous_pei = previous_pei;
    pei_update_info_ext_local_var->previous_pei_timestamp = previous_pei_timestamp;

    return pei_update_info_ext_local_var;
}

void OpenAPI_pei_update_info_ext_free(OpenAPI_pei_update_info_ext_t *pei_update_info_ext)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pei_update_info_ext) {
        return;
    }
    if (pei_update_info_ext->last_pei_change_timestamp) {
        ogs_free(pei_update_info_ext->last_pei_change_timestamp);
        pei_update_info_ext->last_pei_change_timestamp = NULL;
    }
    if (pei_update_info_ext->last_imei_change_timestamp) {
        ogs_free(pei_update_info_ext->last_imei_change_timestamp);
        pei_update_info_ext->last_imei_change_timestamp = NULL;
    }
    if (pei_update_info_ext->previous_pei) {
        ogs_free(pei_update_info_ext->previous_pei);
        pei_update_info_ext->previous_pei = NULL;
    }
    if (pei_update_info_ext->previous_pei_timestamp) {
        ogs_free(pei_update_info_ext->previous_pei_timestamp);
        pei_update_info_ext->previous_pei_timestamp = NULL;
    }
    ogs_free(pei_update_info_ext);
}

cJSON *OpenAPI_pei_update_info_ext_convertToJSON(OpenAPI_pei_update_info_ext_t *pei_update_info_ext)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pei_update_info_ext == NULL) {
        ogs_error("OpenAPI_pei_update_info_ext_convertToJSON() failed [PeiUpdateInfoExt]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pei_update_info_ext->last_pei_change_timestamp) {
    if (cJSON_AddStringToObject(item, "lastPeiChangeTimestamp", pei_update_info_ext->last_pei_change_timestamp) == NULL) {
        ogs_error("OpenAPI_pei_update_info_ext_convertToJSON() failed [last_pei_change_timestamp]");
        goto end;
    }
    }

    if (pei_update_info_ext->last_imei_change_timestamp) {
    if (cJSON_AddStringToObject(item, "lastImeiChangeTimestamp", pei_update_info_ext->last_imei_change_timestamp) == NULL) {
        ogs_error("OpenAPI_pei_update_info_ext_convertToJSON() failed [last_imei_change_timestamp]");
        goto end;
    }
    }

    if (pei_update_info_ext->previous_pei) {
    if (cJSON_AddStringToObject(item, "previousPei", pei_update_info_ext->previous_pei) == NULL) {
        ogs_error("OpenAPI_pei_update_info_ext_convertToJSON() failed [previous_pei]");
        goto end;
    }
    }

    if (pei_update_info_ext->previous_pei_timestamp) {
    if (cJSON_AddStringToObject(item, "previousPeiTimestamp", pei_update_info_ext->previous_pei_timestamp) == NULL) {
        ogs_error("OpenAPI_pei_update_info_ext_convertToJSON() failed [previous_pei_timestamp]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pei_update_info_ext_t *OpenAPI_pei_update_info_ext_parseFromJSON(cJSON *pei_update_info_extJSON)
{
    OpenAPI_pei_update_info_ext_t *pei_update_info_ext_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *last_pei_change_timestamp = NULL;
    cJSON *last_imei_change_timestamp = NULL;
    cJSON *previous_pei = NULL;
    cJSON *previous_pei_timestamp = NULL;
    last_pei_change_timestamp = cJSON_GetObjectItemCaseSensitive(pei_update_info_extJSON, "lastPeiChangeTimestamp");
    if (last_pei_change_timestamp) {
    if (!cJSON_IsString(last_pei_change_timestamp) && !cJSON_IsNull(last_pei_change_timestamp)) {
        ogs_error("OpenAPI_pei_update_info_ext_parseFromJSON() failed [last_pei_change_timestamp]");
        goto end;
    }
    }

    last_imei_change_timestamp = cJSON_GetObjectItemCaseSensitive(pei_update_info_extJSON, "lastImeiChangeTimestamp");
    if (last_imei_change_timestamp) {
    if (!cJSON_IsString(last_imei_change_timestamp) && !cJSON_IsNull(last_imei_change_timestamp)) {
        ogs_error("OpenAPI_pei_update_info_ext_parseFromJSON() failed [last_imei_change_timestamp]");
        goto end;
    }
    }

    previous_pei = cJSON_GetObjectItemCaseSensitive(pei_update_info_extJSON, "previousPei");
    if (previous_pei) {
    if (!cJSON_IsString(previous_pei) && !cJSON_IsNull(previous_pei)) {
        ogs_error("OpenAPI_pei_update_info_ext_parseFromJSON() failed [previous_pei]");
        goto end;
    }
    }

    previous_pei_timestamp = cJSON_GetObjectItemCaseSensitive(pei_update_info_extJSON, "previousPeiTimestamp");
    if (previous_pei_timestamp) {
    if (!cJSON_IsString(previous_pei_timestamp) && !cJSON_IsNull(previous_pei_timestamp)) {
        ogs_error("OpenAPI_pei_update_info_ext_parseFromJSON() failed [previous_pei_timestamp]");
        goto end;
    }
    }

    pei_update_info_ext_local_var = OpenAPI_pei_update_info_ext_create (
        last_pei_change_timestamp && !cJSON_IsNull(last_pei_change_timestamp) ? ogs_strdup(last_pei_change_timestamp->valuestring) : NULL,
        last_imei_change_timestamp && !cJSON_IsNull(last_imei_change_timestamp) ? ogs_strdup(last_imei_change_timestamp->valuestring) : NULL,
        previous_pei && !cJSON_IsNull(previous_pei) ? ogs_strdup(previous_pei->valuestring) : NULL,
        previous_pei_timestamp && !cJSON_IsNull(previous_pei_timestamp) ? ogs_strdup(previous_pei_timestamp->valuestring) : NULL
    );

    return pei_update_info_ext_local_var;
end:
    return NULL;
}

OpenAPI_pei_update_info_ext_t *OpenAPI_pei_update_info_ext_copy(OpenAPI_pei_update_info_ext_t *dst, OpenAPI_pei_update_info_ext_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pei_update_info_ext_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pei_update_info_ext_convertToJSON() failed");
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

    OpenAPI_pei_update_info_ext_free(dst);
    dst = OpenAPI_pei_update_info_ext_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

