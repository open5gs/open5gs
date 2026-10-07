/*
 * Copyright (C) 2026 by Open5GS contributors
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

/* Exercise the production EMM FSM without Diameter, GTP or S1 peers.
 * Only context lookup and outbound messages are substituted. Attach handling,
 * RES validation, security encoding, mementos and bearer state checks are real.
 * Include the FSM with private names so the normal libmme remains untouched. */
#include "mme/mme-event.h"
#include "mme/mme-timer.h"
#include "mme/s1ap-handler.h"
#include "mme/mme-gn-handler.h"
#include "mme/mme-fd-path.h"
#include "mme/mme-s13-handler.h"
#include "mme/emm-handler.h"
#include "mme/emm-build.h"
#include "mme/esm-handler.h"
#include "mme/nas-path.h"
#include "mme/nas-security.h"
#include "mme/s1ap-path.h"
#include "mme/sgsap-types.h"
#include "mme/sgsap-path.h"
#include "mme/mme-gtp-path.h"
#include "mme/mme-path.h"
#include "mme/mme-sm.h"
#include "core/abts.h"

static struct {
    mme_ue_t ue;
    enb_ue_t enb;
    mme_sess_t sess[2];
    mme_bearer_t bearer[2];
    sgw_ue_t sgw;
    ogs_timer_mgr_t *timers;
    int air, deletes, continuation, eir, smc, rejects, delete_action, releases;
    int xacts;
    uint8_t reject_cause;
    uint32_t m_tmsi;
} *f;

static mme_ue_t *attach_ue(ogs_pool_id_t id)
{ return id == f->ue.id ? &f->ue : NULL; }
static enb_ue_t *attach_enb(ogs_pool_id_t id)
{ return id == f->enb.id ? &f->enb : NULL; }
static void attach_air(enb_ue_t *enb, mme_ue_t *ue,
        ogs_nas_authentication_failure_parameter_t *auts)
{
    /* Model only the local effects of the outbound AIR boundary. */
    CLEAR_SECURITY_CONTEXT(ue);
    ue->xres_len = 0;
    f->air++;
}
static void attach_delete(enb_ue_t *enb, mme_ue_t *ue, int action)
{
    f->deletes++;
    f->delete_action = action;
    /* Sending Delete Session creates transactions, not a bearer transition. */
    f->xacts += mme_sess_count(ue);
}
static void attach_ulr(enb_ue_t *enb, mme_ue_t *ue, uint32_t flags)
{ f->continuation++; }
static void attach_eir(enb_ue_t *enb, mme_ue_t *ue)
{ f->eir++; }
static mme_sess_t *attach_sess(ogs_pool_id_t id)
{
    mme_sess_t *sess;
    ogs_list_for_each(&f->ue.sess_list, sess)
        if (sess->id == id) return sess;
    return NULL;
}
static sgw_ue_t *attach_sgw(ogs_pool_id_t id)
{ return &f->sgw; }
static int attach_commit(ogs_gtp_xact_t *xact)
{
    if (f->xacts)
        f->xacts--;
    return OGS_OK;
}
static int attach_xact_count(mme_ue_t *ue, uint8_t originator)
{ return f->xacts; }
static void attach_remove(mme_sess_t *sess)
{ ogs_list_remove(&f->ue.sess_list, sess); }
static void attach_guti(mme_ue_t *ue)
{ ue->next.m_tmsi = &f->m_tmsi; }
static int attach_command(mme_ue_t *ue)
{
    ogs_pkbuf_t *pkbuf = emm_build_security_mode_command(ue);
    if (!pkbuf) return OGS_ERROR;
    ogs_pkbuf_free(pkbuf);
    ogs_timer_start(ue->t3460.timer, ogs_time_from_sec(1));
    f->smc++;
    return OGS_OK;
}
static int attach_auth_request(mme_ue_t *ue)
{ return OGS_OK; }
static int attach_auth_reject(mme_ue_t *ue)
{ f->rejects++; return OGS_OK; }
static int attach_reject(enb_ue_t *enb, mme_ue_t *ue,
        uint8_t emm_cause, uint8_t esm_cause)
{ f->rejects++; f->reject_cause = emm_cause; return OGS_OK; }
static int attach_service_reject(enb_ue_t *enb, mme_ue_t *ue, uint8_t cause)
{ f->rejects++; return OGS_OK; }
void attach_continue(enb_ue_t *, mme_ue_t *);
void attach_tau_accept(enb_ue_t *, mme_ue_t *);
void attach_delete_or_detach(enb_ue_t *, mme_ue_t *);

