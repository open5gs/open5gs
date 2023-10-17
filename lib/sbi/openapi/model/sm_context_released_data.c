
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_context_released_data.h"

OpenAPI_sm_context_released_data_t *OpenAPI_sm_context_released_data_create(
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status
)
{
    OpenAPI_sm_context_released_data_t *sm_context_released_data_local_var = ogs_malloc(sizeof(OpenAPI_sm_context_released_data_t));
    ogs_assert(sm_context_released_data_local_var);

    sm_context_released_data_local_var->small_data_rate_status = small_data_rate_status;
    sm_context_released_data_local_var->apn_rate_status = apn_rate_status;

    return sm_context_released_data_local_var;
}

void OpenAPI_sm_context_released_data_free(OpenAPI_sm_context_released_data_t *sm_context_released_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sm_context_released_data) {
        return;
    }
    if (sm_context_released_data->small_data_rate_status) {
        OpenAPI_small_data_rate_status_free(sm_context_released_data->small_data_rate_status);
        sm_context_released_data->small_data_rate_status = NULL;
    }
    if (sm_context_released_data->apn_rate_status) {
        OpenAPI_apn_rate_status_free(sm_context_released_data->apn_rate_status);
        sm_context_released_data->apn_rate_status = NULL;
    }
    ogs_free(sm_context_released_data);
}

cJSON *OpenAPI_sm_context_released_data_convertToJSON(OpenAPI_sm_context_released_data_t *sm_context_released_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sm_context_released_data == NULL) {
        ogs_error("OpenAPI_sm_context_released_data_convertToJSON() failed [SmContextReleasedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_context_released_data->small_data_rate_status) {
    cJSON *small_data_rate_status_local_JSON = OpenAPI_small_data_rate_status_convertToJSON(sm_context_released_data->small_data_rate_status);
    if (small_data_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_released_data_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smallDataRateStatus", small_data_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_released_data_convertToJSON() failed [small_data_rate_status]");
        goto end;
    }
    }

    if (sm_context_released_data->apn_rate_status) {
    cJSON *apn_rate_status_local_JSON = OpenAPI_apn_rate_status_convertToJSON(sm_context_released_data->apn_rate_status);
    if (apn_rate_status_local_JSON == NULL) {
        ogs_error("OpenAPI_sm_context_released_data_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    cJSON_AddItemToObject(item, "apnRateStatus", apn_rate_status_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sm_context_released_data_convertToJSON() failed [apn_rate_status]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sm_context_released_data_t *OpenAPI_sm_context_released_data_parseFromJSON(cJSON *sm_context_released_dataJSON)
{
    OpenAPI_sm_context_released_data_t *sm_context_released_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *small_data_rate_status = NULL;
    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    cJSON *apn_rate_status = NULL;
    OpenAPI_apn_rate_status_t *apn_rate_status_local_nonprim = NULL;
    small_data_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_released_dataJSON, "smallDataRateStatus");
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    if (!small_data_rate_status_local_nonprim) {
        ogs_error("OpenAPI_small_data_rate_status_parseFromJSON failed [small_data_rate_status]");
        goto end;
    }
    }

    apn_rate_status = cJSON_GetObjectItemCaseSensitive(sm_context_released_dataJSON, "apnRateStatus");
    if (apn_rate_status) {
    apn_rate_status_local_nonprim = OpenAPI_apn_rate_status_parseFromJSON(apn_rate_status);
    if (!apn_rate_status_local_nonprim) {
        ogs_error("OpenAPI_apn_rate_status_parseFromJSON failed [apn_rate_status]");
        goto end;
    }
    }

    sm_context_released_data_local_var = OpenAPI_sm_context_released_data_create (
        small_data_rate_status ? small_data_rate_status_local_nonprim : NULL,
        apn_rate_status ? apn_rate_status_local_nonprim : NULL
    );

    return sm_context_released_data_local_var;
end:
    if (small_data_rate_status_local_nonprim) {
        OpenAPI_small_data_rate_status_free(small_data_rate_status_local_nonprim);
        small_data_rate_status_local_nonprim = NULL;
    }
    if (apn_rate_status_local_nonprim) {
        OpenAPI_apn_rate_status_free(apn_rate_status_local_nonprim);
        apn_rate_status_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_sm_context_released_data_t *OpenAPI_sm_context_released_data_copy(OpenAPI_sm_context_released_data_t *dst, OpenAPI_sm_context_released_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_context_released_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_context_released_data_convertToJSON() failed");
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

    OpenAPI_sm_context_released_data_free(dst);
    dst = OpenAPI_sm_context_released_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

