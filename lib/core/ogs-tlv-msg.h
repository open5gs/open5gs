/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
 * Copyright (C) 2022 by sysmocom - s.f.m.c. GmbH <info@sysmocom.de>
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

#if !defined(OGS_CORE_INSIDE) && !defined(OGS_CORE_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_TLV_MSG_H
#define OGS_TLV_MSG_H

#ifdef __cplusplus
extern "C" {
#endif

#define OGS_TLV_MAX_HEADROOM 16
#define OGS_TLV_VARIABLE_LEN 0
#define OGS_TLV_MAX_MORE 16
#define OGS_TLV_1_OR_MORE(__v) __v[OGS_TLV_MAX_MORE]

#define OGS_TLV_MAX_CHILD_DESC 128

typedef enum {
    OGS_TLV_UINT8,
    OGS_TLV_UINT16,
    OGS_TLV_UINT24,
    OGS_TLV_UINT32,
    OGS_TLV_INT8,
    OGS_TLV_INT16,
    OGS_TLV_INT24,
    OGS_TLV_INT32,
    OGS_TLV_FIXED_STR,
    OGS_TLV_VAR_STR,
    OGS_TLV_NULL,
    OGS_TLV_MORE,
    OGS_TLV_COMPOUND,
    OGS_TLV_MESSAGE,
    OGS_TV_UINT8,
    OGS_TV_UINT16,
    OGS_TV_UINT24,
    OGS_TV_UINT32,
    OGS_TV_INT8,
    OGS_TV_INT16,
    OGS_TV_INT24,
    OGS_TV_INT32,
    OGS_TV_FIXED_STR,
    OGS_TV_VAR_STR,
    OGS_TV_NULL,
    OGS_TV_MORE,
} ogs_tlv_type_e;

typedef struct ogs_tlv_desc_s {
    ogs_tlv_type_e ctype;
    const char *name;
    uint16_t type;
    uint16_t length;
    uint8_t  instance;
    uint16_t vsize;
    void *child_descs[OGS_TLV_MAX_CHILD_DESC];
} ogs_tlv_desc_t;

extern ogs_tlv_desc_t ogs_tlv_desc_more1;
extern ogs_tlv_desc_t ogs_tlv_desc_more2;
extern ogs_tlv_desc_t ogs_tlv_desc_more3;
extern ogs_tlv_desc_t ogs_tlv_desc_more4;
extern ogs_tlv_desc_t ogs_tlv_desc_more5;
extern ogs_tlv_desc_t ogs_tlv_desc_more6;
extern ogs_tlv_desc_t ogs_tlv_desc_more7;
extern ogs_tlv_desc_t ogs_tlv_desc_more8;
extern ogs_tlv_desc_t ogs_tlv_desc_more9;
extern ogs_tlv_desc_t ogs_tlv_desc_more10;
extern ogs_tlv_desc_t ogs_tlv_desc_more11;
extern ogs_tlv_desc_t ogs_tlv_desc_more12;
extern ogs_tlv_desc_t ogs_tlv_desc_more13;
extern ogs_tlv_desc_t ogs_tlv_desc_more14;
extern ogs_tlv_desc_t ogs_tlv_desc_more15;
extern ogs_tlv_desc_t ogs_tlv_desc_more16;

typedef uint64_t ogs_tlv_presence_t;

/* 8-bit Unsigned integer */
typedef struct ogs_tlv_uint8_s {
    ogs_tlv_presence_t presence;
    uint8_t u8;
} ogs_tlv_uint8_t;

/* 16-bit Unsigned integer */
typedef struct ogs_tlv_uint16_s {
    ogs_tlv_presence_t presence;
    uint16_t u16;
} ogs_tlv_uint16_t;

/* 24-bit Unsigned integer */
typedef struct ogs_tlv_uint24_s {
    ogs_tlv_presence_t presence;
    uint32_t u24; /* Only 3 bytes valid */
} ogs_tlv_uint24_t;

/* 32-bit Unsigned integer */
typedef struct ogs_tlv_uint32_s {
    ogs_tlv_presence_t presence;
    uint32_t u32;
} ogs_tlv_uint32_t;

/* 8-bit Signed integer */
typedef struct ogs_tlv_int8_s {
    ogs_tlv_presence_t presence;
    int8_t i8;
} ogs_tlv_int8_t;

/* 16-bit Signed integer */
typedef struct ogs_tlv_int16_s {
    ogs_tlv_presence_t presence;
    int16_t i16;
} ogs_tlv_int16_t;

/* 24-bit Signed integer */
typedef struct tlv_int24_s {
    ogs_tlv_presence_t presence;
    int32_t i24; /* Only 3 bytes valid */
} tlv_int24_t;

/* 32-bit Signed integer */
typedef struct ogs_tlv_int32_s {
    ogs_tlv_presence_t presence;
    int32_t i32;
} ogs_tlv_int32_t;

/* Octets */
#define OGS_TLV_CLEAR_DATA(__dATA) \
    do { \
        ogs_assert((__dATA)); \
        if ((__dATA)->data) { \
            ogs_free((__dATA)->data); \
            (__dATA)->data = NULL; \
            (__dATA)->len = 0; \
            (__dATA)->presence = 0; \
        } \
    } while(0)
#define OGS_TLV_STORE_DATA(__dST, __sRC) \
    do { \
        ogs_assert((__sRC)); \
        ogs_assert((__sRC)->data); \
        ogs_assert((__dST)); \
        OGS_TLV_CLEAR_DATA(__dST); \
        (__dST)->presence = (__sRC)->presence; \
        (__dST)->len = (__sRC)->len; \
        (__dST)->data = ogs_calloc((__dST)->len, sizeof(uint8_t)); \
        ogs_assert((__dST)->data); \
        memcpy((__dST)->data, (__sRC)->data, (__dST)->len); \
    } while(0)
typedef struct ogs_tlv_octet_s {
    ogs_tlv_presence_t presence;
    void *data;
    uint32_t len;
} ogs_tlv_octet_t;

/* No value */
typedef struct ogs_tlv_null_s {
    ogs_tlv_presence_t presence;
} ogs_tlv_null_t;

ogs_pkbuf_t *ogs_tlv_build_msg(ogs_tlv_desc_t *desc, void *msg, int mode);
int ogs_tlv_parse_msg(
        void *msg, ogs_tlv_desc_t *desc, ogs_pkbuf_t *pkbuf, int mode);
int ogs_tlv_parse_msg_desc(
        void *msg, ogs_tlv_desc_t *desc, ogs_pkbuf_t *pkbuf, int msg_mode);

#ifdef __cplusplus
}
#endif

#endif /* OGS_GTP_TLV_H */
