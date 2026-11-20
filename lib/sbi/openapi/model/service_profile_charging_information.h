/*
 * service_profile_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_service_profile_charging_information_H_
#define _OpenAPI_service_profile_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_service_profile_charging_information_s OpenAPI_service_profile_charging_information_t;
#include "ee_perf_req.h"
#include "mobility_level.h"
#include "sharing_level.h"
#include "snssai.h"
#include "support.h"
#include "throughput.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_service_profile_charging_information_s {
    char *service_profile_identifier;
    OpenAPI_list_t *s_nssai_list;
    bool is_s_st;
    int s_st;
    bool is_latency;
    int latency;
    bool is_availability;
    double availability;
    OpenAPI_sharing_level_e resource_sharing_level;
    bool is_jitter;
    int jitter;
    char *reliability;
    bool is_max_numberof_ues;
    int max_numberof_ues;
    char *coverage_area;
    OpenAPI_mobility_level_e u_e_mobility_level;
    OpenAPI_support_e delay_tolerance_indicator;
    struct OpenAPI_throughput_s *d_l_thpt_per_slice;
    struct OpenAPI_throughput_s *d_l_thpt_per_ue;
    struct OpenAPI_throughput_s *u_l_thpt_per_slice;
    struct OpenAPI_throughput_s *u_l_thpt_per_ue;
    bool is_max_numberof_pd_usessions;
    int max_numberof_pd_usessions;
    char *k_pi_monitoring_list;
    bool is_supported_access_technology;
    int supported_access_technology;
    OpenAPI_support_e v2_x_communication_mode_indicator;
    struct OpenAPI_ee_perf_req_s *energy_efficiency;
    char *add_service_profile_info;
};

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
);
void OpenAPI_service_profile_charging_information_free(OpenAPI_service_profile_charging_information_t *service_profile_charging_information);
OpenAPI_service_profile_charging_information_t *OpenAPI_service_profile_charging_information_parseFromJSON(cJSON *service_profile_charging_informationJSON);
cJSON *OpenAPI_service_profile_charging_information_convertToJSON(OpenAPI_service_profile_charging_information_t *service_profile_charging_information);
OpenAPI_service_profile_charging_information_t *OpenAPI_service_profile_charging_information_copy(OpenAPI_service_profile_charging_information_t *dst, OpenAPI_service_profile_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_service_profile_charging_information_H_ */

