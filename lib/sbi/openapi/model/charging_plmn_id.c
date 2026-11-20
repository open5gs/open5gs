
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_plmn_id.h"

OpenAPI_charging_plmn_id_t *OpenAPI_charging_plmn_id_create(
    char *mcc,
    char *mnc
)
{
    OpenAPI_charging_plmn_id_t *charging_plmn_id_local_var = ogs_malloc(sizeof(OpenAPI_charging_plmn_id_t));
    ogs_assert(charging_plmn_id_local_var);

    charging_plmn_id_local_var->mcc = mcc;
    charging_plmn_id_local_var->mnc = mnc;

    return charging_plmn_id_local_var;
}

void OpenAPI_charging_plmn_id_free(OpenAPI_charging_plmn_id_t *charging_plmn_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_plmn_id) {
        return;
    }
    if (charging_plmn_id->mcc) {
        ogs_free(charging_plmn_id->mcc);
        charging_plmn_id->mcc = NULL;
    }
    if (charging_plmn_id->mnc) {
        ogs_free(charging_plmn_id->mnc);
        charging_plmn_id->mnc = NULL;
    }
    ogs_free(charging_plmn_id);
}

cJSON *OpenAPI_charging_plmn_id_convertToJSON(OpenAPI_charging_plmn_id_t *charging_plmn_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_plmn_id == NULL) {
        ogs_error("OpenAPI_charging_plmn_id_convertToJSON() failed [ChargingPlmnId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (charging_plmn_id->mcc) {
    if (cJSON_AddStringToObject(item, "mcc", charging_plmn_id->mcc) == NULL) {
        ogs_error("OpenAPI_charging_plmn_id_convertToJSON() failed [mcc]");
        goto end;
    }
    }

    if (charging_plmn_id->mnc) {
    if (cJSON_AddStringToObject(item, "mnc", charging_plmn_id->mnc) == NULL) {
        ogs_error("OpenAPI_charging_plmn_id_convertToJSON() failed [mnc]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_charging_plmn_id_t *OpenAPI_charging_plmn_id_parseFromJSON(cJSON *charging_plmn_idJSON)
{
    OpenAPI_charging_plmn_id_t *charging_plmn_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mcc = NULL;
    cJSON *mnc = NULL;
    mcc = cJSON_GetObjectItemCaseSensitive(charging_plmn_idJSON, "mcc");
    if (mcc) {
    if (!cJSON_IsString(mcc) && !cJSON_IsNull(mcc)) {
        ogs_error("OpenAPI_charging_plmn_id_parseFromJSON() failed [mcc]");
        goto end;
    }
    }

    mnc = cJSON_GetObjectItemCaseSensitive(charging_plmn_idJSON, "mnc");
    if (mnc) {
    if (!cJSON_IsString(mnc) && !cJSON_IsNull(mnc)) {
        ogs_error("OpenAPI_charging_plmn_id_parseFromJSON() failed [mnc]");
        goto end;
    }
    }

    charging_plmn_id_local_var = OpenAPI_charging_plmn_id_create (
        mcc && !cJSON_IsNull(mcc) ? ogs_strdup(mcc->valuestring) : NULL,
        mnc && !cJSON_IsNull(mnc) ? ogs_strdup(mnc->valuestring) : NULL
    );

    return charging_plmn_id_local_var;
end:
    return NULL;
}

OpenAPI_charging_plmn_id_t *OpenAPI_charging_plmn_id_copy(OpenAPI_charging_plmn_id_t *dst, OpenAPI_charging_plmn_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_plmn_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_plmn_id_convertToJSON() failed");
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

    OpenAPI_charging_plmn_id_free(dst);
    dst = OpenAPI_charging_plmn_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

