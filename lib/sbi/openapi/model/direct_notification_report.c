
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "direct_notification_report.h"

OpenAPI_direct_notification_report_t *OpenAPI_direct_notification_report_create(
    OpenAPI_qos_monitoring_param_type_e qos_mon_param_type,
    OpenAPI_list_t *flows
)
{
    OpenAPI_direct_notification_report_t *direct_notification_report_local_var = ogs_malloc(sizeof(OpenAPI_direct_notification_report_t));
    ogs_assert(direct_notification_report_local_var);

    direct_notification_report_local_var->qos_mon_param_type = qos_mon_param_type;
    direct_notification_report_local_var->flows = flows;

    return direct_notification_report_local_var;
}

void OpenAPI_direct_notification_report_free(OpenAPI_direct_notification_report_t *direct_notification_report)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == direct_notification_report) {
        return;
    }
    if (direct_notification_report->flows) {
        OpenAPI_list_for_each(direct_notification_report->flows, node) {
            OpenAPI_flows_free(node->data);
        }
        OpenAPI_list_free(direct_notification_report->flows);
        direct_notification_report->flows = NULL;
    }
    ogs_free(direct_notification_report);
}

cJSON *OpenAPI_direct_notification_report_convertToJSON(OpenAPI_direct_notification_report_t *direct_notification_report)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (direct_notification_report == NULL) {
        ogs_error("OpenAPI_direct_notification_report_convertToJSON() failed [DirectNotificationReport]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (direct_notification_report->qos_mon_param_type == OpenAPI_qos_monitoring_param_type_NULL) {
        ogs_error("OpenAPI_direct_notification_report_convertToJSON() failed [qos_mon_param_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "qosMonParamType", OpenAPI_qos_monitoring_param_type_ToString(direct_notification_report->qos_mon_param_type)) == NULL) {
        ogs_error("OpenAPI_direct_notification_report_convertToJSON() failed [qos_mon_param_type]");
        goto end;
    }

    if (direct_notification_report->flows) {
    cJSON *flowsList = cJSON_AddArrayToObject(item, "flows");
    if (flowsList == NULL) {
        ogs_error("OpenAPI_direct_notification_report_convertToJSON() failed [flows]");
        goto end;
    }
    OpenAPI_list_for_each(direct_notification_report->flows, node) {
        cJSON *itemLocal = OpenAPI_flows_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_direct_notification_report_convertToJSON() failed [flows]");
            goto end;
        }
        cJSON_AddItemToArray(flowsList, itemLocal);
    }
    }

end:
    return item;
}

OpenAPI_direct_notification_report_t *OpenAPI_direct_notification_report_parseFromJSON(cJSON *direct_notification_reportJSON)
{
    OpenAPI_direct_notification_report_t *direct_notification_report_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *qos_mon_param_type = NULL;
    OpenAPI_qos_monitoring_param_type_e qos_mon_param_typeVariable = 0;
    cJSON *flows = NULL;
    OpenAPI_list_t *flowsList = NULL;
    qos_mon_param_type = cJSON_GetObjectItemCaseSensitive(direct_notification_reportJSON, "qosMonParamType");
    if (!qos_mon_param_type) {
        ogs_error("OpenAPI_direct_notification_report_parseFromJSON() failed [qos_mon_param_type]");
        goto end;
    }
    if (!cJSON_IsString(qos_mon_param_type)) {
        ogs_error("OpenAPI_direct_notification_report_parseFromJSON() failed [qos_mon_param_type]");
        goto end;
    }
    qos_mon_param_typeVariable = OpenAPI_qos_monitoring_param_type_FromString(qos_mon_param_type->valuestring);

    flows = cJSON_GetObjectItemCaseSensitive(direct_notification_reportJSON, "flows");
    if (flows) {
        cJSON *flows_local = NULL;
        if (!cJSON_IsArray(flows)) {
            ogs_error("OpenAPI_direct_notification_report_parseFromJSON() failed [flows]");
            goto end;
        }

        flowsList = OpenAPI_list_create();

        cJSON_ArrayForEach(flows_local, flows) {
            if (!cJSON_IsObject(flows_local)) {
                ogs_error("OpenAPI_direct_notification_report_parseFromJSON() failed [flows]");
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

    direct_notification_report_local_var = OpenAPI_direct_notification_report_create (
        qos_mon_param_typeVariable,
        flows ? flowsList : NULL
    );

    return direct_notification_report_local_var;
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

OpenAPI_direct_notification_report_t *OpenAPI_direct_notification_report_copy(OpenAPI_direct_notification_report_t *dst, OpenAPI_direct_notification_report_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_direct_notification_report_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_direct_notification_report_convertToJSON() failed");
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

    OpenAPI_direct_notification_report_free(dst);
    dst = OpenAPI_direct_notification_report_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

