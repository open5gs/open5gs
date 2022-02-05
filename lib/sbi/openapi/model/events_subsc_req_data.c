
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "events_subsc_req_data.h"

OpenAPI_events_subsc_req_data_t *OpenAPI_events_subsc_req_data_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    OpenAPI_list_t *req_qos_mon_params,
    OpenAPI_qos_monitoring_information_t *qos_mon,
    OpenAPI_list_t *req_anis,
    OpenAPI_usage_threshold_t *usg_thres,
    char *notif_corre_id
)
{
    OpenAPI_events_subsc_req_data_t *events_subsc_req_data_local_var = ogs_malloc(sizeof(OpenAPI_events_subsc_req_data_t));
    ogs_assert(events_subsc_req_data_local_var);

    events_subsc_req_data_local_var->events = events;
    events_subsc_req_data_local_var->notif_uri = notif_uri;
    events_subsc_req_data_local_var->req_qos_mon_params = req_qos_mon_params;
    events_subsc_req_data_local_var->qos_mon = qos_mon;
    events_subsc_req_data_local_var->req_anis = req_anis;
    events_subsc_req_data_local_var->usg_thres = usg_thres;
    events_subsc_req_data_local_var->notif_corre_id = notif_corre_id;

    return events_subsc_req_data_local_var;
}

void OpenAPI_events_subsc_req_data_free(OpenAPI_events_subsc_req_data_t *events_subsc_req_data)
{
    if (NULL == events_subsc_req_data) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_list_for_each(events_subsc_req_data->events, node) {
        OpenAPI_af_event_subscription_free(node->data);
    }
    OpenAPI_list_free(events_subsc_req_data->events);
    ogs_free(events_subsc_req_data->notif_uri);
    OpenAPI_list_free(events_subsc_req_data->req_qos_mon_params);
    OpenAPI_qos_monitoring_information_free(events_subsc_req_data->qos_mon);
    OpenAPI_list_free(events_subsc_req_data->req_anis);
    OpenAPI_usage_threshold_free(events_subsc_req_data->usg_thres);
    ogs_free(events_subsc_req_data->notif_corre_id);
    ogs_free(events_subsc_req_data);
}

