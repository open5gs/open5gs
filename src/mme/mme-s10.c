/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#include "mme-s10.h"
#include "mme-event.h"
#include "mme-fd-path.h"
#include "mme-sm.h"
#include "nas-path.h"
#include "nas-security.h"
#include "emm-handler.h"
#include "s1ap-path.h"
#include "mme-path.h"
#include "mme-s13-handler.h"

static ogs_list_t peers;

void mme_s10_init(void)
{
    ogs_list_init(&peers);
}

void mme_s10_final(void)
{
    mme_s10_peer_t *peer, *next;
    ogs_list_for_each_safe(&peers, next, peer) {
        ogs_list_remove(&peers, peer);
        ogs_gtp_xact_delete_all(&peer->gnode);
        ogs_freeaddrinfo(peer->gnode.sa_list);
        ogs_free(peer);
    }
}

/* Reject ambiguous routes and malformed numeric configuration at startup. */
static int number(const char *text, unsigned long max, unsigned long *value)
{
    char *end;
    if (!text || !*text || *text < '0' || *text > '9') return OGS_ERROR;
    errno = 0;
    *value = strtoul(text, &end, 10);
    return !errno && !*end && *value <= max ? OGS_OK : OGS_ERROR;
}

int mme_s10_parse_peer(ogs_yaml_iter_t *parent)
{
    ogs_yaml_iter_t array, iter, gummei, plmn;
    const char *address = NULL, *mcc = NULL, *mnc = NULL;
    unsigned long port, gid = 0, code = 0, value;
    bool have_gid = false, have_code = false;
    mme_s10_peer_t *peer, *other;
    ogs_sockaddr_t *addr = NULL;

    ogs_yaml_iter_recurse(parent, &array);
    do {
        address = mcc = mnc = NULL;
        have_gid = have_code = false;
        port = ogs_gtp_self()->gtpc_port;
        if (ogs_yaml_iter_type(&array) == YAML_MAPPING_NODE)
            memcpy(&iter, &array, sizeof(iter));
        else if (ogs_yaml_iter_type(&array) == YAML_SEQUENCE_NODE) {
            if (!ogs_yaml_iter_next(&array)) break;
            ogs_yaml_iter_recurse(&array, &iter);
        } else goto invalid;
        while (ogs_yaml_iter_next(&iter)) {
            const char *key = ogs_yaml_iter_key(&iter);
            if (!strcmp(key, "address")) address = ogs_yaml_iter_value(&iter);
            else if (!strcmp(key, "port")) {
                if (number(ogs_yaml_iter_value(&iter), 65535, &port) != OGS_OK ||
                        !port) goto invalid;
            } else if (!strcmp(key, "gummei")) {
                ogs_yaml_iter_recurse(&iter, &gummei);
                while (ogs_yaml_iter_next(&gummei)) {
                    key = ogs_yaml_iter_key(&gummei);
                    if (!strcmp(key, "mme_gid")) {
                        if (number(ogs_yaml_iter_value(&gummei), 65535,
                                    &gid) != OGS_OK) goto invalid;
                        have_gid = true;
                    } else if (!strcmp(key, "mme_code")) {
                        if (number(ogs_yaml_iter_value(&gummei), 255,
                                    &code) != OGS_OK) goto invalid;
                        have_code = true;
                    } else if (!strcmp(key, "plmn_id")) {
                        ogs_yaml_iter_recurse(&gummei, &plmn);
                        while (ogs_yaml_iter_next(&plmn)) {
                            key = ogs_yaml_iter_key(&plmn);
                            if (!strcmp(key, "mcc"))
                                mcc = ogs_yaml_iter_value(&plmn);
                            else if (!strcmp(key, "mnc"))
                                mnc = ogs_yaml_iter_value(&plmn);
                            else goto invalid;
                        }
                    } else goto invalid;
                }
            } else goto invalid;
        }
        if (!address || !mcc || !mnc || !have_gid || !have_code ||
                strlen(mcc) != 3 || (strlen(mnc) != 2 && strlen(mnc) != 3) ||
                number(mcc, 999, &value) != OGS_OK ||
                number(mnc, 999, &value) != OGS_OK) goto invalid;
        if (ogs_getaddrinfo(&addr, AF_UNSPEC, address, port, 0) != OGS_OK)
            goto invalid;
        peer = ogs_calloc(1, sizeof(*peer));
        ogs_assert(peer);
        ogs_plmn_id_build(&peer->plmn_id, atoi(mcc), atoi(mnc), strlen(mnc));
        peer->mme_gid = gid;
        peer->mme_code = code;
        ogs_list_for_each(&peers, other) {
            if (other->mme_gid == gid && other->mme_code == code &&
                    !memcmp(&other->plmn_id, &peer->plmn_id,
                        sizeof(peer->plmn_id))) {
                ogs_free(peer);
                ogs_freeaddrinfo(addr);
                goto invalid;
            }
        }
        peer->node = &peer->gnode;
        ogs_list_for_each(&peers, other) {
            if (ogs_sockaddr_is_equal(other->gnode.sa_list, addr) &&
                    OGS_PORT(other->gnode.sa_list) == OGS_PORT(addr)) {
                peer->node = other->node;
                break;
            }
        }
        peer->gnode.sa_list = addr;
        ogs_list_init(&peer->gnode.local_list);
        ogs_list_init(&peer->gnode.remote_list);
        ogs_list_add(&peers, peer);
    } while (ogs_yaml_iter_type(&array) == YAML_SEQUENCE_NODE);
    return OGS_OK;
invalid:
    ogs_error("Invalid mme.gtpc.client.mme peer: address, port or GUMMEI");
    return OGS_ERROR;
}

