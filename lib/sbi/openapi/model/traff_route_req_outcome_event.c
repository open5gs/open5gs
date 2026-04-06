
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "traff_route_req_outcome_event.h"

OpenAPI_traff_route_req_outcome_event_t *OpenAPI_traff_route_req_outcome_event_create(
    char *notification_uri,
    char *notif_corre_id
)
{
    OpenAPI_traff_route_req_outcome_event_t *traff_route_req_outcome_event_local_var = ogs_malloc(sizeof(OpenAPI_traff_route_req_outcome_event_t));
    ogs_assert(traff_route_req_outcome_event_local_var);

    traff_route_req_outcome_event_local_var->notification_uri = notification_uri;
    traff_route_req_outcome_event_local_var->notif_corre_id = notif_corre_id;

    return traff_route_req_outcome_event_local_var;
}

void OpenAPI_traff_route_req_outcome_event_free(OpenAPI_traff_route_req_outcome_event_t *traff_route_req_outcome_event)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == traff_route_req_outcome_event) {
        return;
    }
    if (traff_route_req_outcome_event->notification_uri) {
        ogs_free(traff_route_req_outcome_event->notification_uri);
        traff_route_req_outcome_event->notification_uri = NULL;
    }
    if (traff_route_req_outcome_event->notif_corre_id) {
        ogs_free(traff_route_req_outcome_event->notif_corre_id);
        traff_route_req_outcome_event->notif_corre_id = NULL;
    }
    ogs_free(traff_route_req_outcome_event);
}

cJSON *OpenAPI_traff_route_req_outcome_event_convertToJSON(OpenAPI_traff_route_req_outcome_event_t *traff_route_req_outcome_event)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (traff_route_req_outcome_event == NULL) {
        ogs_error("OpenAPI_traff_route_req_outcome_event_convertToJSON() failed [TraffRouteReqOutcomeEvent]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!traff_route_req_outcome_event->notification_uri) {
        ogs_error("OpenAPI_traff_route_req_outcome_event_convertToJSON() failed [notification_uri]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notificationUri", traff_route_req_outcome_event->notification_uri) == NULL) {
        ogs_error("OpenAPI_traff_route_req_outcome_event_convertToJSON() failed [notification_uri]");
        goto end;
    }

    if (!traff_route_req_outcome_event->notif_corre_id) {
        ogs_error("OpenAPI_traff_route_req_outcome_event_convertToJSON() failed [notif_corre_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "notifCorreId", traff_route_req_outcome_event->notif_corre_id) == NULL) {
        ogs_error("OpenAPI_traff_route_req_outcome_event_convertToJSON() failed [notif_corre_id]");
        goto end;
    }

end:
    return item;
}

OpenAPI_traff_route_req_outcome_event_t *OpenAPI_traff_route_req_outcome_event_parseFromJSON(cJSON *traff_route_req_outcome_eventJSON)
{
    OpenAPI_traff_route_req_outcome_event_t *traff_route_req_outcome_event_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notification_uri = NULL;
    cJSON *notif_corre_id = NULL;
    notification_uri = cJSON_GetObjectItemCaseSensitive(traff_route_req_outcome_eventJSON, "notificationUri");
    if (!notification_uri) {
        ogs_error("OpenAPI_traff_route_req_outcome_event_parseFromJSON() failed [notification_uri]");
        goto end;
    }
    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_traff_route_req_outcome_event_parseFromJSON() failed [notification_uri]");
        goto end;
    }

    notif_corre_id = cJSON_GetObjectItemCaseSensitive(traff_route_req_outcome_eventJSON, "notifCorreId");
    if (!notif_corre_id) {
        ogs_error("OpenAPI_traff_route_req_outcome_event_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }
    if (!cJSON_IsString(notif_corre_id)) {
        ogs_error("OpenAPI_traff_route_req_outcome_event_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }

    traff_route_req_outcome_event_local_var = OpenAPI_traff_route_req_outcome_event_create (
        ogs_strdup(notification_uri->valuestring),
        ogs_strdup(notif_corre_id->valuestring)
    );

    return traff_route_req_outcome_event_local_var;
end:
    return NULL;
}

OpenAPI_traff_route_req_outcome_event_t *OpenAPI_traff_route_req_outcome_event_copy(OpenAPI_traff_route_req_outcome_event_t *dst, OpenAPI_traff_route_req_outcome_event_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_traff_route_req_outcome_event_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_traff_route_req_outcome_event_convertToJSON() failed");
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

    OpenAPI_traff_route_req_outcome_event_free(dst);
    dst = OpenAPI_traff_route_req_outcome_event_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

