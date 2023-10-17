
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "am_influ_data.h"

OpenAPI_am_influ_data_t *OpenAPI_am_influ_data_create(
    OpenAPI_list_t *app_ids,
    OpenAPI_list_t *dnn_snssai_infos,
    char *inter_group_id,
    char *supi,
    bool is_any_ue_ind,
    int any_ue_ind,
    bool is_policy_duration,
    int policy_duration,
    OpenAPI_list_t *ev_subs,
    char *notif_uri,
    char *notif_corr_id,
    OpenAPI_list_t *headers,
    bool is_thru_req,
    int thru_req,
    OpenAPI_list_t *cov_req,
    char *supported_features,
    char *res_uri,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_am_influ_data_t *am_influ_data_local_var = ogs_malloc(sizeof(OpenAPI_am_influ_data_t));
    ogs_assert(am_influ_data_local_var);

    am_influ_data_local_var->app_ids = app_ids;
    am_influ_data_local_var->dnn_snssai_infos = dnn_snssai_infos;
    am_influ_data_local_var->inter_group_id = inter_group_id;
    am_influ_data_local_var->supi = supi;
    am_influ_data_local_var->is_any_ue_ind = is_any_ue_ind;
    am_influ_data_local_var->any_ue_ind = any_ue_ind;
    am_influ_data_local_var->is_policy_duration = is_policy_duration;
    am_influ_data_local_var->policy_duration = policy_duration;
    am_influ_data_local_var->ev_subs = ev_subs;
    am_influ_data_local_var->notif_uri = notif_uri;
    am_influ_data_local_var->notif_corr_id = notif_corr_id;
    am_influ_data_local_var->headers = headers;
    am_influ_data_local_var->is_thru_req = is_thru_req;
    am_influ_data_local_var->thru_req = thru_req;
    am_influ_data_local_var->cov_req = cov_req;
    am_influ_data_local_var->supported_features = supported_features;
    am_influ_data_local_var->res_uri = res_uri;
    am_influ_data_local_var->reset_ids = reset_ids;

    return am_influ_data_local_var;
}

