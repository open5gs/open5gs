
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "matching_direction.h"

OpenAPI_matching_direction_t *OpenAPI_matching_direction_create(
)
{
    OpenAPI_matching_direction_t *matching_direction_local_var = ogs_malloc(sizeof(OpenAPI_matching_direction_t));
    ogs_assert(matching_direction_local_var);


    return matching_direction_local_var;
}

void OpenAPI_matching_direction_free(OpenAPI_matching_direction_t *matching_direction)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == matching_direction) {
        return;
    }
    ogs_free(matching_direction);
}

cJSON *OpenAPI_matching_direction_convertToJSON(OpenAPI_matching_direction_t *matching_direction)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (matching_direction == NULL) {
        ogs_error("OpenAPI_matching_direction_convertToJSON() failed [MatchingDirection]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_matching_direction_t *OpenAPI_matching_direction_parseFromJSON(cJSON *matching_directionJSON)
{
    OpenAPI_matching_direction_t *matching_direction_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    matching_direction_local_var = OpenAPI_matching_direction_create (
    );

    return matching_direction_local_var;
end:
    return NULL;
}

OpenAPI_matching_direction_t *OpenAPI_matching_direction_copy(OpenAPI_matching_direction_t *dst, OpenAPI_matching_direction_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_matching_direction_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_matching_direction_convertToJSON() failed");
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

    OpenAPI_matching_direction_free(dst);
    dst = OpenAPI_matching_direction_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

