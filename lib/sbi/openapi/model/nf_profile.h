/*
 * nf_profile.h
 *
 * Information of an NF Instance registered in the NRF
 */

#ifndef _OpenAPI_nf_profile_H_
#define _OpenAPI_nf_profile_H_

#include <string.h>
#include "../external/cJSON.h"
#include "../include/list.h"
#include "../include/keyValuePair.h"
#include "../include/binary.h"
#include "amf_info.h"
#include "ausf_info.h"
#include "bsf_info.h"
#include "chf_info.h"
#include "default_notification_subscription.h"
#include "ext_snssai.h"
#include "gmlc_info.h"
#include "hss_info.h"
#include "lmf_info.h"
#include "nef_info.h"
#include "nf_service.h"
#include "nf_status.h"
#include "nf_type.h"
#include "nrf_info.h"
#include "nwdaf_info.h"
#include "object.h"
#include "pcf_info.h"
#include "pcscf_info.h"
#include "plmn_id.h"
#include "plmn_id_nid.h"
#include "plmn_snssai.h"
#include "scp_info.h"
#include "smf_info.h"
#include "udm_info.h"
#include "udr_info.h"
#include "udsf_info.h"
#include "upf_info.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct OpenAPI_nf_profile_s OpenAPI_nf_profile_t;
typedef struct OpenAPI_nf_profile_s {
    char *nf_instance_id;
    char *nf_instance_name;
    OpenAPI_nf_type_e nf_type;
    OpenAPI_nf_status_e nf_status;
    bool is_heart_beat_timer;
    int heart_beat_timer;
    OpenAPI_list_t *plmn_list;
    OpenAPI_list_t *snpn_list;
    OpenAPI_list_t *s_nssais;
    OpenAPI_list_t *per_plmn_snssai_list;
    OpenAPI_list_t *nsi_list;
    char *fqdn;
    char *inter_plmn_fqdn;
    OpenAPI_list_t *ipv4_addresses;
    OpenAPI_list_t *ipv6_addresses;
    OpenAPI_list_t *allowed_plmns;
    OpenAPI_list_t *allowed_snpns;
    OpenAPI_list_t *allowed_nf_types;
    OpenAPI_list_t *allowed_nf_domains;
    OpenAPI_list_t *allowed_nssais;
    bool is_priority;
    int priority;
    bool is_capacity;
    int capacity;
    bool is_load;
    int load;
    char *load_time_stamp;
    char *locality;
    struct OpenAPI_udr_info_s *udr_info;
    OpenAPI_list_t* udr_info_list;
    struct OpenAPI_udm_info_s *udm_info;
    OpenAPI_list_t* udm_info_list;
    struct OpenAPI_ausf_info_s *ausf_info;
    OpenAPI_list_t* ausf_info_list;
    struct OpenAPI_amf_info_s *amf_info;
    OpenAPI_list_t* amf_info_list;
    struct OpenAPI_smf_info_s *smf_info;
    OpenAPI_list_t* smf_info_list;
    struct OpenAPI_upf_info_s *upf_info;
    OpenAPI_list_t* upf_info_list;
    struct OpenAPI_pcf_info_s *pcf_info;
    OpenAPI_list_t* pcf_info_list;
    struct OpenAPI_bsf_info_s *bsf_info;
    OpenAPI_list_t* bsf_info_list;
    struct OpenAPI_chf_info_s *chf_info;
    OpenAPI_list_t* chf_info_list;
    struct OpenAPI_nef_info_s *nef_info;
    struct OpenAPI_nrf_info_s *nrf_info;
    struct OpenAPI_udsf_info_s *udsf_info;
    OpenAPI_list_t* udsf_info_list;
    struct OpenAPI_nwdaf_info_s *nwdaf_info;
    OpenAPI_list_t* pcscf_info_list;
    OpenAPI_list_t* hss_info_list;
    OpenAPI_object_t *custom_info;
    char *recovery_time;
    bool is_nf_service_persistence;
    int nf_service_persistence;
    OpenAPI_list_t *nf_services;
    OpenAPI_list_t* nf_service_list;
    bool is_nf_profile_changes_support_ind;
    int nf_profile_changes_support_ind;
    bool is_nf_profile_changes_ind;
    int nf_profile_changes_ind;
    OpenAPI_list_t *default_notification_subscriptions;
    struct OpenAPI_lmf_info_s *lmf_info;
    struct OpenAPI_gmlc_info_s *gmlc_info;
    OpenAPI_list_t *nf_set_id_list;
    OpenAPI_list_t *serving_scope;
    bool is_lc_h_support_ind;
    int lc_h_support_ind;
    bool is_olc_h_support_ind;
    int olc_h_support_ind;
    OpenAPI_list_t* nf_set_recovery_time_list;
    OpenAPI_list_t* service_set_recovery_time_list;
    OpenAPI_list_t *scp_domains;
    struct OpenAPI_scp_info_s *scp_info;
} OpenAPI_nf_profile_t;

