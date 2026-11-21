
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "service_profile_charging_information.h"

OpenAPI_service_profile_charging_information_t *OpenAPI_service_profile_charging_information_create(
    char *service_profile_identifier,
    OpenAPI_list_t *s_nssai_list,
    bool is_s_st,
    int s_st,
    bool is_latency,
    int latency,
    bool is_availability,
    double availability,
    OpenAPI_sharing_level_e resource_sharing_level,
    bool is_jitter,
    int jitter,
    char *reliability,
    bool is_max_numberof_ues,
    int max_numberof_ues,
    char *coverage_area,
    OpenAPI_mobility_level_e u_e_mobility_level,
    OpenAPI_support_e delay_tolerance_indicator,
    OpenAPI_throughput_t *d_l_thpt_per_slice,
    OpenAPI_throughput_t *d_l_thpt_per_ue,
    OpenAPI_throughput_t *u_l_thpt_per_slice,
    OpenAPI_throughput_t *u_l_thpt_per_ue,
    bool is_max_numberof_pd_usessions,
    int max_numberof_pd_usessions,
    char *k_pi_monitoring_list,
    bool is_supported_access_technology,
    int supported_access_technology,
    OpenAPI_support_e v2_x_communication_mode_indicator,
    OpenAPI_ee_perf_req_t *energy_efficiency,
    char *add_service_profile_info
)
{
    OpenAPI_service_profile_charging_information_t *service_profile_charging_information_local_var = ogs_malloc(sizeof(OpenAPI_service_profile_charging_information_t));
    ogs_assert(service_profile_charging_information_local_var);

    service_profile_charging_information_local_var->service_profile_identifier = service_profile_identifier;
    service_profile_charging_information_local_var->s_nssai_list = s_nssai_list;
    service_profile_charging_information_local_var->is_s_st = is_s_st;
    service_profile_charging_information_local_var->s_st = s_st;
    service_profile_charging_information_local_var->is_latency = is_latency;
    service_profile_charging_information_local_var->latency = latency;
    service_profile_charging_information_local_var->is_availability = is_availability;
    service_profile_charging_information_local_var->availability = availability;
    service_profile_charging_information_local_var->resource_sharing_level = resource_sharing_level;
    service_profile_charging_information_local_var->is_jitter = is_jitter;
    service_profile_charging_information_local_var->jitter = jitter;
    service_profile_charging_information_local_var->reliability = reliability;
    service_profile_charging_information_local_var->is_max_numberof_ues = is_max_numberof_ues;
    service_profile_charging_information_local_var->max_numberof_ues = max_numberof_ues;
    service_profile_charging_information_local_var->coverage_area = coverage_area;
    service_profile_charging_information_local_var->u_e_mobility_level = u_e_mobility_level;
    service_profile_charging_information_local_var->delay_tolerance_indicator = delay_tolerance_indicator;
    service_profile_charging_information_local_var->d_l_thpt_per_slice = d_l_thpt_per_slice;
    service_profile_charging_information_local_var->d_l_thpt_per_ue = d_l_thpt_per_ue;
    service_profile_charging_information_local_var->u_l_thpt_per_slice = u_l_thpt_per_slice;
    service_profile_charging_information_local_var->u_l_thpt_per_ue = u_l_thpt_per_ue;
    service_profile_charging_information_local_var->is_max_numberof_pd_usessions = is_max_numberof_pd_usessions;
    service_profile_charging_information_local_var->max_numberof_pd_usessions = max_numberof_pd_usessions;
    service_profile_charging_information_local_var->k_pi_monitoring_list = k_pi_monitoring_list;
    service_profile_charging_information_local_var->is_supported_access_technology = is_supported_access_technology;
    service_profile_charging_information_local_var->supported_access_technology = supported_access_technology;
    service_profile_charging_information_local_var->v2_x_communication_mode_indicator = v2_x_communication_mode_indicator;
    service_profile_charging_information_local_var->energy_efficiency = energy_efficiency;
    service_profile_charging_information_local_var->add_service_profile_info = add_service_profile_info;

    return service_profile_charging_information_local_var;
}

