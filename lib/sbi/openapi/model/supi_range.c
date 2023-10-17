
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "supi_range.h"

OpenAPI_supi_range_t *OpenAPI_supi_range_create(
    char *start,
    char *end,
    char *pattern
)
{
    OpenAPI_supi_range_t *supi_range_local_var = ogs_malloc(sizeof(OpenAPI_supi_range_t));
    ogs_assert(supi_range_local_var);

    supi_range_local_var->start = start;
    supi_range_local_var->end = end;
    supi_range_local_var->pattern = pattern;

    return supi_range_local_var;
}

void OpenAPI_supi_range_free(OpenAPI_supi_range_t *supi_range)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == supi_range) {
        return;
    }
    if (supi_range->start) {
        ogs_free(supi_range->start);
        supi_range->start = NULL;
    }
    if (supi_range->end) {
        ogs_free(supi_range->end);
        supi_range->end = NULL;
    }
    if (supi_range->pattern) {
        ogs_free(supi_range->pattern);
        supi_range->pattern = NULL;
    }
    ogs_free(supi_range);
}

cJSON *OpenAPI_supi_range_convertToJSON(OpenAPI_supi_range_t *supi_range)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (supi_range == NULL) {
        ogs_error("OpenAPI_supi_range_convertToJSON() failed [SupiRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (supi_range->start) {
    if (cJSON_AddStringToObject(item, "start", supi_range->start) == NULL) {
        ogs_error("OpenAPI_supi_range_convertToJSON() failed [start]");
        goto end;
    }
    }

    if (supi_range->end) {
    if (cJSON_AddStringToObject(item, "end", supi_range->end) == NULL) {
        ogs_error("OpenAPI_supi_range_convertToJSON() failed [end]");
        goto end;
    }
    }

    if (supi_range->pattern) {
    if (cJSON_AddStringToObject(item, "pattern", supi_range->pattern) == NULL) {
        ogs_error("OpenAPI_supi_range_convertToJSON() failed [pattern]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_supi_range_t *OpenAPI_supi_range_parseFromJSON(cJSON *supi_rangeJSON)
{
    OpenAPI_supi_range_t *supi_range_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *start = NULL;
    cJSON *end = NULL;
    cJSON *pattern = NULL;
    start = cJSON_GetObjectItemCaseSensitive(supi_rangeJSON, "start");
    if (start) {
    if (!cJSON_IsString(start) && !cJSON_IsNull(start)) {
        ogs_error("OpenAPI_supi_range_parseFromJSON() failed [start]");
        goto end;
    }
    }

    end = cJSON_GetObjectItemCaseSensitive(supi_rangeJSON, "end");
    if (end) {
    if (!cJSON_IsString(end) && !cJSON_IsNull(end)) {
        ogs_error("OpenAPI_supi_range_parseFromJSON() failed [end]");
        goto end;
    }
    }

    pattern = cJSON_GetObjectItemCaseSensitive(supi_rangeJSON, "pattern");
    if (pattern) {
    if (!cJSON_IsString(pattern) && !cJSON_IsNull(pattern)) {
        ogs_error("OpenAPI_supi_range_parseFromJSON() failed [pattern]");
        goto end;
    }
    }

    supi_range_local_var = OpenAPI_supi_range_create (
        start && !cJSON_IsNull(start) ? ogs_strdup(start->valuestring) : NULL,
        end && !cJSON_IsNull(end) ? ogs_strdup(end->valuestring) : NULL,
        pattern && !cJSON_IsNull(pattern) ? ogs_strdup(pattern->valuestring) : NULL
    );

    return supi_range_local_var;
end:
    return NULL;
}

OpenAPI_supi_range_t *OpenAPI_supi_range_copy(OpenAPI_supi_range_t *dst, OpenAPI_supi_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_supi_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_supi_range_convertToJSON() failed");
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

    OpenAPI_supi_range_free(dst);
    dst = OpenAPI_supi_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

