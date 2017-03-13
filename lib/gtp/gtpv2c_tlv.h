#ifndef __GTPV2C_TLV_H__
#define __GTPV2C_TLV_H__

#include "core_tlv_msg.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TLV_AUTHORIZATION_POLICY_SUPPORT_TYPE 21
#define TLV_AUTHORIZATION_POLICY_SUPPORT_LEN 1
typedef tlv_uint8_t tlv_authorization_policy_support_t;
extern tlv_desc_t tlv_desc_authorization_policy_support;

#define TLV_MS_SECURITY_HISTORY_TYPE 108
#define TLV_MS_SECURITY_HISTORY_LEN TLV_VARIABLE_LEN
typedef struct _tlv_ms_security_history_t {
    tlv_header_t h;
    tlv_authorization_policy_support_t authorization_policy_support;
} tlv_ms_security_history_t;
extern tlv_desc_t tlv_desc_ms_security_history;

#define TLV_MS_INFO_TYPE 103
#define TLV_MS_INFO_LEN TLV_VARIABLE_LEN
typedef struct _tlv_ms_info_t {
    tlv_header_t h;
    tlv_ms_security_history_t ms_security_history;
} tlv_ms_info_t;
extern tlv_desc_t tlv_desc_ms_info;

#define TLV_BS_ID_TYPE 25
#define TLV_BS_ID_LEN TLV_VARIABLE_LEN
typedef tlv_octet_t tlv_bs_id_t;
extern tlv_desc_t tlv_desc_bs_id;

#define TLV_BS_INFO_TYPE 26
#define TLV_BS_INFO_LEN TLV_VARIABLE_LEN
typedef struct _tlv_bs_info_t {
    tlv_header_t h;
    tlv_bs_id_t bs_id;
} tlv_bs_info_t;
extern tlv_desc_t tlv_desc_bs_info;

typedef struct _tlv_msg_ms_preattachment_req {
    tlv_ms_info_t ms_info;
    tlv_bs_info_t bs_info;
} tlv_msg_ms_preattachment_req;

extern tlv_desc_t tlv_desc_msg_ms_preattachment_req;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __GTPV2C_TLV_H__ */
