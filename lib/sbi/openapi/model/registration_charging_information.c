
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "registration_charging_information.h"

OpenAPI_registration_charging_information_t *OpenAPI_registration_charging_information_create(
    OpenAPI_registration_message_type_e registration_messagetype,
    OpenAPI_user_information_t *user_information,
    OpenAPI_charging_user_location_t *user_locationinfo,
    OpenAPI_ps_cell_information_t *p_s_cell_information,
    char *uetime_zone,
    OpenAPI_rat_type_e r_at_type,
    char *_5_gmm_capability,
    OpenAPI_mico_mode_indication_e m_ico_mode_indication,
    OpenAPI_sms_indication_e sms_indication,
    OpenAPI_list_t *tai_list,
    OpenAPI_list_t *service_area_restriction,
    OpenAPI_list_t *requested_nssai,
    OpenAPI_list_t *allowed_nssai,
    OpenAPI_list_t *rejected_nssai,
    OpenAPI_list_t *n_ssai_map_list,
    OpenAPI_list_t *alternative_nssai_map,
    bool is_amf_ue_ngap_id,
    int amf_ue_ngap_id,
    bool is_ran_ue_ngap_id,
    int ran_ue_ngap_id,
    OpenAPI_global_ran_node_id_t *ran_node_id,
    OpenAPI_plmn_id_nid_t *s_npnid,
    OpenAPI_list_t *c_agid_list,
    bool is_satellite_access_indicator,
    int satellite_access_indicator,
    char *service_level_aa
)
{
    OpenAPI_registration_charging_information_t *registration_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_registration_charging_information_t));
    ogs_assert(registration_charging_information_local_var);

    registration_charging_information_local_var->registration_messagetype = registration_messagetype;
    registration_charging_information_local_var->user_information = user_information;
    registration_charging_information_local_var->user_locationinfo = user_locationinfo;
    registration_charging_information_local_var->p_s_cell_information = p_s_cell_information;
    registration_charging_information_local_var->uetime_zone = uetime_zone;
    registration_charging_information_local_var->r_at_type = r_at_type;
    registration_charging_information_local_var->_5_gmm_capability = _5_gmm_capability;
    registration_charging_information_local_var->m_ico_mode_indication = m_ico_mode_indication;
    registration_charging_information_local_var->sms_indication = sms_indication;
    registration_charging_information_local_var->tai_list = tai_list;
    registration_charging_information_local_var->service_area_restriction = service_area_restriction;
    registration_charging_information_local_var->requested_nssai = requested_nssai;
    registration_charging_information_local_var->allowed_nssai = allowed_nssai;
    registration_charging_information_local_var->rejected_nssai = rejected_nssai;
    registration_charging_information_local_var->n_ssai_map_list = n_ssai_map_list;
    registration_charging_information_local_var->alternative_nssai_map = alternative_nssai_map;
    registration_charging_information_local_var->is_amf_ue_ngap_id = is_amf_ue_ngap_id;
    registration_charging_information_local_var->amf_ue_ngap_id = amf_ue_ngap_id;
    registration_charging_information_local_var->is_ran_ue_ngap_id = is_ran_ue_ngap_id;
    registration_charging_information_local_var->ran_ue_ngap_id = ran_ue_ngap_id;
    registration_charging_information_local_var->ran_node_id = ran_node_id;
    registration_charging_information_local_var->s_npnid = s_npnid;
    registration_charging_information_local_var->c_agid_list = c_agid_list;
    registration_charging_information_local_var->is_satellite_access_indicator = is_satellite_access_indicator;
    registration_charging_information_local_var->satellite_access_indicator = satellite_access_indicator;
    registration_charging_information_local_var->service_level_aa = service_level_aa;

    return registration_charging_information_local_var;
}

