
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "monitoring_configuration_1.h"

OpenAPI_monitoring_configuration_1_t *OpenAPI_monitoring_configuration_1_create(
    OpenAPI_event_type_e event_type,
    bool is_immediate_flag,
    int immediate_flag,
    OpenAPI_location_reporting_configuration_1_t *location_reporting_configuration,
    OpenAPI_association_type_e association_type,
    OpenAPI_datalink_reporting_configuration_1_t *datalink_report_cfg,
    OpenAPI_loss_connectivity_cfg_1_t *loss_connectivity_cfg,
    bool is_maximum_latency,
    int maximum_latency,
    bool is_maximum_response_time,
    int maximum_response_time,
    bool is_suggested_packet_num_dl,
    int suggested_packet_num_dl,
    char *dnn,
    OpenAPI_snssai_t *single_nssai,
    char *app_id,
    OpenAPI_pdu_session_status_cfg_1_t *pdu_session_status_cfg,
    OpenAPI_reachability_for_sms_configuration_e reachability_for_sms_cfg,
    char *mtc_provider_information,
    char *af_id,
    OpenAPI_reachability_for_data_configuration_1_t *reachability_for_data_cfg,
    bool is_idle_status_ind,
    int idle_status_ind,
    OpenAPI_monitoring_suspension_1_t *monitoring_suspension,
    OpenAPI_list_t *shared_monitoring_suspension_id_list,
    bool is_pei_requested,
    int pei_requested
)
{
    OpenAPI_monitoring_configuration_1_t *monitoring_configuration_1_local_var = ogs_malloc(sizeof(OpenAPI_monitoring_configuration_1_t));
    ogs_assert(monitoring_configuration_1_local_var);

    monitoring_configuration_1_local_var->event_type = event_type;
    monitoring_configuration_1_local_var->is_immediate_flag = is_immediate_flag;
    monitoring_configuration_1_local_var->immediate_flag = immediate_flag;
    monitoring_configuration_1_local_var->location_reporting_configuration = location_reporting_configuration;
    monitoring_configuration_1_local_var->association_type = association_type;
    monitoring_configuration_1_local_var->datalink_report_cfg = datalink_report_cfg;
    monitoring_configuration_1_local_var->loss_connectivity_cfg = loss_connectivity_cfg;
    monitoring_configuration_1_local_var->is_maximum_latency = is_maximum_latency;
    monitoring_configuration_1_local_var->maximum_latency = maximum_latency;
    monitoring_configuration_1_local_var->is_maximum_response_time = is_maximum_response_time;
    monitoring_configuration_1_local_var->maximum_response_time = maximum_response_time;
    monitoring_configuration_1_local_var->is_suggested_packet_num_dl = is_suggested_packet_num_dl;
    monitoring_configuration_1_local_var->suggested_packet_num_dl = suggested_packet_num_dl;
    monitoring_configuration_1_local_var->dnn = dnn;
    monitoring_configuration_1_local_var->single_nssai = single_nssai;
    monitoring_configuration_1_local_var->app_id = app_id;
    monitoring_configuration_1_local_var->pdu_session_status_cfg = pdu_session_status_cfg;
    monitoring_configuration_1_local_var->reachability_for_sms_cfg = reachability_for_sms_cfg;
    monitoring_configuration_1_local_var->mtc_provider_information = mtc_provider_information;
    monitoring_configuration_1_local_var->af_id = af_id;
    monitoring_configuration_1_local_var->reachability_for_data_cfg = reachability_for_data_cfg;
    monitoring_configuration_1_local_var->is_idle_status_ind = is_idle_status_ind;
    monitoring_configuration_1_local_var->idle_status_ind = idle_status_ind;
    monitoring_configuration_1_local_var->monitoring_suspension = monitoring_suspension;
    monitoring_configuration_1_local_var->shared_monitoring_suspension_id_list = shared_monitoring_suspension_id_list;
    monitoring_configuration_1_local_var->is_pei_requested = is_pei_requested;
    monitoring_configuration_1_local_var->pei_requested = pei_requested;

    return monitoring_configuration_1_local_var;
}

