/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#include "mme-s10.h"
#include "mme-as-security.h"
#include "mme-event.h"
#include "mme-sm.h"
#include "s1ap-path.h"
#include "s1ap-build.h"
#include "mme-gtp-path.h"
#include "mme-s11-build.h"
#include "mme-fd-path.h"
#include "nas-path.h"

typedef enum {
    HO_SOURCE_WAIT,
    HO_SOURCE_PREPARING,
    HO_SOURCE_COMMAND,
    HO_SOURCE_CANCELLING,
    HO_TARGET_PREPARING,
    HO_TARGET_READY,
    HO_TARGET_COMPLETE,
    HO_TARGET_RELEASING,
    HO_SOURCE_COMPLETE
} handover_phase_t;

typedef struct mme_s10_handover_s {
    handover_phase_t phase;
    mme_s10_peer_t *peer;
    uint32_t remote_teid;
    ogs_pool_id_t enb_ue_id;
    ogs_pool_id_t xact_id;
    ogs_pool_id_t indirect_xact_id;
    ogs_pool_id_t cleanup_xact_id;
    ogs_pool_id_t status_xact_id;
    ogs_pool_id_t cancel_xact_id;
    ogs_pool_id_t complete_xact_id;
    ogs_pool_id_t sgw_xact_id;
    ogs_timer_t *timer;
    ogs_time_t deadline;
    bool direct;
    bool command_sent;
    bool admission_known;
    uint16_t admitted_ebis;
    bool cancel_complete;
    bool indirect_uncertain;
    bool gateway_updated;
    bool hss_update_started;
    bool tau_pending;
    S1AP_TargeteNB_ID_t target;
    S1AP_Cause_t cause;
    ogs_pkbuf_t *container;
    ogs_pkbuf_t *downlink[16];
    unsigned int num_downlink;
} handover_t;

static void timer_expired(void *data)
{
    mme_event_t *e = mme_event_new(MME_EVENT_S10_TIMER);
    ogs_assert(e);
    e->mme_ue_id = OGS_POINTER_TO_UINT(data);
    e->timer_id = 1;
    if (ogs_queue_push(ogs_app()->queue, e) != OGS_OK) mme_event_free(e);
}

static handover_t *allocate(mme_ue_t *ue, handover_phase_t phase)
{
    handover_t *ho;
    ogs_assert(!ue->s10_handover);
    ho = ogs_calloc(1, sizeof(*ho));
    ogs_assert(ho);
    ho->phase = phase;
    ho->enb_ue_id = ue->enb_ue_id;
    ho->timer = ogs_timer_add(ogs_app()->timer_mgr, timer_expired,
            OGS_UINT_TO_POINTER(ue->id));
    ogs_assert(ho->timer);
    ue->s10_handover = ho;
    return ho;
}

static void xact_deleted(ogs_gtp_xact_t *xact)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    handover_t *ho = ue ? ue->s10_handover : NULL;
    if (!ho) return;
    if (ho->indirect_xact_id == xact->id) ho->indirect_xact_id = 0;
    if (ho->cleanup_xact_id == xact->id) ho->cleanup_xact_id = 0;
    if (ho->status_xact_id == xact->id) ho->status_xact_id = 0;
    if (ho->cancel_xact_id == xact->id) ho->cancel_xact_id = 0;
    if (ho->xact_id == xact->id) ho->xact_id = OGS_INVALID_POOL_ID;
    if (ho->complete_xact_id == xact->id) ho->complete_xact_id = OGS_INVALID_POOL_ID;
    if (ho->sgw_xact_id == xact->id) ho->sgw_xact_id = OGS_INVALID_POOL_ID;
}

void mme_s10_handover_clear(mme_ue_t *ue)
{
    handover_t *ho = ue->s10_handover;
    ogs_gtp_xact_t *xact;
    unsigned int i;
    if (!ho) return;
    xact = ogs_gtp_xact_find_by_id(ho->xact_id);
    if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id))
        ogs_gtp_xact_cancel(xact);
    xact = ogs_gtp_xact_find_by_id(ho->complete_xact_id);
    if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id))
        ogs_gtp_xact_cancel(xact);
    xact = ogs_gtp_xact_find_by_id(ho->sgw_xact_id);
    if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id))
        ogs_gtp_xact_cancel(xact);
    xact = ogs_gtp_xact_find_by_id(ho->indirect_xact_id);
    if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id))
        ogs_gtp_xact_cancel(xact);
    xact = ogs_gtp_xact_find_by_id(ho->cleanup_xact_id);
    if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id))
        ogs_gtp_xact_cancel(xact);
    xact = ogs_gtp_xact_find_by_id(ho->status_xact_id);
    if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id))
        ogs_gtp_xact_cancel(xact);
    xact = ogs_gtp_xact_find_by_id(ho->cancel_xact_id);
    if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id))
        ogs_gtp_xact_cancel(xact);
    if (ho->container) ogs_pkbuf_free(ho->container);
    for (i = 0; i < ho->num_downlink; i++) ogs_pkbuf_free(ho->downlink[i]);
    ogs_asn_free(&asn_DEF_S1AP_TargeteNB_ID, &ho->target);
    ogs_timer_delete(ho->timer);
    ogs_free(ho);
    ue->s10_handover = NULL;
}

static void abandon_remote(mme_ue_t *ue, const handover_t *ho)
{
    ogs_gtp2_message_t *msg = ogs_calloc(1, sizeof(*msg));
    ogs_gtp2_header_t h = {0};
    ogs_gtp_xact_t *xact;
    ogs_pkbuf_t *p;
    ogs_assert(msg);
    msg->h.type = OGS_GTP2_RELOCATION_CANCEL_REQUEST_TYPE;
    msg->relocation_cancel_request.imsi.presence = 1;
    msg->relocation_cancel_request.imsi.data = ue->imsi;
    msg->relocation_cancel_request.imsi.len = ue->imsi_len;
    p = ogs_gtp2_build_msg(msg);
    ogs_free(msg);
    if (!p) return;
    h.type = OGS_GTP2_RELOCATION_CANCEL_REQUEST_TYPE;
    h.teid = ho->remote_teid;
    /* This cleanup exchange outlives the failed UE procedure. No callback
     * may dereference its context after source_failure() clears the state. */
    xact = ogs_gtp_xact_local_create(ho->peer->node, &h, p, NULL, NULL);
    if (xact) ogs_gtp_xact_commit(xact);
}

static int cleanup_forwarding(mme_ue_t *ue, enb_ue_t *source, bool cancel);

static void source_failure(mme_ue_t *ue, const S1AP_Cause_t *cause)
{
    handover_t *ho = ue->s10_handover;
    enb_ue_t *enb;
    ogs_pkbuf_t *downlink[16];
    unsigned int n, i;
    bool cleanup = ho && ho->phase != HO_SOURCE_CANCELLING &&
        ho->indirect_uncertain;
    if (!ho) return;
    if (ho->phase == HO_SOURCE_PREPARING || ho->phase == HO_SOURCE_COMMAND)
        abandon_remote(ue, ho);
    enb = enb_ue_find_by_id(ho->enb_ue_id);
    if (enb && (ho->phase <= HO_SOURCE_PREPARING ||
                (ho->phase == HO_SOURCE_COMMAND && !ho->command_sent)))
        s1ap_send_handover_preparation_failure(enb,
                cause ? cause->present : S1AP_Cause_PR_radioNetwork,
                cause ? cause->choice.radioNetwork :
                    S1AP_CauseRadioNetwork_ho_failure_in_target_EPC_eNB_or_target_system);
    n = ho->num_downlink;
    memcpy(downlink, ho->downlink, n * sizeof(downlink[0]));
    ho->num_downlink = 0;
    ue->s10.source_transfer = ue->s10.acknowledged = false;
    ue->s10.cancel_location = false;
    ue->s10.sgw_changed = false;
    mme_s10_handover_clear(ue);
    if (cleanup) cleanup_forwarding(ue, NULL, false);
    for (i = 0; i < n; i++) {
        if (enb) s1ap_send_to_enb_ue(enb, downlink[i]);
        else ogs_pkbuf_free(downlink[i]);
    }
}

bool mme_s10_handover_bearer_rejected(const mme_ue_t *ue, uint8_t ebi)
{
    const handover_t *ho = ue ? ue->s10_handover : NULL;
    return ho && ho->admission_known && ebi >= 5 && ebi <= 15 &&
        !(ho->admitted_ebis & (1 << ebi));
}

bool mme_s10_handover_busy(const mme_ue_t *ue)
{
    return ue && ue->s10_handover &&
        (ue->s10_handover->phase <= HO_SOURCE_CANCELLING ||
         ue->s10_handover->phase == HO_SOURCE_COMPLETE);
}

bool mme_s10_handover_queue_downlink(enb_ue_t *enb, ogs_pkbuf_t *p)
{
    mme_ue_t *ue = mme_ue_find_by_id(enb->mme_ue_id);
    handover_t *ho = ue ? ue->s10_handover : NULL;
    if (mme_s10_source_committed(ue)) {
        ogs_pkbuf_free(p);
        return true;
    }
    if (!ho || ho->enb_ue_id != enb->id || ho->phase == HO_SOURCE_WAIT ||
            !mme_s10_handover_busy(ue)) return false;
    if (ho->phase == HO_SOURCE_COMPLETE) {
        ogs_pkbuf_free(p);
        return true;
    }
    if (ho->num_downlink >= OGS_ARRAY_SIZE(ho->downlink)) {
        ogs_warn("S10 downlink queue exhausted, failing relocation [ue:%u]", ue->id);
        mme_s10_handover_cancel(enb);
        ogs_pkbuf_free(p);
        return true;
    }
    /* TS 33.401 7.2.10: no downlink NAS after Forward Relocation Request.
     * Encoded counters remain local; discard queued PDUs on success, or
     * resume their original order and COUNT values on preparation failure. */
    ho->downlink[ho->num_downlink++] = p;
    return true;
}

