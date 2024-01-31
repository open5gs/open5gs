
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "ue_sms_context_data.h"

OpenAPI_ue_sms_context_data_t *OpenAPI_ue_sms_context_data_create(
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
    OpenAPI_ue_sms_context_data_t *ue_sms_context_data_local_var = ogs_malloc(sizeof(OpenAPI_ue_sms_context_data_t));
    ogs_assert(ue_sms_context_data_local_var);

    ue_sms_context_data_local_var->supi = supi;
    ue_sms_context_data_local_var->pei = pei;
    ue_sms_context_data_local_var->amf_id = amf_id;
    ue_sms_context_data_local_var->guamis = guamis;
    ue_sms_context_data_local_var->access_type = access_type;
    ue_sms_context_data_local_var->additional_access_type = additional_access_type;
    ue_sms_context_data_local_var->gpsi = gpsi;
    ue_sms_context_data_local_var->ue_location = ue_location;
    ue_sms_context_data_local_var->ue_time_zone = ue_time_zone;
    ue_sms_context_data_local_var->is_trace_data_null = is_trace_data_null;
    ue_sms_context_data_local_var->trace_data = trace_data;
    ue_sms_context_data_local_var->backup_amf_info = backup_amf_info;
    ue_sms_context_data_local_var->udm_group_id = udm_group_id;
    ue_sms_context_data_local_var->routing_indicator = routing_indicator;
    ue_sms_context_data_local_var->is_h_nw_pub_key_id = is_h_nw_pub_key_id;
    ue_sms_context_data_local_var->h_nw_pub_key_id = h_nw_pub_key_id;
    ue_sms_context_data_local_var->rat_type = rat_type;
    ue_sms_context_data_local_var->additional_rat_type = additional_rat_type;
    ue_sms_context_data_local_var->supported_features = supported_features;

    return ue_sms_context_data_local_var;
}

void OpenAPI_ue_sms_context_data_free(OpenAPI_ue_sms_context_data_t *ue_sms_context_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == ue_sms_context_data) {
        return;
    }
    if (ue_sms_context_data->supi) {
        ogs_free(ue_sms_context_data->supi);
        ue_sms_context_data->supi = NULL;
    }
    if (ue_sms_context_data->pei) {
        ogs_free(ue_sms_context_data->pei);
        ue_sms_context_data->pei = NULL;
    }
    if (ue_sms_context_data->amf_id) {
        ogs_free(ue_sms_context_data->amf_id);
        ue_sms_context_data->amf_id = NULL;
    }
    if (ue_sms_context_data->guamis) {
        OpenAPI_list_for_each(ue_sms_context_data->guamis, node) {
            OpenAPI_guami_free(node->data);
        }
        OpenAPI_list_free(ue_sms_context_data->guamis);
        ue_sms_context_data->guamis = NULL;
    }
    if (ue_sms_context_data->gpsi) {
        ogs_free(ue_sms_context_data->gpsi);
        ue_sms_context_data->gpsi = NULL;
    }
    if (ue_sms_context_data->ue_location) {
        OpenAPI_user_location_free(ue_sms_context_data->ue_location);
        ue_sms_context_data->ue_location = NULL;
    }
    if (ue_sms_context_data->ue_time_zone) {
        ogs_free(ue_sms_context_data->ue_time_zone);
        ue_sms_context_data->ue_time_zone = NULL;
    }
    if (ue_sms_context_data->trace_data) {
        OpenAPI_trace_data_free(ue_sms_context_data->trace_data);
        ue_sms_context_data->trace_data = NULL;
    }
    if (ue_sms_context_data->backup_amf_info) {
        OpenAPI_list_for_each(ue_sms_context_data->backup_amf_info, node) {
            OpenAPI_backup_amf_info_free(node->data);
        }
        OpenAPI_list_free(ue_sms_context_data->backup_amf_info);
        ue_sms_context_data->backup_amf_info = NULL;
    }
    if (ue_sms_context_data->udm_group_id) {
        ogs_free(ue_sms_context_data->udm_group_id);
        ue_sms_context_data->udm_group_id = NULL;
    }
    if (ue_sms_context_data->routing_indicator) {
        ogs_free(ue_sms_context_data->routing_indicator);
        ue_sms_context_data->routing_indicator = NULL;
    }
    if (ue_sms_context_data->supported_features) {
        ogs_free(ue_sms_context_data->supported_features);
        ue_sms_context_data->supported_features = NULL;
    }
    ogs_free(ue_sms_context_data);
}

