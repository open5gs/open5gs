
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "location_data.h"

OpenAPI_location_data_t *OpenAPI_location_data_create(
    OpenAPI_geographic_area_t *location_estimate,
    OpenAPI_accuracy_fulfilment_indicator_t *accuracy_fulfilment_indicator,
    bool is_age_of_location_estimate,
    int age_of_location_estimate,
    char *timestamp_of_location_estimate,
    OpenAPI_velocity_estimate_t *velocity_estimate,
    OpenAPI_civic_address_t *civic_address,
    OpenAPI_local_area_t *local_location_estimate,
    OpenAPI_list_t *positioning_data_list,
    OpenAPI_list_t *gnss_positioning_data_list,
    OpenAPI_ecgi_t *ecgi,
    OpenAPI_ncgi_t *ncgi,
    bool is_altitude,
    double altitude,
    bool is_barometric_pressure,
    int barometric_pressure,
    char *serving_lmf_identification,
    char *ue_positioning_cap,
    OpenAPI_ue_area_indication_t *ue_area_ind,
    char *supported_features,
    OpenAPI_minor_location_qo_s_t *achieved_qos
)
{
    OpenAPI_location_data_t *location_data_local_var = ogs_malloc(sizeof(OpenAPI_location_data_t));
    ogs_assert(location_data_local_var);

    location_data_local_var->location_estimate = location_estimate;
    location_data_local_var->accuracy_fulfilment_indicator = accuracy_fulfilment_indicator;
    location_data_local_var->is_age_of_location_estimate = is_age_of_location_estimate;
    location_data_local_var->age_of_location_estimate = age_of_location_estimate;
    location_data_local_var->timestamp_of_location_estimate = timestamp_of_location_estimate;
    location_data_local_var->velocity_estimate = velocity_estimate;
    location_data_local_var->civic_address = civic_address;
    location_data_local_var->local_location_estimate = local_location_estimate;
    location_data_local_var->positioning_data_list = positioning_data_list;
    location_data_local_var->gnss_positioning_data_list = gnss_positioning_data_list;
    location_data_local_var->ecgi = ecgi;
    location_data_local_var->ncgi = ncgi;
    location_data_local_var->is_altitude = is_altitude;
    location_data_local_var->altitude = altitude;
    location_data_local_var->is_barometric_pressure = is_barometric_pressure;
    location_data_local_var->barometric_pressure = barometric_pressure;
    location_data_local_var->serving_lmf_identification = serving_lmf_identification;
    location_data_local_var->ue_positioning_cap = ue_positioning_cap;
    location_data_local_var->ue_area_ind = ue_area_ind;
    location_data_local_var->supported_features = supported_features;
    location_data_local_var->achieved_qos = achieved_qos;

    return location_data_local_var;
}

