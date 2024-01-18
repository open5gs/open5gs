
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
    char *notif_corre_id,
    OpenAPI_list_t *af_app_ids,
    bool is_direct_notif_ind,
    int direct_notif_ind
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
    events_subsc_req_data_local_var->af_app_ids = af_app_ids;
    events_subsc_req_data_local_var->is_direct_notif_ind = is_direct_notif_ind;
    events_subsc_req_data_local_var->direct_notif_ind = direct_notif_ind;

    return events_subsc_req_data_local_var;
}

void OpenAPI_events_subsc_req_data_free(OpenAPI_events_subsc_req_data_t *events_subsc_req_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == events_subsc_req_data) {
        return;
    }
    if (events_subsc_req_data->events) {
        OpenAPI_list_for_each(events_subsc_req_data->events, node) {
            OpenAPI_af_event_subscription_free(node->data);
        }
        OpenAPI_list_free(events_subsc_req_data->events);
        events_subsc_req_data->events = NULL;
    }
    if (events_subsc_req_data->notif_uri) {
        ogs_free(events_subsc_req_data->notif_uri);
        events_subsc_req_data->notif_uri = NULL;
    }
    if (events_subsc_req_data->req_qos_mon_params) {
        OpenAPI_list_free(events_subsc_req_data->req_qos_mon_params);
        events_subsc_req_data->req_qos_mon_params = NULL;
    }
    if (events_subsc_req_data->qos_mon) {
        OpenAPI_qos_monitoring_information_free(events_subsc_req_data->qos_mon);
        events_subsc_req_data->qos_mon = NULL;
    }
    if (events_subsc_req_data->req_anis) {
        OpenAPI_list_free(events_subsc_req_data->req_anis);
        events_subsc_req_data->req_anis = NULL;
    }
    if (events_subsc_req_data->usg_thres) {
        OpenAPI_usage_threshold_free(events_subsc_req_data->usg_thres);
        events_subsc_req_data->usg_thres = NULL;
    }
    if (events_subsc_req_data->notif_corre_id) {
        ogs_free(events_subsc_req_data->notif_corre_id);
        events_subsc_req_data->notif_corre_id = NULL;
    }
    if (events_subsc_req_data->af_app_ids) {
        OpenAPI_list_for_each(events_subsc_req_data->af_app_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(events_subsc_req_data->af_app_ids);
        events_subsc_req_data->af_app_ids = NULL;
    }
    ogs_free(events_subsc_req_data);
}

