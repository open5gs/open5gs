
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
    bool is_smf_sel_info_null,
    OpenAPI_smf_selection_data_t *smf_sel_info,
    OpenAPI_ambr_t *ue_ambr,
    OpenAPI_list_t *ue_slice_mbrs,
    OpenAPI_list_t* pra_statuses,
    OpenAPI_user_location_t *user_loc,
    OpenAPI_list_t *allowed_snssais,
    OpenAPI_list_t *target_snssais,
    OpenAPI_list_t *mapping_snssais,
    OpenAPI_list_t *access_types,
    OpenAPI_list_t *rat_types,
    OpenAPI_list_t *n3g_allowed_snssais,
    bool is_trace_req_null,
    OpenAPI_trace_data_t *trace_req,
    OpenAPI_guami_t *guami,
    bool is_nwdaf_datas_null,
    OpenAPI_list_t *nwdaf_datas
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
    policy_association_update_request_local_var->is_smf_sel_info_null = is_smf_sel_info_null;
    policy_association_update_request_local_var->smf_sel_info = smf_sel_info;
    policy_association_update_request_local_var->ue_ambr = ue_ambr;
    policy_association_update_request_local_var->ue_slice_mbrs = ue_slice_mbrs;
    policy_association_update_request_local_var->pra_statuses = pra_statuses;
    policy_association_update_request_local_var->user_loc = user_loc;
    policy_association_update_request_local_var->allowed_snssais = allowed_snssais;
    policy_association_update_request_local_var->target_snssais = target_snssais;
    policy_association_update_request_local_var->mapping_snssais = mapping_snssais;
    policy_association_update_request_local_var->access_types = access_types;
    policy_association_update_request_local_var->rat_types = rat_types;
    policy_association_update_request_local_var->n3g_allowed_snssais = n3g_allowed_snssais;
    policy_association_update_request_local_var->is_trace_req_null = is_trace_req_null;
    policy_association_update_request_local_var->trace_req = trace_req;
    policy_association_update_request_local_var->guami = guami;
    policy_association_update_request_local_var->is_nwdaf_datas_null = is_nwdaf_datas_null;
    policy_association_update_request_local_var->nwdaf_datas = nwdaf_datas;

    return policy_association_update_request_local_var;
}

