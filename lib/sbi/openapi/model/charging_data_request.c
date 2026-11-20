
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "charging_data_request.h"

OpenAPI_charging_data_request_t *OpenAPI_charging_data_request_create(
    char *subscriber_identifier,
    char *tenant_identifier,
    bool is_charging_id,
    int charging_id,
    char *mn_s_consumer_identifier,
    OpenAPI_nf_identification_t *nf_consumer_identification,
    char *invocation_time_stamp,
    int invocation_sequence_number,
    bool is_retransmission_indicator,
    int retransmission_indicator,
    bool is_one_time_event,
    int one_time_event,
    OpenAPI_one_time_event_type_e one_time_event_type,
    char *notify_uri,
    char *supported_features,
    char *service_specification_info,
    OpenAPI_list_t *multiple_unit_usage,
    OpenAPI_list_t *triggers,
    char *easid,
    char *ednid,
    char *e_as_provider_identifier,
    char *a_mfid,
    OpenAPI_pdu_session_charging_information_t *p_du_session_charging_information,
    OpenAPI_roaming_qbc_information_t *roaming_qbc_information,
    OpenAPI_sms_charging_information_t *s_ms_charging_information,
    OpenAPI_nef_charging_information_t *n_ef_charging_information,
    OpenAPI_registration_charging_information_t *registration_charging_information,
    OpenAPI_n2_connection_charging_information_t *n2_connection_charging_information,
    OpenAPI_location_reporting_charging_information_t *location_reporting_charging_information,
    OpenAPI_nspa_charging_information_t *n_spa_charging_information,
    OpenAPI_network_sharing_charging_information_t *network_sharing_charging_information,
    OpenAPI_nsm_charging_information_t *n_sm_charging_information,
    OpenAPI_mm_tel_charging_information_t *m_m_tel_charging_information,
    OpenAPI_ims_charging_information_t *i_ms_charging_information,
    OpenAPI_edge_infrastructure_usage_charging_information_t *edge_infrastructure_usage_charging_information,
    OpenAPI_eas_deployment_charging_information_t *e_as_deployment_charging_information,
    OpenAPI_nef_charging_information_t *direct_edge_enabling_service_charging_information,
    OpenAPI_nef_charging_information_t *exposed_edge_enabling_service_charging_information,
    OpenAPI_prose_charging_information_t *pro_se_charging_information,
    OpenAPI_mms_charging_information_t *m_ms_charging_information,
    OpenAPI_mbs_session_charging_information_t *m_bs_session_charging_information,
    OpenAPI_tsn_charging_information_t *t_sn_charging_information,
    OpenAPI_inter_chf_information_t *inter_chf_information,
    OpenAPI_nsacf_charging_information_t *n_sacf_charging_information,
    OpenAPI_nssaa_charging_information_t *n_ssaa_charging_information,
    OpenAPI_ranging_sl_charging_information_t *ranging_sl_charging_information,
    OpenAPI_lcs_information_t *l_cs_information
)
{
    OpenAPI_charging_data_request_t *charging_data_request_local_var = ogs_malloc(sizeof(OpenAPI_charging_data_request_t));
    ogs_assert(charging_data_request_local_var);

    charging_data_request_local_var->subscriber_identifier = subscriber_identifier;
    charging_data_request_local_var->tenant_identifier = tenant_identifier;
    charging_data_request_local_var->is_charging_id = is_charging_id;
    charging_data_request_local_var->charging_id = charging_id;
    charging_data_request_local_var->mn_s_consumer_identifier = mn_s_consumer_identifier;
    charging_data_request_local_var->nf_consumer_identification = nf_consumer_identification;
    charging_data_request_local_var->invocation_time_stamp = invocation_time_stamp;
    charging_data_request_local_var->invocation_sequence_number = invocation_sequence_number;
    charging_data_request_local_var->is_retransmission_indicator = is_retransmission_indicator;
    charging_data_request_local_var->retransmission_indicator = retransmission_indicator;
    charging_data_request_local_var->is_one_time_event = is_one_time_event;
    charging_data_request_local_var->one_time_event = one_time_event;
    charging_data_request_local_var->one_time_event_type = one_time_event_type;
    charging_data_request_local_var->notify_uri = notify_uri;
    charging_data_request_local_var->supported_features = supported_features;
    charging_data_request_local_var->service_specification_info = service_specification_info;
    charging_data_request_local_var->multiple_unit_usage = multiple_unit_usage;
    charging_data_request_local_var->triggers = triggers;
    charging_data_request_local_var->easid = easid;
    charging_data_request_local_var->ednid = ednid;
    charging_data_request_local_var->e_as_provider_identifier = e_as_provider_identifier;
    charging_data_request_local_var->a_mfid = a_mfid;
    charging_data_request_local_var->p_du_session_charging_information = p_du_session_charging_information;
    charging_data_request_local_var->roaming_qbc_information = roaming_qbc_information;
    charging_data_request_local_var->s_ms_charging_information = s_ms_charging_information;
    charging_data_request_local_var->n_ef_charging_information = n_ef_charging_information;
    charging_data_request_local_var->registration_charging_information = registration_charging_information;
    charging_data_request_local_var->n2_connection_charging_information = n2_connection_charging_information;
    charging_data_request_local_var->location_reporting_charging_information = location_reporting_charging_information;
    charging_data_request_local_var->n_spa_charging_information = n_spa_charging_information;
    charging_data_request_local_var->network_sharing_charging_information = network_sharing_charging_information;
    charging_data_request_local_var->n_sm_charging_information = n_sm_charging_information;
    charging_data_request_local_var->m_m_tel_charging_information = m_m_tel_charging_information;
    charging_data_request_local_var->i_ms_charging_information = i_ms_charging_information;
    charging_data_request_local_var->edge_infrastructure_usage_charging_information = edge_infrastructure_usage_charging_information;
    charging_data_request_local_var->e_as_deployment_charging_information = e_as_deployment_charging_information;
    charging_data_request_local_var->direct_edge_enabling_service_charging_information = direct_edge_enabling_service_charging_information;
    charging_data_request_local_var->exposed_edge_enabling_service_charging_information = exposed_edge_enabling_service_charging_information;
    charging_data_request_local_var->pro_se_charging_information = pro_se_charging_information;
    charging_data_request_local_var->m_ms_charging_information = m_ms_charging_information;
    charging_data_request_local_var->m_bs_session_charging_information = m_bs_session_charging_information;
    charging_data_request_local_var->t_sn_charging_information = t_sn_charging_information;
    charging_data_request_local_var->inter_chf_information = inter_chf_information;
    charging_data_request_local_var->n_sacf_charging_information = n_sacf_charging_information;
    charging_data_request_local_var->n_ssaa_charging_information = n_ssaa_charging_information;
    charging_data_request_local_var->ranging_sl_charging_information = ranging_sl_charging_information;
    charging_data_request_local_var->l_cs_information = l_cs_information;

    return charging_data_request_local_var;
}

