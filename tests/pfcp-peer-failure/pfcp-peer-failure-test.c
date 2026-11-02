/*
 * Copyright (C) 2026 by Daniel Brune <daniel.brune.so@outlook.com>
 *
 * This file is part of Open5GS.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

/*
 * pfcp.release_after_peer_failure
 *
 * pfcp-peer-failure.yaml sets it to 40 seconds on the SMF and to 30 seconds
 * on the UPF. In a deployment the UPF value should not be shorter than
 * the SMF value plus the heartbeat detection skew; here each test case
 * makes only one side unreachable, so the values are chosen to keep the
 * test short. A peer is made unreachable without a restart by stopping
 * its process (SIGSTOP) and resuming it later (SIGCONT).
 *
 * With the default timers, the heartbeat gives up 7.5 to 18.5 seconds
 * after the peer stops answering (no_heartbeat 11 s + 3 x T1 2.5 s).
 * A peer stopped for PEER_DOWN seconds has therefore been detected as
 * failed, and the sessions must still be there: they are released by
 * the SMF no earlier than 7.5 + 40 seconds, and deleted by the UPF no
 * earlier than 7.5 + 30 seconds.
 */

#include "test-common.h"

#include <arpa/inet.h>
#include <poll.h>
#include <signal.h>

#define PEER_DOWN               25      /* seconds */
#define SMF_RELEASE_AFTER       40      /* seconds, pfcp-peer-failure.yaml */
#define UPF_RELEASE_AFTER       30      /* seconds, pfcp-peer-failure.yaml */
#define HEARTBEAT_GIVE_UP_MAX   19      /* seconds, see above */

/* Wait up to msec for an NGAP message. Returns its procedure code or -1 */
static int ngap_wait(test_ue_t *test_ue, ogs_socknode_t *ngap, int msec)
{
    struct pollfd pfd;
    ogs_pkbuf_t *recvbuf = NULL;

    pfd.fd = ngap->sock->fd;
    pfd.events = POLLIN;
    if (poll(&pfd, 1, msec) <= 0)
        return -1;

    recvbuf = testgnb_ngap_read(ngap);
    ogs_assert(recvbuf);
    testngap_recv(test_ue, recvbuf);

    return test_ue->ngap_procedure_code;
}

/* No NGAP message for sec seconds */
static bool ngap_silent(test_ue_t *test_ue, ogs_socknode_t *ngap, int sec)
{
    int code = ngap_wait(test_ue, ngap, sec * 1000);

    if (code != -1) {
        ogs_error("Unexpected NGAP procedure [%d]", code);
        return false;
    }
    return true;
}

/*
 * Send one ICMP echo request over the PDU session and wait up to
 * 2 seconds for the answer. Returns the GTP-U message type of the answer,
 * G-PDU for the echo reply or Error Indication if the UPF has no session,
 * or 0 if nothing came back. Late answers to earlier requests, e.g. queued
 * while the UPF was stopped, are discarded first.
 */
static uint8_t ping_answer(ogs_socknode_t *gtpu, test_bearer_t *qos_flow)
{
    struct pollfd pfd;
    ogs_pkbuf_t *recvbuf = NULL;
    uint8_t type = 0;

    pfd.fd = gtpu->sock->fd;
    pfd.events = POLLIN;
    while (poll(&pfd, 1, 0) > 0) {
        recvbuf = testgnb_gtpu_read(gtpu);
        ogs_assert(recvbuf);
        ogs_pkbuf_free(recvbuf);
    }

    ogs_assert(OGS_OK == test_gtpu_send_ping(gtpu, qos_flow, TEST_PING_IPV4));

    if (poll(&pfd, 1, 2000) <= 0)
        return 0;

    recvbuf = testgnb_gtpu_read(gtpu);
    ogs_assert(recvbuf);
    if (recvbuf->len > 1)
        type = ((uint8_t *)recvbuf->data)[1];
    ogs_pkbuf_free(recvbuf);

    return type;
}

static bool ping(ogs_socknode_t *gtpu, test_bearer_t *qos_flow)
{
    uint8_t type = ping_answer(gtpu, qos_flow);

    if (type != OGS_GTPU_MSGTYPE_GPDU) {
        ogs_error("No echo reply [GTP-U message type %d]", type);
        return false;
    }
    return true;
}

