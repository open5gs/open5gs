/* Copyright (C) 2026 Open5GS contributors
 * SPDX-License-Identifier: AGPL-3.0-or-later */
#include "mme-s10.h"
#include "mme-s11-build.h"

typedef struct mme_s10_sgw_s {
    ogs_pool_id_t xact_id;
    ogs_pool_id_t sess_id;
    mme_s10_sgw_ready_t ready;
    bool committed;
    bool idle;
    uint16_t switched_ebis;
} relocation_t;

static void delete_timeout(ogs_gtp_xact_t *xact, void *data)
{
    ogs_warn("S10 old/reserved SGW session cleanup timed out");
}

/* Local-only deletion must outlive both MMEs' UE contexts. Never route it
 * through the ordinary detach response path, which removes the PDN at the MME. */
static void delete_local(mme_ue_t *ue)
{
    sgw_ue_t *sgw = sgw_ue_find_by_id(ue->sgw_ue_id);
    mme_sess_t *sess;
    if (!sgw || !sgw->gnode || !sgw->sgw_s11_teid) return;
    ogs_list_for_each(&ue->sess_list, sess) {
        ogs_gtp2_header_t h = {0};
        ogs_gtp_xact_t *xact;
        mme_bearer_t *def = mme_default_bearer_in_sess(sess);
        bool release_pdn = (ue->s10.sgw_changed &&
            mme_s10_handover_bearer_rejected(ue, def->ebi)) ||
            (ue->s10_sgw && (ue->s10_sgw->switched_ebis & (1 << def->ebi)));
        ogs_pkbuf_t *p = mme_s11_build_delete_session_request(
                OGS_GTP2_DELETE_SESSION_REQUEST_TYPE, sess,
                release_pdn ? OGS_GTP_DELETE_NO_ACTION : OGS_GTP_DELETE_IN_PATH_SWITCH_REQUEST);
        if (!p) continue;
        h.type = OGS_GTP2_DELETE_SESSION_REQUEST_TYPE;
        h.teid = sgw->sgw_s11_teid;
        xact = ogs_gtp_xact_local_create(sgw->gnode, &h, p,
                delete_timeout, NULL);
        if (!xact) continue;
        xact->local_teid = ue->mme_s11_teid;
        xact->delete_action = OGS_GTP_DELETE_IN_S10_RELOCATION;
        ogs_gtp_xact_commit(xact);
    }
}

void mme_s10_sgw_delete_old(mme_ue_t *ue)
{
    if (ue->s10.sgw_changed) {
        delete_local(ue);
        ue->s10.sgw_changed = false;
    }
}

bool mme_s10_sgw_relocated(const mme_ue_t *ue)
{
    return ue && ue->s10_sgw != NULL;
}

void mme_s10_sgw_committed(mme_ue_t *ue)
{
    if (ue->s10_sgw) ue->s10_sgw->committed = true;
}

void mme_s10_sgw_clear(mme_ue_t *ue)
{
    relocation_t *r = ue->s10_sgw;
    ogs_gtp_xact_t *xact;
    if (!r) return;
    xact = ogs_gtp_xact_find_by_id(r->xact_id);
    if (xact && xact->data == OGS_UINT_TO_POINTER(ue->id))
        ogs_gtp_xact_cancel(xact);
    if (!r->committed) delete_local(ue);
    ogs_free(r);
    ue->s10_sgw = NULL;
    ue->s10_selected_sgw = NULL;
}

static void failed(ogs_gtp_xact_t *xact, void *data)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(data));
    relocation_t *r = ue ? ue->s10_sgw : NULL;
    mme_s10_sgw_ready_t ready;
    if (!r || r->xact_id != xact->id) return;
    r->xact_id = 0;
    ready = r->ready;
    r->ready = NULL;
    if (ready) ready(ue, OGS_ERROR);
}

static int create(mme_ue_t *ue, mme_sess_t *sess)
{
    sgw_ue_t *sgw = sgw_ue_find_by_id(ue->sgw_ue_id);
    ogs_gtp2_header_t h = {0};
    ogs_pkbuf_t *p;
    ogs_gtp_xact_t *xact;
    int action = ue->s10_sgw->idle ? OGS_GTP_CREATE_IN_S10_TAU_RELOCATION :
        OGS_GTP_CREATE_IN_S10_RELOCATION;
    h.type = OGS_GTP2_CREATE_SESSION_REQUEST_TYPE;
    h.teid = sgw->sgw_s11_teid;
    p = mme_s11_build_create_session_request(h.type, sess, action);
    if (!p) return OGS_ERROR;
    xact = ogs_gtp_xact_local_create(sgw->gnode, &h, p, failed,
            OGS_UINT_TO_POINTER(ue->id));
    if (!xact) return OGS_ERROR;
    xact->create_action = action;
    xact->local_teid = ue->mme_s11_teid;
    ue->s10_sgw->xact_id = xact->id;
    ue->s10_sgw->sess_id = sess->id;
    return ogs_gtp_xact_commit(xact);
}

