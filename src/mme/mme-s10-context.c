/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#include "mme-s10.h"
#include "mme-sm.h"
#include "mme-as-security.h"

typedef struct {
    uint8_t ebi;
    uint32_t sgw_teid, pgw_teid;
    ogs_ip_t sgw_ip, pgw_ip;
    ogs_qos_t qos;
    ogs_tlv_octet_t tft;
} imported_bearer_t;

typedef struct {
    ogs_session_t profile;
    char apn[OGS_MAX_APN_LEN + 1];
    ogs_paa_t paa;
    uint32_t pgw_teid;
    ogs_ip_t pgw_ip;
    uint8_t linked_ebi;
    int num_bearers;
    imported_bearer_t bearer[OGS_BEARER_PER_UE];
} imported_pdn_t;

static int parse_fteid(const ogs_tlv_octet_t *ie, uint8_t type,
        ogs_ip_t *ip, uint32_t *teid)
{
    ogs_gtp2_f_teid_t *f = ie->data;
    size_t len;
    if (!ie->presence || !f || ie->len < 5) return OGS_ERROR;
    len = 5 + (f->ipv4 ? OGS_IPV4_LEN : 0) + (f->ipv6 ? OGS_IPV6_LEN : 0);
    if (len != ie->len || f->interface_type != type || !f->teid ||
            (!f->ipv4 && !f->ipv6)) return OGS_ERROR;
    *teid = be32toh(f->teid);
    return ogs_gtp2_f_teid_to_ip(f, ip);
}

static int invalid_pdn(int line)
{
    ogs_warn("Invalid EPS PDN context [validation:%d]", line);
    return OGS_ERROR;
}

