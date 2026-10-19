/*
 * Copyright (C) 2026 by Open5GS contributors
 *
 * SPDX-License-Identifier: AGPL-3.0-or-later
 */

/* Run the real ESM handlers and S6a updates. Only context lookup and outbound
 * messages are substituted; APN selection, snapshots and PDN checks are real. */
#include "mme/esm-handler.h"
#include "mme/nas-path.h"
#include "mme/sgsap-path.h"
#include "mme/mme-gtp-path.h"
#include "mme/mme-s6a-handler.h"
#include "core/abts.h"

static struct {
    mme_ue_t ue;
    enb_ue_t enb;
    mme_sess_t sess;
    mme_bearer_t bearer;
    sgw_ue_t sgw;
    ogs_timer_mgr_t *timers;
    ogs_session_t profile[2];
    int creates, information_requests, rejects;
    ogs_log_level_e log_level;
} *f;

static mme_ue_t *apn_ue(ogs_pool_id_t id)
{ return id == f->ue.id ? &f->ue : NULL; }
static mme_sess_t *apn_sess(ogs_pool_id_t id)
{ return id == f->sess.id ? &f->sess : NULL; }
static sgw_ue_t *apn_sgw(ogs_pool_id_t id)
{ return id == f->sgw.id ? &f->sgw : NULL; }
static mme_csmap_t *apn_csmap(const ogs_eps_tai_t *tai)
{ return NULL; }
static int apn_create(enb_ue_t *enb, mme_sess_t *sess, int action)
{ f->creates++; return OGS_OK; }
static int apn_information(mme_bearer_t *bearer)
{ f->information_requests++; return OGS_OK; }
static int apn_accept(mme_ue_t *ue)
{ return OGS_OK; }
static int apn_reject(mme_sess_t *sess, uint8_t cause, int action)
{ f->rejects++; return OGS_OK; }

#define mme_ue_find_by_id apn_ue
#define mme_sess_find_by_id apn_sess
#define sgw_ue_find_by_id apn_sgw
#define mme_csmap_find_by_tai apn_csmap
#define mme_gtp_send_create_session_request apn_create
#define nas_eps_send_esm_information_request apn_information
#define nas_eps_send_attach_accept apn_accept
#define nas_eps_send_pdn_connectivity_reject apn_reject
#define esm_handle_pdn_connectivity_request apn_handle_request
#define esm_handle_information_response apn_handle_information
#define esm_handle_bearer_resource_allocation_request apn_handle_allocation
#define esm_handle_bearer_resource_modification_request apn_handle_modification
#include "../../src/mme/esm-handler.c"

/* Reach the production Gn import helper with the same local session fixture. */
#define mme_gn_handle_echo_request apn_gn_echo_request
#define mme_gn_handle_echo_response apn_gn_echo_response
#define mme_gn_handle_sgsn_context_request apn_gn_context_request
#define mme_gn_handle_sgsn_context_response apn_gn_context_response
#define mme_gn_handle_sgsn_context_acknowledge apn_gn_context_acknowledge
#define mme_gn_handle_ran_information_relay apn_gn_information_relay
#define guti_to_rai_ptmsi apn_guti_to_rai_ptmsi
#include "../../src/mme/mme-gn-handler.c"

static ogs_session_t apn_config(
        const char *name, uint32_t id, uint8_t type, uint8_t qci)
{
    ogs_session_t session = {0};
    session.name = (char *)name;
    session.context_identifier = id;
    session.session_type = type;
    session.qos.index = qci;
    session.qos.arp.priority_level = 1 + id % 15;
    session.qos.arp.pre_emption_capability = id % 2;
    session.qos.arp.pre_emption_vulnerability = (id + 1) % 2;
    session.ambr.uplink = 1000 * id;
    session.ambr.downlink = 2000 * id;
    session.ue_ip.ipv4 = session.smf_ip.ipv4 = 1;
    session.ue_ip.len = session.smf_ip.len = OGS_IPV4_LEN;
    session.ue_ip.addr = htobe32(0x0a000001 + id);
    session.smf_ip.addr = htobe32(0xc0000201 + id);
    session.charging_characteristics_presence = true;
    session.charging_characteristics[0] = id;
    session.charging_characteristics[1] = id + 1;
    return session;
}