void OpenAPI_service_profile_charging_information_free(OpenAPI_service_profile_charging_information_t *service_profile_charging_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == service_profile_charging_information) {
        return;
    }
    if (service_profile_charging_information->service_profile_identifier) {
        ogs_free(service_profile_charging_information->service_profile_identifier);
        service_profile_charging_information->service_profile_identifier = NULL;
    }
    if (service_profile_charging_information->s_nssai_list) {
        OpenAPI_list_for_each(service_profile_charging_information->s_nssai_list, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(service_profile_charging_information->s_nssai_list);
        service_profile_charging_information->s_nssai_list = NULL;
    }
    if (service_profile_charging_information->reliability) {
        ogs_free(service_profile_charging_information->reliability);
        service_profile_charging_information->reliability = NULL;
    }
    if (service_profile_charging_information->coverage_area) {
        ogs_free(service_profile_charging_information->coverage_area);
        service_profile_charging_information->coverage_area = NULL;
    }
    if (service_profile_charging_information->d_l_thpt_per_slice) {
        OpenAPI_throughput_free(service_profile_charging_information->d_l_thpt_per_slice);
        service_profile_charging_information->d_l_thpt_per_slice = NULL;
    }
    if (service_profile_charging_information->d_l_thpt_per_ue) {
        OpenAPI_throughput_free(service_profile_charging_information->d_l_thpt_per_ue);
        service_profile_charging_information->d_l_thpt_per_ue = NULL;
    }
    if (service_profile_charging_information->u_l_thpt_per_slice) {
        OpenAPI_throughput_free(service_profile_charging_information->u_l_thpt_per_slice);
        service_profile_charging_information->u_l_thpt_per_slice = NULL;
    }
    if (service_profile_charging_information->u_l_thpt_per_ue) {
        OpenAPI_throughput_free(service_profile_charging_information->u_l_thpt_per_ue);
        service_profile_charging_information->u_l_thpt_per_ue = NULL;
    }
    if (service_profile_charging_information->k_pi_monitoring_list) {
        ogs_free(service_profile_charging_information->k_pi_monitoring_list);
        service_profile_charging_information->k_pi_monitoring_list = NULL;
    }
    if (service_profile_charging_information->energy_efficiency) {
        OpenAPI_ee_perf_req_free(service_profile_charging_information->energy_efficiency);
        service_profile_charging_information->energy_efficiency = NULL;
    }
    if (service_profile_charging_information->add_service_profile_info) {
        ogs_free(service_profile_charging_information->add_service_profile_info);
        service_profile_charging_information->add_service_profile_info = NULL;
    }
    ogs_free(service_profile_charging_information);
}

