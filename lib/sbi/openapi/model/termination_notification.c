
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "termination_notification.h"

OpenAPI_termination_notification_t *OpenAPI_termination_notification_create(
    char *resource_uri,
    OpenAPI_sm_policy_association_release_cause_t *cause
    )
{
    OpenAPI_termination_notification_t *termination_notification_local_var = OpenAPI_malloc(sizeof(OpenAPI_termination_notification_t));
    if (!termination_notification_local_var) {
        return NULL;
    }
    termination_notification_local_var->resource_uri = resource_uri;
    termination_notification_local_var->cause = cause;

    return termination_notification_local_var;
}

void OpenAPI_termination_notification_free(OpenAPI_termination_notification_t *termination_notification)
{
    if (NULL == termination_notification) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(termination_notification->resource_uri);
    OpenAPI_sm_policy_association_release_cause_free(termination_notification->cause);
    ogs_free(termination_notification);
}

cJSON *OpenAPI_termination_notification_convertToJSON(OpenAPI_termination_notification_t *termination_notification)
{
    cJSON *item = NULL;

    if (termination_notification == NULL) {
        ogs_error("OpenAPI_termination_notification_convertToJSON() failed [TerminationNotification]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!termination_notification->resource_uri) {
        ogs_error("OpenAPI_termination_notification_convertToJSON() failed [resource_uri]");
        goto end;
    }
    if (cJSON_AddStringToObject(item, "resourceUri", termination_notification->resource_uri) == NULL) {
        ogs_error("OpenAPI_termination_notification_convertToJSON() failed [resource_uri]");
        goto end;
    }

    if (!termination_notification->cause) {
        ogs_error("OpenAPI_termination_notification_convertToJSON() failed [cause]");
        goto end;
    }
    cJSON *cause_local_JSON = OpenAPI_sm_policy_association_release_cause_convertToJSON(termination_notification->cause);
    if (cause_local_JSON == NULL) {
        ogs_error("OpenAPI_termination_notification_convertToJSON() failed [cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "cause", cause_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_termination_notification_convertToJSON() failed [cause]");
        goto end;
    }

end:
    return item;
}

OpenAPI_termination_notification_t *OpenAPI_termination_notification_parseFromJSON(cJSON *termination_notificationJSON)
{
    OpenAPI_termination_notification_t *termination_notification_local_var = NULL;
    cJSON *resource_uri = cJSON_GetObjectItemCaseSensitive(termination_notificationJSON, "resourceUri");
    if (!resource_uri) {
        ogs_error("OpenAPI_termination_notification_parseFromJSON() failed [resource_uri]");
        goto end;
    }


    if (!cJSON_IsString(resource_uri)) {
        ogs_error("OpenAPI_termination_notification_parseFromJSON() failed [resource_uri]");
        goto end;
    }

    cJSON *cause = cJSON_GetObjectItemCaseSensitive(termination_notificationJSON, "cause");
    if (!cause) {
        ogs_error("OpenAPI_termination_notification_parseFromJSON() failed [cause]");
        goto end;
    }

    OpenAPI_sm_policy_association_release_cause_t *cause_local_nonprim = NULL;

    cause_local_nonprim = OpenAPI_sm_policy_association_release_cause_parseFromJSON(cause);

    termination_notification_local_var = OpenAPI_termination_notification_create (
        ogs_strdup(resource_uri->valuestring),
        cause_local_nonprim
        );

    return termination_notification_local_var;
end:
    return NULL;
}

OpenAPI_termination_notification_t *OpenAPI_termination_notification_copy(OpenAPI_termination_notification_t *dst, OpenAPI_termination_notification_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_termination_notification_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_termination_notification_convertToJSON() failed");
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

    OpenAPI_termination_notification_free(dst);
    dst = OpenAPI_termination_notification_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