int mme_s10_open(void)
{
    mme_s10_peer_t *peer;
    ogs_list_for_each(&peers, peer) {
        if (peer->node != &peer->gnode) continue;
        if (ogs_gtp_connect(ogs_gtp_self()->gtpc_sock,
                    ogs_gtp_self()->gtpc_sock6, &peer->gnode) != OGS_OK)
            return OGS_ERROR;
    }
    return OGS_OK;
}

mme_s10_peer_t *mme_s10_find_by_addr(const ogs_sockaddr_t *addr)
{
    mme_s10_peer_t *peer;
    ogs_list_for_each(&peers, peer) {
        if (peer->node != &peer->gnode) continue;
        if (ogs_sockaddr_is_equal(&peer->gnode.addr, addr) &&
                OGS_PORT(&peer->gnode.addr) == OGS_PORT(addr)) return peer;
    }
    return NULL;
}

mme_s10_peer_t *mme_s10_find_by_guti(const ogs_nas_eps_guti_t *guti)
{
    mme_s10_peer_t *peer;
    ogs_plmn_id_t plmn_id;
    ogs_nas_to_plmn_id(&plmn_id, &guti->nas_plmn_id);
    ogs_list_for_each(&peers, peer) {
        if (peer->mme_gid == guti->mme_gid &&
                peer->mme_code == guti->mme_code &&
                !memcmp(&peer->plmn_id, &plmn_id, sizeof(plmn_id))) return peer;
    }
    return NULL;
}

bool mme_s10_message_type(uint8_t type)
{
    return type >= OGS_GTP2_IDENTIFICATION_REQUEST_TYPE &&
        type <= OGS_GTP2_CONFIGURATION_TRANSFER_TUNNEL_TYPE;
}

static void owned_xact_deleted(ogs_gtp_xact_t *xact)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    if (!ue) return;
    if (ue->s10.xact_id == xact->id) ue->s10.xact_id = OGS_INVALID_POOL_ID;
    if (ue->s10.sgw_xact_id == xact->id)
        ue->s10.sgw_xact_id = OGS_INVALID_POOL_ID;
}

void mme_s10_ue_clear(mme_ue_t *ue)
{
    ogs_gtp_xact_t *xact;
    if (ue->s10.xact_id != OGS_INVALID_POOL_ID) {
        xact = ogs_gtp_xact_find_by_id(ue->s10.xact_id);
        if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id))
            ogs_gtp_xact_cancel(xact);
    }
    if (ue->s10.sgw_xact_id != OGS_INVALID_POOL_ID) {
        xact = ogs_gtp_xact_find_by_id(ue->s10.sgw_xact_id);
        if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id))
            ogs_gtp_xact_cancel(xact);
    }
    if (ue->s10.request) ogs_pkbuf_free(ue->s10.request);
    if (ue->s10.holding) ogs_timer_delete(ue->s10.holding);
    memset(&ue->s10, 0, sizeof(ue->s10));
}

static void identity_fallback(mme_ue_t *ue)
{
    mme_s10_ue_clear(ue);
    if (ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST &&
            !MME_UE_HAVE_IMSI(ue) &&
            (OGS_FSM_CHECK(&ue->sm, emm_state_de_registered) ||
             OGS_FSM_CHECK(&ue->sm, emm_state_registered)))
        nas_eps_send_identity_request(ue);
}

static void identification_timeout(ogs_gtp_xact_t *xact, void *data)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(data));
    if (ue && ue->s10.xact_id == xact->id) {
        /* The transaction engine owns the expired transaction. */
        ue->s10.xact_id = OGS_INVALID_POOL_ID;
        identity_fallback(ue);
    }
}

/* TS 29.274 8.47: GUTI is PLMN(3), MME group(2), code(1), M-TMSI(4).
 * Do not serialize the padded NAS GUTI host structure. */
static void encode_guti(uint8_t out[10], const ogs_nas_eps_guti_t *guti)
{
    uint16_t gid = htobe16(guti->mme_gid);
    uint32_t tmsi = htobe32(guti->m_tmsi);
    memcpy(out, &guti->nas_plmn_id, 3);
    memcpy(out + 3, &gid, 2);
    out[5] = guti->mme_code;
    memcpy(out + 6, &tmsi, 4);
}

static int decode_guti(ogs_nas_eps_guti_t *guti, const ogs_tlv_octet_t *ie)
{
    uint16_t gid;
    uint32_t tmsi;
    const uint8_t *p = ie->data;
    if (!ie->presence || ie->len != 10 || !p) return OGS_ERROR;
    memset(guti, 0, sizeof(*guti));
    memcpy(&guti->nas_plmn_id, p, 3);
    memcpy(&gid, p + 3, 2);
    guti->mme_gid = be16toh(gid);
    guti->mme_code = p[5];
    memcpy(&tmsi, p + 6, 4);
    guti->m_tmsi = be32toh(tmsi);
    return OGS_OK;
}

