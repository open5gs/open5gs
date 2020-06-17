
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "additional_qos_flow_info.h"

OpenAPI_additional_qos_flow_info_t *OpenAPI_additional_qos_flow_info_create(
    )
{
    OpenAPI_additional_qos_flow_info_t *additional_qos_flow_info_local_var = OpenAPI_malloc(sizeof(OpenAPI_additional_qos_flow_info_t));
    if (!additional_qos_flow_info_local_var) {
        return NULL;
    }

    return additional_qos_flow_info_local_var;
}

void OpenAPI_additional_qos_flow_info_free(OpenAPI_additional_qos_flow_info_t *additional_qos_flow_info)
{
    if (NULL == additional_qos_flow_info) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(additional_qos_flow_info);
}

cJSON *OpenAPI_additional_qos_flow_info_convertToJSON(OpenAPI_additional_qos_flow_info_t *additional_qos_flow_info)
{
    cJSON *item = NULL;

    if (additional_qos_flow_info == NULL) {
        ogs_error("OpenAPI_additional_qos_flow_info_convertToJSON() failed [AdditionalQosFlowInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_additional_qos_flow_info_t *OpenAPI_additional_qos_flow_info_parseFromJSON(cJSON *additional_qos_flow_infoJSON)
{
    OpenAPI_additional_qos_flow_info_t *additional_qos_flow_info_local_var = NULL;
    additional_qos_flow_info_local_var = OpenAPI_additional_qos_flow_info_create (
        );

    return additional_qos_flow_info_local_var;
end:
    return NULL;
}

OpenAPI_additional_qos_flow_info_t *OpenAPI_additional_qos_flow_info_copy(OpenAPI_additional_qos_flow_info_t *dst, OpenAPI_additional_qos_flow_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_additional_qos_flow_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_additional_qos_flow_info_convertToJSON() failed");
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

    OpenAPI_additional_qos_flow_info_free(dst);
    dst = OpenAPI_additional_qos_flow_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

