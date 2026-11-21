
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "n2_connection_charging_information.h"

OpenAPI_n2_connection_charging_information_t *OpenAPI_n2_connection_charging_information_create(
    int n2_connection_message_type,
    OpenAPI_user_information_t *user_information,
    OpenAPI_charging_user_location_t *user_locationinfo,
    OpenAPI_ps_cell_information_t *p_s_cell_information,
    char *uetime_zone,
    OpenAPI_rat_type_e r_at_type,
    bool is_amf_ue_ngap_id,
    int amf_ue_ngap_id,
    bool is_ran_ue_ngap_id,
    int ran_ue_ngap_id,
    OpenAPI_global_ran_node_id_t *ran_node_id,
    OpenAPI_list_t *restricted_rat_list,
    OpenAPI_list_t *forbidden_area_list,
    OpenAPI_list_t *service_area_restriction,
    OpenAPI_list_t *restricted_cn_list,
    OpenAPI_list_t *allowed_nssai,
    OpenAPI_list_t *n_ssai_map_list,
    char *rrc_est_cause,
    bool is_satellite_access_indicator,
    int satellite_access_indicator
)
{
    OpenAPI_n2_connection_charging_information_t *n2_connection_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_n2_connection_charging_information_t));
    ogs_assert(n2_connection_charging_information_local_var);

    n2_connection_charging_information_local_var->n2_connection_message_type = n2_connection_message_type;
    n2_connection_charging_information_local_var->user_information = user_information;
    n2_connection_charging_information_local_var->user_locationinfo = user_locationinfo;
    n2_connection_charging_information_local_var->p_s_cell_information = p_s_cell_information;
    n2_connection_charging_information_local_var->uetime_zone = uetime_zone;
    n2_connection_charging_information_local_var->r_at_type = r_at_type;
    n2_connection_charging_information_local_var->is_amf_ue_ngap_id = is_amf_ue_ngap_id;
    n2_connection_charging_information_local_var->amf_ue_ngap_id = amf_ue_ngap_id;
    n2_connection_charging_information_local_var->is_ran_ue_ngap_id = is_ran_ue_ngap_id;
    n2_connection_charging_information_local_var->ran_ue_ngap_id = ran_ue_ngap_id;
    n2_connection_charging_information_local_var->ran_node_id = ran_node_id;
    n2_connection_charging_information_local_var->restricted_rat_list = restricted_rat_list;
    n2_connection_charging_information_local_var->forbidden_area_list = forbidden_area_list;
    n2_connection_charging_information_local_var->service_area_restriction = service_area_restriction;
    n2_connection_charging_information_local_var->restricted_cn_list = restricted_cn_list;
    n2_connection_charging_information_local_var->allowed_nssai = allowed_nssai;
    n2_connection_charging_information_local_var->n_ssai_map_list = n_ssai_map_list;
    n2_connection_charging_information_local_var->rrc_est_cause = rrc_est_cause;
    n2_connection_charging_information_local_var->is_satellite_access_indicator = is_satellite_access_indicator;
    n2_connection_charging_information_local_var->satellite_access_indicator = satellite_access_indicator;

    return n2_connection_charging_information_local_var;
}