#define emm_state_initial attach_state_initial
#define emm_state_final attach_state_final
#define emm_state_de_registered attach_state_de_registered
#define emm_state_registered attach_state_registered
#define emm_state_authentication attach_state_authentication
#define emm_state_security_mode attach_state_security_mode
#define emm_state_initial_context_setup attach_state_initial_context_setup
#define emm_state_ue_context_will_remove attach_state_ue_context_will_remove
#define emm_state_exception attach_state_exception
#define mme_ue_find_by_id attach_ue
#define enb_ue_find_by_id attach_enb
#define mme_s6a_send_air attach_air
#define mme_s6a_send_ulr attach_ulr
#define mme_s13_start_check attach_eir
#define mme_gtp_send_delete_all_sessions attach_delete
#define mme_ue_xact_count attach_xact_count
#define mme_send_delete_session_or_update_location_request attach_continue
#define mme_ue_new_guti attach_guti
#define nas_eps_send_security_mode_command attach_command
#define nas_eps_send_authentication_request attach_auth_request
#define nas_eps_send_authentication_reject attach_auth_reject
#define nas_eps_send_attach_reject attach_reject
#define nas_eps_send_service_reject attach_service_reject
#define mme_send_delete_session_or_detach attach_delete_or_detach
void emm_state_initial(ogs_fsm_t *, mme_event_t *);
void emm_state_final(ogs_fsm_t *, mme_event_t *);
void emm_state_de_registered(ogs_fsm_t *, mme_event_t *);
void emm_state_registered(ogs_fsm_t *, mme_event_t *);
void emm_state_authentication(ogs_fsm_t *, mme_event_t *);
void emm_state_security_mode(ogs_fsm_t *, mme_event_t *);
void emm_state_initial_context_setup(ogs_fsm_t *, mme_event_t *);
void emm_state_ue_context_will_remove(ogs_fsm_t *, mme_event_t *);
void emm_state_exception(ogs_fsm_t *, mme_event_t *);
#include "../../src/mme/emm-sm.c"

/* Run the real EIR/ULR choice and asynchronous Delete Session Response path. */
#define mme_send_delete_session_or_mme_ue_context_release attach_delete_or_release
#define mme_send_release_access_bearer_or_ue_context_release attach_access_release
#define mme_send_after_paging attach_after_paging
#define mme_send_delete_session_or_tau_accept attach_delete_or_tau
#define mme_send_tau_accept_and_check_release attach_tau_accept
#include "../../src/mme/mme-path.c"
#define mme_sess_find_by_id attach_sess
#define sgw_ue_find_by_id attach_sgw
#define ogs_gtp_xact_commit attach_commit
#define mme_sess_remove attach_remove
#define mme_s11_handle_echo_request attach_echo_request
#define mme_s11_handle_echo_response attach_echo_response
#define mme_s11_handle_create_session_response attach_create_session_response
#define mme_s11_handle_modify_bearer_response attach_modify_bearer_response
#define mme_s11_handle_delete_session_response attach_delete_session_response
#define mme_s11_handle_create_bearer_request attach_create_bearer_request
#define mme_s11_handle_update_bearer_request attach_update_bearer_request
#define mme_s11_handle_delete_bearer_request attach_delete_bearer_request
#define mme_s11_handle_release_access_bearers_response attach_release_response
#define mme_s11_handle_downlink_data_notification attach_notification
#define mme_s11_handle_create_indirect_data_forwarding_tunnel_response attach_create_tunnel_response
#define mme_s11_handle_delete_indirect_data_forwarding_tunnel_response attach_delete_tunnel_response
#define mme_s11_handle_bearer_resource_failure_indication attach_bearer_failure
#include "../../src/mme/mme-s11-handler.c"

/* Keep the EIR completion/reject decisions real, including retained-session
 * cleanup. The outbound ECR is still intercepted at the FSM boundary. */
#define mme_s13_check_wanted attach_check_wanted
#define mme_s13_imeisv_is_usable attach_imeisv_is_usable
#undef mme_s13_start_check
#define mme_s13_start_check attach_real_start_check
#define mme_s13_eca_is_current attach_eca_is_current
#define mme_s13_missing_pei_cause attach_missing_pei_cause
#define mme_s13_failure_cause attach_eir_failure_cause
#define mme_s13_equipment_status_cause attach_equipment_status_cause
#define mme_s13_message_cause attach_eir_message_cause
#define mme_s13_handle_eca attach_handle_eca
#define mme_s13_complete_check attach_complete_check
#define mme_s13_reject_ue attach_reject_ue
#undef MME_S13_HANDLER_H
#include "mme/mme-s13-handler.h"
#include "../../src/mme/mme-s13-handler.c"

/* Invoke the real GTP timeout callback without waiting for a peer. Keep its
 * public send functions private; only the S1 release send boundary is replaced. */
