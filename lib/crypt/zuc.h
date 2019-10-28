/*------------------------------------------------------------------------
 * ZUC.h
 * Code taken from the ZUC specification
 * available on the GSMA website
 *------------------------------------------------------------------------*/

#if !defined(OGS_CRYPT_INSIDE) && !defined(OGS_CRYPT_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef ZUC_H
#define ZUC_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* type definition from */
typedef uint8_t u8;
typedef uint32_t u32;

/*
 * ZUC keystream generator
 * k: secret key (input, 16 bytes)
 * iv: initialization vector (input, 16 bytes)
 * Keystream: produced keystream (output, variable length)
 * KeystreamLen: length in bits requested for the keystream (input)
*/
void zuc_initialize(u8* k, u8* iv);
void zuc_generate_key_stream(u32* pKeystream, u32 KeystreamLen);

/*
 * CK: ciphering key
 * COUNT: frame counter
 * BEARER: radio bearer
 * DIRECTION
 * LENGTH: length of the frame in bits
 * M: original message (input)
 * C: processed message (output)
*/
void zuc_eea3(u8* CK, u32 COUNT, u32 BEARER, u32 DIRECTION, 
		           u32 LENGTH, u8* M, u8* C);

/*
 * IK: integrity key
 * COUNT: frame counter
 * BEARER: radio bearer
 * DIRECTION
 * LENGTH: length of the frame in bits
 * M: original message (input)
 * C: processed message (output)
*/
void zuc_eia3(u8* IK, u32 COUNT, u32 BEARER, u32 DIRECTION,
		           u32 LENGTH, u8* M, u32* MAC);

#ifdef __cplusplus
}
#endif

#endif /* ZUC_H */