int mme_s10_identify(mme_ue_t *ue,
        const ogs_nas_eps_attach_request_t *attach, ogs_pkbuf_t *plain,
        uint8_t nas_type)
{
    ogs_nas_eps_guti_t guti;
    const ogs_nas_eps_mobile_identity_guti_t *id =
        &attach->eps_mobile_identity.guti;
    mme_s10_peer_t *peer;
    ogs_gtp2_message_t *message;
    ogs_gtp2_identification_request_t *req;
    ogs_gtp_xact_t *xact;
    ogs_pkbuf_t *pkbuf;
    uint8_t wire_guti[10], *complete;
    ogs_nas_security_header_type_t h;
    size_t len;
    int rv;

    if (attach->eps_mobile_identity.length != sizeof(*id) ||
            id->type != OGS_NAS_EPS_MOBILE_IDENTITY_GUTI ||
            (attach->presencemask & OGS_NAS_EPS_ATTACH_REQUEST_OLD_GUTI_TYPE_PRESENT &&
             attach->old_guti_type.guti_type)) return OGS_NOTFOUND;
    memset(&guti, 0, sizeof(guti));
    guti.nas_plmn_id = id->nas_plmn_id;
    guti.mme_gid = id->mme_gid;
    guti.mme_code = id->mme_code;
    guti.m_tmsi = id->m_tmsi;
    peer = mme_s10_find_by_guti(&guti);
    if (!peer) return OGS_NOTFOUND;
    if (ue->s10.xact_id) return OGS_RETRY;

    h.type = nas_type;
    len = plain->len + (h.integrity_protected ? 6 : 0);
    if (len > UINT16_MAX - 1) return OGS_ERROR;
    complete = ogs_malloc(len + 1);
    ogs_assert(complete);
    complete[0] = 0; /* Complete Attach Request, TS 29.274 8.46 */
    memcpy(complete + 1, plain->data - (h.integrity_protected ? 6 : 0), len);
    encode_guti(wire_guti, &guti);
    message = ogs_calloc(1, sizeof(*message));
    ogs_assert(message);
    message->h.type = OGS_GTP2_IDENTIFICATION_REQUEST_TYPE;
    req = &message->identification_request;
    req->guti.presence = 1;
    req->guti.data = wire_guti;
    req->guti.len = sizeof(wire_guti);
    req->complete_attach_request_message.presence = 1;
    req->complete_attach_request_message.data = complete;
    req->complete_attach_request_message.len = len + 1;
    req->target_plmn_id.presence = 1;
    req->target_plmn_id.data = &ue->tai.plmn_id;
    req->target_plmn_id.len = sizeof(ue->tai.plmn_id);
    pkbuf = ogs_gtp2_build_msg(message);
    ogs_free(complete);
    if (!pkbuf) { ogs_free(message); return OGS_ERROR; }
    xact = ogs_gtp_xact_local_create(peer->node, &message->h, pkbuf,
            identification_timeout, OGS_UINT_TO_POINTER(ue->id));
    ogs_free(message);
    if (!xact) return OGS_ERROR;
    xact->delete_cb = owned_xact_deleted;
    ue->s10.xact_id = xact->id;
    ue->s10.peer = peer;
    rv = ogs_gtp_xact_commit(xact);
    if (rv != OGS_OK) mme_s10_ue_clear(ue);
    return rv;
}

/* Validate on a copy so malformed/forged requests cannot corrupt the serving
 * UE's replay state. Commit the accepted COUNT only after matching the GUTI. */
int mme_s10_verify_complete_request(mme_ue_t *ue, const ogs_tlv_octet_t *ie,
        uint8_t request, bool *verified)
{
    const uint8_t *bytes = ie->data;
    ogs_nas_eps_message_t nas;
    ogs_nas_eps_mobile_identity_guti_t *id;
    ogs_nas_eps_guti_t guti;
    ogs_nas_security_header_type_t h = {0};
    mme_ue_t *candidate = NULL;
    ogs_pkbuf_t *p;
    int rv = OGS_ERROR;
    *verified = false;
    if (!bytes || ie->len < 3 || request > 1 || bytes[0] != request ||
            (bytes[1] & 15) != OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM)
        return OGS_ERROR;
    p = ogs_pkbuf_alloc(NULL, ie->len + OGS_NAS_HEADROOM);
    ogs_assert(p);
    ogs_pkbuf_reserve(p, OGS_NAS_HEADROOM);
    ogs_pkbuf_put_data(p, bytes + 1, ie->len - 1);
    if ((bytes[1] >> 4) != OGS_NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE) {
        /* Initial Attach cannot use a ciphered or new-context header. */
        if ((bytes[1] >> 4) != OGS_NAS_SECURITY_HEADER_INTEGRITY_PROTECTED ||
                p->len < 8 || !SECURITY_CONTEXT_IS_VALID(ue) ||
                ue->selected_int_algorithm == OGS_NAS_SECURITY_ALGORITHMS_EIA0)
            goto out;
        candidate = ogs_memdup(ue, sizeof(*ue));
        ogs_assert(candidate);
        h.integrity_protected = 1;
        ogs_pkbuf_pull(p, 6);
        if (nas_eps_security_decode(candidate, h, p) != OGS_OK ||
                candidate->mac_failed) goto out;
        *verified = true;
    }
    if (ogs_nas_emm_decode(&nas, p) != OGS_OK) goto out;
    if (request == 0) {
        if (nas.emm.h.message_type != OGS_NAS_EPS_ATTACH_REQUEST ||
                nas.emm.attach_request.eps_mobile_identity.length != sizeof(*id))
            goto out;
        id = &nas.emm.attach_request.eps_mobile_identity.guti;
    } else {
        if (nas.emm.h.message_type != OGS_NAS_EPS_TRACKING_AREA_UPDATE_REQUEST ||
                nas.emm.tracking_area_update_request.old_guti.length != sizeof(*id))
            goto out;
        id = &nas.emm.tracking_area_update_request.old_guti.guti;
    }
    if (id->type != OGS_NAS_EPS_MOBILE_IDENTITY_GUTI) goto out;
    memset(&guti, 0, sizeof(guti));
    guti.nas_plmn_id = id->nas_plmn_id;
    guti.mme_gid = id->mme_gid;
    guti.mme_code = id->mme_code;
    guti.m_tmsi = id->m_tmsi;
    if (mme_ue_find_by_guti(&guti) != ue) goto out;
    if (*verified) {
        uint8_t tsc = request ?
            nas.emm.tracking_area_update_request.eps_update_type.tsc :
            nas.emm.attach_request.eps_attach_type.tsc;
        uint8_t ksi = request ?
            nas.emm.tracking_area_update_request.eps_update_type.nas_key_set_identifier :
            nas.emm.attach_request.eps_attach_type.nas_key_set_identifier;
        if (!emm_check_ksi(ue, tsc, ksi)) goto out;
        ue->ul_count = candidate->ul_count;
        ue->ul_count_accepted = candidate->ul_count_accepted;
    }
    rv = OGS_OK;
out:
    if (rv != OGS_OK) *verified = false;
    if (candidate) ogs_free(candidate);
    ogs_pkbuf_free(p);
    return rv;
}

