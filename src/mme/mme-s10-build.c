/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#include "mme-s10.h"

typedef struct {
    char apn[OGS_MAX_APN_LEN + 1];
    ogs_gtp2_f_teid_t pgw;
    ogs_gtp2_ambr_t ambr;
    ogs_ip_t ue_ip;
    struct {
        ogs_gtp2_f_teid_t sgw, pgw;
        uint8_t qos[GTP2_BEARER_QOS_LEN];
    } bearer[OGS_BEARER_PER_UE];
} pdn_storage_t;

static int set_fteid(ogs_tlv_octet_t *ie, ogs_gtp2_f_teid_t *fteid,
        uint8_t interface, uint32_t teid, ogs_ip_t *ip)
{
    int len;
    memset(fteid, 0, sizeof(*fteid));
    fteid->interface_type = interface;
    fteid->teid = htobe32(teid);
    if (!teid || ogs_gtp2_ip_to_f_teid(ip, fteid, &len) != OGS_OK)
        return OGS_ERROR;
    ie->presence = 1;
    ie->data = fteid;
    ie->len = len;
    return OGS_OK;
}

static int export_pdn(ogs_gtp2_tlv_pdn_connection_t *pdn,
        pdn_storage_t *storage, mme_sess_t *sess)
{
    mme_bearer_t *bearer;
    ogs_session_t *session = sess->session;
    int i = 0;
    if (!session || !session->name || !mme_default_bearer_in_sess(sess))
        return OGS_ERROR;
    pdn->presence = 1;
    pdn->apn.presence = 1;
    pdn->apn.data = storage->apn;
    pdn->apn.len = ogs_fqdn_build(storage->apn,
            session->name, strlen(session->name));
    pdn->linked_eps_bearer_id.presence = 1;
    pdn->linked_eps_bearer_id.u8 = mme_default_bearer_in_sess(sess)->ebi;
    pdn->selection_mode.presence = 1;
    pdn->selection_mode.u8 = OGS_GTP2_SELECTION_MODE_MS_OR_NETWORK_PROVIDED_APN;
    pdn->pdn_type.presence = 1;
    pdn->pdn_type.u8 = sess->paa.session_type;
    if (ogs_paa_to_ip(&sess->paa, &storage->ue_ip) != OGS_OK)
        return OGS_ERROR;
    if (storage->ue_ip.ipv4) {
        pdn->ipv4_address.presence = 1;
        pdn->ipv4_address.data = &storage->ue_ip.addr;
        pdn->ipv4_address.len = OGS_IPV4_LEN;
    }
    if (storage->ue_ip.ipv6) {
        pdn->ipv6_address.presence = 1;
        pdn->ipv6_address.data = storage->ue_ip.addr6;
        pdn->ipv6_address.len = OGS_IPV6_LEN;
    }
    pdn->aggregate_maximum_bit_rate.presence = 1;
    pdn->aggregate_maximum_bit_rate.data = &storage->ambr;
    pdn->aggregate_maximum_bit_rate.len = sizeof(storage->ambr);
    storage->ambr.uplink = htobe32(session->ambr.uplink / 1000);
    storage->ambr.downlink = htobe32(session->ambr.downlink / 1000);
    if (set_fteid(&pdn->pgw_s5_s8_ip_address_for_control_plane_or_pmip,
                &storage->pgw, OGS_GTP2_F_TEID_S5_S8_PGW_GTP_C,
                sess->pgw_s5c_teid, &sess->pgw_s5c_ip) != OGS_OK)
        return OGS_ERROR;
    ogs_list_for_each(&sess->bearer_list, bearer) {
        ogs_gtp2_tlv_eps_pdn_bearer_context_t *bc;
        ogs_gtp2_bearer_qos_t qos = {0};
        if (i >= OGS_BEARER_PER_UE) return OGS_ERROR;
        bc = &pdn->bearer_contexts[i];
        bc->presence = 1;
        bc->eps_bearer_id.presence = 1;
        bc->eps_bearer_id.u8 = bearer->ebi;
        /* TS 29.274 7.3.6: SGW S1-U instance 0, PGW S5/S8-U instance 1. */
        if (set_fteid(&bc->s1_u_sgw_f_teid, &storage->bearer[i].sgw,
                    OGS_GTP2_F_TEID_S1_U_SGW_GTP_U,
                    bearer->sgw_s1u_teid, &bearer->sgw_s1u_ip) != OGS_OK ||
                set_fteid(&bc->s5_s8_u_pgw_f_teid, &storage->bearer[i].pgw,
                    OGS_GTP2_F_TEID_S5_S8_PGW_GTP_U,
                    bearer->pgw_s5u_teid, &bearer->pgw_s5u_ip) != OGS_OK)
            return OGS_ERROR;
        const ogs_qos_t *bqos = bearer->qos.index ? &bearer->qos : &session->qos;
        qos.qci = bqos->index;
        qos.priority_level = bqos->arp.priority_level;
        qos.pre_emption_capability = bqos->arp.pre_emption_capability;
        qos.pre_emption_vulnerability = bqos->arp.pre_emption_vulnerability;
        qos.ul_mbr = bqos->mbr.uplink;
        qos.dl_mbr = bqos->mbr.downlink;
        qos.ul_gbr = bqos->gbr.uplink;
        qos.dl_gbr = bqos->gbr.downlink;
        if (ogs_gtp2_build_bearer_qos(&bc->bearer_level_qos,
                    &qos, storage->bearer[i].qos,
                    sizeof(storage->bearer[i].qos)) != GTP2_BEARER_QOS_LEN)
            return OGS_ERROR;
        bc->bearer_level_qos.presence = 1;
        if (bearer->tft.len) bc->tft = bearer->tft;
        i++;
    }
    return i ? OGS_OK : OGS_ERROR;
}

