
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "identity_range.h"

OpenAPI_identity_range_t *OpenAPI_identity_range_create(
    char *start,
    char *end,
    char *pattern
)
{
    OpenAPI_identity_range_t *identity_range_local_var = ogs_malloc(sizeof(OpenAPI_identity_range_t));
    ogs_assert(identity_range_local_var);

    identity_range_local_var->start = start;
    identity_range_local_var->end = end;
    identity_range_local_var->pattern = pattern;

    return identity_range_local_var;
}

void OpenAPI_identity_range_free(OpenAPI_identity_range_t *identity_range)
{
    if (NULL == identity_range) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(identity_range->start);
    ogs_free(identity_range->end);
    ogs_free(identity_range->pattern);
    ogs_free(identity_range);
}

cJSON *OpenAPI_identity_range_convertToJSON(OpenAPI_identity_range_t *identity_range)
{
    cJSON *item = NULL;

    if (identity_range == NULL) {
        ogs_error("OpenAPI_identity_range_convertToJSON() failed [IdentityRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (identity_range->start) {
    if (cJSON_AddStringToObject(item, "start", identity_range->start) == NULL) {
        ogs_error("OpenAPI_identity_range_convertToJSON() failed [start]");
        goto end;
    }
    }

    if (identity_range->end) {
    if (cJSON_AddStringToObject(item, "end", identity_range->end) == NULL) {
        ogs_error("OpenAPI_identity_range_convertToJSON() failed [end]");
        goto end;
    }
    }

    if (identity_range->pattern) {
    if (cJSON_AddStringToObject(item, "pattern", identity_range->pattern) == NULL) {
        ogs_error("OpenAPI_identity_range_convertToJSON() failed [pattern]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_identity_range_t *OpenAPI_identity_range_parseFromJSON(cJSON *identity_rangeJSON)
{
    OpenAPI_identity_range_t *identity_range_local_var = NULL;
    cJSON *start = cJSON_GetObjectItemCaseSensitive(identity_rangeJSON, "start");

    if (start) {
    if (!cJSON_IsString(start)) {
        ogs_error("OpenAPI_identity_range_parseFromJSON() failed [start]");
        goto end;
    }
    }

    cJSON *end = cJSON_GetObjectItemCaseSensitive(identity_rangeJSON, "end");

    if (end) {
    if (!cJSON_IsString(end)) {
        ogs_error("OpenAPI_identity_range_parseFromJSON() failed [end]");
        goto end;
    }
    }

    cJSON *pattern = cJSON_GetObjectItemCaseSensitive(identity_rangeJSON, "pattern");

    if (pattern) {
    if (!cJSON_IsString(pattern)) {
        ogs_error("OpenAPI_identity_range_parseFromJSON() failed [pattern]");
        goto end;
    }
    }

    identity_range_local_var = OpenAPI_identity_range_create (
        start ? ogs_strdup(start->valuestring) : NULL,
        end ? ogs_strdup(end->valuestring) : NULL,
        pattern ? ogs_strdup(pattern->valuestring) : NULL
    );

    return identity_range_local_var;
end:
    return NULL;
}

OpenAPI_identity_range_t *OpenAPI_identity_range_copy(OpenAPI_identity_range_t *dst, OpenAPI_identity_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_identity_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_identity_range_convertToJSON() failed");
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

    OpenAPI_identity_range_free(dst);
    dst = OpenAPI_identity_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

