
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
    OpenAPI_plmn_range_t *plmn_range_local_var = OpenAPI_malloc(sizeof(OpenAPI_plmn_range_t));
    if (!plmn_range_local_var) {
        return NULL;
    }
    plmn_range_local_var->start = start;
    plmn_range_local_var->end = end;
    plmn_range_local_var->pattern = pattern;

    return plmn_range_local_var;
}

void OpenAPI_plmn_range_free(OpenAPI_plmn_range_t *plmn_range)
{
    if (NULL == plmn_range) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(plmn_range->start);
    ogs_free(plmn_range->end);
    ogs_free(plmn_range->pattern);
    ogs_free(plmn_range);
}

cJSON *OpenAPI_plmn_range_convertToJSON(OpenAPI_plmn_range_t *plmn_range)
{
    cJSON *item = NULL;

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
    cJSON *start = cJSON_GetObjectItemCaseSensitive(plmn_rangeJSON, "start");

    if (start) {
        if (!cJSON_IsString(start)) {
            ogs_error("OpenAPI_plmn_range_parseFromJSON() failed [start]");
            goto end;
        }
    }

    cJSON *end = cJSON_GetObjectItemCaseSensitive(plmn_rangeJSON, "end");

    if (end) {
        if (!cJSON_IsString(end)) {
            ogs_error("OpenAPI_plmn_range_parseFromJSON() failed [end]");
            goto end;
        }
    }

    cJSON *pattern = cJSON_GetObjectItemCaseSensitive(plmn_rangeJSON, "pattern");

    if (pattern) {
        if (!cJSON_IsString(pattern)) {
            ogs_error("OpenAPI_plmn_range_parseFromJSON() failed [pattern]");
            goto end;
        }
    }

    plmn_range_local_var = OpenAPI_plmn_range_create (
        start ? ogs_strdup(start->valuestring) : NULL,
        end ? ogs_strdup(end->valuestring) : NULL,
        pattern ? ogs_strdup(pattern->valuestring) : NULL
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

