
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_resource_type.h"

OpenAPI_qos_resource_type_t *OpenAPI_qos_resource_type_create(
    )
{
    OpenAPI_qos_resource_type_t *qos_resource_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_qos_resource_type_t));
    if (!qos_resource_type_local_var) {
        return NULL;
    }

    return qos_resource_type_local_var;
}

void OpenAPI_qos_resource_type_free(OpenAPI_qos_resource_type_t *qos_resource_type)
{
    if (NULL == qos_resource_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(qos_resource_type);
}

cJSON *OpenAPI_qos_resource_type_convertToJSON(OpenAPI_qos_resource_type_t *qos_resource_type)
{
    cJSON *item = NULL;

    if (qos_resource_type == NULL) {
        ogs_error("OpenAPI_qos_resource_type_convertToJSON() failed [QosResourceType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_qos_resource_type_t *OpenAPI_qos_resource_type_parseFromJSON(cJSON *qos_resource_typeJSON)
{
    OpenAPI_qos_resource_type_t *qos_resource_type_local_var = NULL;
    qos_resource_type_local_var = OpenAPI_qos_resource_type_create (
        );

    return qos_resource_type_local_var;
end:
    return NULL;
}

OpenAPI_qos_resource_type_t *OpenAPI_qos_resource_type_copy(OpenAPI_qos_resource_type_t *dst, OpenAPI_qos_resource_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_resource_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_resource_type_convertToJSON() failed");
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

    OpenAPI_qos_resource_type_free(dst);
    dst = OpenAPI_qos_resource_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

