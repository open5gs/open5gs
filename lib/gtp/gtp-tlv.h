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

#ifndef GTP_TLV_H
#define GTP_TLV_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TLV_MAX_HEADROOM 16
#define TLV_VARIABLE_LEN 0
#define TLV_MAX_MORE 8
#define TLV_1_OR_MORE(__v) __v[TLV_MAX_MORE]

#define TLV_MAX_CHILD_DESC 128

typedef enum {
    TLV_UINT8,
    TLV_UINT16,
    TLV_UINT24,
    TLV_UINT32,
    TLV_INT8,
    TLV_INT16,
    TLV_INT24,
    TLV_INT32,
    TLV_FIXED_STR,
    TLV_VAR_STR,
    TLV_NULL,
    TLV_MORE,
    TLV_COMPOUND,
    TLV_MESSAGE,
} tlv_type_e;

typedef struct tlv_desc_s {
    tlv_type_e ctype;
    char *name;
    uint16_t type;
    uint16_t length;
    uint8_t  instance;
    uint16_t vsize;
    void *child_descs[TLV_MAX_CHILD_DESC];
} tlv_desc_t;

extern tlv_desc_t tlv_desc_more1;
extern tlv_desc_t tlv_desc_more2;
extern tlv_desc_t tlv_desc_more3;
extern tlv_desc_t tlv_desc_more4;
extern tlv_desc_t tlv_desc_more5;
extern tlv_desc_t tlv_desc_more6;
extern tlv_desc_t tlv_desc_more7;
extern tlv_desc_t tlv_desc_more8;

typedef uint64_t tlv_presence_t;

/* 8-bit Unsigned integer */
typedef struct tlv_uint8_s {
    tlv_presence_t presence;
    uint8_t u8;
} tlv_uint8_t;

/* 16-bit Unsigned integer */
typedef struct tlv_uint16_s {
    tlv_presence_t presence;
    uint16_t u16;
} tlv_uint16_t;

/* 24-bit Unsigned integer */
typedef struct tlv_uint24_s {
    tlv_presence_t presence;
    uint32_t u24; /* Only 3 bytes valid */
} tlv_uint24_t;

/* 32-bit Unsigned integer */
typedef struct tlv_uint32_s {
    tlv_presence_t presence;
    uint32_t u32;
} tlv_uint32_t;

/* 8-bit Signed integer */
typedef struct tlv_int8_s {
    tlv_presence_t presence;
    int8_t i8;
} tlv_int8_t;

/* 16-bit Signed integer */
typedef struct tlv_int16_s {
    tlv_presence_t presence;
    int16_t i16;
} tlv_int16_t;

/* 24-bit Signed integer */
typedef struct tlv_int24_s {
    tlv_presence_t presence;
    int32_t i24; /* Only 3 bytes valid */
} tlv_int24_t;

/* 32-bit Signed integer */
typedef struct tlv_int32_s {
    tlv_presence_t presence;
    int32_t i32;
} tlv_int32_t;

/* Octets */
#define TLV_CLEAR_DATA(__dATA) \
    do { \
        ogs_assert((__dATA)); \
        if ((__dATA)->data) \
        { \
            ogs_free((__dATA)->data); \
            (__dATA)->data = NULL; \
            (__dATA)->len = 0; \
            (__dATA)->presence = 0; \
        } \
    } while(0)
#define TLV_STORE_DATA(__dST, __sRC) \
    do { \
        ogs_assert((__sRC)); \
        ogs_assert((__sRC)->data); \
        ogs_assert((__dST)); \
        TLV_CLEAR_DATA(__dST); \
        (__dST)->presence = (__sRC)->presence; \
        (__dST)->len = (__sRC)->len; \
        (__dST)->data = ogs_calloc((__dST)->len, sizeof(uint8_t)); \
        memcpy((__dST)->data, (__sRC)->data, (__dST)->len); \
    } while(0)
typedef struct tlv_octet_s {
    tlv_presence_t presence;
    void *data;
    uint32_t len;
} tlv_octet_t;

/* No value */
typedef struct tlv_null_s {
    tlv_presence_t presence;
} tlv_null_t;

int tlv_build_msg(ogs_pkbuf_t **pkbuf, tlv_desc_t *desc, void *msg, int mode);

int tlv_parse_msg(void *msg, tlv_desc_t *desc, ogs_pkbuf_t *pkbuf, int mode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GTP_TLV_H */
