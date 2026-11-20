
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pdu_session_charging_information.h"

OpenAPI_pdu_session_charging_information_t *OpenAPI_pdu_session_charging_information_create(
    bool is_charging_id,
    int charging_id,
    char *s_m_fcharging_id,
    bool is_home_provided_charging_id,
    int home_provided_charging_id,
    char *s_mf_home_provided_charging_id,
    OpenAPI_user_information_t *user_information,
    OpenAPI_charging_user_location_t *user_locationinfo,
    bool is_i_ms_session_information_null,
    OpenAPI_call_info_t *i_ms_session_information,
    OpenAPI_charging_user_location_t *m_apdu_non3_gpp_user_location_info,
    char *non3_gpp_user_location_time,
    char *m_apdu_non3_gpp_user_location_time,
    OpenAPI_list_t* presence_reporting_area_information,
    char *uetime_zone,
    OpenAPI_pdu_session_information_t *pdu_session_information,
    bool is_unit_count_inactivity_timer,
    int unit_count_inactivity_timer,
    OpenAPI_ran_secondary_rat_usage_report_t *r_an_secondary_rat_usage_report
)
{
    OpenAPI_pdu_session_charging_information_t *pdu_session_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_pdu_session_charging_information_t));
    ogs_assert(pdu_session_charging_information_local_var);

    pdu_session_charging_information_local_var->is_charging_id = is_charging_id;
    pdu_session_charging_information_local_var->charging_id = charging_id;
    pdu_session_charging_information_local_var->s_m_fcharging_id = s_m_fcharging_id;
    pdu_session_charging_information_local_var->is_home_provided_charging_id = is_home_provided_charging_id;
    pdu_session_charging_information_local_var->home_provided_charging_id = home_provided_charging_id;
    pdu_session_charging_information_local_var->s_mf_home_provided_charging_id = s_mf_home_provided_charging_id;
    pdu_session_charging_information_local_var->user_information = user_information;
    pdu_session_charging_information_local_var->user_locationinfo = user_locationinfo;
    pdu_session_charging_information_local_var->is_i_ms_session_information_null = is_i_ms_session_information_null;
    pdu_session_charging_information_local_var->i_ms_session_information = i_ms_session_information;
    pdu_session_charging_information_local_var->m_apdu_non3_gpp_user_location_info = m_apdu_non3_gpp_user_location_info;
    pdu_session_charging_information_local_var->non3_gpp_user_location_time = non3_gpp_user_location_time;
    pdu_session_charging_information_local_var->m_apdu_non3_gpp_user_location_time = m_apdu_non3_gpp_user_location_time;
    pdu_session_charging_information_local_var->presence_reporting_area_information = presence_reporting_area_information;
    pdu_session_charging_information_local_var->uetime_zone = uetime_zone;
    pdu_session_charging_information_local_var->pdu_session_information = pdu_session_information;
    pdu_session_charging_information_local_var->is_unit_count_inactivity_timer = is_unit_count_inactivity_timer;
    pdu_session_charging_information_local_var->unit_count_inactivity_timer = unit_count_inactivity_timer;
    pdu_session_charging_information_local_var->r_an_secondary_rat_usage_report = r_an_secondary_rat_usage_report;

    return pdu_session_charging_information_local_var;
}

