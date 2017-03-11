#ifndef __ASN_MHDR_H__
#define __ASN_MHDR_H__

#include "core.h"
#include "core_errno.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ASN_FUNC_QOS                    1
#define ASN_FUNC_HO_CTRL                2
#define ASN_FUNC_DP_CTRL                3
#define ASN_FUNC_CTX_TRAN               4
#define ASN_FUNC_R3_MOB                 5
#define ASN_FUNC_PAGING                 6
#define ASN_FUNC_RRM                    7
#define ASN_FUNC_AUTH_RELAY             8
#define ASN_FUNC_MS_STATE               9
#define ASN_FUNC_IM_OPERATION           10
#define ASN_FUNC_ACCOUNTING             11
#define ASN_FUNC_BASE                   ASN_FUNC_QOS
#define ASN_FUNC_TOP                    ASN_FUNC_ACCOUNTING

/* 1 ASN_FUNC_QOS */
#define F1_RR_REQ                       1
#define F1_RR_RSP                       2
#define F1_RR_ACK                       3
#define F1_BASE                         F1_RR_REQ
#define F1_TOP                          F1_RR_ACK

/* 2 ASN_FUNC_HO_CTRL */
#define F2_HO_REQ                       1
#define F2_HO_RSP                       2
#define F2_HO_ACK                       3
#define F2_HO_CNF                       4
#define F2_HO_COMPLETE                  5
#define F2_HO_DIRECTIVE                 6
#define F2_HO_DIRECTIVE_RSP             7
#define F2_BASE                         F2_HO_REQ
#define F2_TOP                          F2_HO_DIRECTIVE_RSP
    
/* 3 ASN_FUNC_DP_CTRL */
#define F3_PATH_DEREG_REQ               1
#define F3_PATH_DEREG_RSP               2
#define F3_PATH_DEREG_ACK               3
#define F3_PATH_MODIF_REQ               4
#define F3_PATH_MODIF_RSP               5
#define F3_PATH_MODIF_ACK               6
#define F3_PATH_PREREG_REQ              7
#define F3_PATH_PREREG_RSP              8
#define F3_PATH_PREREG_ACK              9
#define F3_PATH_REG_REQ                 10
#define F3_PATH_REG_RSP                 11
#define F3_PATH_REG_ACK                 12
#define F3_IM_EXIT_STATE_IND            13
#define F3_IM_EXIT_STATE_IND_ACK        14
#define F3_BASE                         F3_PATH_DEREG_REQ
#define F3_TOP                          F3_IM_EXIT_STATE_IND_ACK

/* 4 ASN_FUNC_CTX_TRAN */
#define F4_CONTEXT_REQ                  1
#define F4_CONTEXT_RPT                  2
#define F4_CONTEXT_ACK                  3
#define F4_CMAC_KEY_CNT_UPDATE          4
#define F4_CMAC_KEY_CNT_UPDATE_ACK      5
#define F4_PREPAID_REQ                  8
#define F4_PREPAID_NOTIFY               9
#define F4_BASE                         F4_CONTEXT_REQ
#define F4_TOP                          F4_PREPAID_NOTIFY

/* 5 ASN_FUNC_R3_MOB */
#define F5_ANCHOR_DPF_HO_REQ            1
#define F5_ANCHOR_DPF_HO_TRIG           2
#define F5_ANCHOR_DPF_HO_RSP            3
#define F5_ANCHOR_DPF_RELOC_REQ         4
#define F5_ANCHOR_DPF_RELOC_RSP         5
#define F5_FA_REGISTER_REQ              6
#define F5_FA_REGISTER_RSP              7
#define F5_FA_REVOKE_REQ                8
#define F5_FA_REVOKE_RSP                9
#define F5_ANCHOR_DPF_RELEASE_REQ       10
#define F5_RELOC_READY_REQ              11
#define F5_RELOC_READY_RSP              12
#define F5_BASE                         F5_ANCHOR_DPF_HO_REQ
#define F5_TOP                          F5_RELOC_READY_RSP

/* 6 ASN_FUNC_PAGING */
#define F6_PAGING_ANNOUNCE              1
#define F6_DEL_MS_ENTRY_REQ             2
#define F6_PC_RELOC_IND                 3
#define F6_PC_RELOC_ACK                 4
#define F6_DEL_MS_ENTRY_RSP             5
#define F6_ANCHOR_PC_IND                6
#define F6_ANCHOR_PC_ACK                7
#define F6_BASE                         F6_PAGING_ANNOUNCE
#define F6_TOP                          F6_ANCHOR_PC_ACK

/* 7 ASN_FUNC_RRM */
#define F7_R6_PHY_PARAM_REQ             1
#define F7_R6_PHY_PARAM_RPT             2
#define F7_R4R6_SPARE_CAPA_REQ          3
#define F7_R4R6_SPARE_CAPA_RPT          4
#define F7_R6_NBR_BS_STATUS_UPDATE      5
#define F7_R4R6_RADIO_CONF_UPDATE_REQ   6
#define F7_R4R6_RADIO_CONF_UPDATE_RPT   7
#define F7_R4R6_RADIO_CONF_UPDATE_ACK   8
#define F7_BASE                         F7_R6_PHY_PARAM_REQ
#define F7_TOP                          F7_R4R6_RADIO_CONF_UPDATE_ACK

