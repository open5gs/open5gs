
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_context_in_smf_data_sub_filter.h"

OpenAPI_ue_context_in_smf_data_sub_filter_t *OpenAPI_ue_context_in_smf_data_sub_filter_create(
    OpenAPI_list_t *dnn_list,
    OpenAPI_list_t *snssai_list,
    bool is_emergency_ind,
    int emergency_ind
)
{
    OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_context_in_smf_data_sub_filter_local_var = ogs_malloc(sizeof(OpenAPI_ue_context_in_smf_data_sub_filter_t));
    ogs_assert(ue_context_in_smf_data_sub_filter_local_var);

    ue_context_in_smf_data_sub_filter_local_var->dnn_list = dnn_list;
    ue_context_in_smf_data_sub_filter_local_var->snssai_list = snssai_list;
    ue_context_in_smf_data_sub_filter_local_var->is_emergency_ind = is_emergency_ind;
    ue_context_in_smf_data_sub_filter_local_var->emergency_ind = emergency_ind;

    return ue_context_in_smf_data_sub_filter_local_var;
}

void OpenAPI_ue_context_in_smf_data_sub_filter_free(OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_context_in_smf_data_sub_filter)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_context_in_smf_data_sub_filter) {
        return;
    }
    if (ue_context_in_smf_data_sub_filter->dnn_list) {
        OpenAPI_list_for_each(ue_context_in_smf_data_sub_filter->dnn_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(ue_context_in_smf_data_sub_filter->dnn_list);
        ue_context_in_smf_data_sub_filter->dnn_list = NULL;
    }
    if (ue_context_in_smf_data_sub_filter->snssai_list) {
        OpenAPI_list_for_each(ue_context_in_smf_data_sub_filter->snssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(ue_context_in_smf_data_sub_filter->snssai_list);
        ue_context_in_smf_data_sub_filter->snssai_list = NULL;
    }
    ogs_free(ue_context_in_smf_data_sub_filter);
}

cJSON *OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON(OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_context_in_smf_data_sub_filter)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_context_in_smf_data_sub_filter == NULL) {
        ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON() failed [UeContextInSmfDataSubFilter]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_context_in_smf_data_sub_filter->dnn_list) {
    cJSON *dnn_listList = cJSON_AddArrayToObject(item, "dnnList");
    if (dnn_listList == NULL) {
        ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON() failed [dnn_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context_in_smf_data_sub_filter->dnn_list, node) {
        if (cJSON_AddStringToObject(dnn_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON() failed [dnn_list]");
            goto end;
        }
    }
    }

    if (ue_context_in_smf_data_sub_filter->snssai_list) {
    cJSON *snssai_listList = cJSON_AddArrayToObject(item, "snssaiList");
    if (snssai_listList == NULL) {
        ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON() failed [snssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(ue_context_in_smf_data_sub_filter->snssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON() failed [snssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(snssai_listList, itemLocal);
    }
    }

    if (ue_context_in_smf_data_sub_filter->is_emergency_ind) {
    if (cJSON_AddBoolToObject(item, "emergencyInd", ue_context_in_smf_data_sub_filter->emergency_ind) == NULL) {
        ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON() failed [emergency_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_context_in_smf_data_sub_filter_t *OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON(cJSON *ue_context_in_smf_data_sub_filterJSON)
{
    OpenAPI_ue_context_in_smf_data_sub_filter_t *ue_context_in_smf_data_sub_filter_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *dnn_list = NULL;
    OpenAPI_list_t *dnn_listList = NULL;
    cJSON *snssai_list = NULL;
    OpenAPI_list_t *snssai_listList = NULL;
    cJSON *emergency_ind = NULL;
    dnn_list = cJSON_GetObjectItemCaseSensitive(ue_context_in_smf_data_sub_filterJSON, "dnnList");
    if (dnn_list) {
        cJSON *dnn_list_local = NULL;
        if (!cJSON_IsArray(dnn_list)) {
            ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON() failed [dnn_list]");
            goto end;
        }

        dnn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_list_local, dnn_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(dnn_list_local)) {
                ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON() failed [dnn_list]");
                goto end;
            }
            OpenAPI_list_add(dnn_listList, ogs_strdup(dnn_list_local->valuestring));
        }
    }

    snssai_list = cJSON_GetObjectItemCaseSensitive(ue_context_in_smf_data_sub_filterJSON, "snssaiList");
    if (snssai_list) {
        cJSON *snssai_list_local = NULL;
        if (!cJSON_IsArray(snssai_list)) {
            ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON() failed [snssai_list]");
            goto end;
        }

        snssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(snssai_list_local, snssai_list) {
            if (!cJSON_IsObject(snssai_list_local)) {
                ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON() failed [snssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *snssai_listItem = OpenAPI_snssai_parseFromJSON(snssai_list_local);
            if (!snssai_listItem) {
                ogs_error("No snssai_listItem");
                goto end;
            }
            OpenAPI_list_add(snssai_listList, snssai_listItem);
        }
    }

    emergency_ind = cJSON_GetObjectItemCaseSensitive(ue_context_in_smf_data_sub_filterJSON, "emergencyInd");
    if (emergency_ind) {
    if (!cJSON_IsBool(emergency_ind)) {
        ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON() failed [emergency_ind]");
        goto end;
    }
    }

    ue_context_in_smf_data_sub_filter_local_var = OpenAPI_ue_context_in_smf_data_sub_filter_create (
        dnn_list ? dnn_listList : NULL,
        snssai_list ? snssai_listList : NULL,
        emergency_ind ? true : false,
        emergency_ind ? emergency_ind->valueint : 0
    );

    return ue_context_in_smf_data_sub_filter_local_var;
end:
    if (dnn_listList) {
        OpenAPI_list_for_each(dnn_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(dnn_listList);
        dnn_listList = NULL;
    }
    if (snssai_listList) {
        OpenAPI_list_for_each(snssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(snssai_listList);
        snssai_listList = NULL;
    }
    return NULL;
}

OpenAPI_ue_context_in_smf_data_sub_filter_t *OpenAPI_ue_context_in_smf_data_sub_filter_copy(OpenAPI_ue_context_in_smf_data_sub_filter_t *dst, OpenAPI_ue_context_in_smf_data_sub_filter_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_context_in_smf_data_sub_filter_convertToJSON() failed");
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

    OpenAPI_ue_context_in_smf_data_sub_filter_free(dst);
    dst = OpenAPI_ue_context_in_smf_data_sub_filter_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