static int attach_release(enb_ue_t *enb, S1AP_Cause_PR group, long cause,
        uint8_t action, ogs_time_t duration)
{
    f->releases++;
    enb->ue_ctx_rel_action = action;
    return OGS_OK;
}
#undef mme_gtp_send_delete_all_sessions
#define mme_gtp_open attach_gtp_open
#define mme_gtp_close attach_gtp_close
#define mme_gtp_send_create_session_request attach_gtp_create_session
#define mme_gtp_send_create_session_request_now attach_gtp_create_session_now
#define mme_gtp_send_modify_bearer_request attach_gtp_modify_bearer
#define mme_gtp_send_delete_session_request attach_gtp_delete_session
#define mme_gtp_send_delete_all_sessions attach_gtp_delete_all
#define mme_gtp_send_create_bearer_response attach_gtp_create_bearer
#define mme_gtp_send_update_bearer_response attach_gtp_update_bearer
#define mme_gtp_send_delete_bearer_response attach_gtp_delete_bearer
#define mme_gtp_send_release_access_bearers_request attach_gtp_release_access
#define mme_gtp_send_release_all_ue_in_enb attach_gtp_release_all
#define mme_gtp_send_downlink_data_notification_ack attach_gtp_notification_ack
#define mme_gtp_send_create_indirect_data_forwarding_tunnel_request attach_gtp_create_tunnel
#define mme_gtp_send_delete_indirect_data_forwarding_tunnel_request attach_gtp_delete_tunnel
#define mme_gtp_send_bearer_resource_command attach_gtp_bearer_resource
#define mme_gtp1_send_sgsn_context_request attach_gtp1_context_request
#define mme_gtp1_send_sgsn_context_response attach_gtp1_context_response
#define mme_gtp1_send_sgsn_context_ack attach_gtp1_context_ack
#define mme_gtp1_send_ran_information_relay attach_gtp1_information_relay
#undef MME_S11_PATH_H
#include "mme/mme-gtp-path.h"
#define s1ap_send_ue_context_release_command attach_release
#define timeout attach_gtp_timeout
#include "../../src/mme/mme-gtp-path.c"
#undef timeout
#undef s1ap_send_ue_context_release_command
#undef mme_gtp_open
#undef mme_gtp_close
#undef mme_gtp_send_create_session_request
#undef mme_gtp_send_create_session_request_now
#undef mme_gtp_send_modify_bearer_request
#undef mme_gtp_send_delete_session_request
#undef mme_gtp_send_delete_all_sessions
#define mme_gtp_send_delete_all_sessions attach_delete
#undef mme_gtp_send_create_bearer_response
#undef mme_gtp_send_update_bearer_response
#undef mme_gtp_send_delete_bearer_response
#undef mme_gtp_send_release_access_bearers_request
#undef mme_gtp_send_release_all_ue_in_enb
#undef mme_gtp_send_downlink_data_notification_ack
#undef mme_gtp_send_create_indirect_data_forwarding_tunnel_request
#undef mme_gtp_send_delete_indirect_data_forwarding_tunnel_request
#undef mme_gtp_send_bearer_resource_command
#undef mme_gtp1_send_sgsn_context_request
#undef mme_gtp1_send_sgsn_context_response
#undef mme_gtp1_send_sgsn_context_ack
#undef mme_gtp1_send_ran_information_relay

static void timer_unused(void *data) { }

static void fixture_init(bool sessions)
{
    int i;
    f = ogs_calloc(1, sizeof(*f));
    ogs_assert(f);
    f->ue.id = 1;
    f->enb.id = f->ue.enb_ue_id = 2;
    f->enb.mme_ue_id = f->ue.id;
    f->enb.ue_ctx_rel_action = S1AP_UE_CTX_REL_INVALID_ACTION;
    f->ue.imsi_len = 8;
    ogs_cpystrn(f->ue.imsi_bcd, "001010123456789", sizeof(f->ue.imsi_bcd));
    f->ue.nas_eps.ue.ksi = f->ue.nas_eps.mme.ksi = 2;
    f->ue.nas_eps.ue.tsc = f->ue.nas_eps.mme.tsc = 1;
    f->ue.security_context_available = 1;
    f->ue.selected_int_algorithm = OGS_NAS_SECURITY_ALGORITHMS_128_EIA2;
    f->ue.selected_enc_algorithm = OGS_NAS_SECURITY_ALGORITHMS_128_EEA2;
    f->ue.ue_network_capability.eia = f->ue.ue_network_capability.eea = 0x20;
    f->ue.ul_count.i32 = 41;
    f->ue.ul_count_accepted = true;
    f->ue.dl_count = 37;
    f->ue.xres_len = 8;
    memset(f->ue.xres, 0x33, f->ue.xres_len);
    memset(f->ue.kasme, 0x11, sizeof(f->ue.kasme));
    memset(f->ue.knas_int, 0x22, sizeof(f->ue.knas_int));
    f->timers = ogs_timer_mgr_create(9);
#define TIMER(name) f->ue.name.timer = ogs_timer_add(f->timers, timer_unused, NULL)
    TIMER(t3413); TIMER(t3422); TIMER(t3450); TIMER(t3460);
    TIMER(t3470); TIMER(t_mobile_reachable); TIMER(t_implicit_detach);
#undef TIMER
    for (i = 0; i < 2; i++) {
        f->sess[i].id = 3 + i;
        f->sess[i].mme_ue_id = f->ue.id;
        f->bearer[i].t3489.timer =
            ogs_timer_add(f->timers, timer_unused, NULL);
        OGS_FSM_TRAN(&f->bearer[i].sm, esm_state_active);
        if (sessions) {
            ogs_list_add(&f->ue.sess_list, &f->sess[i]);
            ogs_list_add(&f->sess[i].bearer_list, &f->bearer[i]);
        }
    }
    f->ue.ebi_bitmap = sessions ? 3 : 0;
    OGS_FSM_TRAN(&f->ue.sm, emm_state_registered);
}

