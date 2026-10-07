/*
 * Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

/* Exercise the production GMM FSM, NAS handlers and SM-context completion
 * without NF startup. Pool lookup, fixed serving-area/RAT/slice policy and
 * outbound messages are substituted. NG release completion is not simulated.
 * SBI responses model removal of their completed transaction,
 * as amf-sm.c does before calling the Nsmf completion handler. */
#include "amf/event.h"
#include "amf/timer.h"
#include "amf/gmm-handler.h"
#include "amf/nas-path.h"
#include "amf/ngap-path.h"
#include "amf/nausf-handler.h"
#include "amf/nsmf-handler.h"
#include "amf/nudm-handler.h"
#include "amf/npcf-handler.h"
#include "amf/n5geir-handler.h"
#include "amf/nnrf-handler.h"
#include "amf/sbi-path.h"
#include "amf/amf-sm.h"
#include "amf/metrics.h"
#include "core/abts.h"

static struct {
    amf_ue_t ue;
    ran_ue_t ran;
    amf_sess_t sess[2];
    ogs_sbi_xact_t xact[2];
    ogs_sbi_xact_t unrelated_xact;
    amf_sbi_xact_ctx_t xact_context[2];
    ogs_timer_mgr_t *timers;
    amf_m_tmsi_t m_tmsi;
    int auth, releases, continuation, eir, rejects, smc, ran_releases;
    int eir_result;
    ogs_nas_5gmm_cause_t reject_cause;
    int num_of_integrity_order;
    uint8_t integrity_order;
    bool eir_enabled;
    ogs_eir_action_e unknown_action, failure_action, missing_pei_action;
    bool ran_removed;
} *f;

static amf_ue_t *registration_ue(ogs_pool_id_t id)
{ return id == f->ue.id ? &f->ue : NULL; }
static ran_ue_t *registration_ran(ogs_pool_id_t id)
{ return !f->ran_removed && id == f->ran.id ? &f->ran : NULL; }
static amf_sess_t *registration_sess(ogs_pool_id_t id)
{
    amf_sess_t *sess;
    ogs_list_for_each(&f->ue.sess_list, sess)
        if (sess->id == id) return sess;
    return NULL;
}
static void registration_remove(amf_sess_t *sess)
{ ogs_list_remove(&f->ue.sess_list, sess); }
static void registration_guti(amf_ue_t *ue)
{ ue->next.m_tmsi = &f->m_tmsi; }
static int registration_tai(ogs_5gs_tai_t *tai)
{ return 0; }
static bool registration_rat(amf_ue_t *ue)
{ return false; }
static bool registration_nssai(amf_ue_t *ue)
{ return true; }
static int registration_send(OpenAPI_service_name_e service,
        ogs_sbi_discovery_option_t *option,
        ogs_sbi_request_t *(*build)(amf_ue_t *, void *),
        amf_ue_t *ue, int state, void *data)
{
    if (service == OpenAPI_service_name_nausf_auth) {
        f->auth++;
        /* Model the local effects of a subsequent AUSF vector response. */
        CLEAR_SECURITY_CONTEXT(ue);
        ue->nas.ue.ksi = ue->nas.amf.ksi = 3;
        ue->nas.ue.tsc = ue->nas.amf.tsc = 0;
    } else if (service == OpenAPI_service_name_nudm_uecm)
        f->continuation++;
    return OGS_OK;
}
static int registration_eir(amf_ue_t *ue)
{ f->eir++; return f->eir_result; }
static void registration_release(ran_ue_t *ran, amf_ue_t *ue,
        int state, void *data)
{
    int i;
    amf_sess_t *sess;
    f->releases++;
    ogs_list_for_each(&ue->sess_list, sess) {
        i = sess->id - 3;
        f->xact[i].state = state;
        f->xact[i].service_name = OpenAPI_service_name_nsmf_pdusession;
        f->xact[i].requester_nf_type = OpenAPI_nf_type_AMF;
        f->xact[i].sbi_object = &sess->sbi;
        f->xact[i].sbi_object_id = sess->id;
        f->xact_context[i].ran_ue_id = ran ? ran->id : OGS_INVALID_POOL_ID;
        f->xact[i].user_data = &f->xact_context[i];
        ogs_list_add(&sess->sbi.xact_list, &f->xact[i]);
    }
}
static int registration_reject(amf_ue_t *ue)
{ f->rejects++; return OGS_OK; }
static int registration_gmm_reject(ran_ue_t *ran, amf_ue_t *ue,
        ogs_nas_5gmm_cause_t cause)
{ f->rejects++; f->reject_cause = cause; return OGS_OK; }
static int registration_smc(amf_ue_t *ue)
{
    f->smc++;
    ue->security_context_available = 1;
    return OGS_OK;
}
static void registration_xact_remove(ogs_sbi_xact_t *xact)
{ ogs_list_remove(&xact->sbi_object->xact_list, xact); }
static int registration_ran_release(ran_ue_t *ran,
        NGAP_Cause_PR group, long cause, uint8_t action, ogs_time_t duration)
{
    f->ran_releases++;
    ran->ue_ctx_rel_action = action;
    return OGS_OK;
}
static int registration_sbi_reject(amf_ue_t *ue, int status)
{ f->rejects++; return OGS_OK; }
static void registration_metric(amf_metric_type_global_t metric) { }
static void registration_cause(uint8_t cause,
        amf_metric_type_by_cause_t metric, int value) { }

