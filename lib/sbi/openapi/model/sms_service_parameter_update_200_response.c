
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sms_service_parameter_update_200_response.h"

OpenAPI_sms_service_parameter_update_200_response_t *OpenAPI_sms_service_parameter_update_200_response_create(
    OpenAPI_list_t *report,
    char *supi,
    char *pei,
    char *amf_id,
    OpenAPI_list_t *guamis,
    OpenAPI_access_type_e access_type,
    OpenAPI_access_type_e additional_access_type,
    char *gpsi,
    OpenAPI_user_location_t *ue_location,
    char *ue_time_zone,
    bool is_trace_data_null,
    OpenAPI_trace_data_t *trace_data,
    OpenAPI_list_t *backup_amf_info,
    char *udm_group_id,
    char *routing_indicator,
    bool is_h_nw_pub_key_id,
    int h_nw_pub_key_id,
    OpenAPI_rat_type_e rat_type,
    OpenAPI_rat_type_e additional_rat_type,
    char *supported_features
)
{
    OpenAPI_sms_service_parameter_update_200_response_t *sms_service_parameter_update_200_response_local_var = ogs_malloc(sizeof(OpenAPI_sms_service_parameter_update_200_response_t));
    ogs_assert(sms_service_parameter_update_200_response_local_var);

    sms_service_parameter_update_200_response_local_var->report = report;
    sms_service_parameter_update_200_response_local_var->supi = supi;
    sms_service_parameter_update_200_response_local_var->pei = pei;
    sms_service_parameter_update_200_response_local_var->amf_id = amf_id;
    sms_service_parameter_update_200_response_local_var->guamis = guamis;
    sms_service_parameter_update_200_response_local_var->access_type = access_type;
    sms_service_parameter_update_200_response_local_var->additional_access_type = additional_access_type;
    sms_service_parameter_update_200_response_local_var->gpsi = gpsi;
    sms_service_parameter_update_200_response_local_var->ue_location = ue_location;
    sms_service_parameter_update_200_response_local_var->ue_time_zone = ue_time_zone;
    sms_service_parameter_update_200_response_local_var->is_trace_data_null = is_trace_data_null;
    sms_service_parameter_update_200_response_local_var->trace_data = trace_data;
    sms_service_parameter_update_200_response_local_var->backup_amf_info = backup_amf_info;
    sms_service_parameter_update_200_response_local_var->udm_group_id = udm_group_id;
    sms_service_parameter_update_200_response_local_var->routing_indicator = routing_indicator;
    sms_service_parameter_update_200_response_local_var->is_h_nw_pub_key_id = is_h_nw_pub_key_id;
    sms_service_parameter_update_200_response_local_var->h_nw_pub_key_id = h_nw_pub_key_id;
    sms_service_parameter_update_200_response_local_var->rat_type = rat_type;
    sms_service_parameter_update_200_response_local_var->additional_rat_type = additional_rat_type;
    sms_service_parameter_update_200_response_local_var->supported_features = supported_features;

    return sms_service_parameter_update_200_response_local_var;
}

