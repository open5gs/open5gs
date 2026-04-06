
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "lmf_info.h"

OpenAPI_lmf_info_t *OpenAPI_lmf_info_create(
    OpenAPI_list_t *serving_client_types,
    char *lmf_id,
    OpenAPI_list_t *serving_access_types,
    OpenAPI_list_t *serving_an_node_types,
    OpenAPI_list_t *serving_rat_types,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *tai_range_list,
    OpenAPI_list_t *supported_gad_shapes,
    OpenAPI_pru_existence_info_t *pru_existence_info,
    bool is_pru_support_ind,
    int pru_support_ind,
    bool is_rangingslpos_support_ind,
    int rangingslpos_support_ind,
    bool is_up_positioning_ind,
    int up_positioning_ind,
    bool is_aiml_pos_ind,
    int aiml_pos_ind
)
{
    OpenAPI_lmf_info_t *lmf_info_local_var = ogs_malloc(sizeof(OpenAPI_lmf_info_t));
    ogs_assert(lmf_info_local_var);

    lmf_info_local_var->serving_client_types = serving_client_types;
    lmf_info_local_var->lmf_id = lmf_id;
    lmf_info_local_var->serving_access_types = serving_access_types;
    lmf_info_local_var->serving_an_node_types = serving_an_node_types;
    lmf_info_local_var->serving_rat_types = serving_rat_types;
    lmf_info_local_var->tai_list = tai_list;
    lmf_info_local_var->tai_range_list = tai_range_list;
    lmf_info_local_var->supported_gad_shapes = supported_gad_shapes;
    lmf_info_local_var->pru_existence_info = pru_existence_info;
    lmf_info_local_var->is_pru_support_ind = is_pru_support_ind;
    lmf_info_local_var->pru_support_ind = pru_support_ind;
    lmf_info_local_var->is_rangingslpos_support_ind = is_rangingslpos_support_ind;
    lmf_info_local_var->rangingslpos_support_ind = rangingslpos_support_ind;
    lmf_info_local_var->is_up_positioning_ind = is_up_positioning_ind;
    lmf_info_local_var->up_positioning_ind = up_positioning_ind;
    lmf_info_local_var->is_aiml_pos_ind = is_aiml_pos_ind;
    lmf_info_local_var->aiml_pos_ind = aiml_pos_ind;

    return lmf_info_local_var;
}