static void identification_request(ogs_gtp_xact_t *xact,
        ogs_gtp2_identification_request_t *req)
{
    ogs_nas_eps_guti_t guti;
    mme_ue_t *ue = NULL;
    ogs_gtp2_message_t *message;
    ogs_gtp2_identification_response_t *rsp;
    ogs_gtp2_cause_t cause = {0};
    uint8_t context[2048];
    bool verified = false;
    ogs_pkbuf_t *p;

    cause.value = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;
    if (!req->guti.presence || !req->complete_attach_request_message.presence)
        cause.value = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    else if (req->routeing_area_identity.presence || req->packet_tmsi.presence ||
            req->p_tmsi_signature.presence ||
            decode_guti(&guti, &req->guti) != OGS_OK)
        cause.value = OGS_GTP2_CAUSE_MANDATORY_IE_INCORRECT;
    else if (!(ue = mme_ue_find_by_guti(&guti)) || !MME_UE_HAVE_IMSI(ue))
        cause.value = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    else if (mme_s10_verify_complete_request(ue,
                &req->complete_attach_request_message, 0, &verified) != OGS_OK)
        cause.value = OGS_GTP2_CAUSE_USER_AUTHENTICATION_FAILED;
    message = ogs_calloc(1, sizeof(*message));
    ogs_assert(message);
    message->h.type = OGS_GTP2_IDENTIFICATION_RESPONSE_TYPE;
    rsp = &message->identification_response;
    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);
    if (cause.value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        rsp->imsi.presence = 1;
        rsp->imsi.data = ue->imsi;
        rsp->imsi.len = ue->imsi_len;
        if (verified && mme_s10_export_mm_context(ue, &rsp->mme_sgsn_ue_mm_context,
                    context, sizeof(context)) != OGS_OK) {
            rsp->imsi.presence = 0;
            cause.value = OGS_GTP2_CAUSE_SYSTEM_FAILURE;
        }
    }
    p = ogs_gtp2_build_msg(message);
    if (p && ogs_gtp_xact_update_tx(xact, &message->h, p) == OGS_OK)
        ogs_gtp_xact_commit(xact);
    ogs_free(message);
}

bool mme_s10_imsi_available(mme_ue_t *ue, const char *imsi)
{
    mme_ue_t *old = mme_ue_find_by_imsi_bcd(imsi);
    enb_ue_t *enb;
    if (!old || old == ue) return true;
    if (!old->s10.source_transfer || !old->s10.acknowledged ||
            !old->s10.cancel_location) return false;
    /* The subscriber may return before the source holding timer expires.
     * This obsolete context has already relinquished gateway ownership. */
    enb = enb_ue_find_by_id(old->enb_ue_id);
    if (enb) enb_ue_deassociate_mme_ue(enb, old);
    mme_ue_remove(old);
    return true;
}

static void identification_response(ogs_gtp_xact_t *xact,
        ogs_gtp2_identification_response_t *rsp)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    enb_ue_t *enb;
    ogs_gtp2_cause_t *cause = rsp->cause.data;
    char imsi[OGS_MAX_IMSI_BCD_LEN + 1];
    bool pending = ue && ue->s10.xact_id == xact->id &&
        ue->s10.peer && ue->s10.peer->node == xact->gnode;
    ogs_gtp_xact_commit(xact);
    if (!pending) return;
    ue->s10.xact_id = OGS_INVALID_POOL_ID;
    if (!rsp->cause.presence || rsp->cause.len < 2 ||
            cause->value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED ||
            !rsp->imsi.presence || rsp->imsi.len < 3 ||
            rsp->imsi.len > OGS_MAX_IMSI_LEN) {
        identity_fallback(ue);
        return;
    }
    if (!ogs_buffer_to_bcd(rsp->imsi.data, rsp->imsi.len, imsi, sizeof(imsi))) {
        identity_fallback(ue);
        return;
    }
    if (strlen(imsi) > OGS_MAX_IMSI_BCD_LEN ||
            strspn(imsi, "0123456789") != strlen(imsi) ||
            !mme_s10_imsi_available(ue, imsi)) {
        identity_fallback(ue);
        return;
    }
    enb = enb_ue_find_by_id(ue->enb_ue_id);
    if (!enb || ue->nas_eps.type != MME_EPS_TYPE_ATTACH_REQUEST ||
            !(OGS_FSM_CHECK(&ue->sm, emm_state_de_registered) ||
              OGS_FSM_CHECK(&ue->sm, emm_state_registered))) {
        mme_s10_ue_clear(ue);
        return;
    }
    /* A new AKA is permitted even when the old MME supplied a context.
     * Never treat possession of an IMSI as successful UE authentication. */
    if (mme_ue_set_imsi(ue, imsi, MME_UE_IMSI_FROM_S10_IDENTIFICATION) != OGS_OK) {
        identity_fallback(ue);
        return;
    }
    mme_s10_ue_clear(ue);
    CLEAR_SECURITY_CONTEXT(ue);
    mme_s6a_send_air(enb, ue, NULL);
    OGS_FSM_TRAN(&ue->sm, &emm_state_authentication);
}

static void holding_timeout(void *data)
{
    mme_event_t *e = mme_event_new(MME_EVENT_S10_TIMER);
    ogs_assert(e);
    e->mme_ue_id = OGS_POINTER_TO_UINT(data);
    if (ogs_queue_push(ogs_app()->queue, e) != OGS_OK) mme_event_free(e);
}

