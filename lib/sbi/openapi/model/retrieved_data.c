
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "retrieved_data.h"

OpenAPI_retrieved_data_t *OpenAPI_retrieved_data_create(
    OpenAPI_small_data_rate_status_t *small_data_rate_status
)
{
    OpenAPI_retrieved_data_t *retrieved_data_local_var = ogs_malloc(sizeof(OpenAPI_retrieved_data_t));
    ogs_assert(retrieved_data_local_var);

    retrieved_data_local_var->small_data_rate_status = small_data_rate_status;

    return retrieved_data_local_var;
}

void OpenAPI_retrieved_data_free(OpenAPI_retrieved_data_t *retrieved_data)
{
    if (NULL == retrieved_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_small_data_rate_status_free(retrieved_data->small_data_rate_status);
    ogs_free(retrieved_data);
}

cJSON *OpenAPI_retrieved_data_convertToJSON(OpenAPI_retrieved_data_t *retrieved_data)
{
    cJSON *item = NULL;

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

end:
    return item;
}

OpenAPI_retrieved_data_t *OpenAPI_retrieved_data_parseFromJSON(cJSON *retrieved_dataJSON)
{
    OpenAPI_retrieved_data_t *retrieved_data_local_var = NULL;
    cJSON *small_data_rate_status = cJSON_GetObjectItemCaseSensitive(retrieved_dataJSON, "smallDataRateStatus");

    OpenAPI_small_data_rate_status_t *small_data_rate_status_local_nonprim = NULL;
    if (small_data_rate_status) {
    small_data_rate_status_local_nonprim = OpenAPI_small_data_rate_status_parseFromJSON(small_data_rate_status);
    }

    retrieved_data_local_var = OpenAPI_retrieved_data_create (
        small_data_rate_status ? small_data_rate_status_local_nonprim : NULL
    );

    return retrieved_data_local_var;
end:
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

