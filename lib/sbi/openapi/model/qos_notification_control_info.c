
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qos_notification_control_info.h"

OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_create(
    OpenAPI_qos_notif_type_e notif_type,
    OpenAPI_list_t *flows,
    char *alt_ser_req
)
{
    OpenAPI_qos_notification_control_info_t *qos_notification_control_info_local_var = ogs_malloc(sizeof(OpenAPI_qos_notification_control_info_t));
    ogs_assert(qos_notification_control_info_local_var);

    qos_notification_control_info_local_var->notif_type = notif_type;
    qos_notification_control_info_local_var->flows = flows;
    qos_notification_control_info_local_var->alt_ser_req = alt_ser_req;

    return qos_notification_control_info_local_var;
}

void OpenAPI_qos_notification_control_info_free(OpenAPI_qos_notification_control_info_t *qos_notification_control_info)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == qos_notification_control_info) {
        return;
    }
    if (qos_notification_control_info->flows) {
        OpenAPI_list_for_each(qos_notification_control_info->flows, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(qos_notification_control_info->flows);
        qos_notification_control_info->flows = NULL;
    }
    if (qos_notification_control_info->alt_ser_req) {
        ogs_free(qos_notification_control_info->alt_ser_req);
        qos_notification_control_info->alt_ser_req = NULL;
    }
    ogs_free(qos_notification_control_info);
}

cJSON *OpenAPI_qos_notification_control_info_convertToJSON(OpenAPI_qos_notification_control_info_t *qos_notification_control_info)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (qos_notification_control_info == NULL) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [QosNotificationControlInfo]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (qos_notification_control_info->notif_type == OpenAPI_qos_notif_type_NULL) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [notif_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifType", OpenAPI_qos_notif_type_ToString(qos_notification_control_info->notif_type)) == NULL) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [notif_type]");
        goto end;
    }

    if (qos_notification_control_info->flows) {
    cJSON *flowsList = cJSON_AddArrayToObject(item, "flows");
    if (flowsList == NULL) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [flows]");
        goto end;
    }
    OpenAPI_list_for_each(qos_notification_control_info->flows, node) {
        cJSON *itemLocal = OpenAPI_flows_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [flows]");
            goto end;
        }
        cJSON_AddItemToArray(flowsList, itemLocal);
    }
    }

    if (qos_notification_control_info->alt_ser_req) {
    if (cJSON_AddStringToObject(item, "altSerReq", qos_notification_control_info->alt_ser_req) == NULL) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed [alt_ser_req]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_parseFromJSON(cJSON *qos_notification_control_infoJSON)
{
    OpenAPI_qos_notification_control_info_t *qos_notification_control_info_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notif_type = NULL;
    OpenAPI_qos_notif_type_e notif_typeVariable = 0;
    cJSON *flows = NULL;
    OpenAPI_list_t *flowsList = NULL;
    cJSON *alt_ser_req = NULL;
    notif_type = cJSON_GetObjectItemCaseSensitive(qos_notification_control_infoJSON, "notifType");
    if (!notif_type) {
        ogs_error("OpenAPI_qos_notification_control_info_parseFromJSON() failed [notif_type]");
        goto end;
    }
    if (!cJSON_IsString(notif_type)) {
        ogs_error("OpenAPI_qos_notification_control_info_parseFromJSON() failed [notif_type]");
        goto end;
    }
    notif_typeVariable = OpenAPI_qos_notif_type_FromString(notif_type->valuestring);

    flows = cJSON_GetObjectItemCaseSensitive(qos_notification_control_infoJSON, "flows");
    if (flows) {
        cJSON *flows_local = NULL;
        if (!cJSON_IsArray(flows)) {
            ogs_error("OpenAPI_qos_notification_control_info_parseFromJSON() failed [flows]");
            goto end;
        }

        flowsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flows_local, flows) {
            if (!cJSON_IsObject(flows_local)) {
                ogs_error("OpenAPI_qos_notification_control_info_parseFromJSON() failed [flows]");
                goto end;
            }
            OpenAPI_flows_t *flowsItem = OpenAPI_flows_parseFromJSON(flows_local);
            if (!flowsItem) {
                ogs_error("No flowsItem");
                goto end;
            }
            OpenAPI_list_add(flowsList, flowsItem);
        }
    }

    alt_ser_req = cJSON_GetObjectItemCaseSensitive(qos_notification_control_infoJSON, "altSerReq");
    if (alt_ser_req) {
    if (!cJSON_IsString(alt_ser_req) && !cJSON_IsNull(alt_ser_req)) {
        ogs_error("OpenAPI_qos_notification_control_info_parseFromJSON() failed [alt_ser_req]");
        goto end;
    }
    }

    qos_notification_control_info_local_var = OpenAPI_qos_notification_control_info_create (
        notif_typeVariable,
        flows ? flowsList : NULL,
        alt_ser_req && !cJSON_IsNull(alt_ser_req) ? ogs_strdup(alt_ser_req->valuestring) : NULL
    );

    return qos_notification_control_info_local_var;
end:
    if (flowsList) {
        OpenAPI_list_for_each(flowsList, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(flowsList);
        flowsList = NULL;
    }
    return NULL;
}

OpenAPI_qos_notification_control_info_t *OpenAPI_qos_notification_control_info_copy(OpenAPI_qos_notification_control_info_t *dst, OpenAPI_qos_notification_control_info_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_qos_notification_control_info_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_qos_notification_control_info_convertToJSON() failed");
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

    OpenAPI_qos_notification_control_info_free(dst);
    dst = OpenAPI_qos_notification_control_info_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

