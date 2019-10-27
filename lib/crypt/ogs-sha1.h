/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#if !defined(OGS_CRYPT_INSIDE) && !defined(OGS_CRYPT_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_SHA1_H
#define OGS_SHA1_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_SHA1_DIGEST_SIZE (160 / 8)
#define OGS_SHA1_BLOCK_SIZE  (512 / 8)

typedef struct ogs_sha1_ctx
{
    unsigned Message_Digest[5]; /* Message Digest (output)          */

    unsigned Length_Low;        /* Message length in bits           */
    unsigned Length_High;       /* Message length in bits           */

    unsigned char Message_Block[64]; /* 512-bit message blocks      */
    int Message_Block_Index;    /* Index into message block array   */

    int Computed;               /* Is the digest computed?          */
    int Corrupted;              /* Is the message digest corruped?  */
} ogs_sha1_ctx;

void ogs_sha1_init(ogs_sha1_ctx *ctx);
void ogs_sha1_update(ogs_sha1_ctx *ctx, const uint8_t *message, uint32_t len);
void ogs_sha1_final(ogs_sha1_ctx *ctx, uint8_t *digest);
void ogs_sha1(const uint8_t *message, uint32_t len, uint8_t *digest);

#ifdef __cplusplus
}
#endif

#endif /* OGS_SHA1_H */
