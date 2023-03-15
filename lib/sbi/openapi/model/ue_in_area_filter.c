
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_in_area_filter.h"

OpenAPI_ue_in_area_filter_t *OpenAPI_ue_in_area_filter_create(
    OpenAPI_ue_type_t *ue_type,
    bool is_aerial_srv_dnn_ind,
    int aerial_srv_dnn_ind
)
{
    OpenAPI_ue_in_area_filter_t *ue_in_area_filter_local_var = ogs_malloc(sizeof(OpenAPI_ue_in_area_filter_t));
    ogs_assert(ue_in_area_filter_local_var);

    ue_in_area_filter_local_var->ue_type = ue_type;
    ue_in_area_filter_local_var->is_aerial_srv_dnn_ind = is_aerial_srv_dnn_ind;
    ue_in_area_filter_local_var->aerial_srv_dnn_ind = aerial_srv_dnn_ind;

    return ue_in_area_filter_local_var;
}

void OpenAPI_ue_in_area_filter_free(OpenAPI_ue_in_area_filter_t *ue_in_area_filter)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_in_area_filter) {
        return;
    }
    if (ue_in_area_filter->ue_type) {
        OpenAPI_ue_type_free(ue_in_area_filter->ue_type);
        ue_in_area_filter->ue_type = NULL;
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
    if (ue_in_area_filter->ue_type) {
    cJSON *ue_type_local_JSON = OpenAPI_ue_type_convertToJSON(ue_in_area_filter->ue_type);
    if (ue_type_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_in_area_filter_convertToJSON() failed [ue_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueType", ue_type_local_JSON);
    if (item->child == NULL) {
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

end:
    return item;
}

OpenAPI_ue_in_area_filter_t *OpenAPI_ue_in_area_filter_parseFromJSON(cJSON *ue_in_area_filterJSON)
{
    OpenAPI_ue_in_area_filter_t *ue_in_area_filter_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ue_type = NULL;
    OpenAPI_ue_type_t *ue_type_local_nonprim = NULL;
    cJSON *aerial_srv_dnn_ind = NULL;
    ue_type = cJSON_GetObjectItemCaseSensitive(ue_in_area_filterJSON, "ueType");
    if (ue_type) {
    ue_type_local_nonprim = OpenAPI_ue_type_parseFromJSON(ue_type);
    if (!ue_type_local_nonprim) {
        ogs_error("OpenAPI_ue_type_parseFromJSON failed [ue_type]");
        goto end;
    }
    }

    aerial_srv_dnn_ind = cJSON_GetObjectItemCaseSensitive(ue_in_area_filterJSON, "aerialSrvDnnInd");
    if (aerial_srv_dnn_ind) {
    if (!cJSON_IsBool(aerial_srv_dnn_ind)) {
        ogs_error("OpenAPI_ue_in_area_filter_parseFromJSON() failed [aerial_srv_dnn_ind]");
        goto end;
    }
    }

    ue_in_area_filter_local_var = OpenAPI_ue_in_area_filter_create (
        ue_type ? ue_type_local_nonprim : NULL,
        aerial_srv_dnn_ind ? true : false,
        aerial_srv_dnn_ind ? aerial_srv_dnn_ind->valueint : 0
    );

    return ue_in_area_filter_local_var;
end:
    if (ue_type_local_nonprim) {
        OpenAPI_ue_type_free(ue_type_local_nonprim);
        ue_type_local_nonprim = NULL;
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

