#ifndef __HSS_KDF_H__
#define __HSS_KDF_H__

#include "core.h"

#define HSS_SQN_LEN 6
#define HSS_AK_LEN 6

CORE_DECLARE(void) hss_auc_kasme(const c_uint8_t *ck, const c_uint8_t *ik, 
        const c_uint8_t plmn_id[3], const c_uint8_t *sqn,  const c_uint8_t *ak,
        c_uint8_t *kasme);

CORE_DECLARE(void) hss_auc_sqn(
    const c_uint8_t *opc, const c_uint8_t *k, const c_uint8_t *auts,
    c_uint8_t *sqn_ms, c_uint8_t *mac_s);

#endif /* __HSS_KDF_H__ */
