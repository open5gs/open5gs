#ifndef __HSS_CONTEXT_H__
#define __HSS_CONTEXT_H__

#include "core_list.h"
#include "core_errno.h"
#include "3gpp_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define HSS_MAX_PROFILE_NAME_LEN    255

#define HSS_KEY_LEN                 16
#define HSS_AMF_LEN                 2

typedef c_uint32_t hss_pdn_id_t;
typedef c_uint32_t hss_profile_id_t;

typedef struct _hss_pdn_t {
    lnode_t         node; /**< A node of list_t */

    hss_pdn_id_t    id;
    c_int8_t        apn[MAX_APN_LEN];
#define HSS_PDN_TYPE_IPV4                                   0
#define HSS_PDN_TYPE_IPV6                                   1
#define HSS_PDN_TYPE_IPV4_AND_IPV6                          2
#define HSS_PDN_TYPE_IPV4_OR_IPV6                           3
    c_int8_t        type;

    c_uint32_t      max_bandwidth_ul; /* Kbps */
    c_uint32_t      max_bandwidth_dl; /* Kbps */

#define HSS_PDN_QCI_1                                       1
#define HSS_PDN_QCI_2                                       2
#define HSS_PDN_QCI_3                                       3
#define HSS_PDN_QCI_4                                       4
#define HSS_PDN_QCI_5                                       5
#define HSS_PDN_QCI_6                                       6
#define HSS_PDN_QCI_7                                       7
#define HSS_PDN_QCI_8                                       8
#define HSS_PDN_QCI_9                                       9
#define HSS_PDN_QCI_65                                      65
#define HSS_PDN_QCI_66                                      66
#define HSS_PDN_QCI_69                                      69
#define HSS_PDN_QCI_70                                      70
    c_uint8_t       qci;

    /* Values 1 to 8 should only be assigned for services that are authorized 
     * to receive prioritized treatment within an operator domain. 
     * Values 9 to 15 may be assigned to resources that are authorized 
     * by the home network and thus applicable when a UE is roaming. */
    c_uint8_t       priority_level;

#define HSS_PRE_EMPTION_CAPABILITY_ENABLED                  0
#define HSS_PRE_EMPTION_CAPABILITY_DISABLED                 1
    c_uint8_t       pre_emption_capability;
#define HSS_PRE_EMPTION_VULNERABILITY_ENABLED               0
#define HSS_PRE_EMPTION_VULNERABILITY_DISABLED              1
    c_uint8_t       pre_emption_vulnerability;
} hss_pdn_t;

typedef struct _hss_profile_t {
    lnode_t         node; /**< A node of list_t */

    hss_profile_id_t id;
    c_int8_t        name[HSS_MAX_PROFILE_NAME_LEN];

    /* Security Context */
    c_uint8_t       k[HSS_KEY_LEN];
    c_uint8_t       op[HSS_KEY_LEN];
    c_uint8_t       amf[HSS_AMF_LEN];
    c_uint64_t      sqn;

    c_uint32_t      subscriber_status;
    c_uint32_t      network_access_mode;

    c_uint32_t      max_bandwidth_ul; /* Kbps */
    c_uint32_t      max_bandwidth_dl; /* Kbps */

    c_uint32_t      subscribed_rau_tau_timer; /* minutes */
} hss_profile_t;

typedef struct _hss_ue_t {
    lnode_t         node; /**< A node of list_t */

    /* UE Identitiy */
    c_int8_t        imsi_bcd[MAX_IMSI_BCD_LEN+1];
    c_uint8_t       msisdn[MAX_MSISDN_LEN];
    int             msisdn_len;

    plmn_id_t       visited_plmn_id;

#define HSS_SUBSCRIBER_STATUS_SERVICE_GRANTED               0
#define HSS_SUBSCRIBER_STATUS_OPERATOR_DETERMINED_BARRING   1 
    c_uint32_t      subscriber_status;
#define HSS_NETWORK_ACCESS_MODE_PACKET_AND_CIRCUIT          0
#define HSS_NETWORK_ACCESS_MODE_RESERVED                    1
#define HSS_NETWORK_ACCESS_MODE_ONLY_PACKET                 2
    c_uint32_t      network_access_mode;
    c_uint32_t      max_bandwidth_ul; /* Kbps */
    c_uint32_t      max_bandwidth_dl; /* Kbps */

    c_uint32_t      subscribed_rau_tau_timer; /* minutes */

    hss_pdn_t       *pdn[MAX_NUM_OF_PDN];
    int             num_of_pdn;

    /* Security Context */
    c_uint8_t       k[HSS_KEY_LEN];
    c_uint64_t      sqn;
    c_uint8_t       rand[RAND_LEN];
    c_uint8_t       opc[HSS_KEY_LEN];
    c_uint8_t       op[HSS_KEY_LEN];
    c_uint8_t       amf[HSS_AMF_LEN];

} hss_ue_t;

typedef struct _hss_context_t {
    c_uint32_t      pdn_id;

    list_t          profile_list;
    list_t          pdn_list;
    list_t          ue_list;
} hss_context_t;

CORE_DECLARE(status_t)      hss_context_init(void);
CORE_DECLARE(void)          hss_context_final(void);
CORE_DECLARE(hss_context_t*) hss_self(void);

CORE_DECLARE(hss_pdn_t*)    hss_pdn_add();
CORE_DECLARE(status_t)      hss_pdn_remove(hss_pdn_t *pdn);
CORE_DECLARE(status_t)      hss_pdn_remove_all(void);
CORE_DECLARE(hss_pdn_t*)    hss_pdn_find_by_id(hss_pdn_id_t id);
CORE_DECLARE(hss_pdn_t*)    hss_pdn_first(void);
CORE_DECLARE(hss_pdn_t*)    hss_pdn_next(hss_pdn_t *pdn);

CORE_DECLARE(hss_profile_t*) hss_profile_add(hss_profile_id_t id);
CORE_DECLARE(status_t)      hss_profile_remove(hss_profile_t *profile);
CORE_DECLARE(status_t)      hss_profile_remove_all(void);
CORE_DECLARE(hss_profile_t*) hss_profile_find_by_id(hss_profile_id_t id);
CORE_DECLARE(hss_profile_t*) hss_profile_find_by_name(c_int8_t *name);
CORE_DECLARE(hss_profile_t*) hss_profile_first(void);
CORE_DECLARE(hss_profile_t*) hss_profile_next(hss_profile_t *profile);

CORE_DECLARE(hss_ue_t*)     hss_ue_add(
                                hss_profile_id_t id, c_int8_t *imsi_bcd);
CORE_DECLARE(status_t)      hss_ue_remove(hss_ue_t *ue);
CORE_DECLARE(status_t)      hss_ue_remove_all(void);
CORE_DECLARE(hss_ue_t*)     hss_ue_find_by_imsi_bcd(c_int8_t *imsi_bcd);
CORE_DECLARE(hss_ue_t*)     hss_ue_first(void);
CORE_DECLARE(hss_ue_t*)     hss_ue_next(hss_ue_t *ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HSS_CONTEXT_H__ */