#define amf_ue_find_by_id registration_ue
#define ran_ue_find_by_id registration_ran
#define amf_sess_find_by_id registration_sess
#define amf_sess_remove registration_remove
#define amf_ue_new_guti registration_guti
#define amf_find_served_tai registration_tai
#define amf_ue_is_rat_restricted registration_rat
#define amf_update_allowed_nssai registration_nssai
#define amf_ue_sbi_discover_and_send registration_send
#define amf_ue_sbi_discover_and_send_eir registration_eir
#define amf_sbi_send_release_all_sessions registration_release
#define nas_5gs_send_authentication_reject registration_reject
#define nas_5gs_send_registration_reject registration_gmm_reject
#define nas_5gs_send_gmm_reject registration_gmm_reject
#define nas_5gs_send_security_mode_command registration_smc
#define amf_metrics_inst_global_inc registration_metric
#define amf_metrics_inst_by_cause_add registration_cause
#define amf_sbi_cancel_registration_session_release registration_cancel
#define amf_sbi_fail_registration_session_release registration_failure
#define ngap_send_ran_ue_context_release_command registration_ran_release
#define nas_5gs_send_gmm_reject_from_sbi registration_sbi_reject

#define gmm_handle_registration_request registration_request
#define gmm_handle_registration_update registration_update
#define gmm_registration_request_from_old_amf registration_old_amf
#define gmm_handle_service_request registration_service_request
#define gmm_handle_service_update registration_service_update
#define gmm_handle_deregistration_request registration_deregistration
#define gmm_handle_authentication_response registration_auth_response
#define gmm_handle_identity_response registration_identity_response
#define gmm_handle_security_mode_complete registration_security_complete
#define gmm_handle_ul_nas_transport registration_ul_transport
#undef GMM_HANDLER_H
#include "amf/gmm-handler.h"
#include "../../src/amf/gmm-handler.c"