/* Templates keep borrowed names; each Diameter message owns its input names. */
static uint8_t subscription(int count, uint32_t default_id)
{
    ogs_diam_s6a_message_t *message = ogs_calloc(1, sizeof(*message));
    ogs_subscription_data_t *data;
    ogs_slice_data_t *slice;
    uint8_t rv;
    int i;
    ogs_assert(message);
    data = &message->idr_message.subscription_data;
    message->idr_message.subdatamask = OGS_DIAM_S6A_SUBDATA_APN_CONFIG;
    data->num_of_slice = 1;
    slice = &data->slice[0];
    slice->all_apn_config_inc = OGS_ALL_APN_CONFIGURATIONS_INCLUDED;
    slice->context_identifier = default_id;
    slice->num_of_session = count;
    for (i = 0; i < count; i++) {
        slice->session[i] = f->profile[i];
        slice->session[i].name = ogs_strdup(f->profile[i].name);
        ogs_assert(slice->session[i].name);
    }
    rv = mme_s6a_handle_idr(&f->ue, message);
    ogs_subscription_data_free(data);
    ogs_free(message);
    return rv;
}

static void session_equal(abts_case *tc,
        const ogs_session_t *expected, const ogs_session_t *actual)
{
    ABTS_PTR_NOTNULL(tc, actual);
    if (!actual)
        return;
    ABTS_STR_EQUAL(tc, expected->name, actual->name);
    ABTS_INT_EQUAL(tc, expected->context_identifier,
            actual->context_identifier);
    ABTS_INT_EQUAL(tc, expected->session_type, actual->session_type);
    ABTS_TRUE(tc, !memcmp(&expected->qos, &actual->qos, sizeof(actual->qos)));
    ABTS_TRUE(tc, !memcmp(&expected->ambr, &actual->ambr,
                sizeof(actual->ambr)));
    ABTS_TRUE(tc, !memcmp(&expected->ue_ip, &actual->ue_ip,
                sizeof(actual->ue_ip)));
    ABTS_TRUE(tc, !memcmp(&expected->smf_ip, &actual->smf_ip,
                sizeof(actual->smf_ip)));
    ABTS_INT_EQUAL(tc, expected->charging_characteristics_presence,
            actual->charging_characteristics_presence);
    ABTS_TRUE(tc, !memcmp(expected->charging_characteristics,
                actual->charging_characteristics, OGS_CHRGCHARS_LEN));
}

static void timer_unused(void *data) { }

static void fixture_init(void)
{
    f = ogs_calloc(1, sizeof(*f));
    ogs_assert(f);
    f->ue.id = 1;
    f->ue.imsi_len = 8;
    f->ue.security_context_available = 1;
    f->enb.id = f->ue.enb_ue_id = 2;
    f->sess.id = f->bearer.sess_id = 3;
    f->sess.mme_ue_id = f->bearer.mme_ue_id = f->ue.id;
    f->sgw.id = f->ue.sgw_ue_id = 4;
    ogs_list_add(&f->ue.sess_list, &f->sess);
    ogs_list_add(&f->sess.bearer_list, &f->bearer);
    f->timers = ogs_timer_mgr_create(1);
    f->bearer.t3489.timer = ogs_timer_add(f->timers, timer_unused, NULL);
    if (!__esm_log_domain)
        ogs_log_install_domain(&__esm_log_domain, "esm", OGS_LOG_ERROR);
    f->log_level = ogs_log_get_domain_level(__esm_log_domain);
    if (f->log_level >= OGS_LOG_ERROR && f->log_level < OGS_LOG_DEBUG)
        ogs_log_set_domain_level(__esm_log_domain, OGS_LOG_FATAL);
    f->profile[0] = apn_config("internet", 1, OGS_PDU_SESSION_TYPE_IPV4V6, 9);
    f->profile[1] = apn_config("ims", 2, OGS_PDU_SESSION_TYPE_IPV4, 5);
    ogs_assert(subscription(2, 1) == OGS_OK);
}

static void fixture_final(void)
{
    mme_sess_set_session(&f->sess, NULL);
    mme_session_remove_all(&f->ue);
    ogs_timer_delete(f->bearer.t3489.timer);
    ogs_timer_mgr_destroy(f->timers);
    ogs_log_set_domain_level(__esm_log_domain, f->log_level);
    ogs_free(f);
    f = NULL;
}

static int request(const char *apn, uint8_t type, bool eit)
{
    ogs_nas_eps_pdn_connectivity_request_t req = {0};
    req.request_type.type = type;
    req.request_type.value = OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    if (apn) {
        req.presencemask |=
            OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT;
        req.access_point_name.length = strlen(apn);
        ogs_cpystrn(req.access_point_name.apn, apn,
                sizeof(req.access_point_name.apn));
    }
    if (eit) {
        req.presencemask |=
            OGS_NAS_EPS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_PRESENT;
        req.esm_information_transfer_flag.security_protected_required = 1;
    }
    return apn_handle_request(&f->enb, &f->bearer, &req,
            OGS_GTP_CREATE_IN_ATTACH_REQUEST);
}

