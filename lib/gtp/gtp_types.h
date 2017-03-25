#ifndef __GTP_TYPES_H__
#define __GTP_TYPES_H__

#include "core_tlv_msg.h"
#include "3gpp_defs.h"
#include "3gpp_types.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* 8.17 RAT Type */
#define GTP_RAT_TYPE_UTRAN              1
#define GTP_RAT_TYPE_GERAN              2
#define GTP_RAT_TYPE_WLAN               3
#define GTP_RAT_TYPE_GAN                4
#define GTP_RAT_TYPE_HSPA_EVOLUTION     5
#define GTP_RAT_TYPE_EUTRAN             6
#define GTP_RAT_TYPE_VIRTUAL            7
#define GTP_RAT_TYPE_EUTRAN_NB_IOT      8

/* 8.21 User Location Information (ULI) */
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

CORE_DECLARE(c_int16_t) gtp_parse_uli(gtp_uli_t *uli, tlv_octet_t *octet);
CORE_DECLARE(c_int16_t) gtp_build_uli(
        tlv_octet_t *octet, gtp_uli_t *uli, void *data, int data_len);

/* 8.22 Fully Qualified TEID (F-TEID) */
#define GTP_F_TEID_S1_U_ENODEB_GTP_U                        0
#define GTP_F_TEID_S1_U_SGW_GTP_U                           1
#define GTP_F_TEID_S12_RNC_GTP_U                            2
#define GTP_F_TEID_S12_SGW_GTP_U                            3
#define GTP_F_TEID_S5_S8_SGW_GTP_U                          4
#define GTP_F_TEID_S5_S8_PGW_GTP_U                          5
#define GTP_F_TEID_S5_S8_SGW_GTP_C                          6
#define GTP_F_TEID_S5_S8_PGW_GTP_C                          7
#define GTP_F_TEID_S5_S8_SGW_PMIPV6                         8
#define GTP_F_TEID_S5_S8_PGW_PMIPV6                         9
#define GTP_F_TEID_S11_MME_GTP_C                            10
#define GTP_F_TEID_S11_S4_SGW_GTP_C                         11
#define GTP_F_TEID_S10_MME_GTP_C                            12
#define GTP_F_TEID_S3_MME_GTP_C                             13
#define GTP_F_TEID_S3_SGSN_GTP_C                            14
#define GTP_F_TEID_S4_SGSN_GTP_U                            15
#define GTP_F_TEID_S4_SGW_GTP_U                             16
#define GTP_F_TEID_S4_SGSN_GTP_C                            17
#define GTP_F_TEID_S16_SGSN_GTP_C                           18
#define GTP_F_TEID_ENODEB_GTP_U_FOR_DL_DATA_FORWARDING      19
#define GTP_F_TEID_ENODEB_GTP_U_FOR_UL_DATA_FORWARDING      20
#define GTP_F_TEID_RNC_GTP_U_FOR_DATA_FORWARDING            21
#define GTP_F_TEID_SGSN_GTP_U_FOR_DATA_FORWARDING           22
#define GTP_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING         23
#define GTP_F_TEID_SM_MBMS_GW_GTP_C                         24
#define GTP_F_TEID_SN_MBMS_GW_GTP_C                         25
#define GTP_F_TEID_SM_MME_GTP_C                             26
#define GTP_F_TEID_SN_SGSN_GTP_C                            27
#define GTP_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING         28
#define GTP_F_TEID_SN_SGSN_GTP_U                            29
#define GTP_F_TEID_S2B_EPDG_GTP_C                           30
#define GTP_F_TEID_S2B_U_EPDG_GTP_U                         31
#define GTP_F_TEID_S2B_PGW_GTP_C                            32
#define GTP_F_TEID_S2B_U_PGW_GTP_U                          33
#define GTP_F_TEID_S2A_TWAN_GTP_U                           34
#define GTP_F_TEID_S2A_TWAN_GTP_C                           35
#define GTP_F_TEID_S2A_PGW_GTP_C                            36
#define GTP_F_TEID_S2A_PGW_GTP_U                            37
#define GTP_F_TEID_S11_MME_GTP_U                            38
#define GTP_F_TEID_S11_SGW_GTP_U                            39

#define GTP_F_TEID_IPV4_LEN                                 9
#define GTP_F_TEID_IPV6_LEN                                 21
typedef struct _gtp_f_teid_t {
ED3(c_uint8_t ipv4:1;,
    c_uint8_t ipv6:1;,
    c_uint8_t interface_type:6;)
    c_uint32_t teid;
    union {
        c_uint32_t ipv4_addr;
        c_uint8_t ipv6_addr[IPV6_LEN];
    };
} __attribute__ ((packed)) gtp_f_teid_t;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTP_TYPES_H__ */

