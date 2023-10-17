
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "retrieved_data.h"

OpenAPI_retrieved_data_t *OpenAPI_retrieved_data_create(
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_af_coordination_info_t *af_coordination_info
)
{
    OpenAPI_retrieved_data_t *retrieved_data_local_var = ogs_malloc(sizeof(OpenAPI_retrieved_data_t));
    ogs_assert(retrieved_data_local_var);

    retrieved_data_local_var->small_data_rate_status = small_data_rate_status;
    retrieved_data_local_var->af_coordination_info = af_coordination_info;

    return retrieved_data_local_var;
}

void OpenAPI_retrieved_data_free(OpenAPI_retrieved_data_t *retrieved_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == retrieved_data) {
        return;
    }
    if (retrieved_data->small_data_rate_status) {
        OpenAPI_small_data_rate_status_free(retrieved_data->small_data_rate_status);
        retrieved_data->small_data_rate_status = NULL;
    }
    if (retrieved_data->af_coordination_info) {
        OpenAPI_af_coordination_info_free(retrieved_data->af_coordination_info);
        retrieved_data->af_coordination_info = NULL;
    }
    ogs_free(retrieved_data);
}

cJSON *OpenAPI_retrieved_data_convertToJSON(OpenAPI_retrieved_data_t *retrieved_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (retrieved_data == NULL) {
        ogs_error("OpenAPI_retrieved_data_convertToJSON() failed [RetrievedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (retrieved_data->small_data_rate_status) {
    cJSON *small_data_rate_status_local_JSON = OpenAPI_small_data_rate_status_convertToJSON(retrieved_data->small_data_rate_status);
    if (small_data_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_retrieved_data_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smallDataRateStatus", small_data_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_retrieved_data_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    }

    if (retrieved_data->af_coordination_info) {
    cJSON *af_coordination_info_local_JSON = OpenAPI_af_coordination_info_convertToJSON(retrieved_data->af_coordination_info);
    if (af_coordination_info_local_JSON == NULL) {
        ogs_error("OpenAPI_retrieved_data_convertToJSON() failed [af_coordination_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "afCoordinationInfo", af_coordination_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_retrieved_data_convertToJSON() failed [af_coordination_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_retrieved_data_t *OpenAPI_retrieved_data_parseFromJSON(cJSON *retrieved_dataJSON)
{
    OpenAPI_retrieved_data_t *retrieved_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *small_data_rate_status = NULL;
    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    cJSON *af_coordination_info = NULL;
    OpenAPI_af_coordination_info_t *af_coordination_info_local_nonprim = NULL;
    small_data_rate_status = cJSON_GetObjectItemCaseSensitive(retrieved_dataJSON, "smallDataRateStatus");
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    if (!small_data_rate_status_local_nonprim) {
        ogs_error("OpenAPI_small_data_rate_status_parseFromJSON failed [small_data_rate_status]");
        goto end;
    }
    }

    af_coordination_info = cJSON_GetObjectItemCaseSensitive(retrieved_dataJSON, "afCoordinationInfo");
    if (af_coordination_info) {
    af_coordination_info_local_nonprim = OpenAPI_af_coordination_info_parseFromJSON(af_coordination_info);
    if (!af_coordination_info_local_nonprim) {
        ogs_error("OpenAPI_af_coordination_info_parseFromJSON failed [af_coordination_info]");
        goto end;
    }
    }

    retrieved_data_local_var = OpenAPI_retrieved_data_create (
        small_data_rate_status ? small_data_rate_status_local_nonprim : NULL,
        af_coordination_info ? af_coordination_info_local_nonprim : NULL
    );

    return retrieved_data_local_var;
end:
    if (small_data_rate_status_local_nonprim) {
        OpenAPI_small_data_rate_status_free(small_data_rate_status_local_nonprim);
        small_data_rate_status_local_nonprim = NULL;
    }
    if (af_coordination_info_local_nonprim) {
        OpenAPI_af_coordination_info_free(af_coordination_info_local_nonprim);
        af_coordination_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_retrieved_data_t *OpenAPI_retrieved_data_copy(OpenAPI_retrieved_data_t *dst, OpenAPI_retrieved_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_retrieved_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_retrieved_data_convertToJSON() failed");
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

    OpenAPI_retrieved_data_free(dst);
    dst = OpenAPI_retrieved_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

