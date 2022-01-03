
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sd_range.h"

OpenAPI_sd_range_t *OpenAPI_sd_range_create(
    char *start,
    char *end
)
{
    OpenAPI_sd_range_t *sd_range_local_var = ogs_malloc(sizeof(OpenAPI_sd_range_t));
    ogs_assert(sd_range_local_var);

    sd_range_local_var->start = start;
    sd_range_local_var->end = end;

    return sd_range_local_var;
}

void OpenAPI_sd_range_free(OpenAPI_sd_range_t *sd_range)
{
    if (NULL == sd_range) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sd_range->start);
    ogs_free(sd_range->end);
    ogs_free(sd_range);
}

cJSON *OpenAPI_sd_range_convertToJSON(OpenAPI_sd_range_t *sd_range)
{
    cJSON *item = NULL;

    if (sd_range == NULL) {
        ogs_error("OpenAPI_sd_range_convertToJSON() failed [SdRange]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sd_range->start) {
    if (cJSON_AddStringToObject(item, "start", sd_range->start) == NULL) {
        ogs_error("OpenAPI_sd_range_convertToJSON() failed [start]");
        goto end;
    }
    }

    if (sd_range->end) {
    if (cJSON_AddStringToObject(item, "end", sd_range->end) == NULL) {
        ogs_error("OpenAPI_sd_range_convertToJSON() failed [end]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sd_range_t *OpenAPI_sd_range_parseFromJSON(cJSON *sd_rangeJSON)
{
    OpenAPI_sd_range_t *sd_range_local_var = NULL;
    cJSON *start = cJSON_GetObjectItemCaseSensitive(sd_rangeJSON, "start");

    if (start) {
    if (!cJSON_IsString(start)) {
        ogs_error("OpenAPI_sd_range_parseFromJSON() failed [start]");
        goto end;
    }
    }

    cJSON *end = cJSON_GetObjectItemCaseSensitive(sd_rangeJSON, "end");

    if (end) {
    if (!cJSON_IsString(end)) {
        ogs_error("OpenAPI_sd_range_parseFromJSON() failed [end]");
        goto end;
    }
    }

    sd_range_local_var = OpenAPI_sd_range_create (
        start ? ogs_strdup(start->valuestring) : NULL,
        end ? ogs_strdup(end->valuestring) : NULL
    );

    return sd_range_local_var;
end:
    return NULL;
}

OpenAPI_sd_range_t *OpenAPI_sd_range_copy(OpenAPI_sd_range_t *dst, OpenAPI_sd_range_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sd_range_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sd_range_convertToJSON() failed");
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

    OpenAPI_sd_range_free(dst);
    dst = OpenAPI_sd_range_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

