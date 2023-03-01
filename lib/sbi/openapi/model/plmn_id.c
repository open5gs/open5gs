
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_id.h"

OpenAPI_plmn_id_t *OpenAPI_plmn_id_create(
    char *mcc,
    char *mnc
)
{
    OpenAPI_plmn_id_t *plmn_id_local_var = ogs_malloc(sizeof(OpenAPI_plmn_id_t));
    ogs_assert(plmn_id_local_var);

    plmn_id_local_var->mcc = mcc;
    plmn_id_local_var->mnc = mnc;

    return plmn_id_local_var;
}

void OpenAPI_plmn_id_free(OpenAPI_plmn_id_t *plmn_id)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == plmn_id) {
        return;
    }
    if (plmn_id->mcc) {
        ogs_free(plmn_id->mcc);
        plmn_id->mcc = NULL;
    }
    if (plmn_id->mnc) {
        ogs_free(plmn_id->mnc);
        plmn_id->mnc = NULL;
    }
    ogs_free(plmn_id);
}

cJSON *OpenAPI_plmn_id_convertToJSON(OpenAPI_plmn_id_t *plmn_id)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (plmn_id == NULL) {
        ogs_error("OpenAPI_plmn_id_convertToJSON() failed [PlmnId]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!plmn_id->mcc) {
        ogs_error("OpenAPI_plmn_id_convertToJSON() failed [mcc]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "mcc", plmn_id->mcc) == NULL) {
        ogs_error("OpenAPI_plmn_id_convertToJSON() failed [mcc]");
        goto end;
    }

    if (!plmn_id->mnc) {
        ogs_error("OpenAPI_plmn_id_convertToJSON() failed [mnc]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "mnc", plmn_id->mnc) == NULL) {
        ogs_error("OpenAPI_plmn_id_convertToJSON() failed [mnc]");
        goto end;
    }

end:
    return item;
}

OpenAPI_plmn_id_t *OpenAPI_plmn_id_parseFromJSON(cJSON *plmn_idJSON)
{
    OpenAPI_plmn_id_t *plmn_id_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mcc = NULL;
    cJSON *mnc = NULL;
    mcc = cJSON_GetObjectItemCaseSensitive(plmn_idJSON, "mcc");
    if (!mcc) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON() failed [mcc]");
        goto end;
    }
    if (!cJSON_IsString(mcc)) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON() failed [mcc]");
        goto end;
    }

    mnc = cJSON_GetObjectItemCaseSensitive(plmn_idJSON, "mnc");
    if (!mnc) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON() failed [mnc]");
        goto end;
    }
    if (!cJSON_IsString(mnc)) {
        ogs_error("OpenAPI_plmn_id_parseFromJSON() failed [mnc]");
        goto end;
    }

    plmn_id_local_var = OpenAPI_plmn_id_create (
        ogs_strdup(mcc->valuestring),
        ogs_strdup(mnc->valuestring)
    );

    return plmn_id_local_var;
end:
    return NULL;
}

OpenAPI_plmn_id_t *OpenAPI_plmn_id_copy(OpenAPI_plmn_id_t *dst, OpenAPI_plmn_id_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_plmn_id_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_plmn_id_convertToJSON() failed");
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

    OpenAPI_plmn_id_free(dst);
    dst = OpenAPI_plmn_id_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