static int read_fteid(const ogs_tlv_octet_t *ie, uint8_t interface,
        uint32_t *teid, ogs_ip_t *ip)
{
    ogs_gtp2_f_teid_t *f = ie->data;
    if (!ie->presence || !f || ie->len < 5 || f->interface_type != interface ||
            !f->teid || (!f->ipv4 && !f->ipv6) ||
            ie->len != 5 + (f->ipv4 ? 4 : 0) + (f->ipv6 ? 16 : 0))
        return OGS_ERROR;
    *teid = be32toh(f->teid);
    return ogs_gtp2_f_teid_to_ip(f, ip);
}

static int set_fteid(ogs_tlv_octet_t *ie, ogs_gtp2_f_teid_t *f,
        uint8_t interface, uint32_t teid, ogs_ip_t *ip)
{
    int len;
    f->interface_type = interface;
    f->teid = htobe32(teid);
    if (!teid || ogs_gtp2_ip_to_f_teid(ip, f, &len) != OGS_OK)
        return OGS_ERROR;
    ie->presence = 1;
    ie->data = f;
    ie->len = len;
    return OGS_OK;
}

static int reply(ogs_gtp_xact_t *xact, uint8_t type, uint32_t teid,
        ogs_pkbuf_t *body)
{
    ogs_gtp2_header_t h = {0};
    if (!body) return OGS_ERROR;
    h.type = type;
    h.teid = teid;
    if (ogs_gtp_xact_update_tx(xact, &h, body) != OGS_OK) return OGS_ERROR;
    return ogs_gtp_xact_commit(xact);
}

static int relocation_response(ogs_gtp_xact_t *xact, mme_ue_t *ue,
        uint32_t teid, uint8_t value, const S1AP_Cause_t *radio_cause)
{
    ogs_gtp2_message_t *msg = ogs_calloc(1, sizeof(*msg));
    ogs_gtp2_forward_relocation_response_t *rsp;
    ogs_gtp2_cause_t cause = {0};
    ogs_gtp2_f_teid_t sender = {0}, endpoints[OGS_BEARER_PER_UE][2] = {{{0}}};
    ogs_pkbuf_t *body = NULL;
    mme_sess_t *sess;
    mme_bearer_t *bearer;
    uint8_t f_cause[2], *container = NULL;
    ogs_gtp2_indication_t indication = {0};
    int len, n = 0, rv = OGS_ERROR;
    ogs_assert(msg);
    msg->h.type = OGS_GTP2_FORWARD_RELOCATION_RESPONSE_TYPE;
    rsp = &msg->forward_relocation_response;
    cause.value = value;
    rsp->cause.presence = 1;
    rsp->cause.data = &cause;
    rsp->cause.len = sizeof(cause);
    if (radio_cause && mme_s10_encode_cause(&rsp->s1_ap_cause,
                radio_cause, f_cause) != OGS_OK) goto out;
    if (value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) goto build;
    if (!ue || !ue->container.size || !ue->container.buf ||
            ue->container.size > OGS_MAX_SDU_LEN / 2) goto out;
    sender.interface_type = OGS_GTP2_F_TEID_S10_MME_GTP_C;
    sender.teid = htobe32(ue->mme_s10_teid);
    if (ogs_gtp2_sockaddr_to_f_teid(ogs_gtp_self()->gtpc_addr,
                ogs_gtp_self()->gtpc_addr6, &sender, &len) != OGS_OK) goto out;
    rsp->sender_s_f_teid_for_control_plane.presence = 1;
    rsp->sender_s_f_teid_for_control_plane.data = &sender;
    rsp->sender_s_f_teid_for_control_plane.len = len;
    if (mme_s10_sgw_relocated(ue)) {
        indication.sgw_change_indication = 1;
        rsp->indication_flags.presence = 1;
        rsp->indication_flags.data = &indication;
        rsp->indication_flags.len = 1;
    }
    ogs_list_for_each(&ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            ogs_gtp2_tlv_relocation_bearer_context_t *bc;
            if (!bearer->target_s1u_teid) continue;
            if (n >= OGS_BEARER_PER_UE) goto out;
            bc = &rsp->list_of_set_up_bearers[n];
            bc->presence = 1;
            bc->eps_bearer_id.presence = 1;
            bc->eps_bearer_id.u8 = bearer->ebi;
            if (bearer->enb_dl_teid && set_fteid(&bc->enb_dl_forwarding_f_teid,
                        &endpoints[n][0], OGS_GTP2_F_TEID_ENODEB_GTP_U_FOR_DL_DATA_FORWARDING,
                        bearer->enb_dl_teid, &bearer->enb_dl_ip) != OGS_OK) goto out;
            if (bearer->enb_ul_teid && set_fteid(&bc->enb_ul_forwarding_f_teid,
                        &endpoints[n][1], OGS_GTP2_F_TEID_ENODEB_GTP_U_FOR_UL_DATA_FORWARDING,
                        bearer->enb_ul_teid, &bearer->enb_ul_ip) != OGS_OK) goto out;
            if (mme_s10_sgw_relocated(ue) && !ue->s10_handover->direct) {
                bc->enb_dl_forwarding_f_teid.presence = 0;
                bc->enb_ul_forwarding_f_teid.presence = 0;
                if (bearer->sgw_dl_teid && set_fteid(&bc->sgw_dl_forwarding_f_teid,
                        &endpoints[n][0], OGS_GTP2_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING,
                        bearer->sgw_dl_teid, &bearer->sgw_dl_ip) != OGS_OK) goto out;
                if (bearer->sgw_ul_teid && set_fteid(&bc->sgw_ul_forwarding_f_teid,
                        &endpoints[n][1], OGS_GTP2_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING,
                        bearer->sgw_ul_teid, &bearer->sgw_ul_ip) != OGS_OK) goto out;
            }
            n++;
        }
    }
    if (!n) goto out;
    container = ogs_malloc(ue->container.size + 1);
    ogs_assert(container);
    container[0] = 3;
    memcpy(container + 1, ue->container.buf, ue->container.size);
    rsp->e_utran_transparent_container.presence = 1;
    rsp->e_utran_transparent_container.data = container;
    rsp->e_utran_transparent_container.len = ue->container.size + 1;
build:
    body = ogs_gtp2_build_msg(msg);
    rv = reply(xact, msg->h.type, teid, body);
out:
    if (container) ogs_free(container);
    ogs_free(msg);
    return rv;
}

bool mme_s10_handover_is_target(const mme_ue_t *ue)
{
    return ue && ue->s10_handover &&
        ue->s10_handover->phase >= HO_TARGET_PREPARING &&
        ue->s10_handover->phase <= HO_TARGET_COMPLETE;
}

bool mme_s10_handover_direct(const mme_ue_t *ue)
{
    return ue && ue->s10_handover && ue->s10_handover->direct;
}

static void preparation_timeout(ogs_gtp_xact_t *xact, void *data)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(data));
    if (ue && ue->s10_handover && ue->s10_handover->xact_id == xact->id) {
        ue->s10_handover->xact_id = OGS_INVALID_POOL_ID;
        source_failure(ue, NULL);
    }
}

int mme_s10_handover_required(enb_ue_t *source,
        const S1AP_TargeteNB_ID_t *target, const S1AP_Cause_t *cause,
        const OCTET_STRING_t *container, bool direct)
{
    mme_ue_t *ue = mme_ue_find_by_id(source->mme_ue_id);
    mme_s10_target_t decoded;
    ogs_tlv_octet_t target_ie = {0};
    uint8_t target_buffer[10];
    mme_s10_peer_t *peer;
    handover_t *ho;
    if (!ue || ue->enb_ue_id != source->id ||
            mme_s10_encode_target(&target_ie, target, target_buffer) != OGS_OK ||
            mme_s10_decode_target(&decoded, &target_ie) != OGS_OK)
        return OGS_ERROR;
    if (target->selected_TAI->pLMNidentity.size != sizeof(decoded.tai.plmn_id) ||
            !target->selected_TAI->pLMNidentity.buf) return OGS_ERROR;
    memcpy(&decoded.tai.plmn_id, target->selected_TAI->pLMNidentity.buf,
            sizeof(decoded.tai.plmn_id));
    peer = mme_s10_find_by_tai(&decoded.tai);
    if (!peer) return OGS_NOTFOUND;
    if (ue->s10_handover &&
            ue->s10_handover->phase == HO_TARGET_COMPLETE &&
            !ue->s10_handover->complete_xact_id &&
            ue->s10_handover->gateway_updated &&
            OGS_FSM_CHECK(&ue->sm, emm_state_registered))
        mme_s10_handover_clear(ue);
    if (ue->s10_handover || ue->s10.xact_id || ue->s10.source_transfer ||
            !container || !container->buf || !container->size ||
            container->size > OGS_MAX_SDU_LEN / 2) return OGS_ERROR;
    ho = allocate(ue, HO_SOURCE_WAIT);
    ho->peer = peer;
    ho->direct = direct;
    ho->cause = *cause;
    if (ogs_asn_copy_ie(&asn_DEF_S1AP_TargeteNB_ID,
                (void *)target, &ho->target) != OGS_OK) {
        mme_s10_handover_clear(ue);
        return OGS_ERROR;
    }
    ho->container = ogs_pkbuf_alloc(NULL, container->size);
    ogs_assert(ho->container);
    ogs_pkbuf_put_data(ho->container, container->buf, container->size);
    ho->deadline = ogs_time_now() + ogs_time_from_sec(5);
    /* Wait for concurrent NAS SMC and AS installation to finish. The
     * timer resumes preparation on the MME event loop, never in a callback. */
    ogs_timer_start(ho->timer, ogs_time_from_msec(10));
    return OGS_OK;
}

