
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "slice_policy_data.h"

OpenAPI_slice_policy_data_t *OpenAPI_slice_policy_data_create(
    char *mbr_ul,
    char *mbr_dl,
    char *remain_mbr_ul,
    char *remain_mbr_dl,
    char *supp_feat,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_slice_policy_data_t *slice_policy_data_local_var = ogs_malloc(sizeof(OpenAPI_slice_policy_data_t));
    ogs_assert(slice_policy_data_local_var);

    slice_policy_data_local_var->mbr_ul = mbr_ul;
    slice_policy_data_local_var->mbr_dl = mbr_dl;
    slice_policy_data_local_var->remain_mbr_ul = remain_mbr_ul;
    slice_policy_data_local_var->remain_mbr_dl = remain_mbr_dl;
    slice_policy_data_local_var->supp_feat = supp_feat;
    slice_policy_data_local_var->reset_ids = reset_ids;

    return slice_policy_data_local_var;
}

void OpenAPI_slice_policy_data_free(OpenAPI_slice_policy_data_t *slice_policy_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == slice_policy_data) {
        return;
    }
    if (slice_policy_data->mbr_ul) {
        ogs_free(slice_policy_data->mbr_ul);
        slice_policy_data->mbr_ul = NULL;
    }
    if (slice_policy_data->mbr_dl) {
        ogs_free(slice_policy_data->mbr_dl);
        slice_policy_data->mbr_dl = NULL;
    }
    if (slice_policy_data->remain_mbr_ul) {
        ogs_free(slice_policy_data->remain_mbr_ul);
        slice_policy_data->remain_mbr_ul = NULL;
    }
    if (slice_policy_data->remain_mbr_dl) {
        ogs_free(slice_policy_data->remain_mbr_dl);
        slice_policy_data->remain_mbr_dl = NULL;
    }
    if (slice_policy_data->supp_feat) {
        ogs_free(slice_policy_data->supp_feat);
        slice_policy_data->supp_feat = NULL;
    }
    if (slice_policy_data->reset_ids) {
        OpenAPI_list_for_each(slice_policy_data->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(slice_policy_data->reset_ids);
        slice_policy_data->reset_ids = NULL;
    }
    ogs_free(slice_policy_data);
}

cJSON *OpenAPI_slice_policy_data_convertToJSON(OpenAPI_slice_policy_data_t *slice_policy_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (slice_policy_data == NULL) {
        ogs_error("OpenAPI_slice_policy_data_convertToJSON() failed [SlicePolicyData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (slice_policy_data->mbr_ul) {
    if (cJSON_AddStringToObject(item, "mbrUl", slice_policy_data->mbr_ul) == NULL) {
        ogs_error("OpenAPI_slice_policy_data_convertToJSON() failed [mbr_ul]");
        goto end;
    }
    }

    if (slice_policy_data->mbr_dl) {
    if (cJSON_AddStringToObject(item, "mbrDl", slice_policy_data->mbr_dl) == NULL) {
        ogs_error("OpenAPI_slice_policy_data_convertToJSON() failed [mbr_dl]");
        goto end;
    }
    }

    if (slice_policy_data->remain_mbr_ul) {
    if (cJSON_AddStringToObject(item, "remainMbrUl", slice_policy_data->remain_mbr_ul) == NULL) {
        ogs_error("OpenAPI_slice_policy_data_convertToJSON() failed [remain_mbr_ul]");
        goto end;
    }
    }

    if (slice_policy_data->remain_mbr_dl) {
    if (cJSON_AddStringToObject(item, "remainMbrDl", slice_policy_data->remain_mbr_dl) == NULL) {
        ogs_error("OpenAPI_slice_policy_data_convertToJSON() failed [remain_mbr_dl]");
        goto end;
    }
    }

    if (slice_policy_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", slice_policy_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_slice_policy_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (slice_policy_data->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_slice_policy_data_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(slice_policy_data->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_slice_policy_data_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_slice_policy_data_t *OpenAPI_slice_policy_data_parseFromJSON(cJSON *slice_policy_dataJSON)
{
    OpenAPI_slice_policy_data_t *slice_policy_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *mbr_ul = NULL;
    cJSON *mbr_dl = NULL;
    cJSON *remain_mbr_ul = NULL;
    cJSON *remain_mbr_dl = NULL;
    cJSON *supp_feat = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    mbr_ul = cJSON_GetObjectItemCaseSensitive(slice_policy_dataJSON, "mbrUl");
    if (mbr_ul) {
    if (!cJSON_IsString(mbr_ul) && !cJSON_IsNull(mbr_ul)) {
        ogs_error("OpenAPI_slice_policy_data_parseFromJSON() failed [mbr_ul]");
        goto end;
    }
    }

    mbr_dl = cJSON_GetObjectItemCaseSensitive(slice_policy_dataJSON, "mbrDl");
    if (mbr_dl) {
    if (!cJSON_IsString(mbr_dl) && !cJSON_IsNull(mbr_dl)) {
        ogs_error("OpenAPI_slice_policy_data_parseFromJSON() failed [mbr_dl]");
        goto end;
    }
    }

    remain_mbr_ul = cJSON_GetObjectItemCaseSensitive(slice_policy_dataJSON, "remainMbrUl");
    if (remain_mbr_ul) {
    if (!cJSON_IsString(remain_mbr_ul) && !cJSON_IsNull(remain_mbr_ul)) {
        ogs_error("OpenAPI_slice_policy_data_parseFromJSON() failed [remain_mbr_ul]");
        goto end;
    }
    }

    remain_mbr_dl = cJSON_GetObjectItemCaseSensitive(slice_policy_dataJSON, "remainMbrDl");
    if (remain_mbr_dl) {
    if (!cJSON_IsString(remain_mbr_dl) && !cJSON_IsNull(remain_mbr_dl)) {
        ogs_error("OpenAPI_slice_policy_data_parseFromJSON() failed [remain_mbr_dl]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(slice_policy_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_slice_policy_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(slice_policy_dataJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_slice_policy_data_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_slice_policy_data_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    slice_policy_data_local_var = OpenAPI_slice_policy_data_create (
        mbr_ul && !cJSON_IsNull(mbr_ul) ? ogs_strdup(mbr_ul->valuestring) : NULL,
        mbr_dl && !cJSON_IsNull(mbr_dl) ? ogs_strdup(mbr_dl->valuestring) : NULL,
        remain_mbr_ul && !cJSON_IsNull(remain_mbr_ul) ? ogs_strdup(remain_mbr_ul->valuestring) : NULL,
        remain_mbr_dl && !cJSON_IsNull(remain_mbr_dl) ? ogs_strdup(remain_mbr_dl->valuestring) : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return slice_policy_data_local_var;
end:
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    return NULL;
}

OpenAPI_slice_policy_data_t *OpenAPI_slice_policy_data_copy(OpenAPI_slice_policy_data_t *dst, OpenAPI_slice_policy_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_slice_policy_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_slice_policy_data_convertToJSON() failed");
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

    OpenAPI_slice_policy_data_free(dst);
    dst = OpenAPI_slice_policy_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

