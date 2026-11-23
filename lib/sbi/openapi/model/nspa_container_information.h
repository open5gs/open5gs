/*
 * nspa_container_information.h
 *
 * 
 */

#ifndef _OpenAPI_nspa_container_information_H_
#define _OpenAPI_nspa_container_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_nspa_container_information_s OpenAPI_nspa_container_information_t;
#include "charging_service_experience_info.h"
#include "nf_identification.h"
#include "nsi_load_level_info.h"
#include "throughput.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_nspa_container_information_s {
    bool is_uplink_latency;
    int uplink_latency;
    bool is_downlink_latency;
    int downlink_latency;
    struct OpenAPI_throughput_s *uplink_throughput;
    struct OpenAPI_throughput_s *downlink_throughput;
    bool is_maximum_packet_loss_rate_ul;
    int maximum_packet_loss_rate_ul;
    bool is_maximum_packet_loss_rate_dl;
    int maximum_packet_loss_rate_dl;
    struct OpenAPI_charging_service_experience_info_s *service_experience_statistics_data;
    bool is_the_number_of_pdu_sessions;
    int the_number_of_pdu_sessions;
    bool is_the_number_of_registered_subscribers;
    int the_number_of_registered_subscribers;
    struct OpenAPI_nsi_load_level_info_s *load_level;
    bool is_estimated_energy_consumption;
    int estimated_energy_consumption;
    struct OpenAPI_nf_identification_s *source_nf_identification;
};

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
);
void OpenAPI_nspa_container_information_free(OpenAPI_nspa_container_information_t *nspa_container_information);
OpenAPI_nspa_container_information_t *OpenAPI_nspa_container_information_parseFromJSON(cJSON *nspa_container_informationJSON);
cJSON *OpenAPI_nspa_container_information_convertToJSON(OpenAPI_nspa_container_information_t *nspa_container_information);
OpenAPI_nspa_container_information_t *OpenAPI_nspa_container_information_copy(OpenAPI_nspa_container_information_t *dst, OpenAPI_nspa_container_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nspa_container_information_H_ */