void OpenAPI_location_data_free(OpenAPI_location_data_t *location_data)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == location_data) {
        return;
    }
    if (location_data->location_estimate) {
        OpenAPI_geographic_area_free(location_data->location_estimate);
        location_data->location_estimate = NULL;
    }
    if (location_data->accuracy_fulfilment_indicator) {
        OpenAPI_accuracy_fulfilment_indicator_free(location_data->accuracy_fulfilment_indicator);
        location_data->accuracy_fulfilment_indicator = NULL;
    }
    if (location_data->timestamp_of_location_estimate) {
        ogs_free(location_data->timestamp_of_location_estimate);
        location_data->timestamp_of_location_estimate = NULL;
    }
    if (location_data->velocity_estimate) {
        OpenAPI_velocity_estimate_free(location_data->velocity_estimate);
        location_data->velocity_estimate = NULL;
    }
    if (location_data->civic_address) {
        OpenAPI_civic_address_free(location_data->civic_address);
        location_data->civic_address = NULL;
    }
    if (location_data->local_location_estimate) {
        OpenAPI_local_area_free(location_data->local_location_estimate);
        location_data->local_location_estimate = NULL;
    }
    if (location_data->positioning_data_list) {
        OpenAPI_list_for_each(location_data->positioning_data_list, node) {
            OpenAPI_positioning_method_and_usage_free(node->data);
        }
        OpenAPI_list_free(location_data->positioning_data_list);
        location_data->positioning_data_list = NULL;
    }
    if (location_data->gnss_positioning_data_list) {
        OpenAPI_list_for_each(location_data->gnss_positioning_data_list, node) {
            OpenAPI_gnss_positioning_method_and_usage_free(node->data);
        }
        OpenAPI_list_free(location_data->gnss_positioning_data_list);
        location_data->gnss_positioning_data_list = NULL;
    }
    if (location_data->ecgi) {
        OpenAPI_ecgi_free(location_data->ecgi);
        location_data->ecgi = NULL;
    }
    if (location_data->ncgi) {
        OpenAPI_ncgi_free(location_data->ncgi);
        location_data->ncgi = NULL;
    }
    if (location_data->serving_lmf_identification) {
        ogs_free(location_data->serving_lmf_identification);
        location_data->serving_lmf_identification = NULL;
    }
    if (location_data->ue_positioning_cap) {
        ogs_free(location_data->ue_positioning_cap);
        location_data->ue_positioning_cap = NULL;
    }
    if (location_data->ue_area_ind) {
        OpenAPI_ue_area_indication_free(location_data->ue_area_ind);
        location_data->ue_area_ind = NULL;
    }
    if (location_data->supported_features) {
        ogs_free(location_data->supported_features);
        location_data->supported_features = NULL;
    }
    if (location_data->achieved_qos) {
        OpenAPI_minor_location_qo_s_free(location_data->achieved_qos);
        location_data->achieved_qos = NULL;
    }
    ogs_free(location_data);
}

