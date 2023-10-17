
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_snssai_data.h"

OpenAPI_sm_policy_snssai_data_t *OpenAPI_sm_policy_snssai_data_create(
    OpenAPI_snssai_t *snssai,
    OpenAPI_list_t* sm_policy_dnn_data,
    OpenAPI_slice_mbr_1_t *ue_slice_mbr
)
{
    OpenAPI_sm_policy_snssai_data_t *sm_policy_snssai_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_policy_snssai_data_t));
    ogs_assert(sm_policy_snssai_data_local_var);

    sm_policy_snssai_data_local_var->snssai = snssai;
    sm_policy_snssai_data_local_var->sm_policy_dnn_data = sm_policy_dnn_data;
    sm_policy_snssai_data_local_var->ue_slice_mbr = ue_slice_mbr;

    return sm_policy_snssai_data_local_var;
}

void OpenAPI_sm_policy_snssai_data_free(OpenAPI_sm_policy_snssai_data_t *sm_policy_snssai_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_policy_snssai_data) {
        return;
    }
    if (sm_policy_snssai_data->snssai) {
        OpenAPI_snssai_free(sm_policy_snssai_data->snssai);
        sm_policy_snssai_data->snssai = NULL;
    }
    if (sm_policy_snssai_data->sm_policy_dnn_data) {
        OpenAPI_list_for_each(sm_policy_snssai_data->sm_policy_dnn_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_sm_policy_dnn_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_snssai_data->sm_policy_dnn_data);
        sm_policy_snssai_data->sm_policy_dnn_data = NULL;
    }
    if (sm_policy_snssai_data->ue_slice_mbr) {
        OpenAPI_slice_mbr_1_free(sm_policy_snssai_data->ue_slice_mbr);
        sm_policy_snssai_data->ue_slice_mbr = NULL;
    }
    ogs_free(sm_policy_snssai_data);
}

cJSON *OpenAPI_sm_policy_snssai_data_convertToJSON(OpenAPI_sm_policy_snssai_data_t *sm_policy_snssai_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_policy_snssai_data == NULL) {
        ogs_error("OpenAPI_sm_policy_snssai_data_convertToJSON() failed [SmPolicySnssaiData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sm_policy_snssai_data->snssai) {
        ogs_error("OpenAPI_sm_policy_snssai_data_convertToJSON() failed [snssai]");
        return NULL;
    }
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(sm_policy_snssai_data->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_snssai_data_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_snssai_data_convertToJSON() failed [snssai]");
        goto end;
    }

    if (sm_policy_snssai_data->sm_policy_dnn_data) {
    cJSON *sm_policy_dnn_data = cJSON_AddObjectToObject(item, "smPolicyDnnData");
    if (sm_policy_dnn_data == NULL) {
        ogs_error("OpenAPI_sm_policy_snssai_data_convertToJSON() failed [sm_policy_dnn_data]");
        goto end;
    }
    cJSON *localMapObject = sm_policy_dnn_data;
    if (sm_policy_snssai_data->sm_policy_dnn_data) {
        OpenAPI_list_for_each(sm_policy_snssai_data->sm_policy_dnn_data, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_sm_policy_snssai_data_convertToJSON() failed [sm_policy_dnn_data]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_sm_policy_snssai_data_convertToJSON() failed [sm_policy_dnn_data]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_sm_policy_dnn_data_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_sm_policy_snssai_data_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (sm_policy_snssai_data->ue_slice_mbr) {
    cJSON *ue_slice_mbr_local_JSON = OpenAPI_slice_mbr_1_convertToJSON(sm_policy_snssai_data->ue_slice_mbr);
    if (ue_slice_mbr_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_policy_snssai_data_convertToJSON() failed [ue_slice_mbr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueSliceMbr", ue_slice_mbr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_policy_snssai_data_convertToJSON() failed [ue_slice_mbr]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_policy_snssai_data_t *OpenAPI_sm_policy_snssai_data_parseFromJSON(cJSON *sm_policy_snssai_dataJSON)
{
    OpenAPI_sm_policy_snssai_data_t *sm_policy_snssai_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *sm_policy_dnn_data = NULL;
    OpenAPI_list_t *sm_policy_dnn_dataList = NULL;
    cJSON *ue_slice_mbr = NULL;
    OpenAPI_slice_mbr_1_t *ue_slice_mbr_local_nonprim = NULL;
    snssai = cJSON_GetObjectItemCaseSensitive(sm_policy_snssai_dataJSON, "snssai");
    if (!snssai) {
        ogs_error("OpenAPI_sm_policy_snssai_data_parseFromJSON() failed [snssai]");
        goto end;
    }
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }

    sm_policy_dnn_data = cJSON_GetObjectItemCaseSensitive(sm_policy_snssai_dataJSON, "smPolicyDnnData");
    if (sm_policy_dnn_data) {
        cJSON *sm_policy_dnn_data_local_map = NULL;
        if (!cJSON_IsObject(sm_policy_dnn_data) && !cJSON_IsNull(sm_policy_dnn_data)) {
            ogs_error("OpenAPI_sm_policy_snssai_data_parseFromJSON() failed [sm_policy_dnn_data]");
            goto end;
        }
        if (cJSON_IsObject(sm_policy_dnn_data)) {
            sm_policy_dnn_dataList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(sm_policy_dnn_data_local_map, sm_policy_dnn_data) {
                cJSON *localMapObject = sm_policy_dnn_data_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_sm_policy_dnn_data_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_sm_policy_snssai_data_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(sm_policy_dnn_dataList, localMapKeyPair);
            }
        }
    }

    ue_slice_mbr = cJSON_GetObjectItemCaseSensitive(sm_policy_snssai_dataJSON, "ueSliceMbr");
    if (ue_slice_mbr) {
    ue_slice_mbr_local_nonprim = OpenAPI_slice_mbr_1_parseFromJSON(ue_slice_mbr);
    if (!ue_slice_mbr_local_nonprim) {
        ogs_error("OpenAPI_slice_mbr_1_parseFromJSON failed [ue_slice_mbr]");
        goto end;
    }
    }

    sm_policy_snssai_data_local_var = OpenAPI_sm_policy_snssai_data_create (
        snssai_local_nonprim,
        sm_policy_dnn_data ? sm_policy_dnn_dataList : NULL,
        ue_slice_mbr ? ue_slice_mbr_local_nonprim : NULL
    );

    return sm_policy_snssai_data_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (sm_policy_dnn_dataList) {
        OpenAPI_list_for_each(sm_policy_dnn_dataList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_sm_policy_dnn_data_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(sm_policy_dnn_dataList);
        sm_policy_dnn_dataList = NULL;
    }
    if (ue_slice_mbr_local_nonprim) {
        OpenAPI_slice_mbr_1_free(ue_slice_mbr_local_nonprim);
        ue_slice_mbr_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sm_policy_snssai_data_t *OpenAPI_sm_policy_snssai_data_copy(OpenAPI_sm_policy_snssai_data_t *dst, OpenAPI_sm_policy_snssai_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_snssai_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_snssai_data_convertToJSON() failed");
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

    OpenAPI_sm_policy_snssai_data_free(dst);
    dst = OpenAPI_sm_policy_snssai_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

