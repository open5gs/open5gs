#ifndef __TLV_MSG_H__
#define __TLV_MSG_H__

#include "core_tlv.h"
#include "core_pkbuf.h"

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

typedef struct _tlv_desc_t {
    tlv_type_e ctype;
    c_int8_t *name;
    c_uint16_t type;
    c_uint16_t length;
    c_uint8_t  instance;
    c_uint16_t vsize;
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

typedef c_uint64_t tlv_presence_t;

/* 8-bit Unsigned integer */
typedef struct _tlv_uint8_t {
    tlv_presence_t presence;
    c_uint8_t u8;
} tlv_uint8_t;

/* 16-bit Unsigned integer */
typedef struct _tlv_uint16_t {
    tlv_presence_t presence;
    c_uint16_t u16;
} tlv_uint16_t;

/* 24-bit Unsigned integer */
typedef struct _tlv_uint24_t {
    tlv_presence_t presence;
    c_uint32_t u24; /* Only 3 bytes valid */
} tlv_uint24_t;

/* 32-bit Unsigned integer */
typedef struct _tlv_uint32_t {
    tlv_presence_t presence;
    c_uint32_t u32;
} tlv_uint32_t;

/* 8-bit Signed integer */
typedef struct _tlv_int8_t {
    tlv_presence_t presence;
    c_int8_t i8;
} tlv_int8_t;

/* 16-bit Signed integer */
typedef struct _tlv_int16t {
    tlv_presence_t presence;
    c_int16_t i16;
} tlv_int16_t;

/* 24-bit Signed integer */
typedef struct _tlv_int24_t {
    tlv_presence_t presence;
    c_int32_t i24; /* Only 3 bytes valid */
} tlv_int24_t;

/* 32-bit Signed integer */
typedef struct _tlv_int32_t {
    tlv_presence_t presence;
    c_int32_t i32;
} tlv_int32_t;

/* Octets */
#define TLV_CLEAR_DATA(__dATA) \
    do { \
        d_assert((__dATA), , "Null param"); \
        if ((__dATA)->data) \
        { \
            CORE_FREE((__dATA)->data); \
            (__dATA)->data = NULL; \
            (__dATA)->len = 0; \
            (__dATA)->presence = 0; \
        } \
    } while(0)
#define TLV_STORE_DATA(__dST, __sRC) \
    do { \
        d_assert((__sRC),, "Null param") \
        d_assert((__sRC)->data,, "Null param") \
        d_assert((__dST),, "Null param") \
        TLV_CLEAR_DATA(__dST); \
        (__dST)->presence = (__sRC)->presence; \
        (__dST)->len = (__sRC)->len; \
        (__dST)->data = core_calloc((__dST)->len, sizeof(c_uint8_t)); \
        memcpy((__dST)->data, (__sRC)->data, (__dST)->len); \
    } while(0)
typedef struct _tlv_octet_t {
    tlv_presence_t presence;
    void *data;
    c_uint32_t len;
} tlv_octet_t;

/* No value */
typedef struct _tlv_null {
    tlv_presence_t presence;
} tlv_null_t;

CORE_DECLARE(status_t) tlv_build_msg(
        pkbuf_t **pkbuf, tlv_desc_t *desc, void *msg, int mode);

CORE_DECLARE(status_t) tlv_parse_msg(
        void *msg, tlv_desc_t *desc, pkbuf_t *pkbuf, int mode);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TLV_MSG_H__ */