void OpenAPI_n2_connection_charging_information_free(OpenAPI_n2_connection_charging_information_t *n2_connection_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == n2_connection_charging_information) {
        return;
    }
    if (n2_connection_charging_information->user_information) {
        OpenAPI_user_information_free(n2_connection_charging_information->user_information);
        n2_connection_charging_information->user_information = NULL;
    }
    if (n2_connection_charging_information->user_locationinfo) {
        OpenAPI_charging_user_location_free(n2_connection_charging_information->user_locationinfo);
        n2_connection_charging_information->user_locationinfo = NULL;
    }
    if (n2_connection_charging_information->p_s_cell_information) {
        OpenAPI_ps_cell_information_free(n2_connection_charging_information->p_s_cell_information);
        n2_connection_charging_information->p_s_cell_information = NULL;
    }
    if (n2_connection_charging_information->uetime_zone) {
        ogs_free(n2_connection_charging_information->uetime_zone);
        n2_connection_charging_information->uetime_zone = NULL;
    }
    if (n2_connection_charging_information->ran_node_id) {
        OpenAPI_global_ran_node_id_free(n2_connection_charging_information->ran_node_id);
        n2_connection_charging_information->ran_node_id = NULL;
    }
    if (n2_connection_charging_information->restricted_rat_list) {
        OpenAPI_list_free(n2_connection_charging_information->restricted_rat_list);
        n2_connection_charging_information->restricted_rat_list = NULL;
    }
    if (n2_connection_charging_information->forbidden_area_list) {
        OpenAPI_list_for_each(n2_connection_charging_information->forbidden_area_list, node) {
            OpenAPI_area_free(node->data);
        }
        OpenAPI_list_free(n2_connection_charging_information->forbidden_area_list);
        n2_connection_charging_information->forbidden_area_list = NULL;
    }
    if (n2_connection_charging_information->service_area_restriction) {
        OpenAPI_list_for_each(n2_connection_charging_information->service_area_restriction, node) {
            OpenAPI_service_area_restriction_free(node->data);
        }
        OpenAPI_list_free(n2_connection_charging_information->service_area_restriction);
        n2_connection_charging_information->service_area_restriction = NULL;
    }
    if (n2_connection_charging_information->restricted_cn_list) {
        OpenAPI_list_free(n2_connection_charging_information->restricted_cn_list);
        n2_connection_charging_information->restricted_cn_list = NULL;
    }
    if (n2_connection_charging_information->allowed_nssai) {
        OpenAPI_list_for_each(n2_connection_charging_information->allowed_nssai, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(n2_connection_charging_information->allowed_nssai);
        n2_connection_charging_information->allowed_nssai = NULL;
    }
    if (n2_connection_charging_information->n_ssai_map_list) {
        OpenAPI_list_for_each(n2_connection_charging_information->n_ssai_map_list, node) {
            OpenAPI_nssai_map_free(node->data);
        }
        OpenAPI_list_free(n2_connection_charging_information->n_ssai_map_list);
        n2_connection_charging_information->n_ssai_map_list = NULL;
    }
    if (n2_connection_charging_information->rrc_est_cause) {
        ogs_free(n2_connection_charging_information->rrc_est_cause);
        n2_connection_charging_information->rrc_est_cause = NULL;
    }
    ogs_free(n2_connection_charging_information);
}

