#ifndef __ASN_TLV_DESC_H__
#define __ASN_TLV_DESC_H__

#include "asn_msg.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ASNT_AUTHORIZATION_POLICY_SUPPORT 21
#define ASNL_AUTHORIZATION_POLICY_SUPPORT 1
typedef asnv_uint8_t asnv_authorization_policy_support_t;
extern asnt_desc_t asnt_authorization_policy_support;

#define ASNT_MS_SECURITY_HISTORY 108
#define ASNL_MS_SECURITY_HISTORY ASNL_VARIABLE
typedef struct _asnv_ms_security_history_t {
    asnv_header_t h;
    asnv_authorization_policy_support_t authorization_policy_support;
} asnv_ms_security_history_t;
extern asnt_desc_t asnt_ms_security_history;

#define ASNT_MS_INFO 103
#define ASNL_MS_INFO ASNL_VARIABLE
typedef struct _asnv_ms_info_t {
    asnv_header_t h;
    asnv_ms_security_history_t ms_security_history;
} asnv_ms_info_t;
extern asnt_desc_t asnt_ms_info;

#define ASNT_BS_ID 25
#define ASNL_BS_ID ASNL_VARIABLE
typedef asnv_octets_t asnv_bs_id_t;
extern asnt_desc_t asnt_bs_id;

#define ASNT_BS_INFO 26
#define ASNL_BS_INFO ASNL_VARIABLE
typedef struct _asnv_bs_info_t {
    asnv_header_t h;
    asnv_bs_id_t bs_id;
} asnv_bs_info_t;
extern asnt_desc_t asnt_bs_info;

typedef struct _asnv_msg_ms_preattachment_req {
    asnv_ms_info_t ms_info;
    asnv_bs_info_t bs_info;
} asnv_msg_ms_preattachment_req;

extern asn_msg_desc_t asnt_msg_ms_preattachment_req;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ASN_TLV_DESC_H__ */
