/*
 * pdu_container_information.h
 *
 * 
 */

#ifndef _OpenAPI_pdu_container_information_H_
#define _OpenAPI_pdu_container_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_pdu_container_information_s OpenAPI_pdu_container_information_t;
#include "charging_qos_monitoring_report.h"
#include "charging_user_location.h"
#include "mbs_delivery_method.h"
#include "mbs_session_id.h"
#include "model_3_gppps_data_off_status.h"
#include "presence_info.h"
#include "qos_characteristics.h"
#include "qos_data.h"
#include "rat_type.h"
#include "serving_network_function_id.h"
#include "steering_functionality.h"
#include "steering_mode.h"
#include "traffic_forwarding_way.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_pdu_container_information_s {
    char *timeof_first_usage;
    char *timeof_last_usage;
    bool is_qo_s_information_null;
    struct OpenAPI_qos_data_s *qo_s_information;
    struct OpenAPI_qos_characteristics_s *qo_s_characteristics;
    bool is_af_charging_identifier;
    int af_charging_identifier;
    char *af_charging_id_string;
    struct OpenAPI_charging_user_location_s *user_location_information;
    char *uetime_zone;
    OpenAPI_rat_type_e r_at_type;
    OpenAPI_list_t *serving_node_id;
    OpenAPI_list_t* presence_reporting_area_information;
    OpenAPI_model_3_gppps_data_off_status_e _3gpp_ps_data_off_status;
    char *sponsor_identity;
    char *applicationservice_provider_identity;
    char *charging_rule_base_name;
    OpenAPI_steering_functionality_e m_apdu_steering_functionality;
    struct OpenAPI_steering_mode_s *m_apdu_steering_mode;
    OpenAPI_traffic_forwarding_way_e traffic_forwarding_way;
    OpenAPI_list_t *qos_monitoring_report;
    bool is_m_bs_session_id_null;
    struct OpenAPI_mbs_session_id_s *m_bs_session_id;
    OpenAPI_mbs_delivery_method_e m_bs_delivery_method;
};

OpenAPI_pdu_container_information_t *OpenAPI_pdu_container_information_create(
    char *timeof_first_usage,
    char *timeof_last_usage,
    bool is_qo_s_information_null,
    OpenAPI_qos_data_t *qo_s_information,
    OpenAPI_qos_characteristics_t *qo_s_characteristics,
    bool is_af_charging_identifier,
    int af_charging_identifier,
    char *af_charging_id_string,
    OpenAPI_charging_user_location_t *user_location_information,
    char *uetime_zone,
    OpenAPI_rat_type_e r_at_type,
    OpenAPI_list_t *serving_node_id,
    OpenAPI_list_t* presence_reporting_area_information,
    OpenAPI_model_3_gppps_data_off_status_e _3gpp_ps_data_off_status,
    char *sponsor_identity,
    char *applicationservice_provider_identity,
    char *charging_rule_base_name,
    OpenAPI_steering_functionality_e m_apdu_steering_functionality,
    OpenAPI_steering_mode_t *m_apdu_steering_mode,
    OpenAPI_traffic_forwarding_way_e traffic_forwarding_way,
    OpenAPI_list_t *qos_monitoring_report,
    bool is_m_bs_session_id_null,
    OpenAPI_mbs_session_id_t *m_bs_session_id,
    OpenAPI_mbs_delivery_method_e m_bs_delivery_method
);
void OpenAPI_pdu_container_information_free(OpenAPI_pdu_container_information_t *pdu_container_information);
OpenAPI_pdu_container_information_t *OpenAPI_pdu_container_information_parseFromJSON(cJSON *pdu_container_informationJSON);
cJSON *OpenAPI_pdu_container_information_convertToJSON(OpenAPI_pdu_container_information_t *pdu_container_information);
OpenAPI_pdu_container_information_t *OpenAPI_pdu_container_information_copy(OpenAPI_pdu_container_information_t *dst, OpenAPI_pdu_container_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_container_information_H_ */