cJSON *OpenAPI_n2_connection_charging_information_convertToJSON(OpenAPI_n2_connection_charging_information_t *n2_connection_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (n2_connection_charging_information == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [N2ConnectionChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (cJSON_AddNumberToObject(item, "n2ConnectionMessageType", n2_connection_charging_information->n2_connection_message_type) == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [n2_connection_message_type]");
        goto end;
    }

    if (n2_connection_charging_information->user_information) {
    cJSON *user_information_local_JSON = OpenAPI_user_information_convertToJSON(n2_connection_charging_information->user_information);
    if (user_information_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [user_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userInformation", user_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [user_information]");
        goto end;
    }
    }

    if (n2_connection_charging_information->user_locationinfo) {
    cJSON *user_locationinfo_local_JSON = OpenAPI_charging_user_location_convertToJSON(n2_connection_charging_information->user_locationinfo);
    if (user_locationinfo_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationinfo", user_locationinfo_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    }

    if (n2_connection_charging_information->p_s_cell_information) {
    cJSON *p_s_cell_information_local_JSON = OpenAPI_ps_cell_information_convertToJSON(n2_connection_charging_information->p_s_cell_information);
    if (p_s_cell_information_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [p_s_cell_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pSCellInformation", p_s_cell_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [p_s_cell_information]");
        goto end;
    }
    }

    if (n2_connection_charging_information->uetime_zone) {
    if (cJSON_AddStringToObject(item, "uetimeZone", n2_connection_charging_information->uetime_zone) == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [uetime_zone]");
        goto end;
    }
    }

    if (n2_connection_charging_information->r_at_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "rATType", OpenAPI_rat_type_ToString(n2_connection_charging_information->r_at_type)) == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [r_at_type]");
        goto end;
    }
    }

    if (n2_connection_charging_information->is_amf_ue_ngap_id) {
    if (cJSON_AddNumberToObject(item, "amfUeNgapId", n2_connection_charging_information->amf_ue_ngap_id) == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [amf_ue_ngap_id]");
        goto end;
    }
    }

    if (n2_connection_charging_information->is_ran_ue_ngap_id) {
    if (cJSON_AddNumberToObject(item, "ranUeNgapId", n2_connection_charging_information->ran_ue_ngap_id) == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [ran_ue_ngap_id]");
        goto end;
    }
    }

    if (n2_connection_charging_information->ran_node_id) {
    cJSON *ran_node_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(n2_connection_charging_information->ran_node_id);
    if (ran_node_id_local_JSON == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [ran_node_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ranNodeId", ran_node_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [ran_node_id]");
        goto end;
    }
    }

    if (n2_connection_charging_information->restricted_rat_list != OpenAPI_rat_type_NULL) {
    cJSON *restricted_rat_listList = cJSON_AddArrayToObject(item, "restrictedRatList");
    if (restricted_rat_listList == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [restricted_rat_list]");
        goto end;
    }
    OpenAPI_list_for_each(n2_connection_charging_information->restricted_rat_list, node) {
        if (cJSON_AddStringToObject(restricted_rat_listList, "", OpenAPI_rat_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [restricted_rat_list]");
            goto end;
        }
    }
    }

    if (n2_connection_charging_information->forbidden_area_list) {
    cJSON *forbidden_area_listList = cJSON_AddArrayToObject(item, "forbiddenAreaList");
    if (forbidden_area_listList == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [forbidden_area_list]");
        goto end;
    }
    OpenAPI_list_for_each(n2_connection_charging_information->forbidden_area_list, node) {
        cJSON *itemLocal = OpenAPI_area_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [forbidden_area_list]");
            goto end;
        }
        cJSON_AddItemToArray(forbidden_area_listList, itemLocal);
    }
    }

    if (n2_connection_charging_information->service_area_restriction) {
    cJSON *service_area_restrictionList = cJSON_AddArrayToObject(item, "serviceAreaRestriction");
    if (service_area_restrictionList == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [service_area_restriction]");
        goto end;
    }
    OpenAPI_list_for_each(n2_connection_charging_information->service_area_restriction, node) {
        cJSON *itemLocal = OpenAPI_service_area_restriction_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [service_area_restriction]");
            goto end;
        }
        cJSON_AddItemToArray(service_area_restrictionList, itemLocal);
    }
    }

    if (n2_connection_charging_information->restricted_cn_list != OpenAPI_core_network_type_NULL) {
    cJSON *restricted_cn_listList = cJSON_AddArrayToObject(item, "restrictedCnList");
    if (restricted_cn_listList == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [restricted_cn_list]");
        goto end;
    }
    OpenAPI_list_for_each(n2_connection_charging_information->restricted_cn_list, node) {
        if (cJSON_AddStringToObject(restricted_cn_listList, "", OpenAPI_core_network_type_ToString((intptr_t)node->data)) == NULL) {
            ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [restricted_cn_list]");
            goto end;
        }
    }
    }

    if (n2_connection_charging_information->allowed_nssai) {
    cJSON *allowed_nssaiList = cJSON_AddArrayToObject(item, "allowedNSSAI");
    if (allowed_nssaiList == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [allowed_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(n2_connection_charging_information->allowed_nssai, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [allowed_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_nssaiList, itemLocal);
    }
    }

    if (n2_connection_charging_information->n_ssai_map_list) {
    cJSON *n_ssai_map_listList = cJSON_AddArrayToObject(item, "nSSAIMapList");
    if (n_ssai_map_listList == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [n_ssai_map_list]");
        goto end;
    }
    OpenAPI_list_for_each(n2_connection_charging_information->n_ssai_map_list, node) {
        cJSON *itemLocal = OpenAPI_nssai_map_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [n_ssai_map_list]");
            goto end;
        }
        cJSON_AddItemToArray(n_ssai_map_listList, itemLocal);
    }
    }

    if (n2_connection_charging_information->rrc_est_cause) {
    if (cJSON_AddStringToObject(item, "rrcEstCause", n2_connection_charging_information->rrc_est_cause) == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [rrc_est_cause]");
        goto end;
    }
    }

    if (n2_connection_charging_information->is_satellite_access_indicator) {
    if (cJSON_AddBoolToObject(item, "satelliteAccessIndicator", n2_connection_charging_information->satellite_access_indicator) == NULL) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed [satellite_access_indicator]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_n2_connection_charging_information_t *OpenAPI_n2_connection_charging_information_parseFromJSON(cJSON *n2_connection_charging_informationJSON)
{
    OpenAPI_n2_connection_charging_information_t *n2_connection_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *n2_connection_message_type = NULL;
    cJSON *user_information = NULL;
    OpenAPI_user_information_t *user_information_local_nonprim = NULL;
    cJSON *user_locationinfo = NULL;
    OpenAPI_charging_user_location_t *user_locationinfo_local_nonprim = NULL;
    cJSON *p_s_cell_information = NULL;
    OpenAPI_ps_cell_information_t *p_s_cell_information_local_nonprim = NULL;
    cJSON *uetime_zone = NULL;
    cJSON *r_at_type = NULL;
    OpenAPI_rat_type_e r_at_typeVariable = 0;
    cJSON *amf_ue_ngap_id = NULL;
    cJSON *ran_ue_ngap_id = NULL;
    cJSON *ran_node_id = NULL;
    OpenAPI_global_ran_node_id_t *ran_node_id_local_nonprim = NULL;
    cJSON *restricted_rat_list = NULL;
    OpenAPI_list_t *restricted_rat_listList = NULL;
    cJSON *forbidden_area_list = NULL;
    OpenAPI_list_t *forbidden_area_listList = NULL;
    cJSON *service_area_restriction = NULL;
    OpenAPI_list_t *service_area_restrictionList = NULL;
    cJSON *restricted_cn_list = NULL;
    OpenAPI_list_t *restricted_cn_listList = NULL;
    cJSON *allowed_nssai = NULL;
    OpenAPI_list_t *allowed_nssaiList = NULL;
    cJSON *n_ssai_map_list = NULL;
    OpenAPI_list_t *n_ssai_map_listList = NULL;
    cJSON *rrc_est_cause = NULL;
    cJSON *satellite_access_indicator = NULL;
    n2_connection_message_type = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "n2ConnectionMessageType");
    if (!n2_connection_message_type) {
        ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [n2_connection_message_type]");
        goto end;
    }
    if (!cJSON_IsNumber(n2_connection_message_type)) {
        ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [n2_connection_message_type]");
        goto end;
    }

    user_information = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "userInformation");
    if (user_information) {
    user_information_local_nonprim = OpenAPI_user_information_parseFromJSON(user_information);
    if (!user_information_local_nonprim) {
        ogs_error("OpenAPI_user_information_parseFromJSON failed [user_information]");
        goto end;
    }
    }

    user_locationinfo = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "userLocationinfo");
    if (user_locationinfo) {
    user_locationinfo_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(user_locationinfo);
    if (!user_locationinfo_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [user_locationinfo]");
        goto end;
    }
    }

    p_s_cell_information = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "pSCellInformation");
    if (p_s_cell_information) {
    p_s_cell_information_local_nonprim = OpenAPI_ps_cell_information_parseFromJSON(p_s_cell_information);
    if (!p_s_cell_information_local_nonprim) {
        ogs_error("OpenAPI_ps_cell_information_parseFromJSON failed [p_s_cell_information]");
        goto end;
    }
    }

    uetime_zone = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "uetimeZone");
    if (uetime_zone) {
    if (!cJSON_IsString(uetime_zone) && !cJSON_IsNull(uetime_zone)) {
        ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [uetime_zone]");
        goto end;
    }
    }

    r_at_type = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "rATType");
    if (r_at_type) {
    if (!cJSON_IsString(r_at_type)) {
        ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [r_at_type]");
        goto end;
    }
    r_at_typeVariable = OpenAPI_rat_type_FromString(r_at_type->valuestring);
    }

    amf_ue_ngap_id = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "amfUeNgapId");
    if (amf_ue_ngap_id) {
    if (!cJSON_IsNumber(amf_ue_ngap_id)) {
        ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [amf_ue_ngap_id]");
        goto end;
    }
    }

    ran_ue_ngap_id = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "ranUeNgapId");
    if (ran_ue_ngap_id) {
    if (!cJSON_IsNumber(ran_ue_ngap_id)) {
        ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [ran_ue_ngap_id]");
        goto end;
    }
    }

    ran_node_id = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "ranNodeId");
    if (ran_node_id) {
    ran_node_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(ran_node_id);
    if (!ran_node_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [ran_node_id]");
        goto end;
    }
    }

    restricted_rat_list = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "restrictedRatList");
    if (restricted_rat_list) {
        cJSON *restricted_rat_list_local = NULL;
        if (!cJSON_IsArray(restricted_rat_list)) {
            ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [restricted_rat_list]");
            goto end;
        }

        restricted_rat_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(restricted_rat_list_local, restricted_rat_list) {
            OpenAPI_rat_type_e localEnum = OpenAPI_rat_type_NULL;
            if (!cJSON_IsString(restricted_rat_list_local)) {
                ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [restricted_rat_list]");
                goto end;
            }
            localEnum = OpenAPI_rat_type_FromString(restricted_rat_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"restricted_rat_list\" is not supported. Ignoring it ...",
                         restricted_rat_list_local->valuestring);
            } else {
                OpenAPI_list_add(restricted_rat_listList, (void *)localEnum);
            }
        }
        if (restricted_rat_listList->count == 0) {
            ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed: Expected restricted_rat_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    forbidden_area_list = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "forbiddenAreaList");
    if (forbidden_area_list) {
        cJSON *forbidden_area_list_local = NULL;
        if (!cJSON_IsArray(forbidden_area_list)) {
            ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [forbidden_area_list]");
            goto end;
        }

        forbidden_area_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(forbidden_area_list_local, forbidden_area_list) {
            if (!cJSON_IsObject(forbidden_area_list_local)) {
                ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [forbidden_area_list]");
                goto end;
            }
            OpenAPI_area_t *forbidden_area_listItem = OpenAPI_area_parseFromJSON(forbidden_area_list_local);
            if (!forbidden_area_listItem) {
                ogs_error("No forbidden_area_listItem");
                goto end;
            }
            OpenAPI_list_add(forbidden_area_listList, forbidden_area_listItem);
        }
    }

    service_area_restriction = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "serviceAreaRestriction");
    if (service_area_restriction) {
        cJSON *service_area_restriction_local = NULL;
        if (!cJSON_IsArray(service_area_restriction)) {
            ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [service_area_restriction]");
            goto end;
        }

        service_area_restrictionList = OpenAPI_list_create();

        cJSON_ArrayForEach(service_area_restriction_local, service_area_restriction) {
            if (!cJSON_IsObject(service_area_restriction_local)) {
                ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [service_area_restriction]");
                goto end;
            }
            OpenAPI_service_area_restriction_t *service_area_restrictionItem = OpenAPI_service_area_restriction_parseFromJSON(service_area_restriction_local);
            if (!service_area_restrictionItem) {
                ogs_error("No service_area_restrictionItem");
                goto end;
            }
            OpenAPI_list_add(service_area_restrictionList, service_area_restrictionItem);
        }
    }

    restricted_cn_list = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "restrictedCnList");
    if (restricted_cn_list) {
        cJSON *restricted_cn_list_local = NULL;
        if (!cJSON_IsArray(restricted_cn_list)) {
            ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [restricted_cn_list]");
            goto end;
        }

        restricted_cn_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(restricted_cn_list_local, restricted_cn_list) {
            OpenAPI_core_network_type_e localEnum = OpenAPI_core_network_type_NULL;
            if (!cJSON_IsString(restricted_cn_list_local)) {
                ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [restricted_cn_list]");
                goto end;
            }
            localEnum = OpenAPI_core_network_type_FromString(restricted_cn_list_local->valuestring);
            if (!localEnum) {
                ogs_info("Enum value \"%s\" for field \"restricted_cn_list\" is not supported. Ignoring it ...",
                         restricted_cn_list_local->valuestring);
            } else {
                OpenAPI_list_add(restricted_cn_listList, (void *)localEnum);
            }
        }
        if (restricted_cn_listList->count == 0) {
            ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed: Expected restricted_cn_listList to not be empty (after ignoring unsupported enum values).");
            goto end;
        }
    }

    allowed_nssai = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "allowedNSSAI");
    if (allowed_nssai) {
        cJSON *allowed_nssai_local = NULL;
        if (!cJSON_IsArray(allowed_nssai)) {
            ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [allowed_nssai]");
            goto end;
        }

        allowed_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nssai_local, allowed_nssai) {
            if (!cJSON_IsObject(allowed_nssai_local)) {
                ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [allowed_nssai]");
                goto end;
            }
            OpenAPI_snssai_t *allowed_nssaiItem = OpenAPI_snssai_parseFromJSON(allowed_nssai_local);
            if (!allowed_nssaiItem) {
                ogs_error("No allowed_nssaiItem");
                goto end;
            }
            OpenAPI_list_add(allowed_nssaiList, allowed_nssaiItem);
        }
    }

    n_ssai_map_list = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "nSSAIMapList");
    if (n_ssai_map_list) {
        cJSON *n_ssai_map_list_local = NULL;
        if (!cJSON_IsArray(n_ssai_map_list)) {
            ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [n_ssai_map_list]");
            goto end;
        }

        n_ssai_map_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(n_ssai_map_list_local, n_ssai_map_list) {
            if (!cJSON_IsObject(n_ssai_map_list_local)) {
                ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [n_ssai_map_list]");
                goto end;
            }
            OpenAPI_nssai_map_t *n_ssai_map_listItem = OpenAPI_nssai_map_parseFromJSON(n_ssai_map_list_local);
            if (!n_ssai_map_listItem) {
                ogs_error("No n_ssai_map_listItem");
                goto end;
            }
            OpenAPI_list_add(n_ssai_map_listList, n_ssai_map_listItem);
        }
    }

    rrc_est_cause = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "rrcEstCause");
    if (rrc_est_cause) {
    if (!cJSON_IsString(rrc_est_cause) && !cJSON_IsNull(rrc_est_cause)) {
        ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [rrc_est_cause]");
        goto end;
    }
    }

    satellite_access_indicator = cJSON_GetObjectItemCaseSensitive(n2_connection_charging_informationJSON, "satelliteAccessIndicator");
    if (satellite_access_indicator) {
    if (!cJSON_IsBool(satellite_access_indicator)) {
        ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON() failed [satellite_access_indicator]");
        goto end;
    }
    }

    n2_connection_charging_information_local_var = OpenAPI_n2_connection_charging_information_create (
        
        n2_connection_message_type->valuedouble,
        user_information ? user_information_local_nonprim : NULL,
        user_locationinfo ? user_locationinfo_local_nonprim : NULL,
        p_s_cell_information ? p_s_cell_information_local_nonprim : NULL,
        uetime_zone && !cJSON_IsNull(uetime_zone) ? ogs_strdup(uetime_zone->valuestring) : NULL,
        r_at_type ? r_at_typeVariable : 0,
        amf_ue_ngap_id ? true : false,
        amf_ue_ngap_id ? amf_ue_ngap_id->valuedouble : 0,
        ran_ue_ngap_id ? true : false,
        ran_ue_ngap_id ? ran_ue_ngap_id->valuedouble : 0,
        ran_node_id ? ran_node_id_local_nonprim : NULL,
        restricted_rat_list ? restricted_rat_listList : NULL,
        forbidden_area_list ? forbidden_area_listList : NULL,
        service_area_restriction ? service_area_restrictionList : NULL,
        restricted_cn_list ? restricted_cn_listList : NULL,
        allowed_nssai ? allowed_nssaiList : NULL,
        n_ssai_map_list ? n_ssai_map_listList : NULL,
        rrc_est_cause && !cJSON_IsNull(rrc_est_cause) ? ogs_strdup(rrc_est_cause->valuestring) : NULL,
        satellite_access_indicator ? true : false,
        satellite_access_indicator ? satellite_access_indicator->valueint : 0
    );

    return n2_connection_charging_information_local_var;