int mme_s10_sgw_prepare(mme_ue_t *ue, mme_s10_sgw_ready_t ready)
{
    sgw_ue_t *sgw = sgw_ue_find_by_id(ue->sgw_ue_id);
    if (!sgw || !ue->s10_selected_sgw || ue->s10_sgw) return OGS_ERROR;
    if (!ue->s10_sgw_change_required) {
        ready(ue, OGS_OK);
        return OGS_OK;
    }
    ue->s10_sgw = ogs_calloc(1, sizeof(relocation_t));
    ogs_assert(ue->s10_sgw);
    ue->s10_sgw->ready = ready;
    ue->s10_sgw->idle = !mme_s10_handover_is_target(ue);
    sgw_ue_switch_to_sgw(sgw, ue->s10_selected_sgw);
    sgw->sgw_s11_teid = 0;
    return create(ue, mme_sess_first(ue));
}

static int endpoint(const ogs_tlv_octet_t *ie, uint8_t type,
        uint32_t *teid, ogs_ip_t *ip)
{
    ogs_gtp2_f_teid_t *f = ie->data;
    size_t len;
    if (!ie->presence || !f || ie->len < 5) return OGS_ERROR;
    len = 5 + (f->ipv4 ? 4 : 0) + (f->ipv6 ? 16 : 0);
    if (ie->len != len || !f->teid || f->interface_type != type ||
            (!f->ipv4 && !f->ipv6)) return OGS_ERROR;
    *teid = be32toh(f->teid);
    return ogs_gtp2_f_teid_to_ip(f, ip);
}

void mme_s10_sgw_create_response(ogs_gtp_xact_t *xact, mme_ue_t *ue_from_teid,
        ogs_gtp2_message_t *message)
{
    mme_ue_t *ue = mme_ue_find_by_id(OGS_POINTER_TO_UINT(xact->data));
    ogs_gtp2_create_session_response_t *rsp = &message->create_session_response;
    relocation_t *r = ue ? ue->s10_sgw : NULL;
    ogs_gtp2_cause_t *cause = rsp->cause.data;
    mme_s10_sgw_ready_t ready;
    mme_sess_t *sess;
    mme_bearer_t *b;
    sgw_ue_t *sgw;
    ogs_ip_t ip;
    uint32_t teid;
    uint16_t seen = 0, expected = 0;
    int i;
    struct { mme_bearer_t *b; ogs_ip_t ip; uint32_t teid; }
        staged[OGS_BEARER_PER_UE] = {{0}};
    bool valid = r && r->xact_id == xact->id &&
        xact->data == OGS_UINT_TO_POINTER(ue->id);
    ogs_gtp_xact_commit(xact);
    if (!valid) return;
    r->xact_id = 0;
    ready = r->ready;
    sess = mme_sess_find_by_id(r->sess_id);
    sgw = sgw_ue_find_by_id(ue->sgw_ue_id);
    if (!sess || !sgw || ue != ue_from_teid || message->h.teid != ue->mme_s11_teid ||
            xact->gnode != sgw->gnode ||
            !rsp->cause.presence || rsp->cause.len < 2 || !cause ||
            cause->value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED ||
            endpoint(&rsp->sender_f_teid_for_control_plane,
                OGS_GTP2_F_TEID_S11_S4_SGW_GTP_C, &teid, &ip) != OGS_OK)
        goto fail;
    if (sgw->sgw_s11_teid && sgw->sgw_s11_teid != teid) goto fail;
    /* Retain the endpoint for cleanup even if the bearer list is invalid. */
    sgw->sgw_s11_teid = teid;
    ogs_list_for_each(&sess->bearer_list, b) expected |= 1 << b->ebi;
    for (i = 0; i < OGS_BEARER_PER_UE; i++) {
        const ogs_gtp2_tlv_bearer_context_t *bc = &rsp->bearer_contexts_created[i];
        ogs_gtp2_cause_t *bcause = bc->cause.data;
        uint8_t ebi;
        if (!bc->presence) break;
        ebi = bc->eps_bearer_id.u8;
        if (!bc->eps_bearer_id.presence || ebi < 5 || ebi > 15 ||
                (seen & (1 << ebi)) || !(expected & (1 << ebi)) ||
                !bc->cause.presence || bc->cause.len < 2 || !bcause ||
                bcause->value != OGS_GTP2_CAUSE_REQUEST_ACCEPTED ||
                !(staged[i].b = mme_bearer_find_by_ue_ebi(ue, ebi)) ||
                endpoint(&bc->s1_u_enodeb_f_teid,
                    OGS_GTP2_F_TEID_S1_U_SGW_GTP_U,
                    &staged[i].teid, &staged[i].ip) != OGS_OK) goto fail;
        seen |= 1 << ebi;
    }
    if (seen != expected) goto fail;
    for (i = 0; i < OGS_BEARER_PER_UE && staged[i].b; i++) {
        staged[i].b->sgw_s1u_teid = staged[i].teid;
        staged[i].b->sgw_s1u_ip = staged[i].ip;
    }
    if (r->idle) r->switched_ebis |= 1 << mme_default_bearer_in_sess(sess)->ebi;
    sess = mme_sess_next(sess);
    if (sess) {
        if (create(ue, sess) != OGS_OK) goto fail;
        return;
    }
    r->ready = NULL;
    ready(ue, OGS_OK);
    return;
fail:
    r->ready = NULL;
    if (ready) ready(ue, OGS_ERROR);
}
