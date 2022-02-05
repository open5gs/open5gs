
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "notification_data.h"

OpenAPI_notification_data_t *OpenAPI_notification_data_create(
    OpenAPI_notification_event_type_e event,
    char *nf_instance_uri,
    OpenAPI_nf_profile_t *nf_profile,
    OpenAPI_list_t *profile_changes,
    OpenAPI_condition_event_type_e condition_event
)
{
    OpenAPI_notification_data_t *notification_data_local_var = ogs_malloc(sizeof(OpenAPI_notification_data_t));
    ogs_assert(notification_data_local_var);

    notification_data_local_var->event = event;
    notification_data_local_var->nf_instance_uri = nf_instance_uri;
    notification_data_local_var->nf_profile = nf_profile;
    notification_data_local_var->profile_changes = profile_changes;
    notification_data_local_var->condition_event = condition_event;

    return notification_data_local_var;
}

void OpenAPI_notification_data_free(OpenAPI_notification_data_t *notification_data)
{
    if (NULL == notification_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(notification_data->nf_instance_uri);
    OpenAPI_nf_profile_free(notification_data->nf_profile);
    OpenAPI_list_for_each(notification_data->profile_changes, node) {
        OpenAPI_change_item_free(node->data);
    }
    OpenAPI_list_free(notification_data->profile_changes);
    ogs_free(notification_data);
}

cJSON *OpenAPI_notification_data_convertToJSON(OpenAPI_notification_data_t *notification_data)
{
    cJSON *item = NULL;

    if (notification_data == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [NotificationData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddStringToObject(item, "event", OpenAPI_notification_event_type_ToString(notification_data->event)) == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [event]");
        goto end;
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

    OpenAPI_lnode_t *profile_changes_node;
    if (notification_data->profile_changes) {
        OpenAPI_list_for_each(notification_data->profile_changes, profile_changes_node) {
            cJSON *itemLocal = OpenAPI_change_item_convertToJSON(profile_changes_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_notification_data_convertToJSON() failed [profile_changes]");
                goto end;
            }
            cJSON_AddItemToArray(profile_changesList, itemLocal);
        }
    }
    }

    if (notification_data->condition_event) {
    if (cJSON_AddStringToObject(item, "conditionEvent", OpenAPI_condition_event_type_ToString(notification_data->condition_event)) == NULL) {
        ogs_error("OpenAPI_notification_data_convertToJSON() failed [condition_event]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_notification_data_t *OpenAPI_notification_data_parseFromJSON(cJSON *notification_dataJSON)
{
    OpenAPI_notification_data_t *notification_data_local_var = NULL;
    cJSON *event = cJSON_GetObjectItemCaseSensitive(notification_dataJSON, "event");
    if (!event) {
        ogs_error("OpenAPI_notification_data_parseFromJSON() failed [event]");
        goto end;
    }

    OpenAPI_notification_event_type_e eventVariable;
    if (!cJSON_IsString(event)) {
        ogs_error("OpenAPI_notification_data_parseFromJSON() failed [event]");
        goto end;
    }
    eventVariable = OpenAPI_notification_event_type_FromString(event->valuestring);

    cJSON *nf_instance_uri = cJSON_GetObjectItemCaseSensitive(notification_dataJSON, "nfInstanceUri");
    if (!nf_instance_uri) {
        ogs_error("OpenAPI_notification_data_parseFromJSON() failed [nf_instance_uri]");
        goto end;
    }

    if (!cJSON_IsString(nf_instance_uri)) {
        ogs_error("OpenAPI_notification_data_parseFromJSON() failed [nf_instance_uri]");
        goto end;
    }

    cJSON *nf_profile = cJSON_GetObjectItemCaseSensitive(notification_dataJSON, "nfProfile");

    OpenAPI_nf_profile_t *nf_profile_local_nonprim = NULL;
    if (nf_profile) {
    nf_profile_local_nonprim = OpenAPI_nf_profile_parseFromJSON(nf_profile);
    }

    cJSON *profile_changes = cJSON_GetObjectItemCaseSensitive(notification_dataJSON, "profileChanges");

    OpenAPI_list_t *profile_changesList;
    if (profile_changes) {
    cJSON *profile_changes_local_nonprimitive;
    if (!cJSON_IsArray(profile_changes)){
        ogs_error("OpenAPI_notification_data_parseFromJSON() failed [profile_changes]");
        goto end;
    }

    profile_changesList = OpenAPI_list_create();

    cJSON_ArrayForEach(profile_changes_local_nonprimitive, profile_changes ) {
        if (!cJSON_IsObject(profile_changes_local_nonprimitive)) {
            ogs_error("OpenAPI_notification_data_parseFromJSON() failed [profile_changes]");
            goto end;
        }
        OpenAPI_change_item_t *profile_changesItem = OpenAPI_change_item_parseFromJSON(profile_changes_local_nonprimitive);

        if (!profile_changesItem) {
            ogs_error("No profile_changesItem");
            OpenAPI_list_free(profile_changesList);
            goto end;
        }

        OpenAPI_list_add(profile_changesList, profile_changesItem);
    }
    }

    cJSON *condition_event = cJSON_GetObjectItemCaseSensitive(notification_dataJSON, "conditionEvent");

    OpenAPI_condition_event_type_e condition_eventVariable;
    if (condition_event) {
    if (!cJSON_IsString(condition_event)) {
        ogs_error("OpenAPI_notification_data_parseFromJSON() failed [condition_event]");
        goto end;
    }
    condition_eventVariable = OpenAPI_condition_event_type_FromString(condition_event->valuestring);
    }

    notification_data_local_var = OpenAPI_notification_data_create (
        eventVariable,
        ogs_strdup(nf_instance_uri->valuestring),
        nf_profile ? nf_profile_local_nonprim : NULL,
        profile_changes ? profile_changesList : NULL,
        condition_event ? condition_eventVariable : 0
    );

    return notification_data_local_var;
end:
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

