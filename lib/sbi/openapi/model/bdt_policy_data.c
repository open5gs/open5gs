
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "bdt_policy_data.h"

OpenAPI_bdt_policy_data_t *OpenAPI_bdt_policy_data_create(
    char *inter_group_id,
    char *supi,
    char *bdt_ref_id,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *res_uri
)
{
    OpenAPI_bdt_policy_data_t *bdt_policy_data_local_var = ogs_malloc(sizeof(OpenAPI_bdt_policy_data_t));
    ogs_assert(bdt_policy_data_local_var);

    bdt_policy_data_local_var->inter_group_id = inter_group_id;
    bdt_policy_data_local_var->supi = supi;
    bdt_policy_data_local_var->bdt_ref_id = bdt_ref_id;
    bdt_policy_data_local_var->dnn = dnn;
    bdt_policy_data_local_var->snssai = snssai;
    bdt_policy_data_local_var->res_uri = res_uri;

    return bdt_policy_data_local_var;
}

void OpenAPI_bdt_policy_data_free(OpenAPI_bdt_policy_data_t *bdt_policy_data)
{
    if (NULL == bdt_policy_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(bdt_policy_data->inter_group_id);
    ogs_free(bdt_policy_data->supi);
    ogs_free(bdt_policy_data->bdt_ref_id);
    ogs_free(bdt_policy_data->dnn);
    OpenAPI_snssai_free(bdt_policy_data->snssai);
    ogs_free(bdt_policy_data->res_uri);
    ogs_free(bdt_policy_data);
}

cJSON *OpenAPI_bdt_policy_data_convertToJSON(OpenAPI_bdt_policy_data_t *bdt_policy_data)
{
    cJSON *item = NULL;

    if (bdt_policy_data == NULL) {
        ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed [BdtPolicyData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (bdt_policy_data->inter_group_id) {
    if (cJSON_AddStringToObject(item, "interGroupId", bdt_policy_data->inter_group_id) == NULL) {
        ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed [inter_group_id]");
        goto end;
    }
    }

    if (bdt_policy_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", bdt_policy_data->supi) == NULL) {
        ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (cJSON_AddStringToObject(item, "bdtRefId", bdt_policy_data->bdt_ref_id) == NULL) {
        ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed [bdt_ref_id]");
        goto end;
    }

    if (bdt_policy_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", bdt_policy_data->dnn) == NULL) {
        ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (bdt_policy_data->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(bdt_policy_data->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (bdt_policy_data->res_uri) {
    if (cJSON_AddStringToObject(item, "resUri", bdt_policy_data->res_uri) == NULL) {
        ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed [res_uri]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_bdt_policy_data_t *OpenAPI_bdt_policy_data_parseFromJSON(cJSON *bdt_policy_dataJSON)
{
    OpenAPI_bdt_policy_data_t *bdt_policy_data_local_var = NULL;
    cJSON *inter_group_id = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "interGroupId");

    if (inter_group_id) {
    if (!cJSON_IsString(inter_group_id)) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [inter_group_id]");
        goto end;
    }
    }

    cJSON *supi = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "supi");

    if (supi) {
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    cJSON *bdt_ref_id = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "bdtRefId");
    if (!bdt_ref_id) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [bdt_ref_id]");
        goto end;
    }

    if (!cJSON_IsString(bdt_ref_id)) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [bdt_ref_id]");
        goto end;
    }

    cJSON *dnn = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "dnn");

    if (dnn) {
    if (!cJSON_IsString(dnn)) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    cJSON *snssai = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "snssai");

    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    }

    cJSON *res_uri = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "resUri");

    if (res_uri) {
    if (!cJSON_IsString(res_uri)) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [res_uri]");
        goto end;
    }
    }

    bdt_policy_data_local_var = OpenAPI_bdt_policy_data_create (
        inter_group_id ? ogs_strdup(inter_group_id->valuestring) : NULL,
        supi ? ogs_strdup(supi->valuestring) : NULL,
        ogs_strdup(bdt_ref_id->valuestring),
        dnn ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        res_uri ? ogs_strdup(res_uri->valuestring) : NULL
    );

    return bdt_policy_data_local_var;
end:
    return NULL;
}

OpenAPI_bdt_policy_data_t *OpenAPI_bdt_policy_data_copy(OpenAPI_bdt_policy_data_t *dst, OpenAPI_bdt_policy_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_bdt_policy_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed");
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

    OpenAPI_bdt_policy_data_free(dst);
    dst = OpenAPI_bdt_policy_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