void OpenAPI_monitoring_configuration_1_free(OpenAPI_monitoring_configuration_1_t *monitoring_configuration_1)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == monitoring_configuration_1) {
        return;
    }
    if (monitoring_configuration_1->location_reporting_configuration) {
        OpenAPI_location_reporting_configuration_1_free(monitoring_configuration_1->location_reporting_configuration);
        monitoring_configuration_1->location_reporting_configuration = NULL;
    }
    if (monitoring_configuration_1->datalink_report_cfg) {
        OpenAPI_datalink_reporting_configuration_1_free(monitoring_configuration_1->datalink_report_cfg);
        monitoring_configuration_1->datalink_report_cfg = NULL;
    }
    if (monitoring_configuration_1->loss_connectivity_cfg) {
        OpenAPI_loss_connectivity_cfg_1_free(monitoring_configuration_1->loss_connectivity_cfg);
        monitoring_configuration_1->loss_connectivity_cfg = NULL;
    }
    if (monitoring_configuration_1->dnn) {
        ogs_free(monitoring_configuration_1->dnn);
        monitoring_configuration_1->dnn = NULL;
    }
    if (monitoring_configuration_1->single_nssai) {
        OpenAPI_snssai_free(monitoring_configuration_1->single_nssai);
        monitoring_configuration_1->single_nssai = NULL;
    }
    if (monitoring_configuration_1->app_id) {
        ogs_free(monitoring_configuration_1->app_id);
        monitoring_configuration_1->app_id = NULL;
    }
    if (monitoring_configuration_1->pdu_session_status_cfg) {
        OpenAPI_pdu_session_status_cfg_1_free(monitoring_configuration_1->pdu_session_status_cfg);
        monitoring_configuration_1->pdu_session_status_cfg = NULL;
    }
    if (monitoring_configuration_1->mtc_provider_information) {
        ogs_free(monitoring_configuration_1->mtc_provider_information);
        monitoring_configuration_1->mtc_provider_information = NULL;
    }
    if (monitoring_configuration_1->af_id) {
        ogs_free(monitoring_configuration_1->af_id);
        monitoring_configuration_1->af_id = NULL;
    }
    if (monitoring_configuration_1->reachability_for_data_cfg) {
        OpenAPI_reachability_for_data_configuration_1_free(monitoring_configuration_1->reachability_for_data_cfg);
        monitoring_configuration_1->reachability_for_data_cfg = NULL;
    }
    if (monitoring_configuration_1->monitoring_suspension) {
        OpenAPI_monitoring_suspension_1_free(monitoring_configuration_1->monitoring_suspension);
        monitoring_configuration_1->monitoring_suspension = NULL;
    }
    if (monitoring_configuration_1->shared_monitoring_suspension_id_list) {
        OpenAPI_list_for_each(monitoring_configuration_1->shared_monitoring_suspension_id_list, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(monitoring_configuration_1->shared_monitoring_suspension_id_list);
        monitoring_configuration_1->shared_monitoring_suspension_id_list = NULL;
    }
    ogs_free(monitoring_configuration_1);
}

