
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sm_policy_notification.h"

OpenAPI_sm_policy_notification_t *OpenAPI_sm_policy_notification_create(
    char *resource_uri,
    OpenAPI_sm_policy_decision_t *sm_policy_decision
    )
{
    OpenAPI_sm_policy_notification_t *sm_policy_notification_local_var = OpenAPI_malloc(sizeof(OpenAPI_sm_policy_notification_t));
    if (!sm_policy_notification_local_var) {
        return NULL;
    }
    sm_policy_notification_local_var->resource_uri = resource_uri;
    sm_policy_notification_local_var->sm_policy_decision = sm_policy_decision;

    return sm_policy_notification_local_var;
}

void OpenAPI_sm_policy_notification_free(OpenAPI_sm_policy_notification_t *sm_policy_notification)
{
    if (NULL == sm_policy_notification) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(sm_policy_notification->resource_uri);
    OpenAPI_sm_policy_decision_free(sm_policy_notification->sm_policy_decision);
    ogs_free(sm_policy_notification);
}

cJSON *OpenAPI_sm_policy_notification_convertToJSON(OpenAPI_sm_policy_notification_t *sm_policy_notification)
{
    cJSON *item = NULL;

    if (sm_policy_notification == NULL) {
        ogs_error("OpenAPI_sm_policy_notification_convertToJSON() failed [SmPolicyNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (sm_policy_notification->resource_uri) {
        if (cJSON_AddStringToObject(item, "resourceUri", sm_policy_notification->resource_uri) == NULL) {
            ogs_error("OpenAPI_sm_policy_notification_convertToJSON() failed [resource_uri]");
            goto end;
        }
    }

    if (sm_policy_notification->sm_policy_decision) {
        cJSON *sm_policy_decision_local_JSON = OpenAPI_sm_policy_decision_convertToJSON(sm_policy_notification->sm_policy_decision);
        if (sm_policy_decision_local_JSON == NULL) {
            ogs_error("OpenAPI_sm_policy_notification_convertToJSON() failed [sm_policy_decision]");
            goto end;
        }
        cJSON_AddItemToObject(item, "smPolicyDecision", sm_policy_decision_local_JSON);
        if (item->child == NULL) {
            ogs_error("OpenAPI_sm_policy_notification_convertToJSON() failed [sm_policy_decision]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_sm_policy_notification_t *OpenAPI_sm_policy_notification_parseFromJSON(cJSON *sm_policy_notificationJSON)
{
    OpenAPI_sm_policy_notification_t *sm_policy_notification_local_var = NULL;
    cJSON *resource_uri = cJSON_GetObjectItemCaseSensitive(sm_policy_notificationJSON, "resourceUri");

    if (resource_uri) {
        if (!cJSON_IsString(resource_uri)) {
            ogs_error("OpenAPI_sm_policy_notification_parseFromJSON() failed [resource_uri]");
            goto end;
        }
    }

    cJSON *sm_policy_decision = cJSON_GetObjectItemCaseSensitive(sm_policy_notificationJSON, "smPolicyDecision");

    OpenAPI_sm_policy_decision_t *sm_policy_decision_local_nonprim = NULL;
    if (sm_policy_decision) {
        sm_policy_decision_local_nonprim = OpenAPI_sm_policy_decision_parseFromJSON(sm_policy_decision);
    }

    sm_policy_notification_local_var = OpenAPI_sm_policy_notification_create (
        resource_uri ? ogs_strdup(resource_uri->valuestring) : NULL,
        sm_policy_decision ? sm_policy_decision_local_nonprim : NULL
        );

    return sm_policy_notification_local_var;
end:
    return NULL;
}

OpenAPI_sm_policy_notification_t *OpenAPI_sm_policy_notification_copy(OpenAPI_sm_policy_notification_t *dst, OpenAPI_sm_policy_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sm_policy_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sm_policy_notification_convertToJSON() failed");
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

    OpenAPI_sm_policy_notification_free(dst);
    dst = OpenAPI_sm_policy_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