#define gmm_state_initial registration_state_initial
#define gmm_state_final registration_state_final
#define gmm_state_de_registered registration_state_de_registered
#define gmm_state_authentication registration_state_authentication
#define gmm_state_security_mode registration_state_security_mode
#define gmm_state_initial_context_setup registration_state_context_setup
#define gmm_state_registered registration_state_registered
#define gmm_state_ue_context_will_remove registration_state_will_remove
#define gmm_state_exception registration_state_exception
void gmm_state_initial(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_final(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_de_registered(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_authentication(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_security_mode(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_initial_context_setup(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_registered(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_ue_context_will_remove(ogs_fsm_t *s, amf_event_t *e);
void gmm_state_exception(ogs_fsm_t *s, amf_event_t *e);

#define amf_nsmf_pdusession_handle_create_sm_context registration_create_context
#define amf_nsmf_pdusession_handle_update_sm_context registration_update_context
#define amf_nsmf_pdusession_handle_release_sm_context registration_release_context
#undef AMF_NSMF_HANDLER_H
#include "amf/nsmf-handler.h"

/* Keep cancellation and the timeout/discovery-failure decisions real. The
 * fixture owns its transaction storage; pool removal is the only substitute. */
#define ogs_sbi_xact_remove registration_xact_remove
#define amf_sbi_open registration_sbi_open
#define amf_sbi_close registration_sbi_close
#define amf_sbi_send_request registration_sbi_send_request
#undef amf_ue_sbi_discover_and_send
#define amf_ue_sbi_discover_and_send registration_real_ue_send
#undef amf_ue_sbi_discover_and_send_eir
#define amf_ue_sbi_discover_and_send_eir registration_real_eir_send
#define amf_sess_sbi_discover_and_send registration_real_sess_send
#define amf_sess_sbi_discover_by_nsi registration_discover_nsi
#define amf_sbi_send_activating_session registration_activate
#define amf_sbi_send_deactivate_session registration_deactivate
#define amf_sbi_send_deactivate_all_sessions registration_deactivate_all
#define amf_sbi_send_deactivate_all_ue_in_gnb registration_deactivate_gnb
#define amf_sbi_send_deactivate_stale_user_plane registration_deactivate_stale
#define amf_sbi_send_release_session registration_real_release_session
#undef amf_sbi_send_release_all_sessions
#define amf_sbi_send_release_all_sessions registration_real_release_all
#define amf_sbi_send_n1_n2_failure_notify registration_failure_notify
#define amf_ue_have_session_release_pending registration_ue_release_pending
#define amf_sess_have_session_release_pending registration_sess_release_pending
#define amf_sbi_xact_ctx_s registration_xact_ctx_s
#define amf_sbi_xact_ctx_t registration_xact_ctx_t
#undef AMF_SBI_PATH_H
#include "amf/sbi-path.h"
#include "../../src/amf/sbi-path.c"
#undef amf_ue_sbi_discover_and_send
#define amf_ue_sbi_discover_and_send registration_send
#undef amf_sbi_send_release_all_sessions
#define amf_sbi_send_release_all_sessions registration_release
#undef amf_ue_sbi_discover_and_send_eir
#define amf_ue_sbi_discover_and_send_eir registration_eir
#define amf_nnrf_handle_nf_discover registration_nf_discover
#define amf_nnrf_handle_failed_amf_discovery registration_discovery_failure
#undef AMF_NNRF_HANDLER_H
#include "amf/nnrf-handler.h"
#include "../../src/amf/nnrf-handler.c"
#include "../../src/amf/nsmf-handler.c"
#include "../../src/amf/gmm-sm.c"

static void timer_unused(void *data) { }

static void fixture_init(int sessions)
{
    int i;
    f = ogs_calloc(1, sizeof(*f));
    ogs_assert(f);
    f->num_of_integrity_order = amf_self()->num_of_integrity_order;
    f->integrity_order = amf_self()->integrity_order[0];
    f->eir_enabled = amf_self()->eir.enabled;
    f->unknown_action = amf_self()->eir.unknown_action;
    f->failure_action = amf_self()->eir.failure_action;
    f->missing_pei_action = amf_self()->eir.missing_pei_action;
    f->eir_result = OGS_OK;
    amf_self()->num_of_integrity_order = 1;
    amf_self()->integrity_order[0] = OGS_NAS_SECURITY_ALGORITHMS_128_NIA2;
    amf_self()->eir.enabled = false;
    f->ue.id = 1;
    f->ue.ran_ue_id = f->ran.id = 2;
    f->ran.amf_ue_id = f->ue.id;
    f->ran.ue_ctx_rel_action = NGAP_UE_CTX_REL_INVALID_ACTION;
    f->ue.suci = "suci-0-001-01-0-0-0-0000000001";
    f->ue.supi = "imsi-001010000000001";
    f->ue.pei = "imeisv-4901542032375186";
    f->ue.nas.access_type = OGS_ACCESS_TYPE_3GPP;
    f->ue.nas.message_type = OGS_NAS_5GS_REGISTRATION_REQUEST;
    f->ue.nas.ue.ksi = f->ue.nas.amf.ksi = 2;
    f->ue.nas.ue.tsc = f->ue.nas.amf.tsc = 1;
    f->ue.security_context_available = 1;
    f->ue.ue_security_capability.length = 2;
    f->ue.ue_security_capability.nr_ia = 0x20;
    f->ue.selected_int_algorithm = OGS_NAS_SECURITY_ALGORITHMS_128_NIA2;
    f->ue.ul_count.i32 = 41;
    f->ue.ul_count_accepted = true;
    f->ue.dl_count = 37;
    memset(f->ue.kamf, 0x11, sizeof(f->ue.kamf));
    memset(f->ue.knas_int, 0x22, sizeof(f->ue.knas_int));
    f->timers = ogs_timer_mgr_create(8);
#define TIMER(name) f->ue.name.timer = ogs_timer_add(f->timers, timer_unused, NULL)
    TIMER(t3513); TIMER(t3522); TIMER(t3550); TIMER(t3555);
    TIMER(t3560); TIMER(t3570); TIMER(mobile_reachable);
    TIMER(implicit_deregistration);
#undef TIMER
    for (i = 0; i < sessions; i++) {
        f->sess[i].id = 3 + i;
        f->sess[i].psi = 5 + i;
        f->sess[i].amf_ue_id = f->ue.id;
        f->sess[i].ran_ue_id = f->ran.id;
        f->sess[i].sbi.type = OGS_SBI_OBJ_SESS_TYPE;
        f->sess[i].sm_context_ref = "registered-sm-context";
        ogs_list_add(&f->ue.sess_list, &f->sess[i]);
    }
    OGS_FSM_TRAN(&f->ue.sm, gmm_state_registered);
}

static void fixture_final(void)
{
#define TIMER(name) ogs_timer_delete(f->ue.name.timer)
    TIMER(t3513); TIMER(t3522); TIMER(t3550); TIMER(t3555);
    TIMER(t3560); TIMER(t3570); TIMER(mobile_reachable);
    TIMER(implicit_deregistration);
#undef TIMER
    ogs_timer_mgr_destroy(f->timers);
    amf_self()->num_of_integrity_order = f->num_of_integrity_order;
    amf_self()->integrity_order[0] = f->integrity_order;
    amf_self()->eir.enabled = f->eir_enabled;
    amf_self()->eir.unknown_action = f->unknown_action;
    amf_self()->eir.failure_action = f->failure_action;
    amf_self()->eir.missing_pei_action = f->missing_pei_action;
    ogs_free(f);
    f = NULL;
}

static void dispatch(uint8_t type, bool protected, bool invalid)
{
    amf_event_t event = {0};
    ogs_nas_5gs_message_t message = {0}, inner = {0};
    ogs_nas_5gs_mobile_identity_guti_t guti = {0};
    ogs_nas_security_header_type_t h = {0};
    ogs_pkbuf_t *container = NULL;
    event.h.id = AMF_EVENT_5GMM_MESSAGE;
    event.amf_ue_id = f->ue.id;
    event.ran_ue_id = f->ran.id;
    event.ngap.code = NGAP_ProcedureCode_id_InitialUEMessage;
    event.nas.message = &message;
    h.integrity_protected = protected;
    event.nas.type = h.type;
    message.gmm.h.message_type = type;
    if (type == OGS_NAS_5GS_REGISTRATION_REQUEST) {
        message.gmm.registration_request.registration_type.value =
            OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;
        message.gmm.registration_request.registration_type.ksi = protected ? 2 : 7;
        guti.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_GUTI;
        message.gmm.registration_request.mobile_identity.length = sizeof(guti);
        message.gmm.registration_request.mobile_identity.buffer = &guti;
    } else if (type == OGS_NAS_5GS_AUTHENTICATION_RESPONSE) {
        message.gmm.authentication_response.authentication_response_parameter
            .length = OGS_MAX_RES_LEN;
        memset(message.gmm.authentication_response.authentication_response_parameter
                .res, 0xff, OGS_MAX_RES_LEN);
    } else if (type == OGS_NAS_5GS_AUTHENTICATION_FAILURE)
        message.gmm.authentication_failure.gmm_cause = OGS_5GMM_CAUSE_MAC_FAILURE;
    else if (type == OGS_NAS_5GS_SECURITY_MODE_COMPLETE && !invalid) {
        inner.gmm.h.extended_protocol_discriminator = OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
        inner.gmm.h.message_type = OGS_NAS_5GS_REGISTRATION_REQUEST;
        inner.gmm.registration_request.registration_type.value =
            OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;
        guti.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_GUTI;
        inner.gmm.registration_request.mobile_identity.length = sizeof(guti);
        inner.gmm.registration_request.mobile_identity.buffer = &guti;
        container = ogs_nas_5gs_plain_encode(&inner);
        ogs_assert(container);
        message.gmm.security_mode_complete.presencemask =
            OGS_NAS_5GS_SECURITY_MODE_COMPLETE_NAS_MESSAGE_CONTAINER_PRESENT;
        message.gmm.security_mode_complete.nas_message_container.length = container->len;
        message.gmm.security_mode_complete.nas_message_container.buffer = container->data;
    }
    ogs_fsm_dispatch(&f->ue.sm, &event);
    if (container) ogs_pkbuf_free(container);
}

static void assert_preserved(abts_case *tc, int sessions)
{
    ABTS_INT_EQUAL(tc, sessions, ogs_list_count(&f->ue.sess_list));
    ABTS_INT_EQUAL(tc, 0, f->releases);
    ABTS_INT_EQUAL(tc, 0, f->continuation);
}

static void assert_restored(abts_case *tc, int sessions)
{
    assert_preserved(tc, sessions);
    ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_registered));
    ABTS_INT_EQUAL(tc, 2, f->ue.nas.ue.ksi);
    ABTS_INT_EQUAL(tc, 2, f->ue.nas.amf.ksi);
    ABTS_INT_EQUAL(tc, 1, f->ue.nas.ue.tsc);
    ABTS_INT_EQUAL(tc, 1, f->ue.nas.amf.tsc);
    ABTS_INT_EQUAL(tc, 41, f->ue.ul_count.i32);
    ABTS_TRUE(tc, f->ue.ul_count_accepted);
    ABTS_INT_EQUAL(tc, 37, f->ue.dl_count);
    ABTS_INT_EQUAL(tc, 0x11, f->ue.kamf[0]);
    ABTS_INT_EQUAL(tc, 0x22, f->ue.knas_int[0]);
    ABTS_TRUE(tc, !memcmp(f->ue.kamf, f->ue.memento.kamf, sizeof(f->ue.kamf)));
    ABTS_TRUE(tc, !memcmp(f->ue.knas_int, f->ue.memento.knas_int,
                sizeof(f->ue.knas_int)));
    ABTS_TRUE(tc, SECURITY_CONTEXT_IS_VALID(&f->ue));
    ABTS_TRUE(tc, !f->ue.registration_session_release_after_authentication);
}

static void failures_preserve_sessions(abts_case *tc, void *data)
{
    int sessions = *(int *)data;
    int i;
    uint8_t failures[] = {
        OGS_NAS_5GS_AUTHENTICATION_RESPONSE,
        OGS_NAS_5GS_AUTHENTICATION_FAILURE,
        OGS_NAS_5GS_SECURITY_MODE_REJECT,
        OGS_NAS_5GS_SECURITY_MODE_COMPLETE,
    };
    for (i = 0; i < OGS_ARRAY_SIZE(failures); i++) {
        fixture_init(sessions);
        dispatch(OGS_NAS_5GS_REGISTRATION_REQUEST, false, false);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_authentication));
        ABTS_INT_EQUAL(tc, 1, f->auth);
        ABTS_TRUE(tc, f->ue.registration_session_release_after_authentication);
        assert_preserved(tc, sessions);
        memset(f->ue.kamf, 0x55, sizeof(f->ue.kamf));
        memset(f->ue.knas_int, 0x55, sizeof(f->ue.knas_int));
        f->ue.ul_count.i32 = f->ue.dl_count = 0;
        f->ue.ul_count_accepted = false;
        if (i >= 2) {
            OGS_FSM_TRAN(&f->ue.sm, gmm_state_security_mode);
            f->ue.security_context_available = 1;
        }
        dispatch(failures[i], i >= 2, true);
        assert_restored(tc, sessions);
        fixture_final();
    }
}

static void unverified_nas_is_discarded(abts_case *tc, void *data)
{
    int i, sessions = *(int *)data;
    for (i = 0; i < 2; i++) {
        fixture_init(sessions);
        dispatch(OGS_NAS_5GS_REGISTRATION_REQUEST, false, false);
        if (i) {
            OGS_FSM_TRAN(&f->ue.sm, gmm_state_security_mode);
            f->ue.security_context_available = 1;
            ogs_timer_start(f->ue.t3560.timer, ogs_time_from_sec(1));
            dispatch(OGS_NAS_5GS_SECURITY_MODE_COMPLETE, false, false);
            ABTS_TRUE(tc, f->ue.t3560.timer->running);
            f->ue.mac_failed = 1;
            dispatch(OGS_NAS_5GS_SECURITY_MODE_COMPLETE, true, false);
            ABTS_TRUE(tc, f->ue.t3560.timer->running);
            f->ue.mac_failed = 0;
        }
        dispatch(OGS_NAS_5GS_DEREGISTRATION_REQUEST_FROM_UE, false, false);
        assert_preserved(tc, sessions);
        ABTS_INT_EQUAL(tc, OGS_NAS_5GS_REGISTRATION_REQUEST, f->ue.nas.message_type);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm,
                    (i ? gmm_state_security_mode : gmm_state_authentication)));
        fixture_final();
    }
}

static void release_response(int i)
{
    ogs_sbi_xact_t *xact;
    amf_sess_t *sess = registration_sess(f->sess[i].id);
    if (sess) {
        ogs_list_for_each(&sess->sbi.xact_list, xact) {
            if (xact == &f->xact[i]) {
                ogs_list_remove(&sess->sbi.xact_list, xact);
                break;
            }
        }
    }
    amf_nsmf_pdusession_handle_release_sm_context(&f->ue, &f->ran, sess,
            f->xact[i].state);
}

static void start_security_mode(void)
{
    dispatch(OGS_NAS_5GS_REGISTRATION_REQUEST, false, false);
    OGS_FSM_TRAN(&f->ue.sm, gmm_state_security_mode);
    f->ue.security_context_available = 1;
}

static void equipment_response(int http_status, const char *problem_cause,
        OpenAPI_equipment_status_e status)
{
    amf_event_t event = {0};
    ogs_sbi_message_t message = {0};
    OpenAPI_eir_response_data_t response = {0};
    OpenAPI_problem_details_t problem = {0};
    event.h.id = OGS_EVENT_SBI_CLIENT;
    event.h.sbi.message = &message;
    event.amf_ue_id = f->ue.id;
    message.h.service.name = "n5g-eir-eic";
    message.h.resource.component[0] = OGS_SBI_RESOURCE_NAME_EQUIPMENT_STATUS;
    message.res_status = http_status;
    response.status = status;
    if (http_status == OGS_SBI_HTTP_STATUS_OK)
        message.EirResponseData = &response;
    if (problem_cause) {
        problem.cause = (char *)problem_cause;
        message.ProblemDetails = &problem;
    }
    ogs_fsm_dispatch(&f->ue.sm, &event);
}

static void equipment_status(OpenAPI_equipment_status_e status)
{
    equipment_response(OGS_SBI_HTTP_STATUS_OK, NULL, status);
}

static void eir_policy_completes_registration(abts_case *tc, void *data)
{
    enum { UNKNOWN, HTTP_FAILURE, SEND_FAILURE, ASYNC_FAILURE, MISSING_PEI };
    int scenario, reject, i, sessions = *(int *)data;

    for (scenario = UNKNOWN; scenario <= MISSING_PEI; scenario++) {
        for (reject = 0; reject < 2; reject++) {
            amf_event_t event = {0};
            ogs_eir_action_e action = reject ?
                OGS_EIR_ACTION_REJECT : OGS_EIR_ACTION_ALLOW;

            fixture_init(sessions);
            start_security_mode();
            amf_self()->eir.enabled = true;
            amf_self()->eir.unknown_action = action;
            amf_self()->eir.failure_action = action;
            amf_self()->eir.missing_pei_action = action;
            if (scenario == SEND_FAILURE)
                f->eir_result = OGS_ERROR;
            else if (scenario == MISSING_PEI)
                f->ue.pei = NULL;

            assert_preserved(tc, sessions);
            ABTS_TRUE(tc, f->ue.registration_session_release_after_authentication);
            dispatch(OGS_NAS_5GS_SECURITY_MODE_COMPLETE, true, false);
            ABTS_INT_EQUAL(tc, scenario != MISSING_PEI, f->eir);
            ABTS_TRUE(tc, !f->ue.can_restore_context);

            if (scenario == UNKNOWN || scenario == HTTP_FAILURE ||
                    scenario == ASYNC_FAILURE) {
                assert_preserved(tc, sessions);
                ABTS_TRUE(tc, f->ue.eir_check_pending);
                ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_security_mode));
                if (scenario == UNKNOWN)
                    equipment_response(OGS_SBI_HTTP_STATUS_NOT_FOUND,
                            "ERROR_EQUIPMENT_UNKNOWN", OpenAPI_equipment_status_NULL);
                else if (scenario == HTTP_FAILURE)
                    equipment_response(OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                            NULL, OpenAPI_equipment_status_NULL);
                else {
                    /* The timeout/discovery-failure event enters the real FSM;
                     * transport and wall-clock timers remain outside this fixture. */
                    event.h.id = AMF_EVENT_5GMM_EIR_FAILURE;
                    event.amf_ue_id = f->ue.id;
                    ogs_fsm_dispatch(&f->ue.sm, &event);
                }
            }

            ABTS_TRUE(tc, !f->ue.eir_check_pending);
            ABTS_INT_EQUAL(tc, 1, f->releases);
            ABTS_INT_EQUAL(tc, 0, f->continuation);
            ABTS_INT_EQUAL(tc, reject, f->rejects);
            ABTS_INT_EQUAL(tc, !reject, f->ue.registration_session_release_pending);
            ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, (reject ?
                        gmm_state_exception : gmm_state_initial_context_setup)));
            if (reject)
                ABTS_INT_EQUAL(tc, scenario == UNKNOWN || scenario == MISSING_PEI ?
                        OGS_5GMM_CAUSE_5GS_SERVICES_NOT_ALLOWED :
                        OGS_5GMM_CAUSE_PAYLOAD_WAS_NOT_FORWARDED, f->reject_cause);

            for (i = sessions - 1; i >= 0; i--) {
                ABTS_INT_EQUAL(tc, reject ? AMF_RELEASE_SM_CONTEXT_NO_STATE :
                        AMF_RELEASE_SM_CONTEXT_AUTHENTICATED_REGISTRATION,
                        f->xact[i].state);
                release_response(i);
                ABTS_INT_EQUAL(tc, i, ogs_list_count(&f->ue.sess_list));
                ABTS_INT_EQUAL(tc, i, amf_sess_xact_count(&f->ue));
                ABTS_INT_EQUAL(tc, !reject && i == 0, f->continuation);
            }
            ABTS_TRUE(tc, !f->ue.registration_session_release_pending);
            ABTS_INT_EQUAL(tc, reject, f->ran_releases);
            ABTS_INT_EQUAL(tc, 1, f->auth);
            if (reject)
                ABTS_INT_EQUAL(tc, NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE,
                        f->ran.ue_ctx_rel_action);
            fixture_final();
        }
    }
}

