#ifndef __MME_KDF_H__
#define __MME_KDF_H__

#include "core.h"

#include "nas_ies.h"

#define MME_KDF_NAS_ENC_ALG 0x01
#define MME_KDF_NAS_INT_ALG 0x02

void mme_kdf_nas(c_uint8_t algorithm_type_distinguishers,
    c_uint8_t algorithm_identity, c_uint8_t *kasme, c_uint8_t *knas);

#endif /* __MME_KDF_H__ */
