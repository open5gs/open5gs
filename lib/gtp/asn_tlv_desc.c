#include "asn_tlv_desc.h"

asn_desc_t asnt_authorization_policy_support = 
{
    ASN_UINT8,
    ASNT_AUTHORIZATION_POLICY_SUPPORT,
    ASN_AUTHORIZATION_POLICY_SUPPORT,
    sizeof(asn_authorization_policy_support_t),
    { NULL }
};

asn_desc_t asnt_ms_security_history = 
{
    ASN_COMPOUND,
    ASNT_MS_SECURITY_HISTORY,
    ASN_MS_SECURITY_HISTORY,
    sizeof(asn_ms_security_history_t),
    {
        &asnt_authorization_policy_support,
        NULL,
    }
};

asn_desc_t asnt_ms_info = 
{
    ASN_COMPOUND,
    ASNT_MS_INFO,
    ASN_MS_INFO,
    sizeof(asn_ms_info_t),
    {
        &asnt_ms_security_history,
        NULL,
    }
};

asn_desc_t asnt_bs_id = 
{
    ASN_VAR_STR,
    ASNT_BS_ID,
    ASN_BS_ID,
    sizeof(asn_bs_id_t),
    { NULL }
};

asn_desc_t asnt_bs_info = 
{
    ASN_COMPOUND,
    ASNT_BS_INFO,
    ASN_BS_INFO,
    sizeof(asn_bs_info_t),
    {
        &asnt_bs_id,
        NULL,
    }
};

asn_desc_t asnt_msg_ms_preattachment_req = {
    ASN_MESSAGE, 0, 0, 0, {
    &asnt_ms_info,
    &asnt_bs_info,
    NULL,
}};