void OpenAPI_pdu_session_charging_information_free(OpenAPI_pdu_session_charging_information_t *pdu_session_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == pdu_session_charging_information) {
        return;
    }
    if (pdu_session_charging_information->s_m_fcharging_id) {
        ogs_free(pdu_session_charging_information->s_m_fcharging_id);
        pdu_session_charging_information->s_m_fcharging_id = NULL;
    }
    if (pdu_session_charging_information->s_mf_home_provided_charging_id) {
        ogs_free(pdu_session_charging_information->s_mf_home_provided_charging_id);
        pdu_session_charging_information->s_mf_home_provided_charging_id = NULL;
    }
    if (pdu_session_charging_information->user_information) {
        OpenAPI_user_information_free(pdu_session_charging_information->user_information);
        pdu_session_charging_information->user_information = NULL;
    }
    if (pdu_session_charging_information->user_locationinfo) {
        OpenAPI_charging_user_location_free(pdu_session_charging_information->user_locationinfo);
        pdu_session_charging_information->user_locationinfo = NULL;
    }
    if (pdu_session_charging_information->i_ms_session_information) {
        OpenAPI_call_info_free(pdu_session_charging_information->i_ms_session_information);
        pdu_session_charging_information->i_ms_session_information = NULL;
    }
    if (pdu_session_charging_information->m_apdu_non3_gpp_user_location_info) {
        OpenAPI_charging_user_location_free(pdu_session_charging_information->m_apdu_non3_gpp_user_location_info);
        pdu_session_charging_information->m_apdu_non3_gpp_user_location_info = NULL;
    }
    if (pdu_session_charging_information->non3_gpp_user_location_time) {
        ogs_free(pdu_session_charging_information->non3_gpp_user_location_time);
        pdu_session_charging_information->non3_gpp_user_location_time = NULL;
    }
    if (pdu_session_charging_information->m_apdu_non3_gpp_user_location_time) {
        ogs_free(pdu_session_charging_information->m_apdu_non3_gpp_user_location_time);
        pdu_session_charging_information->m_apdu_non3_gpp_user_location_time = NULL;
    }
    if (pdu_session_charging_information->presence_reporting_area_information) {
        OpenAPI_list_for_each(pdu_session_charging_information->presence_reporting_area_information, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(pdu_session_charging_information->presence_reporting_area_information);
        pdu_session_charging_information->presence_reporting_area_information = NULL;
    }
    if (pdu_session_charging_information->uetime_zone) {
        ogs_free(pdu_session_charging_information->uetime_zone);
        pdu_session_charging_information->uetime_zone = NULL;
    }
    if (pdu_session_charging_information->pdu_session_information) {
        OpenAPI_pdu_session_information_free(pdu_session_charging_information->pdu_session_information);
        pdu_session_charging_information->pdu_session_information = NULL;
    }
    if (pdu_session_charging_information->r_an_secondary_rat_usage_report) {
        OpenAPI_ran_secondary_rat_usage_report_free(pdu_session_charging_information->r_an_secondary_rat_usage_report);
        pdu_session_charging_information->r_an_secondary_rat_usage_report = NULL;
    }
    ogs_free(pdu_session_charging_information);
}