static void fixture_final(void)
{
    int i;
    OGS_NAS_CLEAR_DATA(&f->ue.pdn_connectivity_request);
#define TIMER(name) ogs_timer_delete(f->ue.name.timer)
    TIMER(t3413); TIMER(t3422); TIMER(t3450); TIMER(t3460);
    TIMER(t3470); TIMER(t_mobile_reachable); TIMER(t_implicit_detach);
#undef TIMER
    for (i = 0; i < 2; i++)
        ogs_timer_delete(f->bearer[i].t3489.timer);
    ogs_timer_mgr_destroy(f->timers);
    ogs_free(f);
    f = NULL;
}

static void dispatch(uint8_t type, uint8_t security_type, bool invalid_mac)
{
    mme_event_t event = {0};
    ogs_nas_eps_message_t message = {0};
    ogs_nas_security_header_type_t header = {0};
    uint8_t esm[] = {0x02, 0x01, OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST};
    ogs_pkbuf_t *pkbuf = ogs_pkbuf_alloc(NULL, sizeof(esm));
    ogs_pkbuf_put_data(pkbuf, esm, sizeof(esm));
    event.id = MME_EVENT_EMM_MESSAGE;
    event.mme_ue_id = f->ue.id;
    event.enb_ue_id = f->enb.id;
    event.nas_message = &message;
    event.pkbuf = pkbuf;
    header.integrity_protected = security_type;
    event.nas_type = header.type;
    message.emm.h.message_type = type;
    if (type == OGS_NAS_EPS_ATTACH_REQUEST) {
        message.emm.attach_request.eps_attach_type.value =
            OGS_NAS_ATTACH_TYPE_EPS_ATTACH;
        message.emm.attach_request.eps_attach_type.nas_key_set_identifier =
            security_type ? 2 : 7;
        message.emm.attach_request.eps_mobile_identity.guti.type =
            OGS_NAS_EPS_MOBILE_IDENTITY_GUTI;
        message.emm.attach_request.ue_network_capability.eia = 0x20;
        message.emm.attach_request.ue_network_capability.eea = 0x20;
        message.emm.attach_request.esm_message_container.length = sizeof(esm);
        message.emm.attach_request.esm_message_container.buffer = esm;
    } else if (type == OGS_NAS_EPS_AUTHENTICATION_RESPONSE) {
        message.emm.authentication_response.authentication_response_parameter
            .length = 8;
        memset(message.emm.authentication_response.authentication_response_parameter
            .res, invalid_mac ? 0xff : 0x33, 8);
    } else if (type == OGS_NAS_EPS_AUTHENTICATION_FAILURE) {
        message.emm.authentication_failure.emm_cause = OGS_NAS_EMM_CAUSE_MAC_FAILURE;
    } else if (type == OGS_NAS_EPS_DETACH_REQUEST) {
        message.emm.detach_request_from_ue.detach_type.value =
            OGS_NAS_DETACH_TYPE_FROM_UE_EPS_DETACH;
        message.emm.detach_request_from_ue.detach_type.nas_key_set_identifier = 2;
    }
    f->ue.mac_failed = invalid_mac;
    ogs_fsm_dispatch(&f->ue.sm, &event);
    ogs_pkbuf_free(pkbuf);
}

static void authenticate(void)
{
    /* A successful AIA installs a fresh vector and assigns its KSI. */
    memset(f->ue.kasme, 0x44, sizeof(f->ue.kasme));
    memset(f->ue.xres, 0x33, 8);
    f->ue.xres_len = 8;
    f->ue.nas_eps.ue.ksi = f->ue.nas_eps.mme.ksi = 3;
    dispatch(OGS_NAS_EPS_AUTHENTICATION_RESPONSE, 0, false);
}

static void assert_sessions(abts_case *tc)
{
    int i;
    ABTS_INT_EQUAL(tc, 0, f->deletes);
    ABTS_INT_EQUAL(tc, 2, ogs_list_count(&f->ue.sess_list));
    ABTS_INT_EQUAL(tc, 3, f->ue.ebi_bitmap);
    for (i = 0; i < 2; i++)
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->bearer[i].sm, esm_state_active));
    ABTS_INT_EQUAL(tc, 0, f->continuation);
}

static void attach_starts_authentication(abts_case *tc, void *data)
{
    int invalid_mac;
    for (invalid_mac = 0; invalid_mac < 2; invalid_mac++) {
        fixture_init(true);
        dispatch(OGS_NAS_EPS_ATTACH_REQUEST, invalid_mac ? 1 : 0, invalid_mac);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, emm_state_authentication));
        ABTS_INT_EQUAL(tc, 1, f->air);
        assert_sessions(tc);
        fixture_final();
    }
}