static void eir_and_last_release(abts_case *tc, void *data)
{
    int enabled, sessions = *(int *)data;
    for (enabled = 0; enabled < 2; enabled++) {
        fixture_init(sessions);
        start_security_mode();
        amf_self()->eir.enabled = enabled;
        dispatch(OGS_NAS_5GS_SECURITY_MODE_COMPLETE, true, false);
        ABTS_TRUE(tc, !f->ue.can_restore_context);
        if (enabled) {
            assert_preserved(tc, sessions);
            ABTS_INT_EQUAL(tc, 1, f->eir);
            ABTS_TRUE(tc, f->ue.eir_check_pending);
            equipment_status(OpenAPI_equipment_status_WHITELISTED);
        }
        ABTS_INT_EQUAL(tc, 1, f->releases);
        ABTS_INT_EQUAL(tc, 0, f->continuation);
        ABTS_TRUE(tc, f->ue.registration_session_release_pending);
        dispatch(OGS_NAS_5GS_REGISTRATION_REQUEST, false, false);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_initial_context_setup));
        ABTS_TRUE(tc, f->ue.registration_session_release_pending);
        ABTS_INT_EQUAL(tc, 1, f->auth);
        ABTS_INT_EQUAL(tc, 1, f->releases);
        ABTS_INT_EQUAL(tc, 0, f->continuation);
        ABTS_INT_EQUAL(tc, sessions, ogs_list_count(&f->ue.sess_list));
        ABTS_INT_EQUAL(tc, sessions, amf_sess_xact_count(&f->ue));
        if (sessions == 2) {
            release_response(1);
            ABTS_INT_EQUAL(tc, 0, f->continuation);
            ABTS_INT_EQUAL(tc, 1, ogs_list_count(&f->ue.sess_list));
        }
        release_response(0);
        ABTS_INT_EQUAL(tc, 0, ogs_list_count(&f->ue.sess_list));
        ABTS_INT_EQUAL(tc, 1, f->continuation);
        ABTS_TRUE(tc, !f->ue.registration_session_release_pending);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_initial_context_setup));
        release_response(0);
        ABTS_INT_EQUAL(tc, 1, f->continuation);
        fixture_final();
    }
}

