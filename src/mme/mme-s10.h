/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#ifndef MME_S10_H
#define MME_S10_H
#include "mme-context.h"

/* S10 uses the existing GTP-C socket. Peers are explicitly provisioned. */
typedef struct mme_s10_peer_s {
    ogs_gtp_node_t gnode;
    ogs_gtp_node_t *node; /* Shared by all GUMMEI routes to one endpoint. */
    ogs_plmn_id_t plmn_id;
    uint16_t mme_gid;
    uint8_t mme_code;
} mme_s10_peer_t;

bool mme_s10_security_context_usable(const mme_ue_t *ue,
        const ogs_gtp2_eps_mm_context_t *mm);
int mme_s10_import_context(mme_ue_t *ue, ogs_gtp2_context_response_t *rsp,
        bool keep_security);
int mme_s10_export_mm_context(mme_ue_t *ue, ogs_tlv_octet_t *ie,
        void *buffer, size_t size);
ogs_pkbuf_t *mme_s10_build_context_response(mme_ue_t *ue, uint8_t cause);
bool mme_s10_imsi_available(mme_ue_t *ue, const char *imsi);
void mme_s10_init(void);
void mme_s10_final(void);
int mme_s10_parse_peer(ogs_yaml_iter_t *parent);
int mme_s10_open(void);
mme_s10_peer_t *mme_s10_find_by_addr(const ogs_sockaddr_t *addr);
mme_s10_peer_t *mme_s10_find_by_guti(const ogs_nas_eps_guti_t *guti);
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