void OpenAPI_charging_data_request_free(OpenAPI_charging_data_request_t *charging_data_request)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == charging_data_request) {
        return;
    }
    if (charging_data_request->subscriber_identifier) {
        ogs_free(charging_data_request->subscriber_identifier);
        charging_data_request->subscriber_identifier = NULL;
    }
    if (charging_data_request->tenant_identifier) {
        ogs_free(charging_data_request->tenant_identifier);
        charging_data_request->tenant_identifier = NULL;
    }
    if (charging_data_request->mn_s_consumer_identifier) {
        ogs_free(charging_data_request->mn_s_consumer_identifier);
        charging_data_request->mn_s_consumer_identifier = NULL;
    }
    if (charging_data_request->nf_consumer_identification) {
        OpenAPI_nf_identification_free(charging_data_request->nf_consumer_identification);
        charging_data_request->nf_consumer_identification = NULL;
    }
    if (charging_data_request->invocation_time_stamp) {
        ogs_free(charging_data_request->invocation_time_stamp);
        charging_data_request->invocation_time_stamp = NULL;
    }
    if (charging_data_request->notify_uri) {
        ogs_free(charging_data_request->notify_uri);
        charging_data_request->notify_uri = NULL;
    }
    if (charging_data_request->supported_features) {
        ogs_free(charging_data_request->supported_features);
        charging_data_request->supported_features = NULL;
    }
    if (charging_data_request->service_specification_info) {
        ogs_free(charging_data_request->service_specification_info);
        charging_data_request->service_specification_info = NULL;
    }
    if (charging_data_request->multiple_unit_usage) {
        OpenAPI_list_for_each(charging_data_request->multiple_unit_usage, node) {
            OpenAPI_multiple_unit_usage_free(node->data);
        }
        OpenAPI_list_free(charging_data_request->multiple_unit_usage);
        charging_data_request->multiple_unit_usage = NULL;
    }
    if (charging_data_request->triggers) {
        OpenAPI_list_for_each(charging_data_request->triggers, node) {
            OpenAPI_charging_trigger_free(node->data);
        }
        OpenAPI_list_free(charging_data_request->triggers);
        charging_data_request->triggers = NULL;
    }
    if (charging_data_request->easid) {
        ogs_free(charging_data_request->easid);
        charging_data_request->easid = NULL;
    }
    if (charging_data_request->ednid) {
        ogs_free(charging_data_request->ednid);
        charging_data_request->ednid = NULL;
    }
    if (charging_data_request->e_as_provider_identifier) {
        ogs_free(charging_data_request->e_as_provider_identifier);
        charging_data_request->e_as_provider_identifier = NULL;
    }
    if (charging_data_request->a_mfid) {
        ogs_free(charging_data_request->a_mfid);
        charging_data_request->a_mfid = NULL;
    }
    if (charging_data_request->p_du_session_charging_information) {
        OpenAPI_pdu_session_charging_information_free(charging_data_request->p_du_session_charging_information);
        charging_data_request->p_du_session_charging_information = NULL;
    }
    if (charging_data_request->roaming_qbc_information) {
        OpenAPI_roaming_qbc_information_free(charging_data_request->roaming_qbc_information);
        charging_data_request->roaming_qbc_information = NULL;
    }
    if (charging_data_request->s_ms_charging_information) {
        OpenAPI_sms_charging_information_free(charging_data_request->s_ms_charging_information);
        charging_data_request->s_ms_charging_information = NULL;
    }
    if (charging_data_request->n_ef_charging_information) {
        OpenAPI_nef_charging_information_free(charging_data_request->n_ef_charging_information);
        charging_data_request->n_ef_charging_information = NULL;
    }
    if (charging_data_request->registration_charging_information) {
        OpenAPI_registration_charging_information_free(charging_data_request->registration_charging_information);
        charging_data_request->registration_charging_information = NULL;
    }
    if (charging_data_request->n2_connection_charging_information) {
        OpenAPI_n2_connection_charging_information_free(charging_data_request->n2_connection_charging_information);
        charging_data_request->n2_connection_charging_information = NULL;
    }
    if (charging_data_request->location_reporting_charging_information) {
        OpenAPI_location_reporting_charging_information_free(charging_data_request->location_reporting_charging_information);
        charging_data_request->location_reporting_charging_information = NULL;
    }
    if (charging_data_request->n_spa_charging_information) {
        OpenAPI_nspa_charging_information_free(charging_data_request->n_spa_charging_information);
        charging_data_request->n_spa_charging_information = NULL;
    }
    if (charging_data_request->network_sharing_charging_information) {
        OpenAPI_network_sharing_charging_information_free(charging_data_request->network_sharing_charging_information);
        charging_data_request->network_sharing_charging_information = NULL;
    }
    if (charging_data_request->n_sm_charging_information) {
        OpenAPI_nsm_charging_information_free(charging_data_request->n_sm_charging_information);
        charging_data_request->n_sm_charging_information = NULL;
    }
    if (charging_data_request->m_m_tel_charging_information) {
        OpenAPI_mm_tel_charging_information_free(charging_data_request->m_m_tel_charging_information);
        charging_data_request->m_m_tel_charging_information = NULL;
    }
    if (charging_data_request->i_ms_charging_information) {
        OpenAPI_ims_charging_information_free(charging_data_request->i_ms_charging_information);
        charging_data_request->i_ms_charging_information = NULL;
    }
    if (charging_data_request->edge_infrastructure_usage_charging_information) {
        OpenAPI_edge_infrastructure_usage_charging_information_free(charging_data_request->edge_infrastructure_usage_charging_information);
        charging_data_request->edge_infrastructure_usage_charging_information = NULL;
    }
    if (charging_data_request->e_as_deployment_charging_information) {
        OpenAPI_eas_deployment_charging_information_free(charging_data_request->e_as_deployment_charging_information);
        charging_data_request->e_as_deployment_charging_information = NULL;
    }
    if (charging_data_request->direct_edge_enabling_service_charging_information) {
        OpenAPI_nef_charging_information_free(charging_data_request->direct_edge_enabling_service_charging_information);
        charging_data_request->direct_edge_enabling_service_charging_information = NULL;
    }
    if (charging_data_request->exposed_edge_enabling_service_charging_information) {
        OpenAPI_nef_charging_information_free(charging_data_request->exposed_edge_enabling_service_charging_information);
        charging_data_request->exposed_edge_enabling_service_charging_information = NULL;
    }
    if (charging_data_request->pro_se_charging_information) {
        OpenAPI_prose_charging_information_free(charging_data_request->pro_se_charging_information);
        charging_data_request->pro_se_charging_information = NULL;
    }
    if (charging_data_request->m_ms_charging_information) {
        OpenAPI_mms_charging_information_free(charging_data_request->m_ms_charging_information);
        charging_data_request->m_ms_charging_information = NULL;
    }
    if (charging_data_request->m_bs_session_charging_information) {
        OpenAPI_mbs_session_charging_information_free(charging_data_request->m_bs_session_charging_information);
        charging_data_request->m_bs_session_charging_information = NULL;
    }
    if (charging_data_request->t_sn_charging_information) {
        OpenAPI_tsn_charging_information_free(charging_data_request->t_sn_charging_information);
        charging_data_request->t_sn_charging_information = NULL;
    }
    if (charging_data_request->inter_chf_information) {
        OpenAPI_inter_chf_information_free(charging_data_request->inter_chf_information);
        charging_data_request->inter_chf_information = NULL;
    }
    if (charging_data_request->n_sacf_charging_information) {
        OpenAPI_nsacf_charging_information_free(charging_data_request->n_sacf_charging_information);
        charging_data_request->n_sacf_charging_information = NULL;
    }
    if (charging_data_request->n_ssaa_charging_information) {
        OpenAPI_nssaa_charging_information_free(charging_data_request->n_ssaa_charging_information);
        charging_data_request->n_ssaa_charging_information = NULL;
    }
    if (charging_data_request->ranging_sl_charging_information) {
        OpenAPI_ranging_sl_charging_information_free(charging_data_request->ranging_sl_charging_information);
        charging_data_request->ranging_sl_charging_information = NULL;
    }
    if (charging_data_request->l_cs_information) {
        OpenAPI_lcs_information_free(charging_data_request->l_cs_information);
        charging_data_request->l_cs_information = NULL;
    }
    ogs_free(charging_data_request);
}