static int parse_pdn(imported_pdn_t *out,
        const ogs_gtp2_tlv_pdn_connection_t *pdn, uint16_t *seen)
{
    ogs_gtp2_ambr_t ambr;
    int i, default_index = -1;
    if (!pdn->apn.presence || !pdn->apn.data || !pdn->apn.len ||
            pdn->apn.len > OGS_MAX_APN_LEN ||
            !ogs_fqdn_parse(out->apn, pdn->apn.data, pdn->apn.len) ||
            !pdn->linked_eps_bearer_id.presence ||
            !pdn->aggregate_maximum_bit_rate.presence ||
            pdn->aggregate_maximum_bit_rate.len != sizeof(ambr) ||
            !pdn->pdn_type.presence) return invalid_pdn(__LINE__);
    out->profile.name = out->apn;
    out->profile.session_type = pdn->pdn_type.u8;
    out->paa.session_type = pdn->pdn_type.u8;
    out->linked_ebi = pdn->linked_eps_bearer_id.u8;
    memcpy(&ambr, pdn->aggregate_maximum_bit_rate.data, sizeof(ambr));
    out->profile.ambr.uplink = (uint64_t)be32toh(ambr.uplink) * 1000;
    out->profile.ambr.downlink = (uint64_t)be32toh(ambr.downlink) * 1000;
    if (pdn->pdn_type.u8 < OGS_PDU_SESSION_TYPE_IPV4 ||
            pdn->pdn_type.u8 > OGS_PDU_SESSION_TYPE_IPV4V6) return invalid_pdn(__LINE__);
    if (pdn->pdn_type.u8 != OGS_PDU_SESSION_TYPE_IPV6) {
        if (!pdn->ipv4_address.presence ||
                pdn->ipv4_address.len != OGS_IPV4_LEN) return invalid_pdn(__LINE__);
        out->profile.ue_ip.ipv4 = 1;
        memcpy(&out->profile.ue_ip.addr, pdn->ipv4_address.data, OGS_IPV4_LEN);
    }
    if (pdn->pdn_type.u8 != OGS_PDU_SESSION_TYPE_IPV4) {
        if (!pdn->ipv6_address.presence ||
                pdn->ipv6_address.len != OGS_IPV6_LEN) return invalid_pdn(__LINE__);
        out->profile.ue_ip.ipv6 = 1;
        memcpy(out->profile.ue_ip.addr6, pdn->ipv6_address.data, OGS_IPV6_LEN);
    }
    if (pdn->pdn_type.u8 == OGS_PDU_SESSION_TYPE_IPV4) {
        out->paa.addr = out->profile.ue_ip.addr;
    } else if (pdn->pdn_type.u8 == OGS_PDU_SESSION_TYPE_IPV6) {
        out->paa.len = 64;
        memcpy(out->paa.addr6, out->profile.ue_ip.addr6, OGS_IPV6_LEN);
    } else {
        out->paa.both.len = 64;
        memcpy(out->paa.both.addr6, out->profile.ue_ip.addr6, OGS_IPV6_LEN);
        out->paa.both.addr = out->profile.ue_ip.addr;
    }
    if (parse_fteid(&pdn->pgw_s5_s8_ip_address_for_control_plane_or_pmip,
                OGS_GTP2_F_TEID_S5_S8_PGW_GTP_C, &out->pgw_ip,
                &out->pgw_teid) != OGS_OK) return invalid_pdn(__LINE__);
    for (i = 0; i < OGS_BEARER_PER_UE; i++) {
        const ogs_gtp2_tlv_eps_pdn_bearer_context_t *bc = &pdn->bearer_contexts[i];
        imported_bearer_t *b = &out->bearer[i];
        ogs_gtp2_bearer_qos_t qos;
        uint16_t mask;
        if (!bc->presence) break;
        if (!bc->eps_bearer_id.presence || bc->eps_bearer_id.u8 < 5 ||
                bc->eps_bearer_id.u8 > 15 || !bc->bearer_level_qos.presence ||
                bc->bearer_level_qos.len != GTP2_BEARER_QOS_LEN)
            return invalid_pdn(__LINE__);
        b->ebi = bc->eps_bearer_id.u8;
        mask = (uint16_t)1 << (b->ebi - 5);
        if (*seen & mask) return invalid_pdn(__LINE__);
        *seen |= mask;
        if (parse_fteid(&bc->s1_u_sgw_f_teid,
                    OGS_GTP2_F_TEID_S1_U_SGW_GTP_U,
                    &b->sgw_ip, &b->sgw_teid) != OGS_OK ||
                parse_fteid(&bc->s5_s8_u_pgw_f_teid,
                    OGS_GTP2_F_TEID_S5_S8_PGW_GTP_U,
                    &b->pgw_ip, &b->pgw_teid) != OGS_OK ||
                ogs_gtp2_parse_bearer_qos(&qos,
                    (ogs_tlv_octet_t *)&bc->bearer_level_qos) != GTP2_BEARER_QOS_LEN)
            return invalid_pdn(__LINE__);
        if (!qos.qci || !qos.priority_level) return invalid_pdn(__LINE__);
        b->qos.index = qos.qci;
        b->qos.arp.priority_level = qos.priority_level;
        b->qos.arp.pre_emption_capability = qos.pre_emption_capability;
        b->qos.arp.pre_emption_vulnerability = qos.pre_emption_vulnerability;
        b->qos.mbr.uplink = qos.ul_mbr;
        b->qos.mbr.downlink = qos.dl_mbr;
        b->qos.gbr.uplink = qos.ul_gbr;
        b->qos.gbr.downlink = qos.dl_gbr;
        b->tft = bc->tft;
        if (b->ebi == out->linked_ebi) {
            default_index = i;
            out->profile.qos = b->qos;
        } else if (!bc->tft.presence || !bc->tft.len) return invalid_pdn(__LINE__);
    }
    out->num_bearers = i;
    if (!i || default_index < 0) return invalid_pdn(__LINE__);
    /* The local session list keeps the default bearer first. */
    if (default_index != 0) {
        imported_bearer_t tmp = out->bearer[0];
        out->bearer[0] = out->bearer[default_index];
        out->bearer[default_index] = tmp;
    }
    return OGS_OK;
}

static bool security_algorithms_usable(const ogs_gtp2_eps_mm_context_t *mm)
{
    const uint8_t *cap = mm->ue_network_capability.data;
    bool integrity = false, cipher = false;
    int i;
    if (!mm->integrity_algorithm || mm->integrity_algorithm > 3 ||
            mm->cipher_algorithm > 3 || mm->ue_network_capability.len < 2 ||
            !cap || !(cap[1] & (0x80 >> mm->integrity_algorithm)) ||
            !(cap[0] & (0x80 >> mm->cipher_algorithm))) return false;
    for (i = 0; i < mme_self()->num_of_integrity_order; i++)
        if (mme_self()->integrity_order[i] == mm->integrity_algorithm)
            integrity = true;
    for (i = 0; i < mme_self()->num_of_ciphering_order; i++)
        if (mme_self()->ciphering_order[i] == mm->cipher_algorithm)
            cipher = true;
    return integrity && cipher;
}

bool mme_s10_security_context_usable(const mme_ue_t *ue,
        const ogs_gtp2_eps_mm_context_t *mm)
{
    return mm->ksi == ue->nas_eps.ue.ksi && !ue->nas_eps.ue.tsc &&
        security_algorithms_usable(mm);
}

/* Validate every PDN before creating any local bearer or changing security.
 * Imported TLV views are copied into owned session/TFT storage before return. */
