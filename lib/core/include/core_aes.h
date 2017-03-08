#ifndef _CORE_AES_H__
#define _CORE_AES_H__

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define AES_BLOCK_SIZE 16
#define MAX_KEY_BITS       256

#define KEYLENGTH(keybits) ((keybits)/8)
#define RKLENGTH(keybits)  ((keybits)/8+28)
#define NROUNDS(keybits)   ((keybits)/32+6)

CORE_DECLARE(int) aes_setup_enc(c_uint32_t *rk, const c_uint8_t *key,
        int keybits);
CORE_DECLARE(int) aes_setup_dec(c_uint32_t *rk, const c_uint8_t *key,
        int keybits);

CORE_DECLARE(void) aes_encrypt(const c_uint32_t *rk, int nrounds,
        const c_uint8_t plaintext[16], c_uint8_t ciphertext[16]);
CORE_DECLARE(void) aes_decrypt(const c_uint32_t *rk, int nrounds,
        const c_uint8_t ciphertext[16], c_uint8_t plaintext[16]);

CORE_DECLARE(status_t) aes_cbc_encrypt(const c_uint8_t *key,
        const c_uint32_t keybits, c_uint8_t *ivec,
        const c_uint8_t *in, const c_uint32_t inlen,
        c_uint8_t *out, c_uint32_t *outlen);
CORE_DECLARE(status_t) aes_cbc_decrypt(const c_uint8_t *key,
        const c_uint32_t keybits, c_uint8_t *ivec,
        const c_uint8_t *in, const c_uint32_t inlen,
        c_uint8_t *out, c_uint32_t *outlen);

CORE_DECLARE(status_t) aes_ctr128_encrypt(const c_uint8_t *key,
        c_uint8_t *ivec, const c_uint8_t *in, const c_uint32_t inlen,
        c_uint8_t *out);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif

