
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "internal_group_id_range.h"

OpenAPI_internal_group_id_range_t *OpenAPI_internal_group_id_range_create(
    char *start,
    char *end,
    char *pattern
)
{
    OpenAPI_internal_group_id_range_t *internal_group_id_range_local_var = ogs_malloc(sizeof(OpenAPI_internal_group_id_range_t));
    ogs_assert(internal_group_id_range_local_var);

    internal_group_id_range_local_var->start = start;
    internal_group_id_range_local_var->end = end;
    internal_group_id_range_local_var->pattern = pattern;

    return internal_group_id_range_local_var;
}

void OpenAPI_internal_group_id_range_free(OpenAPI_internal_group_id_range_t *internal_group_id_range)
{
    if (NULL == internal_group_id_range) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(internal_group_id_range->start);
    ogs_free(internal_group_id_range->end);
    ogs_free(internal_group_id_range->pattern);
    ogs_free(internal_group_id_range);
}

cJSON *OpenAPI_internal_group_id_range_convertToJSON(OpenAPI_internal_group_id_range_t *internal_group_id_range)
{
    cJSON *item = NULL;

    if (internal_group_id_range == NULL) {
        ogs_error("OpenAPI_internal_group_id_range_convertToJSON() failed [InternalGroupIdRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (internal_group_id_range->start) {
    if (cJSON_AddStringToObject(item, "start", internal_group_id_range->start) == NULL) {
        ogs_error("OpenAPI_internal_group_id_range_convertToJSON() failed [start]");
        goto end;
    }
    }

    if (internal_group_id_range->end) {
    if (cJSON_AddStringToObject(item, "end", internal_group_id_range->end) == NULL) {
        ogs_error("OpenAPI_internal_group_id_range_convertToJSON() failed [end]");
        goto end;
    }
    }

    if (internal_group_id_range->pattern) {
    if (cJSON_AddStringToObject(item, "pattern", internal_group_id_range->pattern) == NULL) {
        ogs_error("OpenAPI_internal_group_id_range_convertToJSON() failed [pattern]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_internal_group_id_range_t *OpenAPI_internal_group_id_range_parseFromJSON(cJSON *internal_group_id_rangeJSON)
{
    OpenAPI_internal_group_id_range_t *internal_group_id_range_local_var = NULL;
    cJSON *start = cJSON_GetObjectItemCaseSensitive(internal_group_id_rangeJSON, "start");

    if (start) {
    if (!cJSON_IsString(start)) {
        ogs_error("OpenAPI_internal_group_id_range_parseFromJSON() failed [start]");
        goto end;
    }
    }

    cJSON *end = cJSON_GetObjectItemCaseSensitive(internal_group_id_rangeJSON, "end");

    if (end) {
    if (!cJSON_IsString(end)) {
        ogs_error("OpenAPI_internal_group_id_range_parseFromJSON() failed [end]");
        goto end;
    }
    }

    cJSON *pattern = cJSON_GetObjectItemCaseSensitive(internal_group_id_rangeJSON, "pattern");

    if (pattern) {
    if (!cJSON_IsString(pattern)) {
        ogs_error("OpenAPI_internal_group_id_range_parseFromJSON() failed [pattern]");
        goto end;
    }
    }

    internal_group_id_range_local_var = OpenAPI_internal_group_id_range_create (
        start ? ogs_strdup(start->valuestring) : NULL,
        end ? ogs_strdup(end->valuestring) : NULL,
        pattern ? ogs_strdup(pattern->valuestring) : NULL
    );

    return internal_group_id_range_local_var;
end:
    return NULL;
}

OpenAPI_internal_group_id_range_t *OpenAPI_internal_group_id_range_copy(OpenAPI_internal_group_id_range_t *dst, OpenAPI_internal_group_id_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_internal_group_id_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_internal_group_id_range_convertToJSON() failed");
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

    OpenAPI_internal_group_id_range_free(dst);
    dst = OpenAPI_internal_group_id_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

