
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "policy_association_update_request.h"

OpenAPI_policy_association_update_request_t *OpenAPI_policy_association_update_request_create(
    char *notification_uri,
    OpenAPI_list_t *alt_notif_ipv4_addrs,
    OpenAPI_list_t *alt_notif_ipv6_addrs,
    OpenAPI_list_t *alt_notif_fqdns,
    OpenAPI_list_t *triggers,
    OpenAPI_service_area_restriction_t *serv_area_res,
    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res,
    bool is_rfsp,
    int rfsp,
    OpenAPI_smf_selection_data_t *smf_sel_info,
    OpenAPI_ambr_t *ue_ambr,
    OpenAPI_list_t* pra_statuses,
    OpenAPI_user_location_t *user_loc,
    OpenAPI_list_t *allowed_snssais,
    OpenAPI_list_t *mapping_snssais,
    OpenAPI_list_t *access_types,
    OpenAPI_list_t *rat_types,
    OpenAPI_list_t *n3g_allowed_snssais,
    OpenAPI_trace_data_t *trace_req,
    OpenAPI_guami_t *guami
)
{
    OpenAPI_policy_association_update_request_t *policy_association_update_request_local_var = ogs_malloc(sizeof(OpenAPI_policy_association_update_request_t));
    ogs_assert(policy_association_update_request_local_var);

    policy_association_update_request_local_var->notification_uri = notification_uri;
    policy_association_update_request_local_var->alt_notif_ipv4_addrs = alt_notif_ipv4_addrs;
    policy_association_update_request_local_var->alt_notif_ipv6_addrs = alt_notif_ipv6_addrs;
    policy_association_update_request_local_var->alt_notif_fqdns = alt_notif_fqdns;
    policy_association_update_request_local_var->triggers = triggers;
    policy_association_update_request_local_var->serv_area_res = serv_area_res;
    policy_association_update_request_local_var->wl_serv_area_res = wl_serv_area_res;
    policy_association_update_request_local_var->is_rfsp = is_rfsp;
    policy_association_update_request_local_var->rfsp = rfsp;
    policy_association_update_request_local_var->smf_sel_info = smf_sel_info;
    policy_association_update_request_local_var->ue_ambr = ue_ambr;
    policy_association_update_request_local_var->pra_statuses = pra_statuses;
    policy_association_update_request_local_var->user_loc = user_loc;
    policy_association_update_request_local_var->allowed_snssais = allowed_snssais;
    policy_association_update_request_local_var->mapping_snssais = mapping_snssais;
    policy_association_update_request_local_var->access_types = access_types;
    policy_association_update_request_local_var->rat_types = rat_types;
    policy_association_update_request_local_var->n3g_allowed_snssais = n3g_allowed_snssais;
    policy_association_update_request_local_var->trace_req = trace_req;
    policy_association_update_request_local_var->guami = guami;

    return policy_association_update_request_local_var;
}