cJSON *OpenAPI_pdu_session_charging_information_convertToJSON(OpenAPI_pdu_session_charging_information_t *pdu_session_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (pdu_session_charging_information == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [PDUSessionChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (pdu_session_charging_information->is_charging_id) {
    if (cJSON_AddNumberToObject(item, "chargingId", pdu_session_charging_information->charging_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [charging_id]");
        goto end;
    }
    }

    if (pdu_session_charging_information->s_m_fcharging_id) {
    if (cJSON_AddStringToObject(item, "sMFchargingId", pdu_session_charging_information->s_m_fcharging_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [s_m_fcharging_id]");
        goto end;
    }
    }

    if (pdu_session_charging_information->is_home_provided_charging_id) {
    if (cJSON_AddNumberToObject(item, "homeProvidedChargingId", pdu_session_charging_information->home_provided_charging_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [home_provided_charging_id]");
        goto end;
    }
    }

    if (pdu_session_charging_information->s_mf_home_provided_charging_id) {
    if (cJSON_AddStringToObject(item, "sMFHomeProvidedChargingId", pdu_session_charging_information->s_mf_home_provided_charging_id) == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [s_mf_home_provided_charging_id]");
        goto end;
    }
    }

    if (pdu_session_charging_information->user_information) {
    cJSON *user_information_local_JSON = OpenAPI_user_information_convertToJSON(pdu_session_charging_information->user_information);
    if (user_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [user_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userInformation", user_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [user_information]");
        goto end;
    }
    }

    if (pdu_session_charging_information->user_locationinfo) {
    cJSON *user_locationinfo_local_JSON = OpenAPI_charging_user_location_convertToJSON(pdu_session_charging_information->user_locationinfo);
    if (user_locationinfo_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    cJSON_AddItemToObject(item, "userLocationinfo", user_locationinfo_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [user_locationinfo]");
        goto end;
    }
    }

    if (pdu_session_charging_information->i_ms_session_information) {
    cJSON *i_ms_session_information_local_JSON = OpenAPI_call_info_convertToJSON(pdu_session_charging_information->i_ms_session_information);
    if (i_ms_session_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [i_ms_session_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "iMSSessionInformation", i_ms_session_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [i_ms_session_information]");
        goto end;
    }
    } else if (pdu_session_charging_information->is_i_ms_session_information_null) {
        if (cJSON_AddNullToObject(item, "iMSSessionInformation") == NULL) {
            ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [i_ms_session_information]");
            goto end;
        }
    }

    if (pdu_session_charging_information->m_apdu_non3_gpp_user_location_info) {
    cJSON *m_apdu_non3_gpp_user_location_info_local_JSON = OpenAPI_charging_user_location_convertToJSON(pdu_session_charging_information->m_apdu_non3_gpp_user_location_info);
    if (m_apdu_non3_gpp_user_location_info_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [m_apdu_non3_gpp_user_location_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mAPDUNon3GPPUserLocationInfo", m_apdu_non3_gpp_user_location_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [m_apdu_non3_gpp_user_location_info]");
        goto end;
    }
    }

    if (pdu_session_charging_information->non3_gpp_user_location_time) {
    if (cJSON_AddStringToObject(item, "non3GPPUserLocationTime", pdu_session_charging_information->non3_gpp_user_location_time) == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [non3_gpp_user_location_time]");
        goto end;
    }
    }

    if (pdu_session_charging_information->m_apdu_non3_gpp_user_location_time) {
    if (cJSON_AddStringToObject(item, "mAPDUNon3GPPUserLocationTime", pdu_session_charging_information->m_apdu_non3_gpp_user_location_time) == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [m_apdu_non3_gpp_user_location_time]");
        goto end;
    }
    }

    if (pdu_session_charging_information->presence_reporting_area_information) {
    cJSON *presence_reporting_area_information = cJSON_AddObjectToObject(item, "presenceReportingAreaInformation");
    if (presence_reporting_area_information == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [presence_reporting_area_information]");
        goto end;
    }
    cJSON *localMapObject = presence_reporting_area_information;
    if (pdu_session_charging_information->presence_reporting_area_information) {
        OpenAPI_list_for_each(pdu_session_charging_information->presence_reporting_area_information, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            if (localKeyValue == NULL) {
                ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [presence_reporting_area_information]");
                goto end;
            }
            if (localKeyValue->key == NULL) {
                ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [presence_reporting_area_information]");
                goto end;
            }
            cJSON *itemLocal = localKeyValue->value ?
                OpenAPI_presence_info_convertToJSON(localKeyValue->value) :
                cJSON_CreateNull();
            if (itemLocal == NULL) {
                ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [inner]");
                goto end;
            }
            cJSON_AddItemToObject(localMapObject, localKeyValue->key, itemLocal);
        }
    }
    }

    if (pdu_session_charging_information->uetime_zone) {
    if (cJSON_AddStringToObject(item, "uetimeZone", pdu_session_charging_information->uetime_zone) == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [uetime_zone]");
        goto end;
    }
    }

    if (pdu_session_charging_information->pdu_session_information) {
    cJSON *pdu_session_information_local_JSON = OpenAPI_pdu_session_information_convertToJSON(pdu_session_charging_information->pdu_session_information);
    if (pdu_session_information_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [pdu_session_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSessionInformation", pdu_session_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [pdu_session_information]");
        goto end;
    }
    }

    if (pdu_session_charging_information->is_unit_count_inactivity_timer) {
    if (cJSON_AddNumberToObject(item, "unitCountInactivityTimer", pdu_session_charging_information->unit_count_inactivity_timer) == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [unit_count_inactivity_timer]");
        goto end;
    }
    }

    if (pdu_session_charging_information->r_an_secondary_rat_usage_report) {
    cJSON *r_an_secondary_rat_usage_report_local_JSON = OpenAPI_ran_secondary_rat_usage_report_convertToJSON(pdu_session_charging_information->r_an_secondary_rat_usage_report);
    if (r_an_secondary_rat_usage_report_local_JSON == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [r_an_secondary_rat_usage_report]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rANSecondaryRATUsageReport", r_an_secondary_rat_usage_report_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed [r_an_secondary_rat_usage_report]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_pdu_session_charging_information_t *OpenAPI_pdu_session_charging_information_parseFromJSON(cJSON *pdu_session_charging_informationJSON)
{
    OpenAPI_pdu_session_charging_information_t *pdu_session_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *charging_id = NULL;
    cJSON *s_m_fcharging_id = NULL;
    cJSON *home_provided_charging_id = NULL;
    cJSON *s_mf_home_provided_charging_id = NULL;
    cJSON *user_information = NULL;
    OpenAPI_user_information_t *user_information_local_nonprim = NULL;
    cJSON *user_locationinfo = NULL;
    OpenAPI_charging_user_location_t *user_locationinfo_local_nonprim = NULL;
    cJSON *i_ms_session_information = NULL;
    OpenAPI_call_info_t *i_ms_session_information_local_nonprim = NULL;
    cJSON *m_apdu_non3_gpp_user_location_info = NULL;
    OpenAPI_charging_user_location_t *m_apdu_non3_gpp_user_location_info_local_nonprim = NULL;
    cJSON *non3_gpp_user_location_time = NULL;
    cJSON *m_apdu_non3_gpp_user_location_time = NULL;
    cJSON *presence_reporting_area_information = NULL;
    OpenAPI_list_t *presence_reporting_area_informationList = NULL;
    cJSON *uetime_zone = NULL;
    cJSON *pdu_session_information = NULL;
    OpenAPI_pdu_session_information_t *pdu_session_information_local_nonprim = NULL;
    cJSON *unit_count_inactivity_timer = NULL;
    cJSON *r_an_secondary_rat_usage_report = NULL;
    OpenAPI_ran_secondary_rat_usage_report_t *r_an_secondary_rat_usage_report_local_nonprim = NULL;
    charging_id = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "chargingId");
    if (charging_id) {
    if (!cJSON_IsNumber(charging_id)) {
        ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON() failed [charging_id]");
        goto end;
    }
    }

    s_m_fcharging_id = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "sMFchargingId");
    if (s_m_fcharging_id) {
    if (!cJSON_IsString(s_m_fcharging_id) && !cJSON_IsNull(s_m_fcharging_id)) {
        ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON() failed [s_m_fcharging_id]");
        goto end;
    }
    }

    home_provided_charging_id = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "homeProvidedChargingId");
    if (home_provided_charging_id) {
    if (!cJSON_IsNumber(home_provided_charging_id)) {
        ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON() failed [home_provided_charging_id]");
        goto end;
    }
    }

    s_mf_home_provided_charging_id = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "sMFHomeProvidedChargingId");
    if (s_mf_home_provided_charging_id) {
    if (!cJSON_IsString(s_mf_home_provided_charging_id) && !cJSON_IsNull(s_mf_home_provided_charging_id)) {
        ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON() failed [s_mf_home_provided_charging_id]");
        goto end;
    }
    }

    user_information = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "userInformation");
    if (user_information) {
    user_information_local_nonprim = OpenAPI_user_information_parseFromJSON(user_information);
    if (!user_information_local_nonprim) {
        ogs_error("OpenAPI_user_information_parseFromJSON failed [user_information]");
        goto end;
    }
    }

    user_locationinfo = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "userLocationinfo");
    if (user_locationinfo) {
    user_locationinfo_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(user_locationinfo);
    if (!user_locationinfo_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [user_locationinfo]");
        goto end;
    }
    }

    i_ms_session_information = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "iMSSessionInformation");
    if (i_ms_session_information) {
    if (!cJSON_IsNull(i_ms_session_information)) {
    i_ms_session_information_local_nonprim = OpenAPI_call_info_parseFromJSON(i_ms_session_information);
    if (!i_ms_session_information_local_nonprim) {
        ogs_error("OpenAPI_call_info_parseFromJSON failed [i_ms_session_information]");
        goto end;
    }
    }
    }

    m_apdu_non3_gpp_user_location_info = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "mAPDUNon3GPPUserLocationInfo");
    if (m_apdu_non3_gpp_user_location_info) {
    m_apdu_non3_gpp_user_location_info_local_nonprim = OpenAPI_charging_user_location_parseFromJSON(m_apdu_non3_gpp_user_location_info);
    if (!m_apdu_non3_gpp_user_location_info_local_nonprim) {
        ogs_error("OpenAPI_charging_user_location_parseFromJSON failed [m_apdu_non3_gpp_user_location_info]");
        goto end;
    }
    }

    non3_gpp_user_location_time = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "non3GPPUserLocationTime");
    if (non3_gpp_user_location_time) {
    if (!cJSON_IsString(non3_gpp_user_location_time) && !cJSON_IsNull(non3_gpp_user_location_time)) {
        ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON() failed [non3_gpp_user_location_time]");
        goto end;
    }
    }

    m_apdu_non3_gpp_user_location_time = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "mAPDUNon3GPPUserLocationTime");
    if (m_apdu_non3_gpp_user_location_time) {
    if (!cJSON_IsString(m_apdu_non3_gpp_user_location_time) && !cJSON_IsNull(m_apdu_non3_gpp_user_location_time)) {
        ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON() failed [m_apdu_non3_gpp_user_location_time]");
        goto end;
    }
    }

    presence_reporting_area_information = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "presenceReportingAreaInformation");
    if (presence_reporting_area_information) {
        cJSON *presence_reporting_area_information_local_map = NULL;
        if (!cJSON_IsObject(presence_reporting_area_information) && !cJSON_IsNull(presence_reporting_area_information)) {
            ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON() failed [presence_reporting_area_information]");
            goto end;
        }
        if (cJSON_IsObject(presence_reporting_area_information)) {
            presence_reporting_area_informationList = OpenAPI_list_create();
            OpenAPI_map_t *localMapKeyPair = NULL;
            cJSON_ArrayForEach(presence_reporting_area_information_local_map, presence_reporting_area_information) {
                cJSON *localMapObject = presence_reporting_area_information_local_map;
                if (cJSON_IsObject(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(
                        ogs_strdup(localMapObject->string), OpenAPI_presence_info_parseFromJSON(localMapObject));
                } else if (cJSON_IsNull(localMapObject)) {
                    localMapKeyPair = OpenAPI_map_create(ogs_strdup(localMapObject->string), NULL);
                } else {
                    ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON() failed [inner]");
                    goto end;
                }
                if (localMapKeyPair == NULL) {
                    ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON() failed [presence_reporting_area_information]");
                    goto end;
                }
                OpenAPI_list_add(presence_reporting_area_informationList, localMapKeyPair);
            }
        }
    }

    uetime_zone = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "uetimeZone");
    if (uetime_zone) {
    if (!cJSON_IsString(uetime_zone) && !cJSON_IsNull(uetime_zone)) {
        ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON() failed [uetime_zone]");
        goto end;
    }
    }

    pdu_session_information = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "pduSessionInformation");
    if (pdu_session_information) {
    pdu_session_information_local_nonprim = OpenAPI_pdu_session_information_parseFromJSON(pdu_session_information);
    if (!pdu_session_information_local_nonprim) {
        ogs_error("OpenAPI_pdu_session_information_parseFromJSON failed [pdu_session_information]");
        goto end;
    }
    }

    unit_count_inactivity_timer = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "unitCountInactivityTimer");
    if (unit_count_inactivity_timer) {
    if (!cJSON_IsNumber(unit_count_inactivity_timer)) {
        ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON() failed [unit_count_inactivity_timer]");
        goto end;
    }
    }

    r_an_secondary_rat_usage_report = cJSON_GetObjectItemCaseSensitive(pdu_session_charging_informationJSON, "rANSecondaryRATUsageReport");
    if (r_an_secondary_rat_usage_report) {
    r_an_secondary_rat_usage_report_local_nonprim = OpenAPI_ran_secondary_rat_usage_report_parseFromJSON(r_an_secondary_rat_usage_report);
    if (!r_an_secondary_rat_usage_report_local_nonprim) {
        ogs_error("OpenAPI_ran_secondary_rat_usage_report_parseFromJSON failed [r_an_secondary_rat_usage_report]");
        goto end;
    }
    }

    pdu_session_charging_information_local_var = OpenAPI_pdu_session_charging_information_create (
        charging_id ? true : false,
        charging_id ? charging_id->valuedouble : 0,
        s_m_fcharging_id && !cJSON_IsNull(s_m_fcharging_id) ? ogs_strdup(s_m_fcharging_id->valuestring) : NULL,
        home_provided_charging_id ? true : false,
        home_provided_charging_id ? home_provided_charging_id->valuedouble : 0,
        s_mf_home_provided_charging_id && !cJSON_IsNull(s_mf_home_provided_charging_id) ? ogs_strdup(s_mf_home_provided_charging_id->valuestring) : NULL,
        user_information ? user_information_local_nonprim : NULL,
        user_locationinfo ? user_locationinfo_local_nonprim : NULL,
        i_ms_session_information && cJSON_IsNull(i_ms_session_information) ? true : false,
        i_ms_session_information ? i_ms_session_information_local_nonprim : NULL,
        m_apdu_non3_gpp_user_location_info ? m_apdu_non3_gpp_user_location_info_local_nonprim : NULL,
        non3_gpp_user_location_time && !cJSON_IsNull(non3_gpp_user_location_time) ? ogs_strdup(non3_gpp_user_location_time->valuestring) : NULL,
        m_apdu_non3_gpp_user_location_time && !cJSON_IsNull(m_apdu_non3_gpp_user_location_time) ? ogs_strdup(m_apdu_non3_gpp_user_location_time->valuestring) : NULL,
        presence_reporting_area_information ? presence_reporting_area_informationList : NULL,
        uetime_zone && !cJSON_IsNull(uetime_zone) ? ogs_strdup(uetime_zone->valuestring) : NULL,
        pdu_session_information ? pdu_session_information_local_nonprim : NULL,
        unit_count_inactivity_timer ? true : false,
        unit_count_inactivity_timer ? unit_count_inactivity_timer->valuedouble : 0,
        r_an_secondary_rat_usage_report ? r_an_secondary_rat_usage_report_local_nonprim : NULL
    );

    return pdu_session_charging_information_local_var;
