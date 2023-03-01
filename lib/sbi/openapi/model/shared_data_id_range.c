
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shared_data_id_range.h"

OpenAPI_shared_data_id_range_t *OpenAPI_shared_data_id_range_create(
    char *pattern
)
{
    OpenAPI_shared_data_id_range_t *shared_data_id_range_local_var = ogs_malloc(sizeof(OpenAPI_shared_data_id_range_t));
    ogs_assert(shared_data_id_range_local_var);

    shared_data_id_range_local_var->pattern = pattern;

    return shared_data_id_range_local_var;
}

void OpenAPI_shared_data_id_range_free(OpenAPI_shared_data_id_range_t *shared_data_id_range)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == shared_data_id_range) {
        return;
    }
    if (shared_data_id_range->pattern) {
        ogs_free(shared_data_id_range->pattern);
        shared_data_id_range->pattern = NULL;
    }
    ogs_free(shared_data_id_range);
}

cJSON *OpenAPI_shared_data_id_range_convertToJSON(OpenAPI_shared_data_id_range_t *shared_data_id_range)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (shared_data_id_range == NULL) {
        ogs_error("OpenAPI_shared_data_id_range_convertToJSON() failed [SharedDataIdRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (shared_data_id_range->pattern) {
    if (cJSON_AddStringToObject(item, "pattern", shared_data_id_range->pattern) == NULL) {
        ogs_error("OpenAPI_shared_data_id_range_convertToJSON() failed [pattern]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_shared_data_id_range_t *OpenAPI_shared_data_id_range_parseFromJSON(cJSON *shared_data_id_rangeJSON)
{
    OpenAPI_shared_data_id_range_t *shared_data_id_range_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *pattern = NULL;
    pattern = cJSON_GetObjectItemCaseSensitive(shared_data_id_rangeJSON, "pattern");
    if (pattern) {
    if (!cJSON_IsString(pattern) && !cJSON_IsNull(pattern)) {
        ogs_error("OpenAPI_shared_data_id_range_parseFromJSON() failed [pattern]");
        goto end;
    }
    }

    shared_data_id_range_local_var = OpenAPI_shared_data_id_range_create (
        pattern && !cJSON_IsNull(pattern) ? ogs_strdup(pattern->valuestring) : NULL
    );

    return shared_data_id_range_local_var;
end:
    return NULL;
}

OpenAPI_shared_data_id_range_t *OpenAPI_shared_data_id_range_copy(OpenAPI_shared_data_id_range_t *dst, OpenAPI_shared_data_id_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_shared_data_id_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_shared_data_id_range_convertToJSON() failed");
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

    OpenAPI_shared_data_id_range_free(dst);
    dst = OpenAPI_shared_data_id_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

