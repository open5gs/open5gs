/*********************************************************************************/
/* freeDiameter author note:
 *  The content from this file comes directly from the hostap project.
 * It is redistributed under the terms of the BSD license, as allowed
 * by the original copyright reproduced below.
 *  The file has not been modified, except for this notice.
 */
/*********************************************************************************/

/*
 * MD5 hash implementation and interface functions
 * Copyright (c) 2003-2005, Jouni Malinen <j@w1.fi>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * Alternatively, this software may be distributed under the terms of BSD
 * license.
 *
 * See README and COPYING for more details.
 */

#ifndef MD5_H
#define MD5_H

#define MD5_MAC_LEN 16

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;
typedef int64_t s64;
typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

#define HASHLEN 16
typedef char HASH[HASHLEN];
#define HASHHEXLEN 32
typedef char HASHHEX[HASHHEXLEN+1];
#define IN
#define OUT

struct MD5Context {
	u32 buf[4];
	u32 bits[2];
	u8 in[64];
};
typedef struct MD5Context MD5_CTX;
#define os_memcpy(d, s, n) memcpy((d), (s), (n))
#define os_memset(s, c, n) memset(s, c, n)


void MD5Init(struct MD5Context *ctx);
void MD5Update(struct MD5Context *ctx, unsigned char const *buf, unsigned len);
void MD5Final(unsigned char digest[16], struct MD5Context *ctx);

#endif /* MD5_H */
