
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_data_response.h"

OpenAPI_charging_data_response_t *OpenAPI_charging_data_response_create(
    char *invocation_time_stamp,
    int invocation_sequence_number,
    OpenAPI_invocation_result_t *invocation_result,
    OpenAPI_session_failover_e session_failover,
    char *supported_features,
    OpenAPI_list_t *multiple_unit_information,
    OpenAPI_list_t *triggers,
    OpenAPI_pdu_session_charging_information_t *p_du_session_charging_information,
    OpenAPI_roaming_qbc_information_t *roaming_qbc_information,
    OpenAPI_location_reporting_charging_information_t *location_reporting_charging_information,
    OpenAPI_mbs_session_charging_information_t *m_bs_session_charging_information,
    OpenAPI_inter_chf_information_t *inter_chf_information
)
{
    OpenAPI_charging_data_response_t *charging_data_response_local_var = ogs_malloc(sizeof(OpenAPI_charging_data_response_t));
    ogs_assert(charging_data_response_local_var);

    charging_data_response_local_var->invocation_time_stamp = invocation_time_stamp;
    charging_data_response_local_var->invocation_sequence_number = invocation_sequence_number;
    charging_data_response_local_var->invocation_result = invocation_result;
    charging_data_response_local_var->session_failover = session_failover;
    charging_data_response_local_var->supported_features = supported_features;
    charging_data_response_local_var->multiple_unit_information = multiple_unit_information;
    charging_data_response_local_var->triggers = triggers;
    charging_data_response_local_var->p_du_session_charging_information = p_du_session_charging_information;
    charging_data_response_local_var->roaming_qbc_information = roaming_qbc_information;
    charging_data_response_local_var->location_reporting_charging_information = location_reporting_charging_information;
    charging_data_response_local_var->m_bs_session_charging_information = m_bs_session_charging_information;
    charging_data_response_local_var->inter_chf_information = inter_chf_information;

    return charging_data_response_local_var;
}

void OpenAPI_charging_data_response_free(OpenAPI_charging_data_response_t *charging_data_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_data_response) {
        return;
    }
    if (charging_data_response->invocation_time_stamp) {
        ogs_free(charging_data_response->invocation_time_stamp);
        charging_data_response->invocation_time_stamp = NULL;
    }
    if (charging_data_response->invocation_result) {
        OpenAPI_invocation_result_free(charging_data_response->invocation_result);
        charging_data_response->invocation_result = NULL;
    }
    if (charging_data_response->supported_features) {
        ogs_free(charging_data_response->supported_features);
        charging_data_response->supported_features = NULL;
    }
    if (charging_data_response->multiple_unit_information) {
        OpenAPI_list_for_each(charging_data_response->multiple_unit_information, node) {
            OpenAPI_multiple_unit_information_free(node->data);
        }
        OpenAPI_list_free(charging_data_response->multiple_unit_information);
        charging_data_response->multiple_unit_information = NULL;
    }
    if (charging_data_response->triggers) {
        OpenAPI_list_for_each(charging_data_response->triggers, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(charging_data_response->triggers);
        charging_data_response->triggers = NULL;
    }
    if (charging_data_response->p_du_session_charging_information) {
        OpenAPI_pdu_session_charging_information_free(charging_data_response->p_du_session_charging_information);
        charging_data_response->p_du_session_charging_information = NULL;
    }
    if (charging_data_response->roaming_qbc_information) {
        OpenAPI_roaming_qbc_information_free(charging_data_response->roaming_qbc_information);
        charging_data_response->roaming_qbc_information = NULL;
    }
    if (charging_data_response->location_reporting_charging_information) {
        OpenAPI_location_reporting_charging_information_free(charging_data_response->location_reporting_charging_information);
        charging_data_response->location_reporting_charging_information = NULL;
    }
    if (charging_data_response->m_bs_session_charging_information) {
        OpenAPI_mbs_session_charging_information_free(charging_data_response->m_bs_session_charging_information);
        charging_data_response->m_bs_session_charging_information = NULL;
    }
    if (charging_data_response->inter_chf_information) {
        OpenAPI_inter_chf_information_free(charging_data_response->inter_chf_information);
        charging_data_response->inter_chf_information = NULL;
    }
    ogs_free(charging_data_response);
}