void mme_s10_holding_expired(ogs_pool_id_t ue_id)
{
    mme_ue_t *ue = mme_ue_find_by_id(ue_id);
    bool remove;
    if (!ue || !ue->s10.source_transfer) return;
    remove = ue->s10.acknowledged && ue->s10.cancel_location;
    mme_s10_ue_clear(ue);
    /* Gateway sessions now belong to the target. Local removal must not
     * send Delete Session to the shared SGW or PGW. */
    if (remove) mme_ue_remove(ue);
}

bool mme_s10_cancel_location(mme_ue_t *ue)
{
    if (!ue->s10.source_transfer) return false;
    ue->s10.cancel_location = true;
    return true;
}

static int parse_s10_fteid(const ogs_tlv_octet_t *ie, uint32_t *teid)
{
    const ogs_gtp2_f_teid_t *f = ie->data;
    size_t len;
    if (!ie->presence || !f || ie->len < 5) return OGS_ERROR;
    len = 5 + (f->ipv4 ? 4 : 0) + (f->ipv6 ? 16 : 0);
    if (ie->len != len || (!f->ipv4 && !f->ipv6) ||
            f->interface_type != OGS_GTP2_F_TEID_S10_MME_GTP_C ||
            !f->teid) return OGS_ERROR;
    *teid = be32toh(f->teid);
    return OGS_OK;
}

static void context_request(ogs_gtp_xact_t *xact,
        ogs_gtp2_context_request_t *req)
{
    mme_ue_t *ue = NULL;
    ogs_nas_eps_guti_t guti;
    ogs_gtp2_header_t h = {0};
    ogs_pkbuf_t *p;
    uint8_t cause = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;
    uint32_t remote_teid = 0;
    bool verified = false;
    char imsi[OGS_MAX_IMSI_BCD_LEN + 1];
    const ogs_gtp2_indication_t *indication = req->indication.data;
    bool validated = req->indication.presence && req->indication.len >= 2 &&
        indication && indication->ms_validated;

    if (!req->s3_s16_s10_n26_address_and_teid_for_control_plane.presence ||
            (!req->guti.presence && !req->imsi.presence) ||
            !req->rat_type.presence)
        cause = OGS_GTP2_CAUSE_MANDATORY_IE_MISSING;
    else if (parse_s10_fteid(
                &req->s3_s16_s10_n26_address_and_teid_for_control_plane,
                &remote_teid) != OGS_OK ||
            req->rat_type.u8 != OGS_GTP2_RAT_TYPE_EUTRAN ||
            req->routeing_area_identity.presence || req->packet_tmsi.presence ||
            req->p_tmsi_signature.presence)
        cause = OGS_GTP2_CAUSE_MANDATORY_IE_INCORRECT;
    else if (req->imsi.presence) {
        if (!validated || !ogs_buffer_to_bcd(req->imsi.data, req->imsi.len,
                    imsi, sizeof(imsi)))
            cause = OGS_GTP2_CAUSE_MANDATORY_IE_INCORRECT;
        else ue = mme_ue_find_by_imsi_bcd(imsi);
    } else if (decode_guti(&guti, &req->guti) != OGS_OK)
        cause = OGS_GTP2_CAUSE_MANDATORY_IE_INCORRECT;
    else ue = mme_ue_find_by_guti(&guti);
    if (cause == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        if (!ue || !MME_UE_HAVE_IMSI(ue))
            cause = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
        else if (ue->s10.xact_id)
            cause = OGS_GTP2_CAUSE_REQUEST_REJECTED_REASON_NOT_SPECIFIED;
        else if (!SECURITY_CONTEXT_IS_VALID(ue))
            cause = OGS_GTP2_CAUSE_USER_AUTHENTICATION_FAILED;
        else if (!validated &&
                (mme_s10_verify_complete_request(ue,
                    &req->complete_tau_request_message, 1, &verified) != OGS_OK ||
                 !verified))
            cause = OGS_GTP2_CAUSE_USER_AUTHENTICATION_FAILED;
    }
    ogs_info("S10 Context Response [cause:%u]", cause);
    p = mme_s10_build_context_response(ue, cause);
    if (!p) {
        cause = OGS_GTP2_CAUSE_SYSTEM_FAILURE;
        p = mme_s10_build_context_response(NULL, cause);
    }
    h.type = OGS_GTP2_CONTEXT_RESPONSE_TYPE;
    h.teid = remote_teid;
    if (!p || ogs_gtp_xact_update_tx(xact, &h, p) != OGS_OK) return;
    if (cause == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        ue->s10.xact_id = xact->id;
        ue->s10.peer = (mme_s10_peer_t *)xact->gnode;
        ue->s10.remote_teid = remote_teid;
        ue->s10.source_transfer = true;
        ue->s10.holding = ogs_timer_add(ogs_app()->timer_mgr,
                holding_timeout, OGS_UINT_TO_POINTER(ue->id));
        ogs_assert(ue->s10.holding);
        ogs_timer_start(ue->s10.holding, ogs_time_from_sec(10));
        xact->local_teid = ue->mme_s10_teid;
        xact->data = OGS_UINT_TO_POINTER(ue->id);
        xact->delete_cb = owned_xact_deleted;
    }
    ogs_gtp_xact_commit(xact);
}

static void context_acknowledge(ogs_gtp_xact_t *xact,
        ogs_gtp2_message_t *message)
{
    ogs_gtp2_context_acknowledge_t *ack = &message->context_acknowledge;
    ogs_gtp2_cause_t *cause = ack->cause.data;
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    enb_ue_t *enb;
    bool matched = ue && ue->s10.source_transfer &&
        ue->s10.xact_id == xact->id && ue->s10.peer &&
        ue->s10.peer->node == xact->gnode &&
        message->h.teid_presence && message->h.teid == ue->mme_s10_teid;
    ogs_gtp_xact_commit(xact);
    if (!matched) return;
    ue->s10.xact_id = OGS_INVALID_POOL_ID;
    if (!ack->cause.presence || ack->cause.len < 2 || !cause ||
            cause->value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        mme_s10_ue_clear(ue);
        return;
    }
    ue->s10.acknowledged = true;
    enb = enb_ue_find_by_id(ue->enb_ue_id);
    if (enb && enb->ue_ctx_rel_action == S1AP_UE_CTX_REL_INVALID_ACTION)
        s1ap_send_ue_context_release_command(enb, S1AP_Cause_PR_nas,
                S1AP_CauseNas_normal_release,
                S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK, 0);
}

