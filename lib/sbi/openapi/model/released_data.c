
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "released_data.h"

OpenAPI_released_data_t *OpenAPI_released_data_create(
    OpenAPI_small_data_rate_status_t *small_data_rate_status,
    OpenAPI_apn_rate_status_t *apn_rate_status
    )
{
    OpenAPI_released_data_t *released_data_local_var = OpenAPI_malloc(sizeof(OpenAPI_released_data_t));
    if (!released_data_local_var) {
        return NULL;
    }
    released_data_local_var->small_data_rate_status = small_data_rate_status;
    released_data_local_var->apn_rate_status = apn_rate_status;

    return released_data_local_var;
}

void OpenAPI_released_data_free(OpenAPI_released_data_t *released_data)
{
    if (NULL == released_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_small_data_rate_status_free(released_data->small_data_rate_status);
    OpenAPI_apn_rate_status_free(released_data->apn_rate_status);
    ogs_free(released_data);
}

cJSON *OpenAPI_released_data_convertToJSON(OpenAPI_released_data_t *released_data)
{
    cJSON *item = NULL;

    if (released_data == NULL) {
        ogs_error("OpenAPI_released_data_convertToJSON() failed [ReleasedData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (released_data->small_data_rate_status) {
        cJSON *small_data_rate_status_local_JSON = OpenAPI_small_data_rate_status_convertToJSON(released_data->small_data_rate_status);
        if (small_data_rate_status_local_JSON == NULL) {
            ogs_error("OpenAPI_released_data_convertToJSON() failed [small_data_rate_status]");
            goto end;
        }
        cJSON_AddItemToObject(item, "smallDataRateStatus", small_data_rate_status_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_released_data_convertToJSON() failed [small_data_rate_status]");
            goto end;
        }
    }

    if (released_data->apn_rate_status) {
        cJSON *apn_rate_status_local_JSON = OpenAPI_apn_rate_status_convertToJSON(released_data->apn_rate_status);
        if (apn_rate_status_local_JSON == NULL) {
            ogs_error("OpenAPI_released_data_convertToJSON() failed [apn_rate_status]");
            goto end;
        }
        cJSON_AddItemToObject(item, "apnRateStatus", apn_rate_status_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_released_data_convertToJSON() failed [apn_rate_status]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_released_data_t *OpenAPI_released_data_parseFromJSON(cJSON *released_dataJSON)
{
    OpenAPI_released_data_t *released_data_local_var = NULL;
    cJSON *small_data_rate_status = cJSON_GetObjectItemCaseSensitive(released_dataJSON, "smallDataRateStatus");

    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    if (small_data_rate_status) {
        small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    }

    cJSON *apn_rate_status = cJSON_GetObjectItemCaseSensitive(released_dataJSON, "apnRateStatus");

    OpenAPI_apn_rate_status_t *apn_rate_status_local_nonprim = NULL;
    if (apn_rate_status) {
        apn_rate_status_local_nonprim = OpenAPI_apn_rate_status_parseFromJSON(apn_rate_status);
    }

    released_data_local_var = OpenAPI_released_data_create (
        small_data_rate_status ? small_data_rate_status_local_nonprim : NULL,
        apn_rate_status ? apn_rate_status_local_nonprim : NULL
        );

    return released_data_local_var;
end:
    return NULL;
}

OpenAPI_released_data_t *OpenAPI_released_data_copy(OpenAPI_released_data_t *dst, OpenAPI_released_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_released_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_released_data_convertToJSON() failed");
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

    OpenAPI_released_data_free(dst);
    dst = OpenAPI_released_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