cJSON *OpenAPI_service_profile_charging_information_convertToJSON(OpenAPI_service_profile_charging_information_t *service_profile_charging_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (service_profile_charging_information == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [ServiceProfileChargingInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (service_profile_charging_information->service_profile_identifier) {
    if (cJSON_AddStringToObject(item, "serviceProfileIdentifier", service_profile_charging_information->service_profile_identifier) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [service_profile_identifier]");
        goto end;
    }
    }

    if (service_profile_charging_information->s_nssai_list) {
    cJSON *s_nssai_listList = cJSON_AddArrayToObject(item, "sNSSAIList");
    if (s_nssai_listList == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [s_nssai_list]");
        goto end;
    }
    OpenAPI_list_for_each(service_profile_charging_information->s_nssai_list, node) {
        cJSON *itemLocal = OpenAPI_snssai_convertToJSON(node->data);
        if (itemLocal == NULL) {
            ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [s_nssai_list]");
            goto end;
        }
        cJSON_AddItemToArray(s_nssai_listList, itemLocal);
    }
    }

    if (service_profile_charging_information->is_s_st) {
    if (cJSON_AddNumberToObject(item, "sST", service_profile_charging_information->s_st) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [s_st]");
        goto end;
    }
    }

    if (service_profile_charging_information->is_latency) {
    if (cJSON_AddNumberToObject(item, "latency", service_profile_charging_information->latency) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [latency]");
        goto end;
    }
    }

    if (service_profile_charging_information->is_availability) {
    if (cJSON_AddNumberToObject(item, "availability", service_profile_charging_information->availability) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [availability]");
        goto end;
    }
    }

    if (service_profile_charging_information->resource_sharing_level != OpenAPI_sharing_level_NULL) {
    if (cJSON_AddStringToObject(item, "resourceSharingLevel", OpenAPI_sharing_level_ToString(service_profile_charging_information->resource_sharing_level)) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [resource_sharing_level]");
        goto end;
    }
    }

    if (service_profile_charging_information->is_jitter) {
    if (cJSON_AddNumberToObject(item, "jitter", service_profile_charging_information->jitter) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [jitter]");
        goto end;
    }
    }

    if (service_profile_charging_information->reliability) {
    if (cJSON_AddStringToObject(item, "reliability", service_profile_charging_information->reliability) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [reliability]");
        goto end;
    }
    }

    if (service_profile_charging_information->is_max_numberof_ues) {
    if (cJSON_AddNumberToObject(item, "maxNumberofUEs", service_profile_charging_information->max_numberof_ues) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [max_numberof_ues]");
        goto end;
    }
    }

    if (service_profile_charging_information->coverage_area) {
    if (cJSON_AddStringToObject(item, "coverageArea", service_profile_charging_information->coverage_area) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [coverage_area]");
        goto end;
    }
    }

    if (service_profile_charging_information->u_e_mobility_level != OpenAPI_mobility_level_NULL) {
    if (cJSON_AddStringToObject(item, "uEMobilityLevel", OpenAPI_mobility_level_ToString(service_profile_charging_information->u_e_mobility_level)) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [u_e_mobility_level]");
        goto end;
    }
    }

    if (service_profile_charging_information->delay_tolerance_indicator != OpenAPI_support_NULL) {
    if (cJSON_AddStringToObject(item, "delayToleranceIndicator", OpenAPI_support_ToString(service_profile_charging_information->delay_tolerance_indicator)) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [delay_tolerance_indicator]");
        goto end;
    }
    }

    if (service_profile_charging_information->d_l_thpt_per_slice) {
    cJSON *d_l_thpt_per_slice_local_JSON = OpenAPI_throughput_convertToJSON(service_profile_charging_information->d_l_thpt_per_slice);
    if (d_l_thpt_per_slice_local_JSON == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [d_l_thpt_per_slice]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dLThptPerSlice", d_l_thpt_per_slice_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [d_l_thpt_per_slice]");
        goto end;
    }
    }

    if (service_profile_charging_information->d_l_thpt_per_ue) {
    cJSON *d_l_thpt_per_ue_local_JSON = OpenAPI_throughput_convertToJSON(service_profile_charging_information->d_l_thpt_per_ue);
    if (d_l_thpt_per_ue_local_JSON == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [d_l_thpt_per_ue]");
        goto end;
    }
    cJSON_AddItemToObject(item, "dLThptPerUE", d_l_thpt_per_ue_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [d_l_thpt_per_ue]");
        goto end;
    }
    }

    if (service_profile_charging_information->u_l_thpt_per_slice) {
    cJSON *u_l_thpt_per_slice_local_JSON = OpenAPI_throughput_convertToJSON(service_profile_charging_information->u_l_thpt_per_slice);
    if (u_l_thpt_per_slice_local_JSON == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [u_l_thpt_per_slice]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uLThptPerSlice", u_l_thpt_per_slice_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [u_l_thpt_per_slice]");
        goto end;
    }
    }

    if (service_profile_charging_information->u_l_thpt_per_ue) {
    cJSON *u_l_thpt_per_ue_local_JSON = OpenAPI_throughput_convertToJSON(service_profile_charging_information->u_l_thpt_per_ue);
    if (u_l_thpt_per_ue_local_JSON == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [u_l_thpt_per_ue]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uLThptPerUE", u_l_thpt_per_ue_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [u_l_thpt_per_ue]");
        goto end;
    }
    }

    if (service_profile_charging_information->is_max_numberof_pd_usessions) {
    if (cJSON_AddNumberToObject(item, "maxNumberofPDUsessions", service_profile_charging_information->max_numberof_pd_usessions) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [max_numberof_pd_usessions]");
        goto end;
    }
    }

    if (service_profile_charging_information->k_pi_monitoring_list) {
    if (cJSON_AddStringToObject(item, "kPIMonitoringList", service_profile_charging_information->k_pi_monitoring_list) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [k_pi_monitoring_list]");
        goto end;
    }
    }

    if (service_profile_charging_information->is_supported_access_technology) {
    if (cJSON_AddNumberToObject(item, "supportedAccessTechnology", service_profile_charging_information->supported_access_technology) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [supported_access_technology]");
        goto end;
    }
    }

    if (service_profile_charging_information->v2_x_communication_mode_indicator != OpenAPI_support_NULL) {
    if (cJSON_AddStringToObject(item, "v2XCommunicationModeIndicator", OpenAPI_support_ToString(service_profile_charging_information->v2_x_communication_mode_indicator)) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [v2_x_communication_mode_indicator]");
        goto end;
    }
    }

    if (service_profile_charging_information->energy_efficiency) {
    cJSON *energy_efficiency_local_JSON = OpenAPI_ee_perf_req_convertToJSON(service_profile_charging_information->energy_efficiency);
    if (energy_efficiency_local_JSON == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [energy_efficiency]");
        goto end;
    }
    cJSON_AddItemToObject(item, "energyEfficiency", energy_efficiency_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [energy_efficiency]");
        goto end;
    }
    }

    if (service_profile_charging_information->add_service_profile_info) {
    if (cJSON_AddStringToObject(item, "addServiceProfileInfo", service_profile_charging_information->add_service_profile_info) == NULL) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed [add_service_profile_info]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_service_profile_charging_information_t *OpenAPI_service_profile_charging_information_parseFromJSON(cJSON *service_profile_charging_informationJSON)
{
    OpenAPI_service_profile_charging_information_t *service_profile_charging_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *service_profile_identifier = NULL;
    cJSON *s_nssai_list = NULL;
    OpenAPI_list_t *s_nssai_listList = NULL;
    cJSON *s_st = NULL;
    cJSON *latency = NULL;
    cJSON *availability = NULL;
    cJSON *resource_sharing_level = NULL;
    OpenAPI_sharing_level_e resource_sharing_levelVariable = 0;
    cJSON *jitter = NULL;
    cJSON *reliability = NULL;
    cJSON *max_numberof_ues = NULL;
    cJSON *coverage_area = NULL;
    cJSON *u_e_mobility_level = NULL;
    OpenAPI_mobility_level_e u_e_mobility_levelVariable = 0;
    cJSON *delay_tolerance_indicator = NULL;
    OpenAPI_support_e delay_tolerance_indicatorVariable = 0;
    cJSON *d_l_thpt_per_slice = NULL;
    OpenAPI_throughput_t *d_l_thpt_per_slice_local_nonprim = NULL;
    cJSON *d_l_thpt_per_ue = NULL;
    OpenAPI_throughput_t *d_l_thpt_per_ue_local_nonprim = NULL;
    cJSON *u_l_thpt_per_slice = NULL;
    OpenAPI_throughput_t *u_l_thpt_per_slice_local_nonprim = NULL;
    cJSON *u_l_thpt_per_ue = NULL;
    OpenAPI_throughput_t *u_l_thpt_per_ue_local_nonprim = NULL;
    cJSON *max_numberof_pd_usessions = NULL;
    cJSON *k_pi_monitoring_list = NULL;
    cJSON *supported_access_technology = NULL;
    cJSON *v2_x_communication_mode_indicator = NULL;
    OpenAPI_support_e v2_x_communication_mode_indicatorVariable = 0;
    cJSON *energy_efficiency = NULL;
    OpenAPI_ee_perf_req_t *energy_efficiency_local_nonprim = NULL;
    cJSON *add_service_profile_info = NULL;
    service_profile_identifier = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "serviceProfileIdentifier");
    if (service_profile_identifier) {
    if (!cJSON_IsString(service_profile_identifier) && !cJSON_IsNull(service_profile_identifier)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [service_profile_identifier]");
        goto end;
    }
    }

    s_nssai_list = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "sNSSAIList");
    if (s_nssai_list) {
        cJSON *s_nssai_list_local = NULL;
        if (!cJSON_IsArray(s_nssai_list)) {
            ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [s_nssai_list]");
            goto end;
        }

        s_nssai_listList = OpenAPI_list_create();

        cJSON_ArrayForEach(s_nssai_list_local, s_nssai_list) {
            if (!cJSON_IsObject(s_nssai_list_local)) {
                ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [s_nssai_list]");
                goto end;
            }
            OpenAPI_snssai_t *s_nssai_listItem = OpenAPI_snssai_parseFromJSON(s_nssai_list_local);
            if (!s_nssai_listItem) {
                ogs_error("No s_nssai_listItem");
                goto end;
            }
            OpenAPI_list_add(s_nssai_listList, s_nssai_listItem);
        }
    }

    s_st = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "sST");
    if (s_st) {
    if (!cJSON_IsNumber(s_st)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [s_st]");
        goto end;
    }
    }

    latency = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "latency");
    if (latency) {
    if (!cJSON_IsNumber(latency)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [latency]");
        goto end;
    }
    }

    availability = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "availability");
    if (availability) {
    if (!cJSON_IsNumber(availability)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [availability]");
        goto end;
    }
    }

    resource_sharing_level = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "resourceSharingLevel");
    if (resource_sharing_level) {
    if (!cJSON_IsString(resource_sharing_level)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [resource_sharing_level]");
        goto end;
    }
    resource_sharing_levelVariable = OpenAPI_sharing_level_FromString(resource_sharing_level->valuestring);
    }

    jitter = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "jitter");
    if (jitter) {
    if (!cJSON_IsNumber(jitter)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [jitter]");
        goto end;
    }
    }

    reliability = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "reliability");
    if (reliability) {
    if (!cJSON_IsString(reliability) && !cJSON_IsNull(reliability)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [reliability]");
        goto end;
    }
    }

    max_numberof_ues = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "maxNumberofUEs");
    if (max_numberof_ues) {
    if (!cJSON_IsNumber(max_numberof_ues)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [max_numberof_ues]");
        goto end;
    }
    }

    coverage_area = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "coverageArea");
    if (coverage_area) {
    if (!cJSON_IsString(coverage_area) && !cJSON_IsNull(coverage_area)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [coverage_area]");
        goto end;
    }
    }

    u_e_mobility_level = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "uEMobilityLevel");
    if (u_e_mobility_level) {
    if (!cJSON_IsString(u_e_mobility_level)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [u_e_mobility_level]");
        goto end;
    }
    u_e_mobility_levelVariable = OpenAPI_mobility_level_FromString(u_e_mobility_level->valuestring);
    }

    delay_tolerance_indicator = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "delayToleranceIndicator");
    if (delay_tolerance_indicator) {
    if (!cJSON_IsString(delay_tolerance_indicator)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [delay_tolerance_indicator]");
        goto end;
    }
    delay_tolerance_indicatorVariable = OpenAPI_support_FromString(delay_tolerance_indicator->valuestring);
    }

    d_l_thpt_per_slice = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "dLThptPerSlice");
    if (d_l_thpt_per_slice) {
    d_l_thpt_per_slice_local_nonprim = OpenAPI_throughput_parseFromJSON(d_l_thpt_per_slice);
    if (!d_l_thpt_per_slice_local_nonprim) {
        ogs_error("OpenAPI_throughput_parseFromJSON failed [d_l_thpt_per_slice]");
        goto end;
    }
    }

    d_l_thpt_per_ue = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "dLThptPerUE");
    if (d_l_thpt_per_ue) {
    d_l_thpt_per_ue_local_nonprim = OpenAPI_throughput_parseFromJSON(d_l_thpt_per_ue);
    if (!d_l_thpt_per_ue_local_nonprim) {
        ogs_error("OpenAPI_throughput_parseFromJSON failed [d_l_thpt_per_ue]");
        goto end;
    }
    }

    u_l_thpt_per_slice = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "uLThptPerSlice");
    if (u_l_thpt_per_slice) {
    u_l_thpt_per_slice_local_nonprim = OpenAPI_throughput_parseFromJSON(u_l_thpt_per_slice);
    if (!u_l_thpt_per_slice_local_nonprim) {
        ogs_error("OpenAPI_throughput_parseFromJSON failed [u_l_thpt_per_slice]");
        goto end;
    }
    }

    u_l_thpt_per_ue = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "uLThptPerUE");
    if (u_l_thpt_per_ue) {
    u_l_thpt_per_ue_local_nonprim = OpenAPI_throughput_parseFromJSON(u_l_thpt_per_ue);
    if (!u_l_thpt_per_ue_local_nonprim) {
        ogs_error("OpenAPI_throughput_parseFromJSON failed [u_l_thpt_per_ue]");
        goto end;
    }
    }

    max_numberof_pd_usessions = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "maxNumberofPDUsessions");
    if (max_numberof_pd_usessions) {
    if (!cJSON_IsNumber(max_numberof_pd_usessions)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [max_numberof_pd_usessions]");
        goto end;
    }
    }

    k_pi_monitoring_list = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "kPIMonitoringList");
    if (k_pi_monitoring_list) {
    if (!cJSON_IsString(k_pi_monitoring_list) && !cJSON_IsNull(k_pi_monitoring_list)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [k_pi_monitoring_list]");
        goto end;
    }
    }

    supported_access_technology = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "supportedAccessTechnology");
    if (supported_access_technology) {
    if (!cJSON_IsNumber(supported_access_technology)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [supported_access_technology]");
        goto end;
    }
    }

    v2_x_communication_mode_indicator = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "v2XCommunicationModeIndicator");
    if (v2_x_communication_mode_indicator) {
    if (!cJSON_IsString(v2_x_communication_mode_indicator)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [v2_x_communication_mode_indicator]");
        goto end;
    }
    v2_x_communication_mode_indicatorVariable = OpenAPI_support_FromString(v2_x_communication_mode_indicator->valuestring);
    }

    energy_efficiency = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "energyEfficiency");
    if (energy_efficiency) {
    energy_efficiency_local_nonprim = OpenAPI_ee_perf_req_parseFromJSON(energy_efficiency);
    if (!energy_efficiency_local_nonprim) {
        ogs_error("OpenAPI_ee_perf_req_parseFromJSON failed [energy_efficiency]");
        goto end;
    }
    }

    add_service_profile_info = cJSON_GetObjectItemCaseSensitive(service_profile_charging_informationJSON, "addServiceProfileInfo");
    if (add_service_profile_info) {
    if (!cJSON_IsString(add_service_profile_info) && !cJSON_IsNull(add_service_profile_info)) {
        ogs_error("OpenAPI_service_profile_charging_information_parseFromJSON() failed [add_service_profile_info]");
        goto end;
    }
    }

    service_profile_charging_information_local_var = OpenAPI_service_profile_charging_information_create (
        service_profile_identifier && !cJSON_IsNull(service_profile_identifier) ? ogs_strdup(service_profile_identifier->valuestring) : NULL,
        s_nssai_list ? s_nssai_listList : NULL,
        s_st ? true : false,
        s_st ? s_st->valuedouble : 0,
        latency ? true : false,
        latency ? latency->valuedouble : 0,
        availability ? true : false,
        availability ? availability->valuedouble : 0,
        resource_sharing_level ? resource_sharing_levelVariable : 0,
        jitter ? true : false,
        jitter ? jitter->valuedouble : 0,
        reliability && !cJSON_IsNull(reliability) ? ogs_strdup(reliability->valuestring) : NULL,
        max_numberof_ues ? true : false,
        max_numberof_ues ? max_numberof_ues->valuedouble : 0,
        coverage_area && !cJSON_IsNull(coverage_area) ? ogs_strdup(coverage_area->valuestring) : NULL,
        u_e_mobility_level ? u_e_mobility_levelVariable : 0,
        delay_tolerance_indicator ? delay_tolerance_indicatorVariable : 0,
        d_l_thpt_per_slice ? d_l_thpt_per_slice_local_nonprim : NULL,
        d_l_thpt_per_ue ? d_l_thpt_per_ue_local_nonprim : NULL,
        u_l_thpt_per_slice ? u_l_thpt_per_slice_local_nonprim : NULL,
        u_l_thpt_per_ue ? u_l_thpt_per_ue_local_nonprim : NULL,
        max_numberof_pd_usessions ? true : false,
        max_numberof_pd_usessions ? max_numberof_pd_usessions->valuedouble : 0,
        k_pi_monitoring_list && !cJSON_IsNull(k_pi_monitoring_list) ? ogs_strdup(k_pi_monitoring_list->valuestring) : NULL,
        supported_access_technology ? true : false,
        supported_access_technology ? supported_access_technology->valuedouble : 0,
        v2_x_communication_mode_indicator ? v2_x_communication_mode_indicatorVariable : 0,
        energy_efficiency ? energy_efficiency_local_nonprim : NULL,
        add_service_profile_info && !cJSON_IsNull(add_service_profile_info) ? ogs_strdup(add_service_profile_info->valuestring) : NULL
    );

    return service_profile_charging_information_local_var;