static void target_release(mme_ue_t *ue);

static bool bearer_procedures_idle(mme_ue_t *ue)
{
    mme_sess_t *sess;
    mme_bearer_t *bearer;
    ogs_list_for_each(&ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            if (!OGS_FSM_CHECK(&bearer->sm, esm_state_active) ||
                    bearer->s10_not_admitted ||
                    (bearer->release_xact_id &&
                     ogs_gtp_xact_find_by_id(bearer->release_xact_id)) ||
                    (bearer->delete.xact_id &&
                     ogs_gtp_xact_find_by_id(bearer->delete.xact_id)) ||
                    !ogs_list_empty(&bearer->update.xact_list)) return false;
        }
    }
    return true;
}

void mme_s10_handover_tick(ogs_pool_id_t ue_id)
{
    mme_ue_t *ue = mme_ue_find_by_id(ue_id);
    handover_t *ho;
    enb_ue_t *enb;
    ogs_pkbuf_t *body;
    ogs_gtp_xact_t *xact;
    ogs_gtp2_header_t h = {0};
    OCTET_STRING_t container = {0};
    if (!ue || !(ho = ue->s10_handover)) return;
    if (ho->phase == HO_TARGET_COMPLETE && ho->gateway_updated &&
            mme_s10_sgw_relocated(ue) && !ho->direct) {
        cleanup_forwarding(ue, NULL, false);
        return;
    }
    if (ho->phase == HO_TARGET_PREPARING || ho->phase == HO_TARGET_READY) {
        if (ogs_time_now() >= ho->deadline) {
            if (ho->phase == HO_TARGET_PREPARING) {
                xact = ogs_gtp_xact_find_by_id(ho->xact_id);
                if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id))
                    relocation_response(xact, NULL, ho->remote_teid,
                            OGS_GTP2_CAUSE_RELOCATION_FAILURE, NULL);
            }
            target_release(ue);
        }
        return;
    }
    if (ho->phase == HO_SOURCE_COMMAND && ogs_time_now() >= ho->deadline) {
        enb = enb_ue_find_by_id(ho->enb_ue_id);
        if (enb) mme_s10_handover_cancel(enb);
        else source_failure(ue, NULL);
        return;
    }
    if (ho->phase != HO_SOURCE_WAIT) return;
    enb = enb_ue_find_by_id(ho->enb_ue_id);
    if (!enb || ue->enb_ue_id != enb->id || ogs_time_now() >= ho->deadline) {
        source_failure(ue, NULL);
        return;
    }
    if (ue->as_pending.valid || !OGS_FSM_CHECK(&ue->sm, emm_state_registered) ||
            !bearer_procedures_idle(ue)) {
        ogs_timer_start(ho->timer, ogs_time_from_msec(100));
        return;
    }
    if (!ue->as_security.valid || !SECURITY_CONTEXT_IS_VALID(ue) ||
            !ACTIVE_EPS_BEARERS_IS_AVAIABLE(ue) ||
            mme_as_security_next_hop(ue) != OGS_OK) {
        source_failure(ue, NULL);
        return;
    }
    /* TS 33.401 7.2.8.4.3: store the fresh pair even if preparation fails. */
    container.buf = ho->container->data;
    container.size = ho->container->len;
    body = mme_s10_build_forward_relocation_request(ue, &ho->target,
            &ho->cause, &container, ho->direct);
    if (!body) { source_failure(ue, NULL); return; }
    h.type = OGS_GTP2_FORWARD_RELOCATION_REQUEST_TYPE;
    xact = ogs_gtp_xact_local_create(ho->peer->node, &h, body,
            preparation_timeout, OGS_UINT_TO_POINTER(ue->id));
    if (!xact) { source_failure(ue, NULL); return; }
    xact->local_teid = ue->mme_s10_teid;
    xact->delete_cb = xact_deleted;
    ho->xact_id = xact->id;
    ho->phase = HO_SOURCE_PREPARING;
    ue->s10.source_transfer = true;
    enb->handover_type = S1AP_HandoverType_intralte;
    if (ogs_gtp_xact_commit(xact) != OGS_OK) source_failure(ue, NULL);
}

static void target_sgw_prepared(mme_ue_t *ue, int result)
{
    handover_t *ho = ue->s10_handover;
    enb_ue_t *radio = enb_ue_find_by_id(ho->enb_ue_id);
    ogs_gtp_xact_t *xact = ogs_gtp_xact_find_by_id(ho->xact_id);
    S1AP_HandoverType_t type = S1AP_HandoverType_intralte;
    OCTET_STRING_t container = {0};
    ogs_pkbuf_t *p;
    if (result == OGS_OK && radio && xact) {
        container.buf = ho->container->data;
        container.size = ho->container->len;
        p = s1ap_build_handover_request(radio, &type, &ho->cause, &container);
        if (p && s1ap_send_to_enb_ue(radio, p) == OGS_OK) return;
    }
    if (xact) relocation_response(xact, NULL, ho->remote_teid,
            OGS_GTP2_CAUSE_RELOCATION_FAILURE, NULL);
    target_release(ue);
}

static void relocation_request(ogs_gtp_xact_t *xact, ogs_gtp2_message_t *msg)
{
    ogs_gtp2_forward_relocation_request_t *req = &msg->forward_relocation_request;
    mme_s10_target_t target;
    S1AP_Cause_t cause;
    S1AP_HandoverType_t type = S1AP_HandoverType_intralte;
    S1AP_Source_ToTarget_TransparentContainer_t container = {0};
    mme_enb_t *enb;
    enb_ue_t *radio = NULL;
    mme_ue_t *ue = NULL;
    handover_t *ho;
    ogs_ip_t peer_ip;
    uint32_t teid = 0;
    uint8_t error = OGS_GTP2_CAUSE_MANDATORY_IE_INCORRECT;
    char imsi[OGS_MAX_IMSI_BCD_LEN + 1];
    int i;
    if (msg->h.teid || read_fteid(&req->sender_s_f_teid_for_control_plane,
                OGS_GTP2_F_TEID_S10_MME_GTP_C, &teid, &peer_ip) != OGS_OK)
        goto reject;
    if (mme_s10_decode_target(&target, &req->target_identification) != OGS_OK ||
            mme_s10_decode_cause(&cause, &req->s1_ap_cause) != OGS_OK ||
            mme_s10_decode_selected_plmn(&target.tai.plmn_id,
                &req->selected_plmn_id) != OGS_OK ||
            !req->imsi.presence || !req->imsi.data ||
            !req->imsi.len || req->imsi.len > OGS_MAX_IMSI_LEN ||
            !ogs_buffer_to_bcd(req->imsi.data, req->imsi.len, imsi, sizeof(imsi)) ||
            strspn(imsi, "0123456789") != strlen(imsi) ||
            !req->e_utran_transparent_container.presence ||
            !req->e_utran_transparent_container.data ||
            req->e_utran_transparent_container.len < 2 ||
            req->e_utran_transparent_container.len > OGS_MAX_SDU_LEN / 2 ||
            (*(uint8_t *)req->e_utran_transparent_container.data & 15) != 3)
        goto reject;
    enb = mme_enb_find_by_global_id(
            &target.enb_plmn, target.enb_type, target.enb_id);
    error = OGS_GTP2_CAUSE_RELOCATION_FAILURE;
    if (!enb || !enb->state.s1_setup_success ||
            memcmp(&enb->plmn_id, &target.enb_plmn, sizeof(enb->plmn_id)) ||
            mme_find_served_tai(&target.tai) < 0)
        goto reject;
    for (i = 0; i < enb->num_of_supported_ta_list; i++)
        if (enb->supported_ta_list[i].tac == target.tai.tac &&
                !memcmp(&enb->supported_ta_list[i].plmn_id,
                    &target.tai.plmn_id, sizeof(target.tai.plmn_id))) break;
    if (i == enb->num_of_supported_ta_list) goto reject;
    radio = enb_ue_add(enb, INVALID_UE_S1AP_ID);
    if (!radio) goto reject;
    radio->saved.tai = target.tai;
    ue = mme_ue_add(radio);
    if (!ue) { enb_ue_remove(radio); goto reject; }
    enb_ue_associate_mme_ue(radio, ue);
    ue->tai = target.tai;
    radio->saved.tai = target.tai;
    radio->handover_type = type;
    if (!mme_s10_imsi_available(ue, imsi) ||
            mme_s10_import_handover_context(ue, req) != OGS_OK ||
            mme_ue_set_imsi(ue, imsi, MME_UE_IMSI_FROM_S10_CONTEXT) != OGS_OK) {
        enb_ue_remove(radio);
        mme_ue_remove(ue);
        goto reject;
    }
    ho = allocate(ue, HO_TARGET_PREPARING);
    ho->peer = mme_s10_find_by_addr(&xact->gnode->addr);
    ho->remote_teid = teid;
    ho->xact_id = xact->id;
    xact->data = OGS_UINT_TO_POINTER(ue->id);
    xact->delete_cb = xact_deleted;
    container.buf = (uint8_t *)req->e_utran_transparent_container.data + 1;
    container.size = req->e_utran_transparent_container.len - 1;
    ho->cause = cause;
    ho->container = ogs_pkbuf_alloc(NULL, container.size);
    ogs_assert(ho->container);
    ogs_pkbuf_put_data(ho->container, container.buf, container.size);
    if (req->indication_flags.presence && req->indication_flags.data &&
            req->indication_flags.len >= 1)
        ho->direct = ((ogs_gtp2_indication_t *)req->indication_flags.data)->direct_forwarding_indication;
    ho->deadline = ogs_time_now() + ogs_time_from_sec(10);
    ogs_timer_start(ho->timer, ogs_time_from_sec(10));
    if (mme_s10_sgw_prepare(ue, target_sgw_prepared) != OGS_OK)
        target_sgw_prepared(ue, OGS_ERROR);
    return;
reject:
    relocation_response(xact, NULL, teid, error, NULL);
}