static void cancelled_release_does_not_resume(abts_case *tc, void *data)
{
    int sessions = *(int *)data, reason, i;
    for (reason = 0; reason < 4; reason++) {
        fixture_init(sessions);
        start_security_mode();
        dispatch(OGS_NAS_5GS_SECURITY_MODE_COMPLETE, true, false);
        if (reason == 0)
            f->ue.registration_session_release_pending = false;
        else if (reason == 1)
            f->ran.ue_ctx_rel_action = NGAP_UE_CTX_REL_NG_CONTEXT_REMOVE;
        else if (reason == 2)
            f->ue.ran_ue_id = OGS_INVALID_POOL_ID;
        else
            OGS_FSM_TRAN(&f->ue.sm, gmm_state_authentication);
        for (i = sessions - 1; i >= 0; i--)
            release_response(i);
        ABTS_INT_EQUAL(tc, 0, f->continuation);
        ABTS_INT_EQUAL(tc, 0, ogs_list_count(&f->ue.sess_list));
        fixture_final();
    }
}

static void other_procedures_keep_sessions(abts_case *tc, void *data)
{
    int service, sessions = *(int *)data;
    for (service = 0; service < 2; service++) {
        fixture_init(sessions);
        /* Model completion of a transferred Registration or a Service
         * Request. Neither procedure selected same-AMF re-registration
         * deletion, so passing SMC must preserve the existing sessions. */
        f->ue.nas.message_type = service ? OGS_NAS_5GS_SERVICE_REQUEST :
            OGS_NAS_5GS_REGISTRATION_REQUEST;
        f->ue.can_restore_context = false;
        f->ue.registration_session_release_after_authentication = false;
        OGS_FSM_TRAN(&f->ue.sm, gmm_state_security_mode);
        gmm_security_mode_completed(&f->ue.sm, &f->ue);
        ABTS_INT_EQUAL(tc, sessions, ogs_list_count(&f->ue.sess_list));
        ABTS_INT_EQUAL(tc, 0, f->releases);
        ABTS_INT_EQUAL(tc, 1, f->continuation);
        ABTS_TRUE(tc, !f->ue.registration_session_release_pending);
        fixture_final();
    }
}