void OpenAPI_sms_service_parameter_update_200_response_free(OpenAPI_sms_service_parameter_update_200_response_t *sms_service_parameter_update_200_response)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == sms_service_parameter_update_200_response) {
        return;
    }
    if (sms_service_parameter_update_200_response->report) {
        OpenAPI_list_for_each(sms_service_parameter_update_200_response->report, node) {
            OpenAPI_report_item_free(node->data);
        }
        OpenAPI_list_free(sms_service_parameter_update_200_response->report);
        sms_service_parameter_update_200_response->report = NULL;
    }
    if (sms_service_parameter_update_200_response->supi) {
        ogs_free(sms_service_parameter_update_200_response->supi);
        sms_service_parameter_update_200_response->supi = NULL;
    }
    if (sms_service_parameter_update_200_response->pei) {
        ogs_free(sms_service_parameter_update_200_response->pei);
        sms_service_parameter_update_200_response->pei = NULL;
    }
    if (sms_service_parameter_update_200_response->amf_id) {
        ogs_free(sms_service_parameter_update_200_response->amf_id);
        sms_service_parameter_update_200_response->amf_id = NULL;
    }
    if (sms_service_parameter_update_200_response->guamis) {
        OpenAPI_list_for_each(sms_service_parameter_update_200_response->guamis, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(sms_service_parameter_update_200_response->guamis);
        sms_service_parameter_update_200_response->guamis = NULL;
    }
    if (sms_service_parameter_update_200_response->gpsi) {
        ogs_free(sms_service_parameter_update_200_response->gpsi);
        sms_service_parameter_update_200_response->gpsi = NULL;
    }
    if (sms_service_parameter_update_200_response->ue_location) {
        OpenAPI_user_location_free(sms_service_parameter_update_200_response->ue_location);
        sms_service_parameter_update_200_response->ue_location = NULL;
    }
    if (sms_service_parameter_update_200_response->ue_time_zone) {
        ogs_free(sms_service_parameter_update_200_response->ue_time_zone);
        sms_service_parameter_update_200_response->ue_time_zone = NULL;
    }
    if (sms_service_parameter_update_200_response->trace_data) {
        OpenAPI_trace_data_free(sms_service_parameter_update_200_response->trace_data);
        sms_service_parameter_update_200_response->trace_data = NULL;
    }
    if (sms_service_parameter_update_200_response->backup_amf_info) {
        OpenAPI_list_for_each(sms_service_parameter_update_200_response->backup_amf_info, node) {
            OpenAPI_backup_amf_info_free(node->data);
        }
        OpenAPI_list_free(sms_service_parameter_update_200_response->backup_amf_info);
        sms_service_parameter_update_200_response->backup_amf_info = NULL;
    }
    if (sms_service_parameter_update_200_response->udm_group_id) {
        ogs_free(sms_service_parameter_update_200_response->udm_group_id);
        sms_service_parameter_update_200_response->udm_group_id = NULL;
    }
    if (sms_service_parameter_update_200_response->routing_indicator) {
        ogs_free(sms_service_parameter_update_200_response->routing_indicator);
        sms_service_parameter_update_200_response->routing_indicator = NULL;
    }
    if (sms_service_parameter_update_200_response->supported_features) {
        ogs_free(sms_service_parameter_update_200_response->supported_features);
        sms_service_parameter_update_200_response->supported_features = NULL;
    }
    ogs_free(sms_service_parameter_update_200_response);
}