cJSON *OpenAPI_events_subsc_req_data_convertToJSON(OpenAPI_events_subsc_req_data_t *events_subsc_req_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (events_subsc_req_data == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [EventsSubscReqData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!events_subsc_req_data->events) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [events]");
        return NULL;
    }
    cJSON *eventsList = cJSON_AddArrayToObject(item, "events");
    if (eventsList == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [events]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_req_data->events, node) {
        cJSON *itemLocal = OpenAPI_af_event_subscription_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [events]");
            goto end;
        }
        cJSON_AddItemToArray(eventsList, itemLocal);
    }

    if (events_subsc_req_data->notif_uri) {
    if (cJSON_AddStringToObject(item, "notifUri", events_subsc_req_data->notif_uri) == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [notif_uri]");
        goto end;
    }
    }

    if (events_subsc_req_data->req_qos_mon_params != OpenAPI_requested_qos_monitoring_parameter_NULL) {
    cJSON *req_qos_mon_paramsList = cJSON_AddArrayToObject(item, "reqQosMonParams");
    if (req_qos_mon_paramsList == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [req_qos_mon_params]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_req_data->req_qos_mon_params, node) {
        if (cJSON_AddStringToObject(req_qos_mon_paramsList, "", OpenAPI_requested_qos_monitoring_parameter_ToString((intptr_t)node->data)) == NULL) {
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

    if (events_subsc_req_data->req_anis != OpenAPI_required_access_info_NULL) {
    cJSON *req_anisList = cJSON_AddArrayToObject(item, "reqAnis");
    if (req_anisList == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [req_anis]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_req_data->req_anis, node) {
        if (cJSON_AddStringToObject(req_anisList, "", OpenAPI_required_access_info_ToString((intptr_t)node->data)) == NULL) {
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

    if (events_subsc_req_data->af_app_ids) {
    cJSON *af_app_idsList = cJSON_AddArrayToObject(item, "afAppIds");
    if (af_app_idsList == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [af_app_ids]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_req_data->af_app_ids, node) {
        if (cJSON_AddStringToObject(af_app_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [af_app_ids]");
            goto end;
        }
    }
    }

    if (events_subsc_req_data->is_direct_notif_ind) {
    if (cJSON_AddBoolToObject(item, "directNotifInd", events_subsc_req_data->direct_notif_ind) == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_convertToJSON() failed [direct_notif_ind]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_events_subsc_req_data_t *OpenAPI_events_subsc_req_data_parseFromJSON(cJSON *events_subsc_req_dataJSON)
{
    OpenAPI_events_subsc_req_data_t *events_subsc_req_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *events = NULL;
    OpenAPI_list_t *eventsList = NULL;
    cJSON *notif_uri = NULL;
    cJSON *req_qos_mon_params = NULL;
    OpenAPI_list_t *req_qos_mon_paramsList = NULL;
    cJSON *qos_mon = NULL;
    OpenAPI_qos_monitoring_information_t *qos_mon_local_nonprim = NULL;
    cJSON *req_anis = NULL;
    OpenAPI_list_t *req_anisList = NULL;
    cJSON *usg_thres = NULL;
    OpenAPI_usage_threshold_t *usg_thres_local_nonprim = NULL;
    cJSON *notif_corre_id = NULL;
    cJSON *af_app_ids = NULL;
    OpenAPI_list_t *af_app_idsList = NULL;
    cJSON *direct_notif_ind = NULL;
    events = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "events");
    if (!events) {
        ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [events]");
        goto end;
    }
        cJSON *events_local = NULL;
        if (!cJSON_IsArray(events)) {
            ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [events]");
            goto end;
        }

        eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(events_local, events) {
            if (!cJSON_IsObject(events_local)) {
                ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [events]");
                goto end;
            }
            OpenAPI_af_event_subscription_t *eventsItem = OpenAPI_af_event_subscription_parseFromJSON(events_local);
            if (!eventsItem) {
                ogs_error("No eventsItem");
                goto end;
            }
            OpenAPI_list_add(eventsList, eventsItem);
        }

    notif_uri = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "notifUri");
    if (notif_uri) {
    if (!cJSON_IsString(notif_uri) && !cJSON_IsNull(notif_uri)) {
        ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    }

    req_qos_mon_params = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "reqQosMonParams");
    if (req_qos_mon_params) {
        cJSON *req_qos_mon_params_local = NULL;
        if (!cJSON_IsArray(req_qos_mon_params)) {
            ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [req_qos_mon_params]");
            goto end;
        }

        req_qos_mon_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_qos_mon_params_local, req_qos_mon_params) {
            OpenAPI_requested_qos_monitoring_parameter_e localEnum = OpenAPI_requested_qos_monitoring_parameter_NULL;
            if (!cJSON_IsString(req_qos_mon_params_local)) {
                ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [req_qos_mon_params]");
                goto end;
            }
            localEnum = OpenAPI_requested_qos_monitoring_parameter_FromString(req_qos_mon_params_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"req_qos_mon_params\" is not supported. Ignoring it ...",
                         req_qos_mon_params_local->valuestring);
            } else {
                OpenAPI_list_add(req_qos_mon_paramsList, (void *)localEnum);
            }
        }
        if (req_qos_mon_paramsList->count == 0) {
            ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed: Expected req_qos_mon_paramsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    qos_mon = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "qosMon");
    if (qos_mon) {
    qos_mon_local_nonprim = OpenAPI_qos_monitoring_information_parseFromJSON(qos_mon);
    if (!qos_mon_local_nonprim) {
        ogs_error("OpenAPI_qos_monitoring_information_parseFromJSON failed [qos_mon]");
        goto end;
    }
    }

    req_anis = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "reqAnis");
    if (req_anis) {
        cJSON *req_anis_local = NULL;
        if (!cJSON_IsArray(req_anis)) {
            ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [req_anis]");
            goto end;
        }

        req_anisList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_anis_local, req_anis) {
            OpenAPI_required_access_info_e localEnum = OpenAPI_required_access_info_NULL;
            if (!cJSON_IsString(req_anis_local)) {
                ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [req_anis]");
                goto end;
            }
            localEnum = OpenAPI_required_access_info_FromString(req_anis_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"req_anis\" is not supported. Ignoring it ...",
                         req_anis_local->valuestring);
            } else {
                OpenAPI_list_add(req_anisList, (void *)localEnum);
            }
        }
        if (req_anisList->count == 0) {
            ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed: Expected req_anisList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    usg_thres = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "usgThres");
    if (usg_thres) {
    usg_thres_local_nonprim = OpenAPI_usage_threshold_parseFromJSON(usg_thres);
    if (!usg_thres_local_nonprim) {
        ogs_error("OpenAPI_usage_threshold_parseFromJSON failed [usg_thres]");
        goto end;
    }
    }

    notif_corre_id = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "notifCorreId");
    if (notif_corre_id) {
    if (!cJSON_IsString(notif_corre_id) && !cJSON_IsNull(notif_corre_id)) {
        ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }
    }

    af_app_ids = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "afAppIds");
    if (af_app_ids) {
        cJSON *af_app_ids_local = NULL;
        if (!cJSON_IsArray(af_app_ids)) {
            ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [af_app_ids]");
            goto end;
        }

        af_app_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(af_app_ids_local, af_app_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(af_app_ids_local)) {
                ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [af_app_ids]");
                goto end;
            }
            OpenAPI_list_add(af_app_idsList, ogs_strdup(af_app_ids_local->valuestring));
        }
    }

    direct_notif_ind = cJSON_GetObjectItemCaseSensitive(events_subsc_req_dataJSON, "directNotifInd");
    if (direct_notif_ind) {
    if (!cJSON_IsBool(direct_notif_ind)) {
        ogs_error("OpenAPI_events_subsc_req_data_parseFromJSON() failed [direct_notif_ind]");
        goto end;
    }
    }

    events_subsc_req_data_local_var = OpenAPI_events_subsc_req_data_create (
        eventsList,
        notif_uri && !cJSON_IsNull(notif_uri) ? ogs_strdup(notif_uri->valuestring) : NULL,
        req_qos_mon_params ? req_qos_mon_paramsList : NULL,
        qos_mon ? qos_mon_local_nonprim : NULL,
        req_anis ? req_anisList : NULL,
        usg_thres ? usg_thres_local_nonprim : NULL,
        notif_corre_id && !cJSON_IsNull(notif_corre_id) ? ogs_strdup(notif_corre_id->valuestring) : NULL,
        af_app_ids ? af_app_idsList : NULL,
        direct_notif_ind ? true : false,
        direct_notif_ind ? direct_notif_ind->valueint : 0
    );

    return events_subsc_req_data_local_var;
end:
    if (eventsList) {
        OpenAPI_list_for_each(eventsList, node) {
            OpenAPI_af_event_subscription_free(node->data);
        }
        OpenAPI_list_free(eventsList);
        eventsList = NULL;
    }
    if (req_qos_mon_paramsList) {
        OpenAPI_list_free(req_qos_mon_paramsList);
        req_qos_mon_paramsList = NULL;
    }
    if (qos_mon_local_nonprim) {
        OpenAPI_qos_monitoring_information_free(qos_mon_local_nonprim);
        qos_mon_local_nonprim = NULL;
    }
    if (req_anisList) {
        OpenAPI_list_free(req_anisList);
        req_anisList = NULL;
    }
    if (usg_thres_local_nonprim) {
        OpenAPI_usage_threshold_free(usg_thres_local_nonprim);
        usg_thres_local_nonprim = NULL;
    }
    if (af_app_idsList) {
        OpenAPI_list_for_each(af_app_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(af_app_idsList);
        af_app_idsList = NULL;
    }
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

