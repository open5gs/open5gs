
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "nspa_container_information.h"

OpenAPI_nspa_container_information_t *OpenAPI_nspa_container_information_create(
    bool is_uplink_latency,
    int uplink_latency,
    bool is_downlink_latency,
    int downlink_latency,
    OpenAPI_throughput_t *uplink_throughput,
    OpenAPI_throughput_t *downlink_throughput,
    bool is_maximum_packet_loss_rate_ul,
    int maximum_packet_loss_rate_ul,
    bool is_maximum_packet_loss_rate_dl,
    int maximum_packet_loss_rate_dl,
    OpenAPI_charging_service_experience_info_t *service_experience_statistics_data,
    bool is_the_number_of_pdu_sessions,
    int the_number_of_pdu_sessions,
    bool is_the_number_of_registered_subscribers,
    int the_number_of_registered_subscribers,
    OpenAPI_nsi_load_level_info_t *load_level,
    bool is_estimated_energy_consumption,
    int estimated_energy_consumption,
    OpenAPI_nf_identification_t *source_nf_identification
)
{
    OpenAPI_nspa_container_information_t *nspa_container_information_local_var = ogs_malloc(sizeof(OpenAPI_nspa_container_information_t));
    ogs_assert(nspa_container_information_local_var);

    nspa_container_information_local_var->is_uplink_latency = is_uplink_latency;
    nspa_container_information_local_var->uplink_latency = uplink_latency;
    nspa_container_information_local_var->is_downlink_latency = is_downlink_latency;
    nspa_container_information_local_var->downlink_latency = downlink_latency;
    nspa_container_information_local_var->uplink_throughput = uplink_throughput;
    nspa_container_information_local_var->downlink_throughput = downlink_throughput;
    nspa_container_information_local_var->is_maximum_packet_loss_rate_ul = is_maximum_packet_loss_rate_ul;
    nspa_container_information_local_var->maximum_packet_loss_rate_ul = maximum_packet_loss_rate_ul;
    nspa_container_information_local_var->is_maximum_packet_loss_rate_dl = is_maximum_packet_loss_rate_dl;
    nspa_container_information_local_var->maximum_packet_loss_rate_dl = maximum_packet_loss_rate_dl;
    nspa_container_information_local_var->service_experience_statistics_data = service_experience_statistics_data;
    nspa_container_information_local_var->is_the_number_of_pdu_sessions = is_the_number_of_pdu_sessions;
    nspa_container_information_local_var->the_number_of_pdu_sessions = the_number_of_pdu_sessions;
    nspa_container_information_local_var->is_the_number_of_registered_subscribers = is_the_number_of_registered_subscribers;
    nspa_container_information_local_var->the_number_of_registered_subscribers = the_number_of_registered_subscribers;
    nspa_container_information_local_var->load_level = load_level;
    nspa_container_information_local_var->is_estimated_energy_consumption = is_estimated_energy_consumption;
    nspa_container_information_local_var->estimated_energy_consumption = estimated_energy_consumption;
    nspa_container_information_local_var->source_nf_identification = source_nf_identification;

    return nspa_container_information_local_var;
}

void OpenAPI_nspa_container_information_free(OpenAPI_nspa_container_information_t *nspa_container_information)
{
    OpenAPI_lnode_t *node = NULL;

    if (NULL == nspa_container_information) {
        return;
    }
    if (nspa_container_information->uplink_throughput) {
        OpenAPI_throughput_free(nspa_container_information->uplink_throughput);
        nspa_container_information->uplink_throughput = NULL;
    }
    if (nspa_container_information->downlink_throughput) {
        OpenAPI_throughput_free(nspa_container_information->downlink_throughput);
        nspa_container_information->downlink_throughput = NULL;
    }
    if (nspa_container_information->service_experience_statistics_data) {
        OpenAPI_charging_service_experience_info_free(nspa_container_information->service_experience_statistics_data);
        nspa_container_information->service_experience_statistics_data = NULL;
    }
    if (nspa_container_information->load_level) {
        OpenAPI_nsi_load_level_info_free(nspa_container_information->load_level);
        nspa_container_information->load_level = NULL;
    }
    if (nspa_container_information->source_nf_identification) {
        OpenAPI_nf_identification_free(nspa_container_information->source_nf_identification);
        nspa_container_information->source_nf_identification = NULL;
    }
    ogs_free(nspa_container_information);
}

