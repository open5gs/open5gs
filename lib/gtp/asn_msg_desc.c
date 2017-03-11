asn_msg_desc_t asnt_msg_ms_preattachment_req = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_ms_info,
    &asnt_bs_info,
    NULL,
}};

asn_msg_desc_t asnt_msg_ms_preattachment_rsp = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_failure_indication,
    &asnt_ms_info,
    &asnt_ms_security_history,
    &asnt_bs_info,
    NULL,
}};

asn_msg_desc_t asnt_msg_ms_attachment_req = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_ms_info,
    &asnt_bs_info,
    NULL,
}};

asn_msg_desc_t asnt_msg_ms_attachment_rsp = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_failure_indication,
    &asnt_ms_info,
    &asnt_bs_info,
    NULL,
}};

asn_msg_desc_t asnt_msg_path_reg_req = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_registration_type,
    &asnt_ms_info,
    &asnt_bs_info,
    NULL,
}};

asn_msg_desc_t asnt_msg_path_reg_rsp = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_failure_indication,
    &asnt_registration_type,
    &asnt_ms_info,
    &asnt_bs_info,
    NULL,
}};

asn_msg_desc_t asnt_msg_path_reg_ack = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_failure_indication,
    &asnt_bs_info,
    NULL,
}};

asn_msg_desc_t asnt_msg_rr_req = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_ms_info,
    &asnt_bs_info,
    NULL,
}};

asn_msg_desc_t asnt_msg_rr_rsp = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_failure_indication,
    &asnt_ms_info,
    NULL,
}};

asn_msg_desc_t asnt_msg_rr_ack = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_failure_indication,
    &asnt_ms_info,
    &asnt_bs_info,
    NULL,
}};

asn_msg_desc_t asnt_msg_fa_reg_req = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_rrq,
    NULL,
}};

asn_msg_desc_t asnt_msg_fa_reg_rsp = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_rrp,
    NULL,
}};

asn_msg_desc_t asnt_msg_key_change_directive = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_bs_info,
    &asnt_authentication_complete,
    NULL,
}};

asn_msg_desc_t asnt_msg_key_change_cnf = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_failure_indication,
    &asnt_ms_info,
    &asnt_bs_info,
    NULL,
}};

asn_msg_desc_t asnt_msg_key_change_ack = {
    ASNC_MESSAGE, 0, 0, 0, {
    &asnt_failure_indication,
    NULL,
}};