static void repeated_attach(abts_case *tc, void *data)
{
    int security_mode;
    for (security_mode = 0; security_mode < 2; security_mode++) {
        fixture_init(true);
        dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
        if (security_mode) authenticate();
        dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, emm_state_authentication));
        ABTS_INT_EQUAL(tc, 2, f->air);
        assert_sessions(tc);
        fixture_final();
    }
}

/* Bad RES, Authentication Failure, AKA timeout, SMC Reject, SMC timeout,
 * first-attach failure, and an old RES received before the fresh AIA. */
static void attach_failure(abts_case *tc, void *data)
{
    int failure = OGS_POINTER_TO_UINT(data);
    fixture_init(failure != 5);
    if (failure == 5)
        OGS_FSM_TRAN(&f->ue.sm, emm_state_de_registered);
    dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
    if (failure == 3 || failure == 4) authenticate();
    if (failure == 0 || failure == 5 || failure == 6) {
        if (failure != 6) {
            memset(f->ue.xres, 0x33, 8);
            f->ue.xres_len = 8;
        }
        dispatch(OGS_NAS_EPS_AUTHENTICATION_RESPONSE, 0, failure != 6);
    } else if (failure == 1) {
        dispatch(OGS_NAS_EPS_AUTHENTICATION_FAILURE, 0, false);
    } else if (failure == 3) {
        dispatch(OGS_NAS_EPS_SECURITY_MODE_REJECT, 1, false);
    } else {
        mme_event_t event = {0};
        event.id = MME_EVENT_EMM_TIMER;
        event.mme_ue_id = f->ue.id;
        event.timer_id = MME_TIMER_T3460;
        f->ue.t3460.retry_count = mme_timer_cfg(MME_TIMER_T3460)->max_count;
        ogs_fsm_dispatch(&f->ue.sm, &event);
    }
    ABTS_INT_EQUAL(tc, failure == 3 ? 0 : 1, f->rejects);
    if (failure == 5) {
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, emm_state_exception));
        ABTS_INT_EQUAL(tc, 0, ogs_list_count(&f->ue.sess_list));
        fixture_final();
        return;
    }
    ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, emm_state_registered));
    assert_sessions(tc);
    ABTS_TRUE(tc, SECURITY_CONTEXT_IS_VALID(&f->ue));
    ABTS_INT_EQUAL(tc, 2, f->ue.nas_eps.ue.ksi);
    ABTS_INT_EQUAL(tc, 2, f->ue.nas_eps.mme.ksi);
    ABTS_INT_EQUAL(tc, 1, f->ue.nas_eps.ue.tsc);
    ABTS_INT_EQUAL(tc, 1, f->ue.nas_eps.mme.tsc);
    ABTS_INT_EQUAL(tc, 41, f->ue.ul_count.i32);
    ABTS_INT_EQUAL(tc, 37, f->ue.dl_count);
    ABTS_INT_EQUAL(tc, 0x11, f->ue.kasme[0]);
    ABTS_INT_EQUAL(tc, 0x22, f->ue.knas_int[0]);
    fixture_final();
}

static void invalid_security_complete(abts_case *tc, void *data)
{
    int invalid_mac;
    for (invalid_mac = 0; invalid_mac < 2; invalid_mac++) {
        fixture_init(true);
        dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
        authenticate();
        dispatch(OGS_NAS_EPS_SECURITY_MODE_COMPLETE, invalid_mac ? 1 : 0,
                invalid_mac);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, emm_state_security_mode));
        ABTS_TRUE(tc, f->ue.t3460.timer->running);
        assert_sessions(tc);
        fixture_final();
    }
}

static void authenticated_session_delete(abts_case *tc, void *data)
{
    int sessions;
    for (sessions = 0; sessions < 2; sessions++) {
        fixture_init(sessions);
        dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
        authenticate();
        ABTS_INT_EQUAL(tc, 0, f->deletes);
        ABTS_INT_EQUAL(tc, 0, f->continuation);
        dispatch(OGS_NAS_EPS_SECURITY_MODE_COMPLETE, 1, false);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, emm_state_initial_context_setup));
        ABTS_INT_EQUAL(tc, sessions, f->deletes);
        if (sessions)
            ABTS_INT_EQUAL(tc, OGS_GTP_DELETE_SEND_UPDATE_LOCATION_REQUEST,
                    f->delete_action);
        ABTS_INT_EQUAL(tc, !sessions, f->continuation);
        if (sessions) {
            /* Pending deletion must not allow another Attach to restart AKA. */
            dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
            ABTS_INT_EQUAL(tc, 1, f->air);
            ABTS_INT_EQUAL(tc, 1, f->deletes);
            ABTS_INT_EQUAL(tc, 0, f->continuation);
            ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, emm_state_initial_context_setup));
        }
        fixture_final();
    }
}

static void verified_attach(abts_case *tc, void *data)
{
    fixture_init(true);
    dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 1, false);
    ABTS_INT_EQUAL(tc, 0, f->air);
    ABTS_INT_EQUAL(tc, 1, f->deletes);
    ABTS_INT_EQUAL(tc, OGS_GTP_DELETE_HANDLE_PDN_CONNECTIVITY_REQUEST,
            f->delete_action);
    ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, emm_state_initial_context_setup));
    fixture_final();
}

