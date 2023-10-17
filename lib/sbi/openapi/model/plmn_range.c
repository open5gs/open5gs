
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "plmn_range.h"

OpenAPI_plmn_range_t *OpenAPI_plmn_range_create(
    char *start,
    char *end,
    char *pattern
)
{
    OpenAPI_plmn_range_t *plmn_range_local_var = ogs_malloc(sizeof(OpenAPI_plmn_range_t));
    ogs_assert(plmn_range_local_var);

    plmn_range_local_var->start = start;
    plmn_range_local_var->end = end;
    plmn_range_local_var->pattern = pattern;

    return plmn_range_local_var;
}

void OpenAPI_plmn_range_free(OpenAPI_plmn_range_t *plmn_range)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == plmn_range) {
        return;
    }
    if (plmn_range->start) {
        ogs_free(plmn_range->start);
        plmn_range->start = NULL;
    }
    if (plmn_range->end) {
        ogs_free(plmn_range->end);
        plmn_range->end = NULL;
    }
    if (plmn_range->pattern) {
        ogs_free(plmn_range->pattern);
        plmn_range->pattern = NULL;
    }
    ogs_free(plmn_range);
}

cJSON *OpenAPI_plmn_range_convertToJSON(OpenAPI_plmn_range_t *plmn_range)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (plmn_range == NULL) {
        ogs_error("OpenAPI_plmn_range_convertToJSON() failed [PlmnRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (plmn_range->start) {
    if (cJSON_AddStringToObject(item, "start", plmn_range->start) == NULL) {
        ogs_error("OpenAPI_plmn_range_convertToJSON() failed [start]");
        goto end;
    }
    }

    if (plmn_range->end) {
    if (cJSON_AddStringToObject(item, "end", plmn_range->end) == NULL) {
        ogs_error("OpenAPI_plmn_range_convertToJSON() failed [end]");
        goto end;
    }
    }

    if (plmn_range->pattern) {
    if (cJSON_AddStringToObject(item, "pattern", plmn_range->pattern) == NULL) {
        ogs_error("OpenAPI_plmn_range_convertToJSON() failed [pattern]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_plmn_range_t *OpenAPI_plmn_range_parseFromJSON(cJSON *plmn_rangeJSON)
{
    OpenAPI_plmn_range_t *plmn_range_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *start = NULL;
    cJSON *end = NULL;
    cJSON *pattern = NULL;
    start = cJSON_GetObjectItemCaseSensitive(plmn_rangeJSON, "start");
    if (start) {
    if (!cJSON_IsString(start) && !cJSON_IsNull(start)) {
        ogs_error("OpenAPI_plmn_range_parseFromJSON() failed [start]");
        goto end;
    }
    }

    end = cJSON_GetObjectItemCaseSensitive(plmn_rangeJSON, "end");
    if (end) {
    if (!cJSON_IsString(end) && !cJSON_IsNull(end)) {
        ogs_error("OpenAPI_plmn_range_parseFromJSON() failed [end]");
        goto end;
    }
    }

    pattern = cJSON_GetObjectItemCaseSensitive(plmn_rangeJSON, "pattern");
    if (pattern) {
    if (!cJSON_IsString(pattern) && !cJSON_IsNull(pattern)) {
        ogs_error("OpenAPI_plmn_range_parseFromJSON() failed [pattern]");
        goto end;
    }
    }

    plmn_range_local_var = OpenAPI_plmn_range_create (
        start && !cJSON_IsNull(start) ? ogs_strdup(start->valuestring) : NULL,
        end && !cJSON_IsNull(end) ? ogs_strdup(end->valuestring) : NULL,
        pattern && !cJSON_IsNull(pattern) ? ogs_strdup(pattern->valuestring) : NULL
    );

    return plmn_range_local_var;
end:
    return NULL;
}

OpenAPI_plmn_range_t *OpenAPI_plmn_range_copy(OpenAPI_plmn_range_t *dst, OpenAPI_plmn_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_plmn_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_plmn_range_convertToJSON() failed");
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

    OpenAPI_plmn_range_free(dst);
    dst = OpenAPI_plmn_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

