
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