static void unverified_detach_during_attach(abts_case *tc, void *data)
{
    int security_mode, invalid_mac;
    for (security_mode = 0; security_mode < 2; security_mode++) {
        for (invalid_mac = 0; invalid_mac < 2; invalid_mac++) {
            ogs_fsm_handler_t state;
            fixture_init(true);
            dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
            if (security_mode) authenticate();
            state = OGS_FSM_STATE(&f->ue.sm);
            dispatch(OGS_NAS_EPS_DETACH_REQUEST, invalid_mac, invalid_mac);
            ABTS_TRUE(tc, state == OGS_FSM_STATE(&f->ue.sm));
            ABTS_INT_EQUAL(tc, 0, f->rejects);
            ABTS_INT_EQUAL(tc, MME_EPS_TYPE_ATTACH_REQUEST, f->ue.nas_eps.type);
            assert_sessions(tc);
            fixture_final();
        }
    }
}

static void delete_response(int session)
{
    ogs_gtp_xact_t xact = {0};
    ogs_gtp2_delete_session_response_t response = {0};
    xact.data = OGS_UINT_TO_POINTER(f->sess[session].id);
    xact.enb_ue_id = f->enb.id;
    xact.delete_action = OGS_GTP_DELETE_SEND_UPDATE_LOCATION_REQUEST;
    mme_s11_handle_delete_session_response(&xact, &f->ue, &response);
}

static void last_delete_continues_attach(abts_case *tc, void *data)
{
    int eir;
    for (eir = 0; eir < 2; eir++) {
        mme_self()->eir.enabled = eir;
        fixture_init(true);
        dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
        authenticate();
        dispatch(OGS_NAS_EPS_SECURITY_MODE_COMPLETE, 1, false);
        ABTS_INT_EQUAL(tc, eir, f->eir);
        ABTS_INT_EQUAL(tc, 0, f->continuation);
        if (eir) {
            assert_sessions(tc);
            mme_s13_complete_check(&f->enb, &f->ue,
                    OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED);
        }
        delete_response(1); /* Responses may arrive in either order. */
        ABTS_INT_EQUAL(tc, 1, ogs_list_count(&f->ue.sess_list));
        ABTS_INT_EQUAL(tc, 0, f->continuation);
        ABTS_TRUE(tc, f->ue.attach_session_delete_pending);
        delete_response(0);
        ABTS_INT_EQUAL(tc, 0, ogs_list_count(&f->ue.sess_list));
        ABTS_INT_EQUAL(tc, 1, f->continuation);
        ABTS_INT_EQUAL(tc, eir, f->eir);
        ABTS_TRUE(tc, !f->ue.attach_session_delete_pending);
        delete_response(0); /* A duplicate response cannot resume twice. */
        ABTS_INT_EQUAL(tc, 1, f->continuation);
        fixture_final();
    }
    mme_self()->eir.enabled = false;
    fixture_init(true);
    dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
    authenticate();
    dispatch(OGS_NAS_EPS_SECURITY_MODE_COMPLETE, 1, false);
    f->ue.attach_session_delete_pending = false; /* Procedure was cancelled. */
    delete_response(0);
    delete_response(1);
    ABTS_INT_EQUAL(tc, 0, f->continuation);
    fixture_final();
    fixture_init(true);
    dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
    authenticate();
    dispatch(OGS_NAS_EPS_SECURITY_MODE_COMPLETE, 1, false);
    f->enb.ue_ctx_rel_action = S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE;
    delete_response(0);
    delete_response(1);
    ABTS_INT_EQUAL(tc, 0, f->continuation);
    fixture_final();
    mme_self()->eir.enabled = true;
    fixture_init(true);
    dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
    authenticate();
    dispatch(OGS_NAS_EPS_SECURITY_MODE_COMPLETE, 1, false);
    assert_sessions(tc);
    mme_s13_complete_check(&f->enb, &f->ue, OGS_NAS_EMM_CAUSE_ILLEGAL_ME);
    ABTS_INT_EQUAL(tc, 1, f->rejects);
    ABTS_INT_EQUAL(tc, 1, f->deletes);
    ABTS_INT_EQUAL(tc, OGS_GTP_DELETE_SEND_RELEASE_WITH_UE_CONTEXT_REMOVE,
            f->delete_action);
    ABTS_INT_EQUAL(tc, 0, f->continuation);
    fixture_final();
    mme_self()->eir.enabled = false;
}

/* Exercise policy decisions after the real authenticated Attach/SMC path.
 * The ECA and missing-PEI handlers are real; Diameter delivery is not simulated.
 * In particular, UNABLE_TO_COMPLY is the result synthesized by the ECR timeout.
 */
