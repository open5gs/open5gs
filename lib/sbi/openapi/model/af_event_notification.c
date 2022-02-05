
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_event_notification.h"

OpenAPI_af_event_notification_t *OpenAPI_af_event_notification_create(
    OpenAPI_af_event_e event,
    OpenAPI_list_t *flows
)
{
    OpenAPI_af_event_notification_t *af_event_notification_local_var = ogs_malloc(sizeof(OpenAPI_af_event_notification_t));
    ogs_assert(af_event_notification_local_var);

    af_event_notification_local_var->event = event;
    af_event_notification_local_var->flows = flows;

    return af_event_notification_local_var;
}

void OpenAPI_af_event_notification_free(OpenAPI_af_event_notification_t *af_event_notification)
{
    if (NULL == af_event_notification) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(af_event_notification->flows, node) {
        OpenAPI_flows_free(node->data);
    }
    OpenAPI_list_free(af_event_notification->flows);
    ogs_free(af_event_notification);
}

cJSON *OpenAPI_af_event_notification_convertToJSON(OpenAPI_af_event_notification_t *af_event_notification)
{
    cJSON *item = NULL;

    if (af_event_notification == NULL) {
        ogs_error("OpenAPI_af_event_notification_convertToJSON() failed [AfEventNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "event", OpenAPI_af_event_ToString(af_event_notification->event)) == NULL) {
        ogs_error("OpenAPI_af_event_notification_convertToJSON() failed [event]");
        goto end;
    }

    if (af_event_notification->flows) {
    cJSON *flowsList = cJSON_AddArrayToObject(item, "flows");
    if (flowsList == NULL) {
        ogs_error("OpenAPI_af_event_notification_convertToJSON() failed [flows]");
        goto end;
    }

    OpenAPI_lnode_t *flows_node;
    if (af_event_notification->flows) {
        OpenAPI_list_for_each(af_event_notification->flows, flows_node) {
            cJSON *itemLocal = OpenAPI_flows_convertToJSON(flows_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_af_event_notification_convertToJSON() failed [flows]");
                goto end;
            }
            cJSON_AddItemToArray(flowsList, itemLocal);
        }
    }
    }

end:
    return item;
}

OpenAPI_af_event_notification_t *OpenAPI_af_event_notification_parseFromJSON(cJSON *af_event_notificationJSON)
{
    OpenAPI_af_event_notification_t *af_event_notification_local_var = NULL;
    cJSON *event = cJSON_GetObjectItemCaseSensitive(af_event_notificationJSON, "event");
    if (!event) {
        ogs_error("OpenAPI_af_event_notification_parseFromJSON() failed [event]");
        goto end;
    }

    OpenAPI_af_event_e eventVariable;
    if (!cJSON_IsString(event)) {
        ogs_error("OpenAPI_af_event_notification_parseFromJSON() failed [event]");
        goto end;
    }
    eventVariable = OpenAPI_af_event_FromString(event->valuestring);

    cJSON *flows = cJSON_GetObjectItemCaseSensitive(af_event_notificationJSON, "flows");

    OpenAPI_list_t *flowsList;
    if (flows) {
    cJSON *flows_local_nonprimitive;
    if (!cJSON_IsArray(flows)){
        ogs_error("OpenAPI_af_event_notification_parseFromJSON() failed [flows]");
        goto end;
    }

    flowsList = OpenAPI_list_create();

    cJSON_ArrayForEach(flows_local_nonprimitive, flows ) {
        if (!cJSON_IsObject(flows_local_nonprimitive)) {
            ogs_error("OpenAPI_af_event_notification_parseFromJSON() failed [flows]");
            goto end;
        }
        OpenAPI_flows_t *flowsItem = OpenAPI_flows_parseFromJSON(flows_local_nonprimitive);

        if (!flowsItem) {
            ogs_error("No flowsItem");
            OpenAPI_list_free(flowsList);
            goto end;
        }

        OpenAPI_list_add(flowsList, flowsItem);
    }
    }

    af_event_notification_local_var = OpenAPI_af_event_notification_create (
        eventVariable,
        flows ? flowsList : NULL
    );

    return af_event_notification_local_var;
end:
    return NULL;
}

OpenAPI_af_event_notification_t *OpenAPI_af_event_notification_copy(OpenAPI_af_event_notification_t *dst, OpenAPI_af_event_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_event_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_event_notification_convertToJSON() failed");
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

    OpenAPI_af_event_notification_free(dst);
    dst = OpenAPI_af_event_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

