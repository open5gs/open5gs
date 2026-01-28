
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "event_notify_data.h"

OpenAPI_event_notify_data_t *OpenAPI_event_notify_data_create(
    OpenAPI_reported_event_type_t *reported_event_type,
    char *supi,
    char *gpsi,
    char *hgmlc_call_back_uri,
    char *ldr_reference,
    OpenAPI_geographic_area_t *location_estimate,
    bool is_age_of_location_estimate,
    int age_of_location_estimate,
    char *timestamp_of_location_estimate,
    OpenAPI_civic_address_t *civic_address,
    OpenAPI_local_area_t *local_location_estimate,
    OpenAPI_list_t *positioning_data_list,
    OpenAPI_list_t *gnss_positioning_data_list,
    char *serving_lm_fidentification,
    OpenAPI_termination_cause_t *termination_cause,
    OpenAPI_velocity_estimate_t *velocity_estimate,
    bool is_altitude,
    double altitude,
    OpenAPI_minor_location_qo_s_t *achieved_qos,
    char *supported_features
)
{
    OpenAPI_event_notify_data_t *event_notify_data_local_var = ogs_malloc(sizeof(OpenAPI_event_notify_data_t));
    ogs_assert(event_notify_data_local_var);

    event_notify_data_local_var->reported_event_type = reported_event_type;
    event_notify_data_local_var->supi = supi;
    event_notify_data_local_var->gpsi = gpsi;
    event_notify_data_local_var->hgmlc_call_back_uri = hgmlc_call_back_uri;
    event_notify_data_local_var->ldr_reference = ldr_reference;
    event_notify_data_local_var->location_estimate = location_estimate;
    event_notify_data_local_var->is_age_of_location_estimate = is_age_of_location_estimate;
    event_notify_data_local_var->age_of_location_estimate = age_of_location_estimate;
    event_notify_data_local_var->timestamp_of_location_estimate = timestamp_of_location_estimate;
    event_notify_data_local_var->civic_address = civic_address;
    event_notify_data_local_var->local_location_estimate = local_location_estimate;
    event_notify_data_local_var->positioning_data_list = positioning_data_list;
    event_notify_data_local_var->gnss_positioning_data_list = gnss_positioning_data_list;
    event_notify_data_local_var->serving_lm_fidentification = serving_lm_fidentification;
    event_notify_data_local_var->termination_cause = termination_cause;
    event_notify_data_local_var->velocity_estimate = velocity_estimate;
    event_notify_data_local_var->is_altitude = is_altitude;
    event_notify_data_local_var->altitude = altitude;
    event_notify_data_local_var->achieved_qos = achieved_qos;
    event_notify_data_local_var->supported_features = supported_features;

    return event_notify_data_local_var;
}

