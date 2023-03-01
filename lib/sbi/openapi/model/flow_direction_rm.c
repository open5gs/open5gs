
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "flow_direction_rm.h"

OpenAPI_flow_direction_rm_t *OpenAPI_flow_direction_rm_create(
)
{
    OpenAPI_flow_direction_rm_t *flow_direction_rm_local_var = ogs_malloc(sizeof(OpenAPI_flow_direction_rm_t));
    ogs_assert(flow_direction_rm_local_var);


    return flow_direction_rm_local_var;
}

void OpenAPI_flow_direction_rm_free(OpenAPI_flow_direction_rm_t *flow_direction_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == flow_direction_rm) {
        return;
    }
    ogs_free(flow_direction_rm);
}

cJSON *OpenAPI_flow_direction_rm_convertToJSON(OpenAPI_flow_direction_rm_t *flow_direction_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (flow_direction_rm == NULL) {
        ogs_error("OpenAPI_flow_direction_rm_convertToJSON() failed [FlowDirectionRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_flow_direction_rm_t *OpenAPI_flow_direction_rm_parseFromJSON(cJSON *flow_direction_rmJSON)
{
    OpenAPI_flow_direction_rm_t *flow_direction_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    flow_direction_rm_local_var = OpenAPI_flow_direction_rm_create (
    );

    return flow_direction_rm_local_var;
end:
    return NULL;
}

OpenAPI_flow_direction_rm_t *OpenAPI_flow_direction_rm_copy(OpenAPI_flow_direction_rm_t *dst, OpenAPI_flow_direction_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_flow_direction_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_flow_direction_rm_convertToJSON() failed");
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

    OpenAPI_flow_direction_rm_free(dst);
    dst = OpenAPI_flow_direction_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