int mme_s10_handover_acknowledge(enb_ue_t *target)
{
    mme_ue_t *ue = mme_ue_find_by_id(target->mme_ue_id);
    handover_t *ho;
    ogs_gtp_xact_t *xact;
    int rv;
    mme_sess_t *sess;
    mme_bearer_t *bearer;
    bool have_default = false;
    if (!mme_s10_handover_is_target(ue) ||
            (ho = ue->s10_handover)->phase != HO_TARGET_PREPARING ||
            ho->enb_ue_id != target->id) return OGS_ERROR;
    xact = ogs_gtp_xact_find_by_id(ho->xact_id);
    if (!xact || xact->data != OGS_UINT_TO_POINTER(ue->id)) return OGS_ERROR;
    ho->admitted_ebis = 0;
    ogs_list_for_each(&ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            bearer->s10_not_admitted = !bearer->target_s1u_teid;
            if (bearer->target_s1u_teid) {
                ho->admitted_ebis |= 1 << bearer->ebi;
                if (mme_default_bearer_in_sess(sess) == bearer) have_default = true;
            }
        }
    }
    /* At least one PDN must remain usable at the target. */
    if (!have_default) {
        S1AP_Cause_t cause = {0};
        cause.present = S1AP_Cause_PR_radioNetwork;
        cause.choice.radioNetwork = S1AP_CauseRadioNetwork_no_radio_resources_available_in_target_cell;
        relocation_response(xact, NULL, ho->remote_teid,
                OGS_GTP2_CAUSE_RELOCATION_FAILURE, &cause);
        target_release(ue);
        return OGS_ERROR;
    }
    ho->admission_known = true;
    if (mme_s10_sgw_relocated(ue) && !ho->direct && mme_ue_have_indirect_tunnel(ue))
        return mme_gtp_send_create_indirect_data_forwarding_tunnel_request(target, ue);
    rv = relocation_response(xact, ue, ho->remote_teid,
            OGS_GTP2_CAUSE_REQUEST_ACCEPTED, NULL);
    if (rv == OGS_OK) {
        ho->phase = HO_TARGET_READY;
        ho->deadline = ogs_time_now() + ogs_time_from_sec(30);
        ogs_timer_start(ho->timer, ogs_time_from_sec(30));
    }
    return rv;
}

void mme_s10_handover_failure(enb_ue_t *target, const S1AP_Cause_t *cause)
{
    mme_ue_t *ue = mme_ue_find_by_id(target->mme_ue_id);
    handover_t *ho;
    ogs_gtp_xact_t *xact;
    if (!mme_s10_handover_is_target(ue)) return;
    ho = ue->s10_handover;
    if (ho->phase != HO_TARGET_PREPARING || ho->enb_ue_id != target->id) return;
    xact = ogs_gtp_xact_find_by_id(ho->xact_id);
    if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id)) {
        relocation_response(xact, NULL, ho->remote_teid,
                OGS_GTP2_CAUSE_RELOCATION_FAILURE, cause);
        ho->xact_id = 0;
    }
    enb_ue_remove(target);
    mme_ue_remove(ue);
}

static void forward_response(ogs_gtp_xact_t *xact, ogs_gtp2_message_t *msg)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    ogs_gtp2_forward_relocation_response_t *rsp = &msg->forward_relocation_response;
    handover_t *ho = ue ? ue->s10_handover : NULL;
    ogs_gtp2_cause_t *cause = rsp->cause.data;
    S1AP_Cause_t radio_cause;
    OCTET_STRING_t container = {0}, *container_ptr = &container;
    enb_ue_t *source;
    ogs_ip_t peer_ip;
    uint32_t teid;
    uint16_t seen = 0;
    struct {
        mme_bearer_t *bearer;
        ogs_ip_t dl_ip, ul_ip;
        uint32_t dl_teid, ul_teid;
    } admitted[OGS_BEARER_PER_UE] = {{0}};
    int i, n = 0, rv;
    bool matched = ho && ho->phase == HO_SOURCE_PREPARING &&
        ho->xact_id == xact->id && ho->peer->node == xact->gnode &&
        msg->h.teid == ue->mme_s10_teid;
    ogs_gtp_xact_commit(xact);
    if (!matched) return;
    ho->xact_id = OGS_INVALID_POOL_ID;
    if (!rsp->cause.presence || rsp->cause.len < 2 || !cause) goto invalid;
    if (cause->value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        source_failure(ue, mme_s10_decode_cause(&radio_cause,
                    &rsp->s1_ap_cause) == OGS_OK ? &radio_cause : NULL);
        return;
    }
    if (read_fteid(&rsp->sender_s_f_teid_for_control_plane,
                OGS_GTP2_F_TEID_S10_MME_GTP_C, &teid, &peer_ip) != OGS_OK ||
            !rsp->e_utran_transparent_container.presence ||
            !rsp->e_utran_transparent_container.data ||
            rsp->e_utran_transparent_container.len < 2 ||
            rsp->e_utran_transparent_container.len > OGS_MAX_SDU_LEN / 2 ||
            (*(uint8_t *)rsp->e_utran_transparent_container.data & 15) != 3)
        goto invalid;
    ue->s10.sgw_changed = false;
    if (rsp->indication_flags.presence) {
        if (!rsp->indication_flags.data || rsp->indication_flags.len < 1) goto invalid;
        ue->s10.sgw_changed = ((ogs_gtp2_indication_t *)rsp->indication_flags.data)->sgw_change_indication;
    }
    for (i = 0; i < OGS_BEARER_PER_UE; i++) {
        ogs_gtp2_tlv_relocation_bearer_context_t *bc = &rsp->list_of_set_up_bearers[i];
        if (ue->s10.sgw_changed && !ho->direct) {
            bc->enb_dl_forwarding_f_teid = bc->sgw_dl_forwarding_f_teid;
            bc->enb_ul_forwarding_f_teid = bc->sgw_ul_forwarding_f_teid;
        }
        uint8_t ebi;
        if (!bc->presence) break;
        ebi = bc->eps_bearer_id.u8;
        if (!bc->eps_bearer_id.presence || ebi < 5 || ebi > 15 ||
                (seen & (1 << (ebi - 5))) ||
                !(admitted[n].bearer = mme_bearer_find_by_ue_ebi(ue, ebi)))
            goto invalid;
        seen |= 1 << (ebi - 5);
        if (bc->enb_dl_forwarding_f_teid.presence &&
                read_fteid(&bc->enb_dl_forwarding_f_teid,
                    ue->s10.sgw_changed && !ho->direct ? OGS_GTP2_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING : OGS_GTP2_F_TEID_ENODEB_GTP_U_FOR_DL_DATA_FORWARDING,
                    &admitted[n].dl_teid, &admitted[n].dl_ip) != OGS_OK)
            goto invalid;
        if (bc->enb_ul_forwarding_f_teid.presence &&
                read_fteid(&bc->enb_ul_forwarding_f_teid,
                    ue->s10.sgw_changed && !ho->direct ? OGS_GTP2_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING : OGS_GTP2_F_TEID_ENODEB_GTP_U_FOR_UL_DATA_FORWARDING,
                    &admitted[n].ul_teid, &admitted[n].ul_ip) != OGS_OK)
            goto invalid;
        n++;
    }
    source = enb_ue_find_by_id(ho->enb_ue_id);
    if (!n || !source || ue->enb_ue_id != source->id) goto invalid;
    ho->remote_teid = teid;
    ho->admission_known = true;
    ho->admitted_ebis = 0;
    for (i = 0; i < n; i++) ho->admitted_ebis |= 1 << admitted[i].bearer->ebi;
    mme_ue_clear_indirect_tunnel(ue);
    for (i = 0; i < n; i++) {
        admitted[i].bearer->enb_dl_teid = admitted[i].dl_teid;
        admitted[i].bearer->enb_dl_ip = admitted[i].dl_ip;
        admitted[i].bearer->enb_ul_teid = admitted[i].ul_teid;
        admitted[i].bearer->enb_ul_ip = admitted[i].ul_ip;
    }
    container.buf = (uint8_t *)rsp->e_utran_transparent_container.data + 1;
    container.size = rsp->e_utran_transparent_container.len - 1;
    OGS_ASN_STORE_DATA(&ue->container, container_ptr);
    if (!ho->direct && mme_ue_have_indirect_tunnel(ue))
        rv = mme_gtp_send_create_indirect_data_forwarding_tunnel_request(source, ue);
    else {
        rv = s1ap_send_handover_command(source);
        if (rv == OGS_OK) ho->command_sent = true;
    }
    if (rv != OGS_OK) goto invalid;
    ho->phase = HO_SOURCE_COMMAND;
    ho->deadline = ogs_time_now() + ogs_time_from_sec(30);
    ogs_timer_start(ho->timer, ogs_time_from_sec(30));
    return;
