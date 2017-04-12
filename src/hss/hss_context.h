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

typedef c_uint32_t hss_profile_id_t;
typedef struct _hss_profile_t {
    lnode_t         node; /**< A node of list_t */

    hss_profile_id_t id;
    c_int8_t        name[HSS_MAX_PROFILE_NAME_LEN];

    /* Security Context */
    c_uint8_t       k[HSS_KEY_LEN];
    c_uint8_t       op[HSS_KEY_LEN];
    c_uint8_t       amf[HSS_AMF_LEN];
    c_uint64_t      sqn;

    c_uint32_t      access_restriction_data;
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

#define HSS_ACCESS_RESTRICTION_UTRAN_NOT_ALLOWED                    (1)
#define HSS_ACCESS_RESTRICTION_GERAN_NOT_ALLOWED                    (1<<1)
#define HSS_ACCESS_RESTRICTION_GAN_NOT_ALLOWED                      (1<<2)
#define HSS_ACCESS_RESTRICTION_I_HSPA_EVOLUTION_NOT_ALLOWED         (1<<3)
#define HSS_ACCESS_RESTRICTION_WB_E_UTRAN_NOT_ALLOWED               (1<<4)
#define HSS_ACCESS_RESTRICTION_HO_TO_NON_3GPP_ACCESS_NOT_ALLOWED    (1<<5)
#define HSS_ACCESS_RESTRICTION_NB_IOT_NOT_ALLOWED                   (1<<6)
    c_uint32_t      access_restriction_data;

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

    pdn_t           *pdn[MAX_NUM_OF_PDN];
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

CORE_DECLARE(pdn_t*)        hss_pdn_add(c_int8_t *apn);
CORE_DECLARE(status_t)      hss_pdn_remove(pdn_t *pdn);
CORE_DECLARE(status_t)      hss_pdn_remove_all(void);
CORE_DECLARE(pdn_t*)        hss_pdn_find_by_apn(c_int8_t *apn);
CORE_DECLARE(pdn_t*)        hss_pdn_first(void);
CORE_DECLARE(pdn_t*)        hss_pdn_next(pdn_t *pdn);

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