end:
    if (user_information_local_nonprim) {
        OpenAPI_user_information_free(user_information_local_nonprim);
        user_information_local_nonprim = NULL;
    }
    if (user_locationinfo_local_nonprim) {
        OpenAPI_charging_user_location_free(user_locationinfo_local_nonprim);
        user_locationinfo_local_nonprim = NULL;
    }
    if (p_s_cell_information_local_nonprim) {
        OpenAPI_ps_cell_information_free(p_s_cell_information_local_nonprim);
        p_s_cell_information_local_nonprim = NULL;
    }
    if (ran_node_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(ran_node_id_local_nonprim);
        ran_node_id_local_nonprim = NULL;
    }
    if (restricted_rat_listList) {
        OpenAPI_list_free(restricted_rat_listList);
        restricted_rat_listList = NULL;
    }
    if (forbidden_area_listList) {
        OpenAPI_list_for_each(forbidden_area_listList, node) {
            OpenAPI_area_free(node->data);
        }
        OpenAPI_list_free(forbidden_area_listList);
        forbidden_area_listList = NULL;
    }
    if (service_area_restrictionList) {
        OpenAPI_list_for_each(service_area_restrictionList, node) {
            OpenAPI_service_area_restriction_free(node->data);
        }
        OpenAPI_list_free(service_area_restrictionList);
        service_area_restrictionList = NULL;
    }
    if (restricted_cn_listList) {
        OpenAPI_list_free(restricted_cn_listList);
        restricted_cn_listList = NULL;
    }
    if (allowed_nssaiList) {
        OpenAPI_list_for_each(allowed_nssaiList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(allowed_nssaiList);
        allowed_nssaiList = NULL;
    }
    if (n_ssai_map_listList) {
        OpenAPI_list_for_each(n_ssai_map_listList, node) {
            OpenAPI_nssai_map_free(node->data);
        }
        OpenAPI_list_free(n_ssai_map_listList);
        n_ssai_map_listList = NULL;
    }
    return NULL;
}

OpenAPI_n2_connection_charging_information_t *OpenAPI_n2_connection_charging_information_copy(OpenAPI_n2_connection_charging_information_t *dst, OpenAPI_n2_connection_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_n2_connection_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_n2_connection_charging_information_convertToJSON() failed");
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

    OpenAPI_n2_connection_charging_information_free(dst);
    dst = OpenAPI_n2_connection_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