void OpenAPI_policy_association_update_request_free(OpenAPI_policy_association_update_request_t *policy_association_update_request)
{
    if (NULL == policy_association_update_request) {
        return;
    }
    OpenAPI_lnode_t *node;
    ogs_free(policy_association_update_request->notification_uri);
    OpenAPI_list_for_each(policy_association_update_request->alt_notif_ipv4_addrs, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(policy_association_update_request->alt_notif_ipv4_addrs);
    OpenAPI_list_for_each(policy_association_update_request->alt_notif_ipv6_addrs, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(policy_association_update_request->alt_notif_ipv6_addrs);
    OpenAPI_list_for_each(policy_association_update_request->alt_notif_fqdns, node) {
        ogs_free(node->data);
    }
    OpenAPI_list_free(policy_association_update_request->alt_notif_fqdns);
    OpenAPI_list_free(policy_association_update_request->triggers);
    OpenAPI_service_area_restriction_free(policy_association_update_request->serv_area_res);
    OpenAPI_wireline_service_area_restriction_free(policy_association_update_request->wl_serv_area_res);
    OpenAPI_smf_selection_data_free(policy_association_update_request->smf_sel_info);
    OpenAPI_ambr_free(policy_association_update_request->ue_ambr);
    OpenAPI_list_for_each(policy_association_update_request->pra_statuses, node) {
        OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
        ogs_free(localKeyValue->key);
        OpenAPI_presence_info_free(localKeyValue->value);
        ogs_free(localKeyValue);
    }
    OpenAPI_list_free(policy_association_update_request->pra_statuses);
    OpenAPI_user_location_free(policy_association_update_request->user_loc);
    OpenAPI_list_for_each(policy_association_update_request->allowed_snssais, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(policy_association_update_request->allowed_snssais);
    OpenAPI_list_for_each(policy_association_update_request->mapping_snssais, node) {
        OpenAPI_mapping_of_snssai_free(node->data);
    }
    OpenAPI_list_free(policy_association_update_request->mapping_snssais);
    OpenAPI_list_free(policy_association_update_request->access_types);
    OpenAPI_list_free(policy_association_update_request->rat_types);
    OpenAPI_list_for_each(policy_association_update_request->n3g_allowed_snssais, node) {
        OpenAPI_snssai_free(node->data);
    }
    OpenAPI_list_free(policy_association_update_request->n3g_allowed_snssais);
    OpenAPI_trace_data_free(policy_association_update_request->trace_req);
    OpenAPI_guami_free(policy_association_update_request->guami);
    ogs_free(policy_association_update_request);
}

cJSON *OpenAPI_policy_association_update_request_convertToJSON(OpenAPI_policy_association_update_request_t *policy_association_update_request)
{
    cJSON *item = NULL;

    if (policy_association_update_request == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [PolicyAssociationUpdateRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (policy_association_update_request->notification_uri) {
    if (cJSON_AddStringToObject(item, "notificationUri", policy_association_update_request->notification_uri) == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [notification_uri]");
        goto end;
    }
    }

    if (policy_association_update_request->alt_notif_ipv4_addrs) {
    cJSON *alt_notif_ipv4_addrs = cJSON_AddArrayToObject(item, "altNotifIpv4Addrs");
    if (alt_notif_ipv4_addrs == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_ipv4_addrs]");
        goto end;
    }

    OpenAPI_lnode_t *alt_notif_ipv4_addrs_node;
    OpenAPI_list_for_each(policy_association_update_request->alt_notif_ipv4_addrs, alt_notif_ipv4_addrs_node)  {
    if (cJSON_AddStringToObject(alt_notif_ipv4_addrs, "", (char*)alt_notif_ipv4_addrs_node->data) == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_ipv4_addrs]");
        goto end;
    }
                    }
    }

    if (policy_association_update_request->alt_notif_ipv6_addrs) {
    cJSON *alt_notif_ipv6_addrs = cJSON_AddArrayToObject(item, "altNotifIpv6Addrs");
    if (alt_notif_ipv6_addrs == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_ipv6_addrs]");
        goto end;
    }

    OpenAPI_lnode_t *alt_notif_ipv6_addrs_node;
    OpenAPI_list_for_each(policy_association_update_request->alt_notif_ipv6_addrs, alt_notif_ipv6_addrs_node)  {
    if (cJSON_AddStringToObject(alt_notif_ipv6_addrs, "", (char*)alt_notif_ipv6_addrs_node->data) == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_ipv6_addrs]");
        goto end;
    }
                    }
    }

    if (policy_association_update_request->alt_notif_fqdns) {
    cJSON *alt_notif_fqdns = cJSON_AddArrayToObject(item, "altNotifFqdns");
    if (alt_notif_fqdns == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_fqdns]");
        goto end;
    }

    OpenAPI_lnode_t *alt_notif_fqdns_node;
    OpenAPI_list_for_each(policy_association_update_request->alt_notif_fqdns, alt_notif_fqdns_node)  {
    if (cJSON_AddStringToObject(alt_notif_fqdns, "", (char*)alt_notif_fqdns_node->data) == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_fqdns]");
        goto end;
    }
                    }
    }

    if (policy_association_update_request->triggers) {
    cJSON *triggers = cJSON_AddArrayToObject(item, "triggers");
    if (triggers == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_lnode_t *triggers_node;
    OpenAPI_list_for_each(policy_association_update_request->triggers, triggers_node) {
        if (cJSON_AddStringToObject(triggers, "", OpenAPI_request_trigger_ToString((intptr_t)triggers_node->data)) == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [triggers]");
            goto end;
        }
    }
    }

    if (policy_association_update_request->serv_area_res) {
    cJSON *serv_area_res_local_JSON = OpenAPI_service_area_restriction_convertToJSON(policy_association_update_request->serv_area_res);
    if (serv_area_res_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [serv_area_res]");
        goto end;
    }
    cJSON_AddItemToObject(item, "servAreaRes", serv_area_res_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [serv_area_res]");
        goto end;
    }
    }

    if (policy_association_update_request->wl_serv_area_res) {
    cJSON *wl_serv_area_res_local_JSON = OpenAPI_wireline_service_area_restriction_convertToJSON(policy_association_update_request->wl_serv_area_res);
    if (wl_serv_area_res_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [wl_serv_area_res]");
        goto end;
    }
    cJSON_AddItemToObject(item, "wlServAreaRes", wl_serv_area_res_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [wl_serv_area_res]");
        goto end;
    }
    }

    if (policy_association_update_request->is_rfsp) {
    if (cJSON_AddNumberToObject(item, "rfsp", policy_association_update_request->rfsp) == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [rfsp]");
        goto end;
    }
    }

    if (policy_association_update_request->smf_sel_info) {
    cJSON *smf_sel_info_local_JSON = OpenAPI_smf_selection_data_convertToJSON(policy_association_update_request->smf_sel_info);
    if (smf_sel_info_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [smf_sel_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "smfSelInfo", smf_sel_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [smf_sel_info]");
        goto end;
    }
    }

    if (policy_association_update_request->ue_ambr) {
    cJSON *ue_ambr_local_JSON = OpenAPI_ambr_convertToJSON(policy_association_update_request->ue_ambr);
    if (ue_ambr_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [ue_ambr]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueAmbr", ue_ambr_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [ue_ambr]");
        goto end;
    }
    }

    if (policy_association_update_request->pra_statuses) {
    cJSON *pra_statuses = cJSON_AddObjectToObject(item, "praStatuses");
    if (pra_statuses == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [pra_statuses]");
        goto end;
    }
    cJSON *localMapObject = pra_statuses;
    OpenAPI_lnode_t *pra_statuses_node;
    if (policy_association_update_request->pra_statuses) {
        OpenAPI_list_for_each(policy_association_update_request->pra_statuses, pra_statuses_node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)pra_statuses_node->data;
        cJSON *itemLocal = localKeyValue->value ?
            OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
            cJSON_CreateNull();
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [pra_statuses]");
            goto end;
        }
        cJSON_AddItemToObject(pra_statuses, localKeyValue->key, itemLocal);
            }
        }
    }

    if (policy_association_update_request->user_loc) {
    cJSON *user_loc_local_JSON = OpenAPI_user_location_convertToJSON(policy_association_update_request->user_loc);
    if (user_loc_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [user_loc]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLoc", user_loc_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [user_loc]");
        goto end;
    }
    }

    if (policy_association_update_request->allowed_snssais) {
    cJSON *allowed_snssaisList = cJSON_AddArrayToObject(item, "allowedSnssais");
    if (allowed_snssaisList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [allowed_snssais]");
        goto end;
    }

    OpenAPI_lnode_t *allowed_snssais_node;
    if (policy_association_update_request->allowed_snssais) {
        OpenAPI_list_for_each(policy_association_update_request->allowed_snssais, allowed_snssais_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(allowed_snssais_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [allowed_snssais]");
                goto end;
            }
            cJSON_AddItemToArray(allowed_snssaisList, itemLocal);
        }
    }
    }

    if (policy_association_update_request->mapping_snssais) {
    cJSON *mapping_snssaisList = cJSON_AddArrayToObject(item, "mappingSnssais");
    if (mapping_snssaisList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [mapping_snssais]");
        goto end;
    }

    OpenAPI_lnode_t *mapping_snssais_node;
    if (policy_association_update_request->mapping_snssais) {
        OpenAPI_list_for_each(policy_association_update_request->mapping_snssais, mapping_snssais_node) {
            cJSON *itemLocal = OpenAPI_mapping_of_snssai_convertToJSON(mapping_snssais_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [mapping_snssais]");
                goto end;
            }
            cJSON_AddItemToArray(mapping_snssaisList, itemLocal);
        }
    }
    }

    if (policy_association_update_request->access_types) {
    cJSON *access_types = cJSON_AddArrayToObject(item, "accessTypes");
    if (access_types == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [access_types]");
        goto end;
    }
    OpenAPI_lnode_t *access_types_node;
    OpenAPI_list_for_each(policy_association_update_request->access_types, access_types_node) {
        if (cJSON_AddStringToObject(access_types, "", OpenAPI_access_type_ToString((intptr_t)access_types_node->data)) == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [access_types]");
            goto end;
        }
    }
    }

    if (policy_association_update_request->rat_types) {
    cJSON *rat_types = cJSON_AddArrayToObject(item, "ratTypes");
    if (rat_types == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [rat_types]");
        goto end;
    }
    OpenAPI_lnode_t *rat_types_node;
    OpenAPI_list_for_each(policy_association_update_request->rat_types, rat_types_node) {
        if (cJSON_AddStringToObject(rat_types, "", OpenAPI_rat_type_ToString((intptr_t)rat_types_node->data)) == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [rat_types]");
            goto end;
        }
    }
    }

    if (policy_association_update_request->n3g_allowed_snssais) {
    cJSON *n3g_allowed_snssaisList = cJSON_AddArrayToObject(item, "n3gAllowedSnssais");
    if (n3g_allowed_snssaisList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [n3g_allowed_snssais]");
        goto end;
    }

    OpenAPI_lnode_t *n3g_allowed_snssais_node;
    if (policy_association_update_request->n3g_allowed_snssais) {
        OpenAPI_list_for_each(policy_association_update_request->n3g_allowed_snssais, n3g_allowed_snssais_node) {
            cJSON *itemLocal = OpenAPI_snssai_convertToJSON(n3g_allowed_snssais_node->data);
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [n3g_allowed_snssais]");
                goto end;
            }
            cJSON_AddItemToArray(n3g_allowed_snssaisList, itemLocal);
        }
    }
    }

    if (policy_association_update_request->trace_req) {
    cJSON *trace_req_local_JSON = OpenAPI_trace_data_convertToJSON(policy_association_update_request->trace_req);
    if (trace_req_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [trace_req]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceReq", trace_req_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [trace_req]");
        goto end;
    }
    }

    if (policy_association_update_request->guami) {
    cJSON *guami_local_JSON = OpenAPI_guami_convertToJSON(policy_association_update_request->guami);
    if (guami_local_JSON == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [guami]");
        goto end;
    }
    cJSON_AddItemToObject(item, "guami", guami_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [guami]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_policy_association_update_request_t *OpenAPI_policy_association_update_request_parseFromJSON(cJSON *policy_association_update_requestJSON)
{
    OpenAPI_policy_association_update_request_t *policy_association_update_request_local_var = NULL;
    cJSON *notification_uri = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "notificationUri");

    if (notification_uri) {
    if (!cJSON_IsString(notification_uri)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [notification_uri]");
        goto end;
    }
    }

    cJSON *alt_notif_ipv4_addrs = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "altNotifIpv4Addrs");

    OpenAPI_list_t *alt_notif_ipv4_addrsList;
    if (alt_notif_ipv4_addrs) {
    cJSON *alt_notif_ipv4_addrs_local;
    if (!cJSON_IsArray(alt_notif_ipv4_addrs)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_ipv4_addrs]");
        goto end;
    }
    alt_notif_ipv4_addrsList = OpenAPI_list_create();

    cJSON_ArrayForEach(alt_notif_ipv4_addrs_local, alt_notif_ipv4_addrs) {
    if (!cJSON_IsString(alt_notif_ipv4_addrs_local)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_ipv4_addrs]");
        goto end;
    }
    OpenAPI_list_add(alt_notif_ipv4_addrsList , ogs_strdup(alt_notif_ipv4_addrs_local->valuestring));
    }
    }

    cJSON *alt_notif_ipv6_addrs = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "altNotifIpv6Addrs");

    OpenAPI_list_t *alt_notif_ipv6_addrsList;
    if (alt_notif_ipv6_addrs) {
    cJSON *alt_notif_ipv6_addrs_local;
    if (!cJSON_IsArray(alt_notif_ipv6_addrs)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_ipv6_addrs]");
        goto end;
    }
    alt_notif_ipv6_addrsList = OpenAPI_list_create();

    cJSON_ArrayForEach(alt_notif_ipv6_addrs_local, alt_notif_ipv6_addrs) {
    if (!cJSON_IsString(alt_notif_ipv6_addrs_local)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_ipv6_addrs]");
        goto end;
    }
    OpenAPI_list_add(alt_notif_ipv6_addrsList , ogs_strdup(alt_notif_ipv6_addrs_local->valuestring));
    }
    }

    cJSON *alt_notif_fqdns = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "altNotifFqdns");

    OpenAPI_list_t *alt_notif_fqdnsList;
    if (alt_notif_fqdns) {
    cJSON *alt_notif_fqdns_local;
    if (!cJSON_IsArray(alt_notif_fqdns)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_fqdns]");
        goto end;
    }
    alt_notif_fqdnsList = OpenAPI_list_create();

    cJSON_ArrayForEach(alt_notif_fqdns_local, alt_notif_fqdns) {
    if (!cJSON_IsString(alt_notif_fqdns_local)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_fqdns]");
        goto end;
    }
    OpenAPI_list_add(alt_notif_fqdnsList , ogs_strdup(alt_notif_fqdns_local->valuestring));
    }
    }

    cJSON *triggers = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "triggers");

    OpenAPI_list_t *triggersList;
    if (triggers) {
    cJSON *triggers_local_nonprimitive;
    if (!cJSON_IsArray(triggers)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [triggers]");
        goto end;
    }

    triggersList = OpenAPI_list_create();

    cJSON_ArrayForEach(triggers_local_nonprimitive, triggers ) {
        if (!cJSON_IsString(triggers_local_nonprimitive)){
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [triggers]");
            goto end;
        }

        OpenAPI_list_add(triggersList, (void *)OpenAPI_request_trigger_FromString(triggers_local_nonprimitive->valuestring));
    }
    }

    cJSON *serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "servAreaRes");

    OpenAPI_service_area_restriction_t *serv_area_res_local_nonprim = NULL;
    if (serv_area_res) {
    serv_area_res_local_nonprim = OpenAPI_service_area_restriction_parseFromJSON(serv_area_res);
    }

    cJSON *wl_serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "wlServAreaRes");

    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res_local_nonprim = NULL;
    if (wl_serv_area_res) {
    wl_serv_area_res_local_nonprim = OpenAPI_wireline_service_area_restriction_parseFromJSON(wl_serv_area_res);
    }

    cJSON *rfsp = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "rfsp");

    if (rfsp) {
    if (!cJSON_IsNumber(rfsp)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [rfsp]");
        goto end;
    }
    }

    cJSON *smf_sel_info = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "smfSelInfo");

    OpenAPI_smf_selection_data_t *smf_sel_info_local_nonprim = NULL;
    if (smf_sel_info) {
    smf_sel_info_local_nonprim = OpenAPI_smf_selection_data_parseFromJSON(smf_sel_info);
    }

    cJSON *ue_ambr = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "ueAmbr");

    OpenAPI_ambr_t *ue_ambr_local_nonprim = NULL;
    if (ue_ambr) {
    ue_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(ue_ambr);
    }

    cJSON *pra_statuses = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "praStatuses");

    OpenAPI_list_t *pra_statusesList;
    if (pra_statuses) {
    cJSON *pra_statuses_local_map;
    if (!cJSON_IsObject(pra_statuses)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [pra_statuses]");
        goto end;
    }
    pra_statusesList = OpenAPI_list_create();
    OpenAPI_map_t *localMapKeyPair = NULL;
    cJSON_ArrayForEach(pra_statuses_local_map, pra_statuses) {
        cJSON *localMapObject = pra_statuses_local_map;
        if (cJSON_IsObject(pra_statuses_local_map)) {
            localMapKeyPair = OpenAPI_map_create(
                ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
        } else if (cJSON_IsNull(pra_statuses_local_map)) {
            localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
        } else {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [pra_statuses]");
            goto end;
        }
        OpenAPI_list_add(pra_statusesList , localMapKeyPair);
    }
    }

    cJSON *user_loc = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "userLoc");

    OpenAPI_user_location_t *user_loc_local_nonprim = NULL;
    if (user_loc) {
    user_loc_local_nonprim = OpenAPI_user_location_parseFromJSON(user_loc);
    }

    cJSON *allowed_snssais = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "allowedSnssais");

    OpenAPI_list_t *allowed_snssaisList;
    if (allowed_snssais) {
    cJSON *allowed_snssais_local_nonprimitive;
    if (!cJSON_IsArray(allowed_snssais)){
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [allowed_snssais]");
        goto end;
    }

    allowed_snssaisList = OpenAPI_list_create();

    cJSON_ArrayForEach(allowed_snssais_local_nonprimitive, allowed_snssais ) {
        if (!cJSON_IsObject(allowed_snssais_local_nonprimitive)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [allowed_snssais]");
            goto end;
        }
        OpenAPI_snssai_t *allowed_snssaisItem = OpenAPI_snssai_parseFromJSON(allowed_snssais_local_nonprimitive);

        if (!allowed_snssaisItem) {
            ogs_error("No allowed_snssaisItem");
            OpenAPI_list_free(allowed_snssaisList);
            goto end;
        }

        OpenAPI_list_add(allowed_snssaisList, allowed_snssaisItem);
    }
    }

    cJSON *mapping_snssais = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "mappingSnssais");

    OpenAPI_list_t *mapping_snssaisList;
    if (mapping_snssais) {
    cJSON *mapping_snssais_local_nonprimitive;
    if (!cJSON_IsArray(mapping_snssais)){
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [mapping_snssais]");
        goto end;
    }

    mapping_snssaisList = OpenAPI_list_create();

    cJSON_ArrayForEach(mapping_snssais_local_nonprimitive, mapping_snssais ) {
        if (!cJSON_IsObject(mapping_snssais_local_nonprimitive)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [mapping_snssais]");
            goto end;
        }
        OpenAPI_mapping_of_snssai_t *mapping_snssaisItem = OpenAPI_mapping_of_snssai_parseFromJSON(mapping_snssais_local_nonprimitive);

        if (!mapping_snssaisItem) {
            ogs_error("No mapping_snssaisItem");
            OpenAPI_list_free(mapping_snssaisList);
            goto end;
        }

        OpenAPI_list_add(mapping_snssaisList, mapping_snssaisItem);
    }
    }

    cJSON *access_types = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "accessTypes");

    OpenAPI_list_t *access_typesList;
    if (access_types) {
    cJSON *access_types_local_nonprimitive;
    if (!cJSON_IsArray(access_types)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [access_types]");
        goto end;
    }

    access_typesList = OpenAPI_list_create();

    cJSON_ArrayForEach(access_types_local_nonprimitive, access_types ) {
        if (!cJSON_IsString(access_types_local_nonprimitive)){
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [access_types]");
            goto end;
        }

        OpenAPI_list_add(access_typesList, (void *)OpenAPI_access_type_FromString(access_types_local_nonprimitive->valuestring));
    }
    }

    cJSON *rat_types = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "ratTypes");

    OpenAPI_list_t *rat_typesList;
    if (rat_types) {
    cJSON *rat_types_local_nonprimitive;
    if (!cJSON_IsArray(rat_types)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [rat_types]");
        goto end;
    }

    rat_typesList = OpenAPI_list_create();

    cJSON_ArrayForEach(rat_types_local_nonprimitive, rat_types ) {
        if (!cJSON_IsString(rat_types_local_nonprimitive)){
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [rat_types]");
            goto end;
        }

        OpenAPI_list_add(rat_typesList, (void *)OpenAPI_rat_type_FromString(rat_types_local_nonprimitive->valuestring));
    }
    }

    cJSON *n3g_allowed_snssais = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "n3gAllowedSnssais");

    OpenAPI_list_t *n3g_allowed_snssaisList;
    if (n3g_allowed_snssais) {
    cJSON *n3g_allowed_snssais_local_nonprimitive;
    if (!cJSON_IsArray(n3g_allowed_snssais)){
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [n3g_allowed_snssais]");
        goto end;
    }

    n3g_allowed_snssaisList = OpenAPI_list_create();

    cJSON_ArrayForEach(n3g_allowed_snssais_local_nonprimitive, n3g_allowed_snssais ) {
        if (!cJSON_IsObject(n3g_allowed_snssais_local_nonprimitive)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [n3g_allowed_snssais]");
            goto end;
        }
        OpenAPI_snssai_t *n3g_allowed_snssaisItem = OpenAPI_snssai_parseFromJSON(n3g_allowed_snssais_local_nonprimitive);

        if (!n3g_allowed_snssaisItem) {
            ogs_error("No n3g_allowed_snssaisItem");
            OpenAPI_list_free(n3g_allowed_snssaisList);
            goto end;
        }

        OpenAPI_list_add(n3g_allowed_snssaisList, n3g_allowed_snssaisItem);
    }
    }

    cJSON *trace_req = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "traceReq");

    OpenAPI_trace_data_t *trace_req_local_nonprim = NULL;
    if (trace_req) {
    trace_req_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_req);
    }

    cJSON *guami = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "guami");

    OpenAPI_guami_t *guami_local_nonprim = NULL;
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    }

    policy_association_update_request_local_var = OpenAPI_policy_association_update_request_create (
        notification_uri ? ogs_strdup(notification_uri->valuestring) : NULL,
        alt_notif_ipv4_addrs ? alt_notif_ipv4_addrsList : NULL,
        alt_notif_ipv6_addrs ? alt_notif_ipv6_addrsList : NULL,
        alt_notif_fqdns ? alt_notif_fqdnsList : NULL,
        triggers ? triggersList : NULL,
        serv_area_res ? serv_area_res_local_nonprim : NULL,
        wl_serv_area_res ? wl_serv_area_res_local_nonprim : NULL,
        rfsp ? true : false,
        rfsp ? rfsp->valuedouble : 0,
        smf_sel_info ? smf_sel_info_local_nonprim : NULL,
        ue_ambr ? ue_ambr_local_nonprim : NULL,
        pra_statuses ? pra_statusesList : NULL,
        user_loc ? user_loc_local_nonprim : NULL,
        allowed_snssais ? allowed_snssaisList : NULL,
        mapping_snssais ? mapping_snssaisList : NULL,
        access_types ? access_typesList : NULL,
        rat_types ? rat_typesList : NULL,
        n3g_allowed_snssais ? n3g_allowed_snssaisList : NULL,
        trace_req ? trace_req_local_nonprim : NULL,
        guami ? guami_local_nonprim : NULL
    );

    return policy_association_update_request_local_var;
end:
    return NULL;
}

OpenAPI_policy_association_update_request_t *OpenAPI_policy_association_update_request_copy(OpenAPI_policy_association_update_request_t *dst, OpenAPI_policy_association_update_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_policy_association_update_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed");
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

    OpenAPI_policy_association_update_request_free(dst);
    dst = OpenAPI_policy_association_update_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