invalid:
    source_failure(ue, NULL);
}

static mme_ue_t *find_remote(ogs_gtp_xact_t *xact, uint32_t teid)
{
    mme_ue_t *ue = ogs_hash_get(mme_self()->mme_s10_teid_hash, &teid, sizeof(teid));
    return ue && ue->s10_handover &&
        ue->s10_handover->peer->node == xact->gnode ? ue : NULL;
}

static int cause_reply(ogs_gtp_xact_t *xact, uint8_t type,
        uint32_t teid, uint8_t value)
{
    ogs_gtp2_message_t *msg = ogs_calloc(1, sizeof(*msg));
    ogs_gtp2_tlv_cause_t *ie;
    ogs_gtp2_cause_t cause = {0};
    ogs_pkbuf_t *p;
    ogs_assert(msg);
    msg->h.type = type;
    switch (type) {
    case OGS_GTP2_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE_TYPE:
        ie = &msg->forward_relocation_complete_acknowledge.cause;
        break;
    case OGS_GTP2_FORWARD_ACCESS_CONTEXT_ACKNOWLEDGE_TYPE:
        ie = &msg->forward_access_context_acknowledge.cause;
        break;
    case OGS_GTP2_RELOCATION_CANCEL_RESPONSE_TYPE:
        ie = &msg->relocation_cancel_response.cause;
        break;
    default:
        ogs_free(msg);
        return OGS_ERROR;
    }
    cause.value = value;
    ie->presence = 1;
    ie->data = &cause;
    ie->len = sizeof(cause);
    p = ogs_gtp2_build_msg(msg);
    ogs_free(msg);
    return reply(xact, type, teid, p);
}

static void completion_timeout(ogs_gtp_xact_t *xact, void *data)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(data));
    if (ue && ue->s10_handover && ue->s10_handover->complete_xact_id == xact->id) {
        ue->s10_handover->complete_xact_id = 0;
        ogs_warn("S10 relocation completion acknowledgement timed out [ue:%u]", ue->id);
    }
}

static void gateway_timeout(ogs_gtp_xact_t *xact, void *data)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(data));
    enb_ue_t *enb;
    if (!ue || !ue->s10_handover || ue->s10_handover->sgw_xact_id != xact->id) return;
    ue->s10_handover->sgw_xact_id = 0;
    enb = enb_ue_find_by_id(ue->enb_ue_id);
    if (enb) s1ap_send_ue_context_release_command(enb,
            S1AP_Cause_PR_radioNetwork, S1AP_CauseRadioNetwork_unspecified,
            S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK, 0);
}

static int update_gateway(mme_ue_t *ue)
{
    ogs_gtp2_message_t *msg = ogs_calloc(1, sizeof(*msg));
    ogs_gtp2_modify_bearer_request_t *req = &msg->modify_bearer_request;
    ogs_gtp2_f_teid_t sender = {0};
    sgw_ue_t *sgw = sgw_ue_find_by_id(ue->sgw_ue_id);
    handover_t *ho = ue->s10_handover;
    ogs_gtp_xact_t *xact;
    ogs_pkbuf_t *snapshot = NULL, *body = NULL;
    int len, rv = OGS_ERROR;
    ogs_assert(msg);
    if (!sgw || !sgw->gnode || !sgw->sgw_s11_teid ||
            !ogs_list_count(&ue->bearer_to_modify_list)) goto out;
    snapshot = mme_s11_build_modify_bearer_request(
            OGS_GTP2_MODIFY_BEARER_REQUEST_TYPE, ue, 1);
    if (!snapshot || ogs_tlv_parse_msg(req, &ogs_gtp2_tlv_desc_modify_bearer_request,
                snapshot, OGS_TLV_MODE_T1_L2_I1) != OGS_OK) goto out;
    sender.interface_type = OGS_GTP2_F_TEID_S11_MME_GTP_C;
    sender.teid = htobe32(ue->mme_s11_teid);
    if (ogs_gtp2_sockaddr_to_f_teid(ogs_gtp_self()->gtpc_addr,
                ogs_gtp_self()->gtpc_addr6, &sender, &len) != OGS_OK) goto out;
    req->sender_f_teid_for_control_plane.presence = 1;
    req->sender_f_teid_for_control_plane.data = &sender;
    req->sender_f_teid_for_control_plane.len = len;
    req->rat_type.presence = 1;
    req->rat_type.u8 = OGS_GTP2_RAT_TYPE_EUTRAN;
    req->serving_network.presence = 1;
    req->serving_network.data = &ue->tai.plmn_id;
    req->serving_network.len = sizeof(ue->tai.plmn_id);
    msg->h.type = OGS_GTP2_MODIFY_BEARER_REQUEST_TYPE;
    msg->h.teid = sgw->sgw_s11_teid;
    body = ogs_gtp2_build_msg(msg);
    if (!body) goto out;
    xact = ogs_gtp_xact_local_create(sgw->gnode, &msg->h, body,
            gateway_timeout, OGS_UINT_TO_POINTER(ue->id));
    if (!xact) goto out;
    xact->local_teid = ue->mme_s11_teid;
    xact->modify_action = OGS_GTP_MODIFY_IN_S10_HANDOVER;
    xact->enb_ue_id = ue->enb_ue_id;
    xact->delete_cb = xact_deleted;
    ho->sgw_xact_id = xact->id;
    rv = ogs_gtp_xact_commit(xact);
out:
    if (snapshot) ogs_pkbuf_free(snapshot);
    ogs_free(msg);
    return rv;
}

int mme_s10_handover_notify(enb_ue_t *target)
{
    mme_ue_t *ue = mme_ue_find_by_id(target->mme_ue_id);
    handover_t *ho = ue ? ue->s10_handover : NULL;
    ogs_gtp2_header_t h = {0};
    ogs_gtp2_message_t *msg;
    ogs_gtp_xact_t *xact;
    ogs_pkbuf_t *p;
    if (!ho || ho->phase != HO_TARGET_READY || ho->enb_ue_id != target->id)
        return OGS_ERROR;
    msg = ogs_calloc(1, sizeof(*msg));
    ogs_assert(msg);
    msg->h.type = OGS_GTP2_FORWARD_RELOCATION_COMPLETE_NOTIFICATION_TYPE;
    p = ogs_gtp2_build_msg(msg);
    ogs_free(msg);
    if (!p) return OGS_ERROR;
    h.type = OGS_GTP2_FORWARD_RELOCATION_COMPLETE_NOTIFICATION_TYPE;
    h.teid = ho->remote_teid;
    xact = ogs_gtp_xact_local_create(ho->peer->node, &h, p,
            completion_timeout, OGS_UINT_TO_POINTER(ue->id));
    if (!xact) return OGS_ERROR;
    xact->local_teid = ue->mme_s10_teid;
    xact->delete_cb = xact_deleted;
    ho->complete_xact_id = xact->id;
    ho->phase = HO_TARGET_COMPLETE;
    ogs_timer_stop(ho->timer);
    OGS_FSM_TRAN(&ue->sm, &emm_state_registered);
    if (ogs_gtp_xact_commit(xact) != OGS_OK) return OGS_ERROR;
    return update_gateway(ue);
}

static void start_hss_registration(mme_ue_t *ue)
{
    handover_t *ho = ue->s10_handover;
    enb_ue_t *enb;
    if (!ho || !ho->gateway_updated || !ho->tau_pending || ho->hss_update_started) return;
    enb = enb_ue_find_by_id(ue->enb_ue_id);
    if (!enb) return;
    ho->hss_update_started = true;
    ho->tau_pending = false;
    mme_s6a_send_ulr(enb, ue, 0);
}

void mme_s10_handover_sgw_response(ogs_gtp_xact_t *xact, mme_ue_t *ue,
        ogs_gtp2_modify_bearer_response_t *rsp)
{
    handover_t *ho = ue ? ue->s10_handover : NULL;
    ogs_gtp2_cause_t *cause = rsp->cause.data;
    bool matched = ho && ho->phase == HO_TARGET_COMPLETE &&
        ho->sgw_xact_id == xact->id &&
        xact->data == OGS_UINT_TO_POINTER(ue->id);
    enb_ue_t *enb;
    ogs_gtp_xact_commit(xact);
    if (!matched) return;
    ho->sgw_xact_id = 0;
    if (!rsp->cause.presence || !cause || rsp->cause.len < 2 ||
            cause->value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED) {
        enb = enb_ue_find_by_id(ue->enb_ue_id);
        if (enb) s1ap_send_ue_context_release_command(enb,
                S1AP_Cause_PR_radioNetwork, S1AP_CauseRadioNetwork_unspecified,
                S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK, 0);
        return;
    }
    /* TS 23.401 5.5.1.2.2 step 15: after switching the serving MME,
     * release the PDNs and dedicated bearers the target did not admit. */
    {
        mme_sess_t *sess;
        mme_bearer_t *bearer;
        sgw_ue_t *sgw = sgw_ue_find_by_id(ue->sgw_ue_id);
        ogs_list_for_each(&ue->sess_list, sess) {
            mme_bearer_t *def = mme_default_bearer_in_sess(sess);
            if (def->s10_not_admitted) {
                if (mme_gtp_send_delete_session_request(
                        enb_ue_find_by_id(ue->enb_ue_id), sgw, sess,
                        OGS_GTP_DELETE_SEND_DEACTIVATE_BEARER_CONTEXT_REQUEST) == OGS_OK)
                    OGS_FSM_TRAN(&def->sm, esm_state_pdn_will_disconnect);
                continue;
            }
            ogs_list_for_each(&sess->bearer_list, bearer)
                if (bearer->s10_not_admitted)
                    mme_gtp_send_delete_bearer_command(bearer);
        }
    }
    mme_s10_sgw_committed(ue);
    ho->gateway_updated = true;
    if (mme_s10_sgw_relocated(ue) && !ho->direct)
        ogs_timer_start(ho->timer, ogs_time_from_sec(10));
    ogs_info("S10 handover gateway updated [ue:%u]", ue->id);
    start_hss_registration(ue);
}

