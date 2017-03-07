#ifndef __SNOW_3G__
#define __SNOW_3G__

#include "core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef c_uint8_t u8;
typedef c_uint32_t u32;
typedef c_uint64_t u64;

/* Initialization.
* Input k[4]: Four 32-bit words making up 128-bit key.
* Input IV[4]: Four 32-bit words making 128-bit initialization variable.
* Output: All the LFSRs and FSM are initialized for key generation.
* See Section 4.1.
*/

CORE_DECLARE(void) snow_3g_initialize(u32 k[4], u32 IV[4]);

/* Generation of Keystream.
* input n: number of 32-bit words of keystream.
* input z: space for the generated keystream, assumes
* memory is allocated already.
* output: generated keystream which is filled in z
* See section 4.2.
*/

CORE_DECLARE(void) snow_3g_generate_key_stream(u32 n, u32 *z);

/* f8.
* Input key: 128 bit Confidentiality Key.
* Input count:32-bit Count, Frame dependent input.
* Input bearer: 5-bit Bearer identity (in the LSB side).
* Input dir:1 bit, direction of transmission.
* Input data: length number of bits, input bit stream.
* Input length: 32 bit Length, i.e., the number of bits to be encrypted or
* decrypted.
* Output data: Output bit stream. Assumes data is suitably memory
* allocated.
* Encrypts/decrypts blocks of data between 1 and 2^32 bits in length as
* defined in Section 3.
*/

CORE_DECLARE(void) snow_3g_f8( u8 *key, u32 count, u32 bearer, u32 dir,
                  u8 *data, u32 length );

/* f9.
* Input key: 128 bit Integrity Key.
* Input count:32-bit Count, Frame dependent input.
* Input fresh: 32-bit Random number.
* Input dir:1 bit, direction of transmission (in the LSB).
* Input data: length number of bits, input bit stream.
* Input length: 64 bit Length, i.e., the number of bits to be MAC'd.
* Output : 32 bit block used as MAC
* Generates 32-bit MAC using UIA2 algorithm as defined in Section 4.
*/

CORE_DECLARE(void) snow_3g_f9( u8* key, u32 count, u32 fresh, u32 dir,
                 u8 *data, u64 length, u8 *out);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __SNOW_3G__ */