static void eir_rejection_releases_sessions(abts_case *tc, void *data)
{
    int sessions = *(int *)data;
    fixture_init(sessions);
    start_security_mode();
    amf_self()->eir.enabled = true;
    dispatch(OGS_NAS_5GS_SECURITY_MODE_COMPLETE, true, false);
    assert_preserved(tc, sessions);
    equipment_status(OpenAPI_equipment_status_BLACKLISTED);
    ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_exception));
    ABTS_TRUE(tc, !f->ue.can_restore_context);
    ABTS_INT_EQUAL(tc, 1, f->releases);
    ABTS_INT_EQUAL(tc, 0, f->continuation);
    ABTS_INT_EQUAL(tc, 1, f->rejects);
    fixture_final();
}

static void cancel_removes_only_registration_transactions(abts_case *tc, void *data)
{
    int i, sessions = *(int *)data;
    fixture_init(sessions);
    start_security_mode();
    dispatch(OGS_NAS_5GS_SECURITY_MODE_COMPLETE, true, false);
    f->unrelated_xact.state = AMF_UPDATE_SM_CONTEXT_MODIFIED;
    f->unrelated_xact.sbi_object = &f->sess[0].sbi;
    ogs_list_add(&f->sess[0].sbi.xact_list, &f->unrelated_xact);
    amf_sbi_cancel_registration_session_release(&f->ue);
    ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_initial_context_setup));
    ABTS_TRUE(tc, !f->ue.registration_session_release_pending);
    ABTS_TRUE(tc, !f->ue.registration_session_release_after_authentication);
    ABTS_INT_EQUAL(tc, 1, amf_sess_xact_count(&f->ue));
    ABTS_PTR_EQUAL(tc, &f->unrelated_xact,
            ogs_list_first(&f->sess[0].sbi.xact_list));
    ogs_list_remove(&f->sess[0].sbi.xact_list, &f->unrelated_xact);
    for (i = sessions - 1; i >= 0; i--)
        release_response(i);
    ABTS_INT_EQUAL(tc, 0, f->continuation);
    ABTS_INT_EQUAL(tc, 0, ogs_list_count(&f->ue.sess_list));
    fixture_final();
}

