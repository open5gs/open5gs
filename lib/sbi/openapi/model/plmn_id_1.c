
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_id_1.h"

OpenAPI_plmn_id_1_t *OpenAPI_plmn_id_1_create(
    char *mcc,
    char *mnc
)
{
    OpenAPI_plmn_id_1_t *plmn_id_1_local_var = ogs_malloc(sizeof(OpenAPI_plmn_id_1_t));
    ogs_assert(plmn_id_1_local_var);

    plmn_id_1_local_var->mcc = mcc;
    plmn_id_1_local_var->mnc = mnc;

    return plmn_id_1_local_var;
}

void OpenAPI_plmn_id_1_free(OpenAPI_plmn_id_1_t *plmn_id_1)
{
    if (NULL == plmn_id_1) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(plmn_id_1->mcc);
    ogs_free(plmn_id_1->mnc);
    ogs_free(plmn_id_1);
}

cJSON *OpenAPI_plmn_id_1_convertToJSON(OpenAPI_plmn_id_1_t *plmn_id_1)
{
    cJSON *item = NULL;

    if (plmn_id_1 == NULL) {
        ogs_error("OpenAPI_plmn_id_1_convertToJSON() failed [PlmnId_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "mcc", plmn_id_1->mcc) == NULL) {
        ogs_error("OpenAPI_plmn_id_1_convertToJSON() failed [mcc]");
        goto end;
    }

    if (cJSON_AddStringToObject(item, "mnc", plmn_id_1->mnc) == NULL) {
        ogs_error("OpenAPI_plmn_id_1_convertToJSON() failed [mnc]");
        goto end;
    }

end:
    return item;
}

OpenAPI_plmn_id_1_t *OpenAPI_plmn_id_1_parseFromJSON(cJSON *plmn_id_1JSON)
{
    OpenAPI_plmn_id_1_t *plmn_id_1_local_var = NULL;
    cJSON *mcc = cJSON_GetObjectItemCaseSensitive(plmn_id_1JSON, "mcc");
    if (!mcc) {
        ogs_error("OpenAPI_plmn_id_1_parseFromJSON() failed [mcc]");
        goto end;
    }

    if (!cJSON_IsString(mcc)) {
        ogs_error("OpenAPI_plmn_id_1_parseFromJSON() failed [mcc]");
        goto end;
    }

    cJSON *mnc = cJSON_GetObjectItemCaseSensitive(plmn_id_1JSON, "mnc");
    if (!mnc) {
        ogs_error("OpenAPI_plmn_id_1_parseFromJSON() failed [mnc]");
        goto end;
    }

    if (!cJSON_IsString(mnc)) {
        ogs_error("OpenAPI_plmn_id_1_parseFromJSON() failed [mnc]");
        goto end;
    }

    plmn_id_1_local_var = OpenAPI_plmn_id_1_create (
        ogs_strdup(mcc->valuestring),
        ogs_strdup(mnc->valuestring)
    );

    return plmn_id_1_local_var;
end:
    return NULL;
}

OpenAPI_plmn_id_1_t *OpenAPI_plmn_id_1_copy(OpenAPI_plmn_id_1_t *dst, OpenAPI_plmn_id_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_plmn_id_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_plmn_id_1_convertToJSON() failed");
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

    OpenAPI_plmn_id_1_free(dst);
    dst = OpenAPI_plmn_id_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

