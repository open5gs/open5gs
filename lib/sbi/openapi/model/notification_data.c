
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_data.h"

OpenAPI_notification_data_t *OpenAPI_notification_data_create(
    OpenAPI_notification_event_type_e event,
    char *nf_instance_uri,
    OpenAPI_nf_profile_t *nf_profile,
    OpenAPI_list_t *profile_changes,
    OpenAPI_condition_event_type_e condition_event,
    OpenAPI_subscription_context_t *subscription_context
)
{
    OpenAPI_notification_data_t *notification_data_local_var = ogs_malloc(sizeof(OpenAPI_notification_data_t));
    ogs_assert(notification_data_local_var);

    notification_data_local_var->event = event;
    notification_data_local_var->nf_instance_uri = nf_instance_uri;
    notification_data_local_var->nf_profile = nf_profile;
    notification_data_local_var->profile_changes = profile_changes;
    notification_data_local_var->condition_event = condition_event;
    notification_data_local_var->subscription_context = subscription_context;

    return notification_data_local_var;
}

void OpenAPI_notification_data_free(OpenAPI_notification_data_t *notification_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == notification_data) {
        return;
    }
    if (notification_data->nf_instance_uri) {
        ogs_free(notification_data->nf_instance_uri);
        notification_data->nf_instance_uri = NULL;
    }
    if (notification_data->nf_profile) {
        OpenAPI_nf_profile_free(notification_data->nf_profile);
        notification_data->nf_profile = NULL;
    }
    if (notification_data->profile_changes) {
        OpenAPI_list_for_each(notification_data->profile_changes, node) {
            OpenAPI_change_item_free(node->data);
        }
        OpenAPI_list_free(notification_data->profile_changes);
        notification_data->profile_changes = NULL;
    }
    if (notification_data->subscription_context) {
        OpenAPI_subscription_context_free(notification_data->subscription_context);
        notification_data->subscription_context = NULL;
    }
    ogs_free(notification_data);
}