void OpenAPI_policy_association_update_request_free(OpenAPI_policy_association_update_request_t *policy_association_update_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == policy_association_update_request) {
        return;
    }
    if (policy_association_update_request->notification_uri) {
        ogs_free(policy_association_update_request->notification_uri);
        policy_association_update_request->notification_uri = NULL;
    }
    if (policy_association_update_request->alt_notif_ipv4_addrs) {
        OpenAPI_list_for_each(policy_association_update_request->alt_notif_ipv4_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(policy_association_update_request->alt_notif_ipv4_addrs);
        policy_association_update_request->alt_notif_ipv4_addrs = NULL;
    }
    if (policy_association_update_request->alt_notif_ipv6_addrs) {
        OpenAPI_list_for_each(policy_association_update_request->alt_notif_ipv6_addrs, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(policy_association_update_request->alt_notif_ipv6_addrs);
        policy_association_update_request->alt_notif_ipv6_addrs = NULL;
    }
    if (policy_association_update_request->alt_notif_fqdns) {
        OpenAPI_list_for_each(policy_association_update_request->alt_notif_fqdns, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(policy_association_update_request->alt_notif_fqdns);
        policy_association_update_request->alt_notif_fqdns = NULL;
    }
    if (policy_association_update_request->triggers) {
        OpenAPI_list_free(policy_association_update_request->triggers);
        policy_association_update_request->triggers = NULL;
    }
    if (policy_association_update_request->serv_area_res) {
        OpenAPI_service_area_restriction_free(policy_association_update_request->serv_area_res);
        policy_association_update_request->serv_area_res = NULL;
    }
    if (policy_association_update_request->wl_serv_area_res) {
        OpenAPI_wireline_service_area_restriction_free(policy_association_update_request->wl_serv_area_res);
        policy_association_update_request->wl_serv_area_res = NULL;
    }
    if (policy_association_update_request->smf_sel_info) {
        OpenAPI_smf_selection_data_free(policy_association_update_request->smf_sel_info);
        policy_association_update_request->smf_sel_info = NULL;
    }
    if (policy_association_update_request->ue_ambr) {
        OpenAPI_ambr_free(policy_association_update_request->ue_ambr);
        policy_association_update_request->ue_ambr = NULL;
    }
    if (policy_association_update_request->ue_slice_mbrs) {
        OpenAPI_list_for_each(policy_association_update_request->ue_slice_mbrs, node) {
            OpenAPI_ue_slice_mbr_free(node->data);
        }
        OpenAPI_list_free(policy_association_update_request->ue_slice_mbrs);
        policy_association_update_request->ue_slice_mbrs = NULL;
    }
    if (policy_association_update_request->pra_statuses) {
        OpenAPI_list_for_each(policy_association_update_request->pra_statuses, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(policy_association_update_request->pra_statuses);
        policy_association_update_request->pra_statuses = NULL;
    }
    if (policy_association_update_request->user_loc) {
        OpenAPI_user_location_free(policy_association_update_request->user_loc);
        policy_association_update_request->user_loc = NULL;
    }
    if (policy_association_update_request->allowed_snssais) {
        OpenAPI_list_for_each(policy_association_update_request->allowed_snssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(policy_association_update_request->allowed_snssais);
        policy_association_update_request->allowed_snssais = NULL;
    }
    if (policy_association_update_request->target_snssais) {
        OpenAPI_list_for_each(policy_association_update_request->target_snssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(policy_association_update_request->target_snssais);
        policy_association_update_request->target_snssais = NULL;
    }
    if (policy_association_update_request->mapping_snssais) {
        OpenAPI_list_for_each(policy_association_update_request->mapping_snssais, node) {
            OpenAPI_mapping_of_snssai_free(node->data);
        }
        OpenAPI_list_free(policy_association_update_request->mapping_snssais);
        policy_association_update_request->mapping_snssais = NULL;
    }
    if (policy_association_update_request->access_types) {
        OpenAPI_list_free(policy_association_update_request->access_types);
        policy_association_update_request->access_types = NULL;
    }
    if (policy_association_update_request->rat_types) {
        OpenAPI_list_free(policy_association_update_request->rat_types);
        policy_association_update_request->rat_types = NULL;
    }
    if (policy_association_update_request->n3g_allowed_snssais) {
        OpenAPI_list_for_each(policy_association_update_request->n3g_allowed_snssais, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(policy_association_update_request->n3g_allowed_snssais);
        policy_association_update_request->n3g_allowed_snssais = NULL;
    }
    if (policy_association_update_request->trace_req) {
        OpenAPI_trace_data_free(policy_association_update_request->trace_req);
        policy_association_update_request->trace_req = NULL;
    }
    if (policy_association_update_request->guami) {
        OpenAPI_guami_free(policy_association_update_request->guami);
        policy_association_update_request->guami = NULL;
    }
    if (policy_association_update_request->nwdaf_datas) {
        OpenAPI_list_for_each(policy_association_update_request->nwdaf_datas, node) {
            OpenAPI_nwdaf_data_free(node->data);
        }
        OpenAPI_list_free(policy_association_update_request->nwdaf_datas);
        policy_association_update_request->nwdaf_datas = NULL;
    }
    ogs_free(policy_association_update_request);
}

cJSON *OpenAPI_policy_association_update_request_convertToJSON(OpenAPI_policy_association_update_request_t *policy_association_update_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

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
    cJSON *alt_notif_ipv4_addrsList = cJSON_AddArrayToObject(item, "altNotifIpv4Addrs");
    if (alt_notif_ipv4_addrsList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_ipv4_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association_update_request->alt_notif_ipv4_addrs, node) {
        if (cJSON_AddStringToObject(alt_notif_ipv4_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_ipv4_addrs]");
            goto end;
        }
    }
    }

    if (policy_association_update_request->alt_notif_ipv6_addrs) {
    cJSON *alt_notif_ipv6_addrsList = cJSON_AddArrayToObject(item, "altNotifIpv6Addrs");
    if (alt_notif_ipv6_addrsList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_ipv6_addrs]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association_update_request->alt_notif_ipv6_addrs, node) {
        if (cJSON_AddStringToObject(alt_notif_ipv6_addrsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_ipv6_addrs]");
            goto end;
        }
    }
    }

    if (policy_association_update_request->alt_notif_fqdns) {
    cJSON *alt_notif_fqdnsList = cJSON_AddArrayToObject(item, "altNotifFqdns");
    if (alt_notif_fqdnsList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_fqdns]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association_update_request->alt_notif_fqdns, node) {
        if (cJSON_AddStringToObject(alt_notif_fqdnsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [alt_notif_fqdns]");
            goto end;
        }
    }
    }

    if (policy_association_update_request->triggers != OpenAPI_request_trigger_NULL) {
    cJSON *triggersList = cJSON_AddArrayToObject(item, "triggers");
    if (triggersList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association_update_request->triggers, node) {
        if (cJSON_AddStringToObject(triggersList, "", OpenAPI_request_trigger_ToString((intptr_t)node->data)) == NULL) {
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
    } else if (policy_association_update_request->is_smf_sel_info_null) {
        if (cJSON_AddNullToObject(item, "smfSelInfo") == NULL) {
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

    if (policy_association_update_request->ue_slice_mbrs) {
    cJSON *ue_slice_mbrsList = cJSON_AddArrayToObject(item, "ueSliceMbrs");
    if (ue_slice_mbrsList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [ue_slice_mbrs]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association_update_request->ue_slice_mbrs, node) {
        cJSON *itemLocal = OpenAPI_ue_slice_mbr_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [ue_slice_mbrs]");
            goto end;
        }
        cJSON_AddItemToArray(ue_slice_mbrsList, itemLocal);
    }
    }

    if (policy_association_update_request->pra_statuses) {
    cJSON *pra_statuses = cJSON_AddObjectToObject(item, "praStatuses");
    if (pra_statuses == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [pra_statuses]");
        goto end;
    }
    cJSON *localMapObject = pra_statuses;
    if (policy_association_update_request->pra_statuses) {
        OpenAPI_list_for_each(policy_association_update_request->pra_statuses, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [pra_statuses]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [pra_statuses]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
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
    OpenAPI_list_for_each(policy_association_update_request->allowed_snssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [allowed_snssais]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_snssaisList, itemLocal);
    }
    }

    if (policy_association_update_request->target_snssais) {
    cJSON *target_snssaisList = cJSON_AddArrayToObject(item, "targetSnssais");
    if (target_snssaisList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [target_snssais]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association_update_request->target_snssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [target_snssais]");
            goto end;
        }
        cJSON_AddItemToArray(target_snssaisList, itemLocal);
    }
    }

    if (policy_association_update_request->mapping_snssais) {
    cJSON *mapping_snssaisList = cJSON_AddArrayToObject(item, "mappingSnssais");
    if (mapping_snssaisList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [mapping_snssais]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association_update_request->mapping_snssais, node) {
        cJSON *itemLocal = OpenAPI_mapping_of_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [mapping_snssais]");
            goto end;
        }
        cJSON_AddItemToArray(mapping_snssaisList, itemLocal);
    }
    }

    if (policy_association_update_request->access_types != OpenAPI_access_type_NULL) {
    cJSON *access_typesList = cJSON_AddArrayToObject(item, "accessTypes");
    if (access_typesList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [access_types]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association_update_request->access_types, node) {
        if (cJSON_AddStringToObject(access_typesList, "", OpenAPI_access_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [access_types]");
            goto end;
        }
    }
    }

    if (policy_association_update_request->rat_types != OpenAPI_rat_type_NULL) {
    cJSON *rat_typesList = cJSON_AddArrayToObject(item, "ratTypes");
    if (rat_typesList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [rat_types]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association_update_request->rat_types, node) {
        if (cJSON_AddStringToObject(rat_typesList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
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
    OpenAPI_list_for_each(policy_association_update_request->n3g_allowed_snssais, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [n3g_allowed_snssais]");
            goto end;
        }
        cJSON_AddItemToArray(n3g_allowed_snssaisList, itemLocal);
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
    } else if (policy_association_update_request->is_trace_req_null) {
        if (cJSON_AddNullToObject(item, "traceReq") == NULL) {
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

    if (policy_association_update_request->nwdaf_datas) {
    cJSON *nwdaf_datasList = cJSON_AddArrayToObject(item, "nwdafDatas");
    if (nwdaf_datasList == NULL) {
        ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [nwdaf_datas]");
        goto end;
    }
    OpenAPI_list_for_each(policy_association_update_request->nwdaf_datas, node) {
        cJSON *itemLocal = OpenAPI_nwdaf_data_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [nwdaf_datas]");
            goto end;
        }
        cJSON_AddItemToArray(nwdaf_datasList, itemLocal);
    }
    } else if (policy_association_update_request->is_nwdaf_datas_null) {
        if (cJSON_AddNullToObject(item, "nwdafDatas") == NULL) {
            ogs_error("OpenAPI_policy_association_update_request_convertToJSON() failed [nwdaf_datas]");
            goto end;
        }
    }

end:
    return item;
}

OpenAPI_policy_association_update_request_t *OpenAPI_policy_association_update_request_parseFromJSON(cJSON *policy_association_update_requestJSON)
{
    OpenAPI_policy_association_update_request_t *policy_association_update_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *notification_uri = NULL;
    cJSON *alt_notif_ipv4_addrs = NULL;
    OpenAPI_list_t *alt_notif_ipv4_addrsList = NULL;
    cJSON *alt_notif_ipv6_addrs = NULL;
    OpenAPI_list_t *alt_notif_ipv6_addrsList = NULL;
    cJSON *alt_notif_fqdns = NULL;
    OpenAPI_list_t *alt_notif_fqdnsList = NULL;
    cJSON *triggers = NULL;
    OpenAPI_list_t *triggersList = NULL;
    cJSON *serv_area_res = NULL;
    OpenAPI_service_area_restriction_t *serv_area_res_local_nonprim = NULL;
    cJSON *wl_serv_area_res = NULL;
    OpenAPI_wireline_service_area_restriction_t *wl_serv_area_res_local_nonprim = NULL;
    cJSON *rfsp = NULL;
    cJSON *smf_sel_info = NULL;
    OpenAPI_smf_selection_data_t *smf_sel_info_local_nonprim = NULL;
    cJSON *ue_ambr = NULL;
    OpenAPI_ambr_t *ue_ambr_local_nonprim = NULL;
    cJSON *ue_slice_mbrs = NULL;
    OpenAPI_list_t *ue_slice_mbrsList = NULL;
    cJSON *pra_statuses = NULL;
    OpenAPI_list_t *pra_statusesList = NULL;
    cJSON *user_loc = NULL;
    OpenAPI_user_location_t *user_loc_local_nonprim = NULL;
    cJSON *allowed_snssais = NULL;
    OpenAPI_list_t *allowed_snssaisList = NULL;
    cJSON *target_snssais = NULL;
    OpenAPI_list_t *target_snssaisList = NULL;
    cJSON *mapping_snssais = NULL;
    OpenAPI_list_t *mapping_snssaisList = NULL;
    cJSON *access_types = NULL;
    OpenAPI_list_t *access_typesList = NULL;
    cJSON *rat_types = NULL;
    OpenAPI_list_t *rat_typesList = NULL;
    cJSON *n3g_allowed_snssais = NULL;
    OpenAPI_list_t *n3g_allowed_snssaisList = NULL;
    cJSON *trace_req = NULL;
    OpenAPI_trace_data_t *trace_req_local_nonprim = NULL;
    cJSON *guami = NULL;
    OpenAPI_guami_t *guami_local_nonprim = NULL;
    cJSON *nwdaf_datas = NULL;
    OpenAPI_list_t *nwdaf_datasList = NULL;
    notification_uri = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "notificationUri");
    if (notification_uri) {
    if (!cJSON_IsString(notification_uri) && !cJSON_IsNull(notification_uri)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [notification_uri]");
        goto end;
    }
    }

    alt_notif_ipv4_addrs = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "altNotifIpv4Addrs");
    if (alt_notif_ipv4_addrs) {
        cJSON *alt_notif_ipv4_addrs_local = NULL;
        if (!cJSON_IsArray(alt_notif_ipv4_addrs)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_ipv4_addrs]");
            goto end;
        }

        alt_notif_ipv4_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(alt_notif_ipv4_addrs_local, alt_notif_ipv4_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(alt_notif_ipv4_addrs_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_ipv4_addrs]");
                goto end;
            }
            OpenAPI_list_add(alt_notif_ipv4_addrsList, ogs_strdup(alt_notif_ipv4_addrs_local->valuestring));
        }
    }

    alt_notif_ipv6_addrs = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "altNotifIpv6Addrs");
    if (alt_notif_ipv6_addrs) {
        cJSON *alt_notif_ipv6_addrs_local = NULL;
        if (!cJSON_IsArray(alt_notif_ipv6_addrs)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_ipv6_addrs]");
            goto end;
        }

        alt_notif_ipv6_addrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(alt_notif_ipv6_addrs_local, alt_notif_ipv6_addrs) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(alt_notif_ipv6_addrs_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_ipv6_addrs]");
                goto end;
            }
            OpenAPI_list_add(alt_notif_ipv6_addrsList, ogs_strdup(alt_notif_ipv6_addrs_local->valuestring));
        }
    }

    alt_notif_fqdns = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "altNotifFqdns");
    if (alt_notif_fqdns) {
        cJSON *alt_notif_fqdns_local = NULL;
        if (!cJSON_IsArray(alt_notif_fqdns)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_fqdns]");
            goto end;
        }

        alt_notif_fqdnsList = OpenAPI_list_create();

        cJSON_ArrayForEach(alt_notif_fqdns_local, alt_notif_fqdns) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(alt_notif_fqdns_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [alt_notif_fqdns]");
                goto end;
            }
            OpenAPI_list_add(alt_notif_fqdnsList, ogs_strdup(alt_notif_fqdns_local->valuestring));
        }
    }

    triggers = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "triggers");
    if (triggers) {
        cJSON *triggers_local = NULL;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local, triggers) {
            OpenAPI_request_trigger_e localEnum = OpenAPI_request_trigger_NULL;
            if (!cJSON_IsString(triggers_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [triggers]");
                goto end;
            }
            localEnum = OpenAPI_request_trigger_FromString(triggers_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"triggers\" is not supported. Ignoring it ...",
                         triggers_local->valuestring);
            } else {
                OpenAPI_list_add(triggersList, (void *)localEnum);
            }
        }
        if (triggersList->count == 0) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed: Expected triggersList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "servAreaRes");
    if (serv_area_res) {
    serv_area_res_local_nonprim = OpenAPI_service_area_restriction_parseFromJSON(serv_area_res);
    if (!serv_area_res_local_nonprim) {
        ogs_error("OpenAPI_service_area_restriction_parseFromJSON failed [serv_area_res]");
        goto end;
    }
    }

    wl_serv_area_res = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "wlServAreaRes");
    if (wl_serv_area_res) {
    wl_serv_area_res_local_nonprim = OpenAPI_wireline_service_area_restriction_parseFromJSON(wl_serv_area_res);
    if (!wl_serv_area_res_local_nonprim) {
        ogs_error("OpenAPI_wireline_service_area_restriction_parseFromJSON failed [wl_serv_area_res]");
        goto end;
    }
    }

    rfsp = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "rfsp");
    if (rfsp) {
    if (!cJSON_IsNumber(rfsp)) {
        ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [rfsp]");
        goto end;
    }
    }

    smf_sel_info = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "smfSelInfo");
    if (smf_sel_info) {
    if (!cJSON_IsNull(smf_sel_info)) {
    smf_sel_info_local_nonprim = OpenAPI_smf_selection_data_parseFromJSON(smf_sel_info);
    if (!smf_sel_info_local_nonprim) {
        ogs_error("OpenAPI_smf_selection_data_parseFromJSON failed [smf_sel_info]");
        goto end;
    }
    }
    }

    ue_ambr = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "ueAmbr");
    if (ue_ambr) {
    ue_ambr_local_nonprim = OpenAPI_ambr_parseFromJSON(ue_ambr);
    if (!ue_ambr_local_nonprim) {
        ogs_error("OpenAPI_ambr_parseFromJSON failed [ue_ambr]");
        goto end;
    }
    }

    ue_slice_mbrs = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "ueSliceMbrs");
    if (ue_slice_mbrs) {
        cJSON *ue_slice_mbrs_local = NULL;
        if (!cJSON_IsArray(ue_slice_mbrs)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [ue_slice_mbrs]");
            goto end;
        }

        ue_slice_mbrsList = OpenAPI_list_create();

        cJSON_ArrayForEach(ue_slice_mbrs_local, ue_slice_mbrs) {
            if (!cJSON_IsObject(ue_slice_mbrs_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [ue_slice_mbrs]");
                goto end;
            }
            OpenAPI_ue_slice_mbr_t *ue_slice_mbrsItem = OpenAPI_ue_slice_mbr_parseFromJSON(ue_slice_mbrs_local);
            if (!ue_slice_mbrsItem) {
                ogs_error("No ue_slice_mbrsItem");
                goto end;
            }
            OpenAPI_list_add(ue_slice_mbrsList, ue_slice_mbrsItem);
        }
    }

    pra_statuses = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "praStatuses");
    if (pra_statuses) {
        cJSON *pra_statuses_local_map = NULL;
        if (!cJSON_IsObject(pra_statuses) && !cJSON_IsNull(pra_statuses)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [pra_statuses]");
            goto end;
        }
        if (cJSON_IsObject(pra_statuses)) {
            pra_statusesList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(pra_statuses_local_map, pra_statuses) {
                cJSON *localMapObject = pra_statuses_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [inner]");
                    goto end;
                }
                OpenAPI_list_add(pra_statusesList, localMapKeyPair);
            }
        }
    }

    user_loc = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "userLoc");
    if (user_loc) {
    user_loc_local_nonprim = OpenAPI_user_location_parseFromJSON(user_loc);
    if (!user_loc_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [user_loc]");
        goto end;
    }
    }

    allowed_snssais = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "allowedSnssais");
    if (allowed_snssais) {
        cJSON *allowed_snssais_local = NULL;
        if (!cJSON_IsArray(allowed_snssais)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [allowed_snssais]");
            goto end;
        }

        allowed_snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_snssais_local, allowed_snssais) {
            if (!cJSON_IsObject(allowed_snssais_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [allowed_snssais]");
                goto end;
            }
            OpenAPI_snssai_t *allowed_snssaisItem = OpenAPI_snssai_parseFromJSON(allowed_snssais_local);
            if (!allowed_snssaisItem) {
                ogs_error("No allowed_snssaisItem");
                goto end;
            }
            OpenAPI_list_add(allowed_snssaisList, allowed_snssaisItem);
        }
    }

    target_snssais = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "targetSnssais");
    if (target_snssais) {
        cJSON *target_snssais_local = NULL;
        if (!cJSON_IsArray(target_snssais)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [target_snssais]");
            goto end;
        }

        target_snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(target_snssais_local, target_snssais) {
            if (!cJSON_IsObject(target_snssais_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [target_snssais]");
                goto end;
            }
            OpenAPI_snssai_t *target_snssaisItem = OpenAPI_snssai_parseFromJSON(target_snssais_local);
            if (!target_snssaisItem) {
                ogs_error("No target_snssaisItem");
                goto end;
            }
            OpenAPI_list_add(target_snssaisList, target_snssaisItem);
        }
    }

    mapping_snssais = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "mappingSnssais");
    if (mapping_snssais) {
        cJSON *mapping_snssais_local = NULL;
        if (!cJSON_IsArray(mapping_snssais)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [mapping_snssais]");
            goto end;
        }

        mapping_snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(mapping_snssais_local, mapping_snssais) {
            if (!cJSON_IsObject(mapping_snssais_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [mapping_snssais]");
                goto end;
            }
            OpenAPI_mapping_of_snssai_t *mapping_snssaisItem = OpenAPI_mapping_of_snssai_parseFromJSON(mapping_snssais_local);
            if (!mapping_snssaisItem) {
                ogs_error("No mapping_snssaisItem");
                goto end;
            }
            OpenAPI_list_add(mapping_snssaisList, mapping_snssaisItem);
        }
    }

    access_types = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "accessTypes");
    if (access_types) {
        cJSON *access_types_local = NULL;
        if (!cJSON_IsArray(access_types)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [access_types]");
            goto end;
        }

        access_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(access_types_local, access_types) {
            OpenAPI_access_type_e localEnum = OpenAPI_access_type_NULL;
            if (!cJSON_IsString(access_types_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [access_types]");
                goto end;
            }
            localEnum = OpenAPI_access_type_FromString(access_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"access_types\" is not supported. Ignoring it ...",
                         access_types_local->valuestring);
            } else {
                OpenAPI_list_add(access_typesList, (void *)localEnum);
            }
        }
        if (access_typesList->count == 0) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed: Expected access_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    rat_types = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "ratTypes");
    if (rat_types) {
        cJSON *rat_types_local = NULL;
        if (!cJSON_IsArray(rat_types)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [rat_types]");
            goto end;
        }

        rat_typesList = OpenAPI_list_create();

        cJSON_ArrayForEach(rat_types_local, rat_types) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(rat_types_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [rat_types]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(rat_types_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"rat_types\" is not supported. Ignoring it ...",
                         rat_types_local->valuestring);
            } else {
                OpenAPI_list_add(rat_typesList, (void *)localEnum);
            }
        }
        if (rat_typesList->count == 0) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed: Expected rat_typesList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    n3g_allowed_snssais = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "n3gAllowedSnssais");
    if (n3g_allowed_snssais) {
        cJSON *n3g_allowed_snssais_local = NULL;
        if (!cJSON_IsArray(n3g_allowed_snssais)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [n3g_allowed_snssais]");
            goto end;
        }

        n3g_allowed_snssaisList = OpenAPI_list_create();

        cJSON_ArrayForEach(n3g_allowed_snssais_local, n3g_allowed_snssais) {
            if (!cJSON_IsObject(n3g_allowed_snssais_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [n3g_allowed_snssais]");
                goto end;
            }
            OpenAPI_snssai_t *n3g_allowed_snssaisItem = OpenAPI_snssai_parseFromJSON(n3g_allowed_snssais_local);
            if (!n3g_allowed_snssaisItem) {
                ogs_error("No n3g_allowed_snssaisItem");
                goto end;
            }
            OpenAPI_list_add(n3g_allowed_snssaisList, n3g_allowed_snssaisItem);
        }
    }

    trace_req = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "traceReq");
    if (trace_req) {
    if (!cJSON_IsNull(trace_req)) {
    trace_req_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_req);
    if (!trace_req_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_req]");
        goto end;
    }
    }
    }

    guami = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "guami");
    if (guami) {
    guami_local_nonprim = OpenAPI_guami_parseFromJSON(guami);
    if (!guami_local_nonprim) {
        ogs_error("OpenAPI_guami_parseFromJSON failed [guami]");
        goto end;
    }
    }

    nwdaf_datas = cJSON_GetObjectItemCaseSensitive(policy_association_update_requestJSON, "nwdafDatas");
    if (nwdaf_datas) {
    if (!cJSON_IsNull(nwdaf_datas)) {
        cJSON *nwdaf_datas_local = NULL;
        if (!cJSON_IsArray(nwdaf_datas)) {
            ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [nwdaf_datas]");
            goto end;
        }

        nwdaf_datasList = OpenAPI_list_create();

        cJSON_ArrayForEach(nwdaf_datas_local, nwdaf_datas) {
            if (!cJSON_IsObject(nwdaf_datas_local)) {
                ogs_error("OpenAPI_policy_association_update_request_parseFromJSON() failed [nwdaf_datas]");
                goto end;
            }
            OpenAPI_nwdaf_data_t *nwdaf_datasItem = OpenAPI_nwdaf_data_parseFromJSON(nwdaf_datas_local);
            if (!nwdaf_datasItem) {
                ogs_error("No nwdaf_datasItem");
                goto end;
            }
            OpenAPI_list_add(nwdaf_datasList, nwdaf_datasItem);
        }
    }
    }

    policy_association_update_request_local_var = OpenAPI_policy_association_update_request_create (
        notification_uri && !cJSON_IsNull(notification_uri) ? ogs_strdup(notification_uri->valuestring) : NULL,
        alt_notif_ipv4_addrs ? alt_notif_ipv4_addrsList : NULL,
        alt_notif_ipv6_addrs ? alt_notif_ipv6_addrsList : NULL,
        alt_notif_fqdns ? alt_notif_fqdnsList : NULL,
        triggers ? triggersList : NULL,
        serv_area_res ? serv_area_res_local_nonprim : NULL,
        wl_serv_area_res ? wl_serv_area_res_local_nonprim : NULL,
        rfsp ? true : false,
        rfsp ? rfsp->valuedouble : 0,
        smf_sel_info && cJSON_IsNull(smf_sel_info) ? true : false,
        smf_sel_info ? smf_sel_info_local_nonprim : NULL,
        ue_ambr ? ue_ambr_local_nonprim : NULL,
        ue_slice_mbrs ? ue_slice_mbrsList : NULL,
        pra_statuses ? pra_statusesList : NULL,
        user_loc ? user_loc_local_nonprim : NULL,
        allowed_snssais ? allowed_snssaisList : NULL,
        target_snssais ? target_snssaisList : NULL,
        mapping_snssais ? mapping_snssaisList : NULL,
        access_types ? access_typesList : NULL,
        rat_types ? rat_typesList : NULL,
        n3g_allowed_snssais ? n3g_allowed_snssaisList : NULL,
        trace_req && cJSON_IsNull(trace_req) ? true : false,
        trace_req ? trace_req_local_nonprim : NULL,
        guami ? guami_local_nonprim : NULL,
        nwdaf_datas && cJSON_IsNull(nwdaf_datas) ? true : false,
        nwdaf_datas ? nwdaf_datasList : NULL
    );

    return policy_association_update_request_local_var;