cJSON *OpenAPI_ue_sms_context_data_convertToJSON(OpenAPI_ue_sms_context_data_t *ue_sms_context_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (ue_sms_context_data == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [UeSmsContextData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!ue_sms_context_data->supi) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [supi]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "supi", ue_sms_context_data->supi) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [supi]");
        goto end;
    }

    if (ue_sms_context_data->pei) {
    if (cJSON_AddStringToObject(item, "pei", ue_sms_context_data->pei) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [pei]");
        goto end;
    }
    }

    if (!ue_sms_context_data->amf_id) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [amf_id]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "amfId", ue_sms_context_data->amf_id) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [amf_id]");
        goto end;
    }

    if (ue_sms_context_data->guamis) {
    cJSON *guamisList = cJSON_AddArrayToObject(item, "guamis");
    if (guamisList == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [guamis]");
        goto end;
    }
    OpenAPI_list_for_each(ue_sms_context_data->guamis, node) {
        cJSON *itemLocal = OpenAPI_guami_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [guamis]");
            goto end;
        }
        cJSON_AddItemToArray(guamisList, itemLocal);
    }
    }

    if (ue_sms_context_data->access_type == OpenAPI_access_type_NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [access_type]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "accessType", OpenAPI_access_type_ToString(ue_sms_context_data->access_type)) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [access_type]");
        goto end;
    }

    if (ue_sms_context_data->additional_access_type != OpenAPI_access_type_NULL) {
    if (cJSON_AddStringToObject(item, "additionalAccessType", OpenAPI_access_type_ToString(ue_sms_context_data->additional_access_type)) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [additional_access_type]");
        goto end;
    }
    }

    if (ue_sms_context_data->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", ue_sms_context_data->gpsi) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (ue_sms_context_data->ue_location) {
    cJSON *ue_location_local_JSON = OpenAPI_user_location_convertToJSON(ue_sms_context_data->ue_location);
    if (ue_location_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueLocation", ue_location_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [ue_location]");
        goto end;
    }
    }

    if (ue_sms_context_data->ue_time_zone) {
    if (cJSON_AddStringToObject(item, "ueTimeZone", ue_sms_context_data->ue_time_zone) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    if (ue_sms_context_data->trace_data) {
    cJSON *trace_data_local_JSON = OpenAPI_trace_data_convertToJSON(ue_sms_context_data->trace_data);
    if (trace_data_local_JSON == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [trace_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "traceData", trace_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [trace_data]");
        goto end;
    }
    } else if (ue_sms_context_data->is_trace_data_null) {
        if (cJSON_AddNullToObject(item, "traceData") == NULL) {
            ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [trace_data]");
            goto end;
        }
    }

    if (ue_sms_context_data->backup_amf_info) {
    cJSON *backup_amf_infoList = cJSON_AddArrayToObject(item, "backupAmfInfo");
    if (backup_amf_infoList == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [backup_amf_info]");
        goto end;
    }
    OpenAPI_list_for_each(ue_sms_context_data->backup_amf_info, node) {
        cJSON *itemLocal = OpenAPI_backup_amf_info_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [backup_amf_info]");
            goto end;
        }
        cJSON_AddItemToArray(backup_amf_infoList, itemLocal);
    }
    }

    if (ue_sms_context_data->udm_group_id) {
    if (cJSON_AddStringToObject(item, "udmGroupId", ue_sms_context_data->udm_group_id) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [udm_group_id]");
        goto end;
    }
    }

    if (ue_sms_context_data->routing_indicator) {
    if (cJSON_AddStringToObject(item, "routingIndicator", ue_sms_context_data->routing_indicator) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [routing_indicator]");
        goto end;
    }
    }

    if (ue_sms_context_data->is_h_nw_pub_key_id) {
    if (cJSON_AddNumberToObject(item, "hNwPubKeyId", ue_sms_context_data->h_nw_pub_key_id) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    if (ue_sms_context_data->rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "ratType", OpenAPI_rat_type_ToString(ue_sms_context_data->rat_type)) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [rat_type]");
        goto end;
    }
    }

    if (ue_sms_context_data->additional_rat_type != OpenAPI_rat_type_NULL) {
    if (cJSON_AddStringToObject(item, "additionalRatType", OpenAPI_rat_type_ToString(ue_sms_context_data->additional_rat_type)) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [additional_rat_type]");
        goto end;
    }
    }

    if (ue_sms_context_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", ue_sms_context_data->supported_features) == NULL) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_ue_sms_context_data_t *OpenAPI_ue_sms_context_data_parseFromJSON(cJSON *ue_sms_context_dataJSON)
{
    OpenAPI_ue_sms_context_data_t *ue_sms_context_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
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
    supi = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "supi");
    if (!supi) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [supi]");
        goto end;
    }
    if (!cJSON_IsString(supi)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [supi]");
        goto end;
    }

    pei = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "pei");
    if (pei) {
    if (!cJSON_IsString(pei) && !cJSON_IsNull(pei)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [pei]");
        goto end;
    }
    }

    amf_id = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "amfId");
    if (!amf_id) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [amf_id]");
        goto end;
    }
    if (!cJSON_IsString(amf_id)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [amf_id]");
        goto end;
    }

    guamis = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "guamis");
    if (guamis) {
        cJSON *guamis_local = NULL;
        if (!cJSON_IsArray(guamis)) {
            ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [guamis]");
            goto end;
        }

        guamisList = OpenAPI_list_create();

        cJSON_ArrayForEach(guamis_local, guamis) {
            if (!cJSON_IsObject(guamis_local)) {
                ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [guamis]");
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

    access_type = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "accessType");
    if (!access_type) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [access_type]");
        goto end;
    }
    if (!cJSON_IsString(access_type)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [access_type]");
        goto end;
    }
    access_typeVariable = OpenAPI_access_type_FromString(access_type->valuestring);

    additional_access_type = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "additionalAccessType");
    if (additional_access_type) {
    if (!cJSON_IsString(additional_access_type)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [additional_access_type]");
        goto end;
    }
    additional_access_typeVariable = OpenAPI_access_type_FromString(additional_access_type->valuestring);
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    ue_location = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "ueLocation");
    if (ue_location) {
    ue_location_local_nonprim = OpenAPI_user_location_parseFromJSON(ue_location);
    if (!ue_location_local_nonprim) {
        ogs_error("OpenAPI_user_location_parseFromJSON failed [ue_location]");
        goto end;
    }
    }

    ue_time_zone = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "ueTimeZone");
    if (ue_time_zone) {
    if (!cJSON_IsString(ue_time_zone) && !cJSON_IsNull(ue_time_zone)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [ue_time_zone]");
        goto end;
    }
    }

    trace_data = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "traceData");
    if (trace_data) {
    if (!cJSON_IsNull(trace_data)) {
    trace_data_local_nonprim = OpenAPI_trace_data_parseFromJSON(trace_data);
    if (!trace_data_local_nonprim) {
        ogs_error("OpenAPI_trace_data_parseFromJSON failed [trace_data]");
        goto end;
    }
    }
    }

    backup_amf_info = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "backupAmfInfo");
    if (backup_amf_info) {
        cJSON *backup_amf_info_local = NULL;
        if (!cJSON_IsArray(backup_amf_info)) {
            ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [backup_amf_info]");
            goto end;
        }

        backup_amf_infoList = OpenAPI_list_create();

        cJSON_ArrayForEach(backup_amf_info_local, backup_amf_info) {
            if (!cJSON_IsObject(backup_amf_info_local)) {
                ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [backup_amf_info]");
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

    udm_group_id = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "udmGroupId");
    if (udm_group_id) {
    if (!cJSON_IsString(udm_group_id) && !cJSON_IsNull(udm_group_id)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [udm_group_id]");
        goto end;
    }
    }

    routing_indicator = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "routingIndicator");
    if (routing_indicator) {
    if (!cJSON_IsString(routing_indicator) && !cJSON_IsNull(routing_indicator)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [routing_indicator]");
        goto end;
    }
    }

    h_nw_pub_key_id = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "hNwPubKeyId");
    if (h_nw_pub_key_id) {
    if (!cJSON_IsNumber(h_nw_pub_key_id)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [h_nw_pub_key_id]");
        goto end;
    }
    }

    rat_type = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "ratType");
    if (rat_type) {
    if (!cJSON_IsString(rat_type)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [rat_type]");
        goto end;
    }
    rat_typeVariable = OpenAPI_rat_type_FromString(rat_type->valuestring);
    }

    additional_rat_type = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "additionalRatType");
    if (additional_rat_type) {
    if (!cJSON_IsString(additional_rat_type)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [additional_rat_type]");
        goto end;
    }
    additional_rat_typeVariable = OpenAPI_rat_type_FromString(additional_rat_type->valuestring);
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(ue_sms_context_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_ue_sms_context_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    ue_sms_context_data_local_var = OpenAPI_ue_sms_context_data_create (
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

    return ue_sms_context_data_local_var;
end:
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

OpenAPI_ue_sms_context_data_t *OpenAPI_ue_sms_context_data_copy(OpenAPI_ue_sms_context_data_t *dst, OpenAPI_ue_sms_context_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_ue_sms_context_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_ue_sms_context_data_convertToJSON() failed");
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

    OpenAPI_ue_sms_context_data_free(dst);
    dst = OpenAPI_ue_sms_context_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

