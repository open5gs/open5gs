#ifndef __HSS_CTX_H__
#define __HSS_CTX_H__

#include "core_list.h"
#include "core_errno.h"
#include "3gpp_defs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define SIZE_OF_UE_POOL             128

#define HSS_KEY_LEN                 16
#define HSS_AMF_LEN                 2

typedef struct _ue_ctx_t {
    lnode_t         node; /**< A node of list_t */

    c_uint8_t       imsi[MAX_IMSI_LEN+1];
    c_uint8_t       imsi_len;

    c_uint8_t       k[HSS_KEY_LEN];
    c_uint64_t      sqn;
    c_uint8_t       rand[RAND_LEN];
    c_uint8_t       opc[HSS_KEY_LEN];
    c_uint8_t       op[HSS_KEY_LEN];
    c_uint8_t       amf[HSS_AMF_LEN];
} ue_ctx_t;

typedef struct _hss_ctx_t {
    c_uint8_t       op[HSS_KEY_LEN];
    c_uint8_t       amf[HSS_AMF_LEN];
} hss_ctx_t;

CORE_DECLARE(hss_ctx_t*)    hss_self(void);
#define self() hss_self()

CORE_DECLARE(status_t)      hss_ctx_init(void);
CORE_DECLARE(void)          hss_ctx_final(void);

CORE_DECLARE(ue_ctx_t*)     hss_ctx_ue_add(void);
CORE_DECLARE(status_t)      hss_ctx_ue_remove(ue_ctx_t *ue);
CORE_DECLARE(status_t)      hss_ctx_ue_remove_all(void);
CORE_DECLARE(ue_ctx_t*)     hss_ctx_ue_find_by_imsi(
                                c_uint8_t *imsi, c_uint8_t imsi_len);
CORE_DECLARE(ue_ctx_t*)     hss_ctx_ue_first(void);
CORE_DECLARE(ue_ctx_t*)     hss_ctx_ue_next(ue_ctx_t *ue);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HSS_CTX_H__ */
