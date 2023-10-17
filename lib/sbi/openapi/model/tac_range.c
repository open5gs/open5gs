
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tac_range.h"

OpenAPI_tac_range_t *OpenAPI_tac_range_create(
    char *start,
    char *end,
    char *pattern
)
{
    OpenAPI_tac_range_t *tac_range_local_var = ogs_malloc(sizeof(OpenAPI_tac_range_t));
    ogs_assert(tac_range_local_var);

    tac_range_local_var->start = start;
    tac_range_local_var->end = end;
    tac_range_local_var->pattern = pattern;

    return tac_range_local_var;
}

void OpenAPI_tac_range_free(OpenAPI_tac_range_t *tac_range)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == tac_range) {
        return;
    }
    if (tac_range->start) {
        ogs_free(tac_range->start);
        tac_range->start = NULL;
    }
    if (tac_range->end) {
        ogs_free(tac_range->end);
        tac_range->end = NULL;
    }
    if (tac_range->pattern) {
        ogs_free(tac_range->pattern);
        tac_range->pattern = NULL;
    }
    ogs_free(tac_range);
}

cJSON *OpenAPI_tac_range_convertToJSON(OpenAPI_tac_range_t *tac_range)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (tac_range == NULL) {
        ogs_error("OpenAPI_tac_range_convertToJSON() failed [TacRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (tac_range->start) {
    if (cJSON_AddStringToObject(item, "start", tac_range->start) == NULL) {
        ogs_error("OpenAPI_tac_range_convertToJSON() failed [start]");
        goto end;
    }
    }

    if (tac_range->end) {
    if (cJSON_AddStringToObject(item, "end", tac_range->end) == NULL) {
        ogs_error("OpenAPI_tac_range_convertToJSON() failed [end]");
        goto end;
    }
    }

    if (tac_range->pattern) {
    if (cJSON_AddStringToObject(item, "pattern", tac_range->pattern) == NULL) {
        ogs_error("OpenAPI_tac_range_convertToJSON() failed [pattern]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_tac_range_t *OpenAPI_tac_range_parseFromJSON(cJSON *tac_rangeJSON)
{
    OpenAPI_tac_range_t *tac_range_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *start = NULL;
    cJSON *end = NULL;
    cJSON *pattern = NULL;
    start = cJSON_GetObjectItemCaseSensitive(tac_rangeJSON, "start");
    if (start) {
    if (!cJSON_IsString(start) && !cJSON_IsNull(start)) {
        ogs_error("OpenAPI_tac_range_parseFromJSON() failed [start]");
        goto end;
    }
    }

    end = cJSON_GetObjectItemCaseSensitive(tac_rangeJSON, "end");
    if (end) {
    if (!cJSON_IsString(end) && !cJSON_IsNull(end)) {
        ogs_error("OpenAPI_tac_range_parseFromJSON() failed [end]");
        goto end;
    }
    }

    pattern = cJSON_GetObjectItemCaseSensitive(tac_rangeJSON, "pattern");
    if (pattern) {
    if (!cJSON_IsString(pattern) && !cJSON_IsNull(pattern)) {
        ogs_error("OpenAPI_tac_range_parseFromJSON() failed [pattern]");
        goto end;
    }
    }

    tac_range_local_var = OpenAPI_tac_range_create (
        start && !cJSON_IsNull(start) ? ogs_strdup(start->valuestring) : NULL,
        end && !cJSON_IsNull(end) ? ogs_strdup(end->valuestring) : NULL,
        pattern && !cJSON_IsNull(pattern) ? ogs_strdup(pattern->valuestring) : NULL
    );

    return tac_range_local_var;
end:
    return NULL;
}

OpenAPI_tac_range_t *OpenAPI_tac_range_copy(OpenAPI_tac_range_t *dst, OpenAPI_tac_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_tac_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_tac_range_convertToJSON() failed");
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

    OpenAPI_tac_range_free(dst);
    dst = OpenAPI_tac_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

