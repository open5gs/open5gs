/*
 * Copyright 2002-2020 The OpenSSL Project Authors. All Rights Reserved.
 *
 * Licensed under the Apache License 2.0 (the "License").  You may not use
 * this file except in compliance with the License.  You can obtain a copy
 * in the file LICENSE in the source distribution or at
 * https://www.openssl.org/source/license.html
 */

/*
 * Copyright (C) 2019-2020 by Sukchan Lee <acetcom@gmail.com>
 *
 * This file is part of Open5GS.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ogs-crypt.h"

#if (OGS_AES_BLOCK_SIZE != 16)
#error "Wrong AES block size"
#endif

/*  From RFC 4493

    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    +                    Algorithm Generate_Subkey                      +
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    +                                                                   +
    +   Input    : K (128-bit key)                                      +
    +   Output   : K1 (128-bit first subkey)                            +
    +              K2 (128-bit second subkey)                           +
    +-------------------------------------------------------------------+
    +                                                                   +
    +   Constants: const_Zero is 0x00000000000000000000000000000000     +
    +              const_Rb   is 0x00000000000000000000000000000087     +
    +   Variables: L          for output of AES-128 applied to 0^128    +
    +                                                                   +
    +   Step 1.  L := AES-128(K, const_Zero);                           +
    +   Step 2.  if MSB(L) is equal to 0                                +
    +            then    K1 := L << 1;                                  +
    +            else    K1 := (L << 1) XOR const_Rb;                   +
    +   Step 3.  if MSB(K1) is equal to 0                               +
    +            then    K2 := K1 << 1;                                 +
    +            else    K2 := (K1 << 1) XOR const_Rb;                  +
    +   Step 4.  return K1, K2;                                         +
    +                                                                   +
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

static int _generate_subkey(uint8_t *k1, uint8_t *k2,
        const uint8_t *key)
{
    uint8_t zero[16] = {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };
    uint8_t rb[16] = {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x87
    };
    uint8_t L[16];

    uint32_t rk[OGS_AES_RKLENGTH(OGS_AES_MAX_KEY_BITS)];
    int i, nrounds;

    /* Step 1.  L := AES-128(K, const_Zero) */
    nrounds = ogs_aes_setup_enc(rk, key, 128);
    ogs_aes_encrypt(rk, nrounds, zero, L);

    /* Step 2.  if MSB(L) is equal to 0 */
    if ((L[0] & 0x80) == 0)
    {
        /* then    k1 := L << 1; */
        for (i = 0; i < 15; i++)
            k1[i] = ((L[i] << 1) & 0xfe) | ((L[i + 1] & 0x80) ? 1 : 0);
        k1[15] = ((L[15] << 1) & 0xfe);
    }
    else
    {
        /* else    k1 := (L << 1) XOR const_Rb; */
        for (i = 0; i < 15; i++)
            k1[i] = (((L[i] << 1) & 0xfe) | ((L[i + 1] & 0x80) ? 1 : 0))
                ^ rb[i];
        k1[15] = ((L[15] << 1) & 0xfe) ^ rb[15];
    }

    /* Step 3.  if MSB(k1) is equal to 0 */
    if ((k1[0] & 0x80) == 0)
    {
        /* then    k2 := k2 << 1; */
        for (i = 0; i < 15; i++)
            k2[i] = ((k1[i] << 1) & 0xfe) | ((k1[i + 1] & 0x80) ? 1 : 0);
        k2[15] = ((k1[15] << 1) & 0xfe);
    }
    else
    {
        /* else    k2 := (k2 << 1) XOR const_Rb; */
        for (i = 0; i < 15; i++)
            k2[i] = (((k1[i] << 1) & 0xfe) | ((k1[i + 1] & 0x80) ? 1 : 0))
                ^ rb[i];
        k2[15] = ((k1[15] << 1) & 0xfe) ^ rb[15];
    }

    return OGS_OK; 
}

