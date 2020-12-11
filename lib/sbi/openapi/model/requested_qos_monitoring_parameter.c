
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "requested_qos_monitoring_parameter.h"

OpenAPI_requested_qos_monitoring_parameter_t *OpenAPI_requested_qos_monitoring_parameter_create(
    )
{
    OpenAPI_requested_qos_monitoring_parameter_t *requested_qos_monitoring_parameter_local_var = OpenAPI_malloc(sizeof(OpenAPI_requested_qos_monitoring_parameter_t));
    if (!requested_qos_monitoring_parameter_local_var) {
        return NULL;
    }

    return requested_qos_monitoring_parameter_local_var;
}

void OpenAPI_requested_qos_monitoring_parameter_free(OpenAPI_requested_qos_monitoring_parameter_t *requested_qos_monitoring_parameter)
{
    if (NULL == requested_qos_monitoring_parameter) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(requested_qos_monitoring_parameter);
}

cJSON *OpenAPI_requested_qos_monitoring_parameter_convertToJSON(OpenAPI_requested_qos_monitoring_parameter_t *requested_qos_monitoring_parameter)
{
    cJSON *item = NULL;

    if (requested_qos_monitoring_parameter == NULL) {
        ogs_error("OpenAPI_requested_qos_monitoring_parameter_convertToJSON() failed [RequestedQosMonitoringParameter]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_requested_qos_monitoring_parameter_t *OpenAPI_requested_qos_monitoring_parameter_parseFromJSON(cJSON *requested_qos_monitoring_parameterJSON)
{
    OpenAPI_requested_qos_monitoring_parameter_t *requested_qos_monitoring_parameter_local_var = NULL;
    requested_qos_monitoring_parameter_local_var = OpenAPI_requested_qos_monitoring_parameter_create (
        );

    return requested_qos_monitoring_parameter_local_var;
end:
    return NULL;
}

OpenAPI_requested_qos_monitoring_parameter_t *OpenAPI_requested_qos_monitoring_parameter_copy(OpenAPI_requested_qos_monitoring_parameter_t *dst, OpenAPI_requested_qos_monitoring_parameter_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_requested_qos_monitoring_parameter_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_requested_qos_monitoring_parameter_convertToJSON() failed");
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

    OpenAPI_requested_qos_monitoring_parameter_free(dst);
    dst = OpenAPI_requested_qos_monitoring_parameter_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

