
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "monitoring_configuration.h"

OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_create(
    OpenAPI_event_type_t *event_type,
    bool is_immediate_flag,
    int immediate_flag,
    OpenAPI_location_reporting_configuration_t *location_reporting_configuration,
    OpenAPI_association_type_t *association_type,
    OpenAPI_datalink_reporting_configuration_t *datalink_report_cfg,
    OpenAPI_loss_connectivity_cfg_t *loss_connectivity_cfg,
    bool is_maximum_latency,
    int maximum_latency,
    bool is_maximum_response_time,
    int maximum_response_time,
    bool is_suggested_packet_num_dl,
    int suggested_packet_num_dl,
    OpenAPI_pdu_session_status_cfg_t *pdu_session_status_cfg,
    OpenAPI_reachability_for_sms_configuration_t *reachability_for_sms_cfg,
    char *mtc_provider_information,
    char *af_id
)
{
    OpenAPI_monitoring_configuration_t *monitoring_configuration_local_var = ogs_malloc(sizeof(OpenAPI_monitoring_configuration_t));
    ogs_assert(monitoring_configuration_local_var);

    monitoring_configuration_local_var->event_type = event_type;
    monitoring_configuration_local_var->is_immediate_flag = is_immediate_flag;
    monitoring_configuration_local_var->immediate_flag = immediate_flag;
    monitoring_configuration_local_var->location_reporting_configuration = location_reporting_configuration;
    monitoring_configuration_local_var->association_type = association_type;
    monitoring_configuration_local_var->datalink_report_cfg = datalink_report_cfg;
    monitoring_configuration_local_var->loss_connectivity_cfg = loss_connectivity_cfg;
    monitoring_configuration_local_var->is_maximum_latency = is_maximum_latency;
    monitoring_configuration_local_var->maximum_latency = maximum_latency;
    monitoring_configuration_local_var->is_maximum_response_time = is_maximum_response_time;
    monitoring_configuration_local_var->maximum_response_time = maximum_response_time;
    monitoring_configuration_local_var->is_suggested_packet_num_dl = is_suggested_packet_num_dl;
    monitoring_configuration_local_var->suggested_packet_num_dl = suggested_packet_num_dl;
    monitoring_configuration_local_var->pdu_session_status_cfg = pdu_session_status_cfg;
    monitoring_configuration_local_var->reachability_for_sms_cfg = reachability_for_sms_cfg;
    monitoring_configuration_local_var->mtc_provider_information = mtc_provider_information;
    monitoring_configuration_local_var->af_id = af_id;

    return monitoring_configuration_local_var;
}

void OpenAPI_monitoring_configuration_free(OpenAPI_monitoring_configuration_t *monitoring_configuration)
{
    if (NULL == monitoring_configuration) {
        return;
    }
    OpenAPI_lnode_t *node;
    OpenAPI_event_type_free(monitoring_configuration->event_type);
    OpenAPI_location_reporting_configuration_free(monitoring_configuration->location_reporting_configuration);
    OpenAPI_association_type_free(monitoring_configuration->association_type);
    OpenAPI_datalink_reporting_configuration_free(monitoring_configuration->datalink_report_cfg);
    OpenAPI_loss_connectivity_cfg_free(monitoring_configuration->loss_connectivity_cfg);
    OpenAPI_pdu_session_status_cfg_free(monitoring_configuration->pdu_session_status_cfg);
    OpenAPI_reachability_for_sms_configuration_free(monitoring_configuration->reachability_for_sms_cfg);
    ogs_free(monitoring_configuration->mtc_provider_information);
    ogs_free(monitoring_configuration->af_id);
    ogs_free(monitoring_configuration);
}

