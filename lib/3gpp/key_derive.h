#ifndef __3GPP_KDF_H__
#define __3GPP_KDF_H__

#include "core.h"

void key_derive_kasme(const c_uint8_t *ck, const c_uint8_t *ik, 
        const c_uint8_t plmn_id[3], const c_uint8_t *sqn,  const c_uint8_t *ak,
        c_uint8_t *kasme);

#endif /* __3GPP_KDF_H__ */