cJSON *OpenAPI_nspa_container_information_convertToJSON(OpenAPI_nspa_container_information_t *nspa_container_information)
{
    cJSON *item = NULL;
    OpenAPI_lnode_t *node = NULL;

    if (nspa_container_information == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [NSPAContainerInformation]");
        return NULL;
    }

    item = cJSON_CreateObject();
    if (nspa_container_information->is_uplink_latency) {
    if (cJSON_AddNumberToObject(item, "uplinkLatency", nspa_container_information->uplink_latency) == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [uplink_latency]");
        goto end;
    }
    }

    if (nspa_container_information->is_downlink_latency) {
    if (cJSON_AddNumberToObject(item, "downlinkLatency", nspa_container_information->downlink_latency) == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [downlink_latency]");
        goto end;
    }
    }

    if (nspa_container_information->uplink_throughput) {
    cJSON *uplink_throughput_local_JSON = OpenAPI_throughput_convertToJSON(nspa_container_information->uplink_throughput);
    if (uplink_throughput_local_JSON == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [uplink_throughput]");
        goto end;
    }
    cJSON_AddItemToObject(item, "uplinkThroughput", uplink_throughput_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [uplink_throughput]");
        goto end;
    }
    }

    if (nspa_container_information->downlink_throughput) {
    cJSON *downlink_throughput_local_JSON = OpenAPI_throughput_convertToJSON(nspa_container_information->downlink_throughput);
    if (downlink_throughput_local_JSON == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [downlink_throughput]");
        goto end;
    }
    cJSON_AddItemToObject(item, "downlinkThroughput", downlink_throughput_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [downlink_throughput]");
        goto end;
    }
    }

    if (nspa_container_information->is_maximum_packet_loss_rate_ul) {
    if (cJSON_AddNumberToObject(item, "maximumPacketLossRateUL", nspa_container_information->maximum_packet_loss_rate_ul) == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [maximum_packet_loss_rate_ul]");
        goto end;
    }
    }

    if (nspa_container_information->is_maximum_packet_loss_rate_dl) {
    if (cJSON_AddNumberToObject(item, "maximumPacketLossRateDL", nspa_container_information->maximum_packet_loss_rate_dl) == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [maximum_packet_loss_rate_dl]");
        goto end;
    }
    }

    if (nspa_container_information->service_experience_statistics_data) {
    cJSON *service_experience_statistics_data_local_JSON = OpenAPI_charging_service_experience_info_convertToJSON(nspa_container_information->service_experience_statistics_data);
    if (service_experience_statistics_data_local_JSON == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [service_experience_statistics_data]");
        goto end;
    }
    cJSON_AddItemToObject(item, "serviceExperienceStatisticsData", service_experience_statistics_data_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [service_experience_statistics_data]");
        goto end;
    }
    }

    if (nspa_container_information->is_the_number_of_pdu_sessions) {
    if (cJSON_AddNumberToObject(item, "theNumberOfPDUSessions", nspa_container_information->the_number_of_pdu_sessions) == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [the_number_of_pdu_sessions]");
        goto end;
    }
    }

    if (nspa_container_information->is_the_number_of_registered_subscribers) {
    if (cJSON_AddNumberToObject(item, "theNumberOfRegisteredSubscribers", nspa_container_information->the_number_of_registered_subscribers) == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [the_number_of_registered_subscribers]");
        goto end;
    }
    }

    if (nspa_container_information->load_level) {
    cJSON *load_level_local_JSON = OpenAPI_nsi_load_level_info_convertToJSON(nspa_container_information->load_level);
    if (load_level_local_JSON == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [load_level]");
        goto end;
    }
    cJSON_AddItemToObject(item, "loadLevel", load_level_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [load_level]");
        goto end;
    }
    }

    if (nspa_container_information->is_estimated_energy_consumption) {
    if (cJSON_AddNumberToObject(item, "estimatedEnergyConsumption", nspa_container_information->estimated_energy_consumption) == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [estimated_energy_consumption]");
        goto end;
    }
    }

    if (nspa_container_information->source_nf_identification) {
    cJSON *source_nf_identification_local_JSON = OpenAPI_nf_identification_convertToJSON(nspa_container_information->source_nf_identification);
    if (source_nf_identification_local_JSON == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [source_nf_identification]");
        goto end;
    }
    cJSON_AddItemToObject(item, "sourceNFIdentification", source_nf_identification_local_JSON);
    if (item->child == NULL) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed [source_nf_identification]");
        goto end;
    }
    }

