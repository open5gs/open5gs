
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_parameter_data.h"

OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_create(
    char *app_id,
    char *dnn,
    OpenAPI_snssai_t *snssai,
    char *inter_group_id,
    char *supi,
    char *ue_ipv4,
    char *ue_ipv6,
    char *ue_mac,
    bool is_any_ue_ind,
    int any_ue_ind,
    char *param_over_pc5,
    char *param_over_uu,
    char *param_for_pro_se_dd,
    char *param_for_pro_se_dc,
    char *param_for_pro_se_u2_n_rel_ue,
    char *param_for_pro_se_rem_ue,
    OpenAPI_list_t *ursp_guidance,
    OpenAPI_list_t *delivery_events,
    char *polic_deliv_notif_corre_id,
    char *polic_deliv_notif_uri,
    char *supp_feat,
    char *res_uri,
    OpenAPI_list_t *headers,
    OpenAPI_list_t *reset_ids
)
{
    OpenAPI_service_parameter_data_t *service_parameter_data_local_var = ogs_malloc(sizeof(OpenAPI_service_parameter_data_t));
    ogs_assert(service_parameter_data_local_var);

    service_parameter_data_local_var->app_id = app_id;
    service_parameter_data_local_var->dnn = dnn;
    service_parameter_data_local_var->snssai = snssai;
    service_parameter_data_local_var->inter_group_id = inter_group_id;
    service_parameter_data_local_var->supi = supi;
    service_parameter_data_local_var->ue_ipv4 = ue_ipv4;
    service_parameter_data_local_var->ue_ipv6 = ue_ipv6;
    service_parameter_data_local_var->ue_mac = ue_mac;
    service_parameter_data_local_var->is_any_ue_ind = is_any_ue_ind;
    service_parameter_data_local_var->any_ue_ind = any_ue_ind;
    service_parameter_data_local_var->param_over_pc5 = param_over_pc5;
    service_parameter_data_local_var->param_over_uu = param_over_uu;
    service_parameter_data_local_var->param_for_pro_se_dd = param_for_pro_se_dd;
    service_parameter_data_local_var->param_for_pro_se_dc = param_for_pro_se_dc;
    service_parameter_data_local_var->param_for_pro_se_u2_n_rel_ue = param_for_pro_se_u2_n_rel_ue;
    service_parameter_data_local_var->param_for_pro_se_rem_ue = param_for_pro_se_rem_ue;
    service_parameter_data_local_var->ursp_guidance = ursp_guidance;
    service_parameter_data_local_var->delivery_events = delivery_events;
    service_parameter_data_local_var->polic_deliv_notif_corre_id = polic_deliv_notif_corre_id;
    service_parameter_data_local_var->polic_deliv_notif_uri = polic_deliv_notif_uri;
    service_parameter_data_local_var->supp_feat = supp_feat;
    service_parameter_data_local_var->res_uri = res_uri;
    service_parameter_data_local_var->headers = headers;
    service_parameter_data_local_var->reset_ids = reset_ids;

    return service_parameter_data_local_var;
}

