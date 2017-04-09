#ifndef __HSS_CONTEXT_H__
#define __HSS_CONTEXT_H__

#include "core_list.h"
#include "core_errno.h"
#include "3gpp_common.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define HSS_KEY_LEN                 16
#define HSS_AMF_LEN                 2

typedef struct _hss_ue_t {
    lnode_t         node; /**< A node of list_t */

    c_uint8_t       imsi_bcd[MAX_IMSI_BCD_LEN+1];
    int             imsi_bcd_len;

    c_uint8_t       msisdn_bcd[MAX_MSISDN_BCD_LEN+1];
    int             msisdn_bcd_len;

    c_uint8_t       mei_bcd[MAX_MEI_BCD_LEN+1];
    int             mei_bcd_len;

    plmn_id_t       visited_plmn_id;

    c_uint8_t       k[HSS_KEY_LEN];
    c_uint64_t      sqn;
    c_uint8_t       rand[RAND_LEN];
    c_uint8_t       opc[HSS_KEY_LEN];
    c_uint8_t       op[HSS_KEY_LEN];
    c_uint8_t       amf[HSS_AMF_LEN];
} hss_ue_t;

typedef struct _hss_context_t {
    c_uint8_t       op[HSS_KEY_LEN];
    c_uint8_t       amf[HSS_AMF_LEN];

    list_t          ue_list;
} hss_context_t;

CORE_DECLARE(status_t)      hss_context_init(void);
CORE_DECLARE(void)          hss_context_final(void);
CORE_DECLARE(hss_context_t*) hss_self(void);

CORE_DECLARE(hss_ue_t*)     hss_ue_add(void);
CORE_DECLARE(status_t)      hss_ue_remove(hss_ue_t *ue);
CORE_DECLARE(status_t)      hss_ue_remove_all(void);
CORE_DECLARE(hss_ue_t*)     hss_ue_find_by_imsi_bcd(
                                c_uint8_t *imsi_bcd, c_uint8_t imsi_bcd_len);
CORE_DECLARE(hss_ue_t*)     hss_ue_first(void);
CORE_DECLARE(hss_ue_t*)     hss_ue_next(hss_ue_t *ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HSS_CONTEXT_H__ */