static void source_holding_expired(void *data)
{
    mme_event_t *e = mme_event_new(MME_EVENT_S10_TIMER);
    ogs_assert(e);
    e->mme_ue_id = OGS_POINTER_TO_UINT(data);
    if (ogs_queue_push(ogs_app()->queue, e) != OGS_OK) mme_event_free(e);
}

static void relocation_complete(ogs_gtp_xact_t *xact, ogs_gtp2_message_t *msg)
{
    mme_ue_t *ue = find_remote(xact, msg->h.teid);
    handover_t *ho = ue ? ue->s10_handover : NULL;
    enb_ue_t *source;
    if (!ho || (ho->phase != HO_SOURCE_COMMAND && ho->phase != HO_SOURCE_CANCELLING &&
                ho->phase != HO_SOURCE_COMPLETE)) {
        cause_reply(xact, OGS_GTP2_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE_TYPE,
                ho ? ho->remote_teid : 0, OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND);
        return;
    }
    cause_reply(xact, OGS_GTP2_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE_TYPE,
            ho->remote_teid, OGS_GTP2_CAUSE_REQUEST_ACCEPTED);
    if (ho->phase == HO_SOURCE_COMPLETE) return;
    if (ho->phase == HO_SOURCE_CANCELLING) {
        source = enb_ue_find_by_id(ho->enb_ue_id);
        if (source) s1ap_send_handover_cancel_ack(source);
    }
    ho->phase = HO_SOURCE_COMPLETE;
    CLEAR_MME_UE_ALL_TIMERS(ue);
    ue->s10.source_transfer = true;
    ue->s10.acknowledged = true;
    source = enb_ue_find_by_id(ho->enb_ue_id);
    if (source) s1ap_send_ue_context_release_command(source,
            S1AP_Cause_PR_radioNetwork, S1AP_CauseRadioNetwork_successful_handover,
            S1AP_UE_CTX_REL_S1_REMOVE_AND_UNLINK,
            ogs_local_conf()->time.handover.duration);
    if (!ue->s10.holding) ue->s10.holding = ogs_timer_add(ogs_app()->timer_mgr,
            source_holding_expired, OGS_UINT_TO_POINTER(ue->id));
    ogs_assert(ue->s10.holding);
    ogs_timer_start(ue->s10.holding, ogs_time_from_sec(10));
}

static void relocation_complete_ack(ogs_gtp_xact_t *xact, ogs_gtp2_message_t *msg)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    handover_t *ho = ue ? ue->s10_handover : NULL;
    bool matched = ho && ho->complete_xact_id == xact->id &&
        msg->h.teid == ue->mme_s10_teid && ho->peer->node == xact->gnode;
    ogs_gtp_xact_commit(xact);
    if (matched) ho->complete_xact_id = 0;
}

bool mme_s10_handover_tau(mme_ue_t *ue,
        const ogs_nas_eps_tracking_area_update_request_t *req)
{
    handover_t *ho = ue->s10_handover;
    enb_ue_t *enb;
    if (!ho || ho->phase != HO_TARGET_COMPLETE || ho->hss_update_started)
        return false;
    enb = enb_ue_find_by_id(ue->enb_ue_id);
    if (!enb) return false;
    /* The radio path already exists. TS 23.401 5.5.1.2.2 step 18 excludes
     * a second inter-MME Context Request; perform HSS registration and GUTI
     * allocation on the transferred context instead. */
    ue->tracking_area_update_request_presencemask = req->presencemask;
    ue->tracking_area_update_request_ebcs_value = req->eps_bearer_context_status.value;
    ue->tracking_area_update_accept_proc = S1AP_ProcedureCode_id_downlinkNASTransport;
    ue->s10.target_transfer = true;
    ho->tau_pending = true;
    if (!MME_NEXT_GUTI_IS_AVAILABLE(ue)) mme_ue_new_guti(ue);
    OGS_FSM_TRAN(&ue->sm, &emm_state_initial_context_setup);
    start_hss_registration(ue);
    return true;
}

static void status_timeout(ogs_gtp_xact_t *xact, void *data)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(data));
    if (ue && ue->s10_handover && ue->s10_handover->status_xact_id == xact->id) {
        ue->s10_handover->status_xact_id = 0;
        ogs_warn("S10 status transfer acknowledgement timed out [ue:%u]", ue->id);
    }
}

int mme_s10_handover_status(enb_ue_t *source,
        const S1AP_ENB_StatusTransfer_TransparentContainer_t *container)
{
    mme_ue_t *ue = mme_ue_find_by_id(source->mme_ue_id);
    handover_t *ho = ue ? ue->s10_handover : NULL;
    S1AP_ENB_StatusTransfer_TransparentContainer_t copy = {0};
    ogs_gtp2_message_t *msg;
    ogs_gtp2_header_t h = {0};
    ogs_gtp_xact_t *xact;
    ogs_pkbuf_t *encoded, *body;
    uint8_t *fcontainer;
    int rv;
    if (!ho || ho->phase != HO_SOURCE_COMMAND || !ho->remote_teid ||
            ho->enb_ue_id != source->id || ho->status_xact_id) return OGS_ERROR;
    if (ogs_asn_copy_ie(&asn_DEF_S1AP_ENB_StatusTransfer_TransparentContainer,
                (void *)container, &copy) != OGS_OK) return OGS_ERROR;
    encoded = ogs_asn_encode(&asn_DEF_S1AP_ENB_StatusTransfer_TransparentContainer, &copy);
    if (!encoded) return OGS_ERROR;
    if (encoded->len > OGS_MAX_SDU_LEN / 2) {
        ogs_pkbuf_free(encoded);
        return OGS_ERROR;
    }
    msg = ogs_calloc(1, sizeof(*msg));
    fcontainer = ogs_malloc(encoded->len + 1);
    ogs_assert(msg && fcontainer);
    fcontainer[0] = 3;
    memcpy(fcontainer + 1, encoded->data, encoded->len);
    msg->h.type = OGS_GTP2_FORWARD_ACCESS_CONTEXT_NOTIFICATION_TYPE;
    msg->forward_access_context_notification.e_utran_transparent_container.presence = 1;
    msg->forward_access_context_notification.e_utran_transparent_container.data = fcontainer;
    msg->forward_access_context_notification.e_utran_transparent_container.len = encoded->len + 1;
    body = ogs_gtp2_build_msg(msg);
    ogs_free(fcontainer);
    ogs_free(msg);
    ogs_pkbuf_free(encoded);
    if (!body) return OGS_ERROR;
    h.type = OGS_GTP2_FORWARD_ACCESS_CONTEXT_NOTIFICATION_TYPE;
    h.teid = ho->remote_teid;
    xact = ogs_gtp_xact_local_create(ho->peer->node, &h, body,
            status_timeout, OGS_UINT_TO_POINTER(ue->id));
    if (!xact) return OGS_ERROR;
    xact->local_teid = ue->mme_s10_teid;
    xact->delete_cb = xact_deleted;
    ho->status_xact_id = xact->id;
    rv = ogs_gtp_xact_commit(xact);
    return rv;
}

static void forward_status(ogs_gtp_xact_t *xact, ogs_gtp2_message_t *msg)
{
    mme_ue_t *ue = find_remote(xact, msg->h.teid);
    handover_t *ho = ue ? ue->s10_handover : NULL;
    ogs_tlv_octet_t *ie = &msg->forward_access_context_notification.e_utran_transparent_container;
    S1AP_ENB_StatusTransfer_TransparentContainer_t container = {0};
    ogs_pkbuf_t *p;
    enb_ue_t *target;
    uint8_t cause = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    if (!ho || (ho->phase != HO_TARGET_READY && ho->phase != HO_TARGET_COMPLETE)) goto out;
    cause = OGS_GTP2_CAUSE_MANDATORY_IE_INCORRECT;
    if (!ie->presence || !ie->data || ie->len < 2 ||
            ie->len > OGS_MAX_SDU_LEN / 2 || (*(uint8_t *)ie->data & 15) != 3)
        goto out;
    target = enb_ue_find_by_id(ho->enb_ue_id);
    if (!target) goto out;
    p = ogs_pkbuf_alloc(NULL, ie->len - 1);
    ogs_assert(p);
    ogs_pkbuf_put_data(p, (uint8_t *)ie->data + 1, ie->len - 1);
    if (ogs_asn_decode(&asn_DEF_S1AP_ENB_StatusTransfer_TransparentContainer,
                &container, sizeof(container), p) == OGS_OK) {
        cause = s1ap_send_mme_status_transfer(target, &container) == OGS_OK ?
            OGS_GTP2_CAUSE_REQUEST_ACCEPTED : OGS_GTP2_CAUSE_SYSTEM_FAILURE;
    }
    ogs_asn_free(&asn_DEF_S1AP_ENB_StatusTransfer_TransparentContainer, &container);
    ogs_pkbuf_free(p);
out:
    cause_reply(xact, OGS_GTP2_FORWARD_ACCESS_CONTEXT_ACKNOWLEDGE_TYPE,
            ho ? ho->remote_teid : 0, cause);
}

