#ifndef __HSS_CTX_H__
#define __HSS_CTX_H__

#include "core_list.h"
#include "core_errno.h"
#include "3gpp_defs.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define HSS_KEY_LEN                 16
#define HSS_AMF_LEN                 2

typedef struct _user_ctx_t {
    lnode_t         node; /**< A node of list_t */

    c_uint8_t       imsi[MAX_IMSI_LEN+1];
    c_uint8_t       imsi_len;

    c_uint8_t       k[HSS_KEY_LEN];
    c_uint64_t      sqn;
    c_uint8_t       rand[RAND_LEN];
    c_uint8_t       opc[HSS_KEY_LEN];
    c_uint8_t       op[HSS_KEY_LEN];
    c_uint8_t       amf[HSS_AMF_LEN];
} user_ctx_t;

typedef struct _hss_ctx_t {
    c_uint8_t       op[HSS_KEY_LEN];
    c_uint8_t       amf[HSS_AMF_LEN];
} hss_ctx_t;

CORE_DECLARE(hss_ctx_t*)    hss_self(void);
#define self() hss_self()

CORE_DECLARE(status_t)      hss_ctx_init(void);
CORE_DECLARE(void)          hss_ctx_final(void);

CORE_DECLARE(user_ctx_t*)   hss_ctx_user_add(void);
CORE_DECLARE(status_t)      hss_ctx_user_remove(user_ctx_t *user);
CORE_DECLARE(status_t)      hss_ctx_user_remove_all(void);
CORE_DECLARE(user_ctx_t*)   hss_ctx_user_find_by_imsi(
                                c_uint8_t *imsi, c_uint8_t imsi_len);
CORE_DECLARE(user_ctx_t*)   hss_ctx_user_first(void);
CORE_DECLARE(user_ctx_t*)   hss_ctx_user_next(user_ctx_t *user);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __HSS_CTX_H__ */
