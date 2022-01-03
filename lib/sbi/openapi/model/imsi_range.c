
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "imsi_range.h"

OpenAPI_imsi_range_t *OpenAPI_imsi_range_create(
    char *start,
    char *end,
    char *pattern
)
{
    OpenAPI_imsi_range_t *imsi_range_local_var = ogs_malloc(sizeof(OpenAPI_imsi_range_t));
    ogs_assert(imsi_range_local_var);

    imsi_range_local_var->start = start;
    imsi_range_local_var->end = end;
    imsi_range_local_var->pattern = pattern;

    return imsi_range_local_var;
}

void OpenAPI_imsi_range_free(OpenAPI_imsi_range_t *imsi_range)
{
    if (NULL == imsi_range) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(imsi_range->start);
    ogs_free(imsi_range->end);
    ogs_free(imsi_range->pattern);
    ogs_free(imsi_range);
}

cJSON *OpenAPI_imsi_range_convertToJSON(OpenAPI_imsi_range_t *imsi_range)
{
    cJSON *item = NULL;

    if (imsi_range == NULL) {
        ogs_error("OpenAPI_imsi_range_convertToJSON() failed [ImsiRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (imsi_range->start) {
    if (cJSON_AddStringToObject(item, "start", imsi_range->start) == NULL) {
        ogs_error("OpenAPI_imsi_range_convertToJSON() failed [start]");
        goto end;
    }
    }

    if (imsi_range->end) {
    if (cJSON_AddStringToObject(item, "end", imsi_range->end) == NULL) {
        ogs_error("OpenAPI_imsi_range_convertToJSON() failed [end]");
        goto end;
    }
    }

    if (imsi_range->pattern) {
    if (cJSON_AddStringToObject(item, "pattern", imsi_range->pattern) == NULL) {
        ogs_error("OpenAPI_imsi_range_convertToJSON() failed [pattern]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_imsi_range_t *OpenAPI_imsi_range_parseFromJSON(cJSON *imsi_rangeJSON)
{
    OpenAPI_imsi_range_t *imsi_range_local_var = NULL;
    cJSON *start = cJSON_GetObjectItemCaseSensitive(imsi_rangeJSON, "start");

    if (start) {
    if (!cJSON_IsString(start)) {
        ogs_error("OpenAPI_imsi_range_parseFromJSON() failed [start]");
        goto end;
    }
    }

    cJSON *end = cJSON_GetObjectItemCaseSensitive(imsi_rangeJSON, "end");

    if (end) {
    if (!cJSON_IsString(end)) {
        ogs_error("OpenAPI_imsi_range_parseFromJSON() failed [end]");
        goto end;
    }
    }

    cJSON *pattern = cJSON_GetObjectItemCaseSensitive(imsi_rangeJSON, "pattern");

    if (pattern) {
    if (!cJSON_IsString(pattern)) {
        ogs_error("OpenAPI_imsi_range_parseFromJSON() failed [pattern]");
        goto end;
    }
    }

    imsi_range_local_var = OpenAPI_imsi_range_create (
        start ? ogs_strdup(start->valuestring) : NULL,
        end ? ogs_strdup(end->valuestring) : NULL,
        pattern ? ogs_strdup(pattern->valuestring) : NULL
    );

    return imsi_range_local_var;
end:
    return NULL;
}

OpenAPI_imsi_range_t *OpenAPI_imsi_range_copy(OpenAPI_imsi_range_t *dst, OpenAPI_imsi_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_imsi_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_imsi_range_convertToJSON() failed");
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

    OpenAPI_imsi_range_free(dst);
    dst = OpenAPI_imsi_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

