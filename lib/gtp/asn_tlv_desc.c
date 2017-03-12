#include "asn_tlv_desc.h"

asnt_desc_t asnt_authorization_policy_support = 
{
    ASNC_UINT8,
    ASNT_AUTHORIZATION_POLICY_SUPPORT,
    ASNL_AUTHORIZATION_POLICY_SUPPORT,
    sizeof(asnv_authorization_policy_support_t),
    { NULL }
};

asnt_desc_t asnt_ms_security_history = 
{
    ASNC_COMPOUND,
    ASNT_MS_SECURITY_HISTORY,
    ASNL_MS_SECURITY_HISTORY,
    sizeof(asnv_ms_security_history_t),
    {
        &asnt_authorization_policy_support,
        NULL,
    }
};

asnt_desc_t asnt_ms_info = 
{
    ASNC_COMPOUND,
    ASNT_MS_INFO,
    ASNL_MS_INFO,
    sizeof(asnv_ms_info_t),
    {
        &asnt_ms_security_history,
        NULL,
    }
};

asnt_desc_t asnt_bs_id = 
{
    ASNC_VAR_STR,
    ASNT_BS_ID,
    ASNL_BS_ID,
    sizeof(asnv_bs_id_t),
    { NULL }
};

asnt_desc_t asnt_bs_info = 
{
    ASNC_COMPOUND,
    ASNT_BS_INFO,
    ASNL_BS_INFO,
    sizeof(asnv_bs_info_t),
    {
        &asnt_bs_id,
        NULL,
    }
};

asn_msg_desc_t asnt_msg_ms_preattachment_req = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_ms_info,
    &asnt_bs_info,
    NULL,
}};