cJSON *OpenAPI_charging_data_request_convertToJSON(OpenAPI_charging_data_request_t *charging_data_request)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (charging_data_request == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [ChargingDataRequest]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (charging_data_request->subscriber_identifier) {
    if (cJSON_AddStringToObject(item, "subscriberIdentifier", charging_data_request->subscriber_identifier) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [subscriber_identifier]");
        goto end;
    }
    }

    if (charging_data_request->tenant_identifier) {
    if (cJSON_AddStringToObject(item, "tenantIdentifier", charging_data_request->tenant_identifier) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [tenant_identifier]");
        goto end;
    }
    }

    if (charging_data_request->is_charging_id) {
    if (cJSON_AddNumberToObject(item, "chargingId", charging_data_request->charging_id) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [charging_id]");
        goto end;
    }
    }

    if (charging_data_request->mn_s_consumer_identifier) {
    if (cJSON_AddStringToObject(item, "mnSConsumerIdentifier", charging_data_request->mn_s_consumer_identifier) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [mn_s_consumer_identifier]");
        goto end;
    }
    }

    if (!charging_data_request->nf_consumer_identification) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [nf_consumer_identification]");
        return NULL;
    }
    cJSON *nf_consumer_identification_local_JSON = OpenAPI_nf_identification_convertToJSON(charging_data_request->nf_consumer_identification);
    if (nf_consumer_identification_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [nf_consumer_identification]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nfConsumerIdentification", nf_consumer_identification_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [nf_consumer_identification]");
        goto end;
    }

    if (!charging_data_request->invocation_time_stamp) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [invocation_time_stamp]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "invocationTimeStamp", charging_data_request->invocation_time_stamp) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [invocation_time_stamp]");
        goto end;
    }

    if (cJSON_AddNumberToObject(item, "invocationSequenceNumber", charging_data_request->invocation_sequence_number) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [invocation_sequence_number]");
        goto end;
    }

    if (charging_data_request->is_retransmission_indicator) {
    if (cJSON_AddBoolToObject(item, "retransmissionIndicator", charging_data_request->retransmission_indicator) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [retransmission_indicator]");
        goto end;
    }
    }

    if (charging_data_request->is_one_time_event) {
    if (cJSON_AddBoolToObject(item, "oneTimeEvent", charging_data_request->one_time_event) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [one_time_event]");
        goto end;
    }
    }

    if (charging_data_request->one_time_event_type != OpenAPI_one_time_event_type_NULL) {
    if (cJSON_AddStringToObject(item, "oneTimeEventType", OpenAPI_one_time_event_type_ToString(charging_data_request->one_time_event_type)) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [one_time_event_type]");
        goto end;
    }
    }

    if (charging_data_request->notify_uri) {
    if (cJSON_AddStringToObject(item, "notifyUri", charging_data_request->notify_uri) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [notify_uri]");
        goto end;
    }
    }

    if (charging_data_request->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", charging_data_request->supported_features) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (charging_data_request->service_specification_info) {
    if (cJSON_AddStringToObject(item, "serviceSpecificationInfo", charging_data_request->service_specification_info) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [service_specification_info]");
        goto end;
    }
    }

    if (charging_data_request->multiple_unit_usage) {
    cJSON *multiple_unit_usageList = cJSON_AddArrayToObject(item, "multipleUnitUsage");
    if (multiple_unit_usageList == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [multiple_unit_usage]");
        goto end;
    }
    OpenAPI_list_for_each(charging_data_request->multiple_unit_usage, node) {
        cJSON *itemLocal = OpenAPI_multiple_unit_usage_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [multiple_unit_usage]");
            goto end;
        }
        cJSON_AddItemToArray(multiple_unit_usageList, itemLocal);
    }
    }

    if (charging_data_request->triggers) {
    cJSON *triggersList = cJSON_AddArrayToObject(item, "triggers");
    if (triggersList == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [triggers]");
        goto end;
    }
    OpenAPI_list_for_each(charging_data_request->triggers, node) {
        cJSON *itemLocal = OpenAPI_charging_trigger_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [triggers]");
            goto end;
        }
        cJSON_AddItemToArray(triggersList, itemLocal);
    }
    }

    if (charging_data_request->easid) {
    if (cJSON_AddStringToObject(item, "easid", charging_data_request->easid) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [easid]");
        goto end;
    }
    }

    if (charging_data_request->ednid) {
    if (cJSON_AddStringToObject(item, "ednid", charging_data_request->ednid) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [ednid]");
        goto end;
    }
    }

    if (charging_data_request->e_as_provider_identifier) {
    if (cJSON_AddStringToObject(item, "eASProviderIdentifier", charging_data_request->e_as_provider_identifier) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [e_as_provider_identifier]");
        goto end;
    }
    }

    if (charging_data_request->a_mfid) {
    if (cJSON_AddStringToObject(item, "aMFId", charging_data_request->a_mfid) == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [a_mfid]");
        goto end;
    }
    }

    if (charging_data_request->p_du_session_charging_information) {
    cJSON *p_du_session_charging_information_local_JSON = OpenAPI_pdu_session_charging_information_convertToJSON(charging_data_request->p_du_session_charging_information);
    if (p_du_session_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [p_du_session_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pDUSessionChargingInformation", p_du_session_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [p_du_session_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->roaming_qbc_information) {
    cJSON *roaming_qbc_information_local_JSON = OpenAPI_roaming_qbc_information_convertToJSON(charging_data_request->roaming_qbc_information);
    if (roaming_qbc_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [roaming_qbc_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "roamingQBCInformation", roaming_qbc_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [roaming_qbc_information]");
        goto end;
    }
    }

    if (charging_data_request->s_ms_charging_information) {
    cJSON *s_ms_charging_information_local_JSON = OpenAPI_sms_charging_information_convertToJSON(charging_data_request->s_ms_charging_information);
    if (s_ms_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [s_ms_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sMSChargingInformation", s_ms_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [s_ms_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->n_ef_charging_information) {
    cJSON *n_ef_charging_information_local_JSON = OpenAPI_nef_charging_information_convertToJSON(charging_data_request->n_ef_charging_information);
    if (n_ef_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n_ef_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nEFChargingInformation", n_ef_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n_ef_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->registration_charging_information) {
    cJSON *registration_charging_information_local_JSON = OpenAPI_registration_charging_information_convertToJSON(charging_data_request->registration_charging_information);
    if (registration_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [registration_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "registrationChargingInformation", registration_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [registration_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->n2_connection_charging_information) {
    cJSON *n2_connection_charging_information_local_JSON = OpenAPI_n2_connection_charging_information_convertToJSON(charging_data_request->n2_connection_charging_information);
    if (n2_connection_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n2_connection_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "n2ConnectionChargingInformation", n2_connection_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n2_connection_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->location_reporting_charging_information) {
    cJSON *location_reporting_charging_information_local_JSON = OpenAPI_location_reporting_charging_information_convertToJSON(charging_data_request->location_reporting_charging_information);
    if (location_reporting_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [location_reporting_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "locationReportingChargingInformation", location_reporting_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [location_reporting_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->n_spa_charging_information) {
    cJSON *n_spa_charging_information_local_JSON = OpenAPI_nspa_charging_information_convertToJSON(charging_data_request->n_spa_charging_information);
    if (n_spa_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n_spa_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nSPAChargingInformation", n_spa_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n_spa_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->network_sharing_charging_information) {
    cJSON *network_sharing_charging_information_local_JSON = OpenAPI_network_sharing_charging_information_convertToJSON(charging_data_request->network_sharing_charging_information);
    if (network_sharing_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [network_sharing_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "networkSharingChargingInformation", network_sharing_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [network_sharing_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->n_sm_charging_information) {
    cJSON *n_sm_charging_information_local_JSON = OpenAPI_nsm_charging_information_convertToJSON(charging_data_request->n_sm_charging_information);
    if (n_sm_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n_sm_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nSMChargingInformation", n_sm_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n_sm_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->m_m_tel_charging_information) {
    cJSON *m_m_tel_charging_information_local_JSON = OpenAPI_mm_tel_charging_information_convertToJSON(charging_data_request->m_m_tel_charging_information);
    if (m_m_tel_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [m_m_tel_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mMTelChargingInformation", m_m_tel_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [m_m_tel_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->i_ms_charging_information) {
    cJSON *i_ms_charging_information_local_JSON = OpenAPI_ims_charging_information_convertToJSON(charging_data_request->i_ms_charging_information);
    if (i_ms_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [i_ms_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "iMSChargingInformation", i_ms_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [i_ms_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->edge_infrastructure_usage_charging_information) {
    cJSON *edge_infrastructure_usage_charging_information_local_JSON = OpenAPI_edge_infrastructure_usage_charging_information_convertToJSON(charging_data_request->edge_infrastructure_usage_charging_information);
    if (edge_infrastructure_usage_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [edge_infrastructure_usage_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "edgeInfrastructureUsageChargingInformation", edge_infrastructure_usage_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [edge_infrastructure_usage_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->e_as_deployment_charging_information) {
    cJSON *e_as_deployment_charging_information_local_JSON = OpenAPI_eas_deployment_charging_information_convertToJSON(charging_data_request->e_as_deployment_charging_information);
    if (e_as_deployment_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [e_as_deployment_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "eASDeploymentChargingInformation", e_as_deployment_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [e_as_deployment_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->direct_edge_enabling_service_charging_information) {
    cJSON *direct_edge_enabling_service_charging_information_local_JSON = OpenAPI_nef_charging_information_convertToJSON(charging_data_request->direct_edge_enabling_service_charging_information);
    if (direct_edge_enabling_service_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [direct_edge_enabling_service_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "directEdgeEnablingServiceChargingInformation", direct_edge_enabling_service_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [direct_edge_enabling_service_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->exposed_edge_enabling_service_charging_information) {
    cJSON *exposed_edge_enabling_service_charging_information_local_JSON = OpenAPI_nef_charging_information_convertToJSON(charging_data_request->exposed_edge_enabling_service_charging_information);
    if (exposed_edge_enabling_service_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [exposed_edge_enabling_service_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "exposedEdgeEnablingServiceChargingInformation", exposed_edge_enabling_service_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [exposed_edge_enabling_service_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->pro_se_charging_information) {
    cJSON *pro_se_charging_information_local_JSON = OpenAPI_prose_charging_information_convertToJSON(charging_data_request->pro_se_charging_information);
    if (pro_se_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [pro_se_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "proSeChargingInformation", pro_se_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [pro_se_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->m_ms_charging_information) {
    cJSON *m_ms_charging_information_local_JSON = OpenAPI_mms_charging_information_convertToJSON(charging_data_request->m_ms_charging_information);
    if (m_ms_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [m_ms_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mMSChargingInformation", m_ms_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [m_ms_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->m_bs_session_charging_information) {
    cJSON *m_bs_session_charging_information_local_JSON = OpenAPI_mbs_session_charging_information_convertToJSON(charging_data_request->m_bs_session_charging_information);
    if (m_bs_session_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [m_bs_session_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "mBSSessionChargingInformation", m_bs_session_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [m_bs_session_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->t_sn_charging_information) {
    cJSON *t_sn_charging_information_local_JSON = OpenAPI_tsn_charging_information_convertToJSON(charging_data_request->t_sn_charging_information);
    if (t_sn_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [t_sn_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "tSNChargingInformation", t_sn_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [t_sn_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->inter_chf_information) {
    cJSON *inter_chf_information_local_JSON = OpenAPI_inter_chf_information_convertToJSON(charging_data_request->inter_chf_information);
    if (inter_chf_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [inter_chf_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "interCHFInformation", inter_chf_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [inter_chf_information]");
        goto end;
    }
    }

    if (charging_data_request->n_sacf_charging_information) {
    cJSON *n_sacf_charging_information_local_JSON = OpenAPI_nsacf_charging_information_convertToJSON(charging_data_request->n_sacf_charging_information);
    if (n_sacf_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n_sacf_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nSACFChargingInformation", n_sacf_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n_sacf_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->n_ssaa_charging_information) {
    cJSON *n_ssaa_charging_information_local_JSON = OpenAPI_nssaa_charging_information_convertToJSON(charging_data_request->n_ssaa_charging_information);
    if (n_ssaa_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n_ssaa_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "nSSAAChargingInformation", n_ssaa_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [n_ssaa_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->ranging_sl_charging_information) {
    cJSON *ranging_sl_charging_information_local_JSON = OpenAPI_ranging_sl_charging_information_convertToJSON(charging_data_request->ranging_sl_charging_information);
    if (ranging_sl_charging_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [ranging_sl_charging_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "rangingSLChargingInformation", ranging_sl_charging_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [ranging_sl_charging_information]");
        goto end;
    }
    }

    if (charging_data_request->l_cs_information) {
    cJSON *l_cs_information_local_JSON = OpenAPI_lcs_information_convertToJSON(charging_data_request->l_cs_information);
    if (l_cs_information_local_JSON == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [l_cs_information]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lCSInformation", l_cs_information_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed [l_cs_information]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_charging_data_request_t *OpenAPI_charging_data_request_parseFromJSON(cJSON *charging_data_requestJSON)
{
    OpenAPI_charging_data_request_t *charging_data_request_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *subscriber_identifier = NULL;
    cJSON *tenant_identifier = NULL;
    cJSON *charging_id = NULL;
    cJSON *mn_s_consumer_identifier = NULL;
    cJSON *nf_consumer_identification = NULL;
    OpenAPI_nf_identification_t *nf_consumer_identification_local_nonprim = NULL;
    cJSON *invocation_time_stamp = NULL;
    cJSON *invocation_sequence_number = NULL;
    cJSON *retransmission_indicator = NULL;
    cJSON *one_time_event = NULL;
    cJSON *one_time_event_type = NULL;
    OpenAPI_one_time_event_type_e one_time_event_typeVariable = 0;
    cJSON *notify_uri = NULL;
    cJSON *supported_features = NULL;
    cJSON *service_specification_info = NULL;
    cJSON *multiple_unit_usage = NULL;
    OpenAPI_list_t *multiple_unit_usageList = NULL;
    cJSON *triggers = NULL;
    OpenAPI_list_t *triggersList = NULL;
    cJSON *easid = NULL;
    cJSON *ednid = NULL;
    cJSON *e_as_provider_identifier = NULL;
    cJSON *a_mfid = NULL;
    cJSON *p_du_session_charging_information = NULL;
    OpenAPI_pdu_session_charging_information_t *p_du_session_charging_information_local_nonprim = NULL;
    cJSON *roaming_qbc_information = NULL;
    OpenAPI_roaming_qbc_information_t *roaming_qbc_information_local_nonprim = NULL;
    cJSON *s_ms_charging_information = NULL;
    OpenAPI_sms_charging_information_t *s_ms_charging_information_local_nonprim = NULL;
    cJSON *n_ef_charging_information = NULL;
    OpenAPI_nef_charging_information_t *n_ef_charging_information_local_nonprim = NULL;
    cJSON *registration_charging_information = NULL;
    OpenAPI_registration_charging_information_t *registration_charging_information_local_nonprim = NULL;
    cJSON *n2_connection_charging_information = NULL;
    OpenAPI_n2_connection_charging_information_t *n2_connection_charging_information_local_nonprim = NULL;
    cJSON *location_reporting_charging_information = NULL;
    OpenAPI_location_reporting_charging_information_t *location_reporting_charging_information_local_nonprim = NULL;
    cJSON *n_spa_charging_information = NULL;
    OpenAPI_nspa_charging_information_t *n_spa_charging_information_local_nonprim = NULL;
    cJSON *network_sharing_charging_information = NULL;
    OpenAPI_network_sharing_charging_information_t *network_sharing_charging_information_local_nonprim = NULL;
    cJSON *n_sm_charging_information = NULL;
    OpenAPI_nsm_charging_information_t *n_sm_charging_information_local_nonprim = NULL;
    cJSON *m_m_tel_charging_information = NULL;
    OpenAPI_mm_tel_charging_information_t *m_m_tel_charging_information_local_nonprim = NULL;
    cJSON *i_ms_charging_information = NULL;
    OpenAPI_ims_charging_information_t *i_ms_charging_information_local_nonprim = NULL;
    cJSON *edge_infrastructure_usage_charging_information = NULL;
    OpenAPI_edge_infrastructure_usage_charging_information_t *edge_infrastructure_usage_charging_information_local_nonprim = NULL;
    cJSON *e_as_deployment_charging_information = NULL;
    OpenAPI_eas_deployment_charging_information_t *e_as_deployment_charging_information_local_nonprim = NULL;
    cJSON *direct_edge_enabling_service_charging_information = NULL;
    OpenAPI_nef_charging_information_t *direct_edge_enabling_service_charging_information_local_nonprim = NULL;
    cJSON *exposed_edge_enabling_service_charging_information = NULL;
    OpenAPI_nef_charging_information_t *exposed_edge_enabling_service_charging_information_local_nonprim = NULL;
    cJSON *pro_se_charging_information = NULL;
    OpenAPI_prose_charging_information_t *pro_se_charging_information_local_nonprim = NULL;
    cJSON *m_ms_charging_information = NULL;
    OpenAPI_mms_charging_information_t *m_ms_charging_information_local_nonprim = NULL;
    cJSON *m_bs_session_charging_information = NULL;
    OpenAPI_mbs_session_charging_information_t *m_bs_session_charging_information_local_nonprim = NULL;
    cJSON *t_sn_charging_information = NULL;
    OpenAPI_tsn_charging_information_t *t_sn_charging_information_local_nonprim = NULL;
    cJSON *inter_chf_information = NULL;
    OpenAPI_inter_chf_information_t *inter_chf_information_local_nonprim = NULL;
    cJSON *n_sacf_charging_information = NULL;
    OpenAPI_nsacf_charging_information_t *n_sacf_charging_information_local_nonprim = NULL;
    cJSON *n_ssaa_charging_information = NULL;
    OpenAPI_nssaa_charging_information_t *n_ssaa_charging_information_local_nonprim = NULL;
    cJSON *ranging_sl_charging_information = NULL;
    OpenAPI_ranging_sl_charging_information_t *ranging_sl_charging_information_local_nonprim = NULL;
    cJSON *l_cs_information = NULL;
    OpenAPI_lcs_information_t *l_cs_information_local_nonprim = NULL;
    subscriber_identifier = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "subscriberIdentifier");
    if (subscriber_identifier) {
    if (!cJSON_IsString(subscriber_identifier) && !cJSON_IsNull(subscriber_identifier)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [subscriber_identifier]");
        goto end;
    }
    }

    tenant_identifier = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "tenantIdentifier");
    if (tenant_identifier) {
    if (!cJSON_IsString(tenant_identifier) && !cJSON_IsNull(tenant_identifier)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [tenant_identifier]");
        goto end;
    }
    }

    charging_id = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "chargingId");
    if (charging_id) {
    if (!cJSON_IsNumber(charging_id)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [charging_id]");
        goto end;
    }
    }

    mn_s_consumer_identifier = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "mnSConsumerIdentifier");
    if (mn_s_consumer_identifier) {
    if (!cJSON_IsString(mn_s_consumer_identifier) && !cJSON_IsNull(mn_s_consumer_identifier)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [mn_s_consumer_identifier]");
        goto end;
    }
    }

    nf_consumer_identification = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "nfConsumerIdentification");
    if (!nf_consumer_identification) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [nf_consumer_identification]");
        goto end;
    }
    nf_consumer_identification_local_nonprim = OpenAPI_nf_identification_parseFromJSON(nf_consumer_identification);
    if (!nf_consumer_identification_local_nonprim) {
        ogs_error("OpenAPI_nf_identification_parseFromJSON failed [nf_consumer_identification]");
        goto end;
    }

    invocation_time_stamp = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "invocationTimeStamp");
    if (!invocation_time_stamp) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [invocation_time_stamp]");
        goto end;
    }
    if (!cJSON_IsString(invocation_time_stamp) && !cJSON_IsNull(invocation_time_stamp)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [invocation_time_stamp]");
        goto end;
    }

    invocation_sequence_number = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "invocationSequenceNumber");
    if (!invocation_sequence_number) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [invocation_sequence_number]");
        goto end;
    }
    if (!cJSON_IsNumber(invocation_sequence_number)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [invocation_sequence_number]");
        goto end;
    }

    retransmission_indicator = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "retransmissionIndicator");
    if (retransmission_indicator) {
    if (!cJSON_IsBool(retransmission_indicator)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [retransmission_indicator]");
        goto end;
    }
    }

    one_time_event = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "oneTimeEvent");
    if (one_time_event) {
    if (!cJSON_IsBool(one_time_event)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [one_time_event]");
        goto end;
    }
    }

    one_time_event_type = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "oneTimeEventType");
    if (one_time_event_type) {
    if (!cJSON_IsString(one_time_event_type)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [one_time_event_type]");
        goto end;
    }
    one_time_event_typeVariable = OpenAPI_one_time_event_type_FromString(one_time_event_type->valuestring);
    }

    notify_uri = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "notifyUri");
    if (notify_uri) {
    if (!cJSON_IsString(notify_uri) && !cJSON_IsNull(notify_uri)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [notify_uri]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    service_specification_info = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "serviceSpecificationInfo");
    if (service_specification_info) {
    if (!cJSON_IsString(service_specification_info) && !cJSON_IsNull(service_specification_info)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [service_specification_info]");
        goto end;
    }
    }

    multiple_unit_usage = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "multipleUnitUsage");
    if (multiple_unit_usage) {
        cJSON *multiple_unit_usage_local = NULL;
        if (!cJSON_IsArray(multiple_unit_usage)) {
            ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [multiple_unit_usage]");
            goto end;
        }

        multiple_unit_usageList = OpenAPI_list_create();

        cJSON_ArrayForEach(multiple_unit_usage_local, multiple_unit_usage) {
            if (!cJSON_IsObject(multiple_unit_usage_local)) {
                ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [multiple_unit_usage]");
                goto end;
            }
            OpenAPI_multiple_unit_usage_t *multiple_unit_usageItem = OpenAPI_multiple_unit_usage_parseFromJSON(multiple_unit_usage_local);
            if (!multiple_unit_usageItem) {
                ogs_error("No multiple_unit_usageItem");
                goto end;
            }
            OpenAPI_list_add(multiple_unit_usageList, multiple_unit_usageItem);
        }
    }

    triggers = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "triggers");
    if (triggers) {
        cJSON *triggers_local = NULL;
        if (!cJSON_IsArray(triggers)) {
            ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [triggers]");
            goto end;
        }

        triggersList = OpenAPI_list_create();

        cJSON_ArrayForEach(triggers_local, triggers) {
            if (!cJSON_IsObject(triggers_local)) {
                ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [triggers]");
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

    easid = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "easid");
    if (easid) {
    if (!cJSON_IsString(easid) && !cJSON_IsNull(easid)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [easid]");
        goto end;
    }
    }

    ednid = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "ednid");
    if (ednid) {
    if (!cJSON_IsString(ednid) && !cJSON_IsNull(ednid)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [ednid]");
        goto end;
    }
    }

    e_as_provider_identifier = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "eASProviderIdentifier");
    if (e_as_provider_identifier) {
    if (!cJSON_IsString(e_as_provider_identifier) && !cJSON_IsNull(e_as_provider_identifier)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [e_as_provider_identifier]");
        goto end;
    }
    }

    a_mfid = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "aMFId");
    if (a_mfid) {
    if (!cJSON_IsString(a_mfid) && !cJSON_IsNull(a_mfid)) {
        ogs_error("OpenAPI_charging_data_request_parseFromJSON() failed [a_mfid]");
        goto end;
    }
    }

    p_du_session_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "pDUSessionChargingInformation");
    if (p_du_session_charging_information) {
    p_du_session_charging_information_local_nonprim = OpenAPI_pdu_session_charging_information_parseFromJSON(p_du_session_charging_information);
    if (!p_du_session_charging_information_local_nonprim) {
        ogs_error("OpenAPI_pdu_session_charging_information_parseFromJSON failed [p_du_session_charging_information]");
        goto end;
    }
    }

    roaming_qbc_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "roamingQBCInformation");
    if (roaming_qbc_information) {
    roaming_qbc_information_local_nonprim = OpenAPI_roaming_qbc_information_parseFromJSON(roaming_qbc_information);
    if (!roaming_qbc_information_local_nonprim) {
        ogs_error("OpenAPI_roaming_qbc_information_parseFromJSON failed [roaming_qbc_information]");
        goto end;
    }
    }

    s_ms_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "sMSChargingInformation");
    if (s_ms_charging_information) {
    s_ms_charging_information_local_nonprim = OpenAPI_sms_charging_information_parseFromJSON(s_ms_charging_information);
    if (!s_ms_charging_information_local_nonprim) {
        ogs_error("OpenAPI_sms_charging_information_parseFromJSON failed [s_ms_charging_information]");
        goto end;
    }
    }

    n_ef_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "nEFChargingInformation");
    if (n_ef_charging_information) {
    n_ef_charging_information_local_nonprim = OpenAPI_nef_charging_information_parseFromJSON(n_ef_charging_information);
    if (!n_ef_charging_information_local_nonprim) {
        ogs_error("OpenAPI_nef_charging_information_parseFromJSON failed [n_ef_charging_information]");
        goto end;
    }
    }

    registration_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "registrationChargingInformation");
    if (registration_charging_information) {
    registration_charging_information_local_nonprim = OpenAPI_registration_charging_information_parseFromJSON(registration_charging_information);
    if (!registration_charging_information_local_nonprim) {
        ogs_error("OpenAPI_registration_charging_information_parseFromJSON failed [registration_charging_information]");
        goto end;
    }
    }

    n2_connection_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "n2ConnectionChargingInformation");
    if (n2_connection_charging_information) {
    n2_connection_charging_information_local_nonprim = OpenAPI_n2_connection_charging_information_parseFromJSON(n2_connection_charging_information);
    if (!n2_connection_charging_information_local_nonprim) {
        ogs_error("OpenAPI_n2_connection_charging_information_parseFromJSON failed [n2_connection_charging_information]");
        goto end;
    }
    }

    location_reporting_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "locationReportingChargingInformation");
    if (location_reporting_charging_information) {
    location_reporting_charging_information_local_nonprim = OpenAPI_location_reporting_charging_information_parseFromJSON(location_reporting_charging_information);
    if (!location_reporting_charging_information_local_nonprim) {
        ogs_error("OpenAPI_location_reporting_charging_information_parseFromJSON failed [location_reporting_charging_information]");
        goto end;
    }
    }

    n_spa_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "nSPAChargingInformation");
    if (n_spa_charging_information) {
    n_spa_charging_information_local_nonprim = OpenAPI_nspa_charging_information_parseFromJSON(n_spa_charging_information);
    if (!n_spa_charging_information_local_nonprim) {
        ogs_error("OpenAPI_nspa_charging_information_parseFromJSON failed [n_spa_charging_information]");
        goto end;
    }
    }

    network_sharing_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "networkSharingChargingInformation");
    if (network_sharing_charging_information) {
    network_sharing_charging_information_local_nonprim = OpenAPI_network_sharing_charging_information_parseFromJSON(network_sharing_charging_information);
    if (!network_sharing_charging_information_local_nonprim) {
        ogs_error("OpenAPI_network_sharing_charging_information_parseFromJSON failed [network_sharing_charging_information]");
        goto end;
    }
    }

    n_sm_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "nSMChargingInformation");
    if (n_sm_charging_information) {
    n_sm_charging_information_local_nonprim = OpenAPI_nsm_charging_information_parseFromJSON(n_sm_charging_information);
    if (!n_sm_charging_information_local_nonprim) {
        ogs_error("OpenAPI_nsm_charging_information_parseFromJSON failed [n_sm_charging_information]");
        goto end;
    }
    }

    m_m_tel_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "mMTelChargingInformation");
    if (m_m_tel_charging_information) {
    m_m_tel_charging_information_local_nonprim = OpenAPI_mm_tel_charging_information_parseFromJSON(m_m_tel_charging_information);
    if (!m_m_tel_charging_information_local_nonprim) {
        ogs_error("OpenAPI_mm_tel_charging_information_parseFromJSON failed [m_m_tel_charging_information]");
        goto end;
    }
    }

    i_ms_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "iMSChargingInformation");
    if (i_ms_charging_information) {
    i_ms_charging_information_local_nonprim = OpenAPI_ims_charging_information_parseFromJSON(i_ms_charging_information);
    if (!i_ms_charging_information_local_nonprim) {
        ogs_error("OpenAPI_ims_charging_information_parseFromJSON failed [i_ms_charging_information]");
        goto end;
    }
    }

    edge_infrastructure_usage_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "edgeInfrastructureUsageChargingInformation");
    if (edge_infrastructure_usage_charging_information) {
    edge_infrastructure_usage_charging_information_local_nonprim = OpenAPI_edge_infrastructure_usage_charging_information_parseFromJSON(edge_infrastructure_usage_charging_information);
    if (!edge_infrastructure_usage_charging_information_local_nonprim) {
        ogs_error("OpenAPI_edge_infrastructure_usage_charging_information_parseFromJSON failed [edge_infrastructure_usage_charging_information]");
        goto end;
    }
    }

    e_as_deployment_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "eASDeploymentChargingInformation");
    if (e_as_deployment_charging_information) {
    e_as_deployment_charging_information_local_nonprim = OpenAPI_eas_deployment_charging_information_parseFromJSON(e_as_deployment_charging_information);
    if (!e_as_deployment_charging_information_local_nonprim) {
        ogs_error("OpenAPI_eas_deployment_charging_information_parseFromJSON failed [e_as_deployment_charging_information]");
        goto end;
    }
    }

    direct_edge_enabling_service_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "directEdgeEnablingServiceChargingInformation");
    if (direct_edge_enabling_service_charging_information) {
    direct_edge_enabling_service_charging_information_local_nonprim = OpenAPI_nef_charging_information_parseFromJSON(direct_edge_enabling_service_charging_information);
    if (!direct_edge_enabling_service_charging_information_local_nonprim) {
        ogs_error("OpenAPI_nef_charging_information_parseFromJSON failed [direct_edge_enabling_service_charging_information]");
        goto end;
    }
    }

    exposed_edge_enabling_service_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "exposedEdgeEnablingServiceChargingInformation");
    if (exposed_edge_enabling_service_charging_information) {
    exposed_edge_enabling_service_charging_information_local_nonprim = OpenAPI_nef_charging_information_parseFromJSON(exposed_edge_enabling_service_charging_information);
    if (!exposed_edge_enabling_service_charging_information_local_nonprim) {
        ogs_error("OpenAPI_nef_charging_information_parseFromJSON failed [exposed_edge_enabling_service_charging_information]");
        goto end;
    }
    }

    pro_se_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "proSeChargingInformation");
    if (pro_se_charging_information) {
    pro_se_charging_information_local_nonprim = OpenAPI_prose_charging_information_parseFromJSON(pro_se_charging_information);
    if (!pro_se_charging_information_local_nonprim) {
        ogs_error("OpenAPI_prose_charging_information_parseFromJSON failed [pro_se_charging_information]");
        goto end;
    }
    }

    m_ms_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "mMSChargingInformation");
    if (m_ms_charging_information) {
    m_ms_charging_information_local_nonprim = OpenAPI_mms_charging_information_parseFromJSON(m_ms_charging_information);
    if (!m_ms_charging_information_local_nonprim) {
        ogs_error("OpenAPI_mms_charging_information_parseFromJSON failed [m_ms_charging_information]");
        goto end;
    }
    }

    m_bs_session_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "mBSSessionChargingInformation");
    if (m_bs_session_charging_information) {
    m_bs_session_charging_information_local_nonprim = OpenAPI_mbs_session_charging_information_parseFromJSON(m_bs_session_charging_information);
    if (!m_bs_session_charging_information_local_nonprim) {
        ogs_error("OpenAPI_mbs_session_charging_information_parseFromJSON failed [m_bs_session_charging_information]");
        goto end;
    }
    }

    t_sn_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "tSNChargingInformation");
    if (t_sn_charging_information) {
    t_sn_charging_information_local_nonprim = OpenAPI_tsn_charging_information_parseFromJSON(t_sn_charging_information);
    if (!t_sn_charging_information_local_nonprim) {
        ogs_error("OpenAPI_tsn_charging_information_parseFromJSON failed [t_sn_charging_information]");
        goto end;
    }
    }

    inter_chf_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "interCHFInformation");
    if (inter_chf_information) {
    inter_chf_information_local_nonprim = OpenAPI_inter_chf_information_parseFromJSON(inter_chf_information);
    if (!inter_chf_information_local_nonprim) {
        ogs_error("OpenAPI_inter_chf_information_parseFromJSON failed [inter_chf_information]");
        goto end;
    }
    }

    n_sacf_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "nSACFChargingInformation");
    if (n_sacf_charging_information) {
    n_sacf_charging_information_local_nonprim = OpenAPI_nsacf_charging_information_parseFromJSON(n_sacf_charging_information);
    if (!n_sacf_charging_information_local_nonprim) {
        ogs_error("OpenAPI_nsacf_charging_information_parseFromJSON failed [n_sacf_charging_information]");
        goto end;
    }
    }

    n_ssaa_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "nSSAAChargingInformation");
    if (n_ssaa_charging_information) {
    n_ssaa_charging_information_local_nonprim = OpenAPI_nssaa_charging_information_parseFromJSON(n_ssaa_charging_information);
    if (!n_ssaa_charging_information_local_nonprim) {
        ogs_error("OpenAPI_nssaa_charging_information_parseFromJSON failed [n_ssaa_charging_information]");
        goto end;
    }
    }

    ranging_sl_charging_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "rangingSLChargingInformation");
    if (ranging_sl_charging_information) {
    ranging_sl_charging_information_local_nonprim = OpenAPI_ranging_sl_charging_information_parseFromJSON(ranging_sl_charging_information);
    if (!ranging_sl_charging_information_local_nonprim) {
        ogs_error("OpenAPI_ranging_sl_charging_information_parseFromJSON failed [ranging_sl_charging_information]");
        goto end;
    }
    }

    l_cs_information = cJSON_GetObjectItemCaseSensitive(charging_data_requestJSON, "lCSInformation");
    if (l_cs_information) {
    l_cs_information_local_nonprim = OpenAPI_lcs_information_parseFromJSON(l_cs_information);
    if (!l_cs_information_local_nonprim) {
        ogs_error("OpenAPI_lcs_information_parseFromJSON failed [l_cs_information]");
        goto end;
    }
    }

    charging_data_request_local_var = OpenAPI_charging_data_request_create (
        subscriber_identifier && !cJSON_IsNull(subscriber_identifier) ? ogs_strdup(subscriber_identifier->valuestring) : NULL,
        tenant_identifier && !cJSON_IsNull(tenant_identifier) ? ogs_strdup(tenant_identifier->valuestring) : NULL,
        charging_id ? true : false,
        charging_id ? charging_id->valuedouble : 0,
        mn_s_consumer_identifier && !cJSON_IsNull(mn_s_consumer_identifier) ? ogs_strdup(mn_s_consumer_identifier->valuestring) : NULL,
        nf_consumer_identification_local_nonprim,
        ogs_strdup(invocation_time_stamp->valuestring),
        
        invocation_sequence_number->valuedouble,
        retransmission_indicator ? true : false,
        retransmission_indicator ? retransmission_indicator->valueint : 0,
        one_time_event ? true : false,
        one_time_event ? one_time_event->valueint : 0,
        one_time_event_type ? one_time_event_typeVariable : 0,
        notify_uri && !cJSON_IsNull(notify_uri) ? ogs_strdup(notify_uri->valuestring) : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        service_specification_info && !cJSON_IsNull(service_specification_info) ? ogs_strdup(service_specification_info->valuestring) : NULL,
        multiple_unit_usage ? multiple_unit_usageList : NULL,
        triggers ? triggersList : NULL,
        easid && !cJSON_IsNull(easid) ? ogs_strdup(easid->valuestring) : NULL,
        ednid && !cJSON_IsNull(ednid) ? ogs_strdup(ednid->valuestring) : NULL,
        e_as_provider_identifier && !cJSON_IsNull(e_as_provider_identifier) ? ogs_strdup(e_as_provider_identifier->valuestring) : NULL,
        a_mfid && !cJSON_IsNull(a_mfid) ? ogs_strdup(a_mfid->valuestring) : NULL,
        p_du_session_charging_information ? p_du_session_charging_information_local_nonprim : NULL,
        roaming_qbc_information ? roaming_qbc_information_local_nonprim : NULL,
        s_ms_charging_information ? s_ms_charging_information_local_nonprim : NULL,
        n_ef_charging_information ? n_ef_charging_information_local_nonprim : NULL,
        registration_charging_information ? registration_charging_information_local_nonprim : NULL,
        n2_connection_charging_information ? n2_connection_charging_information_local_nonprim : NULL,
        location_reporting_charging_information ? location_reporting_charging_information_local_nonprim : NULL,
        n_spa_charging_information ? n_spa_charging_information_local_nonprim : NULL,
        network_sharing_charging_information ? network_sharing_charging_information_local_nonprim : NULL,
        n_sm_charging_information ? n_sm_charging_information_local_nonprim : NULL,
        m_m_tel_charging_information ? m_m_tel_charging_information_local_nonprim : NULL,
        i_ms_charging_information ? i_ms_charging_information_local_nonprim : NULL,
        edge_infrastructure_usage_charging_information ? edge_infrastructure_usage_charging_information_local_nonprim : NULL,
        e_as_deployment_charging_information ? e_as_deployment_charging_information_local_nonprim : NULL,
        direct_edge_enabling_service_charging_information ? direct_edge_enabling_service_charging_information_local_nonprim : NULL,
        exposed_edge_enabling_service_charging_information ? exposed_edge_enabling_service_charging_information_local_nonprim : NULL,
        pro_se_charging_information ? pro_se_charging_information_local_nonprim : NULL,
        m_ms_charging_information ? m_ms_charging_information_local_nonprim : NULL,
        m_bs_session_charging_information ? m_bs_session_charging_information_local_nonprim : NULL,
        t_sn_charging_information ? t_sn_charging_information_local_nonprim : NULL,
        inter_chf_information ? inter_chf_information_local_nonprim : NULL,
        n_sacf_charging_information ? n_sacf_charging_information_local_nonprim : NULL,
        n_ssaa_charging_information ? n_ssaa_charging_information_local_nonprim : NULL,
        ranging_sl_charging_information ? ranging_sl_charging_information_local_nonprim : NULL,
        l_cs_information ? l_cs_information_local_nonprim : NULL
    );

    return charging_data_request_local_var;
end:
    if (nf_consumer_identification_local_nonprim) {
        OpenAPI_nf_identification_free(nf_consumer_identification_local_nonprim);
        nf_consumer_identification_local_nonprim = NULL;
    }
    if (multiple_unit_usageList) {
        OpenAPI_list_for_each(multiple_unit_usageList, node) {
            OpenAPI_multiple_unit_usage_free(node->data);
        }
        OpenAPI_list_free(multiple_unit_usageList);
        multiple_unit_usageList = NULL;
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
    if (s_ms_charging_information_local_nonprim) {
        OpenAPI_sms_charging_information_free(s_ms_charging_information_local_nonprim);
        s_ms_charging_information_local_nonprim = NULL;
    }
    if (n_ef_charging_information_local_nonprim) {
        OpenAPI_nef_charging_information_free(n_ef_charging_information_local_nonprim);
        n_ef_charging_information_local_nonprim = NULL;
    }
    if (registration_charging_information_local_nonprim) {
        OpenAPI_registration_charging_information_free(registration_charging_information_local_nonprim);
        registration_charging_information_local_nonprim = NULL;
    }
    if (n2_connection_charging_information_local_nonprim) {
        OpenAPI_n2_connection_charging_information_free(n2_connection_charging_information_local_nonprim);
        n2_connection_charging_information_local_nonprim = NULL;
    }
    if (location_reporting_charging_information_local_nonprim) {
        OpenAPI_location_reporting_charging_information_free(location_reporting_charging_information_local_nonprim);
        location_reporting_charging_information_local_nonprim = NULL;
    }
    if (n_spa_charging_information_local_nonprim) {
        OpenAPI_nspa_charging_information_free(n_spa_charging_information_local_nonprim);
        n_spa_charging_information_local_nonprim = NULL;
    }
    if (network_sharing_charging_information_local_nonprim) {
        OpenAPI_network_sharing_charging_information_free(network_sharing_charging_information_local_nonprim);
        network_sharing_charging_information_local_nonprim = NULL;
    }
    if (n_sm_charging_information_local_nonprim) {
        OpenAPI_nsm_charging_information_free(n_sm_charging_information_local_nonprim);
        n_sm_charging_information_local_nonprim = NULL;
    }
    if (m_m_tel_charging_information_local_nonprim) {
        OpenAPI_mm_tel_charging_information_free(m_m_tel_charging_information_local_nonprim);
        m_m_tel_charging_information_local_nonprim = NULL;
    }
    if (i_ms_charging_information_local_nonprim) {
        OpenAPI_ims_charging_information_free(i_ms_charging_information_local_nonprim);
        i_ms_charging_information_local_nonprim = NULL;
    }
    if (edge_infrastructure_usage_charging_information_local_nonprim) {
        OpenAPI_edge_infrastructure_usage_charging_information_free(edge_infrastructure_usage_charging_information_local_nonprim);
        edge_infrastructure_usage_charging_information_local_nonprim = NULL;
    }
    if (e_as_deployment_charging_information_local_nonprim) {
        OpenAPI_eas_deployment_charging_information_free(e_as_deployment_charging_information_local_nonprim);
        e_as_deployment_charging_information_local_nonprim = NULL;
    }
    if (direct_edge_enabling_service_charging_information_local_nonprim) {
        OpenAPI_nef_charging_information_free(direct_edge_enabling_service_charging_information_local_nonprim);
        direct_edge_enabling_service_charging_information_local_nonprim = NULL;
    }
    if (exposed_edge_enabling_service_charging_information_local_nonprim) {
        OpenAPI_nef_charging_information_free(exposed_edge_enabling_service_charging_information_local_nonprim);
        exposed_edge_enabling_service_charging_information_local_nonprim = NULL;
    }
    if (pro_se_charging_information_local_nonprim) {
        OpenAPI_prose_charging_information_free(pro_se_charging_information_local_nonprim);
        pro_se_charging_information_local_nonprim = NULL;
    }
    if (m_ms_charging_information_local_nonprim) {
        OpenAPI_mms_charging_information_free(m_ms_charging_information_local_nonprim);
        m_ms_charging_information_local_nonprim = NULL;
    }
    if (m_bs_session_charging_information_local_nonprim) {
        OpenAPI_mbs_session_charging_information_free(m_bs_session_charging_information_local_nonprim);
        m_bs_session_charging_information_local_nonprim = NULL;
    }
    if (t_sn_charging_information_local_nonprim) {
        OpenAPI_tsn_charging_information_free(t_sn_charging_information_local_nonprim);
        t_sn_charging_information_local_nonprim = NULL;
    }
    if (inter_chf_information_local_nonprim) {
        OpenAPI_inter_chf_information_free(inter_chf_information_local_nonprim);
        inter_chf_information_local_nonprim = NULL;
    }
    if (n_sacf_charging_information_local_nonprim) {
        OpenAPI_nsacf_charging_information_free(n_sacf_charging_information_local_nonprim);
        n_sacf_charging_information_local_nonprim = NULL;
    }
    if (n_ssaa_charging_information_local_nonprim) {
        OpenAPI_nssaa_charging_information_free(n_ssaa_charging_information_local_nonprim);
        n_ssaa_charging_information_local_nonprim = NULL;
    }
    if (ranging_sl_charging_information_local_nonprim) {
        OpenAPI_ranging_sl_charging_information_free(ranging_sl_charging_information_local_nonprim);
        ranging_sl_charging_information_local_nonprim = NULL;
    }
    if (l_cs_information_local_nonprim) {
        OpenAPI_lcs_information_free(l_cs_information_local_nonprim);
        l_cs_information_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_charging_data_request_t *OpenAPI_charging_data_request_copy(OpenAPI_charging_data_request_t *dst, OpenAPI_charging_data_request_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_charging_data_request_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_charging_data_request_convertToJSON() failed");
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

    OpenAPI_charging_data_request_free(dst);
    dst = OpenAPI_charging_data_request_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

