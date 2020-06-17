
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "exemption_ind.h"

OpenAPI_exemption_ind_t *OpenAPI_exemption_ind_create(
    int dnn_congestion,
    int snssai_only_congestion,
    int snssai_dnn_congestion
    )
{
    OpenAPI_exemption_ind_t *exemption_ind_local_var = OpenAPI_malloc(sizeof(OpenAPI_exemption_ind_t));
    if (!exemption_ind_local_var) {
        return NULL;
    }
    exemption_ind_local_var->dnn_congestion = dnn_congestion;
    exemption_ind_local_var->snssai_only_congestion = snssai_only_congestion;
    exemption_ind_local_var->snssai_dnn_congestion = snssai_dnn_congestion;

    return exemption_ind_local_var;
}

void OpenAPI_exemption_ind_free(OpenAPI_exemption_ind_t *exemption_ind)
{
    if (NULL == exemption_ind) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(exemption_ind);
}

cJSON *OpenAPI_exemption_ind_convertToJSON(OpenAPI_exemption_ind_t *exemption_ind)
{
    cJSON *item = NULL;

    if (exemption_ind == NULL) {
        ogs_error("OpenAPI_exemption_ind_convertToJSON() failed [ExemptionInd]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (exemption_ind->dnn_congestion) {
        if (cJSON_AddBoolToObject(item, "dnnCongestion", exemption_ind->dnn_congestion) == NULL) {
            ogs_error("OpenAPI_exemption_ind_convertToJSON() failed [dnn_congestion]");
            goto end;
        }
    }

    if (exemption_ind->snssai_only_congestion) {
        if (cJSON_AddBoolToObject(item, "snssaiOnlyCongestion", exemption_ind->snssai_only_congestion) == NULL) {
            ogs_error("OpenAPI_exemption_ind_convertToJSON() failed [snssai_only_congestion]");
            goto end;
        }
    }

    if (exemption_ind->snssai_dnn_congestion) {
        if (cJSON_AddBoolToObject(item, "snssaiDnnCongestion", exemption_ind->snssai_dnn_congestion) == NULL) {
            ogs_error("OpenAPI_exemption_ind_convertToJSON() failed [snssai_dnn_congestion]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_exemption_ind_t *OpenAPI_exemption_ind_parseFromJSON(cJSON *exemption_indJSON)
{
    OpenAPI_exemption_ind_t *exemption_ind_local_var = NULL;
    cJSON *dnn_congestion = cJSON_GetObjectItemCaseSensitive(exemption_indJSON, "dnnCongestion");

    if (dnn_congestion) {
        if (!cJSON_IsBool(dnn_congestion)) {
            ogs_error("OpenAPI_exemption_ind_parseFromJSON() failed [dnn_congestion]");
            goto end;
        }
    }

    cJSON *snssai_only_congestion = cJSON_GetObjectItemCaseSensitive(exemption_indJSON, "snssaiOnlyCongestion");

    if (snssai_only_congestion) {
        if (!cJSON_IsBool(snssai_only_congestion)) {
            ogs_error("OpenAPI_exemption_ind_parseFromJSON() failed [snssai_only_congestion]");
            goto end;
        }
    }

    cJSON *snssai_dnn_congestion = cJSON_GetObjectItemCaseSensitive(exemption_indJSON, "snssaiDnnCongestion");

    if (snssai_dnn_congestion) {
        if (!cJSON_IsBool(snssai_dnn_congestion)) {
            ogs_error("OpenAPI_exemption_ind_parseFromJSON() failed [snssai_dnn_congestion]");
            goto end;
        }
    }

    exemption_ind_local_var = OpenAPI_exemption_ind_create (
        dnn_congestion ? dnn_congestion->valueint : 0,
        snssai_only_congestion ? snssai_only_congestion->valueint : 0,
        snssai_dnn_congestion ? snssai_dnn_congestion->valueint : 0
        );

    return exemption_ind_local_var;
end:
    return NULL;
}

OpenAPI_exemption_ind_t *OpenAPI_exemption_ind_copy(OpenAPI_exemption_ind_t *dst, OpenAPI_exemption_ind_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_exemption_ind_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_exemption_ind_convertToJSON() failed");
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

    OpenAPI_exemption_ind_free(dst);
    dst = OpenAPI_exemption_ind_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