static void release_timeout_cancels_registration(abts_case *tc, void *data)
{
    int present, i, sessions = *(int *)data;
    for (present = 0; present < 3; present++) {
        fixture_init(sessions);
        start_security_mode();
        dispatch(OGS_NAS_5GS_SECURITY_MODE_COMPLETE, true, false);
        if (!present)
            f->ran_removed = true;
        else if (present == 2)
            f->ue.ran_ue_id = OGS_INVALID_POOL_ID;
        /* This is the production timeout/discovery failure handler invoked
         * synchronously; no peer timeout or artificial timer ENTRY is used. */
        amf_nnrf_handle_failed_amf_discovery(&f->xact[0]);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_initial_context_setup));
        ABTS_INT_EQUAL(tc, sessions, ogs_list_count(&f->ue.sess_list));
        ABTS_INT_EQUAL(tc, present == 1, f->ran_releases);
        ABTS_INT_EQUAL(tc, present == 1, f->rejects);
        ABTS_TRUE(tc, !f->ue.registration_session_release_pending);
        ABTS_INT_EQUAL(tc, 0, amf_sess_xact_count(&f->ue));
        if (present == 1) {
            ABTS_INT_EQUAL(tc, NGAP_UE_CTX_REL_UE_CONTEXT_REMOVE,
                    f->ran.ue_ctx_rel_action);
        }
        for (i = sessions - 1; i >= 0; i--)
            release_response(i);
        ABTS_INT_EQUAL(tc, 0, f->continuation);
        fixture_final();
    }
}

static void restarted_registration_retains_deferred_release(abts_case *tc, void *data)
{
    int sessions = *(int *)data;
    fixture_init(sessions);
    OGS_FSM_TRAN(&f->ue.sm, gmm_state_initial_context_setup);
    dispatch(OGS_NAS_5GS_REGISTRATION_REQUEST, false, false);
    ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_authentication));
    ABTS_TRUE(tc, f->ue.registration_session_release_after_authentication);
    assert_preserved(tc, sessions);
    OGS_FSM_TRAN(&f->ue.sm, gmm_state_security_mode);
    f->ue.security_context_available = 1;
    dispatch(OGS_NAS_5GS_SECURITY_MODE_COMPLETE, true, false);
    ABTS_INT_EQUAL(tc, 1, f->releases);
    ABTS_TRUE(tc, f->ue.registration_session_release_pending);
    fixture_final();
}

static void pending_activation_allows_registration(abts_case *tc, void *data)
{
    int initial_context_setup, sessions = *(int *)data;
    for (initial_context_setup = 0; initial_context_setup < 2;
            initial_context_setup++) {
        fixture_init(sessions);
        if (initial_context_setup)
            OGS_FSM_TRAN(&f->ue.sm, gmm_state_initial_context_setup);
        /* The N3 path can be usable before the final ACTIVATED response
         * arrives. This unrelated transaction must not discard a new
         * Registration Request or release the established sessions. */
        f->unrelated_xact.state = AMF_UPDATE_SM_CONTEXT_ACTIVATED;
        f->unrelated_xact.sbi_object = &f->sess[0].sbi;
        ogs_list_add(&f->sess[0].sbi.xact_list, &f->unrelated_xact);
        dispatch(OGS_NAS_5GS_REGISTRATION_REQUEST, false, false);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_authentication));
        ABTS_INT_EQUAL(tc, 1, f->auth);
        assert_preserved(tc, sessions);
        ABTS_TRUE(tc, f->ue.registration_session_release_after_authentication);
        ABTS_TRUE(tc, !f->ue.registration_session_release_pending);
        ABTS_INT_EQUAL(tc, 1, amf_sess_xact_count(&f->ue));
        ABTS_PTR_EQUAL(tc, &f->unrelated_xact,
                ogs_list_first(&f->sess[0].sbi.xact_list));
        ABTS_INT_EQUAL(tc, AMF_UPDATE_SM_CONTEXT_ACTIVATED,
                f->unrelated_xact.state);
        fixture_final();
    }

    fixture_init(sessions);
    f->unrelated_xact.state = AMF_UPDATE_SM_CONTEXT_ACTIVATED;
    f->unrelated_xact.sbi_object = &f->sess[0].sbi;
    ogs_list_add(&f->sess[0].sbi.xact_list, &f->unrelated_xact);
    dispatch(OGS_NAS_5GS_REGISTRATION_REQUEST, true, false);
    ABTS_INT_EQUAL(tc, 0, f->auth);
    ABTS_INT_EQUAL(tc, 0, f->releases);
    ABTS_INT_EQUAL(tc, 1, f->continuation);
    ABTS_INT_EQUAL(tc, sessions, ogs_list_count(&f->ue.sess_list));
    ABTS_TRUE(tc, !f->ue.registration_session_release_after_authentication);
    ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_initial_context_setup));
    ABTS_INT_EQUAL(tc, 1, amf_sess_xact_count(&f->ue));
    ABTS_PTR_EQUAL(tc, &f->unrelated_xact,
            ogs_list_first(&f->sess[0].sbi.xact_list));
    fixture_final();
}

