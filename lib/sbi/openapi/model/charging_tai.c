
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_tai.h"

OpenAPI_charging_tai_t *OpenAPI_charging_tai_create(
    char *mcc,
    char *mnc,
    char *tac
)
{
    OpenAPI_charging_tai_t *charging_tai_local_var = ogs_malloc(sizeof(OpenAPI_charging_tai_t));
    ogs_assert(charging_tai_local_var);

    charging_tai_local_var->mcc = mcc;
    charging_tai_local_var->mnc = mnc;
    charging_tai_local_var->tac = tac;

    return charging_tai_local_var;
}

void OpenAPI_charging_tai_free(OpenAPI_charging_tai_t *charging_tai)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_tai) {
        return;
    }
    if (charging_tai->mcc) {
        ogs_free(charging_tai->mcc);
        charging_tai->mcc = NULL;
    }
    if (charging_tai->mnc) {
        ogs_free(charging_tai->mnc);
        charging_tai->mnc = NULL;
    }
    if (charging_tai->tac) {
        ogs_free(charging_tai->tac);
        charging_tai->tac = NULL;
    }
    ogs_free(charging_tai);
}

cJSON *OpenAPI_charging_tai_convertToJSON(OpenAPI_charging_tai_t *charging_tai)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_tai == NULL) {
        ogs_error("OpenAPI_charging_tai_convertToJSON() failed [ChargingTai]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (charging_tai->mcc) {
    if (cJSON_AddStringToObject(item, "mcc", charging_tai->mcc) == NULL) {
        ogs_error("OpenAPI_charging_tai_convertToJSON() failed [mcc]");
        goto end;
    }
    }

    if (charging_tai->mnc) {
    if (cJSON_AddStringToObject(item, "mnc", charging_tai->mnc) == NULL) {
        ogs_error("OpenAPI_charging_tai_convertToJSON() failed [mnc]");
        goto end;
    }
    }

    if (charging_tai->tac) {
    if (cJSON_AddStringToObject(item, "tac", charging_tai->tac) == NULL) {
        ogs_error("OpenAPI_charging_tai_convertToJSON() failed [tac]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_charging_tai_t *OpenAPI_charging_tai_parseFromJSON(cJSON *charging_taiJSON)
{
    OpenAPI_charging_tai_t *charging_tai_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mcc = NULL;
    cJSON *mnc = NULL;
    cJSON *tac = NULL;
    mcc = cJSON_GetObjectItemCaseSensitive(charging_taiJSON, "mcc");
    if (mcc) {
    if (!cJSON_IsString(mcc) && !cJSON_IsNull(mcc)) {
        ogs_error("OpenAPI_charging_tai_parseFromJSON() failed [mcc]");
        goto end;
    }
    }

    mnc = cJSON_GetObjectItemCaseSensitive(charging_taiJSON, "mnc");
    if (mnc) {
    if (!cJSON_IsString(mnc) && !cJSON_IsNull(mnc)) {
        ogs_error("OpenAPI_charging_tai_parseFromJSON() failed [mnc]");
        goto end;
    }
    }

    tac = cJSON_GetObjectItemCaseSensitive(charging_taiJSON, "tac");
    if (tac) {
    if (!cJSON_IsString(tac) && !cJSON_IsNull(tac)) {
        ogs_error("OpenAPI_charging_tai_parseFromJSON() failed [tac]");
        goto end;
    }
    }

    charging_tai_local_var = OpenAPI_charging_tai_create (
        mcc && !cJSON_IsNull(mcc) ? ogs_strdup(mcc->valuestring) : NULL,
        mnc && !cJSON_IsNull(mnc) ? ogs_strdup(mnc->valuestring) : NULL,
        tac && !cJSON_IsNull(tac) ? ogs_strdup(tac->valuestring) : NULL
    );

    return charging_tai_local_var;
end:
    return NULL;
}

OpenAPI_charging_tai_t *OpenAPI_charging_tai_copy(OpenAPI_charging_tai_t *dst, OpenAPI_charging_tai_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_tai_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_tai_convertToJSON() failed");
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

    OpenAPI_charging_tai_free(dst);
    dst = OpenAPI_charging_tai_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