cJSON *OpenAPI_monitoring_configuration_1_convertToJSON(OpenAPI_monitoring_configuration_1_t *monitoring_configuration_1)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (monitoring_configuration_1 == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [MonitoringConfiguration_1]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (monitoring_configuration_1->event_type == OpenAPI_event_type_NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [event_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "eventType", OpenAPI_event_type_ToString(monitoring_configuration_1->event_type)) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [event_type]");
        goto end;
    }

    if (monitoring_configuration_1->is_immediate_flag) {
    if (cJSON_AddBoolToObject(item, "immediateFlag", monitoring_configuration_1->immediate_flag) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [immediate_flag]");
        goto end;
    }
    }

    if (monitoring_configuration_1->location_reporting_configuration) {
    cJSON *location_reporting_configuration_local_JSON = OpenAPI_location_reporting_configuration_1_convertToJSON(monitoring_configuration_1->location_reporting_configuration);
    if (location_reporting_configuration_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [location_reporting_configuration]");
        goto end;
    }
    cJSON_AddItemToObject(item, "locationReportingConfiguration", location_reporting_configuration_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [location_reporting_configuration]");
        goto end;
    }
    }

    if (monitoring_configuration_1->association_type != OpenAPI_association_type_NULL) {
    if (cJSON_AddStringToObject(item, "associationType", OpenAPI_association_type_ToString(monitoring_configuration_1->association_type)) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [association_type]");
        goto end;
    }
    }

    if (monitoring_configuration_1->datalink_report_cfg) {
    cJSON *datalink_report_cfg_local_JSON = OpenAPI_datalink_reporting_configuration_1_convertToJSON(monitoring_configuration_1->datalink_report_cfg);
    if (datalink_report_cfg_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [datalink_report_cfg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "datalinkReportCfg", datalink_report_cfg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [datalink_report_cfg]");
        goto end;
    }
    }

    if (monitoring_configuration_1->loss_connectivity_cfg) {
    cJSON *loss_connectivity_cfg_local_JSON = OpenAPI_loss_connectivity_cfg_1_convertToJSON(monitoring_configuration_1->loss_connectivity_cfg);
    if (loss_connectivity_cfg_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [loss_connectivity_cfg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lossConnectivityCfg", loss_connectivity_cfg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [loss_connectivity_cfg]");
        goto end;
    }
    }

    if (monitoring_configuration_1->is_maximum_latency) {
    if (cJSON_AddNumberToObject(item, "maximumLatency", monitoring_configuration_1->maximum_latency) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [maximum_latency]");
        goto end;
    }
    }

    if (monitoring_configuration_1->is_maximum_response_time) {
    if (cJSON_AddNumberToObject(item, "maximumResponseTime", monitoring_configuration_1->maximum_response_time) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [maximum_response_time]");
        goto end;
    }
    }

    if (monitoring_configuration_1->is_suggested_packet_num_dl) {
    if (cJSON_AddNumberToObject(item, "suggestedPacketNumDl", monitoring_configuration_1->suggested_packet_num_dl) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [suggested_packet_num_dl]");
        goto end;
    }
    }

    if (monitoring_configuration_1->dnn) {
    if (cJSON_AddStringToObject(item, "dnn", monitoring_configuration_1->dnn) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [dnn]");
        goto end;
    }
    }

    if (monitoring_configuration_1->single_nssai) {
    cJSON *single_nssai_local_JSON = OpenAPI_snssai_convertToJSON(monitoring_configuration_1->single_nssai);
    if (single_nssai_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [single_nssai]");
        goto end;
    }
    cJSON_AddItemToObject(item, "singleNssai", single_nssai_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [single_nssai]");
        goto end;
    }
    }

    if (monitoring_configuration_1->app_id) {
    if (cJSON_AddStringToObject(item, "appId", monitoring_configuration_1->app_id) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [app_id]");
        goto end;
    }
    }

    if (monitoring_configuration_1->pdu_session_status_cfg) {
    cJSON *pdu_session_status_cfg_local_JSON = OpenAPI_pdu_session_status_cfg_1_convertToJSON(monitoring_configuration_1->pdu_session_status_cfg);
    if (pdu_session_status_cfg_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [pdu_session_status_cfg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSessionStatusCfg", pdu_session_status_cfg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [pdu_session_status_cfg]");
        goto end;
    }
    }

    if (monitoring_configuration_1->reachability_for_sms_cfg != OpenAPI_reachability_for_sms_configuration_NULL) {
    if (cJSON_AddStringToObject(item, "reachabilityForSmsCfg", OpenAPI_reachability_for_sms_configuration_ToString(monitoring_configuration_1->reachability_for_sms_cfg)) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [reachability_for_sms_cfg]");
        goto end;
    }
    }

    if (monitoring_configuration_1->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", monitoring_configuration_1->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    if (monitoring_configuration_1->af_id) {
    if (cJSON_AddStringToObject(item, "afId", monitoring_configuration_1->af_id) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [af_id]");
        goto end;
    }
    }

    if (monitoring_configuration_1->reachability_for_data_cfg) {
    cJSON *reachability_for_data_cfg_local_JSON = OpenAPI_reachability_for_data_configuration_1_convertToJSON(monitoring_configuration_1->reachability_for_data_cfg);
    if (reachability_for_data_cfg_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [reachability_for_data_cfg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "reachabilityForDataCfg", reachability_for_data_cfg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [reachability_for_data_cfg]");
        goto end;
    }
    }

    if (monitoring_configuration_1->is_idle_status_ind) {
    if (cJSON_AddBoolToObject(item, "idleStatusInd", monitoring_configuration_1->idle_status_ind) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [idle_status_ind]");
        goto end;
    }
    }

    if (monitoring_configuration_1->monitoring_suspension) {
    cJSON *monitoring_suspension_local_JSON = OpenAPI_monitoring_suspension_1_convertToJSON(monitoring_configuration_1->monitoring_suspension);
    if (monitoring_suspension_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [monitoring_suspension]");
        goto end;
    }
    cJSON_AddItemToObject(item, "monitoringSuspension", monitoring_suspension_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [monitoring_suspension]");
        goto end;
    }
    }

    if (monitoring_configuration_1->shared_monitoring_suspension_id_list) {
    cJSON *shared_monitoring_suspension_id_listList = cJSON_AddArrayToObject(item, "sharedMonitoringSuspensionIdList");
    if (shared_monitoring_suspension_id_listList == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [shared_monitoring_suspension_id_list]");
        goto end;
    }
    OpenAPI_list_for_each(monitoring_configuration_1->shared_monitoring_suspension_id_list, node) {
        if (cJSON_AddStringToObject(shared_monitoring_suspension_id_listList, "", (char*)node->data) == NULL) {
            ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [shared_monitoring_suspension_id_list]");
            goto end;
        }
    }
    }

    if (monitoring_configuration_1->is_pei_requested) {
    if (cJSON_AddBoolToObject(item, "peiRequested", monitoring_configuration_1->pei_requested) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed [pei_requested]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_monitoring_configuration_1_t *OpenAPI_monitoring_configuration_1_parseFromJSON(cJSON *monitoring_configuration_1JSON)
{
    OpenAPI_monitoring_configuration_1_t *monitoring_configuration_1_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *event_type = NULL;
    OpenAPI_event_type_e event_typeVariable = 0;
    cJSON *immediate_flag = NULL;
    cJSON *location_reporting_configuration = NULL;
    OpenAPI_location_reporting_configuration_1_t *location_reporting_configuration_local_nonprim = NULL;
    cJSON *association_type = NULL;
    OpenAPI_association_type_e association_typeVariable = 0;
    cJSON *datalink_report_cfg = NULL;
    OpenAPI_datalink_reporting_configuration_1_t *datalink_report_cfg_local_nonprim = NULL;
    cJSON *loss_connectivity_cfg = NULL;
    OpenAPI_loss_connectivity_cfg_1_t *loss_connectivity_cfg_local_nonprim = NULL;
    cJSON *maximum_latency = NULL;
    cJSON *maximum_response_time = NULL;
    cJSON *suggested_packet_num_dl = NULL;
    cJSON *dnn = NULL;
    cJSON *single_nssai = NULL;
    OpenAPI_snssai_t *single_nssai_local_nonprim = NULL;
    cJSON *app_id = NULL;
    cJSON *pdu_session_status_cfg = NULL;
    OpenAPI_pdu_session_status_cfg_1_t *pdu_session_status_cfg_local_nonprim = NULL;
    cJSON *reachability_for_sms_cfg = NULL;
    OpenAPI_reachability_for_sms_configuration_e reachability_for_sms_cfgVariable = 0;
    cJSON *mtc_provider_information = NULL;
    cJSON *af_id = NULL;
    cJSON *reachability_for_data_cfg = NULL;
    OpenAPI_reachability_for_data_configuration_1_t *reachability_for_data_cfg_local_nonprim = NULL;
    cJSON *idle_status_ind = NULL;
    cJSON *monitoring_suspension = NULL;
    OpenAPI_monitoring_suspension_1_t *monitoring_suspension_local_nonprim = NULL;
    cJSON *shared_monitoring_suspension_id_list = NULL;
    OpenAPI_list_t *shared_monitoring_suspension_id_listList = NULL;
    cJSON *pei_requested = NULL;
    event_type = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "eventType");
    if (!event_type) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [event_type]");
        goto end;
    }
    if (!cJSON_IsString(event_type)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [event_type]");
        goto end;
    }
    event_typeVariable = OpenAPI_event_type_FromString(event_type->valuestring);

    immediate_flag = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "immediateFlag");
    if (immediate_flag) {
    if (!cJSON_IsBool(immediate_flag)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [immediate_flag]");
        goto end;
    }
    }

    location_reporting_configuration = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "locationReportingConfiguration");
    if (location_reporting_configuration) {
    location_reporting_configuration_local_nonprim = OpenAPI_location_reporting_configuration_1_parseFromJSON(location_reporting_configuration);
    if (!location_reporting_configuration_local_nonprim) {
        ogs_error("OpenAPI_location_reporting_configuration_1_parseFromJSON failed [location_reporting_configuration]");
        goto end;
    }
    }

    association_type = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "associationType");
    if (association_type) {
    if (!cJSON_IsString(association_type)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [association_type]");
        goto end;
    }
    association_typeVariable = OpenAPI_association_type_FromString(association_type->valuestring);
    }

    datalink_report_cfg = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "datalinkReportCfg");
    if (datalink_report_cfg) {
    datalink_report_cfg_local_nonprim = OpenAPI_datalink_reporting_configuration_1_parseFromJSON(datalink_report_cfg);
    if (!datalink_report_cfg_local_nonprim) {
        ogs_error("OpenAPI_datalink_reporting_configuration_1_parseFromJSON failed [datalink_report_cfg]");
        goto end;
    }
    }

    loss_connectivity_cfg = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "lossConnectivityCfg");
    if (loss_connectivity_cfg) {
    loss_connectivity_cfg_local_nonprim = OpenAPI_loss_connectivity_cfg_1_parseFromJSON(loss_connectivity_cfg);
    if (!loss_connectivity_cfg_local_nonprim) {
        ogs_error("OpenAPI_loss_connectivity_cfg_1_parseFromJSON failed [loss_connectivity_cfg]");
        goto end;
    }
    }

    maximum_latency = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "maximumLatency");
    if (maximum_latency) {
    if (!cJSON_IsNumber(maximum_latency)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [maximum_latency]");
        goto end;
    }
    }

    maximum_response_time = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "maximumResponseTime");
    if (maximum_response_time) {
    if (!cJSON_IsNumber(maximum_response_time)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [maximum_response_time]");
        goto end;
    }
    }

    suggested_packet_num_dl = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "suggestedPacketNumDl");
    if (suggested_packet_num_dl) {
    if (!cJSON_IsNumber(suggested_packet_num_dl)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [suggested_packet_num_dl]");
        goto end;
    }
    }

    dnn = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "dnn");
    if (dnn) {
    if (!cJSON_IsString(dnn) && !cJSON_IsNull(dnn)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [dnn]");
        goto end;
    }
    }

    single_nssai = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "singleNssai");
    if (single_nssai) {
    single_nssai_local_nonprim = OpenAPI_snssai_parseFromJSON(single_nssai);
    if (!single_nssai_local_nonprim) {
        ogs_error("OpenAPI_snssai_parseFromJSON failed [single_nssai]");
        goto end;
    }
    }

    app_id = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "appId");
    if (app_id) {
    if (!cJSON_IsString(app_id) && !cJSON_IsNull(app_id)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [app_id]");
        goto end;
    }
    }

    pdu_session_status_cfg = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "pduSessionStatusCfg");
    if (pdu_session_status_cfg) {
    pdu_session_status_cfg_local_nonprim = OpenAPI_pdu_session_status_cfg_1_parseFromJSON(pdu_session_status_cfg);
    if (!pdu_session_status_cfg_local_nonprim) {
        ogs_error("OpenAPI_pdu_session_status_cfg_1_parseFromJSON failed [pdu_session_status_cfg]");
        goto end;
    }
    }

    reachability_for_sms_cfg = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "reachabilityForSmsCfg");
    if (reachability_for_sms_cfg) {
    if (!cJSON_IsString(reachability_for_sms_cfg)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [reachability_for_sms_cfg]");
        goto end;
    }
    reachability_for_sms_cfgVariable = OpenAPI_reachability_for_sms_configuration_FromString(reachability_for_sms_cfg->valuestring);
    }

    mtc_provider_information = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "mtcProviderInformation");
    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information) && !cJSON_IsNull(mtc_provider_information)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    af_id = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "afId");
    if (af_id) {
    if (!cJSON_IsString(af_id) && !cJSON_IsNull(af_id)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [af_id]");
        goto end;
    }
    }

    reachability_for_data_cfg = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "reachabilityForDataCfg");
    if (reachability_for_data_cfg) {
    reachability_for_data_cfg_local_nonprim = OpenAPI_reachability_for_data_configuration_1_parseFromJSON(reachability_for_data_cfg);
    if (!reachability_for_data_cfg_local_nonprim) {
        ogs_error("OpenAPI_reachability_for_data_configuration_1_parseFromJSON failed [reachability_for_data_cfg]");
        goto end;
    }
    }

    idle_status_ind = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "idleStatusInd");
    if (idle_status_ind) {
    if (!cJSON_IsBool(idle_status_ind)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [idle_status_ind]");
        goto end;
    }
    }

    monitoring_suspension = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "monitoringSuspension");
    if (monitoring_suspension) {
    monitoring_suspension_local_nonprim = OpenAPI_monitoring_suspension_1_parseFromJSON(monitoring_suspension);
    if (!monitoring_suspension_local_nonprim) {
        ogs_error("OpenAPI_monitoring_suspension_1_parseFromJSON failed [monitoring_suspension]");
        goto end;
    }
    }

    shared_monitoring_suspension_id_list = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "sharedMonitoringSuspensionIdList");
    if (shared_monitoring_suspension_id_list) {
        cJSON *shared_monitoring_suspension_id_list_local = NULL;
        if (!cJSON_IsArray(shared_monitoring_suspension_id_list)) {
            ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [shared_monitoring_suspension_id_list]");
            goto end;
        }

        shared_monitoring_suspension_id_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(shared_monitoring_suspension_id_list_local, shared_monitoring_suspension_id_list) {
            double *localDouble = NULL;
            int *localInt = NULL;
            if (!cJSON_IsString(shared_monitoring_suspension_id_list_local)) {
                ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [shared_monitoring_suspension_id_list]");
                goto end;
            }
            OpenAPI_list_add(shared_monitoring_suspension_id_listList, ogs_strdup(shared_monitoring_suspension_id_list_local->valuestring));
        }
    }

    pei_requested = cJSON_GetObjectItemCaseSensitive(monitoring_configuration_1JSON, "peiRequested");
    if (pei_requested) {
    if (!cJSON_IsBool(pei_requested)) {
        ogs_error("OpenAPI_monitoring_configuration_1_parseFromJSON() failed [pei_requested]");
        goto end;
    }
    }

    monitoring_configuration_1_local_var = OpenAPI_monitoring_configuration_1_create (
        event_typeVariable,
        immediate_flag ? true : false,
        immediate_flag ? immediate_flag->valueint : 0,
        location_reporting_configuration ? location_reporting_configuration_local_nonprim : NULL,
        association_type ? association_typeVariable : 0,
        datalink_report_cfg ? datalink_report_cfg_local_nonprim : NULL,
        loss_connectivity_cfg ? loss_connectivity_cfg_local_nonprim : NULL,
        maximum_latency ? true : false,
        maximum_latency ? maximum_latency->valuedouble : 0,
        maximum_response_time ? true : false,
        maximum_response_time ? maximum_response_time->valuedouble : 0,
        suggested_packet_num_dl ? true : false,
        suggested_packet_num_dl ? suggested_packet_num_dl->valuedouble : 0,
        dnn && !cJSON_IsNull(dnn) ? ogs_strdup(dnn->valuestring) : NULL,
        single_nssai ? single_nssai_local_nonprim : NULL,
        app_id && !cJSON_IsNull(app_id) ? ogs_strdup(app_id->valuestring) : NULL,
        pdu_session_status_cfg ? pdu_session_status_cfg_local_nonprim : NULL,
        reachability_for_sms_cfg ? reachability_for_sms_cfgVariable : 0,
        mtc_provider_information && !cJSON_IsNull(mtc_provider_information) ? ogs_strdup(mtc_provider_information->valuestring) : NULL,
        af_id && !cJSON_IsNull(af_id) ? ogs_strdup(af_id->valuestring) : NULL,
        reachability_for_data_cfg ? reachability_for_data_cfg_local_nonprim : NULL,
        idle_status_ind ? true : false,
        idle_status_ind ? idle_status_ind->valueint : 0,
        monitoring_suspension ? monitoring_suspension_local_nonprim : NULL,
        shared_monitoring_suspension_id_list ? shared_monitoring_suspension_id_listList : NULL,
        pei_requested ? true : false,
        pei_requested ? pei_requested->valueint : 0
    );

    return monitoring_configuration_1_local_var;