static test_ue_t *ue_add(const char *msin)
{
    ogs_nas_5gs_mobile_identity_suci_t mobile_identity_suci;
    test_ue_t *test_ue = NULL;

    memset(&mobile_identity_suci, 0, sizeof(mobile_identity_suci));

    mobile_identity_suci.h.supi_format = OGS_NAS_5GS_SUPI_FORMAT_IMSI;
    mobile_identity_suci.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;
    mobile_identity_suci.routing_indicator1 = 0;
    mobile_identity_suci.routing_indicator2 = 0xf;
    mobile_identity_suci.routing_indicator3 = 0xf;
    mobile_identity_suci.routing_indicator4 = 0xf;
    mobile_identity_suci.protection_scheme_id = OGS_PROTECTION_SCHEME_NULL;
    mobile_identity_suci.home_network_pki_value = 0;

    test_ue = test_ue_add_by_suci(&mobile_identity_suci, msin);
    ogs_assert(test_ue);

    test_ue->nr_cgi.cell_id = 0x40001;

    test_ue->nas.registration.tsc = 0;
    test_ue->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    test_ue->nas.registration.follow_on_request = 1;
    test_ue->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;

    test_ue->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    test_ue->opc_string = "e8ed289deba952e4283b54e88e6183ca";

    return test_ue;
}

