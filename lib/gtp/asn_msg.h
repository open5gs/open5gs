#ifndef __ASN_MSG_H__
#define __ASN_MSG_H__

#include "core_pkbuf.h"

#define ASN_HEADER_LEN 12
#define ASN_VARIABLE 0

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

typedef struct _asn_header_t {
    c_uint64_t set_ind;
} asn_header_t;

#define VALUE_SET(__m, __value) \
    __m.v = __value; __m.h.set_ind = 1

#define OCTET_SET(__m, __value, __length) \
    __m.v = __value; __m.l = __length; __m.h.set_ind = 1

#define COMPD_SET(__m) __m.h.set_ind = 1

#define VALUE_UNSET(__m) \
    __m.h.set_ind = 0

#define VALUE_ISSET(__m) \
    __m.h.set_ind

#define OCTET_ISSET(__asn) VALUE_ISSET(__asn)

#define COMPD_ISSET(__asn) VALUE_ISSET(__asn)

#define VALUE_GET(__asn) (__asn).v

#define VALUE_COPY(__t_asn, __s_asn) \
    (__t_asn).h = (__s_asn).h; (__t_asn).v = (__s_asn).v

#define OCTET_GET(__value, __length, __asn) \
    __value = (__asn).v; __length = (__asn).l

#define OCTET_COPY(__t_asn, __s_asn) \
    (__t_asn).h = (__s_asn).h; (__t_asn).v = (__s_asn).v; \
    (__t_asn).l = (__s_asn).l

typedef enum {
    ASN_UINT8,
    ASN_UINT16,
    ASN_UINT24,
    ASN_UINT32,
    ASN_INT8,
    ASN_INT16,
    ASN_INT24,
    ASN_INT32,
    ASN_FIXED_STR,
    ASN_VAR_STR,
    ASN_NULL,
    ASN_MORE,
    ASN_COMPOUND,
    ASN_MESSAGE,
} asnc_type_e;

typedef struct _asn_desc_t {
    asnc_type_e ctype;
    c_uint16_t type;
    c_uint16_t length;
    c_uint16_t vsize;
    void *child_descs[];
} asn_desc_t;

typedef asn_desc_t asn_desc_t;

/* 8-bit Unsigned integer */
typedef struct _asn_uint8_t {
    asn_header_t h;
    c_uint8_t v;
    c_uint8_t dummy[3]; /* for 4 bytes align */
} asn_uint8_t;

/* 16-bit Unsigned integer */
typedef struct _asn_uint16t {
    asn_header_t h;
    c_uint16_t v;
    c_uint8_t dummy[2]; /* for 4 bytes align */
} asn_uint16_t;

/* 24-bit Unsigned integer */
typedef struct _asn_uint24t {
    asn_header_t h;
    c_uint32_t v; /* Only 3 bytes valid */
} asn_uint24_t;

/* 32-bit Unsigned integer */
typedef struct _asn_uint32t {
    asn_header_t h;
    c_uint32_t v;
} asn_uint32_t;

/* 8-bit Signed integer */
typedef struct _asn_int8_t {
    asn_header_t h;
    c_int8_t v;
    c_uint8_t dummy[3]; /* for 4 bytes align */
} asn_int8_t;

/* 16-bit Signed integer */
typedef struct _asn_int16t {
    asn_header_t h;
    c_int16_t v;
    c_uint8_t dummy[2]; /* for 4 bytes align */
} asn_int16_t;

/* 24-bit Signed integer */
typedef struct _asn_int24t {
    asn_header_t h;
    c_int32_t v; /* Only 3 bytes valid */
} asn_int24_t;

/* 32-bit Signed integer */
typedef struct _asn_int32t {
    asn_header_t h;
    c_int32_t v;
} asn_int32_t;

/* Octets */
typedef struct _asn_octets_t {
    asn_header_t h;
    c_uint8_t *v;
    c_uint32_t l;
} asn_octets_t;

/* No value */
typedef struct _asn_null {
    asn_header_t h;
} asn_null_t;

#define ASN_MORE 8
#define ASN_1_OR_MORE(__v) __v[ASN_MORE]

extern asn_desc_t asnt_more;

CORE_DECLARE(status_t) asn_build_msg(
        pkbuf_t **msg, asn_desc_t *msg_desc, void *asn);

CORE_DECLARE(status_t) asn_parse_msg(
        void *asn, asn_desc_t *msg_desc, pkbuf_t *msg);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ASN_MSG_H__ */
