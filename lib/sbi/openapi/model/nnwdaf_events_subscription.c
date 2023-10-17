
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nnwdaf_events_subscription.h"

OpenAPI_nnwdaf_events_subscription_t *OpenAPI_nnwdaf_events_subscription_create(
    OpenAPI_list_t *event_subscriptions,
    OpenAPI_reporting_information_t *evt_req,
    char *notification_uri,
    char *notif_corr_id,
    char *supported_features,
    OpenAPI_list_t *event_notifications,
    OpenAPI_list_t *fail_event_reports,
    OpenAPI_prev_sub_info_t *prev_sub,
    OpenAPI_consumer_nf_information_t *cons_nf_info
)
{
    OpenAPI_nnwdaf_events_subscription_t *nnwdaf_events_subscription_local_var = ogs_malloc(sizeof(OpenAPI_nnwdaf_events_subscription_t));
    ogs_assert(nnwdaf_events_subscription_local_var);

    nnwdaf_events_subscription_local_var->event_subscriptions = event_subscriptions;
    nnwdaf_events_subscription_local_var->evt_req = evt_req;
    nnwdaf_events_subscription_local_var->notification_uri = notification_uri;
    nnwdaf_events_subscription_local_var->notif_corr_id = notif_corr_id;
    nnwdaf_events_subscription_local_var->supported_features = supported_features;
    nnwdaf_events_subscription_local_var->event_notifications = event_notifications;
    nnwdaf_events_subscription_local_var->fail_event_reports = fail_event_reports;
    nnwdaf_events_subscription_local_var->prev_sub = prev_sub;
    nnwdaf_events_subscription_local_var->cons_nf_info = cons_nf_info;

    return nnwdaf_events_subscription_local_var;
}

void OpenAPI_nnwdaf_events_subscription_free(OpenAPI_nnwdaf_events_subscription_t *nnwdaf_events_subscription)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nnwdaf_events_subscription) {
        return;
    }
    if (nnwdaf_events_subscription->event_subscriptions) {
        OpenAPI_list_for_each(nnwdaf_events_subscription->event_subscriptions, node) {
            OpenAPI_event_subscription_free(node->data);
        }
        OpenAPI_list_free(nnwdaf_events_subscription->event_subscriptions);
        nnwdaf_events_subscription->event_subscriptions = NULL;
    }
    if (nnwdaf_events_subscription->evt_req) {
        OpenAPI_reporting_information_free(nnwdaf_events_subscription->evt_req);
        nnwdaf_events_subscription->evt_req = NULL;
    }
    if (nnwdaf_events_subscription->notification_uri) {
        ogs_free(nnwdaf_events_subscription->notification_uri);
        nnwdaf_events_subscription->notification_uri = NULL;
    }
    if (nnwdaf_events_subscription->notif_corr_id) {
        ogs_free(nnwdaf_events_subscription->notif_corr_id);
        nnwdaf_events_subscription->notif_corr_id = NULL;
    }
    if (nnwdaf_events_subscription->supported_features) {
        ogs_free(nnwdaf_events_subscription->supported_features);
        nnwdaf_events_subscription->supported_features = NULL;
    }
    if (nnwdaf_events_subscription->event_notifications) {
        OpenAPI_list_for_each(nnwdaf_events_subscription->event_notifications, node) {
            OpenAPI_event_notification_free(node->data);
        }
        OpenAPI_list_free(nnwdaf_events_subscription->event_notifications);
        nnwdaf_events_subscription->event_notifications = NULL;
    }
    if (nnwdaf_events_subscription->fail_event_reports) {
        OpenAPI_list_for_each(nnwdaf_events_subscription->fail_event_reports, node) {
            OpenAPI_failure_event_info_free(node->data);
        }
        OpenAPI_list_free(nnwdaf_events_subscription->fail_event_reports);
        nnwdaf_events_subscription->fail_event_reports = NULL;
    }
    if (nnwdaf_events_subscription->prev_sub) {
        OpenAPI_prev_sub_info_free(nnwdaf_events_subscription->prev_sub);
        nnwdaf_events_subscription->prev_sub = NULL;
    }
    if (nnwdaf_events_subscription->cons_nf_info) {
        OpenAPI_consumer_nf_information_free(nnwdaf_events_subscription->cons_nf_info);
        nnwdaf_events_subscription->cons_nf_info = NULL;
    }
    ogs_free(nnwdaf_events_subscription);
}

