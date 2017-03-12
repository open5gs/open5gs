#ifndef __ASN_MSG_H__
#define __ASN_MSG_H__

#include "core_pkbuf.h"

#define ASN_HEADER_LEN 12
#define ASNL_VARIABLE 0

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _asnv_header_t {
    c_uint64_t set_ind;
} asnv_header_t;

#define VALUE_SET(__m, __value) \
    __m.v = __value; __m.h.set_ind = 1

#define OCTET_SET(__m, __value, __length) \
    __m.v = __value; __m.l = __length; __m.h.set_ind = 1

#define COMPD_SET(__m) __m.h.set_ind = 1

#define VALUE_UNSET(__m) \
    __m.h.set_ind = 0

#define VALUE_ISSET(__m) \
    __m.h.set_ind

#define OCTET_ISSET(__asnv) VALUE_ISSET(__asnv)

#define COMPD_ISSET(__asnv) VALUE_ISSET(__asnv)

#define VALUE_GET(__asnv) (__asnv).v

#define VALUE_COPY(__t_asnv, __s_asnv) \
    (__t_asnv).h = (__s_asnv).h; (__t_asnv).v = (__s_asnv).v

#define OCTET_GET(__value, __length, __asnv) \
    __value = (__asnv).v; __length = (__asnv).l

#define OCTET_COPY(__t_asnv, __s_asnv) \
    (__t_asnv).h = (__s_asnv).h; (__t_asnv).v = (__s_asnv).v; \
    (__t_asnv).l = (__s_asnv).l

typedef enum {
    ASNC_UINT8,
    ASNC_UINT16,
    ASNC_UINT24,
    ASNC_UINT32,
    ASNC_INT8,
    ASNC_INT16,
    ASNC_INT24,
    ASNC_INT32,
    ASNC_FIXED_STR,
    ASNC_VAR_STR,
    ASNC_NULL,
    ASNC_MORE,
    ASNC_COMPOUND,
    ASNC_MESSAGE,
} asnc_type_e;

typedef struct _asnt_desc_t {
    asnc_type_e ctype;
    c_uint16_t type;
    c_uint16_t length;
    c_uint16_t vsize;
    void *child_descs[];
} asnt_desc_t;

typedef asnt_desc_t asn_msg_desc_t;

/* 8-bit Unsigned integer */
typedef struct _asnv_uint8_t {
    asnv_header_t h;
    c_uint8_t v;
    c_uint8_t dummy[3]; /* for 4 bytes align */
} asnv_uint8_t;

/* 16-bit Unsigned integer */
typedef struct _asnv_uint16t {
    asnv_header_t h;
    c_uint16_t v;
    c_uint8_t dummy[2]; /* for 4 bytes align */
} asnv_uint16_t;

/* 24-bit Unsigned integer */
typedef struct _asnv_uint24t {
    asnv_header_t h;
    c_uint32_t v; /* Only 3 bytes valid */
} asnv_uint24_t;

/* 32-bit Unsigned integer */
typedef struct _asnv_uint32t {
    asnv_header_t h;
    c_uint32_t v;
} asnv_uint32_t;

/* 8-bit Signed integer */
typedef struct _asnv_int8_t {
    asnv_header_t h;
    c_int8_t v;
    c_uint8_t dummy[3]; /* for 4 bytes align */
} asnv_int8_t;

/* 16-bit Signed integer */
typedef struct _asnv_int16t {
    asnv_header_t h;
    c_int16_t v;
    c_uint8_t dummy[2]; /* for 4 bytes align */
} asnv_int16_t;

/* 24-bit Signed integer */
typedef struct _asnv_int24t {
    asnv_header_t h;
    c_int32_t v; /* Only 3 bytes valid */
} asnv_int24_t;

/* 32-bit Signed integer */
typedef struct _asnv_int32t {
    asnv_header_t h;
    c_int32_t v;
} asnv_int32_t;

/* Octets */
typedef struct _asnv_octets_t {
    asnv_header_t h;
    c_uint8_t *v;
    c_uint32_t l;
} asnv_octets_t;

/* No value */
typedef struct _asnv_null {
    asnv_header_t h;
} asnv_null_t;

#define ASNL_MORE 8
#define ASNV_1_OR_MORE(__v) __v[ASNL_MORE]

extern asnt_desc_t asnt_more;

CORE_DECLARE(status_t) asn_build_msg(
        pkbuf_t **msg, asn_msg_desc_t *msg_desc, void *asnv);

CORE_DECLARE(status_t) asn_parse_msg(
        void *asnv, asn_msg_desc_t *msg_desc, pkbuf_t *msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ASN_MSG_H__ */