OpenAPI_nf_profile_t *OpenAPI_nf_profile_create(
    char *nf_instance_id,
    char *nf_instance_name,
    OpenAPI_nf_type_e nf_type,
    OpenAPI_nf_status_e nf_status,
    bool is_heart_beat_timer,
    int heart_beat_timer,
    OpenAPI_list_t *plmn_list,
    OpenAPI_list_t *snpn_list,
    OpenAPI_list_t *s_nssais,
    OpenAPI_list_t *per_plmn_snssai_list,
    OpenAPI_list_t *nsi_list,
    char *fqdn,
    char *inter_plmn_fqdn,
    OpenAPI_list_t *ipv4_addresses,
    OpenAPI_list_t *ipv6_addresses,
    OpenAPI_list_t *allowed_plmns,
    OpenAPI_list_t *allowed_snpns,
    OpenAPI_list_t *allowed_nf_types,
    OpenAPI_list_t *allowed_nf_domains,
    OpenAPI_list_t *allowed_nssais,
    bool is_priority,
    int priority,
    bool is_capacity,
    int capacity,
    bool is_load,
    int load,
    char *load_time_stamp,
    char *locality,
    OpenAPI_udr_info_t *udr_info,
    OpenAPI_list_t* udr_info_list,
    OpenAPI_udm_info_t *udm_info,
    OpenAPI_list_t* udm_info_list,
    OpenAPI_ausf_info_t *ausf_info,
    OpenAPI_list_t* ausf_info_list,
    OpenAPI_amf_info_t *amf_info,
    OpenAPI_list_t* amf_info_list,
    OpenAPI_smf_info_t *smf_info,
    OpenAPI_list_t* smf_info_list,
    OpenAPI_upf_info_t *upf_info,
    OpenAPI_list_t* upf_info_list,
    OpenAPI_pcf_info_t *pcf_info,
    OpenAPI_list_t* pcf_info_list,
    OpenAPI_bsf_info_t *bsf_info,
    OpenAPI_list_t* bsf_info_list,
    OpenAPI_chf_info_t *chf_info,
    OpenAPI_list_t* chf_info_list,
    OpenAPI_nef_info_t *nef_info,
    OpenAPI_nrf_info_t *nrf_info,
    OpenAPI_udsf_info_t *udsf_info,
    OpenAPI_list_t* udsf_info_list,
    OpenAPI_nwdaf_info_t *nwdaf_info,
    OpenAPI_list_t* pcscf_info_list,
    OpenAPI_list_t* hss_info_list,
    OpenAPI_object_t *custom_info,
    char *recovery_time,
    bool is_nf_service_persistence,
    int nf_service_persistence,
    OpenAPI_list_t *nf_services,
    OpenAPI_list_t* nf_service_list,
    bool is_nf_profile_changes_support_ind,
    int nf_profile_changes_support_ind,
    bool is_nf_profile_changes_ind,
    int nf_profile_changes_ind,
    OpenAPI_list_t *default_notification_subscriptions,
    OpenAPI_lmf_info_t *lmf_info,
    OpenAPI_gmlc_info_t *gmlc_info,
    OpenAPI_list_t *nf_set_id_list,
    OpenAPI_list_t *serving_scope,
    bool is_lc_h_support_ind,
    int lc_h_support_ind,
    bool is_olc_h_support_ind,
    int olc_h_support_ind,
    OpenAPI_list_t* nf_set_recovery_time_list,
    OpenAPI_list_t* service_set_recovery_time_list,
    OpenAPI_list_t *scp_domains,
    OpenAPI_scp_info_t *scp_info
);
void OpenAPI_nf_profile_free(OpenAPI_nf_profile_t *nf_profile);
OpenAPI_nf_profile_t *OpenAPI_nf_profile_parseFromJSON(cJSON *nf_profileJSON);
cJSON *OpenAPI_nf_profile_convertToJSON(OpenAPI_nf_profile_t *nf_profile);
OpenAPI_nf_profile_t *OpenAPI_nf_profile_copy(OpenAPI_nf_profile_t *dst, OpenAPI_nf_profile_t *src);

#ifdef __cplusplus
}
#endif

#endif /* _OpenAPI_nf_profile_H_ */

