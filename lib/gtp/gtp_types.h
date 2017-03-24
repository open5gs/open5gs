#ifndef __GTP_TYPES_H__
#define __GTP_TYPES_H__

#include "core_tlv_msg.h"
#include "3gpp_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 8.21    User Location Information (ULI) */
typedef struct _gtp_uli_cgi_t {
    plmn_id_t plmn_id;
    c_uint16_t lac;
    c_uint16_t ci;
} __attribute__ ((packed)) gtp_uli_cgi_t;

typedef struct _gtp_uli_sai_t {
    plmn_id_t plmn_id;
    c_uint16_t lac;
    c_uint16_t sac;
} __attribute__ ((packed)) gtp_uli_sai_t;

typedef struct _gtp_uli_rai_t {
    plmn_id_t plmn_id;
    c_uint16_t lac;
    c_uint16_t rac;
} __attribute__ ((packed)) gtp_uli_rai_t;

typedef struct _gtp_uli_tai_t {
    plmn_id_t plmn_id;
    c_uint16_t tac;
} __attribute__ ((packed)) gtp_uli_tai_t;

typedef struct _gtp_uli_ecgi_t {
    plmn_id_t plmn_id;
    c_uint32_t eci; /* 28 bit */
} __attribute__ ((packed)) gtp_uli_ecgi_t;

typedef struct _gtp_uli_lai_t {
    plmn_id_t plmn_id;
    c_uint16_t lac;
} __attribute__ ((packed)) gtp_uli_lai_t;

typedef struct _gtp_uli_t {
    struct {
    ED7(c_uint8_t spare:2;,
        c_uint8_t lai:1;,
        c_uint8_t ecgi:1;,
        c_uint8_t tai:1;,
        c_uint8_t rai:1;,
        c_uint8_t sai:1;,
        c_uint8_t cgi:1;)
    } flags;
    gtp_uli_cgi_t cgi;
    gtp_uli_sai_t sai;
    gtp_uli_rai_t rai;
    gtp_uli_tai_t tai;
    gtp_uli_ecgi_t ecgi;
    gtp_uli_lai_t lai;
} gtp_uli_t;

c_uint16_t gtp_decode_uli(gtp_uli_t *uli, tlv_octet_t *octet);
c_uint16_t gtp_encode_uli(tlv_octet_t *octet, gtp_uli_t *uli);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_TYPES_H__ */

