#ifndef __ASN_TLV_DESC_H__
#define __ASN_TLV_DESC_H__

#include "asn_msg.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define ASNT_AUTHORIZATION_POLICY_SUPPORT 21
#define ASN_AUTHORIZATION_POLICY_SUPPORT 1
typedef asn_uint8_t asn_authorization_policy_support_t;
extern asn_desc_t asnt_authorization_policy_support;

#define ASNT_MS_SECURITY_HISTORY 108
#define ASN_MS_SECURITY_HISTORY ASN_VARIABLE
typedef struct _asn_ms_security_history_t {
    asn_header_t h;
    asn_authorization_policy_support_t authorization_policy_support;
} asn_ms_security_history_t;
extern asn_desc_t asnt_ms_security_history;

#define ASNT_MS_INFO 103
#define ASN_MS_INFO ASN_VARIABLE
typedef struct _asn_ms_info_t {
    asn_header_t h;
    asn_ms_security_history_t ms_security_history;
} asn_ms_info_t;
extern asn_desc_t asnt_ms_info;

#define ASNT_BS_ID 25
#define ASN_BS_ID ASN_VARIABLE
typedef asn_octets_t asn_bs_id_t;
extern asn_desc_t asnt_bs_id;

#define ASNT_BS_INFO 26
#define ASN_BS_INFO ASN_VARIABLE
typedef struct _asn_bs_info_t {
    asn_header_t h;
    asn_bs_id_t bs_id;
} asn_bs_info_t;
extern asn_desc_t asnt_bs_info;

typedef struct _asn_msg_ms_preattachment_req {
    asn_ms_info_t ms_info;
    asn_bs_info_t bs_info;
} asn_msg_ms_preattachment_req;

extern asn_desc_t asnt_msg_ms_preattachment_req;

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __ASN_TLV_DESC_H__ */