cJSON *OpenAPI_charging_data_response_convertToJSON(OpenAPI_charging_data_response_t *charging_data_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_data_response == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [ChargingDataResponse]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!charging_data_response->invocation_time_stamp) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [invocation_time_stamp]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "invocationTimeStamp", charging_data_response->invocation_time_stamp) == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [invocation_time_stamp]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "invocationSequenceNumber", charging_data_response->invocation_sequence_number) == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [invocation_sequence_number]");
        goto end;
    }

    if (charging_data_response->invocation_result) {
    cJSON *invocation_result_local_JSON = OpenAPI_invocation_result_convertToJSON(charging_data_response->invocation_result);
    if (invocation_result_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [invocation_result]");
        goto end;
    }
    cJSON_AddItemToObject(item, "invocationResult", invocation_result_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [invocation_result]");
        goto end;
    }
    }

    if (charging_data_response->session_failover != OpenAPI_session_failover_NULL) {
    if (cJSON_AddStringToObject(item, "sessionFailover", OpenAPI_session_failover_ToString(charging_data_response->session_failover)) == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [session_failover]");
        goto end;
    }
    }

    if (charging_data_response->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", charging_data_response->supported_features) == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (charging_data_response->multiple_unit_information) {
    cJSON *multiple_unit_informationList = cJSON_AddArrayToObject(item, "multipleUnitInformation");
    if (multiple_unit_informationList == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [multiple_unit_information]");
        goto end;
    }
    OpenAPI_list_for_each(charging_data_response->multiple_unit_information, node) {
        cJSON *itemLocal = OpenAPI_multiple_unit_information_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [multiple_unit_information]");
            goto end;
        }
        cJSON_AddItemToArray(multiple_unit_informationList, itemLocal);
    }
    }

    if (charging_data_response->triggers) {
    cJSON *triggersList = cJSON_AddArrayToObject(item, "triggers");
    if (triggersList == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_list_for_each(charging_data_response->triggers, node) {
        cJSON *itemLocal = OpenAPI_charging_trigger_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [triggers]");
            goto end;
        }
        cJSON_AddItemToArray(triggersList, itemLocal);
    }
    }

    if (charging_data_response->p_du_session_charging_information) {
    cJSON *p_du_session_charging_information_local_JSON = OpenAPI_pdu_session_charging_information_convertToJSON(charging_data_response->p_du_session_charging_information);
    if (p_du_session_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [p_du_session_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pDUSessionChargingInformation", p_du_session_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [p_du_session_charging_information]");
        goto end;
    }
    }

    if (charging_data_response->roaming_qbc_information) {
    cJSON *roaming_qbc_information_local_JSON = OpenAPI_roaming_qbc_information_convertToJSON(charging_data_response->roaming_qbc_information);
    if (roaming_qbc_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [roaming_qbc_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "roamingQBCInformation", roaming_qbc_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [roaming_qbc_information]");
        goto end;
    }
    }

    if (charging_data_response->location_reporting_charging_information) {
    cJSON *location_reporting_charging_information_local_JSON = OpenAPI_location_reporting_charging_information_convertToJSON(charging_data_response->location_reporting_charging_information);
    if (location_reporting_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [location_reporting_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "locationReportingChargingInformation", location_reporting_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [location_reporting_charging_information]");
        goto end;
    }
    }

    if (charging_data_response->m_bs_session_charging_information) {
    cJSON *m_bs_session_charging_information_local_JSON = OpenAPI_mbs_session_charging_information_convertToJSON(charging_data_response->m_bs_session_charging_information);
    if (m_bs_session_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [m_bs_session_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mBSSessionChargingInformation", m_bs_session_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [m_bs_session_charging_information]");
        goto end;
    }
    }

    if (charging_data_response->inter_chf_information) {
    cJSON *inter_chf_information_local_JSON = OpenAPI_inter_chf_information_convertToJSON(charging_data_response->inter_chf_information);
    if (inter_chf_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [inter_chf_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "interCHFInformation", inter_chf_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed [inter_chf_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_charging_data_response_t *OpenAPI_charging_data_response_parseFromJSON(cJSON *charging_data_responseJSON)
{
    OpenAPI_charging_data_response_t *charging_data_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *invocation_time_stamp = NULL;
    cJSON *invocation_sequence_number = NULL;
    cJSON *invocation_result = NULL;
    OpenAPI_invocation_result_t *invocation_result_local_nonprim = NULL;
    cJSON *session_failover = NULL;
    OpenAPI_session_failover_e session_failoverVariable = 0;
    cJSON *supported_features = NULL;
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
    invocation_time_stamp = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "invocationTimeStamp");
    if (!invocation_time_stamp) {
        ogs_error("OpenAPI_charging_data_response_parseFromJSON() failed [invocation_time_stamp]");
        goto end;
    }
    if (!cJSON_IsString(invocation_time_stamp) && !cJSON_IsNull(invocation_time_stamp)) {
        ogs_error("OpenAPI_charging_data_response_parseFromJSON() failed [invocation_time_stamp]");
        goto end;
    }

    invocation_sequence_number = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "invocationSequenceNumber");
    if (!invocation_sequence_number) {
        ogs_error("OpenAPI_charging_data_response_parseFromJSON() failed [invocation_sequence_number]");
        goto end;
    }
    if (!cJSON_IsNumber(invocation_sequence_number)) {
        ogs_error("OpenAPI_charging_data_response_parseFromJSON() failed [invocation_sequence_number]");
        goto end;
    }

    invocation_result = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "invocationResult");
    if (invocation_result) {
    invocation_result_local_nonprim = OpenAPI_invocation_result_parseFromJSON(invocation_result);
    if (!invocation_result_local_nonprim) {
        ogs_error("OpenAPI_invocation_result_parseFromJSON failed [invocation_result]");
        goto end;
    }
    }

    session_failover = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "sessionFailover");
    if (session_failover) {
    if (!cJSON_IsString(session_failover)) {
        ogs_error("OpenAPI_charging_data_response_parseFromJSON() failed [session_failover]");
        goto end;
    }
    session_failoverVariable = OpenAPI_session_failover_FromString(session_failover->valuestring);
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_charging_data_response_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    multiple_unit_information = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "multipleUnitInformation");
    if (multiple_unit_information) {
        cJSON *multiple_unit_information_local = NULL;
        if (!cJSON_IsArray(multiple_unit_information)) {
            ogs_error("OpenAPI_charging_data_response_parseFromJSON() failed [multiple_unit_information]");
            goto end;
        }

        multiple_unit_informationList = OpenAPI_list_create();

        cJSON_ArrayForEach(multiple_unit_information_local, multiple_unit_information) {
            if (!cJSON_IsObject(multiple_unit_information_local)) {
                ogs_error("OpenAPI_charging_data_response_parseFromJSON() failed [multiple_unit_information]");
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

    triggers = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "triggers");
    if (triggers) {
        cJSON *triggers_local = NULL;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI_charging_data_response_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local, triggers) {
            if (!cJSON_IsObject(triggers_local)) {
                ogs_error("OpenAPI_charging_data_response_parseFromJSON() failed [triggers]");
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

    p_du_session_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "pDUSessionChargingInformation");
    if (p_du_session_charging_information) {
    p_du_session_charging_information_local_nonprim = OpenAPI_pdu_session_charging_information_parseFromJSON(p_du_session_charging_information);
    if (!p_du_session_charging_information_local_nonprim) {
        ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON failed [p_du_session_charging_information]");
        goto end;
    }
    }

    roaming_qbc_information = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "roamingQBCInformation");
    if (roaming_qbc_information) {
    roaming_qbc_information_local_nonprim = OpenAPI_roaming_qbc_information_parseFromJSON(roaming_qbc_information);
    if (!roaming_qbc_information_local_nonprim) {
        ogs_error("OpenAPI_roaming_qbc_information_parseFromJSON failed [roaming_qbc_information]");
        goto end;
    }
    }

    location_reporting_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "locationReportingChargingInformation");
    if (location_reporting_charging_information) {
    location_reporting_charging_information_local_nonprim = OpenAPI_location_reporting_charging_information_parseFromJSON(location_reporting_charging_information);
    if (!location_reporting_charging_information_local_nonprim) {
        ogs_error("OpenAPI_location_reporting_charging_information_parseFromJSON failed [location_reporting_charging_information]");
        goto end;
    }
    }

    m_bs_session_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "mBSSessionChargingInformation");
    if (m_bs_session_charging_information) {
    m_bs_session_charging_information_local_nonprim = OpenAPI_mbs_session_charging_information_parseFromJSON(m_bs_session_charging_information);
    if (!m_bs_session_charging_information_local_nonprim) {
        ogs_error("OpenAPI_mbs_session_charging_information_parseFromJSON failed [m_bs_session_charging_information]");
        goto end;
    }
    }

    inter_chf_information = cJSON_GetObjectItemCaseSensitive(charging_data_responseJSON, "interCHFInformation");
    if (inter_chf_information) {
    inter_chf_information_local_nonprim = OpenAPI_inter_chf_information_parseFromJSON(inter_chf_information);
    if (!inter_chf_information_local_nonprim) {
        ogs_error("OpenAPI_inter_chf_information_parseFromJSON failed [inter_chf_information]");
        goto end;
    }
    }

    charging_data_response_local_var = OpenAPI_charging_data_response_create (
        ogs_strdup(invocation_time_stamp->valuestring),
        
        invocation_sequence_number->valuedouble,
        invocation_result ? invocation_result_local_nonprim : NULL,
        session_failover ? session_failoverVariable : 0,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        multiple_unit_information ? multiple_unit_informationList : NULL,
        triggers ? triggersList : NULL,
        p_du_session_charging_information ? p_du_session_charging_information_local_nonprim : NULL,
        roaming_qbc_information ? roaming_qbc_information_local_nonprim : NULL,
        location_reporting_charging_information ? location_reporting_charging_information_local_nonprim : NULL,
        m_bs_session_charging_information ? m_bs_session_charging_information_local_nonprim : NULL,
        inter_chf_information ? inter_chf_information_local_nonprim : NULL
    );

    return charging_data_response_local_var;
end:
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

OpenAPI_charging_data_response_t *OpenAPI_charging_data_response_copy(OpenAPI_charging_data_response_t *dst, OpenAPI_charging_data_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_data_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_data_response_convertToJSON() failed");
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

    OpenAPI_charging_data_response_free(dst);
    dst = OpenAPI_charging_data_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