void OpenAPI_event_notify_data_free(OpenAPI_event_notify_data_t *event_notify_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == event_notify_data) {
        return;
    }
    if (event_notify_data->reported_event_type) {
        OpenAPI_reported_event_type_free(event_notify_data->reported_event_type);
        event_notify_data->reported_event_type = NULL;
    }
    if (event_notify_data->supi) {
        ogs_free(event_notify_data->supi);
        event_notify_data->supi = NULL;
    }
    if (event_notify_data->gpsi) {
        ogs_free(event_notify_data->gpsi);
        event_notify_data->gpsi = NULL;
    }
    if (event_notify_data->hgmlc_call_back_uri) {
        ogs_free(event_notify_data->hgmlc_call_back_uri);
        event_notify_data->hgmlc_call_back_uri = NULL;
    }
    if (event_notify_data->ldr_reference) {
        ogs_free(event_notify_data->ldr_reference);
        event_notify_data->ldr_reference = NULL;
    }
    if (event_notify_data->location_estimate) {
        OpenAPI_geographic_area_free(event_notify_data->location_estimate);
        event_notify_data->location_estimate = NULL;
    }
    if (event_notify_data->timestamp_of_location_estimate) {
        ogs_free(event_notify_data->timestamp_of_location_estimate);
        event_notify_data->timestamp_of_location_estimate = NULL;
    }
    if (event_notify_data->civic_address) {
        OpenAPI_civic_address_free(event_notify_data->civic_address);
        event_notify_data->civic_address = NULL;
    }
    if (event_notify_data->local_location_estimate) {
        OpenAPI_local_area_free(event_notify_data->local_location_estimate);
        event_notify_data->local_location_estimate = NULL;
    }
    if (event_notify_data->positioning_data_list) {
        OpenAPI_list_for_each(event_notify_data->positioning_data_list, node) {
            OpenAPI_positioning_method_and_usage_free(node->data);
        }
        OpenAPI_list_free(event_notify_data->positioning_data_list);
        event_notify_data->positioning_data_list = NULL;
    }
    if (event_notify_data->gnss_positioning_data_list) {
        OpenAPI_list_for_each(event_notify_data->gnss_positioning_data_list, node) {
            OpenAPI_gnss_positioning_method_and_usage_free(node->data);
        }
        OpenAPI_list_free(event_notify_data->gnss_positioning_data_list);
        event_notify_data->gnss_positioning_data_list = NULL;
    }
    if (event_notify_data->serving_lm_fidentification) {
        ogs_free(event_notify_data->serving_lm_fidentification);
        event_notify_data->serving_lm_fidentification = NULL;
    }
    if (event_notify_data->termination_cause) {
        OpenAPI_termination_cause_free(event_notify_data->termination_cause);
        event_notify_data->termination_cause = NULL;
    }
    if (event_notify_data->velocity_estimate) {
        OpenAPI_velocity_estimate_free(event_notify_data->velocity_estimate);
        event_notify_data->velocity_estimate = NULL;
    }
    if (event_notify_data->achieved_qos) {
        OpenAPI_minor_location_qo_s_free(event_notify_data->achieved_qos);
        event_notify_data->achieved_qos = NULL;
    }
    if (event_notify_data->supported_features) {
        ogs_free(event_notify_data->supported_features);
        event_notify_data->supported_features = NULL;
    }
    ogs_free(event_notify_data);
}

