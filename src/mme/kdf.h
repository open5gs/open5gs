#ifndef __MME_KDF_H__
#define __MME_KDF_H__

#include "core.h"

#include "nas_ies.h"

/* Algorithm Type Distinguishers */
#define MME_KDF_NAS_ENC_ALG 0x01
#define MME_KDF_NAS_INT_ALG 0x02

/* Algorithm Identity defined in nas_ies.h 
#define NAS_SECURITY_ALGORITHMS_EIA0_EEA0 0
#define NAS_SECURITY_ALGORITHMS_128_EIA1_EEA1 1
#define NAS_SECURITY_ALGORITHMS_128_EIA1_EEA2 2
#define NAS_SECURITY_ALGORITHMS_128_EIA3_EEA3 3
*/

CORE_DECLARE(void) mme_kdf_nas(c_uint8_t algorithm_type_distinguishers,
    c_uint8_t algorithm_identity, c_uint8_t *kasme, c_uint8_t *knas);

#endif /* __MME_KDF_H__ */
