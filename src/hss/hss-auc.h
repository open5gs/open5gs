#ifndef __HSS_KDF_H__
#define __HSS_KDF_H__

#include "ogs-core.h"

#define HSS_SQN_LEN 6
#define HSS_AK_LEN 6

void hss_auc_kasme(const uint8_t *ck, const uint8_t *ik, 
        const uint8_t plmn_id[3], const uint8_t *sqn,  const uint8_t *ak,
        uint8_t *kasme);

void hss_auc_sqn(
    const uint8_t *opc, const uint8_t *k, const uint8_t *auts,
    uint8_t *sqn_ms, uint8_t *mac_s);

#endif /* __HSS_KDF_H__ */
