/*
 *  sha1.h
 *
 *  Copyright (C) 1998, 2009
 *  Paul E. Jones <paulej@packetizer.com>
 *  All Rights Reserved
 *
 *****************************************************************************
 *  $Id: sha1.h 12 2009-06-22 19:34:25Z paulej $
 *****************************************************************************
 *
 *  Description:
 *      This class implements the Secure Hashing Standard as defined
 *      in FIPS PUB 180-1 published April 17, 1995.
 *
 *      Many of the variable names in the sha1_ctx, especially the
 *      single character names, were used because those were the names
 *      used in the publication.
 *
 *      Please read the file sha1.c for more information.
 *
 */

#ifndef _CORE_SHA1_H_
#define _CORE_SHA1_H_

#include "core.h"

#define SHA1_DIGEST_SIZE (160 / 8)
#define SHA1_BLOCK_SIZE  (512 / 8)

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct sha1_ctx
{
    unsigned Message_Digest[5]; /* Message Digest (output)          */

    unsigned Length_Low;        /* Message length in bits           */
    unsigned Length_High;       /* Message length in bits           */

    unsigned char Message_Block[64]; /* 512-bit message blocks      */
    int Message_Block_Index;    /* Index into message block array   */

    int Computed;               /* Is the digest computed?          */
    int Corrupted;              /* Is the message digest corruped?  */
} sha1_ctx;

CORE_DECLARE(void) sha1_init(sha1_ctx *ctx);
CORE_DECLARE(void) sha1_update(sha1_ctx *ctx, const c_uint8_t *message,
        c_uint32_t len);
CORE_DECLARE(void) sha1_final(sha1_ctx *ctx, c_uint8_t *digest);
CORE_DECLARE(void) sha1(const c_uint8_t *message, c_uint32_t len,
        c_uint8_t *digest);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CORE_SHA1_H_ */