/* 8 ASN_FUNC_AUTH_RELAY */
#define F8_AR_EAP_START                 1
#define F8_AR_EAP_TRANSFER              2
#define F8_BULK_INTERIM_UPDATE          3
#define F8_BULK_INTERIM_UPDATE_ACK      4
#define F8_BASE                         F8_AR_EAP_START
#define F8_TOP                          F8_BULK_INTERIM_UPDATE_ACK

/* 9 ASN_FUNC_MS_STATE */
#define F9_MS_PRE_ATTACH_REQ            1
#define F9_MS_PRE_ATTACH_RSP            2
#define F9_MS_PRE_ATTACH_ACK            3
#define F9_MS_ATTACH_REQ                4
#define F9_MS_ATTACH_RSP                5
#define F9_MS_ATTACH_ACK                6
#define F9_KEY_CHANGE_DIRECTIVE         7
#define F9_KEY_CHANGE_CNF               8
#define F9_KEY_CHANGE_ACK               9
#define F9_RELOC_COMPLETE_REQ           10
#define F9_RELOC_COMPLETE_RSP           11
#define F9_RELOC_COMPLETE_ACK           12
#define F9_RELOC_NOTIFY                 13
#define F9_RELOC_REQ                    14
#define F9_RELOC_RSP                    15
#define F9_NETEXIT_MS_STATE_CHANGE_REQ  16
#define F9_NETEXIT_MS_STATE_CHANGE_RSP  17
#define F9_RELOC_NOTIFY_RSP             18
#define F9_BASE                         F9_MS_PRE_ATTACH_REQ
#define F9_TOP                          F9_RELOC_NOTIFY_RSP

/* 10 ASN_FUNC_IM_OPERATION */
#define F10_IM_ENTRY_STATE_CHANGE_REQ   1
#define F10_IM_ENTRY_STATE_CHANGE_RSP   2
#define F10_IM_ENTRY_STATE_CHANGE_ACK   3
#define F10_IM_EXIT_STATE_CHANGE_REQ    4
#define F10_IM_EXIT_STATE_CHANGE_RSP    5
#define F10_INIT_PAGING_REQ             6
#define F10_INIT_PAGING_RSP             7
#define F10_LU_REQ                      8
#define F10_LU_RSP                      9
#define F10_LU_CNF                      10
#define F10_BASE                        F10_IM_ENTRY_STATE_CHANGE_REQ
#define F10_TOP                         F10_LU_CNF

/* 11 ASN_FUNC_ACCOUNTING */
#define F11_HOT_LINING_REQ              1
#define F11_HOT_LINING_RSP              2
#define F11_BASE                        F11_HOT_LINING_REQ
#define F11_TOP                         F11_HOT_LINING_RSP

typedef struct _asn_hdr_t {
#define ASN_HDR_VER_1           0x01
    c_uint8_t   version;
#define ASN_HDR_FLAG_RESTART    0x01 /**< Restart next expected transaction ID */
#define ASN_HDR_FLAG_RELAY      0x02 /**< Relay mode of operation */
#define ASN_HDR_FLAG_LEGACY     0x04 /**< 0: Sender is a lagacy node, 1: not legacy */
#define ASN_HDR_FLAG_COMP       0x08 /**< Comprehension is required for function type, OP ID and message type */
#define ASN_HDR_FLAG_ERR        0x10 /**< 1: Error reflection message, 0: not */
    c_uint8_t   flags;
    c_uint8_t   ftype;
#define ASN_HDR_OPID_REQ        0x01
#define ASN_HDR_OPID_RSP        0x02
#define ASN_HDR_OPID_ACK        0x03
#define ASN_HDR_OPID_IND        0x04
#define ASN_HDR_OPID_RSVD1      0x05
#define ASN_HDR_OPID_RSVD2      0x06
#define ASN_HDR_OPID_RSVD3      0x07
ED2(c_uint8_t   opid:3;,
    c_uint8_t   mtype:5;)
    c_uint16_t  length;
    c_uint8_t   msid[6];
    c_uint32_t  reserved1;
    c_uint16_t  trns_id;
    c_uint16_t  reserved2;
} __attribute__ ((packed)) asn_hdr_t;

#define ASN_HEADER_LEN 20 /* size of asn_hdr_t */

typedef struct _asn_hdr_desc_t {
    c_uint8_t   flags;
    c_uint8_t   ftype;
    c_uint8_t   opid;
    c_uint8_t   mtype;
    c_uint8_t   msid[6];
    c_uint16_t  trns_id;
} asn_hdr_desc_t;

extern char asn_func_type_str[][16];
extern char asn_opid_str[][4];
extern char asn_msg_type_str[][19][32];

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !__ASN_MHDR_H__ */