static int information_response(const char *apn)
{
    ogs_nas_eps_esm_information_response_t rsp = {0};
    if (apn) {
        rsp.presencemask |=
            OGS_NAS_EPS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT;
        rsp.access_point_name.length = strlen(apn);
        ogs_cpystrn(rsp.access_point_name.apn, apn,
                sizeof(rsp.access_point_name.apn));
    }
    return apn_handle_information(&f->enb, &f->sess, &rsp);
}

static void idr_reorders_apns(abts_case *tc, void *data)
{
    ogs_session_t expected, *selected;
    fixture_init();
    expected = f->profile[1];
    ABTS_INT_EQUAL(tc, OGS_OK,
            request("ims", OGS_NAS_EPS_PDN_TYPE_IPV4, false));
    selected = f->sess.session;
    f->profile[0] = apn_config("ims", 20, OGS_PDU_SESSION_TYPE_IPV6, 7);
    f->profile[1] = apn_config("internet", 10, OGS_PDU_SESSION_TYPE_IPV4V6, 9);
    ABTS_INT_EQUAL(tc, OGS_OK, subscription(2, 10));
    ABTS_INT_EQUAL(tc, 2, f->ue.num_of_session);
    session_equal(tc, &f->profile[0], mme_session_find_by_apn(&f->ue, "ims"));
    session_equal(tc, &f->profile[1], mme_default_session(&f->ue));
    ABTS_PTR_EQUAL(tc, selected, f->sess.session);
    session_equal(tc, &expected, f->sess.session);
    fixture_final();
}

static void idr_removes_active_apn(abts_case *tc, void *data)
{
    fixture_init();
    ABTS_INT_EQUAL(tc, OGS_OK,
            request("ims", OGS_NAS_EPS_PDN_TYPE_IPV4, false));
    ABTS_INT_EQUAL(tc, OGS_OK, subscription(1, 1));
    ABTS_PTR_EQUAL(tc, NULL, mme_session_find_by_apn(&f->ue, "ims"));
    ABTS_PTR_EQUAL(tc, &f->sess, mme_sess_find_by_apn(&f->ue, "ims"));
    session_equal(tc, &f->profile[1], f->sess.session);

    mme_sess_set_session(&f->sess, NULL);
    ABTS_PTR_EQUAL(tc, NULL, f->sess.session);
    ABTS_PTR_EQUAL(tc, NULL, mme_session_find_by_apn(&f->ue, "ims"));
    ABTS_PTR_EQUAL(tc, NULL, mme_sess_find_by_apn(&f->ue, "ims"));
    ABTS_INT_EQUAL(tc, 1, f->ue.num_of_session);
    session_equal(tc, &f->profile[0], mme_default_session(&f->ue));
    fixture_final();
}

static void idr_empty_profile(abts_case *tc, void *data)
{
    ogs_log_level_e level;
    fixture_init();
    ABTS_INT_EQUAL(tc, OGS_OK,
            request("ims", OGS_NAS_EPS_PDN_TYPE_IPV4, false));
    level = ogs_log_get_domain_level(__mme_log_domain);
    if (level >= OGS_LOG_ERROR && level < OGS_LOG_DEBUG)
        ogs_log_set_domain_level(__mme_log_domain, OGS_LOG_FATAL);
    ABTS_INT_EQUAL(tc, (uint8_t)OGS_ERROR, subscription(0, 1));
    ABTS_INT_EQUAL(tc, 0, f->ue.num_of_session);
    ABTS_PTR_EQUAL(tc, NULL, f->ue.session[0].name);
    ABTS_PTR_EQUAL(tc, NULL, f->ue.session[1].name);
    ABTS_PTR_EQUAL(tc, NULL, mme_default_session(&f->ue));
    session_equal(tc, &f->profile[1], f->sess.session);

    /* Invalid PDN type frees its newly copied name before rejecting. */
    f->profile[0] = apn_config("bad", 3, OGS_PDU_SESSION_TYPE_UNSTRUCTURED, 9);
    ABTS_INT_EQUAL(tc, (uint8_t)OGS_ERROR, subscription(1, 3));
    ABTS_INT_EQUAL(tc, 0, f->ue.num_of_session);
    ABTS_PTR_EQUAL(tc, NULL, f->ue.session[0].name);
    session_equal(tc, &f->profile[1], f->sess.session);
    ogs_log_set_domain_level(__mme_log_domain, level);
    fixture_final();
}

