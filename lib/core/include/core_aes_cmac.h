#ifndef _CORE_AES_CMAC_H
#define _CORE_AES_CMAC_H

#include "core_aes.h"

#define AES_CMAC_DIGEST_LEN 16

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Caculate CMAC value
 *
 * @param cmac
 * @param key
 * @param msg
 * @param len
 *
 * @return CORE_OK
 *         CORE_ERROR
 */
CORE_DECLARE(status_t) aes_cmac_calculate(c_uint8_t *cmac, const c_uint8_t *key,
        const c_uint8_t *msg, const c_uint32_t len);

/**
 * Verify CMAC value
 *
 * @param cmac
 * @param key
 * @param msg
 * @param len
 *
 * @return CORE_OK
 *         CORE_ERROR
 *         ERR_INVALID_CMAC
 */
#define ERR_INVALID_CMAC -2
CORE_DECLARE(status_t) aes_cmac_verify(c_uint8_t *cmac, const c_uint8_t *key,
        const c_uint8_t *msg, const c_uint32_t len);

#ifdef __cplusplus
}
#endif

#endif /* ! _CORE_AES_CMAC_H */