void OpenAPI_am_influ_data_free(OpenAPI_am_influ_data_t *am_influ_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == am_influ_data) {
        return;
    }
    if (am_influ_data->app_ids) {
        OpenAPI_list_for_each(am_influ_data->app_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(am_influ_data->app_ids);
        am_influ_data->app_ids = NULL;
    }
    if (am_influ_data->dnn_snssai_infos) {
        OpenAPI_list_for_each(am_influ_data->dnn_snssai_infos, node) {
            OpenAPI_dnn_snssai_information_free(node->data);
        }
        OpenAPI_list_free(am_influ_data->dnn_snssai_infos);
        am_influ_data->dnn_snssai_infos = NULL;
    }
    if (am_influ_data->inter_group_id) {
        ogs_free(am_influ_data->inter_group_id);
        am_influ_data->inter_group_id = NULL;
    }
    if (am_influ_data->supi) {
        ogs_free(am_influ_data->supi);
        am_influ_data->supi = NULL;
    }
    if (am_influ_data->ev_subs) {
        OpenAPI_list_for_each(am_influ_data->ev_subs, node) {
            OpenAPI_am_influ_event_free(node->data);
        }
        OpenAPI_list_free(am_influ_data->ev_subs);
        am_influ_data->ev_subs = NULL;
    }
    if (am_influ_data->notif_uri) {
        ogs_free(am_influ_data->notif_uri);
        am_influ_data->notif_uri = NULL;
    }
    if (am_influ_data->notif_corr_id) {
        ogs_free(am_influ_data->notif_corr_id);
        am_influ_data->notif_corr_id = NULL;
    }
    if (am_influ_data->headers) {
        OpenAPI_list_for_each(am_influ_data->headers, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(am_influ_data->headers);
        am_influ_data->headers = NULL;
    }
    if (am_influ_data->cov_req) {
        OpenAPI_list_for_each(am_influ_data->cov_req, node) {
            OpenAPI_service_area_coverage_info_free(node->data);
        }
        OpenAPI_list_free(am_influ_data->cov_req);
        am_influ_data->cov_req = NULL;
    }
    if (am_influ_data->supported_features) {
        ogs_free(am_influ_data->supported_features);
        am_influ_data->supported_features = NULL;
    }
    if (am_influ_data->res_uri) {
        ogs_free(am_influ_data->res_uri);
        am_influ_data->res_uri = NULL;
    }
    if (am_influ_data->reset_ids) {
        OpenAPI_list_for_each(am_influ_data->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(am_influ_data->reset_ids);
        am_influ_data->reset_ids = NULL;
    }
    ogs_free(am_influ_data);
}

cJSON *OpenAPI_am_influ_data_convertToJSON(OpenAPI_am_influ_data_t *am_influ_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (am_influ_data == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [AmInfluData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (am_influ_data->app_ids) {
    cJSON *app_idsList = cJSON_AddArrayToObject(item, "appIds");
    if (app_idsList == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [app_ids]");
        goto end;
    }
    OpenAPI_list_for_each(am_influ_data->app_ids, node) {
        if (cJSON_AddStringToObject(app_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [app_ids]");
            goto end;
        }
    }
    }

    if (am_influ_data->dnn_snssai_infos) {
    cJSON *dnn_snssai_infosList = cJSON_AddArrayToObject(item, "dnnSnssaiInfos");
    if (dnn_snssai_infosList == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [dnn_snssai_infos]");
        goto end;
    }
    OpenAPI_list_for_each(am_influ_data->dnn_snssai_infos, node) {
        cJSON *itemLocal = OpenAPI_dnn_snssai_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [dnn_snssai_infos]");
            goto end;
        }
        cJSON_AddItemToArray(dnn_snssai_infosList, itemLocal);
    }
    }

    if (am_influ_data->inter_group_id) {
    if (cJSON_AddStringToObject(item, "interGroupId", am_influ_data->inter_group_id) == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [inter_group_id]");
        goto end;
    }
    }

    if (am_influ_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", am_influ_data->supi) == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (am_influ_data->is_any_ue_ind) {
    if (cJSON_AddBoolToObject(item, "anyUeInd", am_influ_data->any_ue_ind) == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [any_ue_ind]");
        goto end;
    }
    }

    if (am_influ_data->is_policy_duration) {
    if (cJSON_AddNumberToObject(item, "policyDuration", am_influ_data->policy_duration) == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [policy_duration]");
        goto end;
    }
    }

    if (am_influ_data->ev_subs) {
    cJSON *ev_subsList = cJSON_AddArrayToObject(item, "evSubs");
    if (ev_subsList == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [ev_subs]");
        goto end;
    }
    OpenAPI_list_for_each(am_influ_data->ev_subs, node) {
        cJSON *itemLocal = OpenAPI_am_influ_event_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [ev_subs]");
            goto end;
        }
        cJSON_AddItemToArray(ev_subsList, itemLocal);
    }
    }

    if (am_influ_data->notif_uri) {
    if (cJSON_AddStringToObject(item, "notifUri", am_influ_data->notif_uri) == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [notif_uri]");
        goto end;
    }
    }

    if (am_influ_data->notif_corr_id) {
    if (cJSON_AddStringToObject(item, "notifCorrId", am_influ_data->notif_corr_id) == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [notif_corr_id]");
        goto end;
    }
    }

    if (am_influ_data->headers) {
    cJSON *headersList = cJSON_AddArrayToObject(item, "headers");
    if (headersList == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [headers]");
        goto end;
    }
    OpenAPI_list_for_each(am_influ_data->headers, node) {
        if (cJSON_AddStringToObject(headersList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [headers]");
            goto end;
        }
    }
    }

    if (am_influ_data->is_thru_req) {
    if (cJSON_AddBoolToObject(item, "thruReq", am_influ_data->thru_req) == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [thru_req]");
        goto end;
    }
    }

    if (am_influ_data->cov_req) {
    cJSON *cov_reqList = cJSON_AddArrayToObject(item, "covReq");
    if (cov_reqList == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [cov_req]");
        goto end;
    }
    OpenAPI_list_for_each(am_influ_data->cov_req, node) {
        cJSON *itemLocal = OpenAPI_service_area_coverage_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [cov_req]");
            goto end;
        }
        cJSON_AddItemToArray(cov_reqList, itemLocal);
    }
    }

    if (am_influ_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", am_influ_data->supported_features) == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (am_influ_data->res_uri) {
    if (cJSON_AddStringToObject(item, "resUri", am_influ_data->res_uri) == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [res_uri]");
        goto end;
    }
    }

    if (am_influ_data->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(am_influ_data->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_am_influ_data_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_am_influ_data_t *OpenAPI_am_influ_data_parseFromJSON(cJSON *am_influ_dataJSON)
{
    OpenAPI_am_influ_data_t *am_influ_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_ids = NULL;
    OpenAPI_list_t *app_idsList = NULL;
    cJSON *dnn_snssai_infos = NULL;
    OpenAPI_list_t *dnn_snssai_infosList = NULL;
    cJSON *inter_group_id = NULL;
    cJSON *supi = NULL;
    cJSON *any_ue_ind = NULL;
    cJSON *policy_duration = NULL;
    cJSON *ev_subs = NULL;
    OpenAPI_list_t *ev_subsList = NULL;
    cJSON *notif_uri = NULL;
    cJSON *notif_corr_id = NULL;
    cJSON *headers = NULL;
    OpenAPI_list_t *headersList = NULL;
    cJSON *thru_req = NULL;
    cJSON *cov_req = NULL;
    OpenAPI_list_t *cov_reqList = NULL;
    cJSON *supported_features = NULL;
    cJSON *res_uri = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    app_ids = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "appIds");
    if (app_ids) {
        cJSON *app_ids_local = NULL;
        if (!cJSON_IsArray(app_ids)) {
            ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [app_ids]");
            goto end;
        }

        app_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(app_ids_local, app_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(app_ids_local)) {
                ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [app_ids]");
                goto end;
            }
            OpenAPI_list_add(app_idsList, ogs_strdup(app_ids_local->valuestring));
        }
    }

    dnn_snssai_infos = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "dnnSnssaiInfos");
    if (dnn_snssai_infos) {
        cJSON *dnn_snssai_infos_local = NULL;
        if (!cJSON_IsArray(dnn_snssai_infos)) {
            ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [dnn_snssai_infos]");
            goto end;
        }

        dnn_snssai_infosList = OpenAPI_list_create();

        cJSON_ArrayForEach(dnn_snssai_infos_local, dnn_snssai_infos) {
            if (!cJSON_IsObject(dnn_snssai_infos_local)) {
                ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [dnn_snssai_infos]");
                goto end;
            }
            OpenAPI_dnn_snssai_information_t *dnn_snssai_infosItem = OpenAPI_dnn_snssai_information_parseFromJSON(dnn_snssai_infos_local);
            if (!dnn_snssai_infosItem) {
                ogs_error("No dnn_snssai_infosItem");
                goto end;
            }
            OpenAPI_list_add(dnn_snssai_infosList, dnn_snssai_infosItem);
        }
    }

    inter_group_id = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "interGroupId");
    if (inter_group_id) {
    if (!cJSON_IsString(inter_group_id) && !cJSON_IsNull(inter_group_id)) {
        ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [inter_group_id]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    any_ue_ind = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "anyUeInd");
    if (any_ue_ind) {
    if (!cJSON_IsBool(any_ue_ind)) {
        ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [any_ue_ind]");
        goto end;
    }
    }

    policy_duration = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "policyDuration");
    if (policy_duration) {
    if (!cJSON_IsNumber(policy_duration)) {
        ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [policy_duration]");
        goto end;
    }
    }

    ev_subs = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "evSubs");
    if (ev_subs) {
        cJSON *ev_subs_local = NULL;
        if (!cJSON_IsArray(ev_subs)) {
            ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [ev_subs]");
            goto end;
        }

        ev_subsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ev_subs_local, ev_subs) {
            if (!cJSON_IsObject(ev_subs_local)) {
                ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [ev_subs]");
                goto end;
            }
            OpenAPI_am_influ_event_t *ev_subsItem = OpenAPI_am_influ_event_parseFromJSON(ev_subs_local);
            if (!ev_subsItem) {
                ogs_error("No ev_subsItem");
                goto end;
            }
            OpenAPI_list_add(ev_subsList, ev_subsItem);
        }
    }

    notif_uri = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "notifUri");
    if (notif_uri) {
    if (!cJSON_IsString(notif_uri) && !cJSON_IsNull(notif_uri)) {
        ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [notif_uri]");
        goto end;
    }
    }

    notif_corr_id = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "notifCorrId");
    if (notif_corr_id) {
    if (!cJSON_IsString(notif_corr_id) && !cJSON_IsNull(notif_corr_id)) {
        ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [notif_corr_id]");
        goto end;
    }
    }

    headers = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "headers");
    if (headers) {
        cJSON *headers_local = NULL;
        if (!cJSON_IsArray(headers)) {
            ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [headers]");
            goto end;
        }

        headersList = OpenAPI_list_create();

        cJSON_ArrayForEach(headers_local, headers) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(headers_local)) {
                ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [headers]");
                goto end;
            }
            OpenAPI_list_add(headersList, ogs_strdup(headers_local->valuestring));
        }
    }

    thru_req = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "thruReq");
    if (thru_req) {
    if (!cJSON_IsBool(thru_req)) {
        ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [thru_req]");
        goto end;
    }
    }

    cov_req = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "covReq");
    if (cov_req) {
        cJSON *cov_req_local = NULL;
        if (!cJSON_IsArray(cov_req)) {
            ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [cov_req]");
            goto end;
        }

        cov_reqList = OpenAPI_list_create();

        cJSON_ArrayForEach(cov_req_local, cov_req) {
            if (!cJSON_IsObject(cov_req_local)) {
                ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [cov_req]");
                goto end;
            }
            OpenAPI_service_area_coverage_info_t *cov_reqItem = OpenAPI_service_area_coverage_info_parseFromJSON(cov_req_local);
            if (!cov_reqItem) {
                ogs_error("No cov_reqItem");
                goto end;
            }
            OpenAPI_list_add(cov_reqList, cov_reqItem);
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    res_uri = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "resUri");
    if (res_uri) {
    if (!cJSON_IsString(res_uri) && !cJSON_IsNull(res_uri)) {
        ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [res_uri]");
        goto end;
    }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(am_influ_dataJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_am_influ_data_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    am_influ_data_local_var = OpenAPI_am_influ_data_create (
        app_ids ? app_idsList : NULL,
        dnn_snssai_infos ? dnn_snssai_infosList : NULL,
        inter_group_id && !cJSON_IsNull(inter_group_id) ? ogs_strdup(inter_group_id->valuestring) : NULL,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        any_ue_ind ? true : false,
        any_ue_ind ? any_ue_ind->valueint : 0,
        policy_duration ? true : false,
        policy_duration ? policy_duration->valuedouble : 0,
        ev_subs ? ev_subsList : NULL,
        notif_uri && !cJSON_IsNull(notif_uri) ? ogs_strdup(notif_uri->valuestring) : NULL,
        notif_corr_id && !cJSON_IsNull(notif_corr_id) ? ogs_strdup(notif_corr_id->valuestring) : NULL,
        headers ? headersList : NULL,
        thru_req ? true : false,
        thru_req ? thru_req->valueint : 0,
        cov_req ? cov_reqList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        res_uri && !cJSON_IsNull(res_uri) ? ogs_strdup(res_uri->valuestring) : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return am_influ_data_local_var;
end:
    if (app_idsList) {
        OpenAPI_list_for_each(app_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(app_idsList);
        app_idsList = NULL;
    }
    if (dnn_snssai_infosList) {
        OpenAPI_list_for_each(dnn_snssai_infosList, node) {
            OpenAPI_dnn_snssai_information_free(node->data);
        }
        OpenAPI_list_free(dnn_snssai_infosList);
        dnn_snssai_infosList = NULL;
    }
    if (ev_subsList) {
        OpenAPI_list_for_each(ev_subsList, node) {
            OpenAPI_am_influ_event_free(node->data);
        }
        OpenAPI_list_free(ev_subsList);
        ev_subsList = NULL;
    }
    if (headersList) {
        OpenAPI_list_for_each(headersList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(headersList);
        headersList = NULL;
    }
    if (cov_reqList) {
        OpenAPI_list_for_each(cov_reqList, node) {
            OpenAPI_service_area_coverage_info_free(node->data);
        }
        OpenAPI_list_free(cov_reqList);
        cov_reqList = NULL;
    }
    if (reset_idsList) {
        OpenAPI_list_for_each(reset_idsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(reset_idsList);
        reset_idsList = NULL;
    }
    return NULL;
}

OpenAPI_am_influ_data_t *OpenAPI_am_influ_data_copy(OpenAPI_am_influ_data_t *dst, OpenAPI_am_influ_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_am_influ_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_am_influ_data_convertToJSON() failed");
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

    OpenAPI_am_influ_data_free(dst);
    dst = OpenAPI_am_influ_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