static void forward_status_ack(ogs_gtp_xact_t *xact, ogs_gtp2_message_t *msg)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    handover_t *ho = ue ? ue->s10_handover : NULL;
    bool matched = ho && ho->status_xact_id == xact->id &&
        ho->peer->node == xact->gnode && msg->h.teid == ue->mme_s10_teid;
    ogs_gtp_xact_commit(xact);
    if (matched) ho->status_xact_id = 0;
}


static void cancel_finished(mme_ue_t *ue)
{
    handover_t *ho = ue->s10_handover;
    enb_ue_t *source = enb_ue_find_by_id(ho->enb_ue_id);
    mme_sess_t *sess;
    mme_bearer_t *bearer;
    bool indirect = ho->indirect_uncertain;
    if (ho->indirect_xact_id) {
        ho->cancel_complete = true;
        return;
    }
    ogs_list_for_each(&ue->sess_list, sess)
        ogs_list_for_each(&sess->bearer_list, bearer)
            if (MME_HAVE_SGW_DL_INDIRECT_TUNNEL(bearer) ||
                    MME_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer)) indirect = true;
    /* The cancellation phase suppresses HandoverPreparationFailure. */
    ue->s10_handover->phase = HO_SOURCE_CANCELLING;
    source_failure(ue, NULL);
    if (!source) return;
    if (indirect) cleanup_forwarding(ue, source, true);
    else {
        mme_ue_clear_indirect_tunnel(ue);
        s1ap_send_handover_cancel_ack(source);
    }
}

static void cancel_timeout(ogs_gtp_xact_t *xact, void *data)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(data));
    if (ue && ue->s10_handover && ue->s10_handover->cancel_xact_id == xact->id) {
        ue->s10_handover->cancel_xact_id = 0;
        cancel_finished(ue);
    }
}

int mme_s10_handover_cancel(enb_ue_t *source)
{
    mme_ue_t *ue = mme_ue_find_by_id(source->mme_ue_id);
    handover_t *ho = ue ? ue->s10_handover : NULL;
    ogs_gtp2_message_t *msg;
    ogs_gtp2_header_t h = {0};
    ogs_pkbuf_t *p;
    ogs_gtp_xact_t *xact;
    if (!ho || ho->enb_ue_id != source->id ||
            ho->phase > HO_SOURCE_CANCELLING) return OGS_ERROR;
    if (ho->phase == HO_SOURCE_WAIT) { cancel_finished(ue); return OGS_OK; }
    if (ho->phase == HO_SOURCE_CANCELLING) return OGS_OK;
    msg = ogs_calloc(1, sizeof(*msg));
    ogs_assert(msg);
    msg->h.type = OGS_GTP2_RELOCATION_CANCEL_REQUEST_TYPE;
    msg->relocation_cancel_request.imsi.presence = 1;
    msg->relocation_cancel_request.imsi.data = ue->imsi;
    msg->relocation_cancel_request.imsi.len = ue->imsi_len;
    p = ogs_gtp2_build_msg(msg);
    ogs_free(msg);
    if (!p) return OGS_ERROR;
    h.type = OGS_GTP2_RELOCATION_CANCEL_REQUEST_TYPE;
    h.teid = ho->remote_teid;
    xact = ogs_gtp_xact_local_create(ho->peer->node, &h, p,
            cancel_timeout, OGS_UINT_TO_POINTER(ue->id));
    if (!xact) return OGS_ERROR;
    ho->cancel_xact_id = xact->id;
    ho->phase = HO_SOURCE_CANCELLING;
    xact->local_teid = ue->mme_s10_teid;
    xact->delete_cb = xact_deleted;
    return ogs_gtp_xact_commit(xact);
}

static void target_release(mme_ue_t *ue)
{
    handover_t *ho = ue->s10_handover;
    enb_ue_t *radio = enb_ue_find_by_id(ho->enb_ue_id);
    /* Cached response views do not own the UE. Preserve duplicate handling. */
    ho->xact_id = 0;
    ho->phase = HO_TARGET_RELEASING;
    mme_s10_sgw_clear(ue);
    ogs_timer_stop(ho->timer);
    if (radio && radio->enb_ue_s1ap_id != INVALID_UE_S1AP_ID &&
            s1ap_send_ue_context_release_command(radio,
                S1AP_Cause_PR_radioNetwork, S1AP_CauseRadioNetwork_handover_cancelled,
                S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE, 0) == OGS_OK) return;
    if (radio) enb_ue_remove(radio);
    /* These are imported reservations. Never delete the source SGW/PGW session. */
    mme_ue_remove(ue);
}

static void relocation_cancel(ogs_gtp_xact_t *xact, ogs_gtp2_message_t *msg)
{
    mme_ue_t *ue = NULL;
    handover_t *ho;
    ogs_tlv_octet_t *imsi = &msg->relocation_cancel_request.imsi;
    char bcd[OGS_MAX_IMSI_BCD_LEN + 1];
    uint8_t cause = OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND;
    uint32_t teid = 0;
    if (msg->h.teid) ue = find_remote(xact, msg->h.teid);
    else if (imsi->presence && imsi->data && imsi->len && imsi->len <= OGS_MAX_IMSI_LEN &&
            ogs_buffer_to_bcd(imsi->data, imsi->len, bcd, sizeof(bcd))) {
        ue = mme_ue_find_by_imsi_bcd(bcd);
        if (ue && (!ue->s10_handover || ue->s10_handover->peer->node != xact->gnode))
            ue = NULL;
    }
    ho = ue ? ue->s10_handover : NULL;
    if (ho && (ho->phase == HO_TARGET_PREPARING || ho->phase == HO_TARGET_READY)) {
        teid = ho->remote_teid;
        cause = OGS_GTP2_CAUSE_REQUEST_ACCEPTED;
    } else if (ho) {
        teid = ho->remote_teid;
        cause = OGS_GTP2_CAUSE_RELOCATION_FAILURE;
    }
    cause_reply(xact, OGS_GTP2_RELOCATION_CANCEL_RESPONSE_TYPE, teid, cause);
    if (cause == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) target_release(ue);
}

static void relocation_cancel_response(ogs_gtp_xact_t *xact, ogs_gtp2_message_t *msg)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    handover_t *ho = ue ? ue->s10_handover : NULL;
    ogs_gtp2_cause_t *cause = msg->relocation_cancel_response.cause.data;
    bool accepted = msg->relocation_cancel_response.cause.presence && cause &&
        msg->relocation_cancel_response.cause.len >= 2 &&
        (cause->value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED ||
         cause->value == OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND);
    bool matched = ho && ho->phase == HO_SOURCE_CANCELLING &&
        ho->cancel_xact_id == xact->id && ho->peer->node == xact->gnode &&
        msg->h.teid == ue->mme_s10_teid;
    ogs_gtp_xact_commit(xact);
    if (matched) {
        ho->cancel_xact_id = 0;
        if (accepted) cancel_finished(ue);
        else {
            enb_ue_t *source = enb_ue_find_by_id(ho->enb_ue_id);
            /* A completed relocation cannot be rolled back by a late cancel. */
            ho->phase = HO_SOURCE_COMMAND;
            if (source) s1ap_send_handover_cancel_ack(source);
        }
    }
}

static void indirect_timeout(ogs_gtp_xact_t *xact, void *data)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(data));
    handover_t *ho = ue ? ue->s10_handover : NULL;
    if (!ho || ho->indirect_xact_id != xact->id) return;
    ho->indirect_xact_id = 0;
    ho->indirect_uncertain = true;
    if (ho->phase == HO_SOURCE_CANCELLING && ho->cancel_complete)
        cancel_finished(ue);
    else if (ho->phase == HO_TARGET_PREPARING) target_sgw_prepared(ue, OGS_ERROR);
    else source_failure(ue, NULL);
}

void mme_s10_handover_bind_indirect(mme_ue_t *ue, ogs_gtp_xact_t *xact)
{
    handover_t *ho = ue->s10_handover;
    if (!ho || (ho->phase != HO_SOURCE_PREPARING && ho->phase != HO_TARGET_PREPARING) || ho->direct) return;
    ho->indirect_xact_id = xact->id;
    xact->cb = indirect_timeout;
    xact->delete_cb = xact_deleted;
    xact->local_teid = ue->mme_s11_teid;
}

