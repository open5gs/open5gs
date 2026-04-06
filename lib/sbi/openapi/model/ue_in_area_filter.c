
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_in_area_filter.h"

OpenAPI_ue_in_area_filter_t *OpenAPI_ue_in_area_filter_create(
    OpenAPI_ue_type_e ue_type,
    bool is_aerial_srv_dnn_ind,
    int aerial_srv_dnn_ind,
    bool is_ue_id_omit_ind,
    int ue_id_omit_ind,
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcasting_assistance_data_type
)
{
    OpenAPI_ue_in_area_filter_t *ue_in_area_filter_local_var = ogs_malloc(sizeof(OpenAPI_ue_in_area_filter_t));
    ogs_assert(ue_in_area_filter_local_var);

    ue_in_area_filter_local_var->ue_type = ue_type;
    ue_in_area_filter_local_var->is_aerial_srv_dnn_ind = is_aerial_srv_dnn_ind;
    ue_in_area_filter_local_var->aerial_srv_dnn_ind = aerial_srv_dnn_ind;
    ue_in_area_filter_local_var->is_ue_id_omit_ind = is_ue_id_omit_ind;
    ue_in_area_filter_local_var->ue_id_omit_ind = ue_id_omit_ind;
    ue_in_area_filter_local_var->lcs_broadcasting_assistance_data_type = lcs_broadcasting_assistance_data_type;

    return ue_in_area_filter_local_var;
}

void OpenAPI_ue_in_area_filter_free(OpenAPI_ue_in_area_filter_t *ue_in_area_filter)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_in_area_filter) {
        return;
    }
    if (ue_in_area_filter->lcs_broadcasting_assistance_data_type) {
        OpenAPI_lcs_broadcast_assistance_types_data_free(ue_in_area_filter->lcs_broadcasting_assistance_data_type);
        ue_in_area_filter->lcs_broadcasting_assistance_data_type = NULL;
    }
    ogs_free(ue_in_area_filter);
}

