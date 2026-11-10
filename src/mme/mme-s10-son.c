/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#include "mme-s10.h"
#include "s1ap-path.h"

int mme_s10_configuration_transfer(const S1AP_SONConfigurationTransfer_t *son)
{
    S1AP_SONConfigurationTransfer_t copy = {0};
    mme_s10_target_t target;
    mme_s10_peer_t *peer;
    ogs_gtp2_message_t *msg;
    ogs_gtp2_configuration_transfer_tunnel_t *req;
    ogs_tlv_octet_t target_ie = {0};
    ogs_gtp2_header_t *h;
    ogs_pkbuf_t *encoded, *body;
    uint8_t target_buffer[10], *container;
    uint32_t sequence;
    int rv;
    if (!son || mme_s10_encode_target(&target_ie, son->targeteNB_ID,
                target_buffer) != OGS_OK ||
            mme_s10_decode_target(&target, &target_ie) != OGS_OK ||
            son->targeteNB_ID->selected_TAI->pLMNidentity.size !=
                sizeof(target.tai.plmn_id) ||
            !son->targeteNB_ID->selected_TAI->pLMNidentity.buf) return OGS_ERROR;
    memcpy(&target.tai.plmn_id,
            son->targeteNB_ID->selected_TAI->pLMNidentity.buf, sizeof(target.tai.plmn_id));
    peer = mme_s10_find_by_tai(&target.tai);
    if (!peer) return OGS_NOTFOUND;
    if (ogs_asn_copy_ie(&asn_DEF_S1AP_SONConfigurationTransfer,
                (void *)son, &copy) != OGS_OK) return OGS_ERROR;
    encoded = ogs_asn_encode(&asn_DEF_S1AP_SONConfigurationTransfer, &copy);
    if (!encoded) return OGS_ERROR;
    if (encoded->len > OGS_MAX_SDU_LEN / 2) {
        ogs_pkbuf_free(encoded);
        return OGS_ERROR;
    }
    msg = ogs_calloc(1, sizeof(*msg));
    container = ogs_malloc(encoded->len + 1);
    ogs_assert(msg && container);
    req = &msg->configuration_transfer_tunnel;
    req->target_enodeb_id_en_gnb_id_gnb_id = target_ie;
    container[0] = 3;
    memcpy(container + 1, encoded->data, encoded->len);
    req->e_utran_transparent_container_en_dc_container_inter_system_son_container.presence = 1;
    req->e_utran_transparent_container_en_dc_container_inter_system_son_container.data = container;
    req->e_utran_transparent_container_en_dc_container_inter_system_son_container.len = encoded->len + 1;
    msg->h.type = OGS_GTP2_CONFIGURATION_TRANSFER_TUNNEL_TYPE;
    body = ogs_gtp2_build_msg(msg);
    ogs_free(container);
    ogs_free(msg);
    ogs_pkbuf_free(encoded);
    if (!body) return OGS_ERROR;
    /* TS 29.274 7.3.18: one-way signaling, not a request/response GTP
     * transaction. TEID zero and an independent 24-bit sequence number. */
    ogs_random(&sequence, sizeof(sequence));
    h = ogs_pkbuf_push(body, OGS_GTPV2C_HEADER_LEN);
    memset(h, 0, OGS_GTPV2C_HEADER_LEN);
    h->flags = 0x48;
    h->type = OGS_GTP2_CONFIGURATION_TRANSFER_TUNNEL_TYPE;
    h->length = htobe16(body->len - 4);
    h->sqn = OGS_GTP2_XID_TO_SQN(sequence & 0xffffff);
    rv = ogs_gtp_sendto(peer->node, body);
    ogs_pkbuf_free(body);
    return rv;
}

void mme_s10_configuration_receive(const ogs_gtp2_message_t *msg)
{
    const ogs_gtp2_configuration_transfer_tunnel_t *req = &msg->configuration_transfer_tunnel;
    const ogs_tlv_octet_t *ie =
        &req->e_utran_transparent_container_en_dc_container_inter_system_son_container;
    S1AP_SONConfigurationTransfer_t son = {0};
    mme_s10_target_t target, embedded;
    ogs_tlv_octet_t target_ie = {0};
    uint8_t target_buffer[10];
    ogs_pkbuf_t *p;
    mme_enb_t *enb;
    if (msg->h.teid || !ie->presence || !ie->data || ie->len < 2 ||
            ie->len > OGS_MAX_SDU_LEN / 2 || (*(uint8_t *)ie->data & 15) != 3 ||
            mme_s10_decode_target(&target,
                &req->target_enodeb_id_en_gnb_id_gnb_id) != OGS_OK) return;
    p = ogs_pkbuf_alloc(NULL, ie->len - 1);
    ogs_assert(p);
    ogs_pkbuf_put_data(p, (uint8_t *)ie->data + 1, ie->len - 1);
    if (ogs_asn_decode(&asn_DEF_S1AP_SONConfigurationTransfer,
                &son, sizeof(son), p) != OGS_OK ||
            mme_s10_encode_target(&target_ie, son.targeteNB_ID, target_buffer) != OGS_OK ||
            mme_s10_decode_target(&embedded, &target_ie) != OGS_OK ||
            target.enb_type != embedded.enb_type || target.enb_id != embedded.enb_id ||
            target.tai.tac != embedded.tai.tac ||
            memcmp(&target.enb_plmn, &embedded.enb_plmn, sizeof(target.enb_plmn))) goto out;
    enb = mme_enb_find_by_enb_id(target.enb_id);
    if (enb && enb->state.s1_setup_success &&
            !memcmp(&enb->plmn_id, &target.enb_plmn, sizeof(target.enb_plmn)))
        s1ap_send_mme_configuration_transfer(enb, &son);
out:
    ogs_asn_free(&asn_DEF_S1AP_SONConfigurationTransfer, &son);
    ogs_pkbuf_free(p);
}