bool mme_s10_handover_indirect_response(ogs_gtp_xact_t *xact,
        const ogs_gtp2_message_t *msg)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    handover_t *ho = ue ? ue->s10_handover : NULL;
    const ogs_gtp2_create_indirect_data_forwarding_tunnel_response_t *rsp =
        &msg->create_indirect_data_forwarding_tunnel_response;
    ogs_gtp2_cause_t *cause = rsp->cause.data;
    enb_ue_t *source;
    int i, n = 0;
    uint16_t seen = 0;
    bool valid;
    struct {
        mme_bearer_t *bearer;
        uint32_t dl_teid, ul_teid;
        ogs_ip_t dl_ip, ul_ip;
    } forwarding[OGS_BEARER_PER_UE] = {{0}};
    if (!ho || ho->indirect_xact_id != xact->id) return false;
    valid = msg->h.teid == ue->mme_s11_teid && rsp->cause.presence &&
        cause && rsp->cause.len >= 2 && cause->value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED;
    for (i = 0; valid && i < OGS_BEARER_PER_UE; i++) {
        const ogs_gtp2_tlv_bearer_context_t *bc = &rsp->bearer_contexts[i];
        uint8_t ebi = bc->eps_bearer_id.u8;
        if (!bc->presence) break;
        if (!bc->eps_bearer_id.presence || ebi < 5 || ebi > 15 ||
                (seen & (1 << (ebi - 5))) ||
                !(forwarding[n].bearer = mme_bearer_find_by_ue_ebi(ue, ebi))) {
            valid = false;
            break;
        }
        seen |= 1 << (ebi - 5);
        if (bc->s4_u_sgsn_f_teid.presence && read_fteid(&bc->s4_u_sgsn_f_teid,
                    OGS_GTP2_F_TEID_SGW_GTP_U_FOR_DL_DATA_FORWARDING,
                    &forwarding[n].dl_teid, &forwarding[n].dl_ip) != OGS_OK) valid = false;
        if (bc->s2b_u_epdg_f_teid_5.presence && read_fteid(&bc->s2b_u_epdg_f_teid_5,
                    OGS_GTP2_F_TEID_SGW_GTP_U_FOR_UL_DATA_FORWARDING,
                    &forwarding[n].ul_teid, &forwarding[n].ul_ip) != OGS_OK) valid = false;
        if ((forwarding[n].bearer->enb_dl_teid && !forwarding[n].dl_teid) ||
                (forwarding[n].bearer->enb_ul_teid && !forwarding[n].ul_teid)) valid = false;
        n++;
    }
    ogs_gtp_xact_commit(xact);
    ho->indirect_xact_id = 0;
    if (!valid || !n) {
        ho->indirect_uncertain = !rsp->cause.presence || !cause || rsp->cause.len < 2 ||
            (cause->value != OGS_GTP2_CAUSE_TEMPORARILY_REJECTED_DUE_TO_HANDOVER_IN_PROGRESS &&
             cause->value != OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND);
        if (ho->phase == HO_SOURCE_CANCELLING && ho->cancel_complete) cancel_finished(ue);
        else if (ho->phase == HO_TARGET_PREPARING) target_sgw_prepared(ue, OGS_ERROR);
        else source_failure(ue, NULL);
        return true;
    }
    ho->indirect_uncertain = true;
    for (i = 0; i < n; i++) {
        forwarding[i].bearer->sgw_dl_teid = forwarding[i].dl_teid;
        forwarding[i].bearer->sgw_dl_ip = forwarding[i].dl_ip;
        forwarding[i].bearer->sgw_ul_teid = forwarding[i].ul_teid;
        forwarding[i].bearer->sgw_ul_ip = forwarding[i].ul_ip;
    }
    if (ho->phase == HO_TARGET_PREPARING) {
        ogs_gtp_xact_t *request = ogs_gtp_xact_find_by_id(ho->xact_id);
        if (!request || relocation_response(request, ue, ho->remote_teid,
                    OGS_GTP2_CAUSE_REQUEST_ACCEPTED, NULL) != OGS_OK) {
            target_sgw_prepared(ue, OGS_ERROR);
            return true;
        }
        ho->phase = HO_TARGET_READY;
        ho->deadline = ogs_time_now() + ogs_time_from_sec(30);
        ogs_timer_start(ho->timer, ogs_time_from_sec(30));
        return true;
    }
    if (ho->phase == HO_SOURCE_CANCELLING) {
        if (ho->cancel_complete) cancel_finished(ue);
        return true;
    }
    source = enb_ue_find_by_id(ho->enb_ue_id);
    if (!source || s1ap_send_handover_command(source) != OGS_OK)
        source_failure(ue, NULL);
    else ho->command_sent = true;
    return true;
}

static void cleanup_timeout(ogs_gtp_xact_t *xact, void *data)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(data));
    enb_ue_t *source;
    if (!ue) return;
    if (ue->s10_handover && ue->s10_handover->cleanup_xact_id == xact->id)
        ue->s10_handover->cleanup_xact_id = 0;
    ogs_warn("S10 forwarding cleanup timed out [ue:%u]", ue->id);
    if (xact->delete_indirect_action == OGS_GTP_DELETE_INDIRECT_S10_CANCEL) {
        source = enb_ue_find_by_id(xact->enb_ue_id);
        if (source) s1ap_send_handover_cancel_ack(source);
    }
}

static int cleanup_forwarding(mme_ue_t *ue, enb_ue_t *source, bool cancel)
{
    sgw_ue_t *sgw = sgw_ue_find_by_id(ue->sgw_ue_id);
    ogs_gtp2_header_t h = {0};
    ogs_pkbuf_t *p;
    ogs_gtp_xact_t *xact;
    if (!sgw || !sgw->gnode || !sgw->sgw_s11_teid) return OGS_ERROR;
    p = ogs_pkbuf_alloc(NULL, OGS_TLV_MAX_HEADROOM);
    ogs_assert(p);
    ogs_pkbuf_reserve(p, OGS_TLV_MAX_HEADROOM);
    h.type = OGS_GTP2_DELETE_INDIRECT_DATA_FORWARDING_TUNNEL_REQUEST_TYPE;
    h.teid = sgw->sgw_s11_teid;
    xact = ogs_gtp_xact_local_create(sgw->gnode, &h, p,
            cleanup_timeout, OGS_UINT_TO_POINTER(ue->id));
    if (!xact) return OGS_ERROR;
    xact->local_teid = ue->mme_s11_teid;
    xact->enb_ue_id = source ? source->id : OGS_INVALID_POOL_ID;
    xact->delete_indirect_action = cancel ?
        OGS_GTP_DELETE_INDIRECT_S10_CANCEL : OGS_GTP_DELETE_INDIRECT_S10_COMPLETE;
    if (ue->s10_handover) {
        ue->s10_handover->cleanup_xact_id = xact->id;
        xact->delete_cb = xact_deleted;
    }
    return ogs_gtp_xact_commit(xact);
}

void mme_s10_handover_source_released(mme_ue_t *ue, ogs_pool_id_t enb_ue_id)
{
    handover_t *ho = ue->s10_handover;
    mme_sess_t *sess;
    mme_bearer_t *bearer;
    if (!ho || ho->phase != HO_SOURCE_COMPLETE || ho->enb_ue_id != enb_ue_id ||
            ho->direct || ho->cleanup_xact_id) return;
    ogs_list_for_each(&ue->sess_list, sess) {
        ogs_list_for_each(&sess->bearer_list, bearer) {
            if (MME_HAVE_SGW_DL_INDIRECT_TUNNEL(bearer) ||
                    MME_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer)) {
                cleanup_forwarding(ue, NULL, false);
                return;
            }
        }
    }
}

void mme_s10_handover_cleanup_response(ogs_gtp_xact_t *xact,
        const ogs_gtp2_message_t *msg)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    enb_ue_t *source = enb_ue_find_by_id(xact->enb_ue_id);
    ogs_gtp2_cause_t *cause = msg->delete_indirect_data_forwarding_tunnel_response.cause.data;
    bool accepted = ue && cause &&
        msg->delete_indirect_data_forwarding_tunnel_response.cause.presence &&
        msg->delete_indirect_data_forwarding_tunnel_response.cause.len >= 2 &&
        ((msg->h.teid == ue->mme_s11_teid && cause->value == OGS_GTP2_CAUSE_REQUEST_ACCEPTED) ||
         cause->value == OGS_GTP2_CAUSE_CONTEXT_NOT_FOUND);
    bool cancel = xact->delete_indirect_action == OGS_GTP_DELETE_INDIRECT_S10_CANCEL;
    ogs_gtp_xact_commit(xact);
    if (accepted) {
        mme_ue_clear_indirect_tunnel(ue);
        if (ue->s10_handover) ue->s10_handover->indirect_uncertain = false;
    }
    else ogs_warn("S10 forwarding cleanup rejected; retain gateway sessions");
    /* The previous MME is never allowed to delete the target's PDNs here. */
    if (cancel && source) s1ap_send_handover_cancel_ack(source);
}

void mme_s10_handover_receive(ogs_gtp_xact_t *xact, ogs_gtp2_message_t *msg)
{
    switch (msg->h.type) {
    case OGS_GTP2_FORWARD_RELOCATION_REQUEST_TYPE:
        relocation_request(xact, msg);
        break;
    case OGS_GTP2_FORWARD_RELOCATION_RESPONSE_TYPE:
        forward_response(xact, msg);
        break;
    case OGS_GTP2_FORWARD_RELOCATION_COMPLETE_NOTIFICATION_TYPE:
        relocation_complete(xact, msg);
        break;
    case OGS_GTP2_FORWARD_RELOCATION_COMPLETE_ACKNOWLEDGE_TYPE:
        relocation_complete_ack(xact, msg);
        break;
    case OGS_GTP2_FORWARD_ACCESS_CONTEXT_NOTIFICATION_TYPE:
        forward_status(xact, msg);
        break;
    case OGS_GTP2_FORWARD_ACCESS_CONTEXT_ACKNOWLEDGE_TYPE:
        forward_status_ack(xact, msg);
        break;
    case OGS_GTP2_RELOCATION_CANCEL_REQUEST_TYPE:
        relocation_cancel(xact, msg);
        break;
    case OGS_GTP2_RELOCATION_CANCEL_RESPONSE_TYPE:
        relocation_cancel_response(xact, msg);
        break;
    default:
        ogs_warn("Unhandled connected S10 procedure [%u]", msg->h.type);
        break;
    }
}