end:
    if (user_information_local_nonprim) {
        OpenAPI_user_information_free(user_information_local_nonprim);
        user_information_local_nonprim = NULL;
    }
    if (user_locationinfo_local_nonprim) {
        OpenAPI_charging_user_location_free(user_locationinfo_local_nonprim);
        user_locationinfo_local_nonprim = NULL;
    }
    if (i_ms_session_information_local_nonprim) {
        OpenAPI_call_info_free(i_ms_session_information_local_nonprim);
        i_ms_session_information_local_nonprim = NULL;
    }
    if (m_apdu_non3_gpp_user_location_info_local_nonprim) {
        OpenAPI_charging_user_location_free(m_apdu_non3_gpp_user_location_info_local_nonprim);
        m_apdu_non3_gpp_user_location_info_local_nonprim = NULL;
    }
    if (presence_reporting_area_informationList) {
        OpenAPI_list_for_each(presence_reporting_area_informationList, node) {
            OpenAPI_map_t *localKeyValue = (OpenAPI_map_t*)node->data;
            ogs_free(localKeyValue->key);
            OpenAPI_presence_info_free(localKeyValue->value);
            OpenAPI_map_free(localKeyValue);
        }
        OpenAPI_list_free(presence_reporting_area_informationList);
        presence_reporting_area_informationList = NULL;
    }
    if (pdu_session_information_local_nonprim) {
        OpenAPI_pdu_session_information_free(pdu_session_information_local_nonprim);
        pdu_session_information_local_nonprim = NULL;
    }
    if (r_an_secondary_rat_usage_report_local_nonprim) {
        OpenAPI_ran_secondary_rat_usage_report_free(r_an_secondary_rat_usage_report_local_nonprim);
        r_an_secondary_rat_usage_report_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_pdu_session_charging_information_t *OpenAPI_pdu_session_charging_information_copy(OpenAPI_pdu_session_charging_information_t *dst, OpenAPI_pdu_session_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_pdu_session_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_pdu_session_charging_information_convertToJSON() failed");
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

    OpenAPI_pdu_session_charging_information_free(dst);
    dst = OpenAPI_pdu_session_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