cJSON *OpenAPI_notification_data_convertToJSON(OpenAPI_notification_data_t *notification_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (notification_data == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [NotificationData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (notification_data->event == OpenAPI_notification_event_type_NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [event]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "event", OpenAPI_notification_event_type_ToString(notification_data->event)) == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [event]");
        goto end;
    }

    if (!notification_data->nf_instance_uri) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [nf_instance_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "nfInstanceUri", notification_data->nf_instance_uri) == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [nf_instance_uri]");
        goto end;
    }

    if (notification_data->nf_profile) {
    cJSON *nf_profile_local_JSON = OpenAPI_nf_profile_convertToJSON(notification_data->nf_profile);
    if (nf_profile_local_JSON == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [nf_profile]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nfProfile", nf_profile_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [nf_profile]");
        goto end;
    }
    }

    if (notification_data->profile_changes) {
    cJSON *profile_changesList = cJSON_AddArrayToObject(item, "profileChanges");
    if (profile_changesList == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [profile_changes]");
        goto end;
    }
    OpenAPI_list_for_each(notification_data->profile_changes, node) {
        cJSON *itemLocal = OpenAPI_change_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_notification_data_convertToJSON() failed [profile_changes]");
            goto end;
        }
        cJSON_AddItemToArray(profile_changesList, itemLocal);
    }
    }

    if (notification_data->condition_event != OpenAPI_condition_event_type_NULL) {
    if (cJSON_AddStringToObject(item, "conditionEvent", OpenAPI_condition_event_type_ToString(notification_data->condition_event)) == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [condition_event]");
        goto end;
    }
    }

    if (notification_data->subscription_context) {
    cJSON *subscription_context_local_JSON = OpenAPI_subscription_context_convertToJSON(notification_data->subscription_context);
    if (subscription_context_local_JSON == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [subscription_context]");
        goto end;
    }
    cJSON_AddItemToObject(item, "subscriptionContext", subscription_context_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [subscription_context]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_notification_data_t *OpenAPI_notification_data_parseFromJSON(cJSON *notification_dataJSON)
{
    OpenAPI_notification_data_t *notification_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event = NULL;
    OpenAPI_notification_event_type_e eventVariable = 0;
    cJSON *nf_instance_uri = NULL;
    cJSON *nf_profile = NULL;
    OpenAPI_nf_profile_t *nf_profile_local_nonprim = NULL;
    cJSON *profile_changes = NULL;
    OpenAPI_list_t *profile_changesList = NULL;
    cJSON *condition_event = NULL;
    OpenAPI_condition_event_type_e condition_eventVariable = 0;
    cJSON *subscription_context = NULL;
    OpenAPI_subscription_context_t *subscription_context_local_nonprim = NULL;
    event = cJSON_GetObjectItemCaseSensitive(notification_dataJSON, "event");
    if (!event) {
        ogs_error("OpenAPI_notification_data_parseFromJSON() failed [event]");
        goto end;
    }
    if (!cJSON_IsString(event)) {
        ogs_error("OpenAPI_notification_data_parseFromJSON() failed [event]");
        goto end;
    }
    eventVariable = OpenAPI_notification_event_type_FromString(event->valuestring);

    nf_instance_uri = cJSON_GetObjectItemCaseSensitive(notification_dataJSON, "nfInstanceUri");
    if (!nf_instance_uri) {
        ogs_error("OpenAPI_notification_data_parseFromJSON() failed [nf_instance_uri]");
        goto end;
    }
    if (!cJSON_IsString(nf_instance_uri)) {
        ogs_error("OpenAPI_notification_data_parseFromJSON() failed [nf_instance_uri]");
        goto end;
    }

    nf_profile = cJSON_GetObjectItemCaseSensitive(notification_dataJSON, "nfProfile");
    if (nf_profile) {
    nf_profile_local_nonprim = OpenAPI_nf_profile_parseFromJSON(nf_profile);
    if (!nf_profile_local_nonprim) {
        ogs_error("OpenAPI_nf_profile_parseFromJSON failed [nf_profile]");
        goto end;
    }
    }

    profile_changes = cJSON_GetObjectItemCaseSensitive(notification_dataJSON, "profileChanges");
    if (profile_changes) {
        cJSON *profile_changes_local = NULL;
        if (!cJSON_IsArray(profile_changes)) {
            ogs_error("OpenAPI_notification_data_parseFromJSON() failed [profile_changes]");
            goto end;
        }

        profile_changesList = OpenAPI_list_create();

        cJSON_ArrayForEach(profile_changes_local, profile_changes) {
            if (!cJSON_IsObject(profile_changes_local)) {
                ogs_error("OpenAPI_notification_data_parseFromJSON() failed [profile_changes]");
                goto end;
            }
            OpenAPI_change_item_t *profile_changesItem = OpenAPI_change_item_parseFromJSON(profile_changes_local);
            if (!profile_changesItem) {
                ogs_error("No profile_changesItem");
                goto end;
            }
            OpenAPI_list_add(profile_changesList, profile_changesItem);
        }
    }

    condition_event = cJSON_GetObjectItemCaseSensitive(notification_dataJSON, "conditionEvent");
    if (condition_event) {
    if (!cJSON_IsString(condition_event)) {
        ogs_error("OpenAPI_notification_data_parseFromJSON() failed [condition_event]");
        goto end;
    }
    condition_eventVariable = OpenAPI_condition_event_type_FromString(condition_event->valuestring);
    }

    subscription_context = cJSON_GetObjectItemCaseSensitive(notification_dataJSON, "subscriptionContext");
    if (subscription_context) {
    subscription_context_local_nonprim = OpenAPI_subscription_context_parseFromJSON(subscription_context);
    if (!subscription_context_local_nonprim) {
        ogs_error("OpenAPI_subscription_context_parseFromJSON failed [subscription_context]");
        goto end;
    }
    }

    notification_data_local_var = OpenAPI_notification_data_create (
        eventVariable,
        ogs_strdup(nf_instance_uri->valuestring),
        nf_profile ? nf_profile_local_nonprim : NULL,
        profile_changes ? profile_changesList : NULL,
        condition_event ? condition_eventVariable : 0,
        subscription_context ? subscription_context_local_nonprim : NULL
    );

    return notification_data_local_var;
end:
    if (nf_profile_local_nonprim) {
        OpenAPI_nf_profile_free(nf_profile_local_nonprim);
        nf_profile_local_nonprim = NULL;
    }
    if (profile_changesList) {
        OpenAPI_list_for_each(profile_changesList, node) {
            OpenAPI_change_item_free(node->data);
        }
        OpenAPI_list_free(profile_changesList);
        profile_changesList = NULL;
    }
    if (subscription_context_local_nonprim) {
        OpenAPI_subscription_context_free(subscription_context_local_nonprim);
        subscription_context_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_notification_data_t *OpenAPI_notification_data_copy(OpenAPI_notification_data_t *dst, OpenAPI_notification_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_notification_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed");
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

    OpenAPI_notification_data_free(dst);
    dst = OpenAPI_notification_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

