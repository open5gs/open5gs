
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "time_unit.h"

OpenAPI_time_unit_t *OpenAPI_time_unit_create(
)
{
    OpenAPI_time_unit_t *time_unit_local_var = ogs_malloc(sizeof(OpenAPI_time_unit_t));
    ogs_assert(time_unit_local_var);


    return time_unit_local_var;
}

void OpenAPI_time_unit_free(OpenAPI_time_unit_t *time_unit)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == time_unit) {
        return;
    }
    ogs_free(time_unit);
}

cJSON *OpenAPI_time_unit_convertToJSON(OpenAPI_time_unit_t *time_unit)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (time_unit == NULL) {
        ogs_error("OpenAPI_time_unit_convertToJSON() failed [TimeUnit]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_time_unit_t *OpenAPI_time_unit_parseFromJSON(cJSON *time_unitJSON)
{
    OpenAPI_time_unit_t *time_unit_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    time_unit_local_var = OpenAPI_time_unit_create (
    );

    return time_unit_local_var;
end:
    return NULL;
}

OpenAPI_time_unit_t *OpenAPI_time_unit_copy(OpenAPI_time_unit_t *dst, OpenAPI_time_unit_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_time_unit_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_time_unit_convertToJSON() failed");
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

    OpenAPI_time_unit_free(dst);
    dst = OpenAPI_time_unit_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

