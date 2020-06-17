
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "flow_direction.h"

OpenAPI_flow_direction_t *OpenAPI_flow_direction_create(
    )
{
    OpenAPI_flow_direction_t *flow_direction_local_var = OpenAPI_malloc(sizeof(OpenAPI_flow_direction_t));
    if (!flow_direction_local_var) {
        return NULL;
    }

    return flow_direction_local_var;
}

void OpenAPI_flow_direction_free(OpenAPI_flow_direction_t *flow_direction)
{
    if (NULL == flow_direction) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(flow_direction);
}

cJSON *OpenAPI_flow_direction_convertToJSON(OpenAPI_flow_direction_t *flow_direction)
{
    cJSON *item = NULL;

    if (flow_direction == NULL) {
        ogs_error("OpenAPI_flow_direction_convertToJSON() failed [FlowDirection]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_flow_direction_t *OpenAPI_flow_direction_parseFromJSON(cJSON *flow_directionJSON)
{
    OpenAPI_flow_direction_t *flow_direction_local_var = NULL;
    flow_direction_local_var = OpenAPI_flow_direction_create (
        );

    return flow_direction_local_var;
end:
    return NULL;
}

OpenAPI_flow_direction_t *OpenAPI_flow_direction_copy(OpenAPI_flow_direction_t *dst, OpenAPI_flow_direction_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_flow_direction_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_flow_direction_convertToJSON() failed");
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

    OpenAPI_flow_direction_free(dst);
    dst = OpenAPI_flow_direction_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

