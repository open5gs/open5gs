/*
 * pdu_session_created_data.h
 *
 *
 */

#ifndef _OpenAPI_pdu_session_created_data_H_
#define _OpenAPI_pdu_session_created_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ambr.h"
#include "eps_bearer_info.h"
#include "eps_pdn_cnx_info.h"
#include "max_integrity_protected_data_rate.h"
#include "pdu_session_type.h"
#include "qos_flow_setup_item.h"
#include "ref_to_binary_data.h"
#include "roaming_charging_profile.h"
#include "snssai.h"
#include "tunnel_info.h"
#include "up_security.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_pdu_session_created_data_s OpenAPI_pdu_session_created_data_t;
typedef struct OpenAPI_pdu_session_created_data_s {
    OpenAPI_pdu_session_type_e pdu_session_type;
    char *ssc_mode;
    struct OpenAPI_tunnel_info_s *hcn_tunnel_info;
    struct OpenAPI_tunnel_info_s *cn_tunnel_info;
    struct OpenAPI_tunnel_info_s *additional_cn_tunnel_info;
    struct OpenAPI_ambr_s *session_ambr;
    OpenAPI_list_t *qos_flows_setup_list;
    char *h_smf_instance_id;
    char *smf_instance_id;
    int pdu_session_id;
    struct OpenAPI_snssai_s *s_nssai;
    int enable_pause_charging;
    char *ue_ipv4_address;
    char *ue_ipv6_prefix;
    struct OpenAPI_ref_to_binary_data_s *n1_sm_info_to_ue;
    struct OpenAPI_eps_pdn_cnx_info_s *eps_pdn_cnx_info;
    OpenAPI_list_t *eps_bearer_info;
    char *supported_features;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate;
    int always_on_granted;
    char *gpsi;
    struct OpenAPI_up_security_s *up_security;
    struct OpenAPI_roaming_charging_profile_s *roaming_charging_profile;
    char *h_smf_service_instance_id;
    char *smf_service_instance_id;
    char *recovery_time;
    OpenAPI_list_t *dnai_list;
    int ipv6_multi_homing_ind;
    int ma_accepted_ind;
    char *home_provided_charging_id;
    int nef_ext_buf_support_ind;
} OpenAPI_pdu_session_created_data_t;

OpenAPI_pdu_session_created_data_t *OpenAPI_pdu_session_created_data_create(
    OpenAPI_pdu_session_type_e pdu_session_type,
    char *ssc_mode,
    OpenAPI_tunnel_info_t *hcn_tunnel_info,
    OpenAPI_tunnel_info_t *cn_tunnel_info,
    OpenAPI_tunnel_info_t *additional_cn_tunnel_info,
    OpenAPI_ambr_t *session_ambr,
    OpenAPI_list_t *qos_flows_setup_list,
    char *h_smf_instance_id,
    char *smf_instance_id,
    int pdu_session_id,
    OpenAPI_snssai_t *s_nssai,
    int enable_pause_charging,
    char *ue_ipv4_address,
    char *ue_ipv6_prefix,
    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue,
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info,
    OpenAPI_list_t *eps_bearer_info,
    char *supported_features,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate,
    int always_on_granted,
    char *gpsi,
    OpenAPI_up_security_t *up_security,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    char *h_smf_service_instance_id,
    char *smf_service_instance_id,
    char *recovery_time,
    OpenAPI_list_t *dnai_list,
    int ipv6_multi_homing_ind,
    int ma_accepted_ind,
    char *home_provided_charging_id,
    int nef_ext_buf_support_ind
    );
void OpenAPI_pdu_session_created_data_free(OpenAPI_pdu_session_created_data_t *pdu_session_created_data);
OpenAPI_pdu_session_created_data_t *OpenAPI_pdu_session_created_data_parseFromJSON(cJSON *pdu_session_created_dataJSON);
cJSON *OpenAPI_pdu_session_created_data_convertToJSON(OpenAPI_pdu_session_created_data_t *pdu_session_created_data);
OpenAPI_pdu_session_created_data_t *OpenAPI_pdu_session_created_data_copy(OpenAPI_pdu_session_created_data_t *dst, OpenAPI_pdu_session_created_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_pdu_session_created_data_H_ */