cJSON *OpenAPI_location_data_convertToJSON(OpenAPI_location_data_t *location_data)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (location_data == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [LocationData]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (!location_data->location_estimate) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [location_estimate]");
        return NULL;
    }
    cJSON *location_estimate_local_JSON = OpenAPI_geographic_area_convertToJSON(location_data->location_estimate);
    if (location_estimate_local_JSON == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [location_estimate]");
        goto end;
    }
    cJSON_AddItemToObject(item, "locationEstimate", location_estimate_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [location_estimate]");
        goto end;
    }

    if (location_data->accuracy_fulfilment_indicator) {
    cJSON *accuracy_fulfilment_indicator_local_JSON = OpenAPI_accuracy_fulfilment_indicator_convertToJSON(location_data->accuracy_fulfilment_indicator);
    if (accuracy_fulfilment_indicator_local_JSON == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [accuracy_fulfilment_indicator]");
        goto end;
    }
    cJSON_AddItemToObject(item, "accuracyFulfilmentIndicator", accuracy_fulfilment_indicator_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [accuracy_fulfilment_indicator]");
        goto end;
    }
    }

    if (location_data->is_age_of_location_estimate) {
    if (cJSON_AddNumberToObject(item, "ageOfLocationEstimate", location_data->age_of_location_estimate) == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [age_of_location_estimate]");
        goto end;
    }
    }

    if (location_data->timestamp_of_location_estimate) {
    if (cJSON_AddStringToObject(item, "timestampOfLocationEstimate", location_data->timestamp_of_location_estimate) == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [timestamp_of_location_estimate]");
        goto end;
    }
    }

    if (location_data->velocity_estimate) {
    cJSON *velocity_estimate_local_JSON = OpenAPI_velocity_estimate_convertToJSON(location_data->velocity_estimate);
    if (velocity_estimate_local_JSON == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [velocity_estimate]");
        goto end;
    }
    cJSON_AddItemToObject(item, "velocityEstimate", velocity_estimate_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [velocity_estimate]");
        goto end;
    }
    }

    if (location_data->civic_address) {
    cJSON *civic_address_local_JSON = OpenAPI_civic_address_convertToJSON(location_data->civic_address);
    if (civic_address_local_JSON == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [civic_address]");
        goto end;
    }
    cJSON_AddItemToObject(item, "civicAddress", civic_address_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [civic_address]");
        goto end;
    }
    }

    if (location_data->local_location_estimate) {
    cJSON *local_location_estimate_local_JSON = OpenAPI_local_area_convertToJSON(location_data->local_location_estimate);
    if (local_location_estimate_local_JSON == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [local_location_estimate]");
        goto end;
    }
    cJSON_AddItemToObject(item, "localLocationEstimate", local_location_estimate_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [local_location_estimate]");
        goto end;
    }
    }

    if (location_data->positioning_data_list) {
    cJSON *positioning_data_listList = cJSON_AddArrayToObject(item, "positioningDataList");
    if (positioning_data_listList == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [positioning_data_list]");
        goto end;
    }
    OpenAPI_list_for_each(location_data->positioning_data_list, node) {
        cJSON *itemLocal = OpenAPI_positioning_method_and_usage_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_location_data_convertToJSON() failed [positioning_data_list]");
            goto end;
        }
        cJSON_AddItemToArray(positioning_data_listList, itemLocal);
    }
    }

    if (location_data->gnss_positioning_data_list) {
    cJSON *gnss_positioning_data_listList = cJSON_AddArrayToObject(item, "gnssPositioningDataList");
    if (gnss_positioning_data_listList == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [gnss_positioning_data_list]");
        goto end;
    }
    OpenAPI_list_for_each(location_data->gnss_positioning_data_list, node) {
        cJSON *itemLocal = OpenAPI_gnss_positioning_method_and_usage_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_location_data_convertToJSON() failed [gnss_positioning_data_list]");
            goto end;
        }
        cJSON_AddItemToArray(gnss_positioning_data_listList, itemLocal);
    }
    }

    if (location_data->ecgi) {
    cJSON *ecgi_local_JSON = OpenAPI_ecgi_convertToJSON(location_data->ecgi);
    if (ecgi_local_JSON == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [ecgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ecgi", ecgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [ecgi]");
        goto end;
    }
    }

    if (location_data->ncgi) {
    cJSON *ncgi_local_JSON = OpenAPI_ncgi_convertToJSON(location_data->ncgi);
    if (ncgi_local_JSON == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [ncgi]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ncgi", ncgi_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [ncgi]");
        goto end;
    }
    }

    if (location_data->is_altitude) {
    if (cJSON_AddNumberToObject(item, "altitude", location_data->altitude) == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [altitude]");
        goto end;
    }
    }

    if (location_data->is_barometric_pressure) {
    if (cJSON_AddNumberToObject(item, "barometricPressure", location_data->barometric_pressure) == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [barometric_pressure]");
        goto end;
    }
    }

    if (location_data->serving_lmf_identification) {
    if (cJSON_AddStringToObject(item, "servingLMFIdentification", location_data->serving_lmf_identification) == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [serving_lmf_identification]");
        goto end;
    }
    }

    if (location_data->ue_positioning_cap) {
    if (cJSON_AddStringToObject(item, "uePositioningCap", location_data->ue_positioning_cap) == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [ue_positioning_cap]");
        goto end;
    }
    }

    if (location_data->ue_area_ind) {
    cJSON *ue_area_ind_local_JSON = OpenAPI_ue_area_indication_convertToJSON(location_data->ue_area_ind);
    if (ue_area_ind_local_JSON == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [ue_area_ind]");
        goto end;
    }
    cJSON_AddItemToObject(item, "ueAreaInd", ue_area_ind_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [ue_area_ind]");
        goto end;
    }
    }

    if (location_data->supported_features) {
    if (cJSON_AddStringToObject(item, "supportedFeatures", location_data->supported_features) == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [supported_features]");
        goto end;
    }
    }

    if (location_data->achieved_qos) {
    cJSON *achieved_qos_local_JSON = OpenAPI_minor_location_qo_s_convertToJSON(location_data->achieved_qos);
    if (achieved_qos_local_JSON == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [achieved_qos]");
        goto end;
    }
    cJSON_AddItemToObject(item, "achievedQos", achieved_qos_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed [achieved_qos]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_location_data_t *OpenAPI_location_data_parseFromJSON(cJSON *location_dataJSON)
{
    OpenAPI_location_data_t *location_data_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *location_estimate = NULL;
    OpenAPI_geographic_area_t *location_estimate_local_nonprim = NULL;
    cJSON *accuracy_fulfilment_indicator = NULL;
    OpenAPI_accuracy_fulfilment_indicator_t *accuracy_fulfilment_indicator_local_nonprim = NULL;
    cJSON *age_of_location_estimate = NULL;
    cJSON *timestamp_of_location_estimate = NULL;
    cJSON *velocity_estimate = NULL;
    OpenAPI_velocity_estimate_t *velocity_estimate_local_nonprim = NULL;
    cJSON *civic_address = NULL;
    OpenAPI_civic_address_t *civic_address_local_nonprim = NULL;
    cJSON *local_location_estimate = NULL;
    OpenAPI_local_area_t *local_location_estimate_local_nonprim = NULL;
    cJSON *positioning_data_list = NULL;
    OpenAPI_list_t *positioning_data_listList = NULL;
    cJSON *gnss_positioning_data_list = NULL;
    OpenAPI_list_t *gnss_positioning_data_listList = NULL;
    cJSON *ecgi = NULL;
    OpenAPI_ecgi_t *ecgi_local_nonprim = NULL;
    cJSON *ncgi = NULL;
    OpenAPI_ncgi_t *ncgi_local_nonprim = NULL;
    cJSON *altitude = NULL;
    cJSON *barometric_pressure = NULL;
    cJSON *serving_lmf_identification = NULL;
    cJSON *ue_positioning_cap = NULL;
    cJSON *ue_area_ind = NULL;
    OpenAPI_ue_area_indication_t *ue_area_ind_local_nonprim = NULL;
    cJSON *supported_features = NULL;
    cJSON *achieved_qos = NULL;
    OpenAPI_minor_location_qo_s_t *achieved_qos_local_nonprim = NULL;
    location_estimate = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "locationEstimate");
    if (!location_estimate) {
        ogs_error("OpenAPI_location_data_parseFromJSON() failed [location_estimate]");
        goto end;
    }
    location_estimate_local_nonprim = OpenAPI_geographic_area_parseFromJSON(location_estimate);
    if (!location_estimate_local_nonprim) {
        ogs_error("OpenAPI_geographic_area_parseFromJSON failed [location_estimate]");
        goto end;
    }

    accuracy_fulfilment_indicator = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "accuracyFulfilmentIndicator");
    if (accuracy_fulfilment_indicator) {
    accuracy_fulfilment_indicator_local_nonprim = OpenAPI_accuracy_fulfilment_indicator_parseFromJSON(accuracy_fulfilment_indicator);
    if (!accuracy_fulfilment_indicator_local_nonprim) {
        ogs_error("OpenAPI_accuracy_fulfilment_indicator_parseFromJSON failed [accuracy_fulfilment_indicator]");
        goto end;
    }
    }

    age_of_location_estimate = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "ageOfLocationEstimate");
    if (age_of_location_estimate) {
    if (!cJSON_IsNumber(age_of_location_estimate)) {
        ogs_error("OpenAPI_location_data_parseFromJSON() failed [age_of_location_estimate]");
        goto end;
    }
    }

    timestamp_of_location_estimate = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "timestampOfLocationEstimate");
    if (timestamp_of_location_estimate) {
    if (!cJSON_IsString(timestamp_of_location_estimate) && !cJSON_IsNull(timestamp_of_location_estimate)) {
        ogs_error("OpenAPI_location_data_parseFromJSON() failed [timestamp_of_location_estimate]");
        goto end;
    }
    }

    velocity_estimate = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "velocityEstimate");
    if (velocity_estimate) {
    velocity_estimate_local_nonprim = OpenAPI_velocity_estimate_parseFromJSON(velocity_estimate);
    if (!velocity_estimate_local_nonprim) {
        ogs_error("OpenAPI_velocity_estimate_parseFromJSON failed [velocity_estimate]");
        goto end;
    }
    }

    civic_address = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "civicAddress");
    if (civic_address) {
    civic_address_local_nonprim = OpenAPI_civic_address_parseFromJSON(civic_address);
    if (!civic_address_local_nonprim) {
        ogs_error("OpenAPI_civic_address_parseFromJSON failed [civic_address]");
        goto end;
    }
    }

    local_location_estimate = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "localLocationEstimate");
    if (local_location_estimate) {
    local_location_estimate_local_nonprim = OpenAPI_local_area_parseFromJSON(local_location_estimate);
    if (!local_location_estimate_local_nonprim) {
        ogs_error("OpenAPI_local_area_parseFromJSON failed [local_location_estimate]");
        goto end;
    }
    }

    positioning_data_list = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "positioningDataList");
    if (positioning_data_list) {
        cJSON *positioning_data_list_local = NULL;
        if (!cJSON_IsArray(positioning_data_list)) {
            ogs_error("OpenAPI_location_data_parseFromJSON() failed [positioning_data_list]");
            goto end;
        }

        positioning_data_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(positioning_data_list_local, positioning_data_list) {
            if (!cJSON_IsObject(positioning_data_list_local)) {
                ogs_error("OpenAPI_location_data_parseFromJSON() failed [positioning_data_list]");
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

    gnss_positioning_data_list = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "gnssPositioningDataList");
    if (gnss_positioning_data_list) {
        cJSON *gnss_positioning_data_list_local = NULL;
        if (!cJSON_IsArray(gnss_positioning_data_list)) {
            ogs_error("OpenAPI_location_data_parseFromJSON() failed [gnss_positioning_data_list]");
            goto end;
        }

        gnss_positioning_data_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(gnss_positioning_data_list_local, gnss_positioning_data_list) {
            if (!cJSON_IsObject(gnss_positioning_data_list_local)) {
                ogs_error("OpenAPI_location_data_parseFromJSON() failed [gnss_positioning_data_list]");
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

    ecgi = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "ecgi");
    if (ecgi) {
    ecgi_local_nonprim = OpenAPI_ecgi_parseFromJSON(ecgi);
    if (!ecgi_local_nonprim) {
        ogs_error("OpenAPI_ecgi_parseFromJSON failed [ecgi]");
        goto end;
    }
    }

    ncgi = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "ncgi");
    if (ncgi) {
    ncgi_local_nonprim = OpenAPI_ncgi_parseFromJSON(ncgi);
    if (!ncgi_local_nonprim) {
        ogs_error("OpenAPI_ncgi_parseFromJSON failed [ncgi]");
        goto end;
    }
    }

    altitude = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "altitude");
    if (altitude) {
    if (!cJSON_IsNumber(altitude)) {
        ogs_error("OpenAPI_location_data_parseFromJSON() failed [altitude]");
        goto end;
    }
    }

    barometric_pressure = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "barometricPressure");
    if (barometric_pressure) {
    if (!cJSON_IsNumber(barometric_pressure)) {
        ogs_error("OpenAPI_location_data_parseFromJSON() failed [barometric_pressure]");
        goto end;
    }
    }

    serving_lmf_identification = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "servingLMFIdentification");
    if (serving_lmf_identification) {
    if (!cJSON_IsString(serving_lmf_identification) && !cJSON_IsNull(serving_lmf_identification)) {
        ogs_error("OpenAPI_location_data_parseFromJSON() failed [serving_lmf_identification]");
        goto end;
    }
    }

    ue_positioning_cap = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "uePositioningCap");
    if (ue_positioning_cap) {
    if (!cJSON_IsString(ue_positioning_cap) && !cJSON_IsNull(ue_positioning_cap)) {
        ogs_error("OpenAPI_location_data_parseFromJSON() failed [ue_positioning_cap]");
        goto end;
    }
    }

    ue_area_ind = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "ueAreaInd");
    if (ue_area_ind) {
    ue_area_ind_local_nonprim = OpenAPI_ue_area_indication_parseFromJSON(ue_area_ind);
    if (!ue_area_ind_local_nonprim) {
        ogs_error("OpenAPI_ue_area_indication_parseFromJSON failed [ue_area_ind]");
        goto end;
    }
    }

    supported_features = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "supportedFeatures");
    if (supported_features) {
    if (!cJSON_IsString(supported_features) && !cJSON_IsNull(supported_features)) {
        ogs_error("OpenAPI_location_data_parseFromJSON() failed [supported_features]");
        goto end;
    }
    }

    achieved_qos = cJSON_GetObjectItemCaseSensitive(location_dataJSON, "achievedQos");
    if (achieved_qos) {
    achieved_qos_local_nonprim = OpenAPI_minor_location_qo_s_parseFromJSON(achieved_qos);
    if (!achieved_qos_local_nonprim) {
        ogs_error("OpenAPI_minor_location_qo_s_parseFromJSON failed [achieved_qos]");
        goto end;
    }
    }

    location_data_local_var = OpenAPI_location_data_create (
        location_estimate_local_nonprim,
        accuracy_fulfilment_indicator ? accuracy_fulfilment_indicator_local_nonprim : NULL,
        age_of_location_estimate ? true : false,
        age_of_location_estimate ? age_of_location_estimate->valuedouble : 0,
        timestamp_of_location_estimate && !cJSON_IsNull(timestamp_of_location_estimate) ? ogs_strdup(timestamp_of_location_estimate->valuestring) : NULL,
        velocity_estimate ? velocity_estimate_local_nonprim : NULL,
        civic_address ? civic_address_local_nonprim : NULL,
        local_location_estimate ? local_location_estimate_local_nonprim : NULL,
        positioning_data_list ? positioning_data_listList : NULL,
        gnss_positioning_data_list ? gnss_positioning_data_listList : NULL,
        ecgi ? ecgi_local_nonprim : NULL,
        ncgi ? ncgi_local_nonprim : NULL,
        altitude ? true : false,
        altitude ? altitude->valuedouble : 0,
        barometric_pressure ? true : false,
        barometric_pressure ? barometric_pressure->valuedouble : 0,
        serving_lmf_identification && !cJSON_IsNull(serving_lmf_identification) ? ogs_strdup(serving_lmf_identification->valuestring) : NULL,
        ue_positioning_cap && !cJSON_IsNull(ue_positioning_cap) ? ogs_strdup(ue_positioning_cap->valuestring) : NULL,
        ue_area_ind ? ue_area_ind_local_nonprim : NULL,
        supported_features && !cJSON_IsNull(supported_features) ? ogs_strdup(supported_features->valuestring) : NULL,
        achieved_qos ? achieved_qos_local_nonprim : NULL
    );

    return location_data_local_var;