void OpenAPI_lmf_info_free(OpenAPI_lmf_info_t *lmf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == lmf_info) {
        return;
    }
    if (lmf_info->serving_client_types) {
        OpenAPI_list_free(lmf_info->serving_client_types);
        lmf_info->serving_client_types = NULL;
    }
    if (lmf_info->lmf_id) {
        ogs_free(lmf_info->lmf_id);
        lmf_info->lmf_id = NULL;
    }
    if (lmf_info->serving_access_types) {
        OpenAPI_list_free(lmf_info->serving_access_types);
        lmf_info->serving_access_types = NULL;
    }
    if (lmf_info->serving_an_node_types) {
        OpenAPI_list_free(lmf_info->serving_an_node_types);
        lmf_info->serving_an_node_types = NULL;
    }
    if (lmf_info->serving_rat_types) {
        OpenAPI_list_free(lmf_info->serving_rat_types);
        lmf_info->serving_rat_types = NULL;
    }
    if (lmf_info->tai_list) {
        OpenAPI_list_for_each(lmf_info->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(lmf_info->tai_list);
        lmf_info->tai_list = NULL;
    }
    if (lmf_info->tai_range_list) {
        OpenAPI_list_for_each(lmf_info->tai_range_list, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(lmf_info->tai_range_list);
        lmf_info->tai_range_list = NULL;
    }
    if (lmf_info->supported_gad_shapes) {
        OpenAPI_list_free(lmf_info->supported_gad_shapes);
        lmf_info->supported_gad_shapes = NULL;
    }
    if (lmf_info->pru_existence_info) {
        OpenAPI_pru_existence_info_free(lmf_info->pru_existence_info);
        lmf_info->pru_existence_info = NULL;
    }
    ogs_free(lmf_info);
}

cJSON *OpenAPI_lmf_info_convertToJSON(OpenAPI_lmf_info_t *lmf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (lmf_info == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [LmfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (lmf_info->serving_client_types != OpenAPI_external_client_type_NULL) {
    cJSON *serving_client_typesList = cJSON_AddArrayToObject(item, "servingClientTypes");
    if (serving_client_typesList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_client_types]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->serving_client_types, node) {
        if (cJSON_AddStringToObject(serving_client_typesList, "", OpenAPI_external_client_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_client_types]");
            goto end;
        }
    }
    }

    if (lmf_info->lmf_id) {
    if (cJSON_AddStringToObject(item, "lmfId", lmf_info->lmf_id) == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [lmf_id]");
        goto end;
    }
    }

    if (lmf_info->serving_access_types != OpenAPI_access_type_NULL) {
    cJSON *serving_access_typesList = cJSON_AddArrayToObject(item, "servingAccessTypes");
    if (serving_access_typesList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_access_types]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->serving_access_types, node) {
        if (cJSON_AddStringToObject(serving_access_typesList, "", OpenAPI_access_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_access_types]");
            goto end;
        }
    }
    }

    if (lmf_info->serving_an_node_types != OpenAPI_an_node_type_NULL) {
    cJSON *serving_an_node_typesList = cJSON_AddArrayToObject(item, "servingAnNodeTypes");
    if (serving_an_node_typesList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_an_node_types]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->serving_an_node_types, node) {
        if (cJSON_AddStringToObject(serving_an_node_typesList, "", OpenAPI_an_node_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_an_node_types]");
            goto end;
        }
    }
    }

    if (lmf_info->serving_rat_types != OpenAPI_rat_type_NULL) {
    cJSON *serving_rat_typesList = cJSON_AddArrayToObject(item, "servingRatTypes");
    if (serving_rat_typesList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_rat_types]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->serving_rat_types, node) {
        if (cJSON_AddStringToObject(serving_rat_typesList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [serving_rat_types]");
            goto end;
        }
    }
    }

    if (lmf_info->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (lmf_info->tai_range_list) {
    cJSON *tai_range_listList = cJSON_AddArrayToObject(item, "taiRangeList");
    if (tai_range_listList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [tai_range_list]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->tai_range_list, node) {
        cJSON *itemLocal = OpenAPI_tai_range_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [tai_range_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_range_listList, itemLocal);
    }
    }

    if (lmf_info->supported_gad_shapes != OpenAPI_supported_gad_shapes_NULL) {
    cJSON *supported_gad_shapesList = cJSON_AddArrayToObject(item, "supportedGADShapes");
    if (supported_gad_shapesList == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [supported_gad_shapes]");
        goto end;
    }
    OpenAPI_list_for_each(lmf_info->supported_gad_shapes, node) {
        if (cJSON_AddStringToObject(supported_gad_shapesList, "", OpenAPI_supported_gad_shapes_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_lmf_info_convertToJSON() failed [supported_gad_shapes]");
            goto end;
        }
    }
    }

    if (lmf_info->pru_existence_info) {
    cJSON *pru_existence_info_local_JSON = OpenAPI_pru_existence_info_convertToJSON(lmf_info->pru_existence_info);
    if (pru_existence_info_local_JSON == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [pru_existence_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pruExistenceInfo", pru_existence_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [pru_existence_info]");
        goto end;
    }
    }

    if (lmf_info->is_pru_support_ind) {
    if (cJSON_AddBoolToObject(item, "pruSupportInd", lmf_info->pru_support_ind) == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [pru_support_ind]");
        goto end;
    }
    }

    if (lmf_info->is_rangingslpos_support_ind) {
    if (cJSON_AddBoolToObject(item, "rangingslposSupportInd", lmf_info->rangingslpos_support_ind) == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [rangingslpos_support_ind]");
        goto end;
    }
    }

    if (lmf_info->is_up_positioning_ind) {
    if (cJSON_AddBoolToObject(item, "upPositioningInd", lmf_info->up_positioning_ind) == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [up_positioning_ind]");
        goto end;
    }
    }

    if (lmf_info->is_aiml_pos_ind) {
    if (cJSON_AddBoolToObject(item, "aimlPosInd", lmf_info->aiml_pos_ind) == NULL) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed [aiml_pos_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_lmf_info_t *OpenAPI_lmf_info_parseFromJSON(cJSON *lmf_infoJSON)
{
    OpenAPI_lmf_info_t *lmf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *serving_client_types = NULL;
    OpenAPI_list_t *serving_client_typesList = NULL;
    cJSON *lmf_id = NULL;
    cJSON *serving_access_types = NULL;
    OpenAPI_list_t *serving_access_typesList = NULL;
    cJSON *serving_an_node_types = NULL;
    OpenAPI_list_t *serving_an_node_typesList = NULL;
    cJSON *serving_rat_types = NULL;
    OpenAPI_list_t *serving_rat_typesList = NULL;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *tai_range_list = NULL;
    OpenAPI_list_t *tai_range_listList = NULL;
    cJSON *supported_gad_shapes = NULL;
    OpenAPI_list_t *supported_gad_shapesList = NULL;
    cJSON *pru_existence_info = NULL;
    OpenAPI_pru_existence_info_t *pru_existence_info_local_nonprim = NULL;
    cJSON *pru_support_ind = NULL;
    cJSON *rangingslpos_support_ind = NULL;
    cJSON *up_positioning_ind = NULL;
    cJSON *aiml_pos_ind = NULL;
    serving_client_types = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "servingClientTypes");
    if (serving_client_types) {
        cJSON *serving_client_types_local = NULL;
        if (!cJSON_IsArray(serving_client_types)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_client_types]");
            goto end;
        }

        serving_client_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_client_types_local, serving_client_types) {
            OpenAPI_external_client_type_e localEnum = OpenAPI_external_client_type_NULL;
            if (!cJSON_IsString(serving_client_types_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_client_types]");
                goto end;
            }
            localEnum = OpenAPI_external_client_type_FromString(serving_client_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"serving_client_types\" is not supported. Ignoring it ...",
                         serving_client_types_local->valuestring);
            } else {
                OpenAPI_list_add(serving_client_typesList, (void *)localEnum);
            }
        }
        if (serving_client_typesList->count == 0) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed: Expected serving_client_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    lmf_id = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "lmfId");
    if (lmf_id) {
    if (!cJSON_IsString(lmf_id) && !cJSON_IsNull(lmf_id)) {
        ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [lmf_id]");
        goto end;
    }
    }

    serving_access_types = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "servingAccessTypes");
    if (serving_access_types) {
        cJSON *serving_access_types_local = NULL;
        if (!cJSON_IsArray(serving_access_types)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_access_types]");
            goto end;
        }

        serving_access_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_access_types_local, serving_access_types) {
            OpenAPI_access_type_e localEnum = OpenAPI_access_type_NULL;
            if (!cJSON_IsString(serving_access_types_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_access_types]");
                goto end;
            }
            localEnum = OpenAPI_access_type_FromString(serving_access_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"serving_access_types\" is not supported. Ignoring it ...",
                         serving_access_types_local->valuestring);
            } else {
                OpenAPI_list_add(serving_access_typesList, (void *)localEnum);
            }
        }
        if (serving_access_typesList->count == 0) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed: Expected serving_access_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    serving_an_node_types = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "servingAnNodeTypes");
    if (serving_an_node_types) {
        cJSON *serving_an_node_types_local = NULL;
        if (!cJSON_IsArray(serving_an_node_types)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_an_node_types]");
            goto end;
        }

        serving_an_node_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_an_node_types_local, serving_an_node_types) {
            OpenAPI_an_node_type_e localEnum = OpenAPI_an_node_type_NULL;
            if (!cJSON_IsString(serving_an_node_types_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_an_node_types]");
                goto end;
            }
            localEnum = OpenAPI_an_node_type_FromString(serving_an_node_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"serving_an_node_types\" is not supported. Ignoring it ...",
                         serving_an_node_types_local->valuestring);
            } else {
                OpenAPI_list_add(serving_an_node_typesList, (void *)localEnum);
            }
        }
        if (serving_an_node_typesList->count == 0) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed: Expected serving_an_node_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    serving_rat_types = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "servingRatTypes");
    if (serving_rat_types) {
        cJSON *serving_rat_types_local = NULL;
        if (!cJSON_IsArray(serving_rat_types)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_rat_types]");
            goto end;
        }

        serving_rat_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(serving_rat_types_local, serving_rat_types) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(serving_rat_types_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [serving_rat_types]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(serving_rat_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"serving_rat_types\" is not supported. Ignoring it ...",
                         serving_rat_types_local->valuestring);
            } else {
                OpenAPI_list_add(serving_rat_typesList, (void *)localEnum);
            }
        }
        if (serving_rat_typesList->count == 0) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed: Expected serving_rat_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    tai_range_list = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "taiRangeList");
    if (tai_range_list) {
        cJSON *tai_range_list_local = NULL;
        if (!cJSON_IsArray(tai_range_list)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [tai_range_list]");
            goto end;
        }

        tai_range_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_range_list_local, tai_range_list) {
            if (!cJSON_IsObject(tai_range_list_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [tai_range_list]");
                goto end;
            }
            OpenAPI_tai_range_t *tai_range_listItem = OpenAPI_tai_range_parseFromJSON(tai_range_list_local);
            if (!tai_range_listItem) {
                ogs_error("No tai_range_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_range_listList, tai_range_listItem);
        }
    }

    supported_gad_shapes = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "supportedGADShapes");
    if (supported_gad_shapes) {
        cJSON *supported_gad_shapes_local = NULL;
        if (!cJSON_IsArray(supported_gad_shapes)) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [supported_gad_shapes]");
            goto end;
        }

        supported_gad_shapesList = OpenAPI_list_create();

        cJSON_ArrayForEach(supported_gad_shapes_local, supported_gad_shapes) {
            OpenAPI_supported_gad_shapes_e localEnum = OpenAPI_supported_gad_shapes_NULL;
            if (!cJSON_IsString(supported_gad_shapes_local)) {
                ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [supported_gad_shapes]");
                goto end;
            }
            localEnum = OpenAPI_supported_gad_shapes_FromString(supported_gad_shapes_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"supported_gad_shapes\" is not supported. Ignoring it ...",
                         supported_gad_shapes_local->valuestring);
            } else {
                OpenAPI_list_add(supported_gad_shapesList, (void *)localEnum);
            }
        }
        if (supported_gad_shapesList->count == 0) {
            ogs_error("OpenAPI_lmf_info_parseFromJSON() failed: Expected supported_gad_shapesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    pru_existence_info = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "pruExistenceInfo");
    if (pru_existence_info) {
    pru_existence_info_local_nonprim = OpenAPI_pru_existence_info_parseFromJSON(pru_existence_info);
    if (!pru_existence_info_local_nonprim) {
        ogs_error("OpenAPI_pru_existence_info_parseFromJSON failed [pru_existence_info]");
        goto end;
    }
    }

    pru_support_ind = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "pruSupportInd");
    if (pru_support_ind) {
    if (!cJSON_IsBool(pru_support_ind)) {
        ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [pru_support_ind]");
        goto end;
    }
    }

    rangingslpos_support_ind = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "rangingslposSupportInd");
    if (rangingslpos_support_ind) {
    if (!cJSON_IsBool(rangingslpos_support_ind)) {
        ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [rangingslpos_support_ind]");
        goto end;
    }
    }

    up_positioning_ind = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "upPositioningInd");
    if (up_positioning_ind) {
    if (!cJSON_IsBool(up_positioning_ind)) {
        ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [up_positioning_ind]");
        goto end;
    }
    }

    aiml_pos_ind = cJSON_GetObjectItemCaseSensitive(lmf_infoJSON, "aimlPosInd");
    if (aiml_pos_ind) {
    if (!cJSON_IsBool(aiml_pos_ind)) {
        ogs_error("OpenAPI_lmf_info_parseFromJSON() failed [aiml_pos_ind]");
        goto end;
    }
    }

    lmf_info_local_var = OpenAPI_lmf_info_create (
        serving_client_types ? serving_client_typesList : NULL,
        lmf_id && !cJSON_IsNull(lmf_id) ? ogs_strdup(lmf_id->valuestring) : NULL,
        serving_access_types ? serving_access_typesList : NULL,
        serving_an_node_types ? serving_an_node_typesList : NULL,
        serving_rat_types ? serving_rat_typesList : NULL,
        tai_list ? tai_listList : NULL,
        tai_range_list ? tai_range_listList : NULL,
        supported_gad_shapes ? supported_gad_shapesList : NULL,
        pru_existence_info ? pru_existence_info_local_nonprim : NULL,
        pru_support_ind ? true : false,
        pru_support_ind ? pru_support_ind->valueint : 0,
        rangingslpos_support_ind ? true : false,
        rangingslpos_support_ind ? rangingslpos_support_ind->valueint : 0,
        up_positioning_ind ? true : false,
        up_positioning_ind ? up_positioning_ind->valueint : 0,
        aiml_pos_ind ? true : false,
        aiml_pos_ind ? aiml_pos_ind->valueint : 0
    );

    return lmf_info_local_var;
