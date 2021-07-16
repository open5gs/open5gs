/*
 * hsmf_updated_data.h
 *
 * 
 */

#ifndef _OpenAPI_hsmf_updated_data_H_
#define _OpenAPI_hsmf_updated_data_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "ambr.h"
#include "eps_bearer_info.h"
#include "eps_pdn_cnx_info.h"
#include "max_integrity_protected_data_rate.h"
#include "n4_information.h"
#include "qos_flow_setup_item.h"
#include "ref_to_binary_data.h"
#include "roaming_charging_profile.h"
#include "up_security.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_hsmf_updated_data_s OpenAPI_hsmf_updated_data_t;
typedef struct OpenAPI_hsmf_updated_data_s {
    struct OpenAPI_ref_to_binary_data_s *n1_sm_info_to_ue;
    struct OpenAPI_n4_information_s *n4_info;
    struct OpenAPI_n4_information_s *n4_info_ext1;
    struct OpenAPI_n4_information_s *n4_info_ext2;
    OpenAPI_list_t *dnai_list;
    char *supported_features;
    struct OpenAPI_roaming_charging_profile_s *roaming_charging_profile;
    struct OpenAPI_up_security_s *up_security;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ul;
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl;
    bool is_ipv6_multi_homing_ind;
    int ipv6_multi_homing_ind;
    OpenAPI_list_t *qos_flows_setup_list;
    struct OpenAPI_ambr_s *session_ambr;
    struct OpenAPI_eps_pdn_cnx_info_s *eps_pdn_cnx_info;
    OpenAPI_list_t *eps_bearer_info;
    bool is_pti;
    int pti;
} OpenAPI_hsmf_updated_data_t;

OpenAPI_hsmf_updated_data_t *OpenAPI_hsmf_updated_data_create(
    OpenAPI_ref_to_binary_data_t *n1_sm_info_to_ue,
    OpenAPI_n4_information_t *n4_info,
    OpenAPI_n4_information_t *n4_info_ext1,
    OpenAPI_n4_information_t *n4_info_ext2,
    OpenAPI_list_t *dnai_list,
    char *supported_features,
    OpenAPI_roaming_charging_profile_t *roaming_charging_profile,
    OpenAPI_up_security_t *up_security,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_ul,
    OpenAPI_max_integrity_protected_data_rate_e max_integrity_protected_data_rate_dl,
    bool is_ipv6_multi_homing_ind,
    int ipv6_multi_homing_ind,
    OpenAPI_list_t *qos_flows_setup_list,
    OpenAPI_ambr_t *session_ambr,
    OpenAPI_eps_pdn_cnx_info_t *eps_pdn_cnx_info,
    OpenAPI_list_t *eps_bearer_info,
    bool is_pti,
    int pti
);
void OpenAPI_hsmf_updated_data_free(OpenAPI_hsmf_updated_data_t *hsmf_updated_data);
OpenAPI_hsmf_updated_data_t *OpenAPI_hsmf_updated_data_parseFromJSON(cJSON *hsmf_updated_dataJSON);
cJSON *OpenAPI_hsmf_updated_data_convertToJSON(OpenAPI_hsmf_updated_data_t *hsmf_updated_data);
OpenAPI_hsmf_updated_data_t *OpenAPI_hsmf_updated_data_copy(OpenAPI_hsmf_updated_data_t *dst, OpenAPI_hsmf_updated_data_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_hsmf_updated_data_H_ */