static void tau_failure(mme_ue_t *ue, uint8_t cause)
{
    enb_ue_t *enb = enb_ue_find_by_id(ue->enb_ue_id);
    bool gateway_updated = ue->s10.gateway_updated;
    mme_s10_ue_clear(ue);
    if (!gateway_updated) {
        mme_sess_remove_all(ue);
        CLEAR_SESSION_CONTEXT(ue);
    }
    OGS_FSM_TRAN(&ue->sm, &emm_state_exception);
    if (enb) {
        nas_eps_send_tau_reject(enb, ue, cause);
        mme_send_delete_session_or_mme_ue_context_release(enb, ue);
    } else mme_ue_remove(ue);
}

static void tau_timeout(ogs_gtp_xact_t *xact, void *data)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(data));
    if (!ue) return;
    if (ue->s10.xact_id == xact->id) ue->s10.xact_id = 0;
    else if (ue->s10.sgw_xact_id == xact->id) ue->s10.sgw_xact_id = 0;
    else return;
    tau_failure(ue, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE);
}

static int send_context_request(mme_ue_t *ue)
{
    ogs_gtp2_message_t *message = ogs_calloc(1, sizeof(*message));
    ogs_gtp2_context_request_t *req;
    ogs_gtp2_f_teid_t fteid = {0};
    ogs_gtp2_indication_t indication = {0};
    uint8_t guti[10];
    ogs_pkbuf_t *p;
    ogs_gtp_xact_t *xact;
    int len;
    ogs_assert(message);
    ogs_assert(ue->s10.peer);
    message->h.type = OGS_GTP2_CONTEXT_REQUEST_TYPE;
    req = &message->context_request;
    if (ue->s10.fresh_security) {
        req->imsi.presence = 1;
        req->imsi.data = ue->imsi;
        req->imsi.len = ue->imsi_len;
        indication.ms_validated = 1;
        req->indication.presence = 1;
        req->indication.data = &indication;
        req->indication.len = 2;
    } else {
        encode_guti(guti, &ue->old_guti);
        req->guti.presence = 1;
        req->guti.data = guti;
        req->guti.len = sizeof(guti);
    }
    fteid.interface_type = OGS_GTP2_F_TEID_S10_MME_GTP_C;
    fteid.teid = htobe32(ue->mme_s10_teid);
    if (ogs_gtp2_sockaddr_to_f_teid(ogs_gtp_self()->gtpc_addr,
                ogs_gtp_self()->gtpc_addr6, &fteid, &len) != OGS_OK) {
        ogs_free(message);
        return OGS_ERROR;
    }
    req->s3_s16_s10_n26_address_and_teid_for_control_plane.presence = 1;
    req->s3_s16_s10_n26_address_and_teid_for_control_plane.data = &fteid;
    req->s3_s16_s10_n26_address_and_teid_for_control_plane.len = len;
    req->rat_type.presence = 1;
    req->rat_type.u8 = OGS_GTP2_RAT_TYPE_EUTRAN;
    req->target_plmn_id.presence = 1;
    req->target_plmn_id.data = &ue->tai.plmn_id;
    req->target_plmn_id.len = sizeof(ue->tai.plmn_id);
    if (ue->s10.request) {
        req->complete_tau_request_message.presence = 1;
        req->complete_tau_request_message.data = ue->s10.request->data;
        req->complete_tau_request_message.len = ue->s10.request->len;
    }
    p = ogs_gtp2_build_msg(message);
    if (!p) { ogs_free(message); return OGS_ERROR; }
    xact = ogs_gtp_xact_local_create(ue->s10.peer->node,
            &message->h, p, tau_timeout, OGS_UINT_TO_POINTER(ue->id));
    ogs_free(message);
    if (!xact) return OGS_ERROR;
    xact->delete_cb = owned_xact_deleted;
    xact->local_teid = ue->mme_s10_teid;
    ue->s10.xact_id = xact->id;
    return ogs_gtp_xact_commit(xact);
}

int mme_s10_tau_request(mme_ue_t *ue, ogs_pkbuf_t *plain, uint8_t nas_type)
{
    mme_s10_peer_t *peer = mme_s10_find_by_guti(&ue->old_guti);
    ogs_nas_security_header_type_t h;
    size_t len;
    if (!peer) {
        ogs_debug("No S10 route for old GUMMEI [%u:%u]", ue->old_guti.mme_gid,
                ue->old_guti.mme_code);
        return OGS_NOTFOUND;
    }
    ogs_info("S10 Context Request [ue:%u]", ue->id);
    if (ue->s10.xact_id || ue->s10.auth_pending) return OGS_RETRY;
    mme_s10_ue_clear(ue);
    h.type = nas_type;
    len = plain->len + (h.integrity_protected ? 6 : 0);
    if (len > UINT16_MAX - 1) return OGS_ERROR;
    ue->s10.request = ogs_pkbuf_alloc(NULL, len + 1);
    ogs_assert(ue->s10.request);
    *(uint8_t *)ogs_pkbuf_put(ue->s10.request, 1) = 1;
    ogs_pkbuf_put_data(ue->s10.request,
            plain->data - (h.integrity_protected ? 6 : 0), len);
    ue->s10.peer = peer;
    ue->s10.target_transfer = true;
    return send_context_request(ue);
}

int mme_s10_authentication_complete(mme_ue_t *ue)
{
    if (!ue->s10.target_transfer || !ue->s10.auth_pending)
        return OGS_NOTFOUND;
    ue->s10.auth_pending = false;
    ue->s10.fresh_security = true;
    return send_context_request(ue);
}

