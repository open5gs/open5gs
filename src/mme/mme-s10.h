/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#ifndef MME_S10_H
#define MME_S10_H
#include "mme-context.h"

typedef void (*mme_s10_sgw_ready_t)(mme_ue_t *, int);
bool mme_s10_return_tau(mme_ue_t *ue);
int mme_s10_sgw_prepare(mme_ue_t *ue, mme_s10_sgw_ready_t ready);
bool mme_s10_sgw_relocated(const mme_ue_t *ue);
void mme_s10_sgw_clear(mme_ue_t *ue);
void mme_s10_sgw_committed(mme_ue_t *ue);
void mme_s10_sgw_delete_old(mme_ue_t *ue);
void mme_s10_sgw_create_response(ogs_gtp_xact_t *xact, mme_ue_t *ue,
        ogs_gtp2_message_t *message);
/* S10 uses the existing GTP-C socket. Peers are explicitly provisioned. */
typedef struct mme_s10_peer_s {
    ogs_gtp_node_t gnode;
    ogs_gtp_node_t *node; /* Shared by all GUMMEI routes to one endpoint. */
    ogs_plmn_id_t plmn_id;
    uint16_t mme_gid;
    uint8_t mme_code;
    int num_tai;
    ogs_eps_tai_t tai[OGS_MAX_NUM_OF_SUPPORTED_TA];
} mme_s10_peer_t;

int mme_s10_configuration_transfer(const S1AP_SONConfigurationTransfer_t *son);
void mme_s10_configuration_receive(const ogs_gtp2_message_t *msg);

/* Connected S1 mobility. The handover state owns all asynchronous snapshots. */
void mme_s10_handover_clear(mme_ue_t *ue);
void mme_s10_handover_tick(ogs_pool_id_t ue_id);
int mme_s10_handover_required(enb_ue_t *source,
        const S1AP_TargeteNB_ID_t *target, const S1AP_Cause_t *cause,
        const OCTET_STRING_t *container, bool direct);
bool mme_s10_source_committed(const mme_ue_t *ue);
bool mme_s10_handover_bearer_rejected(const mme_ue_t *ue, uint8_t ebi);
bool mme_s10_handover_busy(const mme_ue_t *ue);
bool mme_s10_handover_queue_downlink(enb_ue_t *enb, ogs_pkbuf_t *p);
bool mme_s10_handover_direct(const mme_ue_t *ue);
bool mme_s10_handover_is_target(const mme_ue_t *ue);
int mme_s10_handover_acknowledge(enb_ue_t *target);
void mme_s10_handover_failure(enb_ue_t *target, const S1AP_Cause_t *cause);
void mme_s10_handover_bind_indirect(mme_ue_t *ue, ogs_gtp_xact_t *xact);
bool mme_s10_handover_indirect_response(ogs_gtp_xact_t *xact,
        const ogs_gtp2_message_t *msg);
void mme_s10_handover_source_released(mme_ue_t *ue, ogs_pool_id_t enb_ue_id);
void mme_s10_handover_cleanup_response(ogs_gtp_xact_t *xact,
        const ogs_gtp2_message_t *msg);
int mme_s10_handover_status(enb_ue_t *source,
        const S1AP_ENB_StatusTransfer_TransparentContainer_t *container);
int mme_s10_handover_cancel(enb_ue_t *source);
int mme_s10_handover_notify(enb_ue_t *target);
void mme_s10_handover_sgw_response(ogs_gtp_xact_t *xact, mme_ue_t *ue,
        ogs_gtp2_modify_bearer_response_t *rsp);
bool mme_s10_handover_tau(mme_ue_t *ue,
        const ogs_nas_eps_tracking_area_update_request_t *req);
void mme_s10_handover_receive(ogs_gtp_xact_t *xact, ogs_gtp2_message_t *msg);
bool mme_s10_security_context_usable(const mme_ue_t *ue,
        const ogs_gtp2_eps_mm_context_t *mm);
typedef struct {
    ogs_plmn_id_t enb_plmn;
    ogs_eps_tai_t tai;
    uint32_t enb_id;
    S1AP_ENB_ID_PR enb_type;
} mme_s10_target_t;
int mme_s10_encode_selected_plmn(ogs_tlv_octet_t *ie,
        const ogs_plmn_id_t *plmn, uint8_t buffer[3]);
int mme_s10_decode_selected_plmn(ogs_plmn_id_t *plmn,
        const ogs_tlv_octet_t *ie);
int mme_s10_encode_target(ogs_tlv_octet_t *ie,
        const S1AP_TargeteNB_ID_t *target, uint8_t buffer[10]);
int mme_s10_decode_target(mme_s10_target_t *target, const ogs_tlv_octet_t *ie);
int mme_s10_encode_cause(ogs_tlv_octet_t *ie,
        const S1AP_Cause_t *cause, uint8_t buffer[2]);
int mme_s10_decode_cause(S1AP_Cause_t *cause, const ogs_tlv_octet_t *ie);
int mme_s10_import_handover_context(mme_ue_t *ue,
        const ogs_gtp2_forward_relocation_request_t *req);
int mme_s10_import_context(mme_ue_t *ue, ogs_gtp2_context_response_t *rsp,
        bool keep_security);
int mme_s10_export_mm_context(mme_ue_t *ue, ogs_tlv_octet_t *ie,
        void *buffer, size_t size);
ogs_pkbuf_t *mme_s10_build_forward_relocation_request(mme_ue_t *ue,
        const S1AP_TargeteNB_ID_t *target, const S1AP_Cause_t *cause,
        const OCTET_STRING_t *container, bool direct_forwarding);
ogs_pkbuf_t *mme_s10_build_context_response(mme_ue_t *ue, uint8_t cause);
bool mme_s10_imsi_available(mme_ue_t *ue, const char *imsi);
void mme_s10_init(void);
void mme_s10_final(void);
int mme_s10_parse_peer(ogs_yaml_iter_t *parent);
int mme_s10_open(void);
mme_s10_peer_t *mme_s10_find_by_addr(const ogs_sockaddr_t *addr);
mme_s10_peer_t *mme_s10_find_by_guti(const ogs_nas_eps_guti_t *guti);
mme_s10_peer_t *mme_s10_find_by_tai(const ogs_eps_tai_t *tai);
bool mme_s10_message_type(uint8_t type);
void mme_s10_holding_expired(ogs_pool_id_t ue_id);
bool mme_s10_cancel_location(mme_ue_t *ue);
void mme_s10_receive(ogs_gtp_node_t *node, ogs_pkbuf_t *pkbuf);
int mme_s10_tau_request(mme_ue_t *ue, ogs_pkbuf_t *plain, uint8_t nas_type);
int mme_s10_authentication_complete(mme_ue_t *ue);
void mme_s10_sgw_response(ogs_gtp_xact_t *xact, mme_ue_t *ue,
        ogs_gtp2_modify_bearer_response_t *rsp);
int mme_s10_identify(mme_ue_t *ue,
        const ogs_nas_eps_attach_request_t *attach, ogs_pkbuf_t *plain,
        uint8_t nas_type);
/* request: Complete Request Message discriminator (0 Attach, 1 TAU). */
int mme_s10_verify_complete_request(mme_ue_t *ue, const ogs_tlv_octet_t *ie,
        uint8_t request, bool *verified);
void mme_s10_ue_clear(mme_ue_t *ue);
#endif
