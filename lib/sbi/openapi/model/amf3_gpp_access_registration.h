/*
 * amf3_gpp_access_registration.h
 *
 * This datatype contains the set of information relevant to the AMF where the UE has registered via 3GPP access. 
 */

#ifndef _OpenAPI_amf3_gpp_access_registration_H_
#define _OpenAPI_amf3_gpp_access_registration_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
typedef struct OpenAPI_amf3_gpp_access_registration_s OpenAPI_amf3_gpp_access_registration_t;
#include "backup_amf_info.h"
#include "context_info.h"
#include "eps_interworking_info.h"
#include "guami.h"
#include "ims_vo_ps.h"
#include "rat_type.h"
#include "service_name.h"
#include "ue_reachable_ind.h"
#include "vgmlc_address.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OpenAPI_amf3_gpp_access_registration_s {
    char *amf_instance_id;
    char *supported_features;
    bool is_purge_flag;
    int purge_flag;
    char *pei;
    OpenAPI_ims_vo_ps_e ims_vo_ps;
    char *dereg_callback_uri;
    OpenAPI_service_name_e amf_service_name_dereg;
    char *pcscf_restoration_callback_uri;
    OpenAPI_service_name_e amf_service_name_pcscf_rest;
    bool is_initial_registration_ind;
    int initial_registration_ind;
    bool is_emergency_registration_ind;
    int emergency_registration_ind;
    struct OpenAPI_guami_s *guami;
    OpenAPI_list_t *backup_amf_info;
    bool is_dr_flag;
    int dr_flag;
    bool is_eps5_gs_mobility_wo_n26;
    int eps5_gs_mobility_wo_n26;
    OpenAPI_rat_type_e rat_type;
    bool is_urrp_indicator;
    int urrp_indicator;
    char *amf_ee_subscription_id;
    struct OpenAPI_eps_interworking_info_s *eps_interworking_info;
    bool is_ue_srvcc_capability;
    int ue_srvcc_capability;
    char *registration_time;
    struct OpenAPI_vgmlc_address_s *vgmlc_address;
    struct OpenAPI_context_info_s *context_info;
    bool is_no_ee_subscription_ind;
    int no_ee_subscription_ind;
    char *supi;
    OpenAPI_ue_reachable_ind_e ue_reachable_ind;
    bool is_re_registration_required;
    int re_registration_required;
    bool is_admin_dereg_sub_withdrawn;
    int admin_dereg_sub_withdrawn;
    char *data_restoration_callback_uri;
    OpenAPI_list_t *reset_ids;
    bool is_disaster_roaming_ind;
    int disaster_roaming_ind;
    bool is_ue_mint_capability;
    int ue_mint_capability;
    bool is_ue_mint_eps_capability;
    int ue_mint_eps_capability;
    bool is_sor_snpn_si_supported;
    int sor_snpn_si_supported;
    bool is_udr_restart_ind;
    int udr_restart_ind;
    char *last_synchronization_time;
    bool is_ue_snpn_subscription_ind;
    int ue_snpn_subscription_ind;
};

OpenAPI_amf3_gpp_access_registration_t *OpenAPI_amf3_gpp_access_registration_create(
    char *amf_instance_id,
    char *supported_features,
    bool is_purge_flag,
    int purge_flag,
    char *pei,
    OpenAPI_ims_vo_ps_e ims_vo_ps,
    char *dereg_callback_uri,
    OpenAPI_service_name_e amf_service_name_dereg,
    char *pcscf_restoration_callback_uri,
    OpenAPI_service_name_e amf_service_name_pcscf_rest,
    bool is_initial_registration_ind,
    int initial_registration_ind,
    bool is_emergency_registration_ind,
    int emergency_registration_ind,
    OpenAPI_guami_t *guami,
    OpenAPI_list_t *backup_amf_info,
    bool is_dr_flag,
    int dr_flag,
    bool is_eps5_gs_mobility_wo_n26,
    int eps5_gs_mobility_wo_n26,
    OpenAPI_rat_type_e rat_type,
    bool is_urrp_indicator,
    int urrp_indicator,
    char *amf_ee_subscription_id,
    OpenAPI_eps_interworking_info_t *eps_interworking_info,
    bool is_ue_srvcc_capability,
    int ue_srvcc_capability,
    char *registration_time,
    OpenAPI_vgmlc_address_t *vgmlc_address,
    OpenAPI_context_info_t *context_info,
    bool is_no_ee_subscription_ind,
    int no_ee_subscription_ind,
    char *supi,
    OpenAPI_ue_reachable_ind_e ue_reachable_ind,
    bool is_re_registration_required,
    int re_registration_required,
    bool is_admin_dereg_sub_withdrawn,
    int admin_dereg_sub_withdrawn,
    char *data_restoration_callback_uri,
    OpenAPI_list_t *reset_ids,
    bool is_disaster_roaming_ind,
    int disaster_roaming_ind,
    bool is_ue_mint_capability,
    int ue_mint_capability,
    bool is_ue_mint_eps_capability,
    int ue_mint_eps_capability,
    bool is_sor_snpn_si_supported,
    int sor_snpn_si_supported,
    bool is_udr_restart_ind,
    int udr_restart_ind,
    char *last_synchronization_time,
    bool is_ue_snpn_subscription_ind,
    int ue_snpn_subscription_ind
);
void OpenAPI_amf3_gpp_access_registration_free(OpenAPI_amf3_gpp_access_registration_t *amf3_gpp_access_registration);
OpenAPI_amf3_gpp_access_registration_t *OpenAPI_amf3_gpp_access_registration_parseFromJSON(cJSON *amf3_gpp_access_registrationJSON);
cJSON *OpenAPI_amf3_gpp_access_registration_convertToJSON(OpenAPI_amf3_gpp_access_registration_t *amf3_gpp_access_registration);
OpenAPI_amf3_gpp_access_registration_t *OpenAPI_amf3_gpp_access_registration_copy(OpenAPI_amf3_gpp_access_registration_t *dst, OpenAPI_amf3_gpp_access_registration_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_amf3_gpp_access_registration_H_ */

