
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "af_event_subscription.h"

OpenAPI_af_event_subscription_t *OpenAPI_af_event_subscription_create(
    OpenAPI_af_event_t *event,
    OpenAPI_af_notif_method_e notif_method,
    int rep_period,
    int wait_time
    )
{
    OpenAPI_af_event_subscription_t *af_event_subscription_local_var = OpenAPI_malloc(sizeof(OpenAPI_af_event_subscription_t));
    if (!af_event_subscription_local_var) {
        return NULL;
    }
    af_event_subscription_local_var->event = event;
    af_event_subscription_local_var->notif_method = notif_method;
    af_event_subscription_local_var->rep_period = rep_period;
    af_event_subscription_local_var->wait_time = wait_time;

    return af_event_subscription_local_var;
}

void OpenAPI_af_event_subscription_free(OpenAPI_af_event_subscription_t *af_event_subscription)
{
    if (NULL == af_event_subscription) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_af_event_free(af_event_subscription->event);
    ogs_free(af_event_subscription);
}

cJSON *OpenAPI_af_event_subscription_convertToJSON(OpenAPI_af_event_subscription_t *af_event_subscription)
{
    cJSON *item = NULL;

    if (af_event_subscription == NULL) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [AfEventSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *event_local_JSON = OpenAPI_af_event_convertToJSON(af_event_subscription->event);
    if (event_local_JSON == NULL) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [event]");
        goto end;
    }
    cJSON_AddItemToObject(item, "event", event_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [event]");
        goto end;
    }

    if (af_event_subscription->notif_method) {
        if (cJSON_AddStringToObject(item, "notifMethod", OpenAPI_af_notif_method_ToString(af_event_subscription->notif_method)) == NULL) {
            ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [notif_method]");
            goto end;
        }
    }

    if (af_event_subscription->rep_period) {
        if (cJSON_AddNumberToObject(item, "repPeriod", af_event_subscription->rep_period) == NULL) {
            ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [rep_period]");
            goto end;
        }
    }

    if (af_event_subscription->wait_time) {
        if (cJSON_AddNumberToObject(item, "waitTime", af_event_subscription->wait_time) == NULL) {
            ogs_error("OpenAPI_af_event_subscription_convertToJSON() failed [wait_time]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_af_event_subscription_t *OpenAPI_af_event_subscription_parseFromJSON(cJSON *af_event_subscriptionJSON)
{
    OpenAPI_af_event_subscription_t *af_event_subscription_local_var = NULL;
    cJSON *event = cJSON_GetObjectItemCaseSensitive(af_event_subscriptionJSON, "event");
    if (!event) {
        ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [event]");
        goto end;
    }

    OpenAPI_af_event_t *event_local_nonprim = NULL;

    event_local_nonprim = OpenAPI_af_event_parseFromJSON(event);

    cJSON *notif_method = cJSON_GetObjectItemCaseSensitive(af_event_subscriptionJSON, "notifMethod");

    OpenAPI_af_notif_method_e notif_methodVariable;
    if (notif_method) {
        if (!cJSON_IsString(notif_method)) {
            ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [notif_method]");
            goto end;
        }
        notif_methodVariable = OpenAPI_af_notif_method_FromString(notif_method->valuestring);
    }

    cJSON *rep_period = cJSON_GetObjectItemCaseSensitive(af_event_subscriptionJSON, "repPeriod");

    if (rep_period) {
        if (!cJSON_IsNumber(rep_period)) {
            ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [rep_period]");
            goto end;
        }
    }

    cJSON *wait_time = cJSON_GetObjectItemCaseSensitive(af_event_subscriptionJSON, "waitTime");

    if (wait_time) {
        if (!cJSON_IsNumber(wait_time)) {
            ogs_error("OpenAPI_af_event_subscription_parseFromJSON() failed [wait_time]");
            goto end;
        }
    }

    af_event_subscription_local_var = OpenAPI_af_event_subscription_create (
        event_local_nonprim,
        notif_method ? notif_methodVariable : 0,
        rep_period ? rep_period->valuedouble : 0,
        wait_time ? wait_time->valuedouble : 0
        );

    return af_event_subscription_local_var;
end:
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