end:
    return item;
}

OpenAPI_nspa_container_information_t *OpenAPI_nspa_container_information_parseFromJSON(cJSON *nspa_container_informationJSON)
{
    OpenAPI_nspa_container_information_t *nspa_container_information_local_var = NULL;
    OpenAPI_lnode_t *node = NULL;
    cJSON *uplink_latency = NULL;
    cJSON *downlink_latency = NULL;
    cJSON *uplink_throughput = NULL;
    OpenAPI_throughput_t *uplink_throughput_local_nonprim = NULL;
    cJSON *downlink_throughput = NULL;
    OpenAPI_throughput_t *downlink_throughput_local_nonprim = NULL;
    cJSON *maximum_packet_loss_rate_ul = NULL;
    cJSON *maximum_packet_loss_rate_dl = NULL;
    cJSON *service_experience_statistics_data = NULL;
    OpenAPI_charging_service_experience_info_t *service_experience_statistics_data_local_nonprim = NULL;
    cJSON *the_number_of_pdu_sessions = NULL;
    cJSON *the_number_of_registered_subscribers = NULL;
    cJSON *load_level = NULL;
    OpenAPI_nsi_load_level_info_t *load_level_local_nonprim = NULL;
    cJSON *estimated_energy_consumption = NULL;
    cJSON *source_nf_identification = NULL;
    OpenAPI_nf_identification_t *source_nf_identification_local_nonprim = NULL;
    uplink_latency = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "uplinkLatency");
    if (uplink_latency) {
    if (!cJSON_IsNumber(uplink_latency)) {
        ogs_error("OpenAPI_nspa_container_information_parseFromJSON() failed [uplink_latency]");
        goto end;
    }
    }

    downlink_latency = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "downlinkLatency");
    if (downlink_latency) {
    if (!cJSON_IsNumber(downlink_latency)) {
        ogs_error("OpenAPI_nspa_container_information_parseFromJSON() failed [downlink_latency]");
        goto end;
    }
    }

    uplink_throughput = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "uplinkThroughput");
    if (uplink_throughput) {
    uplink_throughput_local_nonprim = OpenAPI_throughput_parseFromJSON(uplink_throughput);
    if (!uplink_throughput_local_nonprim) {
        ogs_error("OpenAPI_throughput_parseFromJSON failed [uplink_throughput]");
        goto end;
    }
    }

    downlink_throughput = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "downlinkThroughput");
    if (downlink_throughput) {
    downlink_throughput_local_nonprim = OpenAPI_throughput_parseFromJSON(downlink_throughput);
    if (!downlink_throughput_local_nonprim) {
        ogs_error("OpenAPI_throughput_parseFromJSON failed [downlink_throughput]");
        goto end;
    }
    }

    maximum_packet_loss_rate_ul = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "maximumPacketLossRateUL");
    if (maximum_packet_loss_rate_ul) {
    if (!cJSON_IsNumber(maximum_packet_loss_rate_ul)) {
        ogs_error("OpenAPI_nspa_container_information_parseFromJSON() failed [maximum_packet_loss_rate_ul]");
        goto end;
    }
    }

    maximum_packet_loss_rate_dl = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "maximumPacketLossRateDL");
    if (maximum_packet_loss_rate_dl) {
    if (!cJSON_IsNumber(maximum_packet_loss_rate_dl)) {
        ogs_error("OpenAPI_nspa_container_information_parseFromJSON() failed [maximum_packet_loss_rate_dl]");
        goto end;
    }
    }

    service_experience_statistics_data = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "serviceExperienceStatisticsData");
    if (service_experience_statistics_data) {
    service_experience_statistics_data_local_nonprim = OpenAPI_charging_service_experience_info_parseFromJSON(service_experience_statistics_data);
    if (!service_experience_statistics_data_local_nonprim) {
        ogs_error("OpenAPI_charging_service_experience_info_parseFromJSON failed [service_experience_statistics_data]");
        goto end;
    }
    }

    the_number_of_pdu_sessions = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "theNumberOfPDUSessions");
    if (the_number_of_pdu_sessions) {
    if (!cJSON_IsNumber(the_number_of_pdu_sessions)) {
        ogs_error("OpenAPI_nspa_container_information_parseFromJSON() failed [the_number_of_pdu_sessions]");
        goto end;
    }
    }

    the_number_of_registered_subscribers = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "theNumberOfRegisteredSubscribers");
    if (the_number_of_registered_subscribers) {
    if (!cJSON_IsNumber(the_number_of_registered_subscribers)) {
        ogs_error("OpenAPI_nspa_container_information_parseFromJSON() failed [the_number_of_registered_subscribers]");
        goto end;
    }
    }

    load_level = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "loadLevel");
    if (load_level) {
    load_level_local_nonprim = OpenAPI_nsi_load_level_info_parseFromJSON(load_level);
    if (!load_level_local_nonprim) {
        ogs_error("OpenAPI_nsi_load_level_info_parseFromJSON failed [load_level]");
        goto end;
    }
    }

    estimated_energy_consumption = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "estimatedEnergyConsumption");
    if (estimated_energy_consumption) {
    if (!cJSON_IsNumber(estimated_energy_consumption)) {
        ogs_error("OpenAPI_nspa_container_information_parseFromJSON() failed [estimated_energy_consumption]");
        goto end;
    }
    }

    source_nf_identification = cJSON_GetObjectItemCaseSensitive(nspa_container_informationJSON, "sourceNFIdentification");
    if (source_nf_identification) {
    source_nf_identification_local_nonprim = OpenAPI_nf_identification_parseFromJSON(source_nf_identification);
    if (!source_nf_identification_local_nonprim) {
        ogs_error("OpenAPI_nf_identification_parseFromJSON failed [source_nf_identification]");
        goto end;
    }
    }

    nspa_container_information_local_var = OpenAPI_nspa_container_information_create (
        uplink_latency ? true : false,
        uplink_latency ? uplink_latency->valuedouble : 0,
        downlink_latency ? true : false,
        downlink_latency ? downlink_latency->valuedouble : 0,
        uplink_throughput ? uplink_throughput_local_nonprim : NULL,
        downlink_throughput ? downlink_throughput_local_nonprim : NULL,
        maximum_packet_loss_rate_ul ? true : false,
        maximum_packet_loss_rate_ul ? maximum_packet_loss_rate_ul->valuedouble : 0,
        maximum_packet_loss_rate_dl ? true : false,
        maximum_packet_loss_rate_dl ? maximum_packet_loss_rate_dl->valuedouble : 0,
        service_experience_statistics_data ? service_experience_statistics_data_local_nonprim : NULL,
        the_number_of_pdu_sessions ? true : false,
        the_number_of_pdu_sessions ? the_number_of_pdu_sessions->valuedouble : 0,
        the_number_of_registered_subscribers ? true : false,
        the_number_of_registered_subscribers ? the_number_of_registered_subscribers->valuedouble : 0,
        load_level ? load_level_local_nonprim : NULL,
        estimated_energy_consumption ? true : false,
        estimated_energy_consumption ? estimated_energy_consumption->valuedouble : 0,
        source_nf_identification ? source_nf_identification_local_nonprim : NULL
    );

    return nspa_container_information_local_var;