static int import_context(mme_ue_t *ue, ogs_gtp2_context_response_t *rsp,
        bool keep_security, bool handover)
{
    imported_pdn_t *pdns = ogs_calloc(OGS_MAX_NUM_OF_SESS, sizeof(*pdns));
    ogs_gtp2_eps_mm_context_t mm;
    ogs_ip_t sgw_ip;
    ogs_nas_mobile_identity_imeisv_t mei = {0};
    char imeisv_bcd[OGS_MAX_IMEISV_BCD_LEN + 1] = {0};
    ogs_sockaddr_t *addrs = NULL, *addr;
    mme_sgw_t *sgw = NULL;
    sgw_ue_t *sgw_ue;
    uint32_t sgw_teid;
    uint16_t seen = 0;
    int n, i, j, stage = 0, rv = OGS_ERROR;
    const char *stages[] = {"MM context", "SGW endpoint", "PDN connections", "local allocation"};
    ogs_assert(pdns);
    if (ogs_list_count(&ue->sess_list) ||
            !rsp->mme_sgsn_amf_ue_mm_context.presence ||
            ogs_gtp2_parse_eps_mm_context(&mm,
                &rsp->mme_sgsn_amf_ue_mm_context) != OGS_OK ||
            (!handover && mm.old_context_present) ||
            mm.ksi == OGS_NAS_KSI_NO_KEY_IS_AVAILABLE ||
            (handover && (!mm.nh_present ||
                (mm.old_context_present && (!mm.old_nh_present ||
                 mm.old_ksi == OGS_NAS_KSI_NO_KEY_IS_AVAILABLE)))) ||
            mm.ue_network_capability.len > sizeof(ue->ue_network_capability) - 1 ||
            mm.ms_network_capability.len > sizeof(ue->ms_network_capability) - 1 ||
            mm.mei.len > sizeof(mei) ||
            (!keep_security && (handover ? !security_algorithms_usable(&mm) :
                !mme_s10_security_context_usable(ue, &mm)))) goto out;
    if (mm.mei.len) {
        memcpy(&mei, mm.mei.data, mm.mei.len);
        if (mei.type == OGS_NAS_MOBILE_IDENTITY_IMEISV) {
            if (mm.mei.len != sizeof(mei) ||
                    ogs_nas_imeisv_to_bcd(&mei, mm.mei.len, imeisv_bcd) != OGS_OK)
                goto out;
        } else if (mei.type != OGS_NAS_MOBILE_IDENTITY_IMEI ||
                mm.mei.len != sizeof(ogs_nas_mobile_identity_imei_t)) goto out;
    }
    stage = 1;
    if (parse_fteid(&rsp->sgw_s11_s4_ip_address_and_teid_for_control_plane,
                OGS_GTP2_F_TEID_S11_S4_SGW_GTP_C, &sgw_ip, &sgw_teid) != OGS_OK ||
            ogs_ip_to_sockaddr(&sgw_ip, ogs_gtp_self()->gtpc_port, &addrs) != OGS_OK)
        goto out;
    for (addr = addrs; addr; addr = addr->next) {
        sgw = mme_sgw_find_by_addr(addr);
        if (sgw) break;
    }
    if (!sgw) goto out;
    stage = 2;
    for (n = 0; n < OGS_MAX_NUM_OF_SESS; n++) {
        if (!rsp->mme_sgsn_amf_ue_eps_pdn_connections[n].presence) break;
        if (parse_pdn(&pdns[n],
                    &rsp->mme_sgsn_amf_ue_eps_pdn_connections[n], &seen) != OGS_OK)
            goto out;
        for (i = 0; i < n; i++)
            if (!strcmp(pdns[i].apn, pdns[n].apn)) goto out;
    }
    if (!n) goto out;
    stage = 3;
    for (i = 0; i < n; i++) {
        mme_sess_t *sess = mme_sess_add(ue, i + 1);
        if (!sess) goto rollback;
        mme_sess_set_session(sess, &pdns[i].profile);
        sess->paa = pdns[i].paa;
        sess->pgw_s5c_ip = pdns[i].pgw_ip;
        sess->pgw_s5c_teid = pdns[i].pgw_teid;
        for (j = 0; j < pdns[i].num_bearers; j++) {
            imported_bearer_t *in = &pdns[i].bearer[j];
            mme_bearer_t *b = j ? mme_bearer_add(sess) :
                mme_default_bearer_in_sess(sess);
            if (!b) goto rollback;
            if (b->ebi != in->ebi) {
                mme_ebi_free(ue, b->ebi);
                if (mme_ebi_reserve(ue, in->ebi) != OGS_OK) goto rollback;
                b->ebi = in->ebi;
            }
            b->qos = in->qos;
            b->sgw_s1u_ip = in->sgw_ip;
            b->sgw_s1u_teid = in->sgw_teid;
            b->pgw_s5u_ip = in->pgw_ip;
            b->pgw_s5u_teid = in->pgw_teid;
            if (in->tft.len) OGS_TLV_STORE_DATA(&b->tft, &in->tft);
            OGS_FSM_TRAN(&b->sm, &esm_state_active);
        }
    }
    sgw_ue = sgw_ue_find_by_id(ue->sgw_ue_id);
    if (!sgw_ue) goto rollback;
    sgw_ue->gnode = &sgw->gnode;
    sgw_ue->sgw_s11_teid = sgw_teid;
    if (!keep_security) {
        ue->nas_eps.mme.tsc = 0;
        ue->nas_eps.mme.ksi = mm.ksi;
        ue->selected_int_algorithm = mm.integrity_algorithm;
        ue->selected_enc_algorithm = mm.cipher_algorithm;
        memcpy(ue->kasme, mm.kasme, sizeof(ue->kasme));
        ue->ul_count.i32 = mm.ul_count;
        ue->ul_count_accepted = true;
        ue->dl_count = mm.dl_count;
        ogs_kdf_nas_eps(OGS_KDF_NAS_INT_ALG, ue->selected_int_algorithm,
                ue->kasme, ue->knas_int);
        ogs_kdf_nas_eps(OGS_KDF_NAS_ENC_ALG, ue->selected_enc_algorithm,
                ue->kasme, ue->knas_enc);
        ue->security_context_available = 1;
        ue->mac_failed = 0;
        ue->ue_network_capability.length = mm.ue_network_capability.len;
        memcpy((uint8_t *)&ue->ue_network_capability + 1,
                mm.ue_network_capability.data, mm.ue_network_capability.len);
        ue->ms_network_capability.length = mm.ms_network_capability.len;
        memcpy((uint8_t *)&ue->ms_network_capability + 1,
                mm.ms_network_capability.data, mm.ms_network_capability.len);
        if (mm.subscribed_ambr_present) {
            ue->ambr.uplink = (uint64_t)mm.subscribed_ambr_ul * 1000;
            ue->ambr.downlink = (uint64_t)mm.subscribed_ambr_dl * 1000;
        }
    }
    if (mm.mei.len) {
        ue->nas_mobile_identity_imeisv = mei;
        if (imeisv_bcd[0]) {
            ogs_cpystrn(ue->imeisv_bcd, imeisv_bcd, sizeof(ue->imeisv_bcd));
            ogs_bcd_to_buffer(imeisv_bcd, ue->imeisv, &ue->imeisv_len);
            ogs_nas_imeisv_bcd_to_buffer(imeisv_bcd,
                    ue->masked_imeisv, &ue->masked_imeisv_len);
            ue->masked_imeisv[5] = ue->masked_imeisv[6] = 0xff;
        }
    }
    if (handover) {
        /* The received pair was advanced by the source. Advancing it again
         * would disagree with the UE during vertical key derivation. */
        memcpy(ue->nh, mm.nh, sizeof(ue->nh));
        ue->nhcc = mm.ncc;
        mme_as_security_import(ue, &mm);
        ue->as_security.enb_ue_id = ue->enb_ue_id;
    } else {
        ogs_kdf_kenb(ue->kasme, ue->ul_count.i32, ue->kenb);
        ogs_kdf_nh_enb(ue->kasme, ue->kenb, ue->nh);
        ue->nhcc = 1;
    }
    rv = OGS_OK;
    goto out;
rollback:
    mme_sess_remove_all(ue);
out:
    if (rv != OGS_OK) ogs_warn("S10 context import rejected [%s]", stages[stage]);
    if (addrs) ogs_freeaddrinfo(addrs);
    ogs_free(pdns);
    return rv;
}

int mme_s10_import_context(mme_ue_t *ue, ogs_gtp2_context_response_t *rsp,
        bool keep_security)
{
    return import_context(ue, rsp, keep_security, false);
}

int mme_s10_import_handover_context(mme_ue_t *ue,
        const ogs_gtp2_forward_relocation_request_t *req)
{
    ogs_gtp2_context_response_t *ctx = ogs_calloc(1, sizeof(*ctx));
    int i, rv;
    ogs_assert(ctx);
    ctx->mme_sgsn_amf_ue_mm_context = req->mme_sgsn_amf_ue_mm_context;
    ctx->sgw_s11_s4_ip_address_and_teid_for_control_plane =
        req->sgw_s11_s4_ip_address_and_teid_for_control_plane;
    for (i = 0; i < OGS_MAX_NUM_OF_SESS; i++)
        ctx->mme_sgsn_amf_ue_eps_pdn_connections[i] =
            req->mme_sgsn_amf_ue_eps_pdn_connections[i];
    rv = import_context(ue, ctx, false, true);
    ogs_free(ctx);
    return rv;
}