end:
    if (alt_notif_ipv4_addrsList) {
        OpenAPI_list_for_each(alt_notif_ipv4_addrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(alt_notif_ipv4_addrsList);
        alt_notif_ipv4_addrsList = NULL;
    }
    if (alt_notif_ipv6_addrsList) {
        OpenAPI_list_for_each(alt_notif_ipv6_addrsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(alt_notif_ipv6_addrsList);
        alt_notif_ipv6_addrsList = NULL;
    }
    if (alt_notif_fqdnsList) {
        OpenAPI_list_for_each(alt_notif_fqdnsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(alt_notif_fqdnsList);
        alt_notif_fqdnsList = NULL;
    }
    if (triggersList) {
        OpenAPI_list_free(triggersList);
        triggersList = NULL;
    }
    if (serv_area_res_local_nonprim) {
        OpenAPI_service_area_restriction_free(serv_area_res_local_nonprim);
        serv_area_res_local_nonprim = NULL;
    }
    if (wl_serv_area_res_local_nonprim) {
        OpenAPI_wireline_service_area_restriction_free(wl_serv_area_res_local_nonprim);
        wl_serv_area_res_local_nonprim = NULL;
    }
    if (smf_sel_info_local_nonprim) {
        OpenAPI_smf_selection_data_free(smf_sel_info_local_nonprim);
        smf_sel_info_local_nonprim = NULL;
    }
    if (ue_ambr_local_nonprim) {
        OpenAPI_ambr_free(ue_ambr_local_nonprim);
        ue_ambr_local_nonprim = NULL;
    }
    if (ue_slice_mbrsList) {
        OpenAPI_list_for_each(ue_slice_mbrsList, node) {
            OpenAPI_ue_slice_mbr_free(node->data);
        }
        OpenAPI_list_free(ue_slice_mbrsList);
        ue_slice_mbrsList = NULL;
    }
    if (pra_statusesList) {
        OpenAPI_list_for_each(pra_statusesList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*) node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pra_statusesList);
        pra_statusesList = NULL;
    }
    if (user_loc_local_nonprim) {
        OpenAPI_user_location_free(user_loc_local_nonprim);
        user_loc_local_nonprim = NULL;
    }
    if (allowed_snssaisList) {
        OpenAPI_list_for_each(allowed_snssaisList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(allowed_snssaisList);
        allowed_snssaisList = NULL;
    }
    if (target_snssaisList) {
        OpenAPI_list_for_each(target_snssaisList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(target_snssaisList);
        target_snssaisList = NULL;
    }
    if (mapping_snssaisList) {
        OpenAPI_list_for_each(mapping_snssaisList, node) {
            OpenAPI_mapping_of_snssai_free(node->data);
        }
        OpenAPI_list_free(mapping_snssaisList);
        mapping_snssaisList = NULL;
    }
    if (access_typesList) {
        OpenAPI_list_free(access_typesList);
        access_typesList = NULL;
    }
    if (rat_typesList) {
        OpenAPI_list_free(rat_typesList);
        rat_typesList = NULL;
    }
    if (n3g_allowed_snssaisList) {
        OpenAPI_list_for_each(n3g_allowed_snssaisList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(n3g_allowed_snssaisList);
        n3g_allowed_snssaisList = NULL;
    }
    if (trace_req_local_nonprim) {
        OpenAPI_trace_data_free(trace_req_local_nonprim);
        trace_req_local_nonprim = NULL;
    }
    if (guami_local_nonprim) {
        OpenAPI_guami_free(guami_local_nonprim);
        guami_local_nonprim = NULL;
    }
    if (nwdaf_datasList) {
        OpenAPI_list_for_each(nwdaf_datasList, node) {
            OpenAPI_nwdaf_data_free(node->data);
        }
        OpenAPI_list_free(nwdaf_datasList);
        nwdaf_datasList = NULL;
    }
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

