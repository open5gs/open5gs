
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "events_subsc_req_data_rm.h"

OpenAPI_events_subsc_req_data_rm_t *OpenAPI_events_subsc_req_data_rm_create(
    OpenAPI_list_t *events,
    char *notif_uri,
    OpenAPI_list_t *req_qos_mon_params,
    bool is_qos_mon_null,
    OpenAPI_qos_monitoring_information_rm_t *qos_mon,
    OpenAPI_list_t *req_anis,
    bool is_usg_thres_null,
    OpenAPI_usage_threshold_rm_t *usg_thres,
    char *notif_corre_id,
    bool is_direct_notif_ind_null,
    bool is_direct_notif_ind,
    int direct_notif_ind
)
{
    OpenAPI_events_subsc_req_data_rm_t *events_subsc_req_data_rm_local_var = ogs_malloc(sizeof(OpenAPI_events_subsc_req_data_rm_t));
    ogs_assert(events_subsc_req_data_rm_local_var);

    events_subsc_req_data_rm_local_var->events = events;
    events_subsc_req_data_rm_local_var->notif_uri = notif_uri;
    events_subsc_req_data_rm_local_var->req_qos_mon_params = req_qos_mon_params;
    events_subsc_req_data_rm_local_var->is_qos_mon_null = is_qos_mon_null;
    events_subsc_req_data_rm_local_var->qos_mon = qos_mon;
    events_subsc_req_data_rm_local_var->req_anis = req_anis;
    events_subsc_req_data_rm_local_var->is_usg_thres_null = is_usg_thres_null;
    events_subsc_req_data_rm_local_var->usg_thres = usg_thres;
    events_subsc_req_data_rm_local_var->notif_corre_id = notif_corre_id;
    events_subsc_req_data_rm_local_var->is_direct_notif_ind_null = is_direct_notif_ind_null;
    events_subsc_req_data_rm_local_var->is_direct_notif_ind = is_direct_notif_ind;
    events_subsc_req_data_rm_local_var->direct_notif_ind = direct_notif_ind;

    return events_subsc_req_data_rm_local_var;
}

void OpenAPI_events_subsc_req_data_rm_free(OpenAPI_events_subsc_req_data_rm_t *events_subsc_req_data_rm)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == events_subsc_req_data_rm) {
        return;
    }
    if (events_subsc_req_data_rm->events) {
        OpenAPI_list_for_each(events_subsc_req_data_rm->events, node) {
            OpenAPI_af_event_subscription_free(node->data);
        }
        OpenAPI_list_free(events_subsc_req_data_rm->events);
        events_subsc_req_data_rm->events = NULL;
    }
    if (events_subsc_req_data_rm->notif_uri) {
        ogs_free(events_subsc_req_data_rm->notif_uri);
        events_subsc_req_data_rm->notif_uri = NULL;
    }
    if (events_subsc_req_data_rm->req_qos_mon_params) {
        OpenAPI_list_free(events_subsc_req_data_rm->req_qos_mon_params);
        events_subsc_req_data_rm->req_qos_mon_params = NULL;
    }
    if (events_subsc_req_data_rm->qos_mon) {
        OpenAPI_qos_monitoring_information_rm_free(events_subsc_req_data_rm->qos_mon);
        events_subsc_req_data_rm->qos_mon = NULL;
    }
    if (events_subsc_req_data_rm->req_anis) {
        OpenAPI_list_free(events_subsc_req_data_rm->req_anis);
        events_subsc_req_data_rm->req_anis = NULL;
    }
    if (events_subsc_req_data_rm->usg_thres) {
        OpenAPI_usage_threshold_rm_free(events_subsc_req_data_rm->usg_thres);
        events_subsc_req_data_rm->usg_thres = NULL;
    }
    if (events_subsc_req_data_rm->notif_corre_id) {
        ogs_free(events_subsc_req_data_rm->notif_corre_id);
        events_subsc_req_data_rm->notif_corre_id = NULL;
    }
    ogs_free(events_subsc_req_data_rm);
}