end:
    if (location_reporting_configuration_local_nonprim) {
        OpenAPI_location_reporting_configuration_1_free(location_reporting_configuration_local_nonprim);
        location_reporting_configuration_local_nonprim = NULL;
    }
    if (datalink_report_cfg_local_nonprim) {
        OpenAPI_datalink_reporting_configuration_1_free(datalink_report_cfg_local_nonprim);
        datalink_report_cfg_local_nonprim = NULL;
    }
    if (loss_connectivity_cfg_local_nonprim) {
        OpenAPI_loss_connectivity_cfg_1_free(loss_connectivity_cfg_local_nonprim);
        loss_connectivity_cfg_local_nonprim = NULL;
    }
    if (single_nssai_local_nonprim) {
        OpenAPI_snssai_free(single_nssai_local_nonprim);
        single_nssai_local_nonprim = NULL;
    }
    if (pdu_session_status_cfg_local_nonprim) {
        OpenAPI_pdu_session_status_cfg_1_free(pdu_session_status_cfg_local_nonprim);
        pdu_session_status_cfg_local_nonprim = NULL;
    }
    if (reachability_for_data_cfg_local_nonprim) {
        OpenAPI_reachability_for_data_configuration_1_free(reachability_for_data_cfg_local_nonprim);
        reachability_for_data_cfg_local_nonprim = NULL;
    }
    if (monitoring_suspension_local_nonprim) {
        OpenAPI_monitoring_suspension_1_free(monitoring_suspension_local_nonprim);
        monitoring_suspension_local_nonprim = NULL;
    }
    if (shared_monitoring_suspension_id_listList) {
        OpenAPI_list_for_each(shared_monitoring_suspension_id_listList, node) {
            ogs_free(node->data);
        }
        OpenAPI_list_free(shared_monitoring_suspension_id_listList);
        shared_monitoring_suspension_id_listList = NULL;
    }
    return NULL;
}

OpenAPI_monitoring_configuration_1_t *OpenAPI_monitoring_configuration_1_copy(OpenAPI_monitoring_configuration_1_t *dst, OpenAPI_monitoring_configuration_1_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_monitoring_configuration_1_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_monitoring_configuration_1_convertToJSON() failed");
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

    OpenAPI_monitoring_configuration_1_free(dst);
    dst = OpenAPI_monitoring_configuration_1_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

