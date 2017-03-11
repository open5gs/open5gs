typedef struct _asnv_msg_ms_preattachment_req {
    asnv_ms_info_t ms_info;
    asnv_bs_info_t bs_info;
} asnv_msg_ms_preattachment_req;

extern asn_msg_desc_t asnt_msg_ms_preattachment_req;

typedef struct _asnv_msg_ms_preattachment_rsp {
    asnv_failure_indication_t failure_indication; 
    asnv_ms_info_t ms_info;
    asnv_ms_security_history_t ms_security_history;
    asnv_bs_info_t bs_info;
} asnv_msg_ms_preattachment_rsp;

extern asn_msg_desc_t asnt_msg_ms_preattachment_rsp;

typedef struct _asnv_msg_ms_attachment_req {
    asnv_ms_info_t ms_info;
    asnv_bs_info_t bs_info;
} asnv_msg_ms_attachment_req;

extern asn_msg_desc_t asnt_msg_ms_attachment_req;

typedef struct _asnv_msg_ms_attachment_rsp {
    asnv_failure_indication_t failure_indication; 
    asnv_ms_info_t ms_info;
    asnv_bs_info_t bs_info;
} asnv_msg_ms_attachment_rsp;

extern asn_msg_desc_t asnt_msg_ms_attachment_rsp;

typedef struct _asnv_msg_path_reg_req {
    asnv_registration_type_t registration_type;
    asnv_ms_info_t ms_info;
    asnv_bs_info_t bs_info;
} asnv_msg_path_reg_req;

extern asn_msg_desc_t asnt_msg_path_reg_req;

typedef struct _asnv_msg_path_reg_rsp {
    asnv_failure_indication_t failure_indication; 
    asnv_registration_type_t registration_type;
    asnv_ms_info_t ms_info;
    asnv_bs_info_t bs_info;
} asnv_msg_path_reg_rsp;

extern asn_msg_desc_t asnt_msg_path_reg_rsp;

typedef struct _asnv_msg_path_reg_ack {
    asnv_failure_indication_t failure_indication; 
    asnv_bs_info_t bs_info;
} asnv_msg_path_reg_ack;

extern asn_msg_desc_t asnt_msg_path_reg_ack;

typedef struct _asnv_msg_rr_req {
    asnv_ms_info_t ms_info;
    asnv_bs_info_t bs_info;
} asnv_msg_rr_req;

extern asn_msg_desc_t asnt_msg_rr_req;

typedef struct _asnv_msg_rr_rsp {
    asnv_failure_indication_t failure_indication; 
    asnv_ms_info_t ms_info;
} asnv_msg_rr_rsp;

extern asn_msg_desc_t asnt_msg_rr_rsp;

typedef struct _asnv_msg_rr_ack {
    asnv_failure_indication_t failure_indication; 
    asnv_ms_info_t ms_info;
    asnv_bs_info_t bs_info;
} asnv_msg_rr_ack;

extern asn_msg_desc_t asnt_msg_rr_ack;

typedef struct _asnv_msg_fa_reg_req {
    asnv_rrq_t rrq;
#if 0   /* FIXME: */
    asnv_fa_ha_key_t fa_ha_key;
    asnv_fa_ha_key_lifetime_t fa_ha_lifetime;
    asnv_fa_ha_key_spi_t fa_ha_spi;
#endif
} asnv_msg_fa_reg_req;

extern asn_msg_desc_t asnt_msg_fa_reg_req;

typedef struct _asnv_msg_fa_reg_rsp {
    asnv_rrp_t rrp;
} asnv_msg_fa_reg_rsp;

extern asn_msg_desc_t asnt_msg_fa_reg_rsp;

typedef struct _asnv_msg_key_change_directive {
    asnv_bs_info_t bs_info;
    asnv_authentication_complete_t authentication_complete;
} asnv_msg_key_change_directive;

extern asn_msg_desc_t asnt_msg_key_change_directive;

typedef struct _asnv_msg_key_change_cnf {
    asnv_failure_indication_t failure_indication; 
    asnv_ms_info_t ms_info;
    asnv_bs_info_t bs_info;
} asnv_msg_key_change_cnf;

extern asn_msg_desc_t asnt_msg_key_change_cnf;

typedef struct _asnv_msg_key_change_ack {
    asnv_failure_indication_t failure_indication;
} asnv_msg_key_change_ack;

extern asn_msg_desc_t asnt_msg_key_change_ack;