static void selected_snapshot_survives_idr(abts_case *tc, void *data)
{
    int removed;
    for (removed = 0; removed < 2; removed++) {
        ogs_session_t expected, *selected;
        fixture_init();
        expected = f->profile[1];
        ABTS_INT_EQUAL(tc, OGS_OK,
                request("ims", OGS_NAS_EPS_PDN_TYPE_IPV4, true));
        ABTS_INT_EQUAL(tc, 1, f->information_requests);
        selected = f->sess.session;
        f->profile[1] = apn_config(removed ? "corporate" : "ims",
                20, OGS_PDU_SESSION_TYPE_IPV4V6, 7);
        ABTS_INT_EQUAL(tc, OGS_OK, subscription(2, 1));
        ABTS_PTR_EQUAL(tc, selected, f->sess.session);
        /* An omitted response APN keeps the selection made before IDR. */
        ABTS_INT_EQUAL(tc, OGS_OK, information_response(NULL));
        ABTS_PTR_EQUAL(tc, selected, f->sess.session);
        session_equal(tc, &expected, f->sess.session);
        ABTS_INT_EQUAL(tc, 1, f->creates);
        ABTS_INT_EQUAL(tc, 0, f->rejects);
        fixture_final();
    }
}

static void gn_import_preserves_pdn_type(abts_case *tc, void *data)
{
    int subscribed;
    for (subscribed = 0; subscribed < 2; subscribed++) {
        ogs_gtp1_pdp_context_decoded_t pdp = {0};
        ogs_session_t expected;
        fixture_init();
        if (!subscribed)
            mme_session_remove_all(&f->ue);
        f->sess.pti = 1;
        f->bearer.ebi = pdp.nsapi = 5;
        pdp.pdp_ctx_id = 1;
        pdp.pdp_type_num[0] = subscribed ?
            OGS_PDU_SESSION_TYPE_IPV4 : OGS_PDU_SESSION_TYPE_IPV4V6;
        pdp.pdp_address[0].ipv4 = 1;
        pdp.pdp_address[0].ipv6 = !subscribed;
        ogs_cpystrn(pdp.apn, subscribed ? "internet" : "roaming",
                sizeof(pdp.apn));

        ABTS_PTR_EQUAL(tc, &f->sess,
                mme_ue_session_from_gtp1_pdp_ctx(&f->ue, &pdp));
        ABTS_TRUE(tc, f->sess.session != &f->ue.session[0]);
        ABTS_TRUE(tc, f->sess.session->name != f->ue.session[0].name);
        /* Fill a missing type on the snapshot only; preserve an existing
         * IPv4v6 subscription even when the transferred PDN uses IPv4. */
        ABTS_INT_EQUAL(tc, subscribed ? OGS_PDU_SESSION_TYPE_IPV4V6 : 0,
                f->ue.session[0].session_type);
        ABTS_INT_EQUAL(tc, pdp.pdp_type_num[0], f->sess.ue_request_type.type);
        expected = f->ue.session[0];
        expected.name = pdp.apn;
        expected.session_type = OGS_PDU_SESSION_TYPE_IPV4V6;
        session_equal(tc, &expected, f->sess.session);
        ABTS_INT_EQUAL(tc, OGS_OK, subscription(2, 1));
        session_equal(tc, &expected, f->sess.session);
        fixture_final();
    }
}

static void apn_selection_is_owned(abts_case *tc, void *data)
{
    int mode;
    /* Default request, explicit request, and explicit Information Response. */
    for (mode = 0; mode < 3; mode++) {
        int index = mode ? 1 : 0;
        const ogs_session_t *expected;
        fixture_init();
        expected = &f->profile[index];
        if (mode == 2) {
            ABTS_INT_EQUAL(tc, OGS_OK,
                    request("internet", OGS_NAS_EPS_PDN_TYPE_IPV4, true));
            session_equal(tc, &f->profile[0], f->sess.session);
            ABTS_INT_EQUAL(tc, OGS_OK, information_response("ims"));
        } else {
            ABTS_INT_EQUAL(tc, OGS_OK,
                    request(mode ? "ims" : NULL,
                        OGS_NAS_EPS_PDN_TYPE_IPV4, false));
        }
        ABTS_INT_EQUAL(tc, 1, f->creates);
        ABTS_INT_EQUAL(tc, 0, f->rejects);
        ABTS_TRUE(tc, f->sess.session != &f->ue.session[index]);
        ABTS_TRUE(tc, f->sess.session->name != f->ue.session[index].name);
        session_equal(tc, expected, f->sess.session);
        mme_session_remove_all(&f->ue);
        session_equal(tc, expected, f->sess.session);
        ABTS_PTR_EQUAL(tc, &f->sess,
                mme_sess_find_by_apn(&f->ue, expected->name));
        fixture_final();
    }
}

abts_suite *test_mme_apn(abts_suite *suite)
{
    suite = ADD_SUITE(suite)
    abts_run_test(suite, idr_reorders_apns, NULL);
    abts_run_test(suite, idr_removes_active_apn, NULL);
    abts_run_test(suite, idr_empty_profile, NULL);
    abts_run_test(suite, selected_snapshot_survives_idr, NULL);
    abts_run_test(suite, gn_import_preserves_pdn_type, NULL);
    abts_run_test(suite, apn_selection_is_owned, NULL);
    return suite;
}
