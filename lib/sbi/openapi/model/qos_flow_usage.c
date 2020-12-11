
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_flow_usage.h"

OpenAPI_qos_flow_usage_t *OpenAPI_qos_flow_usage_create(
    )
{
    OpenAPI_qos_flow_usage_t *qos_flow_usage_local_var = OpenAPI_malloc(sizeof(OpenAPI_qos_flow_usage_t));
    if (!qos_flow_usage_local_var) {
        return NULL;
    }

    return qos_flow_usage_local_var;
}

void OpenAPI_qos_flow_usage_free(OpenAPI_qos_flow_usage_t *qos_flow_usage)
{
    if (NULL == qos_flow_usage) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(qos_flow_usage);
}

cJSON *OpenAPI_qos_flow_usage_convertToJSON(OpenAPI_qos_flow_usage_t *qos_flow_usage)
{
    cJSON *item = NULL;

    if (qos_flow_usage == NULL) {
        ogs_error("OpenAPI_qos_flow_usage_convertToJSON() failed [QosFlowUsage]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_qos_flow_usage_t *OpenAPI_qos_flow_usage_parseFromJSON(cJSON *qos_flow_usageJSON)
{
    OpenAPI_qos_flow_usage_t *qos_flow_usage_local_var = NULL;
    qos_flow_usage_local_var = OpenAPI_qos_flow_usage_create (
        );

    return qos_flow_usage_local_var;
end:
    return NULL;
}

OpenAPI_qos_flow_usage_t *OpenAPI_qos_flow_usage_copy(OpenAPI_qos_flow_usage_t *dst, OpenAPI_qos_flow_usage_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_flow_usage_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_flow_usage_convertToJSON() failed");
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

    OpenAPI_qos_flow_usage_free(dst);
    dst = OpenAPI_qos_flow_usage_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

