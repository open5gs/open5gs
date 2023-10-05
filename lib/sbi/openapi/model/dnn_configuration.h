/*
 * dnn_configuration.h
 *
 * 
 */

#ifndef _OpenAPI_dnn_configuration_H_
#define _OpenAPI_dnn_configuration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "acs_info.h"
#include "aerial_ue_indication.h"
#include "ambr.h"
#include "ecs_addr_config_info.h"
#include "frame_route_info.h"
#include "ip_address.h"
#include "ip_index.h"
#include "nidd_information.h"
#include "pdu_session_continuity_ind.h"
#include "pdu_session_types.h"
#include "ssc_modes.h"
#include "subscribed_default_qos.h"
#include "up_security.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_dnn_configuration_s OpenAPI_dnn_configuration_t;
typedef struct OpenAPI_dnn_configuration_s {
    struct OpenAPI_pdu_session_types_s *pdu_session_types;
    struct OpenAPI_ssc_modes_s *ssc_modes;
    bool is_iwk_eps_ind;
    int iwk_eps_ind;
    struct OpenAPI_subscribed_default_qos_s *_5g_qos_profile;
    struct OpenAPI_ambr_s *session_ambr;
    char *_3gpp_charging_characteristics;
    OpenAPI_list_t *static_ip_address;
    struct OpenAPI_up_security_s *up_security;
    OpenAPI_pdu_session_continuity_ind_e pdu_session_continuity_ind;
    char *nidd_nef_id;
    struct OpenAPI_nidd_information_s *nidd_info;
    bool is_redundant_session_allowed;
    int redundant_session_allowed;
    struct OpenAPI_acs_info_s *acs_info;
    OpenAPI_list_t *ipv4_frame_route_list;
    OpenAPI_list_t *ipv6_frame_route_list;
    bool is_atsss_allowed;
    int atsss_allowed;
    bool is_secondary_auth;
    int secondary_auth;
    bool is_uav_secondary_auth;
    int uav_secondary_auth;
    bool is_dn_aaa_ip_address_allocation;
    int dn_aaa_ip_address_allocation;
    struct OpenAPI_ip_address_s *dn_aaa_address;
    OpenAPI_list_t *additional_dn_aaa_addresses;
    char *dn_aaa_fqdn;
    char *iptv_acc_ctrl_info;
    struct OpenAPI_ip_index_s *ipv4_index;
    struct OpenAPI_ip_index_s *ipv6_index;
    bool is_ecs_addr_config_info_null;
    struct OpenAPI_ecs_addr_config_info_s *ecs_addr_config_info;
    OpenAPI_list_t *additional_ecs_addr_config_infos;
    char *shared_ecs_addr_config_info;
    OpenAPI_list_t *additional_shared_ecs_addr_config_info_ids;
    bool is_eas_discovery_authorized;
    int eas_discovery_authorized;
    bool is_onboarding_ind;
    int onboarding_ind;
    OpenAPI_aerial_ue_indication_e aerial_ue_ind;
    bool is_subscribed_max_ipv6_prefix_size;
    int subscribed_max_ipv6_prefix_size;
} OpenAPI_dnn_configuration_t;

OpenAPI_dnn_configuration_t *OpenAPI_dnn_configuration_create(
    OpenAPI_pdu_session_types_t *pdu_session_types,
    OpenAPI_ssc_modes_t *ssc_modes,
    bool is_iwk_eps_ind,
    int iwk_eps_ind,
    OpenAPI_subscribed_default_qos_t *_5g_qos_profile,
    OpenAPI_ambr_t *session_ambr,
    char *_3gpp_charging_characteristics,
    OpenAPI_list_t *static_ip_address,
    OpenAPI_up_security_t *up_security,
    OpenAPI_pdu_session_continuity_ind_e pdu_session_continuity_ind,
    char *nidd_nef_id,
    OpenAPI_nidd_information_t *nidd_info,
    bool is_redundant_session_allowed,
    int redundant_session_allowed,
    OpenAPI_acs_info_t *acs_info,
    OpenAPI_list_t *ipv4_frame_route_list,
    OpenAPI_list_t *ipv6_frame_route_list,
    bool is_atsss_allowed,
    int atsss_allowed,
    bool is_secondary_auth,
    int secondary_auth,
    bool is_uav_secondary_auth,
    int uav_secondary_auth,
    bool is_dn_aaa_ip_address_allocation,
    int dn_aaa_ip_address_allocation,
    OpenAPI_ip_address_t *dn_aaa_address,
    OpenAPI_list_t *additional_dn_aaa_addresses,
    char *dn_aaa_fqdn,
    char *iptv_acc_ctrl_info,
    OpenAPI_ip_index_t *ipv4_index,
    OpenAPI_ip_index_t *ipv6_index,
    bool is_ecs_addr_config_info_null,
    OpenAPI_ecs_addr_config_info_t *ecs_addr_config_info,
    OpenAPI_list_t *additional_ecs_addr_config_infos,
    char *shared_ecs_addr_config_info,
    OpenAPI_list_t *additional_shared_ecs_addr_config_info_ids,
    bool is_eas_discovery_authorized,
    int eas_discovery_authorized,
    bool is_onboarding_ind,
    int onboarding_ind,
    OpenAPI_aerial_ue_indication_e aerial_ue_ind,
    bool is_subscribed_max_ipv6_prefix_size,
    int subscribed_max_ipv6_prefix_size
);
void OpenAPI_dnn_configuration_free(OpenAPI_dnn_configuration_t *dnn_configuration);
OpenAPI_dnn_configuration_t *OpenAPI_dnn_configuration_parseFromJSON(cJSON *dnn_configurationJSON);
cJSON *OpenAPI_dnn_configuration_convertToJSON(OpenAPI_dnn_configuration_t *dnn_configuration);
OpenAPI_dnn_configuration_t *OpenAPI_dnn_configuration_copy(OpenAPI_dnn_configuration_t *dst, OpenAPI_dnn_configuration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_dnn_configuration_H_ */