cJSON *OpenAPI_events_subsc_req_data_rm_convertToJSON(OpenAPI_events_subsc_req_data_rm_t *events_subsc_req_data_rm)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (events_subsc_req_data_rm == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [EventsSubscReqDataRm]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!events_subsc_req_data_rm->events) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [events]");
        return NULL;
    }
    cJSON *eventsList = cJSON_AddArrayToObject(item, "events");
    if (eventsList == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [events]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_req_data_rm->events, node) {
        cJSON *itemLocal = OpenAPI_af_event_subscription_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [events]");
            goto end;
        }
        cJSON_AddItemToArray(eventsList, itemLocal);
    }

    if (events_subsc_req_data_rm->notif_uri) {
    if (cJSON_AddStringToObject(item, "notifUri", events_subsc_req_data_rm->notif_uri) == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [notif_uri]");
        goto end;
    }
    }

    if (events_subsc_req_data_rm->req_qos_mon_params != OpenAPI_requested_qos_monitoring_parameter_NULL) {
    cJSON *req_qos_mon_paramsList = cJSON_AddArrayToObject(item, "reqQosMonParams");
    if (req_qos_mon_paramsList == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [req_qos_mon_params]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_req_data_rm->req_qos_mon_params, node) {
        if (cJSON_AddStringToObject(req_qos_mon_paramsList, "", OpenAPI_requested_qos_monitoring_parameter_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [req_qos_mon_params]");
            goto end;
        }
    }
    }

    if (events_subsc_req_data_rm->qos_mon) {
    cJSON *qos_mon_local_JSON = OpenAPI_qos_monitoring_information_rm_convertToJSON(events_subsc_req_data_rm->qos_mon);
    if (qos_mon_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [qos_mon]");
        goto end;
    }
    cJSON_AddItemToObject(item, "qosMon", qos_mon_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [qos_mon]");
        goto end;
    }
    } else if (events_subsc_req_data_rm->is_qos_mon_null) {
        if (cJSON_AddNullToObject(item, "qosMon") == NULL) {
            ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [qos_mon]");
            goto end;
        }
    }

    if (events_subsc_req_data_rm->req_anis != OpenAPI_required_access_info_NULL) {
    cJSON *req_anisList = cJSON_AddArrayToObject(item, "reqAnis");
    if (req_anisList == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [req_anis]");
        goto end;
    }
    OpenAPI_list_for_each(events_subsc_req_data_rm->req_anis, node) {
        if (cJSON_AddStringToObject(req_anisList, "", OpenAPI_required_access_info_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [req_anis]");
            goto end;
        }
    }
    }

    if (events_subsc_req_data_rm->usg_thres) {
    cJSON *usg_thres_local_JSON = OpenAPI_usage_threshold_rm_convertToJSON(events_subsc_req_data_rm->usg_thres);
    if (usg_thres_local_JSON == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [usg_thres]");
        goto end;
    }
    cJSON_AddItemToObject(item, "usgThres", usg_thres_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [usg_thres]");
        goto end;
    }
    } else if (events_subsc_req_data_rm->is_usg_thres_null) {
        if (cJSON_AddNullToObject(item, "usgThres") == NULL) {
            ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [usg_thres]");
            goto end;
        }
    }

    if (events_subsc_req_data_rm->notif_corre_id) {
    if (cJSON_AddStringToObject(item, "notifCorreId", events_subsc_req_data_rm->notif_corre_id) == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [notif_corre_id]");
        goto end;
    }
    }

    if (events_subsc_req_data_rm->is_direct_notif_ind) {
    if (cJSON_AddBoolToObject(item, "directNotifInd", events_subsc_req_data_rm->direct_notif_ind) == NULL) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [direct_notif_ind]");
        goto end;
    }
    } else if (events_subsc_req_data_rm->is_direct_notif_ind_null) {
        if (cJSON_AddNullToObject(item, "directNotifInd") == NULL) {
            ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed [direct_notif_ind]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_events_subsc_req_data_rm_t *OpenAPI_events_subsc_req_data_rm_parseFromJSON(cJSON *events_subsc_req_data_rmJSON)
{
    OpenAPI_events_subsc_req_data_rm_t *events_subsc_req_data_rm_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *events = NULL;
    OpenAPI_list_t *eventsList = NULL;
    cJSON *notif_uri = NULL;
    cJSON *req_qos_mon_params = NULL;
    OpenAPI_list_t *req_qos_mon_paramsList = NULL;
    cJSON *qos_mon = NULL;
    OpenAPI_qos_monitoring_information_rm_t *qos_mon_local_nonprim = NULL;
    cJSON *req_anis = NULL;
    OpenAPI_list_t *req_anisList = NULL;
    cJSON *usg_thres = NULL;
    OpenAPI_usage_threshold_rm_t *usg_thres_local_nonprim = NULL;
    cJSON *notif_corre_id = NULL;
    cJSON *direct_notif_ind = NULL;
    events = cJSON_GetObjectItemCaseSensitive(events_subsc_req_data_rmJSON, "events");
    if (!events) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed [events]");
        goto end;
    }
        cJSON *events_local = NULL;
        if (!cJSON_IsArray(events)) {
            ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed [events]");
            goto end;
        }

        eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(events_local, events) {
            if (!cJSON_IsObject(events_local)) {
                ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed [events]");
                goto end;
            }
            OpenAPI_af_event_subscription_t *eventsItem = OpenAPI_af_event_subscription_parseFromJSON(events_local);
            if (!eventsItem) {
                ogs_error("No eventsItem");
                goto end;
            }
            OpenAPI_list_add(eventsList, eventsItem);
        }

    notif_uri = cJSON_GetObjectItemCaseSensitive(events_subsc_req_data_rmJSON, "notifUri");
    if (notif_uri) {
    if (!cJSON_IsString(notif_uri) && !cJSON_IsNull(notif_uri)) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    }

    req_qos_mon_params = cJSON_GetObjectItemCaseSensitive(events_subsc_req_data_rmJSON, "reqQosMonParams");
    if (req_qos_mon_params) {
        cJSON *req_qos_mon_params_local = NULL;
        if (!cJSON_IsArray(req_qos_mon_params)) {
            ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed [req_qos_mon_params]");
            goto end;
        }

        req_qos_mon_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_qos_mon_params_local, req_qos_mon_params) {
            OpenAPI_requested_qos_monitoring_parameter_e localEnum = OpenAPI_requested_qos_monitoring_parameter_NULL;
            if (!cJSON_IsString(req_qos_mon_params_local)) {
                ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed [req_qos_mon_params]");
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
            ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed: Expected req_qos_mon_paramsList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    qos_mon = cJSON_GetObjectItemCaseSensitive(events_subsc_req_data_rmJSON, "qosMon");
    if (qos_mon) {
    if (!cJSON_IsNull(qos_mon)) {
    qos_mon_local_nonprim = OpenAPI_qos_monitoring_information_rm_parseFromJSON(qos_mon);
    if (!qos_mon_local_nonprim) {
        ogs_error("OpenAPI_qos_monitoring_information_rm_parseFromJSON failed [qos_mon]");
        goto end;
    }
    }
    }

    req_anis = cJSON_GetObjectItemCaseSensitive(events_subsc_req_data_rmJSON, "reqAnis");
    if (req_anis) {
        cJSON *req_anis_local = NULL;
        if (!cJSON_IsArray(req_anis)) {
            ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed [req_anis]");
            goto end;
        }

        req_anisList = OpenAPI_list_create();

        cJSON_ArrayForEach(req_anis_local, req_anis) {
            OpenAPI_required_access_info_e localEnum = OpenAPI_required_access_info_NULL;
            if (!cJSON_IsString(req_anis_local)) {
                ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed [req_anis]");
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
            ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed: Expected req_anisList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    usg_thres = cJSON_GetObjectItemCaseSensitive(events_subsc_req_data_rmJSON, "usgThres");
    if (usg_thres) {
    if (!cJSON_IsNull(usg_thres)) {
    usg_thres_local_nonprim = OpenAPI_usage_threshold_rm_parseFromJSON(usg_thres);
    if (!usg_thres_local_nonprim) {
        ogs_error("OpenAPI_usage_threshold_rm_parseFromJSON failed [usg_thres]");
        goto end;
    }
    }
    }

    notif_corre_id = cJSON_GetObjectItemCaseSensitive(events_subsc_req_data_rmJSON, "notifCorreId");
    if (notif_corre_id) {
    if (!cJSON_IsString(notif_corre_id) && !cJSON_IsNull(notif_corre_id)) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed [notif_corre_id]");
        goto end;
    }
    }

    direct_notif_ind = cJSON_GetObjectItemCaseSensitive(events_subsc_req_data_rmJSON, "directNotifInd");
    if (direct_notif_ind) {
    if (!cJSON_IsNull(direct_notif_ind)) {
    if (!cJSON_IsBool(direct_notif_ind)) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_parseFromJSON() failed [direct_notif_ind]");
        goto end;
    }
    }
    }

    events_subsc_req_data_rm_local_var = OpenAPI_events_subsc_req_data_rm_create (
        eventsList,
        notif_uri && !cJSON_IsNull(notif_uri) ? ogs_strdup(notif_uri->valuestring) : NULL,
        req_qos_mon_params ? req_qos_mon_paramsList : NULL,
        qos_mon && cJSON_IsNull(qos_mon) ? true : false,
        qos_mon ? qos_mon_local_nonprim : NULL,
        req_anis ? req_anisList : NULL,
        usg_thres && cJSON_IsNull(usg_thres) ? true : false,
        usg_thres ? usg_thres_local_nonprim : NULL,
        notif_corre_id && !cJSON_IsNull(notif_corre_id) ? ogs_strdup(notif_corre_id->valuestring) : NULL,
        direct_notif_ind && cJSON_IsNull(direct_notif_ind) ? true : false,
        direct_notif_ind ? true : false,
        direct_notif_ind ? direct_notif_ind->valueint : 0
    );

    return events_subsc_req_data_rm_local_var;
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
        OpenAPI_qos_monitoring_information_rm_free(qos_mon_local_nonprim);
        qos_mon_local_nonprim = NULL;
    }
    if (req_anisList) {
        OpenAPI_list_free(req_anisList);
        req_anisList = NULL;
    }
    if (usg_thres_local_nonprim) {
        OpenAPI_usage_threshold_rm_free(usg_thres_local_nonprim);
        usg_thres_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_events_subsc_req_data_rm_t *OpenAPI_events_subsc_req_data_rm_copy(OpenAPI_events_subsc_req_data_rm_t *dst, OpenAPI_events_subsc_req_data_rm_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_events_subsc_req_data_rm_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_events_subsc_req_data_rm_convertToJSON() failed");
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

    OpenAPI_events_subsc_req_data_rm_free(dst);
    dst = OpenAPI_events_subsc_req_data_rm_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