static int send_context_ack(ogs_gtp_xact_t *xact, uint32_t teid, uint8_t value)
{
    ogs_gtp2_message_t *message = ogs_calloc(1, sizeof(*message));
    ogs_gtp2_cause_t cause = {0};
    ogs_pkbuf_t *p;
    int rv = OGS_ERROR;
    ogs_assert(message);
    message->h.type = OGS_GTP2_CONTEXT_ACKNOWLEDGE_TYPE;
    message->h.teid = teid;
    cause.value = value;
    message->context_acknowledge.cause.presence = 1;
    message->context_acknowledge.cause.data = &cause;
    message->context_acknowledge.cause.len = sizeof(cause);
    p = ogs_gtp2_build_msg(message);
    if (p && ogs_gtp_xact_update_tx(xact, &message->h, p) == OGS_OK)
        rv = ogs_gtp_xact_commit(xact);
    ogs_free(message);
    return rv;
}

static int send_sgw_update(mme_ue_t *ue)
{
    ogs_gtp2_message_t *message = ogs_calloc(1, sizeof(*message));
    ogs_gtp2_modify_bearer_request_t *req;
    ogs_gtp2_f_teid_t fteid = {0};
    sgw_ue_t *sgw = sgw_ue_find_by_id(ue->sgw_ue_id);
    ogs_gtp_xact_t *xact;
    ogs_pkbuf_t *p;
    int len;
    ogs_assert(message);
    if (!sgw || !sgw->sgw_s11_teid) { ogs_free(message); return OGS_ERROR; }
    message->h.type = OGS_GTP2_MODIFY_BEARER_REQUEST_TYPE;
    message->h.teid = sgw->sgw_s11_teid;
    req = &message->modify_bearer_request;
    fteid.interface_type = OGS_GTP2_F_TEID_S11_MME_GTP_C;
    fteid.teid = htobe32(ue->mme_s11_teid);
    if (ogs_gtp2_sockaddr_to_f_teid(ogs_gtp_self()->gtpc_addr,
                ogs_gtp_self()->gtpc_addr6, &fteid, &len) != OGS_OK) {
        ogs_free(message);
        return OGS_ERROR;
    }
    req->sender_f_teid_for_control_plane.presence = 1;
    req->sender_f_teid_for_control_plane.data = &fteid;
    req->sender_f_teid_for_control_plane.len = len;
    req->rat_type.presence = 1;
    req->rat_type.u8 = OGS_GTP2_RAT_TYPE_EUTRAN;
    req->serving_network.presence = 1;
    req->serving_network.data = &ue->tai.plmn_id;
    req->serving_network.len = sizeof(ue->tai.plmn_id);
    p = ogs_gtp2_build_msg(message);
    if (!p) { ogs_free(message); return OGS_ERROR; }
    xact = ogs_gtp_xact_local_create(sgw->gnode, &message->h, p,
            tau_timeout, OGS_UINT_TO_POINTER(ue->id));
    ogs_free(message);
    if (!xact) return OGS_ERROR;
    xact->delete_cb = owned_xact_deleted;
    xact->modify_action = OGS_GTP_MODIFY_IN_S10_TAU;
    xact->local_teid = ue->mme_s11_teid;
    ue->s10.sgw_xact_id = xact->id;
    return ogs_gtp_xact_commit(xact);
}

void mme_s10_sgw_response(ogs_gtp_xact_t *xact, mme_ue_t *ue,
        ogs_gtp2_modify_bearer_response_t *rsp)
{
    mme_ue_t *pending = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    ogs_gtp2_cause_t *cause = rsp->cause.data;
    enb_ue_t *enb;
    bool matched = ue && ue == pending && ue->s10.target_transfer &&
        ue->s10.sgw_xact_id == xact->id;
    ogs_gtp_xact_commit(xact);
    if (!matched) return;
    ue->s10.sgw_xact_id = 0;
    if (!rsp->cause.presence || rsp->cause.len < 2 || !cause ||
            cause->value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        tau_failure(ue, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE);
        return;
    }
    ue->s10.gateway_updated = true;
    enb = enb_ue_find_by_id(ue->enb_ue_id);
    if (!enb) { tau_failure(ue, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE); return; }
    OGS_FSM_TRAN(&ue->sm, &emm_state_initial_context_setup);
    if (mme_s13_check_wanted(ue)) mme_s13_start_check(enb, ue);
    else mme_s6a_send_ulr(enb, ue, 0);
}