cJSON *OpenAPI_ue_in_area_filter_convertToJSON(OpenAPI_ue_in_area_filter_t *ue_in_area_filter)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_in_area_filter == NULL) {
        ogs_error("OpenAPI_ue_in_area_filter_convertToJSON() failed [UeInAreaFilter]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (ue_in_area_filter->ue_type != OpenAPI_ue_type_NULL) {
    if (cJSON_AddStringToObject(item, "ueType", OpenAPI_ue_type_ToString(ue_in_area_filter->ue_type)) == NULL) {
        ogs_error("OpenAPI_ue_in_area_filter_convertToJSON() failed [ue_type]");
        goto end;
    }
    }

    if (ue_in_area_filter->is_aerial_srv_dnn_ind) {
    if (cJSON_AddBoolToObject(item, "aerialSrvDnnInd", ue_in_area_filter->aerial_srv_dnn_ind) == NULL) {
        ogs_error("OpenAPI_ue_in_area_filter_convertToJSON() failed [aerial_srv_dnn_ind]");
        goto end;
    }
    }

    if (ue_in_area_filter->is_ue_id_omit_ind) {
    if (cJSON_AddBoolToObject(item, "ueIdOmitInd", ue_in_area_filter->ue_id_omit_ind) == NULL) {
        ogs_error("OpenAPI_ue_in_area_filter_convertToJSON() failed [ue_id_omit_ind]");
        goto end;
    }
    }

    if (ue_in_area_filter->lcs_broadcasting_assistance_data_type) {
    cJSON *lcs_broadcasting_assistance_data_type_local_JSON = OpenAPI_lcs_broadcast_assistance_types_data_convertToJSON(ue_in_area_filter->lcs_broadcasting_assistance_data_type);
    if (lcs_broadcasting_assistance_data_type_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_in_area_filter_convertToJSON() failed [lcs_broadcasting_assistance_data_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lcsBroadcastingAssistanceDataType", lcs_broadcasting_assistance_data_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_in_area_filter_convertToJSON() failed [lcs_broadcasting_assistance_data_type]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_in_area_filter_t *OpenAPI_ue_in_area_filter_parseFromJSON(cJSON *ue_in_area_filterJSON)
{
    OpenAPI_ue_in_area_filter_t *ue_in_area_filter_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_type = NULL;
    OpenAPI_ue_type_e ue_typeVariable = 0;
    cJSON *aerial_srv_dnn_ind = NULL;
    cJSON *ue_id_omit_ind = NULL;
    cJSON *lcs_broadcasting_assistance_data_type = NULL;
    OpenAPI_lcs_broadcast_assistance_types_data_t *lcs_broadcasting_assistance_data_type_local_nonprim = NULL;
    ue_type = cJSON_GetObjectItemCaseSensitive(ue_in_area_filterJSON, "ueType");
    if (ue_type) {
    if (!cJSON_IsString(ue_type)) {
        ogs_error("OpenAPI_ue_in_area_filter_parseFromJSON() failed [ue_type]");
        goto end;
    }
    ue_typeVariable = OpenAPI_ue_type_FromString(ue_type->valuestring);
    }

    aerial_srv_dnn_ind = cJSON_GetObjectItemCaseSensitive(ue_in_area_filterJSON, "aerialSrvDnnInd");
    if (aerial_srv_dnn_ind) {
    if (!cJSON_IsBool(aerial_srv_dnn_ind)) {
        ogs_error("OpenAPI_ue_in_area_filter_parseFromJSON() failed [aerial_srv_dnn_ind]");
        goto end;
    }
    }

    ue_id_omit_ind = cJSON_GetObjectItemCaseSensitive(ue_in_area_filterJSON, "ueIdOmitInd");
    if (ue_id_omit_ind) {
    if (!cJSON_IsBool(ue_id_omit_ind)) {
        ogs_error("OpenAPI_ue_in_area_filter_parseFromJSON() failed [ue_id_omit_ind]");
        goto end;
    }
    }

    lcs_broadcasting_assistance_data_type = cJSON_GetObjectItemCaseSensitive(ue_in_area_filterJSON, "lcsBroadcastingAssistanceDataType");
    if (lcs_broadcasting_assistance_data_type) {
    lcs_broadcasting_assistance_data_type_local_nonprim = OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON(lcs_broadcasting_assistance_data_type);
    if (!lcs_broadcasting_assistance_data_type_local_nonprim) {
        ogs_error("OpenAPI_lcs_broadcast_assistance_types_data_parseFromJSON failed [lcs_broadcasting_assistance_data_type]");
        goto end;
    }
    }

    ue_in_area_filter_local_var = OpenAPI_ue_in_area_filter_create (
        ue_type ? ue_typeVariable : 0,
        aerial_srv_dnn_ind ? true : false,
        aerial_srv_dnn_ind ? aerial_srv_dnn_ind->valueint : 0,
        ue_id_omit_ind ? true : false,
        ue_id_omit_ind ? ue_id_omit_ind->valueint : 0,
        lcs_broadcasting_assistance_data_type ? lcs_broadcasting_assistance_data_type_local_nonprim : NULL
    );

    return ue_in_area_filter_local_var;
end:
    if (lcs_broadcasting_assistance_data_type_local_nonprim) {
        OpenAPI_lcs_broadcast_assistance_types_data_free(lcs_broadcasting_assistance_data_type_local_nonprim);
        lcs_broadcasting_assistance_data_type_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_ue_in_area_filter_t *OpenAPI_ue_in_area_filter_copy(OpenAPI_ue_in_area_filter_t *dst, OpenAPI_ue_in_area_filter_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_in_area_filter_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_in_area_filter_convertToJSON() failed");
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

    OpenAPI_ue_in_area_filter_free(dst);
    dst = OpenAPI_ue_in_area_filter_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