cJSON *OpenAPI_nnwdaf_events_subscription_convertToJSON(OpenAPI_nnwdaf_events_subscription_t *nnwdaf_events_subscription)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nnwdaf_events_subscription == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [NnwdafEventsSubscription]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!nnwdaf_events_subscription->event_subscriptions) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [event_subscriptions]");
        return NULL;
    }
    cJSON *event_subscriptionsList = cJSON_AddArrayToObject(item, "eventSubscriptions");
    if (event_subscriptionsList == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [event_subscriptions]");
        goto end;
    }
    OpenAPI_list_for_each(nnwdaf_events_subscription->event_subscriptions, node) {
        cJSON *itemLocal = OpenAPI_event_subscription_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [event_subscriptions]");
            goto end;
        }
        cJSON_AddItemToArray(event_subscriptionsList, itemLocal);
    }

    if (nnwdaf_events_subscription->evt_req) {
    cJSON *evt_req_local_JSON = OpenAPI_reporting_information_convertToJSON(nnwdaf_events_subscription->evt_req);
    if (evt_req_local_JSON == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [evt_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "evtReq", evt_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [evt_req]");
        goto end;
    }
    }

    if (nnwdaf_events_subscription->notification_uri) {
    if (cJSON_AddStringToObject(item, "notificationURI", nnwdaf_events_subscription->notification_uri) == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [notification_uri]");
        goto end;
    }
    }

    if (nnwdaf_events_subscription->notif_corr_id) {
    if (cJSON_AddStringToObject(item, "notifCorrId", nnwdaf_events_subscription->notif_corr_id) == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [notif_corr_id]");
        goto end;
    }
    }

    if (nnwdaf_events_subscription->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", nnwdaf_events_subscription->supported_features) == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (nnwdaf_events_subscription->event_notifications) {
    cJSON *event_notificationsList = cJSON_AddArrayToObject(item, "eventNotifications");
    if (event_notificationsList == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [event_notifications]");
        goto end;
    }
    OpenAPI_list_for_each(nnwdaf_events_subscription->event_notifications, node) {
        cJSON *itemLocal = OpenAPI_event_notification_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [event_notifications]");
            goto end;
        }
        cJSON_AddItemToArray(event_notificationsList, itemLocal);
    }
    }

    if (nnwdaf_events_subscription->fail_event_reports) {
    cJSON *fail_event_reportsList = cJSON_AddArrayToObject(item, "failEventReports");
    if (fail_event_reportsList == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [fail_event_reports]");
        goto end;
    }
    OpenAPI_list_for_each(nnwdaf_events_subscription->fail_event_reports, node) {
        cJSON *itemLocal = OpenAPI_failure_event_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [fail_event_reports]");
            goto end;
        }
        cJSON_AddItemToArray(fail_event_reportsList, itemLocal);
    }
    }

    if (nnwdaf_events_subscription->prev_sub) {
    cJSON *prev_sub_local_JSON = OpenAPI_prev_sub_info_convertToJSON(nnwdaf_events_subscription->prev_sub);
    if (prev_sub_local_JSON == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [prev_sub]");
        goto end;
    }
    cJSON_AddItemToObject(item, "prevSub", prev_sub_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [prev_sub]");
        goto end;
    }
    }

    if (nnwdaf_events_subscription->cons_nf_info) {
    cJSON *cons_nf_info_local_JSON = OpenAPI_consumer_nf_information_convertToJSON(nnwdaf_events_subscription->cons_nf_info);
    if (cons_nf_info_local_JSON == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [cons_nf_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "consNfInfo", cons_nf_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed [cons_nf_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nnwdaf_events_subscription_t *OpenAPI_nnwdaf_events_subscription_parseFromJSON(cJSON *nnwdaf_events_subscriptionJSON)
{
    OpenAPI_nnwdaf_events_subscription_t *nnwdaf_events_subscription_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event_subscriptions = NULL;
    OpenAPI_list_t *event_subscriptionsList = NULL;
    cJSON *evt_req = NULL;
    OpenAPI_reporting_information_t *evt_req_local_nonprim = NULL;
    cJSON *notification_uri = NULL;
    cJSON *notif_corr_id = NULL;
    cJSON *supported_features = NULL;
    cJSON *event_notifications = NULL;
    OpenAPI_list_t *event_notificationsList = NULL;
    cJSON *fail_event_reports = NULL;
    OpenAPI_list_t *fail_event_reportsList = NULL;
    cJSON *prev_sub = NULL;
    OpenAPI_prev_sub_info_t *prev_sub_local_nonprim = NULL;
    cJSON *cons_nf_info = NULL;
    OpenAPI_consumer_nf_information_t *cons_nf_info_local_nonprim = NULL;
    event_subscriptions = cJSON_GetObjectItemCaseSensitive(nnwdaf_events_subscriptionJSON, "eventSubscriptions");
    if (!event_subscriptions) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_parseFromJSON() failed [event_subscriptions]");
        goto end;
    }
        cJSON *event_subscriptions_local = NULL;
        if (!cJSON_IsArray(event_subscriptions)) {
            ogs_error("OpenAPI_nnwdaf_events_subscription_parseFromJSON() failed [event_subscriptions]");
            goto end;
        }

        event_subscriptionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_subscriptions_local, event_subscriptions) {
            if (!cJSON_IsObject(event_subscriptions_local)) {
                ogs_error("OpenAPI_nnwdaf_events_subscription_parseFromJSON() failed [event_subscriptions]");
                goto end;
            }
            OpenAPI_event_subscription_t *event_subscriptionsItem = OpenAPI_event_subscription_parseFromJSON(event_subscriptions_local);
            if (!event_subscriptionsItem) {
                ogs_error("No event_subscriptionsItem");
                goto end;
            }
            OpenAPI_list_add(event_subscriptionsList, event_subscriptionsItem);
        }

    evt_req = cJSON_GetObjectItemCaseSensitive(nnwdaf_events_subscriptionJSON, "evtReq");
    if (evt_req) {
    evt_req_local_nonprim = OpenAPI_reporting_information_parseFromJSON(evt_req);
    if (!evt_req_local_nonprim) {
        ogs_error("OpenAPI_reporting_information_parseFromJSON failed [evt_req]");
        goto end;
    }
    }

    notification_uri = cJSON_GetObjectItemCaseSensitive(nnwdaf_events_subscriptionJSON, "notificationURI");
    if (notification_uri) {
    if (!cJSON_IsString(notification_uri) && !cJSON_IsNull(notification_uri)) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_parseFromJSON() failed [notification_uri]");
        goto end;
    }
    }

    notif_corr_id = cJSON_GetObjectItemCaseSensitive(nnwdaf_events_subscriptionJSON, "notifCorrId");
    if (notif_corr_id) {
    if (!cJSON_IsString(notif_corr_id) && !cJSON_IsNull(notif_corr_id)) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_parseFromJSON() failed [notif_corr_id]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(nnwdaf_events_subscriptionJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    event_notifications = cJSON_GetObjectItemCaseSensitive(nnwdaf_events_subscriptionJSON, "eventNotifications");
    if (event_notifications) {
        cJSON *event_notifications_local = NULL;
        if (!cJSON_IsArray(event_notifications)) {
            ogs_error("OpenAPI_nnwdaf_events_subscription_parseFromJSON() failed [event_notifications]");
            goto end;
        }

        event_notificationsList = OpenAPI_list_create();

        cJSON_ArrayForEach(event_notifications_local, event_notifications) {
            if (!cJSON_IsObject(event_notifications_local)) {
                ogs_error("OpenAPI_nnwdaf_events_subscription_parseFromJSON() failed [event_notifications]");
                goto end;
            }
            OpenAPI_event_notification_t *event_notificationsItem = OpenAPI_event_notification_parseFromJSON(event_notifications_local);
            if (!event_notificationsItem) {
                ogs_error("No event_notificationsItem");
                goto end;
            }
            OpenAPI_list_add(event_notificationsList, event_notificationsItem);
        }
    }

    fail_event_reports = cJSON_GetObjectItemCaseSensitive(nnwdaf_events_subscriptionJSON, "failEventReports");
    if (fail_event_reports) {
        cJSON *fail_event_reports_local = NULL;
        if (!cJSON_IsArray(fail_event_reports)) {
            ogs_error("OpenAPI_nnwdaf_events_subscription_parseFromJSON() failed [fail_event_reports]");
            goto end;
        }

        fail_event_reportsList = OpenAPI_list_create();

        cJSON_ArrayForEach(fail_event_reports_local, fail_event_reports) {
            if (!cJSON_IsObject(fail_event_reports_local)) {
                ogs_error("OpenAPI_nnwdaf_events_subscription_parseFromJSON() failed [fail_event_reports]");
                goto end;
            }
            OpenAPI_failure_event_info_t *fail_event_reportsItem = OpenAPI_failure_event_info_parseFromJSON(fail_event_reports_local);
            if (!fail_event_reportsItem) {
                ogs_error("No fail_event_reportsItem");
                goto end;
            }
            OpenAPI_list_add(fail_event_reportsList, fail_event_reportsItem);
        }
    }

    prev_sub = cJSON_GetObjectItemCaseSensitive(nnwdaf_events_subscriptionJSON, "prevSub");
    if (prev_sub) {
    prev_sub_local_nonprim = OpenAPI_prev_sub_info_parseFromJSON(prev_sub);
    if (!prev_sub_local_nonprim) {
        ogs_error("OpenAPI_prev_sub_info_parseFromJSON failed [prev_sub]");
        goto end;
    }
    }

    cons_nf_info = cJSON_GetObjectItemCaseSensitive(nnwdaf_events_subscriptionJSON, "consNfInfo");
    if (cons_nf_info) {
    cons_nf_info_local_nonprim = OpenAPI_consumer_nf_information_parseFromJSON(cons_nf_info);
    if (!cons_nf_info_local_nonprim) {
        ogs_error("OpenAPI_consumer_nf_information_parseFromJSON failed [cons_nf_info]");
        goto end;
    }
    }

    nnwdaf_events_subscription_local_var = OpenAPI_nnwdaf_events_subscription_create (
        event_subscriptionsList,
        evt_req ? evt_req_local_nonprim : NULL,
        notification_uri && !cJSON_IsNull(notification_uri) ? ogs_strdup(notification_uri->valuestring) : NULL,
        notif_corr_id && !cJSON_IsNull(notif_corr_id) ? ogs_strdup(notif_corr_id->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        event_notifications ? event_notificationsList : NULL,
        fail_event_reports ? fail_event_reportsList : NULL,
        prev_sub ? prev_sub_local_nonprim : NULL,
        cons_nf_info ? cons_nf_info_local_nonprim : NULL
    );

    return nnwdaf_events_subscription_local_var;
end:
    if (event_subscriptionsList) {
        OpenAPI_list_for_each(event_subscriptionsList, node) {
            OpenAPI_event_subscription_free(node->data);
        }
        OpenAPI_list_free(event_subscriptionsList);
        event_subscriptionsList = NULL;
    }
    if (evt_req_local_nonprim) {
        OpenAPI_reporting_information_free(evt_req_local_nonprim);
        evt_req_local_nonprim = NULL;
    }
    if (event_notificationsList) {
        OpenAPI_list_for_each(event_notificationsList, node) {
            OpenAPI_event_notification_free(node->data);
        }
        OpenAPI_list_free(event_notificationsList);
        event_notificationsList = NULL;
    }
    if (fail_event_reportsList) {
        OpenAPI_list_for_each(fail_event_reportsList, node) {
            OpenAPI_failure_event_info_free(node->data);
        }
        OpenAPI_list_free(fail_event_reportsList);
        fail_event_reportsList = NULL;
    }
    if (prev_sub_local_nonprim) {
        OpenAPI_prev_sub_info_free(prev_sub_local_nonprim);
        prev_sub_local_nonprim = NULL;
    }
    if (cons_nf_info_local_nonprim) {
        OpenAPI_consumer_nf_information_free(cons_nf_info_local_nonprim);
        cons_nf_info_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nnwdaf_events_subscription_t *OpenAPI_nnwdaf_events_subscription_copy(OpenAPI_nnwdaf_events_subscription_t *dst, OpenAPI_nnwdaf_events_subscription_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nnwdaf_events_subscription_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nnwdaf_events_subscription_convertToJSON() failed");
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

    OpenAPI_nnwdaf_events_subscription_free(dst);
    dst = OpenAPI_nnwdaf_events_subscription_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