static void eir_policy_completes_attach(abts_case *tc, void *data)
{
    enum { UNKNOWN, FAILURE, MALFORMED, MISSING_PEI, BLACKLIST } scenario;
    mme_eir_t saved = mme_self()->eir;
    int reject;

    for (scenario = UNKNOWN; scenario <= BLACKLIST; scenario++) {
        for (reject = 0; reject < 2; reject++) {
            ogs_diam_s13_message_t answer = {0};
            ogs_eir_action_e action = reject ?
                OGS_EIR_ACTION_REJECT : OGS_EIR_ACTION_ALLOW;
            bool rejected = reject || scenario == BLACKLIST;
            ogs_nas_emm_cause_t expected = !rejected ?
                OGS_NAS_EMM_CAUSE_REQUEST_ACCEPTED :
                scenario == BLACKLIST ? OGS_NAS_EMM_CAUSE_ILLEGAL_ME :
                scenario == UNKNOWN || scenario == MISSING_PEI ?
                OGS_NAS_EMM_CAUSE_EPS_SERVICES_NOT_ALLOWED :
                OGS_NAS_EMM_CAUSE_NETWORK_FAILURE;

            /* Opposing settings catch use of the wrong policy field. */
            mme_self()->eir.enabled = true;
            mme_self()->eir.unknown_action =
                mme_self()->eir.failure_action =
                mme_self()->eir.missing_pei_action = reject ?
                    OGS_EIR_ACTION_ALLOW : OGS_EIR_ACTION_REJECT;
            if (scenario == UNKNOWN)
                mme_self()->eir.unknown_action = action;
            else if (scenario == MISSING_PEI)
                mme_self()->eir.missing_pei_action = action;
            else
                mme_self()->eir.failure_action = action;

            fixture_init(true);
            dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
            authenticate();
            dispatch(OGS_NAS_EPS_SECURITY_MODE_COMPLETE, 1, false);
            ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm,
                        emm_state_initial_context_setup));
            ABTS_INT_EQUAL(tc, 1, f->eir);
            assert_sessions(tc);

            if (scenario == MISSING_PEI) {
                ABTS_TRUE(tc, !f->ue.imeisv_bcd[0]);
                mme_s13_start_check(&f->enb, &f->ue);
            } else {
                ogs_nas_emm_cause_t cause;

                answer.cmd_code = OGS_DIAM_S13_CMD_CODE_ME_IDENTITY_CHECK;
                answer.result_code = ER_DIAMETER_SUCCESS;
                if (scenario == UNKNOWN) {
                    answer.result_code = OGS_DIAM_S13_ERROR_EQUIPMENT_UNKNOWN;
                    answer.exp_err = &answer.result_code;
                } else if (scenario == FAILURE) {
                    answer.result_code = ER_DIAMETER_UNABLE_TO_COMPLY;
                    answer.err = &answer.result_code;
                } else {
                    answer.eca_message.equipment_status_code =
                        scenario == BLACKLIST ?
                        OGS_DIAM_S13_EQUIPMENT_BLACKLIST : 3;
                }
                cause = mme_s13_handle_eca(&f->ue, &answer);
                ABTS_INT_EQUAL(tc, expected, cause);
                mme_s13_complete_check(&f->enb, &f->ue, cause);
            }

            ABTS_INT_EQUAL(tc, rejected, f->rejects);
            ABTS_INT_EQUAL(tc, rejected ? expected : 0, f->reject_cause);
            ABTS_INT_EQUAL(tc, 1, f->deletes);
            ABTS_INT_EQUAL(tc, rejected ?
                    OGS_GTP_DELETE_SEND_RELEASE_WITH_UE_CONTEXT_REMOVE :
                    OGS_GTP_DELETE_SEND_UPDATE_LOCATION_REQUEST,
                    f->delete_action);
            ABTS_INT_EQUAL(tc, 0, f->continuation);
            ABTS_INT_EQUAL(tc, !rejected, f->ue.attach_session_delete_pending);
            if (!rejected) {
                delete_response(1);
                ABTS_INT_EQUAL(tc, 0, f->continuation);
                delete_response(0);
                ABTS_INT_EQUAL(tc, 1, f->continuation);
                ABTS_INT_EQUAL(tc, 0, mme_sess_count(&f->ue));
                ABTS_TRUE(tc, !f->ue.attach_session_delete_pending);
            }
            fixture_final();
        }
    }
    mme_self()->eir = saved;
}

/* Exercise timeout cancellation and the release send boundary. Release
 * Complete/S1 holding expiry and their UE removal/FSM fini are not simulated. */
