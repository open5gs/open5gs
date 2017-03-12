#ifndef __TLV_TLV_DESC_H__
#define __TLV_TLV_DESC_H__

#include "asn_msg.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define TLV_AUTHORIZATION_POLICY_SUPPORT_TYPE 21
#define TLV_AUTHORIZATION_POLICY_SUPPORT_LEN 1
typedef asn_uint8_t asn_authorization_policy_support_t;
extern asn_desc_t asn_desc_authorization_policy_support;

#define TLV_MS_SECURITY_HISTORY_TYPE 108
#define TLV_MS_SECURITY_HISTORY_LEN TLV_VARIABLE
typedef struct _asn_ms_security_history_t {
    asn_header_t h;
    asn_authorization_policy_support_t authorization_policy_support;
} asn_ms_security_history_t;
extern asn_desc_t asn_desc_ms_security_history;

#define TLV_MS_INFO_TYPE 103
#define TLV_MS_INFO_LEN TLV_VARIABLE
typedef struct _asn_ms_info_t {
    asn_header_t h;
    asn_ms_security_history_t ms_security_history;
} asn_ms_info_t;
extern asn_desc_t asn_desc_ms_info;

#define TLV_BS_ID_TYPE 25
#define TLV_BS_ID_LEN TLV_VARIABLE
typedef asn_octets_t asn_bs_id_t;
extern asn_desc_t asn_desc_bs_id;

#define TLV_BS_INFO_TYPE 26
#define TLV_BS_INFO_LEN TLV_VARIABLE
typedef struct _asn_bs_info_t {
    asn_header_t h;
    asn_bs_id_t bs_id;
} asn_bs_info_t;
extern asn_desc_t asn_desc_bs_info;

typedef struct _asn_msg_ms_preattachment_req {
    asn_ms_info_t ms_info;
    asn_bs_info_t bs_info;
} asn_msg_ms_preattachment_req;

extern asn_desc_t asn_desc_msg_ms_preattachment_req;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __TLV_TLV_DESC_H__ */
