
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_event_subscription.h"

OpenAPI_af_event_subscription_t *OpenAPI_af_event_subscription_create(
    OpenAPI_npcf_af_event_e event,
    OpenAPI_af_notif_method_e notif_method,
    bool is_rep_period,
    int rep_period,
    bool is_wait_time,
    int wait_time,
    OpenAPI_qos_monitoring_param_type_e qos_mon_param_type,
    OpenAPI_list_t *cap_types
)
{
    OpenAPI_af_event_subscription_t *af_event_subscription_local_var = ogs_malloc(sizeof(OpenAPI_af_event_subscription_t));
    ogs_assert(af_event_subscription_local_var);

    af_event_subscription_local_var->event = event;
    af_event_subscription_local_var->notif_method = notif_method;
    af_event_subscription_local_var->is_rep_period = is_rep_period;
    af_event_subscription_local_var->rep_period = rep_period;
    af_event_subscription_local_var->is_wait_time = is_wait_time;
    af_event_subscription_local_var->wait_time = wait_time;
    af_event_subscription_local_var->qos_mon_param_type = qos_mon_param_type;
    af_event_subscription_local_var->cap_types = cap_types;

    return af_event_subscription_local_var;
}

void OpenAPI_af_event_subscription_free(OpenAPI_af_event_subscription_t *af_event_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == af_event_subscription) {
        return;
    }
    if (af_event_subscription->cap_types) {
        OpenAPI_list_free(af_event_subscription->cap_types);
        af_event_subscription->cap_types = NULL;
    }
    ogs_free(af_event_subscription);
}

cJSON *OpenAPI_af_event_subscription_convertToJSON(OpenAPI_af_event_subscription_t *af_event_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (af_event_subscription == NULL) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [AfEventSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (af_event_subscription->event == OpenAPI_npcf_af_event_NULL) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [event]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "event", OpenAPI_npcf_af_event_ToString(af_event_subscription->event)) == NULL) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [event]");
        goto end;
    }

    if (af_event_subscription->notif_method != OpenAPI_af_notif_method_NULL) {
    if (cJSON_AddStringToObject(item, "notifMethod", OpenAPI_af_notif_method_ToString(af_event_subscription->notif_method)) == NULL) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [notif_method]");
        goto end;
    }
    }

    if (af_event_subscription->is_rep_period) {
    if (cJSON_AddNumberToObject(item, "repPeriod", af_event_subscription->rep_period) == NULL) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [rep_period]");
        goto end;
    }
    }

    if (af_event_subscription->is_wait_time) {
    if (cJSON_AddNumberToObject(item, "waitTime", af_event_subscription->wait_time) == NULL) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [wait_time]");
        goto end;
    }
    }

    if (af_event_subscription->qos_mon_param_type != OpenAPI_qos_monitoring_param_type_NULL) {
    if (cJSON_AddStringToObject(item, "qosMonParamType", OpenAPI_qos_monitoring_param_type_ToString(af_event_subscription->qos_mon_param_type)) == NULL) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [qos_mon_param_type]");
        goto end;
    }
    }

    if (af_event_subscription->cap_types != OpenAPI_notif_cap_type_NULL) {
    cJSON *cap_typesList = cJSON_AddArrayToObject(item, "capTypes");
    if (cap_typesList == NULL) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [cap_types]");
        goto end;
    }
    OpenAPI_list_for_each(af_event_subscription->cap_types, node) {
        if (cJSON_AddStringToObject(cap_typesList, "", OpenAPI_notif_cap_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [cap_types]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_af_event_subscription_t *OpenAPI_af_event_subscription_parseFromJSON(cJSON *af_event_subscriptionJSON)
{
    OpenAPI_af_event_subscription_t *af_event_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event = NULL;
    OpenAPI_npcf_af_event_e eventVariable = 0;
    cJSON *notif_method = NULL;
    OpenAPI_af_notif_method_e notif_methodVariable = 0;
    cJSON *rep_period = NULL;
    cJSON *wait_time = NULL;
    cJSON *qos_mon_param_type = NULL;
    OpenAPI_qos_monitoring_param_type_e qos_mon_param_typeVariable = 0;
    cJSON *cap_types = NULL;
    OpenAPI_list_t *cap_typesList = NULL;
    event = cJSON_GetObjectItemCaseSensitive(af_event_subscriptionJSON, "event");
    if (!event) {
        ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [event]");
        goto end;
    }
    if (!cJSON_IsString(event)) {
        ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [event]");
        goto end;
    }
    eventVariable = OpenAPI_npcf_af_event_FromString(event->valuestring);

    notif_method = cJSON_GetObjectItemCaseSensitive(af_event_subscriptionJSON, "notifMethod");
    if (notif_method) {
    if (!cJSON_IsString(notif_method)) {
        ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [notif_method]");
        goto end;
    }
    notif_methodVariable = OpenAPI_af_notif_method_FromString(notif_method->valuestring);
    }

    rep_period = cJSON_GetObjectItemCaseSensitive(af_event_subscriptionJSON, "repPeriod");
    if (rep_period) {
    if (!cJSON_IsNumber(rep_period)) {
        ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [rep_period]");
        goto end;
    }
    }

    wait_time = cJSON_GetObjectItemCaseSensitive(af_event_subscriptionJSON, "waitTime");
    if (wait_time) {
    if (!cJSON_IsNumber(wait_time)) {
        ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [wait_time]");
        goto end;
    }
    }

    qos_mon_param_type = cJSON_GetObjectItemCaseSensitive(af_event_subscriptionJSON, "qosMonParamType");
    if (qos_mon_param_type) {
    if (!cJSON_IsString(qos_mon_param_type)) {
        ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [qos_mon_param_type]");
        goto end;
    }
    qos_mon_param_typeVariable = OpenAPI_qos_monitoring_param_type_FromString(qos_mon_param_type->valuestring);
    }

    cap_types = cJSON_GetObjectItemCaseSensitive(af_event_subscriptionJSON, "capTypes");
    if (cap_types) {
        cJSON *cap_types_local = NULL;
        if (!cJSON_IsArray(cap_types)) {
            ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [cap_types]");
            goto end;
        }

        cap_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(cap_types_local, cap_types) {
            OpenAPI_notif_cap_type_e localEnum = OpenAPI_notif_cap_type_NULL;
            if (!cJSON_IsString(cap_types_local)) {
                ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [cap_types]");
                goto end;
            }
            localEnum = OpenAPI_notif_cap_type_FromString(cap_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"cap_types\" is not supported. Ignoring it ...",
                         cap_types_local->valuestring);
            } else {
                OpenAPI_list_add(cap_typesList, (void *)localEnum);
            }
        }
        if (cap_typesList->count == 0) {
            ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed: Expected cap_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    af_event_subscription_local_var = OpenAPI_af_event_subscription_create (
        eventVariable,
        notif_method ? notif_methodVariable : 0,
        rep_period ? true : false,
        rep_period ? rep_period->valuedouble : 0,
        wait_time ? true : false,
        wait_time ? wait_time->valuedouble : 0,
        qos_mon_param_type ? qos_mon_param_typeVariable : 0,
        cap_types ? cap_typesList : NULL
    );

    return af_event_subscription_local_var;
end:
    if (cap_typesList) {
        OpenAPI_list_free(cap_typesList);
        cap_typesList = NULL;
    }
    return NULL;
}

OpenAPI_af_event_subscription_t *OpenAPI_af_event_subscription_copy(OpenAPI_af_event_subscription_t *dst, OpenAPI_af_event_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_af_event_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed");
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

    OpenAPI_af_event_subscription_free(dst);
    dst = OpenAPI_af_event_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

