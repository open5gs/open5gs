/*
 * prose_charging_information.h
 *
 * 
 */

#ifndef _OpenAPI_prose_charging_information_H_
#define _OpenAPI_prose_charging_information_H_

#include <string.h>
#include "third-party/cjson/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_prose_charging_information_s OpenAPI_prose_charging_information_t;
#include "direct_discovery_model.h"
#include "intermediate_relay_information_container.h"
#include "ip_addr.h"
#include "pc5_data_container.h"
#include "pfi_container_information.h"
#include "plmn_id.h"
#include "prose_event_type.h"
#include "prose_functionality.h"
#include "range_class.h"
#include "role_of_ue.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_prose_charging_information_s {
    struct OpenAPI_plmn_id_s *announcing_plmn_id;
    struct OpenAPI_plmn_id_s *announcing_ue_hplmn_identifier;
    struct OpenAPI_plmn_id_s *announcing_ue_vplmn_identifier;
    struct OpenAPI_plmn_id_s *monitoring_ue_hplmn_identifier;
    struct OpenAPI_plmn_id_s *monitoring_ue_vplmn_identifier;
    struct OpenAPI_plmn_id_s *discoverer_ue_hplmn_identifier;
    struct OpenAPI_plmn_id_s *discoverer_ue_vplmn_identifier;
    struct OpenAPI_plmn_id_s *discoveree_ue_hplmn_identifier;
    struct OpenAPI_plmn_id_s *discoveree_ue_vplmn_identifier;
    struct OpenAPI_plmn_id_s *monitored_plmn_identifier;
    char *prose_application_id;
    char *application_id;
    OpenAPI_list_t *application_specific_data_list;
    OpenAPI_prose_functionality_e prose_functionality;
    OpenAPI_prose_event_type_e prose_event_type;
    OpenAPI_direct_discovery_model_e direct_discovery_model;
    bool is_validity_period;
    int validity_period;
    OpenAPI_role_of_ue_e role_of_ue;
    char *prose_request_timestamp;
    bool is_p_c3_protocol_cause;
    int p_c3_protocol_cause;
    char *monitoring_ue_identifier;
    struct OpenAPI_plmn_id_s *requested_plmn_identifier;
    bool is_time_window;
    int time_window;
    OpenAPI_range_class_e range_class;
    bool is_proximity_alert_indication;
    int proximity_alert_indication;
    char *proximity_alert_timestamp;
    char *proximity_cancellation_timestamp;
    bool is_hop_count;
    int hop_count;
    OpenAPI_list_t *intermediate_relay_information_container;
    struct OpenAPI_ip_addr_s *relay_ip_address;
    char *prose_ueto_network_relay_ueid;
    char *prose_ueto_ue_relay_ueid;
    struct OpenAPI_ip_addr_s *prose_ueto_ue_target_end_ueip_address;
    char *prose_destination_layer2_id;
    OpenAPI_list_t *p_fi_container_information;
    OpenAPI_list_t *transmission_data_container;
    OpenAPI_list_t *reception_data_container;
};

OpenAPI_prose_charging_information_t *OpenAPI_prose_charging_information_create(
    OpenAPI_plmn_id_t *announcing_plmn_id,
    OpenAPI_plmn_id_t *announcing_ue_hplmn_identifier,
    OpenAPI_plmn_id_t *announcing_ue_vplmn_identifier,
    OpenAPI_plmn_id_t *monitoring_ue_hplmn_identifier,
    OpenAPI_plmn_id_t *monitoring_ue_vplmn_identifier,
    OpenAPI_plmn_id_t *discoverer_ue_hplmn_identifier,
    OpenAPI_plmn_id_t *discoverer_ue_vplmn_identifier,
    OpenAPI_plmn_id_t *discoveree_ue_hplmn_identifier,
    OpenAPI_plmn_id_t *discoveree_ue_vplmn_identifier,
    OpenAPI_plmn_id_t *monitored_plmn_identifier,
    char *prose_application_id,
    char *application_id,
    OpenAPI_list_t *application_specific_data_list,
    OpenAPI_prose_functionality_e prose_functionality,
    OpenAPI_prose_event_type_e prose_event_type,
    OpenAPI_direct_discovery_model_e direct_discovery_model,
    bool is_validity_period,
    int validity_period,
    OpenAPI_role_of_ue_e role_of_ue,
    char *prose_request_timestamp,
    bool is_p_c3_protocol_cause,
    int p_c3_protocol_cause,
    char *monitoring_ue_identifier,
    OpenAPI_plmn_id_t *requested_plmn_identifier,
    bool is_time_window,
    int time_window,
    OpenAPI_range_class_e range_class,
    bool is_proximity_alert_indication,
    int proximity_alert_indication,
    char *proximity_alert_timestamp,
    char *proximity_cancellation_timestamp,
    bool is_hop_count,
    int hop_count,
    OpenAPI_list_t *intermediate_relay_information_container,
    OpenAPI_ip_addr_t *relay_ip_address,
    char *prose_ueto_network_relay_ueid,
    char *prose_ueto_ue_relay_ueid,
    OpenAPI_ip_addr_t *prose_ueto_ue_target_end_ueip_address,
    char *prose_destination_layer2_id,
    OpenAPI_list_t *p_fi_container_information,
    OpenAPI_list_t *transmission_data_container,
    OpenAPI_list_t *reception_data_container
);
void OpenAPI_prose_charging_information_free(OpenAPI_prose_charging_information_t *prose_charging_information);
OpenAPI_prose_charging_information_t *OpenAPI_prose_charging_information_parseFromJSON(cJSON *prose_charging_informationJSON);
cJSON *OpenAPI_prose_charging_information_convertToJSON(OpenAPI_prose_charging_information_t *prose_charging_information);
OpenAPI_prose_charging_information_t *OpenAPI_prose_charging_information_copy(OpenAPI_prose_charging_information_t *dst, OpenAPI_prose_charging_information_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_prose_charging_information_H_ */

