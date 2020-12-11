
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "flow_status.h"

OpenAPI_flow_status_t *OpenAPI_flow_status_create(
    )
{
    OpenAPI_flow_status_t *flow_status_local_var = OpenAPI_malloc(sizeof(OpenAPI_flow_status_t));
    if (!flow_status_local_var) {
        return NULL;
    }

    return flow_status_local_var;
}

void OpenAPI_flow_status_free(OpenAPI_flow_status_t *flow_status)
{
    if (NULL == flow_status) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(flow_status);
}

cJSON *OpenAPI_flow_status_convertToJSON(OpenAPI_flow_status_t *flow_status)
{
    cJSON *item = NULL;

    if (flow_status == NULL) {
        ogs_error("OpenAPI_flow_status_convertToJSON() failed [FlowStatus]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_flow_status_t *OpenAPI_flow_status_parseFromJSON(cJSON *flow_statusJSON)
{
    OpenAPI_flow_status_t *flow_status_local_var = NULL;
    flow_status_local_var = OpenAPI_flow_status_create (
        );

    return flow_status_local_var;
end:
    return NULL;
}

OpenAPI_flow_status_t *OpenAPI_flow_status_copy(OpenAPI_flow_status_t *dst, OpenAPI_flow_status_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_flow_status_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_flow_status_convertToJSON() failed");
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

    OpenAPI_flow_status_free(dst);
    dst = OpenAPI_flow_status_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