/* NG Setup and registration */
static void ue_register(abts_case *tc, test_ue_t *test_ue,
        ogs_socknode_t *ngap)
{
    int rv;
    ogs_pkbuf_t *gmmbuf;
    ogs_pkbuf_t *nasbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    bson_t *doc = NULL;

    /* Send NG-Setup Reqeust */
    sendbuf = testngap_build_ng_setup_request(0x4000, 22);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive NG-Setup Response */
    recvbuf = testgnb_ngap_read(ngap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);

    /********** Insert Subscriber in Database */
    doc = test_db_new_simple(test_ue);
    ABTS_PTR_NOTNULL(tc, doc);
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_insert_ue(test_ue, doc));

    /* Send Registration request */
    test_ue->registration_request_param.guti = 1;
    gmmbuf = testgmm_build_registration_request(test_ue, NULL, false, false);
    ABTS_PTR_NOTNULL(tc, gmmbuf);

    test_ue->registration_request_param.gmm_capability = 1;
    test_ue->registration_request_param.requested_nssai = 1;
    test_ue->registration_request_param.last_visited_registered_tai = 1;
    test_ue->registration_request_param.ue_usage_setting = 1;
    nasbuf = testgmm_build_registration_request(test_ue, NULL, false, false);
    ABTS_PTR_NOTNULL(tc, nasbuf);

    sendbuf = testngap_build_initial_ue_message(test_ue, gmmbuf,
                NGAP_RRCEstablishmentCause_mo_Signalling, false, true);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Identity request */
    recvbuf = testgnb_ngap_read(ngap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);

    /* Send Identity response */
    gmmbuf = testgmm_build_identity_response(test_ue);
    ABTS_PTR_NOTNULL(tc, gmmbuf);
    sendbuf = testngap_build_uplink_nas_transport(test_ue, gmmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Authentication request */
    recvbuf = testgnb_ngap_read(ngap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);

    /* Send Authentication response */
    gmmbuf = testgmm_build_authentication_response(test_ue);
    ABTS_PTR_NOTNULL(tc, gmmbuf);
    sendbuf = testngap_build_uplink_nas_transport(test_ue, gmmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Security mode command */
    recvbuf = testgnb_ngap_read(ngap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);

    /* Send Security mode complete */
    gmmbuf = testgmm_build_security_mode_complete(test_ue, nasbuf);
    ABTS_PTR_NOTNULL(tc, gmmbuf);
    sendbuf = testngap_build_uplink_nas_transport(test_ue, gmmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive InitialContextSetupRequest +
     * Registration accept */
    recvbuf = testgnb_ngap_read(ngap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);
    ABTS_INT_EQUAL(tc,
            NGAP_ProcedureCode_id_InitialContextSetup,
            test_ue->ngap_procedure_code);

    /* Send UERadioCapabilityInfoIndication */
    sendbuf = testngap_build_ue_radio_capability_info_indication(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send InitialContextSetupResponse */
    sendbuf = testngap_build_initial_context_setup_response(test_ue, false);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send Registration complete */
    gmmbuf = testgmm_build_registration_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, gmmbuf);
    sendbuf = testngap_build_uplink_nas_transport(test_ue, gmmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive Configuration update command */
    recvbuf = testgnb_ngap_read(ngap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);
}

/* PDU session (DNN internet) with the given PSI */
static test_sess_t *pdu_session_establish(abts_case *tc, test_ue_t *test_ue,
        ogs_socknode_t *ngap, uint8_t psi)
{
    int rv;
    ogs_pkbuf_t *gmmbuf;
    ogs_pkbuf_t *gsmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    test_sess_t *sess = NULL;

    /* Send PDU session establishment request */
    sess = test_sess_add_by_dnn_and_psi(test_ue, "internet", psi);
    ogs_assert(sess);

    sess->ul_nas_transport_param.request_type =
        OGS_NAS_5GS_REQUEST_TYPE_INITIAL;
    sess->ul_nas_transport_param.dnn = 1;
    sess->ul_nas_transport_param.s_nssai = 1;

    sess->pdu_session_establishment_param.ssc_mode = 1;
    sess->pdu_session_establishment_param.epco = 1;

    gsmbuf = testgsm_build_pdu_session_establishment_request(sess);
    ABTS_PTR_NOTNULL(tc, gsmbuf);
    gmmbuf = testgmm_build_ul_nas_transport(sess,
            OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
    ABTS_PTR_NOTNULL(tc, gmmbuf);
    sendbuf = testngap_build_uplink_nas_transport(test_ue, gmmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive PDUSessionResourceSetupRequest +
     * DL NAS transport +
     * PDU session establishment accept */
    recvbuf = testgnb_ngap_read(ngap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);
    ABTS_INT_EQUAL(tc,
            NGAP_ProcedureCode_id_PDUSessionResourceSetup,
            test_ue->ngap_procedure_code);

    /* Send PDUSessionResourceSetupResponse */
    sendbuf = testngap_sess_build_pdu_session_resource_setup_response(sess);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Wait for the PFCP session modification */
    ogs_msleep(100);

    return sess;
}

/*
 * NG Setup, registration and one PDU session (DNN internet).
 * Returns the QoS flow of the PDU session.
 */
static test_bearer_t *ue_attach(abts_case *tc, test_ue_t *test_ue,
        ogs_socknode_t *ngap, ogs_socknode_t *gtpu)
{
    test_sess_t *sess = NULL;
    test_bearer_t *qos_flow = NULL;

    ue_register(tc, test_ue, ngap);
    sess = pdu_session_establish(tc, test_ue, ngap, 5);

    qos_flow = test_qos_flow_find_by_qfi(sess, 1);
    ogs_assert(qos_flow);

    ABTS_TRUE(tc, ping(gtpu, qos_flow));

    return qos_flow;
}

/*
 * Answer the PDU Session Resource Release Command of a PDU session
 * released by the network.
 */
static void pdu_session_release_complete(abts_case *tc, test_ue_t *test_ue,
        ogs_socknode_t *ngap, test_sess_t *sess)
{
    int rv;
    ogs_pkbuf_t *gmmbuf;
    ogs_pkbuf_t *gsmbuf;
    ogs_pkbuf_t *sendbuf;

    /* Send PDUSessionResourceReleaseResponse */
    sendbuf = testngap_build_pdu_session_resource_release_response(sess);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Send UplinkNASTransport +
     * UL NAS trasnport +
     * PDU session resource release complete */
    sess->ul_nas_transport_param.request_type = 0;
    sess->ul_nas_transport_param.dnn = 0;
    sess->ul_nas_transport_param.s_nssai = 0;

    sess->pdu_session_establishment_param.ssc_mode = 0;
    sess->pdu_session_establishment_param.epco = 0;

    gsmbuf = testgsm_build_pdu_session_release_complete(sess);
    ABTS_PTR_NOTNULL(tc, gsmbuf);
    gmmbuf = testgmm_build_ul_nas_transport(sess,
            OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsmbuf);
    ABTS_PTR_NOTNULL(tc, gmmbuf);
    sendbuf = testngap_build_uplink_nas_transport(test_ue, gmmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
}

static void ue_detach(abts_case *tc, test_ue_t *test_ue,
        ogs_socknode_t *ngap, ogs_socknode_t *gtpu)
{
    int rv;
    ogs_pkbuf_t *gmmbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;

    /* Send De-registration request */
    gmmbuf = testgmm_build_de_registration_request(test_ue, 1, true, false);
    ABTS_PTR_NOTNULL(tc, gmmbuf);
    sendbuf = testngap_build_uplink_nas_transport(test_ue, gmmbuf);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    /* Receive UEContextReleaseCommand */
    recvbuf = testgnb_ngap_read(ngap);
    ABTS_PTR_NOTNULL(tc, recvbuf);
    testngap_recv(test_ue, recvbuf);
    ABTS_INT_EQUAL(tc,
            NGAP_ProcedureCode_id_UEContextRelease,
            test_ue->ngap_procedure_code);

    /* Send UEContextReleaseComplete */
    sendbuf = testngap_build_ue_context_release_complete(test_ue);
    ABTS_PTR_NOTNULL(tc, sendbuf);
    rv = testgnb_ngap_send(ngap, sendbuf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);

    ogs_msleep(300);

    /********** Remove Subscriber in Database */
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue));

    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu);

    /* gNB disonncect from AMF */
    testgnb_ngap_close(ngap);

    /* Clear Test UE Context */
    test_ue_remove(test_ue);
}

/*
 * The UPF is unreachable for PEER_DOWN seconds and comes back before
 * release_after_peer_failure expires on the SMF: the PDU session is
 * kept, and it is not released later either.
 */
static void test1_func(abts_case *tc, void *data)
{
    ogs_socknode_t *ngap;
    ogs_socknode_t *gtpu;
    test_ue_t *test_ue = NULL;
    test_bearer_t *qos_flow = NULL;

    test_ue = ue_add("0000203190");

    ngap = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap);
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    qos_flow = ue_attach(tc, test_ue, ngap, gtpu);

    /* UPF unreachable: the SMF must not release the PDU session */
    test_child_signal_with_name("upf", 0, SIGSTOP);
    ABTS_TRUE(tc, ngap_silent(test_ue, ngap, PEER_DOWN));
    test_child_signal_with_name("upf", 0, SIGCONT);

    /*
     * The SMF associates again within the association interval and stops
     * the timer: still no release after it would have expired.
     */
    ABTS_TRUE(tc, ngap_silent(test_ue, ngap, SMF_RELEASE_AFTER));
    ABTS_TRUE(tc, ping(gtpu, qos_flow));

    ue_detach(tc, test_ue, ngap, gtpu);
}

/*
 * The UPF stays unreachable: the SMF keeps the PDU session until
 * release_after_peer_failure expires, then releases it.
 */
static void test2_func(abts_case *tc, void *data)
{
    ogs_socknode_t *ngap;
    ogs_socknode_t *gtpu;
    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;

    test_ue = ue_add("0000203191");

    ngap = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap);
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    ue_attach(tc, test_ue, ngap, gtpu);
    sess = test_sess_find_by_psi(test_ue, 5);
    ogs_assert(sess);

    test_child_signal_with_name("upf", 0, SIGSTOP);

    /* Kept after the heartbeat has given up */
    ABTS_TRUE(tc, ngap_silent(test_ue, ngap, PEER_DOWN));

    /* Receive PDUSessionResourceReleaseCommand +
     * DL NAS transport +
     * PDU session release command */
    ABTS_INT_EQUAL(tc,
            NGAP_ProcedureCode_id_PDUSessionResourceRelease,
            ngap_wait(test_ue, ngap, (SMF_RELEASE_AFTER + 15) * 1000));

    pdu_session_release_complete(tc, test_ue, ngap, sess);

    test_child_signal_with_name("upf", 0, SIGCONT);

    /* Let the SMF associate with the UPF again (association interval 11 s) */
    ogs_msleep(25000);

    ue_detach(tc, test_ue, ngap, gtpu);
}

/*
 * Two UPFs: the UPF of the PDU session is unreachable and the other UPF
 * is associated. The SMF does not wait for release_after_peer_failure:
 * it releases the PDU session ("reactivation requested") as soon as the
 * heartbeat has given up, and the new PDU session goes to the other UPF.
 *
 * pfcp-peer-failure.yaml selects the UPF by DNN (127.0.0.7) or NR cell
 * (127.0.0.17) without round robin: the PDU sessions of the other test
 * cases (cell 0x40001) have no other UPF and are kept, the one of this
 * test case (cell 0x40002) can be served by both.
 */
static void test3_func(abts_case *tc, void *data)
{
    ogs_socknode_t *ngap;
    ogs_socknode_t *gtpu;
    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    uint32_t upf_addr[2];
    int down, up;

    ogs_assert(inet_pton(AF_INET, "127.0.0.7", &upf_addr[0]) == 1);
    ogs_assert(inet_pton(AF_INET, "127.0.0.17", &upf_addr[1]) == 1);

    test_ue = ue_add("0000203193");
    test_ue->nr_cgi.cell_id = 0x40002;

    ngap = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap);
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    ue_register(tc, test_ue, ngap);
    sess = pdu_session_establish(tc, test_ue, ngap, 5);

    /* The UPF instance of the PDU session goes down, the other stays up */
    ABTS_TRUE(tc, sess->upf_n3_ip.ipv4);
    down = sess->upf_n3_ip.addr == upf_addr[0] ? 0 : 1;
    up = 1 - down;
    ABTS_INT_EQUAL(tc, upf_addr[down], sess->upf_n3_ip.addr);

    test_child_signal_with_name("upf", down, SIGSTOP);

    /* Receive PDUSessionResourceReleaseCommand +
     * DL NAS transport +
     * PDU session release command
     * once the heartbeat has given up, not after the timer */
    ABTS_INT_EQUAL(tc,
            NGAP_ProcedureCode_id_PDUSessionResourceRelease,
            ngap_wait(test_ue, ngap, (HEARTBEAT_GIVE_UP_MAX + 3) * 1000));

    pdu_session_release_complete(tc, test_ue, ngap, sess);

    /* The new PDU session goes to the other UPF */
    sess = pdu_session_establish(tc, test_ue, ngap, 6);
    ABTS_INT_EQUAL(tc, upf_addr[up], sess->upf_n3_ip.addr);

    test_child_signal_with_name("upf", down, SIGCONT);

    /* Let the SMF associate with the UPF again (association interval 11 s) */
    ogs_msleep(25000);

    ue_detach(tc, test_ue, ngap, gtpu);
}