static void late_release_does_not_restart_authentication(abts_case *tc, void *data)
{
    int started, i, sessions = *(int *)data;
    for (started = 0; started < 2; started++) {
        fixture_init(sessions);
        for (i = 0; i < sessions; i++) {
            f->xact[i].state = AMF_RELEASE_SM_CONTEXT_NO_STATE;
            f->xact[i].sbi_object = &f->sess[i].sbi;
            ogs_list_add(&f->sess[i].sbi.xact_list, &f->xact[i]);
        }
        if (started) {
            dispatch(OGS_NAS_5GS_REGISTRATION_REQUEST, false, false);
            assert_preserved(tc, sessions);
        } else {
            /* Existing release-driven authentication starts only after
             * the last release response. No AKA has been requested yet. */
            OGS_FSM_TRAN(&f->ue.sm, gmm_state_authentication);
        }
        ABTS_INT_EQUAL(tc, started, f->auth);
        ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_authentication));
        ABTS_INT_EQUAL(tc, started,
                f->ue.registration_session_release_after_authentication);

        for (i = 0; i < sessions; i++) {
            /* amf-sm.c removes the completed SBI transaction before
             * delivering its response to the production Nsmf handler. */
            ogs_list_remove(&f->sess[i].sbi.xact_list, &f->xact[i]);
            amf_nsmf_pdusession_handle_release_sm_context(
                    &f->ue, &f->ran, &f->sess[i], AMF_RELEASE_SM_CONTEXT_NO_STATE);
            ABTS_INT_EQUAL(tc, started || i == sessions - 1, f->auth);
            ABTS_INT_EQUAL(tc, sessions - i - 1,
                    ogs_list_count(&f->ue.sess_list));
            ABTS_INT_EQUAL(tc, sessions - i - 1, amf_sess_xact_count(&f->ue));
            ABTS_TRUE(tc, OGS_FSM_CHECK(&f->ue.sm, gmm_state_authentication));
            ABTS_INT_EQUAL(tc, 0, f->releases);
            ABTS_INT_EQUAL(tc, 0, f->continuation);
        }
        fixture_final();
    }
}

abts_suite *test_amf_registration(abts_suite *suite)
{
    int sessions[] = { 1, 2 };
    int i;
    ogs_log_level_e saved, saved_amf, saved_core;
    suite = ADD_SUITE(suite)
    if (!ogs_log_find_domain("gmm"))
        ogs_log_install_domain(&__gmm_log_domain, "gmm", OGS_LOG_NONE);
    saved = ogs_log_get_domain_level(__gmm_log_domain);
    saved_amf = ogs_log_get_domain_level(__amf_log_domain);
    saved_core = ogs_log_get_domain_level(ogs_log_get_domain_id("core"));
    ogs_log_set_domain_level(__gmm_log_domain, OGS_LOG_NONE);
    ogs_log_set_domain_level(__amf_log_domain, OGS_LOG_NONE);
    ogs_log_set_domain_level(ogs_log_get_domain_id("core"), OGS_LOG_NONE);
    for (i = 0; i < OGS_ARRAY_SIZE(sessions); i++) {
        abts_run_test(suite, failures_preserve_sessions, &sessions[i]);
        abts_run_test(suite, unverified_nas_is_discarded, &sessions[i]);
        abts_run_test(suite, eir_and_last_release, &sessions[i]);
        abts_run_test(suite, eir_policy_completes_registration, &sessions[i]);
        abts_run_test(suite, cancelled_release_does_not_resume, &sessions[i]);
        abts_run_test(suite, other_procedures_keep_sessions, &sessions[i]);
        abts_run_test(suite, eir_rejection_releases_sessions, &sessions[i]);
        abts_run_test(suite, cancel_removes_only_registration_transactions, &sessions[i]);
        abts_run_test(suite, release_timeout_cancels_registration, &sessions[i]);
        abts_run_test(suite, restarted_registration_retains_deferred_release, &sessions[i]);
        abts_run_test(suite, pending_activation_allows_registration, &sessions[i]);
        abts_run_test(suite, late_release_does_not_restart_authentication, &sessions[i]);
    }
    ogs_log_set_domain_level(__gmm_log_domain, saved);
    ogs_log_set_domain_level(__amf_log_domain, saved_amf);
    ogs_log_set_domain_level(ogs_log_get_domain_id("core"), saved_core);
    return suite;
}
