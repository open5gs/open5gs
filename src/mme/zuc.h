/*------------------------------------------------------------------------
 * ZUC.h
 * Code taken from the ZUC specification
 * available on the GSMA website
 *------------------------------------------------------------------------*/

#ifndef __ZUC_H__
#define __ZUC_H__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* type definition from */
typedef c_uint8_t u8;
typedef c_uint32_t u32;

/*
 * ZUC keystream generator
 * k: secret key (input, 16 bytes)
 * iv: initialization vector (input, 16 bytes)
 * Keystream: produced keystream (output, variable length)
 * KeystreamLen: length in bits requested for the keystream (input)
*/
CORE_DECLARE(void) zuc_initialize(u8* k, u8* iv);
CORE_DECLARE(void) zuc_generate_key_stream(u32* pKeystream, u32 KeystreamLen);

/*
 * CK: ciphering key
 * COUNT: frame counter
 * BEARER: radio bearer
 * DIRECTION
 * LENGTH: length of the frame in bits
 * M: original message (input)
 * C: processed message (output)
*/
CORE_DECLARE(void) zuc_eea3(u8* CK, u32 COUNT, u32 BEARER, u32 DIRECTION, 
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
CORE_DECLARE(void) zuc_eia3(u8* IK, u32 COUNT, u32 BEARER, u32 DIRECTION,
		           u32 LENGTH, u8* M, u32* MAC);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ZUC_H__ */
