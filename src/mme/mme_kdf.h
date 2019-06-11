#ifndef __MME_KDF_H__
#define __MME_KDF_H__

#include "nas/nas-ies.h"

/* Algorithm Type Distinguishers */
#define MME_KDF_NAS_ENC_ALG 0x01
#define MME_KDF_NAS_INT_ALG 0x02

/* Algorithm Identity defined in nas_ies.h 
#define NAS_SECURITY_ALGORITHMS_EIA0_EEA0 0
#define NAS_SECURITY_ALGORITHMS_128_EIA1_EEA1 1
#define NAS_SECURITY_ALGORITHMS_128_EIA1_EEA2 2
#define NAS_SECURITY_ALGORITHMS_128_EIA3_EEA3 3
*/

void mme_kdf_nas(uint8_t algorithm_type_distinguishers,
    uint8_t algorithm_identity, uint8_t *kasme, uint8_t *knas);

void mme_kdf_enb(uint8_t *kasme, uint32_t ul_count, uint8_t *kenb);

void mme_kdf_nh(uint8_t *kasme, uint8_t *sync_input, uint8_t *kenb);

#endif /* __MME_KDF_H__ */