cJSON *OpenAPI_events_subsc_req_data_convertToJSON(OpenAPI_events_subsc_req_data_t *events_subsc_req_data)
{
    cJSON *item = NULL;

    if (events_subsc_req_data == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [EventsSubscReqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *eventsList = cJSON_AddArrayToObject(item, "events");
    if (eventsList == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [events]");
        goto end;
    }

    OpenAPI_lnode_t *events_node;
    if (events_subsc_req_data->events) {
        OpenAPI_list_for_each(events_subsc_req_data->events, events_node) {
            cJSON *itemLocal = OpenAPI_af_event_subscription_convertToJSON(events_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [events]");
                goto end;
            }
            cJSON_AddItemToArray(eventsList, itemLocal);
        }
    }

    if (events_subsc_req_data->notif_uri) {
    if (cJSON_AddStringToObject(item, "notifUri", events_subsc_req_data->notif_uri) == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [notif_uri]");
        goto end;
    }
    }

    if (events_subsc_req_data->req_qos_mon_params) {
    cJSON *req_qos_mon_params = cJSON_AddArrayToObject(item, "reqQosMonParams");
    if (req_qos_mon_params == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [req_qos_mon_params]");
        goto end;
    }
    OpenAPI_lnode_t *req_qos_mon_params_node;
    OpenAPI_list_for_each(events_subsc_req_data->req_qos_mon_params, req_qos_mon_params_node) {
        if (cJSON_AddStringToObject(req_qos_mon_params, "", OpenAPI_requested_qos_monitoring_parameter_ToString((intptr_t)req_qos_mon_params_node->data)) == NULL) {
            ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [req_qos_mon_params]");
            goto end;
        }
    }
    }

    if (events_subsc_req_data->qos_mon) {
    cJSON *qos_mon_local_JSON = OpenAPI_qos_monitoring_information_convertToJSON(events_subsc_req_data->qos_mon);
    if (qos_mon_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [qos_mon]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosMon", qos_mon_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [qos_mon]");
        goto end;
    }
    }

    if (events_subsc_req_data->req_anis) {
    cJSON *req_anis = cJSON_AddArrayToObject(item, "reqAnis");
    if (req_anis == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [req_anis]");
        goto end;
    }
    OpenAPI_lnode_t *req_anis_node;
    OpenAPI_list_for_each(events_subsc_req_data->req_anis, req_anis_node) {
        if (cJSON_AddStringToObject(req_anis, "", OpenAPI_required_access_info_ToString((intptr_t)req_anis_node->data)) == NULL) {
            ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [req_anis]");
            goto end;
        }
    }
    }

    if (events_subsc_req_data->usg_thres) {
    cJSON *usg_thres_local_JSON = OpenAPI_usage_threshold_convertToJSON(events_subsc_req_data->usg_thres);
    if (usg_thres_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [usg_thres]");
        goto end;
    }
    cJSON_AddItemToObject(item, "usgThres", usg_thres_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [usg_thres]");
        goto end;
    }
    }

    if (events_subsc_req_data->notif_corre_id) {
    if (cJSON_AddStringToObject(item, "notifCorreId", events_subsc_req_data->notif_corre_id) == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [notif_corre_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_events_subsc_req_data_t *OpenAPI_events_subsc_req_data_parseFromJSON(cJSON *events_subsc_req_dataJSON)
{
    OpenAPI_events_subsc_req_data_t *events_subsc_req_data_local_var = NULL;
    cJSON *events = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "events");
    if (!events) {
        ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [events]");
        goto end;
    }

    OpenAPI_list_t *eventsList;
    cJSON *events_local_nonprimitive;
    if (!cJSON_IsArray(events)){
        ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [events]");
        goto end;
    }

    eventsList = OpenAPI_list_create();

    cJSON_ArrayForEach(events_local_nonprimitive, events ) {
        if (!cJSON_IsObject(events_local_nonprimitive)) {
            ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [events]");
            goto end;
        }
        OpenAPI_af_event_subscription_t *eventsItem = OpenAPI_af_event_subscription_parseFromJSON(events_local_nonprimitive);

        if (!eventsItem) {
            ogs_error("No eventsItem");
            OpenAPI_list_free(eventsList);
            goto end;
        }

        OpenAPI_list_add(eventsList, eventsItem);
    }

    cJSON *notif_uri = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "notifUri");

    if (notif_uri) {
    if (!cJSON_IsString(notif_uri)) {
        ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    }

    cJSON *req_qos_mon_params = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "reqQosMonParams");

    OpenAPI_list_t *req_qos_mon_paramsList;
    if (req_qos_mon_params) {
    cJSON *req_qos_mon_params_local_nonprimitive;
    if (!cJSON_IsArray(req_qos_mon_params)) {
        ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [req_qos_mon_params]");
        goto end;
    }

    req_qos_mon_paramsList = OpenAPI_list_create();

    cJSON_ArrayForEach(req_qos_mon_params_local_nonprimitive, req_qos_mon_params ) {
        if (!cJSON_IsString(req_qos_mon_params_local_nonprimitive)){
            ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [req_qos_mon_params]");
            goto end;
        }

        OpenAPI_list_add(req_qos_mon_paramsList, (void *)OpenAPI_requested_qos_monitoring_parameter_FromString(req_qos_mon_params_local_nonprimitive->valuestring));
    }
    }

    cJSON *qos_mon = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "qosMon");

    OpenAPI_qos_monitoring_information_t *qos_mon_local_nonprim = NULL;
    if (qos_mon) {
    qos_mon_local_nonprim = OpenAPI_qos_monitoring_information_parseFromJSON(qos_mon);
    }

    cJSON *req_anis = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "reqAnis");

    OpenAPI_list_t *req_anisList;
    if (req_anis) {
    cJSON *req_anis_local_nonprimitive;
    if (!cJSON_IsArray(req_anis)) {
        ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [req_anis]");
        goto end;
    }

    req_anisList = OpenAPI_list_create();

    cJSON_ArrayForEach(req_anis_local_nonprimitive, req_anis ) {
        if (!cJSON_IsString(req_anis_local_nonprimitive)){
            ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [req_anis]");
            goto end;
        }

        OpenAPI_list_add(req_anisList, (void *)OpenAPI_required_access_info_FromString(req_anis_local_nonprimitive->valuestring));
    }
    }

    cJSON *usg_thres = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "usgThres");

    OpenAPI_usage_threshold_t *usg_thres_local_nonprim = NULL;
    if (usg_thres) {
    usg_thres_local_nonprim = OpenAPI_usage_threshold_parseFromJSON(usg_thres);
    }

    cJSON *notif_corre_id = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "notifCorreId");

    if (notif_corre_id) {
    if (!cJSON_IsString(notif_corre_id)) {
        ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }
    }

    events_subsc_req_data_local_var = OpenAPI_events_subsc_req_data_create (
        eventsList,
        notif_uri ? ogs_strdup(notif_uri->valuestring) : NULL,
        req_qos_mon_params ? req_qos_mon_paramsList : NULL,
        qos_mon ? qos_mon_local_nonprim : NULL,
        req_anis ? req_anisList : NULL,
        usg_thres ? usg_thres_local_nonprim : NULL,
        notif_corre_id ? ogs_strdup(notif_corre_id->valuestring) : NULL
    );

    return events_subsc_req_data_local_var;
end:
    return NULL;
}

OpenAPI_events_subsc_req_data_t *OpenAPI_events_subsc_req_data_copy(OpenAPI_events_subsc_req_data_t *dst, OpenAPI_events_subsc_req_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_events_subsc_req_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed");
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

    OpenAPI_events_subsc_req_data_free(dst);
    dst = OpenAPI_events_subsc_req_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

