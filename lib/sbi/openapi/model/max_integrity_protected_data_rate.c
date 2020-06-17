
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "max_integrity_protected_data_rate.h"

OpenAPI_max_integrity_protected_data_rate_t *OpenAPI_max_integrity_protected_data_rate_create(
    )
{
    OpenAPI_max_integrity_protected_data_rate_t *max_integrity_protected_data_rate_local_var = OpenAPI_malloc(sizeof(OpenAPI_max_integrity_protected_data_rate_t));
    if (!max_integrity_protected_data_rate_local_var) {
        return NULL;
    }

    return max_integrity_protected_data_rate_local_var;
}

void OpenAPI_max_integrity_protected_data_rate_free(OpenAPI_max_integrity_protected_data_rate_t *max_integrity_protected_data_rate)
{
    if (NULL == max_integrity_protected_data_rate) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(max_integrity_protected_data_rate);
}

cJSON *OpenAPI_max_integrity_protected_data_rate_convertToJSON(OpenAPI_max_integrity_protected_data_rate_t *max_integrity_protected_data_rate)
{
    cJSON *item = NULL;

    if (max_integrity_protected_data_rate == NULL) {
        ogs_error("OpenAPI_max_integrity_protected_data_rate_convertToJSON() failed [MaxIntegrityProtectedDataRate]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_max_integrity_protected_data_rate_t *OpenAPI_max_integrity_protected_data_rate_parseFromJSON(cJSON *max_integrity_protected_data_rateJSON)
{
    OpenAPI_max_integrity_protected_data_rate_t *max_integrity_protected_data_rate_local_var = NULL;
    max_integrity_protected_data_rate_local_var = OpenAPI_max_integrity_protected_data_rate_create (
        );

    return max_integrity_protected_data_rate_local_var;
end:
    return NULL;
}

OpenAPI_max_integrity_protected_data_rate_t *OpenAPI_max_integrity_protected_data_rate_copy(OpenAPI_max_integrity_protected_data_rate_t *dst, OpenAPI_max_integrity_protected_data_rate_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_max_integrity_protected_data_rate_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_max_integrity_protected_data_rate_convertToJSON() failed");
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

    OpenAPI_max_integrity_protected_data_rate_free(dst);
    dst = OpenAPI_max_integrity_protected_data_rate_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