cJSON *OpenAPI_event_notify_data_convertToJSON(OpenAPI_event_notify_data_t *event_notify_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (event_notify_data == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [EventNotifyData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!event_notify_data->reported_event_type) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [reported_event_type]");
        return NULL;
    }
    cJSON *reported_event_type_local_JSON = OpenAPI_reported_event_type_convertToJSON(event_notify_data->reported_event_type);
    if (reported_event_type_local_JSON == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [reported_event_type]");
        goto end;
    }
    cJSON_AddItemToObject(item, "reportedEventType", reported_event_type_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [reported_event_type]");
        goto end;
    }

    if (event_notify_data->supi) {
    if (cJSON_AddStringToObject(item, "supi", event_notify_data->supi) == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [supi]");
        goto end;
    }
    }

    if (event_notify_data->gpsi) {
    if (cJSON_AddStringToObject(item, "gpsi", event_notify_data->gpsi) == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [gpsi]");
        goto end;
    }
    }

    if (event_notify_data->hgmlc_call_back_uri) {
    if (cJSON_AddStringToObject(item, "hgmlcCallBackURI", event_notify_data->hgmlc_call_back_uri) == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [hgmlc_call_back_uri]");
        goto end;
    }
    }

    if (!event_notify_data->ldr_reference) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [ldr_reference]");
        return NULL;
    }
    if (cJSON_AddStringToObject(item, "ldrReference", event_notify_data->ldr_reference) == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [ldr_reference]");
        goto end;
    }

    if (event_notify_data->location_estimate) {
    cJSON *location_estimate_local_JSON = OpenAPI_geographic_area_convertToJSON(event_notify_data->location_estimate);
    if (location_estimate_local_JSON == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [location_estimate]");
        goto end;
    }
    cJSON_AddItemToObject(item, "locationEstimate", location_estimate_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [location_estimate]");
        goto end;
    }
    }

    if (event_notify_data->is_age_of_location_estimate) {
    if (cJSON_AddNumberToObject(item, "ageOfLocationEstimate", event_notify_data->age_of_location_estimate) == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [age_of_location_estimate]");
        goto end;
    }
    }

    if (event_notify_data->timestamp_of_location_estimate) {
    if (cJSON_AddStringToObject(item, "timestampOfLocationEstimate", event_notify_data->timestamp_of_location_estimate) == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [timestamp_of_location_estimate]");
        goto end;
    }
    }

    if (event_notify_data->civic_address) {
    cJSON *civic_address_local_JSON = OpenAPI_civic_address_convertToJSON(event_notify_data->civic_address);
    if (civic_address_local_JSON == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [civic_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "civicAddress", civic_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [civic_address]");
        goto end;
    }
    }

    if (event_notify_data->local_location_estimate) {
    cJSON *local_location_estimate_local_JSON = OpenAPI_local_area_convertToJSON(event_notify_data->local_location_estimate);
    if (local_location_estimate_local_JSON == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [local_location_estimate]");
        goto end;
    }
    cJSON_AddItemToObject(item, "localLocationEstimate", local_location_estimate_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [local_location_estimate]");
        goto end;
    }
    }

    if (event_notify_data->positioning_data_list) {
    cJSON *positioning_data_listList = cJSON_AddArrayToObject(item, "positioningDataList");
    if (positioning_data_listList == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [positioning_data_list]");
        goto end;
    }
    OpenAPI_list_for_each(event_notify_data->positioning_data_list, node) {
        cJSON *itemLocal = OpenAPI_positioning_method_and_usage_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [positioning_data_list]");
            goto end;
        }
        cJSON_AddItemToArray(positioning_data_listList, itemLocal);
    }
    }

    if (event_notify_data->gnss_positioning_data_list) {
    cJSON *gnss_positioning_data_listList = cJSON_AddArrayToObject(item, "gnssPositioningDataList");
    if (gnss_positioning_data_listList == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [gnss_positioning_data_list]");
        goto end;
    }
    OpenAPI_list_for_each(event_notify_data->gnss_positioning_data_list, node) {
        cJSON *itemLocal = OpenAPI_gnss_positioning_method_and_usage_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [gnss_positioning_data_list]");
            goto end;
        }
        cJSON_AddItemToArray(gnss_positioning_data_listList, itemLocal);
    }
    }

    if (event_notify_data->serving_lm_fidentification) {
    if (cJSON_AddStringToObject(item, "servingLMFidentification", event_notify_data->serving_lm_fidentification) == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [serving_lm_fidentification]");
        goto end;
    }
    }

    if (event_notify_data->termination_cause) {
    cJSON *termination_cause_local_JSON = OpenAPI_termination_cause_convertToJSON(event_notify_data->termination_cause);
    if (termination_cause_local_JSON == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [termination_cause]");
        goto end;
    }
    cJSON_AddItemToObject(item, "terminationCause", termination_cause_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [termination_cause]");
        goto end;
    }
    }

    if (event_notify_data->velocity_estimate) {
    cJSON *velocity_estimate_local_JSON = OpenAPI_velocity_estimate_convertToJSON(event_notify_data->velocity_estimate);
    if (velocity_estimate_local_JSON == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [velocity_estimate]");
        goto end;
    }
    cJSON_AddItemToObject(item, "velocityEstimate", velocity_estimate_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [velocity_estimate]");
        goto end;
    }
    }

    if (event_notify_data->is_altitude) {
    if (cJSON_AddNumberToObject(item, "altitude", event_notify_data->altitude) == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [altitude]");
        goto end;
    }
    }

    if (event_notify_data->achieved_qos) {
    cJSON *achieved_qos_local_JSON = OpenAPI_minor_location_qo_s_convertToJSON(event_notify_data->achieved_qos);
    if (achieved_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [achieved_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "achievedQos", achieved_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [achieved_qos]");
        goto end;
    }
    }

    if (event_notify_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", event_notify_data->supported_features) == NULL) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_event_notify_data_t *OpenAPI_event_notify_data_parseFromJSON(cJSON *event_notify_dataJSON)
{
    OpenAPI_event_notify_data_t *event_notify_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *reported_event_type = NULL;
    OpenAPI_reported_event_type_t *reported_event_type_local_nonprim = NULL;
    cJSON *supi = NULL;
    cJSON *gpsi = NULL;
    cJSON *hgmlc_call_back_uri = NULL;
    cJSON *ldr_reference = NULL;
    cJSON *location_estimate = NULL;
    OpenAPI_geographic_area_t *location_estimate_local_nonprim = NULL;
    cJSON *age_of_location_estimate = NULL;
    cJSON *timestamp_of_location_estimate = NULL;
    cJSON *civic_address = NULL;
    OpenAPI_civic_address_t *civic_address_local_nonprim = NULL;
    cJSON *local_location_estimate = NULL;
    OpenAPI_local_area_t *local_location_estimate_local_nonprim = NULL;
    cJSON *positioning_data_list = NULL;
    OpenAPI_list_t *positioning_data_listList = NULL;
    cJSON *gnss_positioning_data_list = NULL;
    OpenAPI_list_t *gnss_positioning_data_listList = NULL;
    cJSON *serving_lm_fidentification = NULL;
    cJSON *termination_cause = NULL;
    OpenAPI_termination_cause_t *termination_cause_local_nonprim = NULL;
    cJSON *velocity_estimate = NULL;
    OpenAPI_velocity_estimate_t *velocity_estimate_local_nonprim = NULL;
    cJSON *altitude = NULL;
    cJSON *achieved_qos = NULL;
    OpenAPI_minor_location_qo_s_t *achieved_qos_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    reported_event_type = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "reportedEventType");
    if (!reported_event_type) {
        ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [reported_event_type]");
        goto end;
    }
    reported_event_type_local_nonprim = OpenAPI_reported_event_type_parseFromJSON(reported_event_type);
    if (!reported_event_type_local_nonprim) {
        ogs_error("OpenAPI_reported_event_type_parseFromJSON failed [reported_event_type]");
        goto end;
    }

    supi = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "supi");
    if (supi) {
    if (!cJSON_IsString(supi) && !cJSON_IsNull(supi)) {
        ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [supi]");
        goto end;
    }
    }

    gpsi = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "gpsi");
    if (gpsi) {
    if (!cJSON_IsString(gpsi) && !cJSON_IsNull(gpsi)) {
        ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [gpsi]");
        goto end;
    }
    }

    hgmlc_call_back_uri = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "hgmlcCallBackURI");
    if (hgmlc_call_back_uri) {
    if (!cJSON_IsString(hgmlc_call_back_uri) && !cJSON_IsNull(hgmlc_call_back_uri)) {
        ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [hgmlc_call_back_uri]");
        goto end;
    }
    }

    ldr_reference = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "ldrReference");
    if (!ldr_reference) {
        ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [ldr_reference]");
        goto end;
    }
    if (!cJSON_IsString(ldr_reference)) {
        ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [ldr_reference]");
        goto end;
    }

    location_estimate = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "locationEstimate");
    if (location_estimate) {
    location_estimate_local_nonprim = OpenAPI_geographic_area_parseFromJSON(location_estimate);
    if (!location_estimate_local_nonprim) {
        ogs_error("OpenAPI_geographic_area_parseFromJSON failed [location_estimate]");
        goto end;
    }
    }

    age_of_location_estimate = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "ageOfLocationEstimate");
    if (age_of_location_estimate) {
    if (!cJSON_IsNumber(age_of_location_estimate)) {
        ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [age_of_location_estimate]");
        goto end;
    }
    }

    timestamp_of_location_estimate = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "timestampOfLocationEstimate");
    if (timestamp_of_location_estimate) {
    if (!cJSON_IsString(timestamp_of_location_estimate) && !cJSON_IsNull(timestamp_of_location_estimate)) {
        ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [timestamp_of_location_estimate]");
        goto end;
    }
    }

    civic_address = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "civicAddress");
    if (civic_address) {
    civic_address_local_nonprim = OpenAPI_civic_address_parseFromJSON(civic_address);
    if (!civic_address_local_nonprim) {
        ogs_error("OpenAPI_civic_address_parseFromJSON failed [civic_address]");
        goto end;
    }
    }

    local_location_estimate = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "localLocationEstimate");
    if (local_location_estimate) {
    local_location_estimate_local_nonprim = OpenAPI_local_area_parseFromJSON(local_location_estimate);
    if (!local_location_estimate_local_nonprim) {
        ogs_error("OpenAPI_local_area_parseFromJSON failed [local_location_estimate]");
        goto end;
    }
    }

    positioning_data_list = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "positioningDataList");
    if (positioning_data_list) {
        cJSON *positioning_data_list_local = NULL;
        if (!cJSON_IsArray(positioning_data_list)) {
            ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [positioning_data_list]");
            goto end;
        }

        positioning_data_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(positioning_data_list_local, positioning_data_list) {
            if (!cJSON_IsObject(positioning_data_list_local)) {
                ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [positioning_data_list]");
                goto end;
            }
            OpenAPI_positioning_method_and_usage_t *positioning_data_listItem = OpenAPI_positioning_method_and_usage_parseFromJSON(positioning_data_list_local);
            if (!positioning_data_listItem) {
                ogs_error("No positioning_data_listItem");
                goto end;
            }
            OpenAPI_list_add(positioning_data_listList, positioning_data_listItem);
        }
    }

    gnss_positioning_data_list = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "gnssPositioningDataList");
    if (gnss_positioning_data_list) {
        cJSON *gnss_positioning_data_list_local = NULL;
        if (!cJSON_IsArray(gnss_positioning_data_list)) {
            ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [gnss_positioning_data_list]");
            goto end;
        }

        gnss_positioning_data_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gnss_positioning_data_list_local, gnss_positioning_data_list) {
            if (!cJSON_IsObject(gnss_positioning_data_list_local)) {
                ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [gnss_positioning_data_list]");
                goto end;
            }
            OpenAPI_gnss_positioning_method_and_usage_t *gnss_positioning_data_listItem = OpenAPI_gnss_positioning_method_and_usage_parseFromJSON(gnss_positioning_data_list_local);
            if (!gnss_positioning_data_listItem) {
                ogs_error("No gnss_positioning_data_listItem");
                goto end;
            }
            OpenAPI_list_add(gnss_positioning_data_listList, gnss_positioning_data_listItem);
        }
    }

    serving_lm_fidentification = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "servingLMFidentification");
    if (serving_lm_fidentification) {
    if (!cJSON_IsString(serving_lm_fidentification) && !cJSON_IsNull(serving_lm_fidentification)) {
        ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [serving_lm_fidentification]");
        goto end;
    }
    }

    termination_cause = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "terminationCause");
    if (termination_cause) {
    termination_cause_local_nonprim = OpenAPI_termination_cause_parseFromJSON(termination_cause);
    if (!termination_cause_local_nonprim) {
        ogs_error("OpenAPI_termination_cause_parseFromJSON failed [termination_cause]");
        goto end;
    }
    }

    velocity_estimate = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "velocityEstimate");
    if (velocity_estimate) {
    velocity_estimate_local_nonprim = OpenAPI_velocity_estimate_parseFromJSON(velocity_estimate);
    if (!velocity_estimate_local_nonprim) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON failed [velocity_estimate]");
        goto end;
    }
    }

    altitude = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "altitude");
    if (altitude) {
    if (!cJSON_IsNumber(altitude)) {
        ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [altitude]");
        goto end;
    }
    }

    achieved_qos = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "achievedQos");
    if (achieved_qos) {
    achieved_qos_local_nonprim = OpenAPI_minor_location_qo_s_parseFromJSON(achieved_qos);
    if (!achieved_qos_local_nonprim) {
        ogs_error("OpenAPI_minor_location_qo_s_parseFromJSON failed [achieved_qos]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(event_notify_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_event_notify_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    event_notify_data_local_var = OpenAPI_event_notify_data_create (
        reported_event_type_local_nonprim,
        supi && !cJSON_IsNull(supi) ? ogs_strdup(supi->valuestring) : NULL,
        gpsi && !cJSON_IsNull(gpsi) ? ogs_strdup(gpsi->valuestring) : NULL,
        hgmlc_call_back_uri && !cJSON_IsNull(hgmlc_call_back_uri) ? ogs_strdup(hgmlc_call_back_uri->valuestring) : NULL,
        ogs_strdup(ldr_reference->valuestring),
        location_estimate ? location_estimate_local_nonprim : NULL,
        age_of_location_estimate ? true : false,
        age_of_location_estimate ? age_of_location_estimate->valuedouble : 0,
        timestamp_of_location_estimate && !cJSON_IsNull(timestamp_of_location_estimate) ? ogs_strdup(timestamp_of_location_estimate->valuestring) : NULL,
        civic_address ? civic_address_local_nonprim : NULL,
        local_location_estimate ? local_location_estimate_local_nonprim : NULL,
        positioning_data_list ? positioning_data_listList : NULL,
        gnss_positioning_data_list ? gnss_positioning_data_listList : NULL,
        serving_lm_fidentification && !cJSON_IsNull(serving_lm_fidentification) ? ogs_strdup(serving_lm_fidentification->valuestring) : NULL,
        termination_cause ? termination_cause_local_nonprim : NULL,
        velocity_estimate ? velocity_estimate_local_nonprim : NULL,
        altitude ? true : false,
        altitude ? altitude->valuedouble : 0,
        achieved_qos ? achieved_qos_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL
    );

    return event_notify_data_local_var;
end:
    if (reported_event_type_local_nonprim) {
        OpenAPI_reported_event_type_free(reported_event_type_local_nonprim);
        reported_event_type_local_nonprim = NULL;
    }
    if (location_estimate_local_nonprim) {
        OpenAPI_geographic_area_free(location_estimate_local_nonprim);
        location_estimate_local_nonprim = NULL;
    }
    if (civic_address_local_nonprim) {
        OpenAPI_civic_address_free(civic_address_local_nonprim);
        civic_address_local_nonprim = NULL;
    }
    if (local_location_estimate_local_nonprim) {
        OpenAPI_local_area_free(local_location_estimate_local_nonprim);
        local_location_estimate_local_nonprim = NULL;
    }
    if (positioning_data_listList) {
        OpenAPI_list_for_each(positioning_data_listList, node) {
            OpenAPI_positioning_method_and_usage_free(node->data);
        }
        OpenAPI_list_free(positioning_data_listList);
        positioning_data_listList = NULL;
    }
    if (gnss_positioning_data_listList) {
        OpenAPI_list_for_each(gnss_positioning_data_listList, node) {
            OpenAPI_gnss_positioning_method_and_usage_free(node->data);
        }
        OpenAPI_list_free(gnss_positioning_data_listList);
        gnss_positioning_data_listList = NULL;
    }
    if (termination_cause_local_nonprim) {
        OpenAPI_termination_cause_free(termination_cause_local_nonprim);
        termination_cause_local_nonprim = NULL;
    }
    if (velocity_estimate_local_nonprim) {
        OpenAPI_velocity_estimate_free(velocity_estimate_local_nonprim);
        velocity_estimate_local_nonprim = NULL;
    }
    if (achieved_qos_local_nonprim) {
        OpenAPI_minor_location_qo_s_free(achieved_qos_local_nonprim);
        achieved_qos_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_event_notify_data_t *OpenAPI_event_notify_data_copy(OpenAPI_event_notify_data_t *dst, OpenAPI_event_notify_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_event_notify_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_event_notify_data_convertToJSON() failed");
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

    OpenAPI_event_notify_data_free(dst);
    dst = OpenAPI_event_notify_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

