#include "asn_tlv_desc.h"

tlv_desc_t tlv_desc_authorization_policy_support = 
{
    TLV_UINT8,
    TLV_AUTHORIZATION_POLICY_SUPPORT_TYPE,
    TLV_AUTHORIZATION_POLICY_SUPPORT_LEN,
    sizeof(tlv_authorization_policy_support_t),
    { NULL }
};

tlv_desc_t tlv_desc_ms_security_history = 
{
    TLV_COMPOUND,
    TLV_MS_SECURITY_HISTORY_TYPE,
    TLV_MS_SECURITY_HISTORY_LEN,
    sizeof(tlv_ms_security_history_t),
    {
        &tlv_desc_authorization_policy_support,
        NULL,
    }
};

tlv_desc_t tlv_desc_ms_info = 
{
    TLV_COMPOUND,
    TLV_MS_INFO_TYPE,
    TLV_MS_INFO_LEN,
    sizeof(tlv_ms_info_t),
    {
        &tlv_desc_ms_security_history,
        NULL,
    }
};

tlv_desc_t tlv_desc_bs_id = 
{
    TLV_VAR_STR,
    TLV_BS_ID_TYPE,
    TLV_BS_ID_LEN,
    sizeof(tlv_bs_id_t),
    { NULL }
};

tlv_desc_t tlv_desc_bs_info = 
{
    TLV_COMPOUND,
    TLV_BS_INFO_TYPE,
    TLV_BS_INFO_LEN,
    sizeof(tlv_bs_info_t),
    {
        &tlv_desc_bs_id,
        NULL,
    }
};

tlv_desc_t tlv_desc_msg_ms_preattachment_req = {
    TLV_MESSAGE, 0, 0, 0, {
    &tlv_desc_ms_info,
    &tlv_desc_bs_info,
    NULL,
}};