cJSON *OpenAPI_monitoring_configuration_convertToJSON(OpenAPI_monitoring_configuration_t *monitoring_configuration)
{
    cJSON *item = NULL;

    if (monitoring_configuration == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [MonitoringConfiguration]");
        return NULL;
    }

    item = cJSON_CreateObject();
    cJSON *event_type_local_JSON = OpenAPI_event_type_convertToJSON(monitoring_configuration->event_type);
    if (event_type_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [event_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "eventType", event_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [event_type]");
        goto end;
    }

    if (monitoring_configuration->is_immediate_flag) {
    if (cJSON_AddBoolToObject(item, "immediateFlag", monitoring_configuration->immediate_flag) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [immediate_flag]");
        goto end;
    }
    }

    if (monitoring_configuration->location_reporting_configuration) {
    cJSON *location_reporting_configuration_local_JSON = OpenAPI_location_reporting_configuration_convertToJSON(monitoring_configuration->location_reporting_configuration);
    if (location_reporting_configuration_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [location_reporting_configuration]");
        goto end;
    }
    cJSON_AddItemToObject(item, "locationReportingConfiguration", location_reporting_configuration_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [location_reporting_configuration]");
        goto end;
    }
    }

    if (monitoring_configuration->association_type) {
    cJSON *association_type_local_JSON = OpenAPI_association_type_convertToJSON(monitoring_configuration->association_type);
    if (association_type_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [association_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "associationType", association_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [association_type]");
        goto end;
    }
    }

    if (monitoring_configuration->datalink_report_cfg) {
    cJSON *datalink_report_cfg_local_JSON = OpenAPI_datalink_reporting_configuration_convertToJSON(monitoring_configuration->datalink_report_cfg);
    if (datalink_report_cfg_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [datalink_report_cfg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "datalinkReportCfg", datalink_report_cfg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [datalink_report_cfg]");
        goto end;
    }
    }

    if (monitoring_configuration->loss_connectivity_cfg) {
    cJSON *loss_connectivity_cfg_local_JSON = OpenAPI_loss_connectivity_cfg_convertToJSON(monitoring_configuration->loss_connectivity_cfg);
    if (loss_connectivity_cfg_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [loss_connectivity_cfg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "lossConnectivityCfg", loss_connectivity_cfg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [loss_connectivity_cfg]");
        goto end;
    }
    }

    if (monitoring_configuration->is_maximum_latency) {
    if (cJSON_AddNumberToObject(item, "maximumLatency", monitoring_configuration->maximum_latency) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [maximum_latency]");
        goto end;
    }
    }

    if (monitoring_configuration->is_maximum_response_time) {
    if (cJSON_AddNumberToObject(item, "maximumResponseTime", monitoring_configuration->maximum_response_time) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [maximum_response_time]");
        goto end;
    }
    }

    if (monitoring_configuration->is_suggested_packet_num_dl) {
    if (cJSON_AddNumberToObject(item, "suggestedPacketNumDl", monitoring_configuration->suggested_packet_num_dl) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [suggested_packet_num_dl]");
        goto end;
    }
    }

    if (monitoring_configuration->pdu_session_status_cfg) {
    cJSON *pdu_session_status_cfg_local_JSON = OpenAPI_pdu_session_status_cfg_convertToJSON(monitoring_configuration->pdu_session_status_cfg);
    if (pdu_session_status_cfg_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [pdu_session_status_cfg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "pduSessionStatusCfg", pdu_session_status_cfg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [pdu_session_status_cfg]");
        goto end;
    }
    }

    if (monitoring_configuration->reachability_for_sms_cfg) {
    cJSON *reachability_for_sms_cfg_local_JSON = OpenAPI_reachability_for_sms_configuration_convertToJSON(monitoring_configuration->reachability_for_sms_cfg);
    if (reachability_for_sms_cfg_local_JSON == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [reachability_for_sms_cfg]");
        goto end;
    }
    cJSON_AddItemToObject(item, "reachabilityForSmsCfg", reachability_for_sms_cfg_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [reachability_for_sms_cfg]");
        goto end;
    }
    }

    if (monitoring_configuration->mtc_provider_information) {
    if (cJSON_AddStringToObject(item, "mtcProviderInformation", monitoring_configuration->mtc_provider_information) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    if (monitoring_configuration->af_id) {
    if (cJSON_AddStringToObject(item, "afId", monitoring_configuration->af_id) == NULL) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed [af_id]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_parseFromJSON(cJSON *monitoring_configurationJSON)
{
    OpenAPI_monitoring_configuration_t *monitoring_configuration_local_var = NULL;
    cJSON *event_type = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "eventType");
    if (!event_type) {
        ogs_error("OpenAPI_monitoring_configuration_parseFromJSON() failed [event_type]");
        goto end;
    }

    OpenAPI_event_type_t *event_type_local_nonprim = NULL;
    event_type_local_nonprim = OpenAPI_event_type_parseFromJSON(event_type);

    cJSON *immediate_flag = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "immediateFlag");

    if (immediate_flag) {
    if (!cJSON_IsBool(immediate_flag)) {
        ogs_error("OpenAPI_monitoring_configuration_parseFromJSON() failed [immediate_flag]");
        goto end;
    }
    }

    cJSON *location_reporting_configuration = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "locationReportingConfiguration");

    OpenAPI_location_reporting_configuration_t *location_reporting_configuration_local_nonprim = NULL;
    if (location_reporting_configuration) {
    location_reporting_configuration_local_nonprim = OpenAPI_location_reporting_configuration_parseFromJSON(location_reporting_configuration);
    }

    cJSON *association_type = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "associationType");

    OpenAPI_association_type_t *association_type_local_nonprim = NULL;
    if (association_type) {
    association_type_local_nonprim = OpenAPI_association_type_parseFromJSON(association_type);
    }

    cJSON *datalink_report_cfg = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "datalinkReportCfg");

    OpenAPI_datalink_reporting_configuration_t *datalink_report_cfg_local_nonprim = NULL;
    if (datalink_report_cfg) {
    datalink_report_cfg_local_nonprim = OpenAPI_datalink_reporting_configuration_parseFromJSON(datalink_report_cfg);
    }

    cJSON *loss_connectivity_cfg = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "lossConnectivityCfg");

    OpenAPI_loss_connectivity_cfg_t *loss_connectivity_cfg_local_nonprim = NULL;
    if (loss_connectivity_cfg) {
    loss_connectivity_cfg_local_nonprim = OpenAPI_loss_connectivity_cfg_parseFromJSON(loss_connectivity_cfg);
    }

    cJSON *maximum_latency = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "maximumLatency");

    if (maximum_latency) {
    if (!cJSON_IsNumber(maximum_latency)) {
        ogs_error("OpenAPI_monitoring_configuration_parseFromJSON() failed [maximum_latency]");
        goto end;
    }
    }

    cJSON *maximum_response_time = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "maximumResponseTime");

    if (maximum_response_time) {
    if (!cJSON_IsNumber(maximum_response_time)) {
        ogs_error("OpenAPI_monitoring_configuration_parseFromJSON() failed [maximum_response_time]");
        goto end;
    }
    }

    cJSON *suggested_packet_num_dl = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "suggestedPacketNumDl");

    if (suggested_packet_num_dl) {
    if (!cJSON_IsNumber(suggested_packet_num_dl)) {
        ogs_error("OpenAPI_monitoring_configuration_parseFromJSON() failed [suggested_packet_num_dl]");
        goto end;
    }
    }

    cJSON *pdu_session_status_cfg = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "pduSessionStatusCfg");

    OpenAPI_pdu_session_status_cfg_t *pdu_session_status_cfg_local_nonprim = NULL;
    if (pdu_session_status_cfg) {
    pdu_session_status_cfg_local_nonprim = OpenAPI_pdu_session_status_cfg_parseFromJSON(pdu_session_status_cfg);
    }

    cJSON *reachability_for_sms_cfg = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "reachabilityForSmsCfg");

    OpenAPI_reachability_for_sms_configuration_t *reachability_for_sms_cfg_local_nonprim = NULL;
    if (reachability_for_sms_cfg) {
    reachability_for_sms_cfg_local_nonprim = OpenAPI_reachability_for_sms_configuration_parseFromJSON(reachability_for_sms_cfg);
    }

    cJSON *mtc_provider_information = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "mtcProviderInformation");

    if (mtc_provider_information) {
    if (!cJSON_IsString(mtc_provider_information)) {
        ogs_error("OpenAPI_monitoring_configuration_parseFromJSON() failed [mtc_provider_information]");
        goto end;
    }
    }

    cJSON *af_id = cJSON_GetObjectItemCaseSensitive(monitoring_configurationJSON, "afId");

    if (af_id) {
    if (!cJSON_IsString(af_id)) {
        ogs_error("OpenAPI_monitoring_configuration_parseFromJSON() failed [af_id]");
        goto end;
    }
    }

    monitoring_configuration_local_var = OpenAPI_monitoring_configuration_create (
        event_type_local_nonprim,
        immediate_flag ? true : false,
        immediate_flag ? immediate_flag->valueint : 0,
        location_reporting_configuration ? location_reporting_configuration_local_nonprim : NULL,
        association_type ? association_type_local_nonprim : NULL,
        datalink_report_cfg ? datalink_report_cfg_local_nonprim : NULL,
        loss_connectivity_cfg ? loss_connectivity_cfg_local_nonprim : NULL,
        maximum_latency ? true : false,
        maximum_latency ? maximum_latency->valuedouble : 0,
        maximum_response_time ? true : false,
        maximum_response_time ? maximum_response_time->valuedouble : 0,
        suggested_packet_num_dl ? true : false,
        suggested_packet_num_dl ? suggested_packet_num_dl->valuedouble : 0,
        pdu_session_status_cfg ? pdu_session_status_cfg_local_nonprim : NULL,
        reachability_for_sms_cfg ? reachability_for_sms_cfg_local_nonprim : NULL,
        mtc_provider_information ? ogs_strdup(mtc_provider_information->valuestring) : NULL,
        af_id ? ogs_strdup(af_id->valuestring) : NULL
    );

    return monitoring_configuration_local_var;
end:
    return NULL;
}

OpenAPI_monitoring_configuration_t *OpenAPI_monitoring_configuration_copy(OpenAPI_monitoring_configuration_t *dst, OpenAPI_monitoring_configuration_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_monitoring_configuration_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_monitoring_configuration_convertToJSON() failed");
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

    OpenAPI_monitoring_configuration_free(dst);
    dst = OpenAPI_monitoring_configuration_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

