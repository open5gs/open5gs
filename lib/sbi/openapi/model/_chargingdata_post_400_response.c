
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "_chargingdata_post_400_response.h"

OpenAPI__chargingdata_post_400_response_t *OpenAPI__chargingdata_post_400_response_create(
    char *type,
    char *title,
    bool is_status,
    int status,
    char *detail,
    char *instance,
    char *cause,
    OpenAPI_list_t *invalid_params,
    char *supported_features,
    OpenAPI_access_token_err_t *access_token_error,
    OpenAPI_access_token_req_t *access_token_request,
    char *nrf_id,
    OpenAPI_list_t *supported_api_versions,
    OpenAPI_no_profile_match_info_t *no_profile_match_info,
    char *invocation_time_stamp,
    bool is_invocation_sequence_number,
    int invocation_sequence_number,
    OpenAPI_invocation_result_t *invocation_result,
    OpenAPI_session_failover_e session_failover,
    OpenAPI_list_t *multiple_unit_information,
    OpenAPI_list_t *triggers,
    OpenAPI_pdu_session_charging_information_t *p_du_session_charging_information,
    OpenAPI_roaming_qbc_information_t *roaming_qbc_information,
    OpenAPI_location_reporting_charging_information_t *location_reporting_charging_information,
    OpenAPI_mbs_session_charging_information_t *m_bs_session_charging_information,
    OpenAPI_inter_chf_information_t *inter_chf_information
)
{
    OpenAPI__chargingdata_post_400_response_t *_chargingdata_post_400_response_local_var = ogs_malloc(sizeof(OpenAPI__chargingdata_post_400_response_t));
    ogs_assert(_chargingdata_post_400_response_local_var);

    _chargingdata_post_400_response_local_var->type = type;
    _chargingdata_post_400_response_local_var->title = title;
    _chargingdata_post_400_response_local_var->is_status = is_status;
    _chargingdata_post_400_response_local_var->status = status;
    _chargingdata_post_400_response_local_var->detail = detail;
    _chargingdata_post_400_response_local_var->instance = instance;
    _chargingdata_post_400_response_local_var->cause = cause;
    _chargingdata_post_400_response_local_var->invalid_params = invalid_params;
    _chargingdata_post_400_response_local_var->supported_features = supported_features;
    _chargingdata_post_400_response_local_var->access_token_error = access_token_error;
    _chargingdata_post_400_response_local_var->access_token_request = access_token_request;
    _chargingdata_post_400_response_local_var->nrf_id = nrf_id;
    _chargingdata_post_400_response_local_var->supported_api_versions = supported_api_versions;
    _chargingdata_post_400_response_local_var->no_profile_match_info = no_profile_match_info;
    _chargingdata_post_400_response_local_var->invocation_time_stamp = invocation_time_stamp;
    _chargingdata_post_400_response_local_var->is_invocation_sequence_number = is_invocation_sequence_number;
    _chargingdata_post_400_response_local_var->invocation_sequence_number = invocation_sequence_number;
    _chargingdata_post_400_response_local_var->invocation_result = invocation_result;
    _chargingdata_post_400_response_local_var->session_failover = session_failover;
    _chargingdata_post_400_response_local_var->multiple_unit_information = multiple_unit_information;
    _chargingdata_post_400_response_local_var->triggers = triggers;
    _chargingdata_post_400_response_local_var->p_du_session_charging_information = p_du_session_charging_information;
    _chargingdata_post_400_response_local_var->roaming_qbc_information = roaming_qbc_information;
    _chargingdata_post_400_response_local_var->location_reporting_charging_information = location_reporting_charging_information;
    _chargingdata_post_400_response_local_var->m_bs_session_charging_information = m_bs_session_charging_information;
    _chargingdata_post_400_response_local_var->inter_chf_information = inter_chf_information;

    return _chargingdata_post_400_response_local_var;
}