static void delete_timeout_cancels_attach(abts_case *tc, void *data)
{
    int pdns, connected, update_location, i;

    for (pdns = 1; pdns <= 2; pdns++) {
        for (connected = 0; connected <= 1; connected++) {
            for (update_location = 0; update_location <= 1; update_location++) {
                ogs_gtp_xact_t xact = {0};

                fixture_init(true);
                if (pdns == 1)
                    ogs_list_remove(&f->ue.sess_list, &f->sess[1]);
                dispatch(OGS_NAS_EPS_ATTACH_REQUEST, 0, false);
                authenticate();
                dispatch(OGS_NAS_EPS_SECURITY_MODE_COMPLETE, 1, false);
                ABTS_TRUE(tc, f->ue.attach_session_delete_pending);
                if (!connected)
                    f->ue.enb_ue_id = OGS_INVALID_POOL_ID;
                f->sgw.sgw_s11_teid = 0x1234;
                xact.seq[0].type = OGS_GTP2_DELETE_SESSION_REQUEST_TYPE;
                xact.delete_action = update_location ?
                    OGS_GTP_DELETE_SEND_UPDATE_LOCATION_REQUEST :
                    OGS_GTP_DELETE_SEND_DETACH_ACCEPT;
                attach_gtp_timeout(&xact, OGS_UINT_TO_POINTER(f->sess[0].id));

                ABTS_INT_EQUAL(tc, 0, f->sgw.sgw_s11_teid);
                ABTS_INT_EQUAL(tc, connected, f->releases);
                if (connected)
                    ABTS_INT_EQUAL(tc, S1AP_UE_CTX_REL_UE_CONTEXT_REMOVE,
                            f->enb.ue_ctx_rel_action);
                ABTS_INT_EQUAL(tc, pdns, mme_sess_count(&f->ue));
                ABTS_INT_EQUAL(tc, 0, f->continuation);
                ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm,
                            emm_state_initial_context_setup));
                for (i = 0; i < pdns; i++)
                    ABTS_TRUE(tc, OGS_FSM_CHECK(&f->bearer[i].sm,
                                esm_state_active));
                if (update_location) {
                    ABTS_TRUE(tc, !f->ue.attach_session_delete_pending);
                    for (i = 0; i < pdns; i++)
                        delete_response(i);
                    ABTS_INT_EQUAL(tc, 0, mme_sess_count(&f->ue));
                    ABTS_INT_EQUAL(tc, 0, f->continuation);
                } else {
                    /* Other actions do not cancel the Attach continuation. */
                    ABTS_TRUE(tc, f->ue.attach_session_delete_pending);
                }
                fixture_final();
            }
        }
    }
    for (update_location = 0; update_location <= 1; update_location++) {
        ogs_gtp_xact_t xact = {0};

        fixture_init(true);
        /* An old timeout cancels continuation without changing the state of
         * an already cancelled Attach or a different procedure. */
        f->ue.attach_session_delete_pending = update_location;
        OGS_FSM_TRAN(&f->ue.sm, update_location ?
                emm_state_authentication : emm_state_initial_context_setup);
        xact.seq[0].type = OGS_GTP2_DELETE_SESSION_REQUEST_TYPE;
        xact.delete_action = OGS_GTP_DELETE_SEND_UPDATE_LOCATION_REQUEST;
        attach_gtp_timeout(&xact, OGS_UINT_TO_POINTER(f->sess[0].id));
        ABTS_TRUE(tc, !f->ue.attach_session_delete_pending);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, (update_location ?
                    emm_state_authentication : emm_state_initial_context_setup)));
        ABTS_INT_EQUAL(tc, 1, f->releases);
        ABTS_INT_EQUAL(tc, 0, f->continuation);
        fixture_final();
    }
}

abts_suite *test_mme_attach(abts_suite *suite)
{
    int i;
    mme_context_t *saved = ogs_memdup(mme_self(), sizeof(*saved));
    ogs_log_level_e saved_log;
    suite = ADD_SUITE(suite)
    if (!ogs_log_find_domain("emm"))
        ogs_log_install_domain(&__emm_log_domain, "emm", OGS_LOG_ERROR);
    saved_log = ogs_log_get_domain_level(__emm_log_domain);
    ogs_log_set_domain_level(__emm_log_domain, OGS_LOG_NONE);
    mme_self()->num_of_integrity_order = 1;
    mme_self()->integrity_order[0] = OGS_NAS_SECURITY_ALGORITHMS_128_EIA2;
    mme_self()->num_of_ciphering_order = 1;
    mme_self()->ciphering_order[0] = OGS_NAS_SECURITY_ALGORITHMS_128_EEA2;
    mme_self()->num_of_served_tai = 1;
    memset(mme_self()->served_tai, 0, sizeof(mme_self()->served_tai));
    mme_self()->served_tai[0].list2.type = OGS_TAI2_TYPE;
    mme_self()->served_tai[0].list2.num = 1;
    mme_self()->eir.enabled = false;
    abts_run_test(suite, attach_starts_authentication, NULL);
    abts_run_test(suite, repeated_attach, NULL);
    for (i = 0; i < 7; i++)
        abts_run_test(suite, attach_failure, OGS_UINT_TO_POINTER(i));
    abts_run_test(suite, invalid_security_complete, NULL);
    abts_run_test(suite, authenticated_session_delete, NULL);
    abts_run_test(suite, verified_attach, NULL);
    abts_run_test(suite, unverified_detach_during_attach, NULL);
    abts_run_test(suite, last_delete_continues_attach, NULL);
    abts_run_test(suite, eir_policy_completes_attach, NULL);
    abts_run_test(suite, delete_timeout_cancels_attach, NULL);
    *mme_self() = *saved;
    ogs_free(saved);
    ogs_log_set_domain_level(__emm_log_domain, saved_log);
    return suite;
}
