#include "asn_tlv_desc.h"

asn_desc_t asn_desc_authorization_policy_support = 
{
    TLV_UINT8,
    TLV_AUTHORIZATION_POLICY_SUPPORT_TYPE,
    TLV_AUTHORIZATION_POLICY_SUPPORT_LEN,
    sizeof(asn_authorization_policy_support_t),
    { NULL }
};

asn_desc_t asn_desc_ms_security_history = 
{
    TLV_COMPOUND,
    TLV_MS_SECURITY_HISTORY_TYPE,
    TLV_MS_SECURITY_HISTORY_LEN,
    sizeof(asn_ms_security_history_t),
    {
        &asn_desc_authorization_policy_support,
        NULL,
    }
};

asn_desc_t asn_desc_ms_info = 
{
    TLV_COMPOUND,
    TLV_MS_INFO_TYPE,
    TLV_MS_INFO_LEN,
    sizeof(asn_ms_info_t),
    {
        &asn_desc_ms_security_history,
        NULL,
    }
};

asn_desc_t asn_desc_bs_id = 
{
    TLV_VAR_STR,
    TLV_BS_ID_TYPE,
    TLV_BS_ID_LEN,
    sizeof(asn_bs_id_t),
    { NULL }
};

asn_desc_t asn_desc_bs_info = 
{
    TLV_COMPOUND,
    TLV_BS_INFO_TYPE,
    TLV_BS_INFO_LEN,
    sizeof(asn_bs_info_t),
    {
        &asn_desc_bs_id,
        NULL,
    }
};

asn_desc_t asn_desc_msg_ms_preattachment_req = {
    TLV_MESSAGE, 0, 0, 0, {
    &asn_desc_ms_info,
    &asn_desc_bs_info,
    NULL,
}};