/*  From RFC 4493

    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    +                   Algorithm AES-CMAC                              +
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    +                                                                   +
    +   Input    : K    ( 128-bit key )                                 +
    +            : M    ( message to be authenticated )                 +
    +            : len  ( length of the message in octets )             +
    +   Output   : T    ( message authentication code )                 +
    +                                                                   +
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    +   Constants: const_Zero is 0x00000000000000000000000000000000     +
    +              const_Bsize is 16                                    +
    +                                                                   +
    +   Variables: K1, K2 for 128-bit subkeys                           +
    +              M_i is the i-th block (i=1..ceil(len/const_Bsize))   +
    +              M_last is the last block xor-ed with K1 or K2        +
    +              n      for number of blocks to be processed          +
    +              r      for number of octets of last block            +
    +              flag   for denoting if last block is complete or not +
    +                                                                   +
    +   Step 1.  (K1,K2) := Generate_Subkey(K);                         +
    +   Step 2.  n := ceil(len/const_Bsize);                            +
    +   Step 3.  if n = 0                                               +
    +            then                                                   +
    +                 n := 1;                                           +
    +                 flag := false;                                    +
    +            else                                                   +
    +                 if len mod const_Bsize is 0                       +
    +                 then flag := true;                                +
    +                 else flag := false;                               +
    +                                                                   +
    +   Step 4.  if flag is true                                        +
    +            then M_last := M_n XOR K1;                             +
    +            else M_last := padding(M_n) XOR K2;                    +
    +   Step 5.  X := const_Zero;                                       +
    +   Step 6.  for i := 1 to n-1 do                                   +
    +                begin                                              +
    +                  Y := X XOR M_i;                                  +
    +                  X := AES-128(K,Y);                               +
    +                end                                                +
    +            Y := M_last XOR X;                                     +
    +            T := AES-128(K,Y);                                     +
    +   Step 7.  return T;                                              +
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

int ogs_aes_cmac_calculate(uint8_t *cmac, const uint8_t *key,
        const uint8_t *msg, const uint32_t len)
{
    uint8_t x[16] = {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };
    uint8_t y[16], m_last[16];
    uint8_t k1[16], k2[16];
    int i, j, n, bs, flag;
    uint32_t rk[OGS_AES_RKLENGTH(OGS_AES_MAX_KEY_BITS)];
    int nrounds;

    ogs_assert(cmac);
    ogs_assert(key);
    ogs_assert(msg);

    /* Step 1.  (K1,K2) := Generate_Subkey(K); */
    _generate_subkey(k1, k2, key);

    /* Step 2.  n := ceil(len/const_Bsize); */
    n = (len + 15) / OGS_AES_BLOCK_SIZE;

    /* Step 3.  if n = 0
               then
                   n := 1;
                   flag := false;
               else
                   if len mod const_Bsize is 0
                   then flag := true;
                   else flag := false;
     */
    if (n == 0)
    {
        n = 1;
        flag = 0;
    }
    else
    {
        if (len % OGS_AES_BLOCK_SIZE == 0)
            flag = 1;
        else
            flag = 0;
    }

    /* Step 4.  if flag is true
                then M_last := M_n XOR K1;
                else M_last := padding(M_n) XOR K2;
                */
    bs = (n - 1) * OGS_AES_BLOCK_SIZE;

    if (flag)
    {
        for (i = 0; i < 16; i++)
            m_last[i] = msg[bs + i] ^ k1[i];
    }
    else
    {
        for (i = 0; i < len % OGS_AES_BLOCK_SIZE; i++)
            m_last[i] = msg[bs + i] ^ k2[i];

        m_last[i] = 0x80 ^ k2[i];

        for (i = i + 1; i < OGS_AES_BLOCK_SIZE; i++)
            m_last[i] = 0x00 ^ k2[i];
    }


    /* Step 5.  X := const_Zero;  */
    /* Step 6.  for i := 1 to n-1 do
                    begin
                        Y := X XOR M_i;
                        X := AES-128(K,Y);
                    end
                Y := M_last XOR X;
                T := AES-128(K,Y);
     */

    nrounds = ogs_aes_setup_enc(rk, key, 128);

    for (i = 0; i <= n - 2; i++)
    {
        bs = i * OGS_AES_BLOCK_SIZE;
        for (j = 0; j < 16; j++)
            y[j] = x[j] ^ msg[bs + j];
        ogs_aes_encrypt(rk, nrounds, y, x);
    }

    bs = (n - 1) * OGS_AES_BLOCK_SIZE;
    for (j = 0; j < 16; j++)
        y[j] = m_last[j] ^ x[j];
    ogs_aes_encrypt(rk, nrounds, y, cmac);

    return OGS_OK;
}

/*  From RFC 4493

    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    +                      Algorithm Verify_MAC                         +
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
    +                                                                   +
    +   Input    : K    ( 128-bit Key )                                 +
    +            : M    ( message to be verified )                      +
    +            : len  ( length of the message in octets )             +
    +            : T'   ( the received MAC to be verified )             +
    +   Output   : INVALID or VALID                                     +
    +                                                                   +
    +-------------------------------------------------------------------+
    +                                                                   +
    +   Step 1.  T* := AES-CMAC(K,M,len);                               +
    +   Step 2.  if T* is equal to T'                                   +
    +            then                                                   +
    +                 return VALID;                                     +
    +            else                                                   +
    +                 return INVALID;                                   +
    +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */

int ogs_aes_cmac_verify(uint8_t *cmac, const uint8_t *key,
        const uint8_t *msg, const uint32_t len)
{
    int rv;
    uint8_t cmac_calc[16];

    rv = ogs_aes_cmac_calculate(cmac_calc, key, msg, len);
    if (rv != OGS_OK)
        return rv;

    if (memcmp(cmac_calc, cmac, 16) != 0)
        return OGS_ERR_INVALID_CMAC;

    return OGS_OK;
}