end:
    if (serving_client_typesList) {
        OpenAPI_list_free(serving_client_typesList);
        serving_client_typesList = NULL;
    }
    if (serving_access_typesList) {
        OpenAPI_list_free(serving_access_typesList);
        serving_access_typesList = NULL;
    }
    if (serving_an_node_typesList) {
        OpenAPI_list_free(serving_an_node_typesList);
        serving_an_node_typesList = NULL;
    }
    if (serving_rat_typesList) {
        OpenAPI_list_free(serving_rat_typesList);
        serving_rat_typesList = NULL;
    }
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (tai_range_listList) {
        OpenAPI_list_for_each(tai_range_listList, node) {
            OpenAPI_tai_range_free(node->data);
        }
        OpenAPI_list_free(tai_range_listList);
        tai_range_listList = NULL;
    }
    if (supported_gad_shapesList) {
        OpenAPI_list_free(supported_gad_shapesList);
        supported_gad_shapesList = NULL;
    }
    if (pru_existence_info_local_nonprim) {
        OpenAPI_pru_existence_info_free(pru_existence_info_local_nonprim);
        pru_existence_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_lmf_info_t *OpenAPI_lmf_info_copy(OpenAPI_lmf_info_t *dst, OpenAPI_lmf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_lmf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_lmf_info_convertToJSON() failed");
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

    OpenAPI_lmf_info_free(dst);
    dst = OpenAPI_lmf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

