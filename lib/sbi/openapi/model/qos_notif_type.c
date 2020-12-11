
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_notif_type.h"

OpenAPI_qos_notif_type_t *OpenAPI_qos_notif_type_create(
    )
{
    OpenAPI_qos_notif_type_t *qos_notif_type_local_var = OpenAPI_malloc(sizeof(OpenAPI_qos_notif_type_t));
    if (!qos_notif_type_local_var) {
        return NULL;
    }

    return qos_notif_type_local_var;
}

void OpenAPI_qos_notif_type_free(OpenAPI_qos_notif_type_t *qos_notif_type)
{
    if (NULL == qos_notif_type) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(qos_notif_type);
}

cJSON *OpenAPI_qos_notif_type_convertToJSON(OpenAPI_qos_notif_type_t *qos_notif_type)
{
    cJSON *item = NULL;

    if (qos_notif_type == NULL) {
        ogs_error("OpenAPI_qos_notif_type_convertToJSON() failed [QosNotifType]");
        return NULL;
    }

    item = cJSON_CreateObject();
end:
    return item;
}

OpenAPI_qos_notif_type_t *OpenAPI_qos_notif_type_parseFromJSON(cJSON *qos_notif_typeJSON)
{
    OpenAPI_qos_notif_type_t *qos_notif_type_local_var = NULL;
    qos_notif_type_local_var = OpenAPI_qos_notif_type_create (
        );

    return qos_notif_type_local_var;
end:
    return NULL;
}

OpenAPI_qos_notif_type_t *OpenAPI_qos_notif_type_copy(OpenAPI_qos_notif_type_t *dst, OpenAPI_qos_notif_type_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_notif_type_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_notif_type_convertToJSON() failed");
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

    OpenAPI_qos_notif_type_free(dst);
    dst = OpenAPI_qos_notif_type_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