void OpenAPI_registration_charging_information_free(OpenAPI_registration_charging_information_t *registration_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == registration_charging_information) {
        return;
    }
    if (registration_charging_information->user_information) {
        OpenAPI_user_information_free(registration_charging_information->user_information);
        registration_charging_information->user_information = NULL;
    }
    if (registration_charging_information->user_locationinfo) {
        OpenAPI_charging_user_location_free(registration_charging_information->user_locationinfo);
        registration_charging_information->user_locationinfo = NULL;
    }
    if (registration_charging_information->p_s_cell_information) {
        OpenAPI_ps_cell_information_free(registration_charging_information->p_s_cell_information);
        registration_charging_information->p_s_cell_information = NULL;
    }
    if (registration_charging_information->uetime_zone) {
        ogs_free(registration_charging_information->uetime_zone);
        registration_charging_information->uetime_zone = NULL;
    }
    if (registration_charging_information->_5_gmm_capability) {
        ogs_free(registration_charging_information->_5_gmm_capability);
        registration_charging_information->_5_gmm_capability = NULL;
    }
    if (registration_charging_information->tai_list) {
        OpenAPI_list_for_each(registration_charging_information->tai_list, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(registration_charging_information->tai_list);
        registration_charging_information->tai_list = NULL;
    }
    if (registration_charging_information->service_area_restriction) {
        OpenAPI_list_for_each(registration_charging_information->service_area_restriction, node) {
            OpenAPI_service_area_restriction_free(node->data);
        }
        OpenAPI_list_free(registration_charging_information->service_area_restriction);
        registration_charging_information->service_area_restriction = NULL;
    }
    if (registration_charging_information->requested_nssai) {
        OpenAPI_list_for_each(registration_charging_information->requested_nssai, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(registration_charging_information->requested_nssai);
        registration_charging_information->requested_nssai = NULL;
    }
    if (registration_charging_information->allowed_nssai) {
        OpenAPI_list_for_each(registration_charging_information->allowed_nssai, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(registration_charging_information->allowed_nssai);
        registration_charging_information->allowed_nssai = NULL;
    }
    if (registration_charging_information->rejected_nssai) {
        OpenAPI_list_for_each(registration_charging_information->rejected_nssai, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(registration_charging_information->rejected_nssai);
        registration_charging_information->rejected_nssai = NULL;
    }
    if (registration_charging_information->n_ssai_map_list) {
        OpenAPI_list_for_each(registration_charging_information->n_ssai_map_list, node) {
            OpenAPI_nssai_map_free(node->data);
        }
        OpenAPI_list_free(registration_charging_information->n_ssai_map_list);
        registration_charging_information->n_ssai_map_list = NULL;
    }
    if (registration_charging_information->alternative_nssai_map) {
        OpenAPI_list_for_each(registration_charging_information->alternative_nssai_map, node) {
            OpenAPI_alternative_nssai_map_free(node->data);
        }
        OpenAPI_list_free(registration_charging_information->alternative_nssai_map);
        registration_charging_information->alternative_nssai_map = NULL;
    }
    if (registration_charging_information->ran_node_id) {
        OpenAPI_global_ran_node_id_free(registration_charging_information->ran_node_id);
        registration_charging_information->ran_node_id = NULL;
    }
    if (registration_charging_information->s_npnid) {
        OpenAPI_plmn_id_nid_free(registration_charging_information->s_npnid);
        registration_charging_information->s_npnid = NULL;
    }
    if (registration_charging_information->c_agid_list) {
        OpenAPI_list_for_each(registration_charging_information->c_agid_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(registration_charging_information->c_agid_list);
        registration_charging_information->c_agid_list = NULL;
    }
    if (registration_charging_information->service_level_aa) {
        ogs_free(registration_charging_information->service_level_aa);
        registration_charging_information->service_level_aa = NULL;
    }
    ogs_free(registration_charging_information);
}

cJSON *OpenAPI_registration_charging_information_convertToJSON(OpenAPI_registration_charging_information_t *registration_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (registration_charging_information == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [RegistrationChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (registration_charging_information->registration_messagetype == OpenAPI_registration_message_type_NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [registration_messagetype]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "registrationMessagetype", OpenAPI_registration_message_type_ToString(registration_charging_information->registration_messagetype)) == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [registration_messagetype]");
        goto end;
    }

    if (registration_charging_information->user_information) {
    cJSON *user_information_local_JSON = OpenAPI_user_information_convertToJSON(registration_charging_information->user_information);
    if (user_information_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [user_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userInformation", user_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [user_information]");
        goto end;
    }
    }

    if (registration_charging_information->user_locationinfo) {
    cJSON *user_locationinfo_local_JSON = OpenAPI_charging_user_location_convertToJSON(registration_charging_information->user_locationinfo);
    if (user_locationinfo_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationinfo", user_locationinfo_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    }

    if (registration_charging_information->p_s_cell_information) {
    cJSON *p_s_cell_information_local_JSON = OpenAPI_ps_cell_information_convertToJSON(registration_charging_information->p_s_cell_information);
    if (p_s_cell_information_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [p_s_cell_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pSCellInformation", p_s_cell_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [p_s_cell_information]");
        goto end;
    }
    }

    if (registration_charging_information->uetime_zone) {
    if (cJSON_AddStringToObject(item, "uetimeZone", registration_charging_information->uetime_zone) == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [uetime_zone]");
        goto end;
    }
    }

    if (registration_charging_information->r_at_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "rATType", OpenAPI_rat_type_ToString(registration_charging_information->r_at_type)) == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [r_at_type]");
        goto end;
    }
    }

    if (registration_charging_information->_5_gmm_capability) {
    if (cJSON_AddStringToObject(item, "5GMMCapability", registration_charging_information->_5_gmm_capability) == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [_5_gmm_capability]");
        goto end;
    }
    }

    if (registration_charging_information->m_ico_mode_indication != OpenAPI_mico_mode_indication_NULL) {
    if (cJSON_AddStringToObject(item, "mICOModeIndication", OpenAPI_mico_mode_indication_ToString(registration_charging_information->m_ico_mode_indication)) == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [m_ico_mode_indication]");
        goto end;
    }
    }

    if (registration_charging_information->sms_indication != OpenAPI_sms_indication_NULL) {
    if (cJSON_AddStringToObject(item, "smsIndication", OpenAPI_sms_indication_ToString(registration_charging_information->sms_indication)) == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [sms_indication]");
        goto end;
    }
    }

    if (registration_charging_information->tai_list) {
    cJSON *tai_listList = cJSON_AddArrayToObject(item, "taiList");
    if (tai_listList == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [tai_list]");
        goto end;
    }
    OpenAPI_list_for_each(registration_charging_information->tai_list, node) {
        cJSON *itemLocal = OpenAPI_tai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [tai_list]");
            goto end;
        }
        cJSON_AddItemToArray(tai_listList, itemLocal);
    }
    }

    if (registration_charging_information->service_area_restriction) {
    cJSON *service_area_restrictionList = cJSON_AddArrayToObject(item, "serviceAreaRestriction");
    if (service_area_restrictionList == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [service_area_restriction]");
        goto end;
    }
    OpenAPI_list_for_each(registration_charging_information->service_area_restriction, node) {
        cJSON *itemLocal = OpenAPI_service_area_restriction_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [service_area_restriction]");
            goto end;
        }
        cJSON_AddItemToArray(service_area_restrictionList, itemLocal);
    }
    }

    if (registration_charging_information->requested_nssai) {
    cJSON *requested_nssaiList = cJSON_AddArrayToObject(item, "requestedNSSAI");
    if (requested_nssaiList == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [requested_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(registration_charging_information->requested_nssai, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [requested_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(requested_nssaiList, itemLocal);
    }
    }

    if (registration_charging_information->allowed_nssai) {
    cJSON *allowed_nssaiList = cJSON_AddArrayToObject(item, "allowedNSSAI");
    if (allowed_nssaiList == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [allowed_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(registration_charging_information->allowed_nssai, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [allowed_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(allowed_nssaiList, itemLocal);
    }
    }

    if (registration_charging_information->rejected_nssai) {
    cJSON *rejected_nssaiList = cJSON_AddArrayToObject(item, "rejectedNSSAI");
    if (rejected_nssaiList == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [rejected_nssai]");
        goto end;
    }
    OpenAPI_list_for_each(registration_charging_information->rejected_nssai, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [rejected_nssai]");
            goto end;
        }
        cJSON_AddItemToArray(rejected_nssaiList, itemLocal);
    }
    }

    if (registration_charging_information->n_ssai_map_list) {
    cJSON *n_ssai_map_listList = cJSON_AddArrayToObject(item, "nSSAIMapList");
    if (n_ssai_map_listList == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [n_ssai_map_list]");
        goto end;
    }
    OpenAPI_list_for_each(registration_charging_information->n_ssai_map_list, node) {
        cJSON *itemLocal = OpenAPI_nssai_map_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [n_ssai_map_list]");
            goto end;
        }
        cJSON_AddItemToArray(n_ssai_map_listList, itemLocal);
    }
    }

    if (registration_charging_information->alternative_nssai_map) {
    cJSON *alternative_nssai_mapList = cJSON_AddArrayToObject(item, "alternativeNSSAIMap");
    if (alternative_nssai_mapList == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [alternative_nssai_map]");
        goto end;
    }
    OpenAPI_list_for_each(registration_charging_information->alternative_nssai_map, node) {
        cJSON *itemLocal = OpenAPI_alternative_nssai_map_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [alternative_nssai_map]");
            goto end;
        }
        cJSON_AddItemToArray(alternative_nssai_mapList, itemLocal);
    }
    }

    if (registration_charging_information->is_amf_ue_ngap_id) {
    if (cJSON_AddNumberToObject(item, "amfUeNgapId", registration_charging_information->amf_ue_ngap_id) == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [amf_ue_ngap_id]");
        goto end;
    }
    }

    if (registration_charging_information->is_ran_ue_ngap_id) {
    if (cJSON_AddNumberToObject(item, "ranUeNgapId", registration_charging_information->ran_ue_ngap_id) == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [ran_ue_ngap_id]");
        goto end;
    }
    }

    if (registration_charging_information->ran_node_id) {
    cJSON *ran_node_id_local_JSON = OpenAPI_global_ran_node_id_convertToJSON(registration_charging_information->ran_node_id);
    if (ran_node_id_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [ran_node_id]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ranNodeId", ran_node_id_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [ran_node_id]");
        goto end;
    }
    }

    if (registration_charging_information->s_npnid) {
    cJSON *s_npnid_local_JSON = OpenAPI_plmn_id_nid_convertToJSON(registration_charging_information->s_npnid);
    if (s_npnid_local_JSON == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [s_npnid]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sNPNID", s_npnid_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [s_npnid]");
        goto end;
    }
    }

    if (registration_charging_information->c_agid_list) {
    cJSON *c_agid_listList = cJSON_AddArrayToObject(item, "cAGIDList");
    if (c_agid_listList == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [c_agid_list]");
        goto end;
    }
    OpenAPI_list_for_each(registration_charging_information->c_agid_list, node) {
        if (cJSON_AddStringToObject(c_agid_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [c_agid_list]");
            goto end;
        }
    }
    }

    if (registration_charging_information->is_satellite_access_indicator) {
    if (cJSON_AddBoolToObject(item, "satelliteAccessIndicator", registration_charging_information->satellite_access_indicator) == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [satellite_access_indicator]");
        goto end;
    }
    }

    if (registration_charging_information->service_level_aa) {
    if (cJSON_AddStringToObject(item, "serviceLevelAA", registration_charging_information->service_level_aa) == NULL) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed [service_level_aa]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_registration_charging_information_t *OpenAPI_registration_charging_information_parseFromJSON(cJSON *registration_charging_informationJSON)
{
    OpenAPI_registration_charging_information_t *registration_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *registration_messagetype = NULL;
    OpenAPI_registration_message_type_e registration_messagetypeVariable = 0;
    cJSON *user_information = NULL;
    OpenAPI_user_information_t *user_information_local_nonprim = NULL;
    cJSON *user_locationinfo = NULL;
    OpenAPI_charging_user_location_t *user_locationinfo_local_nonprim = NULL;
    cJSON *p_s_cell_information = NULL;
    OpenAPI_ps_cell_information_t *p_s_cell_information_local_nonprim = NULL;
    cJSON *uetime_zone = NULL;
    cJSON *r_at_type = NULL;
    OpenAPI_rat_type_e r_at_typeVariable = 0;
    cJSON *_5_gmm_capability = NULL;
    cJSON *m_ico_mode_indication = NULL;
    OpenAPI_mico_mode_indication_e m_ico_mode_indicationVariable = 0;
    cJSON *sms_indication = NULL;
    OpenAPI_sms_indication_e sms_indicationVariable = 0;
    cJSON *tai_list = NULL;
    OpenAPI_list_t *tai_listList = NULL;
    cJSON *service_area_restriction = NULL;
    OpenAPI_list_t *service_area_restrictionList = NULL;
    cJSON *requested_nssai = NULL;
    OpenAPI_list_t *requested_nssaiList = NULL;
    cJSON *allowed_nssai = NULL;
    OpenAPI_list_t *allowed_nssaiList = NULL;
    cJSON *rejected_nssai = NULL;
    OpenAPI_list_t *rejected_nssaiList = NULL;
    cJSON *n_ssai_map_list = NULL;
    OpenAPI_list_t *n_ssai_map_listList = NULL;
    cJSON *alternative_nssai_map = NULL;
    OpenAPI_list_t *alternative_nssai_mapList = NULL;
    cJSON *amf_ue_ngap_id = NULL;
    cJSON *ran_ue_ngap_id = NULL;
    cJSON *ran_node_id = NULL;
    OpenAPI_global_ran_node_id_t *ran_node_id_local_nonprim = NULL;
    cJSON *s_npnid = NULL;
    OpenAPI_plmn_id_nid_t *s_npnid_local_nonprim = NULL;
    cJSON *c_agid_list = NULL;
    OpenAPI_list_t *c_agid_listList = NULL;
    cJSON *satellite_access_indicator = NULL;
    cJSON *service_level_aa = NULL;
    registration_messagetype = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "registrationMessagetype");
    if (!registration_messagetype) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [registration_messagetype]");
        goto end;
    }
    if (!cJSON_IsString(registration_messagetype)) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [registration_messagetype]");
        goto end;
    }
    registration_messagetypeVariable = OpenAPI_registration_message_type_FromString(registration_messagetype->valuestring);

    user_information = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "userInformation");
    if (user_information) {
    user_information_local_nonprim = OpenAPI_user_information_parseFromJSON(user_information);
    if (!user_information_local_nonprim) {
        ogs_error("OpenAPI_user_information_parseFromJSON failed [user_information]");
        goto end;
    }
    }

    user_locationinfo = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "userLocationinfo");
    if (user_locationinfo) {
    user_locationinfo_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(user_locationinfo);
    if (!user_locationinfo_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [user_locationinfo]");
        goto end;
    }
    }

    p_s_cell_information = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "pSCellInformation");
    if (p_s_cell_information) {
    p_s_cell_information_local_nonprim = OpenAPI_ps_cell_information_parseFromJSON(p_s_cell_information);
    if (!p_s_cell_information_local_nonprim) {
        ogs_error("OpenAPI_ps_cell_information_parseFromJSON failed [p_s_cell_information]");
        goto end;
    }
    }

    uetime_zone = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "uetimeZone");
    if (uetime_zone) {
    if (!cJSON_IsString(uetime_zone) && !cJSON_IsNull(uetime_zone)) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [uetime_zone]");
        goto end;
    }
    }

    r_at_type = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "rATType");
    if (r_at_type) {
    if (!cJSON_IsString(r_at_type)) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [r_at_type]");
        goto end;
    }
    r_at_typeVariable = OpenAPI_rat_type_FromString(r_at_type->valuestring);
    }

    _5_gmm_capability = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "5GMMCapability");
    if (_5_gmm_capability) {
    if (!cJSON_IsString(_5_gmm_capability) && !cJSON_IsNull(_5_gmm_capability)) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [_5_gmm_capability]");
        goto end;
    }
    }

    m_ico_mode_indication = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "mICOModeIndication");
    if (m_ico_mode_indication) {
    if (!cJSON_IsString(m_ico_mode_indication)) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [m_ico_mode_indication]");
        goto end;
    }
    m_ico_mode_indicationVariable = OpenAPI_mico_mode_indication_FromString(m_ico_mode_indication->valuestring);
    }

    sms_indication = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "smsIndication");
    if (sms_indication) {
    if (!cJSON_IsString(sms_indication)) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [sms_indication]");
        goto end;
    }
    sms_indicationVariable = OpenAPI_sms_indication_FromString(sms_indication->valuestring);
    }

    tai_list = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "taiList");
    if (tai_list) {
        cJSON *tai_list_local = NULL;
        if (!cJSON_IsArray(tai_list)) {
            ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [tai_list]");
            goto end;
        }

        tai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(tai_list_local, tai_list) {
            if (!cJSON_IsObject(tai_list_local)) {
                ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [tai_list]");
                goto end;
            }
            OpenAPI_tai_t *tai_listItem = OpenAPI_tai_parseFromJSON(tai_list_local);
            if (!tai_listItem) {
                ogs_error("No tai_listItem");
                goto end;
            }
            OpenAPI_list_add(tai_listList, tai_listItem);
        }
    }

    service_area_restriction = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "serviceAreaRestriction");
    if (service_area_restriction) {
        cJSON *service_area_restriction_local = NULL;
        if (!cJSON_IsArray(service_area_restriction)) {
            ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [service_area_restriction]");
            goto end;
        }

        service_area_restrictionList = OpenAPI_list_create();

        cJSON_ArrayForEach(service_area_restriction_local, service_area_restriction) {
            if (!cJSON_IsObject(service_area_restriction_local)) {
                ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [service_area_restriction]");
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

    requested_nssai = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "requestedNSSAI");
    if (requested_nssai) {
        cJSON *requested_nssai_local = NULL;
        if (!cJSON_IsArray(requested_nssai)) {
            ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [requested_nssai]");
            goto end;
        }

        requested_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(requested_nssai_local, requested_nssai) {
            if (!cJSON_IsObject(requested_nssai_local)) {
                ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [requested_nssai]");
                goto end;
            }
            OpenAPI_snssai_t *requested_nssaiItem = OpenAPI_snssai_parseFromJSON(requested_nssai_local);
            if (!requested_nssaiItem) {
                ogs_error("No requested_nssaiItem");
                goto end;
            }
            OpenAPI_list_add(requested_nssaiList, requested_nssaiItem);
        }
    }

    allowed_nssai = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "allowedNSSAI");
    if (allowed_nssai) {
        cJSON *allowed_nssai_local = NULL;
        if (!cJSON_IsArray(allowed_nssai)) {
            ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [allowed_nssai]");
            goto end;
        }

        allowed_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(allowed_nssai_local, allowed_nssai) {
            if (!cJSON_IsObject(allowed_nssai_local)) {
                ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [allowed_nssai]");
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

    rejected_nssai = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "rejectedNSSAI");
    if (rejected_nssai) {
        cJSON *rejected_nssai_local = NULL;
        if (!cJSON_IsArray(rejected_nssai)) {
            ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [rejected_nssai]");
            goto end;
        }

        rejected_nssaiList = OpenAPI_list_create();

        cJSON_ArrayForEach(rejected_nssai_local, rejected_nssai) {
            if (!cJSON_IsObject(rejected_nssai_local)) {
                ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [rejected_nssai]");
                goto end;
            }
            OpenAPI_snssai_t *rejected_nssaiItem = OpenAPI_snssai_parseFromJSON(rejected_nssai_local);
            if (!rejected_nssaiItem) {
                ogs_error("No rejected_nssaiItem");
                goto end;
            }
            OpenAPI_list_add(rejected_nssaiList, rejected_nssaiItem);
        }
    }

    n_ssai_map_list = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "nSSAIMapList");
    if (n_ssai_map_list) {
        cJSON *n_ssai_map_list_local = NULL;
        if (!cJSON_IsArray(n_ssai_map_list)) {
            ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [n_ssai_map_list]");
            goto end;
        }

        n_ssai_map_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(n_ssai_map_list_local, n_ssai_map_list) {
            if (!cJSON_IsObject(n_ssai_map_list_local)) {
                ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [n_ssai_map_list]");
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

    alternative_nssai_map = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "alternativeNSSAIMap");
    if (alternative_nssai_map) {
        cJSON *alternative_nssai_map_local = NULL;
        if (!cJSON_IsArray(alternative_nssai_map)) {
            ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [alternative_nssai_map]");
            goto end;
        }

        alternative_nssai_mapList = OpenAPI_list_create();

        cJSON_ArrayForEach(alternative_nssai_map_local, alternative_nssai_map) {
            if (!cJSON_IsObject(alternative_nssai_map_local)) {
                ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [alternative_nssai_map]");
                goto end;
            }
            OpenAPI_alternative_nssai_map_t *alternative_nssai_mapItem = OpenAPI_alternative_nssai_map_parseFromJSON(alternative_nssai_map_local);
            if (!alternative_nssai_mapItem) {
                ogs_error("No alternative_nssai_mapItem");
                goto end;
            }
            OpenAPI_list_add(alternative_nssai_mapList, alternative_nssai_mapItem);
        }
    }

    amf_ue_ngap_id = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "amfUeNgapId");
    if (amf_ue_ngap_id) {
    if (!cJSON_IsNumber(amf_ue_ngap_id)) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [amf_ue_ngap_id]");
        goto end;
    }
    }

    ran_ue_ngap_id = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "ranUeNgapId");
    if (ran_ue_ngap_id) {
    if (!cJSON_IsNumber(ran_ue_ngap_id)) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [ran_ue_ngap_id]");
        goto end;
    }
    }

    ran_node_id = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "ranNodeId");
    if (ran_node_id) {
    ran_node_id_local_nonprim = OpenAPI_global_ran_node_id_parseFromJSON(ran_node_id);
    if (!ran_node_id_local_nonprim) {
        ogs_error("OpenAPI_global_ran_node_id_parseFromJSON failed [ran_node_id]");
        goto end;
    }
    }

    s_npnid = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "sNPNID");
    if (s_npnid) {
    s_npnid_local_nonprim = OpenAPI_plmn_id_nid_parseFromJSON(s_npnid);
    if (!s_npnid_local_nonprim) {
        ogs_error("OpenAPI_plmn_id_nid_parseFromJSON failed [s_npnid]");
        goto end;
    }
    }

    c_agid_list = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "cAGIDList");
    if (c_agid_list) {
        cJSON *c_agid_list_local = NULL;
        if (!cJSON_IsArray(c_agid_list)) {
            ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [c_agid_list]");
            goto end;
        }

        c_agid_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(c_agid_list_local, c_agid_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(c_agid_list_local)) {
                ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [c_agid_list]");
                goto end;
            }
            OpenAPI_list_add(c_agid_listList, ogs_strdup(c_agid_list_local->valuestring));
        }
    }

    satellite_access_indicator = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "satelliteAccessIndicator");
    if (satellite_access_indicator) {
    if (!cJSON_IsBool(satellite_access_indicator)) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [satellite_access_indicator]");
        goto end;
    }
    }

    service_level_aa = cJSON_GetObjectItemCaseSensitive(registration_charging_informationJSON, "serviceLevelAA");
    if (service_level_aa) {
    if (!cJSON_IsString(service_level_aa) && !cJSON_IsNull(service_level_aa)) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON() failed [service_level_aa]");
        goto end;
    }
    }

    registration_charging_information_local_var = OpenAPI_registration_charging_information_create (
        registration_messagetypeVariable,
        user_information ? user_information_local_nonprim : NULL,
        user_locationinfo ? user_locationinfo_local_nonprim : NULL,
        p_s_cell_information ? p_s_cell_information_local_nonprim : NULL,
        uetime_zone && !cJSON_IsNull(uetime_zone) ? ogs_strdup(uetime_zone->valuestring) : NULL,
        r_at_type ? r_at_typeVariable : 0,
        _5_gmm_capability && !cJSON_IsNull(_5_gmm_capability) ? ogs_strdup(_5_gmm_capability->valuestring) : NULL,
        m_ico_mode_indication ? m_ico_mode_indicationVariable : 0,
        sms_indication ? sms_indicationVariable : 0,
        tai_list ? tai_listList : NULL,
        service_area_restriction ? service_area_restrictionList : NULL,
        requested_nssai ? requested_nssaiList : NULL,
        allowed_nssai ? allowed_nssaiList : NULL,
        rejected_nssai ? rejected_nssaiList : NULL,
        n_ssai_map_list ? n_ssai_map_listList : NULL,
        alternative_nssai_map ? alternative_nssai_mapList : NULL,
        amf_ue_ngap_id ? true : false,
        amf_ue_ngap_id ? amf_ue_ngap_id->valuedouble : 0,
        ran_ue_ngap_id ? true : false,
        ran_ue_ngap_id ? ran_ue_ngap_id->valuedouble : 0,
        ran_node_id ? ran_node_id_local_nonprim : NULL,
        s_npnid ? s_npnid_local_nonprim : NULL,
        c_agid_list ? c_agid_listList : NULL,
        satellite_access_indicator ? true : false,
        satellite_access_indicator ? satellite_access_indicator->valueint : 0,
        service_level_aa && !cJSON_IsNull(service_level_aa) ? ogs_strdup(service_level_aa->valuestring) : NULL
    );

    return registration_charging_information_local_var;
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
    if (tai_listList) {
        OpenAPI_list_for_each(tai_listList, node) {
            OpenAPI_tai_free(node->data);
        }
        OpenAPI_list_free(tai_listList);
        tai_listList = NULL;
    }
    if (service_area_restrictionList) {
        OpenAPI_list_for_each(service_area_restrictionList, node) {
            OpenAPI_service_area_restriction_free(node->data);
        }
        OpenAPI_list_free(service_area_restrictionList);
        service_area_restrictionList = NULL;
    }
    if (requested_nssaiList) {
        OpenAPI_list_for_each(requested_nssaiList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(requested_nssaiList);
        requested_nssaiList = NULL;
    }
    if (allowed_nssaiList) {
        OpenAPI_list_for_each(allowed_nssaiList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(allowed_nssaiList);
        allowed_nssaiList = NULL;
    }
    if (rejected_nssaiList) {
        OpenAPI_list_for_each(rejected_nssaiList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(rejected_nssaiList);
        rejected_nssaiList = NULL;
    }
    if (n_ssai_map_listList) {
        OpenAPI_list_for_each(n_ssai_map_listList, node) {
            OpenAPI_nssai_map_free(node->data);
        }
        OpenAPI_list_free(n_ssai_map_listList);
        n_ssai_map_listList = NULL;
    }
    if (alternative_nssai_mapList) {
        OpenAPI_list_for_each(alternative_nssai_mapList, node) {
            OpenAPI_alternative_nssai_map_free(node->data);
        }
        OpenAPI_list_free(alternative_nssai_mapList);
        alternative_nssai_mapList = NULL;
    }
    if (ran_node_id_local_nonprim) {
        OpenAPI_global_ran_node_id_free(ran_node_id_local_nonprim);
        ran_node_id_local_nonprim = NULL;
    }
    if (s_npnid_local_nonprim) {
        OpenAPI_plmn_id_nid_free(s_npnid_local_nonprim);
        s_npnid_local_nonprim = NULL;
    }
    if (c_agid_listList) {
        OpenAPI_list_for_each(c_agid_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(c_agid_listList);
        c_agid_listList = NULL;
    }
    return NULL;
}

OpenAPI_registration_charging_information_t *OpenAPI_registration_charging_information_copy(OpenAPI_registration_charging_information_t *dst, OpenAPI_registration_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_registration_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_registration_charging_information_convertToJSON() failed");
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

    OpenAPI_registration_charging_information_free(dst);
    dst = OpenAPI_registration_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