cJSON *OpenAPI_sms_service_parameter_update_200_response_convertToJSON(OpenAPI_sms_service_parameter_update_200_response_t *sms_service_parameter_update_200_response)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (sms_service_parameter_update_200_response == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [SMSServiceParameterUpdate_200_response]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!sms_service_parameter_update_200_response->report) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [report]");
        return NULL;
    }
    cJSON *reportList = cJSON_AddArrayToObject(item, "report");
    if (reportList == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [report]");
        goto end;
    }
    OpenAPI_list_for_each(sms_service_parameter_update_200_response->report, node) {
        cJSON *itemLocal = OpenAPI_report_item_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [report]");
            goto end;
        }
        cJSON_AddItemToArray(reportList, itemLocal);
    }

    if (!sms_service_parameter_update_200_response->supi) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", sms_service_parameter_update_200_response->supi) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [supi]");
        goto end;
    }

    if (sms_service_parameter_update_200_response->pei) {
    if (cJSON_AddStringToObject(item, "pei", sms_service_parameter_update_200_response->pei) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (!sms_service_parameter_update_200_response->amf_id) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [amf_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "amfId", sms_service_parameter_update_200_response->amf_id) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [amf_id]");
        goto end;
    }

    if (sms_service_parameter_update_200_response->guamis) {
    cJSON *guamisList = cJSON_AddArrayToObject(item, "guamis");
    if (guamisList == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [guamis]");
        goto end;
    }
    OpenAPI_list_for_each(sms_service_parameter_update_200_response->guamis, node) {
        cJSON *itemLocal = OpenAPI_guami_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [guamis]");
            goto end;
        }
        cJSON_AddItemToArray(guamisList, itemLocal);
    }
    }

    if (sms_service_parameter_update_200_response->access_type == OpenAPI_access_type_NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [access_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(sms_service_parameter_update_200_response->access_type)) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [access_type]");
        goto end;
    }

    if (sms_service_parameter_update_200_response->additional_access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "additionalAccessType", OpenAPI_access_type_ToString(sms_service_parameter_update_200_response->additional_access_type)) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [additional_access_type]");
        goto end;
    }
    }

    if (sms_service_parameter_update_200_response->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", sms_service_parameter_update_200_response->gpsi) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (sms_service_parameter_update_200_response->ue_location) {
    cJSON *ue_location_local_JSON = OpenAPI_user_location_convertToJSON(sms_service_parameter_update_200_response->ue_location);
    if (ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLocation", ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [ue_location]");
        goto end;
    }
    }

    if (sms_service_parameter_update_200_response->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", sms_service_parameter_update_200_response->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (sms_service_parameter_update_200_response->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(sms_service_parameter_update_200_response->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [trace_data]");
        goto end;
    }
    } else if (sms_service_parameter_update_200_response->is_trace_data_null) {
        if (cJSON_AddNullToObject(item, "traceData") == NULL) {
            ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (sms_service_parameter_update_200_response->backup_amf_info) {
    cJSON *backup_amf_infoList = cJSON_AddArrayToObject(item, "backupAmfInfo");
    if (backup_amf_infoList == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [backup_amf_info]");
        goto end;
    }
    OpenAPI_list_for_each(sms_service_parameter_update_200_response->backup_amf_info, node) {
        cJSON *itemLocal = OpenAPI_backup_amf_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [backup_amf_info]");
            goto end;
        }
        cJSON_AddItemToArray(backup_amf_infoList, itemLocal);
    }
    }

    if (sms_service_parameter_update_200_response->udm_group_id) {
    if (cJSON_AddStringToObject(item, "udmGroupId", sms_service_parameter_update_200_response->udm_group_id) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [udm_group_id]");
        goto end;
    }
    }

    if (sms_service_parameter_update_200_response->routing_indicator) {
    if (cJSON_AddStringToObject(item, "routingIndicator", sms_service_parameter_update_200_response->routing_indicator) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [routing_indicator]");
        goto end;
    }
    }

    if (sms_service_parameter_update_200_response->is_h_nw_pub_key_id) {
    if (cJSON_AddNumberToObject(item, "hNwPubKeyId", sms_service_parameter_update_200_response->h_nw_pub_key_id) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    if (sms_service_parameter_update_200_response->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(sms_service_parameter_update_200_response->rat_type)) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (sms_service_parameter_update_200_response->additional_rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "additionalRatType", OpenAPI_rat_type_ToString(sms_service_parameter_update_200_response->additional_rat_type)) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [additional_rat_type]");
        goto end;
    }
    }

    if (sms_service_parameter_update_200_response->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", sms_service_parameter_update_200_response->supported_features) == NULL) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_sms_service_parameter_update_200_response_t *OpenAPI_sms_service_parameter_update_200_response_parseFromJSON(cJSON *sms_service_parameter_update_200_responseJSON)
{
    OpenAPI_sms_service_parameter_update_200_response_t *sms_service_parameter_update_200_response_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *report = NULL;
    OpenAPI_list_t *reportList = NULL;
    cJSON *supi = NULL;
    cJSON *pei = NULL;
    cJSON *amf_id = NULL;
    cJSON *guamis = NULL;
    OpenAPI_list_t *guamisList = NULL;
    cJSON *access_type = NULL;
    OpenAPI_access_type_e access_typeVariable = 0;
    cJSON *additional_access_type = NULL;
    OpenAPI_access_type_e additional_access_typeVariable = 0;
    cJSON *gpsi = NULL;
    cJSON *ue_location = NULL;
    OpenAPI_user_location_t *ue_location_local_nonprim = NULL;
    cJSON *ue_time_zone = NULL;
    cJSON *trace_data = NULL;
    OpenAPI_trace_data_t *trace_data_local_nonprim = NULL;
    cJSON *backup_amf_info = NULL;
    OpenAPI_list_t *backup_amf_infoList = NULL;
    cJSON *udm_group_id = NULL;
    cJSON *routing_indicator = NULL;
    cJSON *h_nw_pub_key_id = NULL;
    cJSON *rat_type = NULL;
    OpenAPI_rat_type_e rat_typeVariable = 0;
    cJSON *additional_rat_type = NULL;
    OpenAPI_rat_type_e additional_rat_typeVariable = 0;
    cJSON *supported_features = NULL;
    report = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "report");
    if (!report) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [report]");
        goto end;
    }
        cJSON *report_local = NULL;
        if (!cJSON_IsArray(report)) {
            ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [report]");
            goto end;
        }

        reportList = OpenAPI_list_create();

        cJSON_ArrayForEach(report_local, report) {
            if (!cJSON_IsObject(report_local)) {
                ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [report]");
                goto end;
            }
            OpenAPI_report_item_t *reportItem = OpenAPI_report_item_parseFromJSON(report_local);
            if (!reportItem) {
                ogs_error("No reportItem");
                goto end;
            }
            OpenAPI_list_add(reportList, reportItem);
        }

    supi = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [supi]");
        goto end;
    }

    pei = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    amf_id = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "amfId");
    if (!amf_id) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [amf_id]");
        goto end;
    }
    if (!cJSON_IsString(amf_id)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [amf_id]");
        goto end;
    }

    guamis = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "guamis");
    if (guamis) {
        cJSON *guamis_local = NULL;
        if (!cJSON_IsArray(guamis)) {
            ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [guamis]");
            goto end;
        }

        guamisList = OpenAPI_list_create();

        cJSON_ArrayForEach(guamis_local, guamis) {
            if (!cJSON_IsObject(guamis_local)) {
                ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [guamis]");
                goto end;
            }
            OpenAPI_guami_t *guamisItem = OpenAPI_guami_parseFromJSON(guamis_local);
            if (!guamisItem) {
                ogs_error("No guamisItem");
                goto end;
            }
            OpenAPI_list_add(guamisList, guamisItem);
        }
    }

    access_type = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "accessType");
    if (!access_type) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [access_type]");
        goto end;
    }
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);

    additional_access_type = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "additionalAccessType");
    if (additional_access_type) {
    if (!cJSON_IsString(additional_access_type)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [additional_access_type]");
        goto end;
    }
    additional_access_typeVariable = OpenAPI_access_type_FromString(additional_access_type->valuestring);
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    ue_location = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "ueLocation");
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    if (!ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_location]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    trace_data = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "traceData");
    if (trace_data) {
    if (!cJSON_IsNull(trace_data)) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    if (!trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
        goto end;
    }
    }
    }

    backup_amf_info = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "backupAmfInfo");
    if (backup_amf_info) {
        cJSON *backup_amf_info_local = NULL;
        if (!cJSON_IsArray(backup_amf_info)) {
            ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [backup_amf_info]");
            goto end;
        }

        backup_amf_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(backup_amf_info_local, backup_amf_info) {
            if (!cJSON_IsObject(backup_amf_info_local)) {
                ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [backup_amf_info]");
                goto end;
            }
            OpenAPI_backup_amf_info_t *backup_amf_infoItem = OpenAPI_backup_amf_info_parseFromJSON(backup_amf_info_local);
            if (!backup_amf_infoItem) {
                ogs_error("No backup_amf_infoItem");
                goto end;
            }
            OpenAPI_list_add(backup_amf_infoList, backup_amf_infoItem);
        }
    }

    udm_group_id = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "udmGroupId");
    if (udm_group_id) {
    if (!cJSON_IsString(udm_group_id) && !cJSON_IsNull(udm_group_id)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [udm_group_id]");
        goto end;
    }
    }

    routing_indicator = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "routingIndicator");
    if (routing_indicator) {
    if (!cJSON_IsString(routing_indicator) && !cJSON_IsNull(routing_indicator)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [routing_indicator]");
        goto end;
    }
    }

    h_nw_pub_key_id = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "hNwPubKeyId");
    if (h_nw_pub_key_id) {
    if (!cJSON_IsNumber(h_nw_pub_key_id)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    additional_rat_type = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "additionalRatType");
    if (additional_rat_type) {
    if (!cJSON_IsString(additional_rat_type)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [additional_rat_type]");
        goto end;
    }
    additional_rat_typeVariable = OpenAPI_rat_type_FromString(additional_rat_type->valuestring);
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(sms_service_parameter_update_200_responseJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    sms_service_parameter_update_200_response_local_var = OpenAPI_sms_service_parameter_update_200_response_create (
        reportList,
        ogs_strdup(supi->valuestring),
        pei && !cJSON_IsNull(pei) ? ogs_strdup(pei->valuestring) : NULL,
        ogs_strdup(amf_id->valuestring),
        guamis ? guamisList : NULL,
        access_typeVariable,
        additional_access_type ? additional_access_typeVariable : 0,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        ue_location ? ue_location_local_nonprim : NULL,
        ue_time_zone && !cJSON_IsNull(ue_time_zone) ? ogs_strdup(ue_time_zone->valuestring) : NULL,
        trace_data && cJSON_IsNull(trace_data) ? true : false,
        trace_data ? trace_data_local_nonprim : NULL,
        backup_amf_info ? backup_amf_infoList : NULL,
        udm_group_id && !cJSON_IsNull(udm_group_id) ? ogs_strdup(udm_group_id->valuestring) : NULL,
        routing_indicator && !cJSON_IsNull(routing_indicator) ? ogs_strdup(routing_indicator->valuestring) : NULL,
        h_nw_pub_key_id ? true : false,
        h_nw_pub_key_id ? h_nw_pub_key_id->valuedouble : 0,
        rat_type ? rat_typeVariable : 0,
        additional_rat_type ? additional_rat_typeVariable : 0,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return sms_service_parameter_update_200_response_local_var;
end:
    if (reportList) {
        OpenAPI_list_for_each(reportList, node) {
            OpenAPI_report_item_free(node->data);
        }
        OpenAPI_list_free(reportList);
        reportList = NULL;
    }
    if (guamisList) {
        OpenAPI_list_for_each(guamisList, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(guamisList);
        guamisList = NULL;
    }
    if (ue_location_local_nonprim) {
        OpenAPI_user_location_free(ue_location_local_nonprim);
        ue_location_local_nonprim = NULL;
    }
    if (trace_data_local_nonprim) {
        OpenAPI_trace_data_free(trace_data_local_nonprim);
        trace_data_local_nonprim = NULL;
    }
    if (backup_amf_infoList) {
        OpenAPI_list_for_each(backup_amf_infoList, node) {
            OpenAPI_backup_amf_info_free(node->data);
        }
        OpenAPI_list_free(backup_amf_infoList);
        backup_amf_infoList = NULL;
    }
    return NULL;
}

OpenAPI_sms_service_parameter_update_200_response_t *OpenAPI_sms_service_parameter_update_200_response_copy(OpenAPI_sms_service_parameter_update_200_response_t *dst, OpenAPI_sms_service_parameter_update_200_response_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_sms_service_parameter_update_200_response_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_sms_service_parameter_update_200_response_convertToJSON() failed");
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

    OpenAPI_sms_service_parameter_update_200_response_free(dst);
    dst = OpenAPI_sms_service_parameter_update_200_response_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