end:
    if (uplink_throughput_local_nonprim) {
        OpenAPI_throughput_free(uplink_throughput_local_nonprim);
        uplink_throughput_local_nonprim = NULL;
    }
    if (downlink_throughput_local_nonprim) {
        OpenAPI_throughput_free(downlink_throughput_local_nonprim);
        downlink_throughput_local_nonprim = NULL;
    }
    if (service_experience_statistics_data_local_nonprim) {
        OpenAPI_charging_service_experience_info_free(service_experience_statistics_data_local_nonprim);
        service_experience_statistics_data_local_nonprim = NULL;
    }
    if (load_level_local_nonprim) {
        OpenAPI_nsi_load_level_info_free(load_level_local_nonprim);
        load_level_local_nonprim = NULL;
    }
    if (source_nf_identification_local_nonprim) {
        OpenAPI_nf_identification_free(source_nf_identification_local_nonprim);
        source_nf_identification_local_nonprim = NULL;
    }
    return NULL;
}

OpenAPI_nspa_container_information_t *OpenAPI_nspa_container_information_copy(OpenAPI_nspa_container_information_t *dst, OpenAPI_nspa_container_information_t *src)
{
    cJSON *item = NULL;
    char *content = NULL;

    ogs_assert(src);
    item = OpenAPI_nspa_container_information_convertToJSON(src);
    if (!item) {
        ogs_error("OpenAPI_nspa_container_information_convertToJSON() failed");
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

    OpenAPI_nspa_container_information_free(dst);
    dst = OpenAPI_nspa_container_information_parseFromJSON(item);
    cJSON_Delete(item);

    return dst;
}