void OpenAPI_service_parameter_data_free(OpenAPI_service_parameter_data_t *service_parameter_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == service_parameter_data) {
        return;
    }
    if (service_parameter_data->app_id) {
        ogs_free(service_parameter_data->app_id);
        service_parameter_data->app_id = NULL;
    }
    if (service_parameter_data->dnn) {
        ogs_free(service_parameter_data->dnn);
        service_parameter_data->dnn = NULL;
    }
    if (service_parameter_data->snssai) {
        OpenAPI_snssai_free(service_parameter_data->snssai);
        service_parameter_data->snssai = NULL;
    }
    if (service_parameter_data->inter_group_id) {
        ogs_free(service_parameter_data->inter_group_id);
        service_parameter_data->inter_group_id = NULL;
    }
    if (service_parameter_data->supi) {
        ogs_free(service_parameter_data->supi);
        service_parameter_data->supi = NULL;
    }
    if (service_parameter_data->ue_ipv4) {
        ogs_free(service_parameter_data->ue_ipv4);
        service_parameter_data->ue_ipv4 = NULL;
    }
    if (service_parameter_data->ue_ipv6) {
        ogs_free(service_parameter_data->ue_ipv6);
        service_parameter_data->ue_ipv6 = NULL;
    }
    if (service_parameter_data->ue_mac) {
        ogs_free(service_parameter_data->ue_mac);
        service_parameter_data->ue_mac = NULL;
    }
    if (service_parameter_data->param_over_pc5) {
        ogs_free(service_parameter_data->param_over_pc5);
        service_parameter_data->param_over_pc5 = NULL;
    }
    if (service_parameter_data->param_over_uu) {
        ogs_free(service_parameter_data->param_over_uu);
        service_parameter_data->param_over_uu = NULL;
    }
    if (service_parameter_data->param_for_pro_se_dd) {
        ogs_free(service_parameter_data->param_for_pro_se_dd);
        service_parameter_data->param_for_pro_se_dd = NULL;
    }
    if (service_parameter_data->param_for_pro_se_dc) {
        ogs_free(service_parameter_data->param_for_pro_se_dc);
        service_parameter_data->param_for_pro_se_dc = NULL;
    }
    if (service_parameter_data->param_for_pro_se_u2_n_rel_ue) {
        ogs_free(service_parameter_data->param_for_pro_se_u2_n_rel_ue);
        service_parameter_data->param_for_pro_se_u2_n_rel_ue = NULL;
    }
    if (service_parameter_data->param_for_pro_se_rem_ue) {
        ogs_free(service_parameter_data->param_for_pro_se_rem_ue);
        service_parameter_data->param_for_pro_se_rem_ue = NULL;
    }
    if (service_parameter_data->ursp_guidance) {
        OpenAPI_list_for_each(service_parameter_data->ursp_guidance, node) {
            OpenAPI_ursp_rule_request_free(node->data);
        }
        OpenAPI_list_free(service_parameter_data->ursp_guidance);
        service_parameter_data->ursp_guidance = NULL;
    }
    if (service_parameter_data->delivery_events) {
        OpenAPI_list_for_each(service_parameter_data->delivery_events, node) {
            OpenAPI_event_free(node->data);
        }
        OpenAPI_list_free(service_parameter_data->delivery_events);
        service_parameter_data->delivery_events = NULL;
    }
    if (service_parameter_data->polic_deliv_notif_corre_id) {
        ogs_free(service_parameter_data->polic_deliv_notif_corre_id);
        service_parameter_data->polic_deliv_notif_corre_id = NULL;
    }
    if (service_parameter_data->polic_deliv_notif_uri) {
        ogs_free(service_parameter_data->polic_deliv_notif_uri);
        service_parameter_data->polic_deliv_notif_uri = NULL;
    }
    if (service_parameter_data->supp_feat) {
        ogs_free(service_parameter_data->supp_feat);
        service_parameter_data->supp_feat = NULL;
    }
    if (service_parameter_data->res_uri) {
        ogs_free(service_parameter_data->res_uri);
        service_parameter_data->res_uri = NULL;
    }
    if (service_parameter_data->headers) {
        OpenAPI_list_for_each(service_parameter_data->headers, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(service_parameter_data->headers);
        service_parameter_data->headers = NULL;
    }
    if (service_parameter_data->reset_ids) {
        OpenAPI_list_for_each(service_parameter_data->reset_ids, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(service_parameter_data->reset_ids);
        service_parameter_data->reset_ids = NULL;
    }
    ogs_free(service_parameter_data);
}

cJSON *OpenAPI_service_parameter_data_convertToJSON(OpenAPI_service_parameter_data_t *service_parameter_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (service_parameter_data == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [ServiceParameterData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (service_parameter_data->app_id) {
    if (cJSON_AddStringToObject(item, "appId", service_parameter_data->app_id) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (service_parameter_data->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", service_parameter_data->dnn) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (service_parameter_data->snssai) {
    cJSON *snssai_local_JSON = OpenAPI_snssai_convertToJSON(service_parameter_data->snssai);
    if (snssai_local_JSON == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [snssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "snssai", snssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [snssai]");
        goto end;
    }
    }

    if (service_parameter_data->inter_group_id) {
    if (cJSON_AddStringToObject(item, "interGroupId", service_parameter_data->inter_group_id) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [inter_group_id]");
        goto end;
    }
    }

    if (service_parameter_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", service_parameter_data->supi) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (service_parameter_data->ue_ipv4) {
    if (cJSON_AddStringToObject(item, "ueIpv4", service_parameter_data->ue_ipv4) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [ue_ipv4]");
        goto end;
    }
    }

    if (service_parameter_data->ue_ipv6) {
    if (cJSON_AddStringToObject(item, "ueIpv6", service_parameter_data->ue_ipv6) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [ue_ipv6]");
        goto end;
    }
    }

    if (service_parameter_data->ue_mac) {
    if (cJSON_AddStringToObject(item, "ueMac", service_parameter_data->ue_mac) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [ue_mac]");
        goto end;
    }
    }

    if (service_parameter_data->is_any_ue_ind) {
    if (cJSON_AddBoolToObject(item, "anyUeInd", service_parameter_data->any_ue_ind) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [any_ue_ind]");
        goto end;
    }
    }

    if (service_parameter_data->param_over_pc5) {
    if (cJSON_AddStringToObject(item, "paramOverPc5", service_parameter_data->param_over_pc5) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [param_over_pc5]");
        goto end;
    }
    }

    if (service_parameter_data->param_over_uu) {
    if (cJSON_AddStringToObject(item, "paramOverUu", service_parameter_data->param_over_uu) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [param_over_uu]");
        goto end;
    }
    }

    if (service_parameter_data->param_for_pro_se_dd) {
    if (cJSON_AddStringToObject(item, "paramForProSeDd", service_parameter_data->param_for_pro_se_dd) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [param_for_pro_se_dd]");
        goto end;
    }
    }

    if (service_parameter_data->param_for_pro_se_dc) {
    if (cJSON_AddStringToObject(item, "paramForProSeDc", service_parameter_data->param_for_pro_se_dc) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [param_for_pro_se_dc]");
        goto end;
    }
    }

    if (service_parameter_data->param_for_pro_se_u2_n_rel_ue) {
    if (cJSON_AddStringToObject(item, "paramForProSeU2NRelUe", service_parameter_data->param_for_pro_se_u2_n_rel_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [param_for_pro_se_u2_n_rel_ue]");
        goto end;
    }
    }

    if (service_parameter_data->param_for_pro_se_rem_ue) {
    if (cJSON_AddStringToObject(item, "paramForProSeRemUe", service_parameter_data->param_for_pro_se_rem_ue) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [param_for_pro_se_rem_ue]");
        goto end;
    }
    }

    if (service_parameter_data->ursp_guidance) {
    cJSON *ursp_guidanceList = cJSON_AddArrayToObject(item, "urspGuidance");
    if (ursp_guidanceList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [ursp_guidance]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data->ursp_guidance, node) {
        cJSON *itemLocal = OpenAPI_ursp_rule_request_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [ursp_guidance]");
            goto end;
        }
        cJSON_AddItemToArray(ursp_guidanceList, itemLocal);
    }
    }

    if (service_parameter_data->delivery_events) {
    cJSON *delivery_eventsList = cJSON_AddArrayToObject(item, "deliveryEvents");
    if (delivery_eventsList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [delivery_events]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data->delivery_events, node) {
        cJSON *itemLocal = OpenAPI_event_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [delivery_events]");
            goto end;
        }
        cJSON_AddItemToArray(delivery_eventsList, itemLocal);
    }
    }

    if (service_parameter_data->polic_deliv_notif_corre_id) {
    if (cJSON_AddStringToObject(item, "policDelivNotifCorreId", service_parameter_data->polic_deliv_notif_corre_id) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [polic_deliv_notif_corre_id]");
        goto end;
    }
    }

    if (service_parameter_data->polic_deliv_notif_uri) {
    if (cJSON_AddStringToObject(item, "policDelivNotifUri", service_parameter_data->polic_deliv_notif_uri) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [polic_deliv_notif_uri]");
        goto end;
    }
    }

    if (service_parameter_data->supp_feat) {
    if (cJSON_AddStringToObject(item, "suppFeat", service_parameter_data->supp_feat) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [supp_feat]");
        goto end;
    }
    }

    if (service_parameter_data->res_uri) {
    if (cJSON_AddStringToObject(item, "resUri", service_parameter_data->res_uri) == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [res_uri]");
        goto end;
    }
    }

    if (service_parameter_data->headers) {
    cJSON *headersList = cJSON_AddArrayToObject(item, "headers");
    if (headersList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [headers]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data->headers, node) {
        if (cJSON_AddStringToObject(headersList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [headers]");
            goto end;
        }
    }
    }

    if (service_parameter_data->reset_ids) {
    cJSON *reset_idsList = cJSON_AddArrayToObject(item, "resetIds");
    if (reset_idsList == NULL) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [reset_ids]");
        goto end;
    }
    OpenAPI_list_for_each(service_parameter_data->reset_ids, node) {
        if (cJSON_AddStringToObject(reset_idsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed [reset_ids]");
            goto end;
        }
    }
    }

end:
    return item;
}

OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_parseFromJSON(cJSON *service_parameter_dataJSON)
{
    OpenAPI_service_parameter_data_t *service_parameter_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *app_id = NULL;
    cJSON *dnn = NULL;
    cJSON *snssai = NULL;
    OpenAPI_snssai_t *snssai_local_nonprim = NULL;
    cJSON *inter_group_id = NULL;
    cJSON *supi = NULL;
    cJSON *ue_ipv4 = NULL;
    cJSON *ue_ipv6 = NULL;
    cJSON *ue_mac = NULL;
    cJSON *any_ue_ind = NULL;
    cJSON *param_over_pc5 = NULL;
    cJSON *param_over_uu = NULL;
    cJSON *param_for_pro_se_dd = NULL;
    cJSON *param_for_pro_se_dc = NULL;
    cJSON *param_for_pro_se_u2_n_rel_ue = NULL;
    cJSON *param_for_pro_se_rem_ue = NULL;
    cJSON *ursp_guidance = NULL;
    OpenAPI_list_t *ursp_guidanceList = NULL;
    cJSON *delivery_events = NULL;
    OpenAPI_list_t *delivery_eventsList = NULL;
    cJSON *polic_deliv_notif_corre_id = NULL;
    cJSON *polic_deliv_notif_uri = NULL;
    cJSON *supp_feat = NULL;
    cJSON *res_uri = NULL;
    cJSON *headers = NULL;
    OpenAPI_list_t *headersList = NULL;
    cJSON *reset_ids = NULL;
    OpenAPI_list_t *reset_idsList = NULL;
    app_id = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    snssai = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "snssai");
    if (snssai) {
    snssai_local_nonprim = OpenAPI_snssai_parseFromJSON(snssai);
    if (!snssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [snssai]");
        goto end;
    }
    }

    inter_group_id = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "interGroupId");
    if (inter_group_id) {
    if (!cJSON_IsString(inter_group_id) && !cJSON_IsNull(inter_group_id)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [inter_group_id]");
        goto end;
    }
    }

    supi = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    ue_ipv4 = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "ueIpv4");
    if (ue_ipv4) {
    if (!cJSON_IsString(ue_ipv4) && !cJSON_IsNull(ue_ipv4)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [ue_ipv4]");
        goto end;
    }
    }

    ue_ipv6 = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "ueIpv6");
    if (ue_ipv6) {
    if (!cJSON_IsString(ue_ipv6) && !cJSON_IsNull(ue_ipv6)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [ue_ipv6]");
        goto end;
    }
    }

    ue_mac = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "ueMac");
    if (ue_mac) {
    if (!cJSON_IsString(ue_mac) && !cJSON_IsNull(ue_mac)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [ue_mac]");
        goto end;
    }
    }

    any_ue_ind = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "anyUeInd");
    if (any_ue_ind) {
    if (!cJSON_IsBool(any_ue_ind)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [any_ue_ind]");
        goto end;
    }
    }

    param_over_pc5 = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "paramOverPc5");
    if (param_over_pc5) {
    if (!cJSON_IsString(param_over_pc5) && !cJSON_IsNull(param_over_pc5)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [param_over_pc5]");
        goto end;
    }
    }

    param_over_uu = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "paramOverUu");
    if (param_over_uu) {
    if (!cJSON_IsString(param_over_uu) && !cJSON_IsNull(param_over_uu)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [param_over_uu]");
        goto end;
    }
    }

    param_for_pro_se_dd = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "paramForProSeDd");
    if (param_for_pro_se_dd) {
    if (!cJSON_IsString(param_for_pro_se_dd) && !cJSON_IsNull(param_for_pro_se_dd)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [param_for_pro_se_dd]");
        goto end;
    }
    }

    param_for_pro_se_dc = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "paramForProSeDc");
    if (param_for_pro_se_dc) {
    if (!cJSON_IsString(param_for_pro_se_dc) && !cJSON_IsNull(param_for_pro_se_dc)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [param_for_pro_se_dc]");
        goto end;
    }
    }

    param_for_pro_se_u2_n_rel_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "paramForProSeU2NRelUe");
    if (param_for_pro_se_u2_n_rel_ue) {
    if (!cJSON_IsString(param_for_pro_se_u2_n_rel_ue) && !cJSON_IsNull(param_for_pro_se_u2_n_rel_ue)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [param_for_pro_se_u2_n_rel_ue]");
        goto end;
    }
    }

    param_for_pro_se_rem_ue = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "paramForProSeRemUe");
    if (param_for_pro_se_rem_ue) {
    if (!cJSON_IsString(param_for_pro_se_rem_ue) && !cJSON_IsNull(param_for_pro_se_rem_ue)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [param_for_pro_se_rem_ue]");
        goto end;
    }
    }

    ursp_guidance = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "urspGuidance");
    if (ursp_guidance) {
        cJSON *ursp_guidance_local = NULL;
        if (!cJSON_IsArray(ursp_guidance)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [ursp_guidance]");
            goto end;
        }

        ursp_guidanceList = OpenAPI_list_create();

        cJSON_ArrayForEach(ursp_guidance_local, ursp_guidance) {
            if (!cJSON_IsObject(ursp_guidance_local)) {
                ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [ursp_guidance]");
                goto end;
            }
            OpenAPI_ursp_rule_request_t *ursp_guidanceItem = OpenAPI_ursp_rule_request_parseFromJSON(ursp_guidance_local);
            if (!ursp_guidanceItem) {
                ogs_error("No ursp_guidanceItem");
                goto end;
            }
            OpenAPI_list_add(ursp_guidanceList, ursp_guidanceItem);
        }
    }

    delivery_events = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "deliveryEvents");
    if (delivery_events) {
        cJSON *delivery_events_local = NULL;
        if (!cJSON_IsArray(delivery_events)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [delivery_events]");
            goto end;
        }

        delivery_eventsList = OpenAPI_list_create();

        cJSON_ArrayForEach(delivery_events_local, delivery_events) {
            if (!cJSON_IsObject(delivery_events_local)) {
                ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [delivery_events]");
                goto end;
            }
            OpenAPI_event_t *delivery_eventsItem = OpenAPI_event_parseFromJSON(delivery_events_local);
            if (!delivery_eventsItem) {
                ogs_error("No delivery_eventsItem");
                goto end;
            }
            OpenAPI_list_add(delivery_eventsList, delivery_eventsItem);
        }
    }

    polic_deliv_notif_corre_id = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "policDelivNotifCorreId");
    if (polic_deliv_notif_corre_id) {
    if (!cJSON_IsString(polic_deliv_notif_corre_id) && !cJSON_IsNull(polic_deliv_notif_corre_id)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [polic_deliv_notif_corre_id]");
        goto end;
    }
    }

    polic_deliv_notif_uri = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "policDelivNotifUri");
    if (polic_deliv_notif_uri) {
    if (!cJSON_IsString(polic_deliv_notif_uri) && !cJSON_IsNull(polic_deliv_notif_uri)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [polic_deliv_notif_uri]");
        goto end;
    }
    }

    supp_feat = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "suppFeat");
    if (supp_feat) {
    if (!cJSON_IsString(supp_feat) && !cJSON_IsNull(supp_feat)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [supp_feat]");
        goto end;
    }
    }

    res_uri = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "resUri");
    if (res_uri) {
    if (!cJSON_IsString(res_uri) && !cJSON_IsNull(res_uri)) {
        ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [res_uri]");
        goto end;
    }
    }

    headers = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "headers");
    if (headers) {
        cJSON *headers_local = NULL;
        if (!cJSON_IsArray(headers)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [headers]");
            goto end;
        }

        headersList = OpenAPI_list_create();

        cJSON_ArrayForEach(headers_local, headers) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(headers_local)) {
                ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [headers]");
                goto end;
            }
            OpenAPI_list_add(headersList, ogs_strdup(headers_local->valuestring));
        }
    }

    reset_ids = cJSON_GetObjectItemCaseSensitive(service_parameter_dataJSON, "resetIds");
    if (reset_ids) {
        cJSON *reset_ids_local = NULL;
        if (!cJSON_IsArray(reset_ids)) {
            ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [reset_ids]");
            goto end;
        }

        reset_idsList = OpenAPI_list_create();

        cJSON_ArrayForEach(reset_ids_local, reset_ids) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(reset_ids_local)) {
                ogs_error("OpenAPI_service_parameter_data_parseFromJSON() failed [reset_ids]");
                goto end;
            }
            OpenAPI_list_add(reset_idsList, ogs_strdup(reset_ids_local->valuestring));
        }
    }

    service_parameter_data_local_var = OpenAPI_service_parameter_data_create (
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        snssai ? snssai_local_nonprim : NULL,
        inter_group_id && !cJSON_IsNull(inter_group_id) ? ogs_strdup(inter_group_id->valuestring) : NULL,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        ue_ipv4 && !cJSON_IsNull(ue_ipv4) ? ogs_strdup(ue_ipv4->valuestring) : NULL,
        ue_ipv6 && !cJSON_IsNull(ue_ipv6) ? ogs_strdup(ue_ipv6->valuestring) : NULL,
        ue_mac && !cJSON_IsNull(ue_mac) ? ogs_strdup(ue_mac->valuestring) : NULL,
        any_ue_ind ? true : false,
        any_ue_ind ? any_ue_ind->valueint : 0,
        param_over_pc5 && !cJSON_IsNull(param_over_pc5) ? ogs_strdup(param_over_pc5->valuestring) : NULL,
        param_over_uu && !cJSON_IsNull(param_over_uu) ? ogs_strdup(param_over_uu->valuestring) : NULL,
        param_for_pro_se_dd && !cJSON_IsNull(param_for_pro_se_dd) ? ogs_strdup(param_for_pro_se_dd->valuestring) : NULL,
        param_for_pro_se_dc && !cJSON_IsNull(param_for_pro_se_dc) ? ogs_strdup(param_for_pro_se_dc->valuestring) : NULL,
        param_for_pro_se_u2_n_rel_ue && !cJSON_IsNull(param_for_pro_se_u2_n_rel_ue) ? ogs_strdup(param_for_pro_se_u2_n_rel_ue->valuestring) : NULL,
        param_for_pro_se_rem_ue && !cJSON_IsNull(param_for_pro_se_rem_ue) ? ogs_strdup(param_for_pro_se_rem_ue->valuestring) : NULL,
        ursp_guidance ? ursp_guidanceList : NULL,
        delivery_events ? delivery_eventsList : NULL,
        polic_deliv_notif_corre_id && !cJSON_IsNull(polic_deliv_notif_corre_id) ? ogs_strdup(polic_deliv_notif_corre_id->valuestring) : NULL,
        polic_deliv_notif_uri && !cJSON_IsNull(polic_deliv_notif_uri) ? ogs_strdup(polic_deliv_notif_uri->valuestring) : NULL,
        supp_feat && !cJSON_IsNull(supp_feat) ? ogs_strdup(supp_feat->valuestring) : NULL,
        res_uri && !cJSON_IsNull(res_uri) ? ogs_strdup(res_uri->valuestring) : NULL,
        headers ? headersList : NULL,
        reset_ids ? reset_idsList : NULL
    );

    return service_parameter_data_local_var;
end:
    if (snssai_local_nonprim) {
        OpenAPI_snssai_free(snssai_local_nonprim);
        snssai_local_nonprim = NULL;
    }
    if (ursp_guidanceList) {
        OpenAPI_list_for_each(ursp_guidanceList, node) {
            OpenAPI_ursp_rule_request_free(node->data);
        }
        OpenAPI_list_free(ursp_guidanceList);
        ursp_guidanceList = NULL;
    }
    if (delivery_eventsList) {
        OpenAPI_list_for_each(delivery_eventsList, node) {
            OpenAPI_event_free(node->data);
        }
        OpenAPI_list_free(delivery_eventsList);
        delivery_eventsList = NULL;
    }
    if (headersList) {
        OpenAPI_list_for_each(headersList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(headersList);
        headersList = NULL;
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

OpenAPI_service_parameter_data_t *OpenAPI_service_parameter_data_copy(OpenAPI_service_parameter_data_t *dst, OpenAPI_service_parameter_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_parameter_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_parameter_data_convertToJSON() failed");
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

    OpenAPI_service_parameter_data_free(dst);
    dst = OpenAPI_service_parameter_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

