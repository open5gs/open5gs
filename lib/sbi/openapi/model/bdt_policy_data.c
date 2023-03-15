
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
    char *res_uri,
    OpenAPI_list_t *reset_ids
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
    bdt_policy_data_local_var->reset_ids = reset_ids;

    return bdt_policy_data_local_var;
}

void OpenAPI_bdt_policy_data_free(OpenAPI_bdt_policy_data_t *bdt_policy_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == bdt_policy_data) {
        return;
    }
    if (bdt_policy_data->inter_group_id) {
        ogs_free(bdt_policy_data->inter_group_id);
        bdt_policy_data->inter_group_id = NULL;
    }
    if (bdt_policy_data->supi) {
        ogs_free(bdt_policy_data->supi);
        bdt_policy_data->supi = NULL;
    }
    if (bdt_policy_data->bdt_ref_id) {
        ogs_free(bdt_policy_data->bdt_ref_id);
        bdt_policy_data->bdt_ref_id = NULL;
    }
    if (bdt_policy_data->dnn) {
        ogs_free(bdt_policy_data->dnn);
        bdt_policy_data->dnn = NULL;
    }
    if (bdt_policy_data->snssai) {
        OpenAPI_snssai_free(bdt_policy_data->snssai);
        bdt_policy_data->snssai = NULL;
    }
    if (bdt_policy_data->res_uri) {
        ogs_free(bdt_policy_data->res_uri);
        bdt_policy_data->res_uri = NULL;
    }
    if (bdt_policy_data->reset_ids) {
        OpenAPI_list_for_each(bdt_policy_data->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(bdt_policy_data->reset_ids);
        bdt_policy_data->reset_ids = NULL;
    }
    ogs_free(bdt_policy_data);
}

cJSON *OpenAPI_bdt_policy_data_convertToJSON(OpenAPI_bdt_policy_data_t *bdt_policy_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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

    if (!bdt_policy_data->bdt_ref_id) {
        ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed [bdt_ref_id]");
        return NULL;
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

    if (bdt_policy_data->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(bdt_policy_data->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_bdt_policy_data_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_bdt_policy_data_t *OpenAPI_bdt_policy_data_parseFromJSON(cJSON *bdt_policy_dataJSON)
{
    OpenAPI_bdt_policy_data_t *bdt_policy_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *inter_group_id = NULL;
    cJSON *supi = NULL;
    cJSON *bdt_ref_id = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *res_uri = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    inter_group_id = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "interGroupId");
    if (inter_group_id) {
    if (!cJSON_IsString(inter_group_id) && !cJSON_IsNull(inter_group_id)) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [inter_group_id]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    bdt_ref_id = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "bdtRefId");
    if (!bdt_ref_id) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [bdt_ref_id]");
        goto end;
    }
    if (!cJSON_IsString(bdt_ref_id)) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [bdt_ref_id]");
        goto end;
    }

    dnn = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    res_uri = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "resUri");
    if (res_uri) {
    if (!cJSON_IsString(res_uri) && !cJSON_IsNull(res_uri)) {
        ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [res_uri]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(bdt_policy_dataJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_bdt_policy_data_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    bdt_policy_data_local_var = OpenAPI_bdt_policy_data_create (
        inter_group_id && !cJSON_IsNull(inter_group_id) ? ogs_strdup(inter_group_id->valuestring) : NULL,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        ogs_strdup(bdt_ref_id->valuestring),
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        res_uri && !cJSON_IsNull(res_uri) ? ogs_strdup(res_uri->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return bdt_policy_data_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
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