void OpenAPI__chargingdata_post_400_response_free(OpenAPI__chargingdata_post_400_response_t *_chargingdata_post_400_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == _chargingdata_post_400_response) {
        return;
    }
    if (_chargingdata_post_400_response->type) {
        ogs_free(_chargingdata_post_400_response->type);
        _chargingdata_post_400_response->type = NULL;
    }
    if (_chargingdata_post_400_response->title) {
        ogs_free(_chargingdata_post_400_response->title);
        _chargingdata_post_400_response->title = NULL;
    }
    if (_chargingdata_post_400_response->detail) {
        ogs_free(_chargingdata_post_400_response->detail);
        _chargingdata_post_400_response->detail = NULL;
    }
    if (_chargingdata_post_400_response->instance) {
        ogs_free(_chargingdata_post_400_response->instance);
        _chargingdata_post_400_response->instance = NULL;
    }
    if (_chargingdata_post_400_response->cause) {
        ogs_free(_chargingdata_post_400_response->cause);
        _chargingdata_post_400_response->cause = NULL;
    }
    if (_chargingdata_post_400_response->invalid_params) {
        OpenAPI_list_for_each(_chargingdata_post_400_response->invalid_params, node) {
            OpenAPI_invalid_param_free(node->data);
        }
        OpenAPI_list_free(_chargingdata_post_400_response->invalid_params);
        _chargingdata_post_400_response->invalid_params = NULL;
    }
    if (_chargingdata_post_400_response->supported_features) {
        ogs_free(_chargingdata_post_400_response->supported_features);
        _chargingdata_post_400_response->supported_features = NULL;
    }
    if (_chargingdata_post_400_response->access_token_error) {
        OpenAPI_access_token_err_free(_chargingdata_post_400_response->access_token_error);
        _chargingdata_post_400_response->access_token_error = NULL;
    }
    if (_chargingdata_post_400_response->access_token_request) {
        OpenAPI_access_token_req_free(_chargingdata_post_400_response->access_token_request);
        _chargingdata_post_400_response->access_token_request = NULL;
    }
    if (_chargingdata_post_400_response->nrf_id) {
        ogs_free(_chargingdata_post_400_response->nrf_id);
        _chargingdata_post_400_response->nrf_id = NULL;
    }
    if (_chargingdata_post_400_response->supported_api_versions) {
        OpenAPI_list_for_each(_chargingdata_post_400_response->supported_api_versions, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(_chargingdata_post_400_response->supported_api_versions);
        _chargingdata_post_400_response->supported_api_versions = NULL;
    }
    if (_chargingdata_post_400_response->no_profile_match_info) {
        OpenAPI_no_profile_match_info_free(_chargingdata_post_400_response->no_profile_match_info);
        _chargingdata_post_400_response->no_profile_match_info = NULL;
    }
    if (_chargingdata_post_400_response->invocation_time_stamp) {
        ogs_free(_chargingdata_post_400_response->invocation_time_stamp);
        _chargingdata_post_400_response->invocation_time_stamp = NULL;
    }
    if (_chargingdata_post_400_response->invocation_result) {
        OpenAPI_invocation_result_free(_chargingdata_post_400_response->invocation_result);
        _chargingdata_post_400_response->invocation_result = NULL;
    }
    if (_chargingdata_post_400_response->multiple_unit_information) {
        OpenAPI_list_for_each(_chargingdata_post_400_response->multiple_unit_information, node) {
            OpenAPI_multiple_unit_information_free(node->data);
        }
        OpenAPI_list_free(_chargingdata_post_400_response->multiple_unit_information);
        _chargingdata_post_400_response->multiple_unit_information = NULL;
    }
    if (_chargingdata_post_400_response->triggers) {
        OpenAPI_list_for_each(_chargingdata_post_400_response->triggers, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(_chargingdata_post_400_response->triggers);
        _chargingdata_post_400_response->triggers = NULL;
    }
    if (_chargingdata_post_400_response->p_du_session_charging_information) {
        OpenAPI_pdu_session_charging_information_free(_chargingdata_post_400_response->p_du_session_charging_information);
        _chargingdata_post_400_response->p_du_session_charging_information = NULL;
    }
    if (_chargingdata_post_400_response->roaming_qbc_information) {
        OpenAPI_roaming_qbc_information_free(_chargingdata_post_400_response->roaming_qbc_information);
        _chargingdata_post_400_response->roaming_qbc_information = NULL;
    }
    if (_chargingdata_post_400_response->location_reporting_charging_information) {
        OpenAPI_location_reporting_charging_information_free(_chargingdata_post_400_response->location_reporting_charging_information);
        _chargingdata_post_400_response->location_reporting_charging_information = NULL;
    }
    if (_chargingdata_post_400_response->m_bs_session_charging_information) {
        OpenAPI_mbs_session_charging_information_free(_chargingdata_post_400_response->m_bs_session_charging_information);
        _chargingdata_post_400_response->m_bs_session_charging_information = NULL;
    }
    if (_chargingdata_post_400_response->inter_chf_information) {
        OpenAPI_inter_chf_information_free(_chargingdata_post_400_response->inter_chf_information);
        _chargingdata_post_400_response->inter_chf_information = NULL;
    }
    ogs_free(_chargingdata_post_400_response);
}

cJSON *OpenAPI__chargingdata_post_400_response_convertToJSON(OpenAPI__chargingdata_post_400_response_t *_chargingdata_post_400_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (_chargingdata_post_400_response == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [_chargingdata_post_400_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (_chargingdata_post_400_response->type) {
    if (cJSON_AddStringToObject(item, "type", _chargingdata_post_400_response->type) == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [type]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->title) {
    if (cJSON_AddStringToObject(item, "title", _chargingdata_post_400_response->title) == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [title]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->is_status) {
    if (cJSON_AddNumberToObject(item, "status", _chargingdata_post_400_response->status) == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [status]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->detail) {
    if (cJSON_AddStringToObject(item, "detail", _chargingdata_post_400_response->detail) == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [detail]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->instance) {
    if (cJSON_AddStringToObject(item, "instance", _chargingdata_post_400_response->instance) == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [instance]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->cause) {
    if (cJSON_AddStringToObject(item, "cause", _chargingdata_post_400_response->cause) == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [cause]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->invalid_params) {
    cJSON *invalid_paramsList = cJSON_AddArrayToObject(item, "invalidParams");
    if (invalid_paramsList == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [invalid_params]");
        goto end;
    }
    OpenAPI_list_for_each(_chargingdata_post_400_response->invalid_params, node) {
        cJSON *itemLocal = OpenAPI_invalid_param_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [invalid_params]");
            goto end;
        }
        cJSON_AddItemToArray(invalid_paramsList, itemLocal);
    }
    }

    if (_chargingdata_post_400_response->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", _chargingdata_post_400_response->supported_features) == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->access_token_error) {
    cJSON *access_token_error_local_JSON = OpenAPI_access_token_err_convertToJSON(_chargingdata_post_400_response->access_token_error);
    if (access_token_error_local_JSON == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [access_token_error]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accessTokenError", access_token_error_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [access_token_error]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->access_token_request) {
    cJSON *access_token_request_local_JSON = OpenAPI_access_token_req_convertToJSON(_chargingdata_post_400_response->access_token_request);
    if (access_token_request_local_JSON == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [access_token_request]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accessTokenRequest", access_token_request_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [access_token_request]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->nrf_id) {
    if (cJSON_AddStringToObject(item, "nrfId", _chargingdata_post_400_response->nrf_id) == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [nrf_id]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->supported_api_versions) {
    cJSON *supported_api_versionsList = cJSON_AddArrayToObject(item, "supportedApiVersions");
    if (supported_api_versionsList == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [supported_api_versions]");
        goto end;
    }
    OpenAPI_list_for_each(_chargingdata_post_400_response->supported_api_versions, node) {
        if (cJSON_AddStringToObject(supported_api_versionsList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [supported_api_versions]");
            goto end;
        }
    }
    }

    if (_chargingdata_post_400_response->no_profile_match_info) {
    cJSON *no_profile_match_info_local_JSON = OpenAPI_no_profile_match_info_convertToJSON(_chargingdata_post_400_response->no_profile_match_info);
    if (no_profile_match_info_local_JSON == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [no_profile_match_info]");
        goto end;
    }
    cJSON_AddItemToObject(item, "noProfileMatchInfo", no_profile_match_info_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [no_profile_match_info]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->invocation_time_stamp) {
    if (cJSON_AddStringToObject(item, "invocationTimeStamp", _chargingdata_post_400_response->invocation_time_stamp) == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [invocation_time_stamp]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->is_invocation_sequence_number) {
    if (cJSON_AddNumberToObject(item, "invocationSequenceNumber", _chargingdata_post_400_response->invocation_sequence_number) == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [invocation_sequence_number]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->invocation_result) {
    cJSON *invocation_result_local_JSON = OpenAPI_invocation_result_convertToJSON(_chargingdata_post_400_response->invocation_result);
    if (invocation_result_local_JSON == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [invocation_result]");
        goto end;
    }
    cJSON_AddItemToObject(item, "invocationResult", invocation_result_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [invocation_result]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->session_failover != OpenAPI_session_failover_NULL) {
    if (cJSON_AddStringToObject(item, "sessionFailover", OpenAPI_session_failover_ToString(_chargingdata_post_400_response->session_failover)) == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [session_failover]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->multiple_unit_information) {
    cJSON *multiple_unit_informationList = cJSON_AddArrayToObject(item, "multipleUnitInformation");
    if (multiple_unit_informationList == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [multiple_unit_information]");
        goto end;
    }
    OpenAPI_list_for_each(_chargingdata_post_400_response->multiple_unit_information, node) {
        cJSON *itemLocal = OpenAPI_multiple_unit_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [multiple_unit_information]");
            goto end;
        }
        cJSON_AddItemToArray(multiple_unit_informationList, itemLocal);
    }
    }

    if (_chargingdata_post_400_response->triggers) {
    cJSON *triggersList = cJSON_AddArrayToObject(item, "triggers");
    if (triggersList == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_list_for_each(_chargingdata_post_400_response->triggers, node) {
        cJSON *itemLocal = OpenAPI_charging_trigger_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [triggers]");
            goto end;
        }
        cJSON_AddItemToArray(triggersList, itemLocal);
    }
    }

    if (_chargingdata_post_400_response->p_du_session_charging_information) {
    cJSON *p_du_session_charging_information_local_JSON = OpenAPI_pdu_session_charging_information_convertToJSON(_chargingdata_post_400_response->p_du_session_charging_information);
    if (p_du_session_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [p_du_session_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pDUSessionChargingInformation", p_du_session_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [p_du_session_charging_information]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->roaming_qbc_information) {
    cJSON *roaming_qbc_information_local_JSON = OpenAPI_roaming_qbc_information_convertToJSON(_chargingdata_post_400_response->roaming_qbc_information);
    if (roaming_qbc_information_local_JSON == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [roaming_qbc_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "roamingQBCInformation", roaming_qbc_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [roaming_qbc_information]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->location_reporting_charging_information) {
    cJSON *location_reporting_charging_information_local_JSON = OpenAPI_location_reporting_charging_information_convertToJSON(_chargingdata_post_400_response->location_reporting_charging_information);
    if (location_reporting_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [location_reporting_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "locationReportingChargingInformation", location_reporting_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [location_reporting_charging_information]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->m_bs_session_charging_information) {
    cJSON *m_bs_session_charging_information_local_JSON = OpenAPI_mbs_session_charging_information_convertToJSON(_chargingdata_post_400_response->m_bs_session_charging_information);
    if (m_bs_session_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [m_bs_session_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mBSSessionChargingInformation", m_bs_session_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [m_bs_session_charging_information]");
        goto end;
    }
    }

    if (_chargingdata_post_400_response->inter_chf_information) {
    cJSON *inter_chf_information_local_JSON = OpenAPI_inter_chf_information_convertToJSON(_chargingdata_post_400_response->inter_chf_information);
    if (inter_chf_information_local_JSON == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [inter_chf_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "interCHFInformation", inter_chf_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed [inter_chf_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI__chargingdata_post_400_response_t *OpenAPI__chargingdata_post_400_response_parseFromJSON(cJSON *_chargingdata_post_400_responseJSON)
{
    OpenAPI__chargingdata_post_400_response_t *_chargingdata_post_400_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *type = NULL;
    cJSON *title = NULL;
    cJSON *status = NULL;
    cJSON *detail = NULL;
    cJSON *instance = NULL;
    cJSON *cause = NULL;
    cJSON *invalid_params = NULL;
    OpenAPI_list_t *invalid_paramsList = NULL;
    cJSON *supported_features = NULL;
    cJSON *access_token_error = NULL;
    OpenAPI_access_token_err_t *access_token_error_local_nonprim = NULL;
    cJSON *access_token_request = NULL;
    OpenAPI_access_token_req_t *access_token_request_local_nonprim = NULL;
    cJSON *nrf_id = NULL;
    cJSON *supported_api_versions = NULL;
    OpenAPI_list_t *supported_api_versionsList = NULL;
    cJSON *no_profile_match_info = NULL;
    OpenAPI_no_profile_match_info_t *no_profile_match_info_local_nonprim = NULL;
    cJSON *invocation_time_stamp = NULL;
    cJSON *invocation_sequence_number = NULL;
    cJSON *invocation_result = NULL;
    OpenAPI_invocation_result_t *invocation_result_local_nonprim = NULL;
    cJSON *session_failover = NULL;
    OpenAPI_session_failover_e session_failoverVariable = 0;
    cJSON *multiple_unit_information = NULL;
    OpenAPI_list_t *multiple_unit_informationList = NULL;
    cJSON *triggers = NULL;
    OpenAPI_list_t *triggersList = NULL;
    cJSON *p_du_session_charging_information = NULL;
    OpenAPI_pdu_session_charging_information_t *p_du_session_charging_information_local_nonprim = NULL;
    cJSON *roaming_qbc_information = NULL;
    OpenAPI_roaming_qbc_information_t *roaming_qbc_information_local_nonprim = NULL;
    cJSON *location_reporting_charging_information = NULL;
    OpenAPI_location_reporting_charging_information_t *location_reporting_charging_information_local_nonprim = NULL;
    cJSON *m_bs_session_charging_information = NULL;
    OpenAPI_mbs_session_charging_information_t *m_bs_session_charging_information_local_nonprim = NULL;
    cJSON *inter_chf_information = NULL;
    OpenAPI_inter_chf_information_t *inter_chf_information_local_nonprim = NULL;
    type = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "type");
    if (type) {
    if (!cJSON_IsString(type) && !cJSON_IsNull(type)) {
        ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [type]");
        goto end;
    }
    }

    title = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "title");
    if (title) {
    if (!cJSON_IsString(title) && !cJSON_IsNull(title)) {
        ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [title]");
        goto end;
    }
    }

    status = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "status");
    if (status) {
    if (!cJSON_IsNumber(status)) {
        ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [status]");
        goto end;
    }
    }

    detail = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "detail");
    if (detail) {
    if (!cJSON_IsString(detail) && !cJSON_IsNull(detail)) {
        ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [detail]");
        goto end;
    }
    }

    instance = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "instance");
    if (instance) {
    if (!cJSON_IsString(instance) && !cJSON_IsNull(instance)) {
        ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [instance]");
        goto end;
    }
    }

    cause = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "cause");
    if (cause) {
    if (!cJSON_IsString(cause) && !cJSON_IsNull(cause)) {
        ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [cause]");
        goto end;
    }
    }

    invalid_params = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "invalidParams");
    if (invalid_params) {
        cJSON *invalid_params_local = NULL;
        if (!cJSON_IsArray(invalid_params)) {
            ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [invalid_params]");
            goto end;
        }

        invalid_paramsList = OpenAPI_list_create();

        cJSON_ArrayForEach(invalid_params_local, invalid_params) {
            if (!cJSON_IsObject(invalid_params_local)) {
                ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [invalid_params]");
                goto end;
            }
            OpenAPI_invalid_param_t *invalid_paramsItem = OpenAPI_invalid_param_parseFromJSON(invalid_params_local);
            if (!invalid_paramsItem) {
                ogs_error("No invalid_paramsItem");
                goto end;
            }
            OpenAPI_list_add(invalid_paramsList, invalid_paramsItem);
        }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    access_token_error = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "accessTokenError");
    if (access_token_error) {
    access_token_error_local_nonprim = OpenAPI_access_token_err_parseFromJSON(access_token_error);
    if (!access_token_error_local_nonprim) {
        ogs_error("OpenAPI_access_token_err_parseFromJSON failed [access_token_error]");
        goto end;
    }
    }

    access_token_request = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "accessTokenRequest");
    if (access_token_request) {
    access_token_request_local_nonprim = OpenAPI_access_token_req_parseFromJSON(access_token_request);
    if (!access_token_request_local_nonprim) {
        ogs_error("OpenAPI_access_token_req_parseFromJSON failed [access_token_request]");
        goto end;
    }
    }

    nrf_id = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "nrfId");
    if (nrf_id) {
    if (!cJSON_IsString(nrf_id) && !cJSON_IsNull(nrf_id)) {
        ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [nrf_id]");
        goto end;
    }
    }

    supported_api_versions = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "supportedApiVersions");
    if (supported_api_versions) {
        cJSON *supported_api_versions_local = NULL;
        if (!cJSON_IsArray(supported_api_versions)) {
            ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [supported_api_versions]");
            goto end;
        }

        supported_api_versionsList = OpenAPI_list_create();

        cJSON_ArrayForEach(supported_api_versions_local, supported_api_versions) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(supported_api_versions_local)) {
                ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [supported_api_versions]");
                goto end;
            }
            OpenAPI_list_add(supported_api_versionsList, ogs_strdup(supported_api_versions_local->valuestring));
        }
    }

    no_profile_match_info = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "noProfileMatchInfo");
    if (no_profile_match_info) {
    no_profile_match_info_local_nonprim = OpenAPI_no_profile_match_info_parseFromJSON(no_profile_match_info);
    if (!no_profile_match_info_local_nonprim) {
        ogs_error("OpenAPI_no_profile_match_info_parseFromJSON failed [no_profile_match_info]");
        goto end;
    }
    }

    invocation_time_stamp = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "invocationTimeStamp");
    if (invocation_time_stamp) {
    if (!cJSON_IsString(invocation_time_stamp) && !cJSON_IsNull(invocation_time_stamp)) {
        ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [invocation_time_stamp]");
        goto end;
    }
    }

    invocation_sequence_number = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "invocationSequenceNumber");
    if (invocation_sequence_number) {
    if (!cJSON_IsNumber(invocation_sequence_number)) {
        ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [invocation_sequence_number]");
        goto end;
    }
    }

    invocation_result = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "invocationResult");
    if (invocation_result) {
    invocation_result_local_nonprim = OpenAPI_invocation_result_parseFromJSON(invocation_result);
    if (!invocation_result_local_nonprim) {
        ogs_error("OpenAPI_invocation_result_parseFromJSON failed [invocation_result]");
        goto end;
    }
    }

    session_failover = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "sessionFailover");
    if (session_failover) {
    if (!cJSON_IsString(session_failover)) {
        ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [session_failover]");
        goto end;
    }
    session_failoverVariable = OpenAPI_session_failover_FromString(session_failover->valuestring);
    }

    multiple_unit_information = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "multipleUnitInformation");
    if (multiple_unit_information) {
        cJSON *multiple_unit_information_local = NULL;
        if (!cJSON_IsArray(multiple_unit_information)) {
            ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [multiple_unit_information]");
            goto end;
        }

        multiple_unit_informationList = OpenAPI_list_create();

        cJSON_ArrayForEach(multiple_unit_information_local, multiple_unit_information) {
            if (!cJSON_IsObject(multiple_unit_information_local)) {
                ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [multiple_unit_information]");
                goto end;
            }
            OpenAPI_multiple_unit_information_t *multiple_unit_informationItem = OpenAPI_multiple_unit_information_parseFromJSON(multiple_unit_information_local);
            if (!multiple_unit_informationItem) {
                ogs_error("No multiple_unit_informationItem");
                goto end;
            }
            OpenAPI_list_add(multiple_unit_informationList, multiple_unit_informationItem);
        }
    }

    triggers = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "triggers");
    if (triggers) {
        cJSON *triggers_local = NULL;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local, triggers) {
            if (!cJSON_IsObject(triggers_local)) {
                ogs_error("OpenAPI__chargingdata_post_400_response_parseFromJSON() failed [triggers]");
                goto end;
            }
            OpenAPI_charging_trigger_t *triggersItem = OpenAPI_charging_trigger_parseFromJSON(triggers_local);
            if (!triggersItem) {
                ogs_error("No triggersItem");
                goto end;
            }
            OpenAPI_list_add(triggersList, triggersItem);
        }
    }

    p_du_session_charging_information = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "pDUSessionChargingInformation");
    if (p_du_session_charging_information) {
    p_du_session_charging_information_local_nonprim = OpenAPI_pdu_session_charging_information_parseFromJSON(p_du_session_charging_information);
    if (!p_du_session_charging_information_local_nonprim) {
        ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON failed [p_du_session_charging_information]");
        goto end;
    }
    }

    roaming_qbc_information = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "roamingQBCInformation");
    if (roaming_qbc_information) {
    roaming_qbc_information_local_nonprim = OpenAPI_roaming_qbc_information_parseFromJSON(roaming_qbc_information);
    if (!roaming_qbc_information_local_nonprim) {
        ogs_error("OpenAPI_roaming_qbc_information_parseFromJSON failed [roaming_qbc_information]");
        goto end;
    }
    }

    location_reporting_charging_information = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "locationReportingChargingInformation");
    if (location_reporting_charging_information) {
    location_reporting_charging_information_local_nonprim = OpenAPI_location_reporting_charging_information_parseFromJSON(location_reporting_charging_information);
    if (!location_reporting_charging_information_local_nonprim) {
        ogs_error("OpenAPI_location_reporting_charging_information_parseFromJSON failed [location_reporting_charging_information]");
        goto end;
    }
    }

    m_bs_session_charging_information = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "mBSSessionChargingInformation");
    if (m_bs_session_charging_information) {
    m_bs_session_charging_information_local_nonprim = OpenAPI_mbs_session_charging_information_parseFromJSON(m_bs_session_charging_information);
    if (!m_bs_session_charging_information_local_nonprim) {
        ogs_error("OpenAPI_mbs_session_charging_information_parseFromJSON failed [m_bs_session_charging_information]");
        goto end;
    }
    }

    inter_chf_information = cJSON_GetObjectItemCaseSensitive(_chargingdata_post_400_responseJSON, "interCHFInformation");
    if (inter_chf_information) {
    inter_chf_information_local_nonprim = OpenAPI_inter_chf_information_parseFromJSON(inter_chf_information);
    if (!inter_chf_information_local_nonprim) {
        ogs_error("OpenAPI_inter_chf_information_parseFromJSON failed [inter_chf_information]");
        goto end;
    }
    }

    _chargingdata_post_400_response_local_var = OpenAPI__chargingdata_post_400_response_create (
        type && !cJSON_IsNull(type) ? ogs_strdup(type->valuestring) : NULL,
        title && !cJSON_IsNull(title) ? ogs_strdup(title->valuestring) : NULL,
        status ? true : false,
        status ? status->valuedouble : 0,
        detail && !cJSON_IsNull(detail) ? ogs_strdup(detail->valuestring) : NULL,
        instance && !cJSON_IsNull(instance) ? ogs_strdup(instance->valuestring) : NULL,
        cause && !cJSON_IsNull(cause) ? ogs_strdup(cause->valuestring) : NULL,
        invalid_params ? invalid_paramsList : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        access_token_error ? access_token_error_local_nonprim : NULL,
        access_token_request ? access_token_request_local_nonprim : NULL,
        nrf_id && !cJSON_IsNull(nrf_id) ? ogs_strdup(nrf_id->valuestring) : NULL,
        supported_api_versions ? supported_api_versionsList : NULL,
        no_profile_match_info ? no_profile_match_info_local_nonprim : NULL,
        invocation_time_stamp && !cJSON_IsNull(invocation_time_stamp) ? ogs_strdup(invocation_time_stamp->valuestring) : NULL,
        invocation_sequence_number ? true : false,
        invocation_sequence_number ? invocation_sequence_number->valuedouble : 0,
        invocation_result ? invocation_result_local_nonprim : NULL,
        session_failover ? session_failoverVariable : 0,
        multiple_unit_information ? multiple_unit_informationList : NULL,
        triggers ? triggersList : NULL,
        p_du_session_charging_information ? p_du_session_charging_information_local_nonprim : NULL,
        roaming_qbc_information ? roaming_qbc_information_local_nonprim : NULL,
        location_reporting_charging_information ? location_reporting_charging_information_local_nonprim : NULL,
        m_bs_session_charging_information ? m_bs_session_charging_information_local_nonprim : NULL,
        inter_chf_information ? inter_chf_information_local_nonprim : NULL
    );

    return _chargingdata_post_400_response_local_var;
end:
    if (invalid_paramsList) {
        OpenAPI_list_for_each(invalid_paramsList, node) {
            OpenAPI_invalid_param_free(node->data);
        }
        OpenAPI_list_free(invalid_paramsList);
        invalid_paramsList = NULL;
    }
    if (access_token_error_local_nonprim) {
        OpenAPI_access_token_err_free(access_token_error_local_nonprim);
        access_token_error_local_nonprim = NULL;
    }
    if (access_token_request_local_nonprim) {
        OpenAPI_access_token_req_free(access_token_request_local_nonprim);
        access_token_request_local_nonprim = NULL;
    }
    if (supported_api_versionsList) {
        OpenAPI_list_for_each(supported_api_versionsList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(supported_api_versionsList);
        supported_api_versionsList = NULL;
    }
    if (no_profile_match_info_local_nonprim) {
        OpenAPI_no_profile_match_info_free(no_profile_match_info_local_nonprim);
        no_profile_match_info_local_nonprim = NULL;
    }
    if (invocation_result_local_nonprim) {
        OpenAPI_invocation_result_free(invocation_result_local_nonprim);
        invocation_result_local_nonprim = NULL;
    }
    if (multiple_unit_informationList) {
        OpenAPI_list_for_each(multiple_unit_informationList, node) {
            OpenAPI_multiple_unit_information_free(node->data);
        }
        OpenAPI_list_free(multiple_unit_informationList);
        multiple_unit_informationList = NULL;
    }
    if (triggersList) {
        OpenAPI_list_for_each(triggersList, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(triggersList);
        triggersList = NULL;
    }
    if (p_du_session_charging_information_local_nonprim) {
        OpenAPI_pdu_session_charging_information_free(p_du_session_charging_information_local_nonprim);
        p_du_session_charging_information_local_nonprim = NULL;
    }
    if (roaming_qbc_information_local_nonprim) {
        OpenAPI_roaming_qbc_information_free(roaming_qbc_information_local_nonprim);
        roaming_qbc_information_local_nonprim = NULL;
    }
    if (location_reporting_charging_information_local_nonprim) {
        OpenAPI_location_reporting_charging_information_free(location_reporting_charging_information_local_nonprim);
        location_reporting_charging_information_local_nonprim = NULL;
    }
    if (m_bs_session_charging_information_local_nonprim) {
        OpenAPI_mbs_session_charging_information_free(m_bs_session_charging_information_local_nonprim);
        m_bs_session_charging_information_local_nonprim = NULL;
    }
    if (inter_chf_information_local_nonprim) {
        OpenAPI_inter_chf_information_free(inter_chf_information_local_nonprim);
        inter_chf_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI__chargingdata_post_400_response_t *OpenAPI__chargingdata_post_400_response_copy(OpenAPI__chargingdata_post_400_response_t *dst, OpenAPI__chargingdata_post_400_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI__chargingdata_post_400_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI__chargingdata_post_400_response_convertToJSON() failed");
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

    OpenAPI__chargingdata_post_400_response_free(dst);
    dst = OpenAPI__chargingdata_post_400_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