int mme_s10_export_mm_context(mme_ue_t *ue, ogs_tlv_octet_t *ie,
        void *buffer, size_t size)
{
    ogs_gtp2_eps_mm_context_t mm = {0};
    mm.ksi = ue->nas_eps.mme.ksi;
    mm.integrity_algorithm = ue->selected_int_algorithm;
    mm.cipher_algorithm = ue->selected_enc_algorithm;
    mm.dl_count = ue->dl_count;
    mm.ul_count = ue->ul_count.i32;
    memcpy(mm.kasme, ue->kasme, sizeof(mm.kasme));
    /* MM Context ME Identity uses the TS 29.060 7.7.53 mobile identity
     * value, including the identity type, rather than the standalone MEI IE. */
    if (ue->nas_mobile_identity_imeisv.type == OGS_NAS_MOBILE_IDENTITY_IMEISV &&
            ue->imeisv_len == OGS_MAX_IMEISV_LEN) {
        mm.mei.data = &ue->nas_mobile_identity_imeisv;
        mm.mei.len = sizeof(ue->nas_mobile_identity_imeisv);
    } else if (ue->nas_mobile_identity_imeisv.type == OGS_NAS_MOBILE_IDENTITY_IMEI) {
        mm.mei.data = &ue->nas_mobile_identity_imeisv;
        mm.mei.len = sizeof(ogs_nas_mobile_identity_imei_t);
    }
    mm.nh_present = true;
    memcpy(mm.nh, ue->nh, sizeof(mm.nh));
    mm.ncc = ue->nhcc;
    mm.subscribed_ambr_present = true;
    mm.subscribed_ambr_ul = ue->ambr.uplink / 1000;
    mm.subscribed_ambr_dl = ue->ambr.downlink / 1000;
    mm.ue_network_capability.data = (uint8_t *)&ue->ue_network_capability + 1;
    mm.ue_network_capability.len = ue->ue_network_capability.length;
    mm.ms_network_capability.data = (uint8_t *)&ue->ms_network_capability + 1;
    mm.ms_network_capability.len = ue->ms_network_capability.length;
    return ogs_gtp2_build_eps_mm_context(ie, &mm, buffer, size);
}

ogs_pkbuf_t *mme_s10_build_context_response(mme_ue_t *ue, uint8_t cause_value)
{
    ogs_gtp2_message_t *message = ogs_calloc(1, sizeof(*message));
    ogs_gtp2_context_response_t *rsp;
    pdn_storage_t *storage = NULL;
    ogs_gtp2_cause_t cause = {0};
    ogs_gtp2_f_teid_t sender = {0}, sgw = {0};
    ogs_pkbuf_t *pkbuf;
    mme_sess_t *sess;
    sgw_ue_t *sgw_ue;
    uint8_t context[2048];
    int i = 0, len;
    ogs_assert(message);
    message->h.type = OGS_GTP2_CONTEXT_RESPONSE_TYPE;
    rsp = &message->context_response;
    cause.value = cause_value;
    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);
    if (cause_value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) goto build;
    ogs_assert(ue);
    rsp->imsi.presence = 1;
    rsp->imsi.data = ue->imsi;
    rsp->imsi.len = ue->imsi_len;
    if (mme_s10_export_mm_context(ue, &rsp->mme_sgsn_amf_ue_mm_context,
                context, sizeof(context)) != OGS_OK) goto fail;
    sender.interface_type = OGS_GTP2_F_TEID_S10_MME_GTP_C;
    sender.teid = htobe32(ue->mme_s10_teid);
    if (ogs_gtp2_sockaddr_to_f_teid(ogs_gtp_self()->gtpc_addr,
                ogs_gtp_self()->gtpc_addr6, &sender, &len) != OGS_OK)
        goto fail;
    rsp->sender_f_teid_for_control_plane.presence = 1;
    rsp->sender_f_teid_for_control_plane.data = &sender;
    rsp->sender_f_teid_for_control_plane.len = len;
    sgw_ue = sgw_ue_find_by_id(ue->sgw_ue_id);
    if (!sgw_ue || !sgw_ue->gnode || !sgw_ue->sgw_s11_teid) goto fail;
    sgw.interface_type = OGS_GTP2_F_TEID_S11_S4_SGW_GTP_C;
    sgw.teid = htobe32(sgw_ue->sgw_s11_teid);
    if (ogs_gtp2_sockaddr_to_f_teid(
                sgw_ue->gnode->addr.ogs_sa_family == AF_INET ?
                    &sgw_ue->gnode->addr : NULL,
                sgw_ue->gnode->addr.ogs_sa_family == AF_INET6 ?
                    &sgw_ue->gnode->addr : NULL, &sgw, &len) != OGS_OK) goto fail;
    rsp->sgw_s11_s4_ip_address_and_teid_for_control_plane.presence = 1;
    rsp->sgw_s11_s4_ip_address_and_teid_for_control_plane.data = &sgw;
    rsp->sgw_s11_s4_ip_address_and_teid_for_control_plane.len = len;
    storage = ogs_calloc(OGS_MAX_NUM_OF_SESS, sizeof(*storage));
    ogs_assert(storage);
    ogs_list_for_each(&ue->sess_list, sess) {
        if (i >= OGS_MAX_NUM_OF_SESS ||
                export_pdn(&rsp->mme_sgsn_amf_ue_eps_pdn_connections[i],
                    &storage[i], sess) != OGS_OK) goto fail;
        i++;
    }
    if (!i) goto fail;
build:
    pkbuf = ogs_gtp2_build_msg(message);
    if (storage) ogs_free(storage);
    ogs_free(message);
    return pkbuf;
fail:
    if (storage) ogs_free(storage);
    ogs_free(message);
    return NULL;
}