end:
    if (s_nssai_listList) {
        OpenAPI_list_for_each(s_nssai_listList, node) {
            OpenAPI_snssai_free(node->data);
        }
        OpenAPI_list_free(s_nssai_listList);
        s_nssai_listList = NULL;
    }
    if (d_l_thpt_per_slice_local_nonprim) {
        OpenAPI_throughput_free(d_l_thpt_per_slice_local_nonprim);
        d_l_thpt_per_slice_local_nonprim = NULL;
    }
    if (d_l_thpt_per_ue_local_nonprim) {
        OpenAPI_throughput_free(d_l_thpt_per_ue_local_nonprim);
        d_l_thpt_per_ue_local_nonprim = NULL;
    }
    if (u_l_thpt_per_slice_local_nonprim) {
        OpenAPI_throughput_free(u_l_thpt_per_slice_local_nonprim);
        u_l_thpt_per_slice_local_nonprim = NULL;
    }
    if (u_l_thpt_per_ue_local_nonprim) {
        OpenAPI_throughput_free(u_l_thpt_per_ue_local_nonprim);
        u_l_thpt_per_ue_local_nonprim = NULL;
    }
    if (energy_efficiency_local_nonprim) {
        OpenAPI_ee_perf_req_free(energy_efficiency_local_nonprim);
        energy_efficiency_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_service_profile_charging_information_t *OpenAPI_service_profile_charging_information_copy(OpenAPI_service_profile_charging_information_t *dst, OpenAPI_service_profile_charging_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_service_profile_charging_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_service_profile_charging_information_convertToJSON() failed");
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

    OpenAPI_service_profile_charging_information_free(dst);
    dst = OpenAPI_service_profile_charging_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