/*
 * The SMF is unreachable: the UPF keeps the sessions of the SMF after the
 * heartbeat has given up, so the data path keeps working, and deletes them
 * when release_after_peer_failure expires (TS 23.527 4.4.3.1).
 *
 * This is the last test case: the SMF never noticed the outage and stays
 * associated, so the subscriber is only removed from the database.
 */
static void test4_func(abts_case *tc, void *data)
{
    ogs_time_t start, now;
    ogs_socknode_t *ngap;
    ogs_socknode_t *gtpu;
    test_ue_t *test_ue = NULL;
    test_bearer_t *qos_flow = NULL;
    uint8_t type = 0;

    test_ue = ue_add("0000203192");

    ngap = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap);
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    qos_flow = ue_attach(tc, test_ue, ngap, gtpu);

    test_child_signal_with_name("smf", 0, SIGSTOP);
    start = ogs_get_monotonic_time();

    /* Kept after the heartbeat has given up */
    do {
        ABTS_TRUE(tc, ping(gtpu, qos_flow));
        ogs_msleep(1000);
        now = ogs_get_monotonic_time();
    } while (now - start < ogs_time_from_sec(PEER_DOWN));

    /* Deleted when the timer expires */
    do {
        type = ping_answer(gtpu, qos_flow);
        if (type == OGS_GTPU_MSGTYPE_ERR_IND)
            break;
        ABTS_INT_EQUAL(tc, OGS_GTPU_MSGTYPE_GPDU, type);
        ogs_msleep(1000);
        now = ogs_get_monotonic_time();
    } while (now - start < ogs_time_from_sec(
                HEARTBEAT_GIVE_UP_MAX + UPF_RELEASE_AFTER + 10));
    ABTS_INT_EQUAL(tc, OGS_GTPU_MSGTYPE_ERR_IND, type);

    test_child_signal_with_name("smf", 0, SIGCONT);

    /********** Remove Subscriber in Database */
    ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(test_ue));

    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu);

    /* gNB disonncect from AMF */
    testgnb_ngap_close(ngap);

    /* Clear Test UE Context */
    test_ue_remove(test_ue);
}

abts_suite *test_pfcp_peer_failure(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, test1_func, NULL);
    abts_run_test(suite, test2_func, NULL);
    abts_run_test(suite, test3_func, NULL);
    abts_run_test(suite, test4_func, NULL);

    return suite;
}
