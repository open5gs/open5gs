
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "adrf_info.h"

OpenAPI_adrf_info_t *OpenAPI_adrf_info_create(
    bool is_ml_model_storage_ind,
    int ml_model_storage_ind,
    bool is_data_storage_ind,
    int data_storage_ind
)
{
    OpenAPI_adrf_info_t *adrf_info_local_var = ogs_malloc(sizeof(OpenAPI_adrf_info_t));
    ogs_assert(adrf_info_local_var);

    adrf_info_local_var->is_ml_model_storage_ind = is_ml_model_storage_ind;
    adrf_info_local_var->ml_model_storage_ind = ml_model_storage_ind;
    adrf_info_local_var->is_data_storage_ind = is_data_storage_ind;
    adrf_info_local_var->data_storage_ind = data_storage_ind;

    return adrf_info_local_var;
}

void OpenAPI_adrf_info_free(OpenAPI_adrf_info_t *adrf_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == adrf_info) {
        return;
    }
    ogs_free(adrf_info);
}

cJSON *OpenAPI_adrf_info_convertToJSON(OpenAPI_adrf_info_t *adrf_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (adrf_info == NULL) {
        ogs_error("OpenAPI_adrf_info_convertToJSON() failed [AdrfInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (adrf_info->is_ml_model_storage_ind) {
    if (cJSON_AddBoolToObject(item, "mlModelStorageInd", adrf_info->ml_model_storage_ind) == NULL) {
        ogs_error("OpenAPI_adrf_info_convertToJSON() failed [ml_model_storage_ind]");
        goto end;
    }
    }

    if (adrf_info->is_data_storage_ind) {
    if (cJSON_AddBoolToObject(item, "dataStorageInd", adrf_info->data_storage_ind) == NULL) {
        ogs_error("OpenAPI_adrf_info_convertToJSON() failed [data_storage_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_adrf_info_t *OpenAPI_adrf_info_parseFromJSON(cJSON *adrf_infoJSON)
{
    OpenAPI_adrf_info_t *adrf_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *ml_model_storage_ind = NULL;
    cJSON *data_storage_ind = NULL;
    ml_model_storage_ind = cJSON_GetObjectItemCaseSensitive(adrf_infoJSON, "mlModelStorageInd");
    if (ml_model_storage_ind) {
    if (!cJSON_IsBool(ml_model_storage_ind)) {
        ogs_error("OpenAPI_adrf_info_parseFromJSON() failed [ml_model_storage_ind]");
        goto end;
    }
    }

    data_storage_ind = cJSON_GetObjectItemCaseSensitive(adrf_infoJSON, "dataStorageInd");
    if (data_storage_ind) {
    if (!cJSON_IsBool(data_storage_ind)) {
        ogs_error("OpenAPI_adrf_info_parseFromJSON() failed [data_storage_ind]");
        goto end;
    }
    }

    adrf_info_local_var = OpenAPI_adrf_info_create (
        ml_model_storage_ind ? true : false,
        ml_model_storage_ind ? ml_model_storage_ind->valueint : 0,
        data_storage_ind ? true : false,
        data_storage_ind ? data_storage_ind->valueint : 0
    );

    return adrf_info_local_var;
end:
    return NULL;
}

OpenAPI_adrf_info_t *OpenAPI_adrf_info_copy(OpenAPI_adrf_info_t *dst, OpenAPI_adrf_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_adrf_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_adrf_info_convertToJSON() failed");
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

    OpenAPI_adrf_info_free(dst);
    dst = OpenAPI_adrf_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