static void context_response(ogs_gtp_xact_t *xact, ogs_gtp2_message_t *message)
{
    ogs_gtp2_context_response_t *rsp = &message->context_response;
    ogs_gtp2_cause_t *cause = rsp->cause.data;
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    uint32_t remote_teid = 0;
    char imsi[OGS_MAX_IMSI_BCD_LEN + 1];
    ogs_gtp2_eps_mm_context_t mm;
    if (!ue || !ue->s10.target_transfer || ue->s10.xact_id != xact->id ||
            !ue->s10.peer || ue->s10.peer->node != xact->gnode) return;
    if (!rsp->cause.presence || rsp->cause.len < 2 || !cause ||
            (message->h.teid != 0 && message->h.teid != ue->mme_s10_teid)) {
        tau_failure(ue, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE);
        return;
    }
    ogs_info("S10 received Context Response [cause:%u]", cause->value);
    if (cause->value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        /* There is no successful transfer to acknowledge. Retire the failed
         * exchange before initiating the NAS authentication fallback. */
        mme_s10_peer_t *peer = ue->s10.peer;
        ogs_pkbuf_t *request = ue->s10.request;
        bool retry_auth = !ue->s10.fresh_security &&
            cause->value == OGS_GTP2_CAUSE_USER_AUTHENTICATION_FAILED;
        ue->s10.request = NULL;
        mme_s10_ue_clear(ue);
        if (retry_auth) {
            ue->s10.peer = peer;
            ue->s10.request = request;
            ue->s10.target_transfer = true;
            ue->s10.auth_pending = true;
            nas_eps_send_identity_request(ue);
        } else {
            if (request) ogs_pkbuf_free(request);
            tau_failure(ue,
                OGS_NAS_EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
        }
        return;
    }
    if (parse_s10_fteid(&rsp->sender_f_teid_for_control_plane,
                &remote_teid) != OGS_OK || !rsp->imsi.presence ||
            !ogs_buffer_to_bcd(rsp->imsi.data, rsp->imsi.len, imsi, sizeof(imsi)) ||
            strspn(imsi, "0123456789") != strlen(imsi) ||
            (MME_UE_HAVE_IMSI(ue) && strcmp(imsi, ue->imsi_bcd)) ||
            !mme_s10_imsi_available(ue, imsi) ||
            !rsp->mme_sgsn_amf_ue_mm_context.presence ||
            ogs_gtp2_parse_eps_mm_context(&mm,
                &rsp->mme_sgsn_amf_ue_mm_context) != OGS_OK) {
        send_context_ack(xact, remote_teid,
                OGS_GTP2_CAUSE_MANDATORY_IE_INCORRECT);
        ue->s10.xact_id = 0;
        tau_failure(ue, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE);
        return;
    }
    if (!ue->s10.fresh_security && !mme_s10_security_context_usable(ue, &mm)) {
        enb_ue_t *enb = enb_ue_find_by_id(ue->enb_ue_id);
        send_context_ack(xact, remote_teid,
                OGS_GTP2_CAUSE_USER_AUTHENTICATION_FAILED);
        ue->s10.xact_id = 0;
        if (!enb || mme_ue_set_imsi(ue, imsi,
                    MME_UE_IMSI_FROM_S10_CONTEXT) != OGS_OK) {
            tau_failure(ue, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE);
            return;
        }
        ue->s10.auth_pending = true;
        CLEAR_SECURITY_CONTEXT(ue);
        mme_s6a_send_air(enb, ue, NULL);
        OGS_FSM_TRAN(&ue->sm, &emm_state_authentication);
        return;
    }
    if (mme_s10_import_context(ue, rsp, ue->s10.fresh_security) != OGS_OK) {
        send_context_ack(xact, remote_teid,
                OGS_GTP2_CAUSE_MANDATORY_IE_INCORRECT);
        ue->s10.xact_id = 0;
        tau_failure(ue, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE);
        return;
    }
    if (!MME_UE_HAVE_IMSI(ue) &&
            mme_ue_set_imsi(ue, imsi, MME_UE_IMSI_FROM_S10_CONTEXT) != OGS_OK) {
        send_context_ack(xact, remote_teid, OGS_GTP2_CAUSE_SYSTEM_FAILURE);
        ue->s10.xact_id = 0;
        tau_failure(ue, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE);
        return;
    }
    ue->s10.remote_teid = remote_teid;
    if (send_context_ack(xact, remote_teid,
                OGS_GTP2_CAUSE_REQUEST_ACCEPTED) != OGS_OK) {
        ue->s10.xact_id = 0;
        tau_failure(ue, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE);
        return;
    }
    ue->s10.xact_id = 0; /* Keep the ACK in the transaction duplicate cache. */
    if (!MME_NEXT_GUTI_IS_AVAILABLE(ue)) mme_ue_new_guti(ue);
    if (send_sgw_update(ue) != OGS_OK)
        tau_failure(ue, OGS_NAS_EMM_CAUSE_NETWORK_FAILURE);
}

void mme_s10_receive(ogs_gtp_node_t *node, ogs_pkbuf_t *pkbuf)
{
    ogs_gtp2_message_t *message = ogs_calloc(1, sizeof(*message));
    ogs_gtp_xact_t *xact = NULL;
    ogs_assert(message);
    if (ogs_gtp2_parse_msg(message, pkbuf) != OGS_OK) goto out;
    if (message->h.type != OGS_GTP2_IDENTIFICATION_REQUEST_TYPE &&
            message->h.type != OGS_GTP2_IDENTIFICATION_RESPONSE_TYPE &&
            message->h.type != OGS_GTP2_CONTEXT_REQUEST_TYPE &&
            message->h.type != OGS_GTP2_CONTEXT_RESPONSE_TYPE &&
            message->h.type != OGS_GTP2_CONTEXT_ACKNOWLEDGE_TYPE &&
            message->h.type != OGS_GTP2_ECHO_REQUEST_TYPE &&
            message->h.type != OGS_GTP2_ECHO_RESPONSE_TYPE) {
        ogs_warn("Unsupported S10 procedure [%u]", message->h.type);
        goto out;
    }
    if (ogs_gtp_xact_receive(node, &message->h, &xact) != OGS_OK) goto out;
    switch (message->h.type) {
    case OGS_GTP2_ECHO_REQUEST_TYPE:
        ogs_gtp2_send_echo_response(xact, 0, 0);
        break;
    case OGS_GTP2_ECHO_RESPONSE_TYPE:
        ogs_gtp_xact_commit(xact);
        break;
    case OGS_GTP2_CONTEXT_RESPONSE_TYPE:
        context_response(xact, message);
        break;
    case OGS_GTP2_CONTEXT_REQUEST_TYPE:
        context_request(xact, &message->context_request);
        break;
    case OGS_GTP2_CONTEXT_ACKNOWLEDGE_TYPE:
        context_acknowledge(xact, message);
        break;
    case OGS_GTP2_IDENTIFICATION_REQUEST_TYPE:
        identification_request(xact, &message->identification_request);
        break;
    case OGS_GTP2_IDENTIFICATION_RESPONSE_TYPE:
        identification_response(xact, &message->identification_response);
        break;
    }
out:
    ogs_free(message);
}
