/*---------------------------------------------------------
 *					Kasumi.h
 *---------------------------------------------------------*/

#if !defined(OGS_CRYPT_INSIDE) && !defined(OGS_CRYPT_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef KASUMI_H
#define KASUMI_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t u8;
typedef uint16_t  u16;
/* original reference is using long, which is 64 bits on 64 bits system
   I changed to int to make it work on x86 32 / 64 bits system
typedef unsigned  long  u32;
*/
typedef uint32_t u32;


/*------- unions: used to remove "endian" issues ------------------------*/

typedef union {
	u32 b32;
	u16 b16[2];
	u8  b8[4];
} REGISTER32; /* is redefining DWORD */

typedef union {
	u16 b16;
	u8  b8[2];
} REGISTER16; /* is redefining WORD */

/*----- a 64-bit structure to help with endian issues -----*/

typedef union {
	u32 b32[2];
	u16 b16[4];
	u8  b8[8];
} REGISTER64;

/*------------- prototypes --------------------------------
 * take care: length (in f8 and f9) is always in bits
 *---------------------------------------------------------*/

/* initialize the 128 bits key into the cipher */
void kasumi_key_schedule( u8 *key );

/* cipher a block of 64 bits */
void kasumi( u8 *data );

/* cipher a whole message in 3GPP -counter- mode */
void kasumi_f8( u8 *key, u32 count, u32 bearer, u32 dir, u8 *data, int length );

/* compute a 3GPP MAC on a message */
u8* kasumi_f9( u8 *key, u32 count, u32 fresh, u32 dir, u8 *data, int length );

#ifdef __cplusplus
}
#endif

#endif /* KASUMI_H */