end:
    if (location_estimate_local_nonprim) {
        OpenAPI_geographic_area_free(location_estimate_local_nonprim);
        location_estimate_local_nonprim = NULL;
    }
    if (accuracy_fulfilment_indicator_local_nonprim) {
        OpenAPI_accuracy_fulfilment_indicator_free(accuracy_fulfilment_indicator_local_nonprim);
        accuracy_fulfilment_indicator_local_nonprim = NULL;
    }
    if (velocity_estimate_local_nonprim) {
        OpenAPI_velocity_estimate_free(velocity_estimate_local_nonprim);
        velocity_estimate_local_nonprim = NULL;
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
    if (ecgi_local_nonprim) {
        OpenAPI_ecgi_free(ecgi_local_nonprim);
        ecgi_local_nonprim = NULL;
    }
    if (ncgi_local_nonprim) {
        OpenAPI_ncgi_free(ncgi_local_nonprim);
        ncgi_local_nonprim = NULL;
    }
    if (ue_area_ind_local_nonprim) {
        OpenAPI_ue_area_indication_free(ue_area_ind_local_nonprim);
        ue_area_ind_local_nonprim = NULL;
    }
    if (achieved_qos_local_nonprim) {
        OpenAPI_minor_location_qo_s_free(achieved_qos_local_nonprim);
        achieved_qos_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_location_data_t *OpenAPI_location_data_copy(OpenAPI_location_data_t *dst, OpenAPI_location_data_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_location_data_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_location_data_convertToJSON() failed");
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

    OpenAPI_location_data_free(dst);
    dst = OpenAPI_location_data_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

