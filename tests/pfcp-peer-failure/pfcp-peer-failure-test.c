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
 *
 * A stopped process loses no PFCP message: the messages sent to it are
 * queued and handled when it resumes. Whether the SMF has restored the
 * sessions on the UPF (TS 23.527 4.3.2) is read from the metrics server
 * of the UPF, which counts the PFCP Session Establishment Requests.
 */

#include "test-common.h"

#include <arpa/inet.h>
#include <poll.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define PEER_DOWN               25      /* seconds */
#define SMF_RELEASE_AFTER       40      /* seconds, pfcp-peer-failure.yaml */
#define UPF_RELEASE_AFTER       30      /* seconds, pfcp-peer-failure.yaml */
#define HEARTBEAT_GIVE_UP_MIN   7       /* seconds, see above */
#define HEARTBEAT_GIVE_UP_MAX   19      /* seconds, see above */

/*
 * Read the next NGAP message and return its procedure code.
 *
 * The NGAP socket of the test gNB cannot be polled: with usrsctp (macOS)
 * it is not a file descriptor. The read waits as long as it takes, so it
 * is only called where a message is expected. A message that must not
 * come while the test case waits is caught by the next read, which
 * expects another one.
 */
static int ngap_next(test_ue_t *test_ue, ogs_socknode_t *ngap)
{
    ogs_pkbuf_t *recvbuf = NULL;

    recvbuf = testgnb_ngap_read(ngap);
    ogs_assert(recvbuf);
    testngap_recv(test_ue, recvbuf);

    return test_ue->ngap_procedure_code;
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

/*
 * Number of PFCP Session Establishment Requests received by the UPF
 * (metrics server of the UPF, port 9090 in pfcp-peer-failure.yaml), or -1
 * if it cannot be read. Not to be called while the UPF is stopped.
 */
#define UPF_ESTABLISHMENT_REQUESTS "fivegs_upffunction_sm_n4sessionestabreq"

static long upf_establishment_requests(const char *upf_addr)
{
    static char buf[256 * 1024];
    static const char request[] = "GET /metrics HTTP/1.0\r\n\r\n";
    const char *name = "\n" UPF_ESTABLISHMENT_REQUESTS " ";
    struct sockaddr_in sin;
    struct timeval tv;
    size_t len = 0;
    ssize_t n;
    char *p;
    int fd;

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9090);
    ogs_assert(inet_pton(AF_INET, upf_addr, &sin.sin_addr) == 1);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    ogs_assert(fd >= 0);
    tv.tv_sec = 2;
    tv.tv_usec = 0;
    ogs_assert(setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == 0);

    if (connect(fd, (struct sockaddr *)&sin, sizeof(sin)) != 0 ||
        send(fd, request, sizeof(request) - 1, 0) !=
            (ssize_t)(sizeof(request) - 1)) {
        ogs_error("Cannot read the metrics of UPF %s", upf_addr);
        close(fd);
        return -1;
    }
    while (len < sizeof(buf) - 1 &&
            (n = recv(fd, buf + len, sizeof(buf) - 1 - len, 0)) > 0)
        len += (size_t)n;
    close(fd);
    buf[len] = '\0';

    p = strstr(buf, name);
    if (!p) {
        ogs_error("No %s in the metrics of UPF %s",
                UPF_ESTABLISHMENT_REQUESTS, upf_addr);
        return -1;
    }
    return (long)strtod(p + strlen(name), NULL);
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

/* NG Setup (first UE of the gNB) and registration */
static void ue_register(abts_case *tc, test_ue_t *test_ue,
        ogs_socknode_t *ngap, bool ng_setup)
{
    int rv;
    ogs_pkbuf_t *gmmbuf;
    ogs_pkbuf_t *nasbuf;
    ogs_pkbuf_t *sendbuf;
    ogs_pkbuf_t *recvbuf;
    bson_t *doc = NULL;

    if (ng_setup == true) {
        /* Send NG-Setup Reqeust */
        sendbuf = testngap_build_ng_setup_request(0x4000, 22);
        ABTS_PTR_NOTNULL(tc, sendbuf);
        rv = testgnb_ngap_send(ngap, sendbuf);
        ABTS_INT_EQUAL(tc, OGS_OK, rv);

        /* Receive NG-Setup Response */
        recvbuf = testgnb_ngap_read(ngap);
        ABTS_PTR_NOTNULL(tc, recvbuf);
        testngap_recv(test_ue, recvbuf);
    }

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

    ue_register(tc, test_ue, ngap, true);
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

static void ue_deregister(abts_case *tc, test_ue_t *test_ue,
        ogs_socknode_t *ngap)
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

    /* Clear Test UE Context */
    test_ue_remove(test_ue);
}

static void ue_detach(abts_case *tc, test_ue_t *test_ue,
        ogs_socknode_t *ngap, ogs_socknode_t *gtpu)
{
    ue_deregister(tc, test_ue, ngap);

    /* gNB disonncect from UPF */
    testgnb_gtpu_close(gtpu);

    /* gNB disonncect from AMF */
    testgnb_ngap_close(ngap);
}

/*
 * The UPF is unreachable for PEER_DOWN seconds and comes back before
 * release_after_peer_failure expires on the SMF: the PDU session is
 * kept, and it is not released later either. The SMF asks the UPF to
 * retain it (PFCP Session Retention Information) and does not restore
 * it: the UPF receives no PFCP Session Establishment Request.
 */
static void test1_func(abts_case *tc, void *data)
{
    ogs_socknode_t *ngap;
    ogs_socknode_t *gtpu;
    test_ue_t *test_ue = NULL;
    test_bearer_t *qos_flow = NULL;
    long requests;

    test_ue = ue_add("0000203190");

    ngap = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap);
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    qos_flow = ue_attach(tc, test_ue, ngap, gtpu);

    requests = upf_establishment_requests("127.0.0.7");
    ABTS_TRUE(tc, requests > 0);

    /* UPF unreachable: the SMF must not release the PDU session */
    test_child_signal_with_name("upf", 0, SIGSTOP);
    ogs_msleep(PEER_DOWN * 1000);
    test_child_signal_with_name("upf", 0, SIGCONT);

    /*
     * The SMF associates again within the association interval and stops
     * the timer: still no release after it would have expired. A released
     * session would make ping() fail, and the release would be read by
     * ue_detach() instead of the UE Context Release.
     */
    ogs_msleep(SMF_RELEASE_AFTER * 1000);
    ABTS_TRUE(tc, ping(gtpu, qos_flow));

    /* Retained by the UPF, not restored by the SMF */
    ABTS_INT_EQUAL(tc, (int)requests,
            (int)upf_establishment_requests("127.0.0.7"));

    ue_detach(tc, test_ue, ngap, gtpu);
}

/*
 * The UPF stays unreachable: the SMF keeps the PDU session until
 * release_after_peer_failure expires, then releases it.
 *
 * The PFCP Session Deletion cannot reach the UPF at that time. When the
 * SMF associates again, it no longer asks the UPF to retain the sessions
 * (PFCP Session Retention Information), so the UPF deletes the session
 * left over from the released PDU session.
 */
static void test2_func(abts_case *tc, void *data)
{
    ogs_socknode_t *ngap;
    ogs_socknode_t *gtpu;
    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    test_bearer_t *qos_flow = NULL;
    ogs_time_t start, elapsed;

    test_ue = ue_add("0000203191");

    ngap = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap);
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    qos_flow = ue_attach(tc, test_ue, ngap, gtpu);
    sess = test_sess_find_by_psi(test_ue, 5);
    ogs_assert(sess);

    test_child_signal_with_name("upf", 0, SIGSTOP);
    start = ogs_get_monotonic_time();

    /* Receive PDUSessionResourceReleaseCommand +
     * DL NAS transport +
     * PDU session release command
     * when the timer expires: kept after the heartbeat has given up */
    ABTS_INT_EQUAL(tc,
            NGAP_ProcedureCode_id_PDUSessionResourceRelease,
            ngap_next(test_ue, ngap));
    elapsed = ogs_get_monotonic_time() - start;
    ABTS_TRUE(tc, elapsed >= ogs_time_from_sec(
                HEARTBEAT_GIVE_UP_MIN + SMF_RELEASE_AFTER));
    ABTS_TRUE(tc, elapsed <= ogs_time_from_sec(
                HEARTBEAT_GIVE_UP_MAX + SMF_RELEASE_AFTER + 15));

    pdu_session_release_complete(tc, test_ue, ngap, sess);

    test_child_signal_with_name("upf", 0, SIGCONT);

    /* Let the SMF associate with the UPF again (association interval 11 s) */
    ogs_msleep(25000);

    /* No session left over in the UPF */
    ABTS_INT_EQUAL(tc, OGS_GTPU_MSGTYPE_ERR_IND, ping_answer(gtpu, qos_flow));

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
    ogs_time_t start;

    ogs_assert(inet_pton(AF_INET, "127.0.0.7", &upf_addr[0]) == 1);
    ogs_assert(inet_pton(AF_INET, "127.0.0.17", &upf_addr[1]) == 1);

    test_ue = ue_add("0000203193");
    test_ue->nr_cgi.cell_id = 0x40002;

    ngap = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap);
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    ue_register(tc, test_ue, ngap, true);
    sess = pdu_session_establish(tc, test_ue, ngap, 5);

    /* The UPF instance of the PDU session goes down, the other stays up */
    ABTS_TRUE(tc, sess->upf_n3_ip.ipv4);
    down = sess->upf_n3_ip.addr == upf_addr[0] ? 0 : 1;
    up = 1 - down;
    ABTS_INT_EQUAL(tc, upf_addr[down], sess->upf_n3_ip.addr);

    test_child_signal_with_name("upf", down, SIGSTOP);
    start = ogs_get_monotonic_time();

    /* Receive PDUSessionResourceReleaseCommand +
     * DL NAS transport +
     * PDU session release command
     * once the heartbeat has given up, not after the timer */
    ABTS_INT_EQUAL(tc,
            NGAP_ProcedureCode_id_PDUSessionResourceRelease,
            ngap_next(test_ue, ngap));
    ABTS_TRUE(tc, ogs_get_monotonic_time() - start <=
            ogs_time_from_sec(HEARTBEAT_GIVE_UP_MAX + 3));

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
 * Two PDU sessions on the UPF, and the UPF is unreachable: the SMF keeps
 * the one of the first UE (cell 0x40001, no other UPF) and releases the
 * one of the second UE (cell 0x40002, see test3_func) at once.
 *
 * The PFCP Session Deletion of the released session may not have reached
 * the UPF, so the SMF does not ask the UPF to retain the sessions when it
 * associates again: the UPF deletes both, and the SMF restores the kept
 * one (TS 23.527 4.3.2). The data path of the kept session works again,
 * and the UPF has no session left over from the released one.
 */
static void test4_func(abts_case *tc, void *data)
{
    ogs_socknode_t *ngap;
    ogs_socknode_t *gtpu;
    test_ue_t *test_ue = NULL, *test_ue2 = NULL;
    test_sess_t *sess2 = NULL;
    test_bearer_t *qos_flow = NULL, *qos_flow2 = NULL;
    uint32_t upf_addr;
    uint8_t psi;
    long requests;
    ogs_time_t start;

    ogs_assert(inet_pton(AF_INET, "127.0.0.7", &upf_addr) == 1);

    test_ue = ue_add("0000203194");
    test_ue2 = ue_add("0000203195");
    test_ue2->nr_cgi.cell_id = 0x40002;

    ngap = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap);
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    /* First UE: PDU session on 127.0.0.7 */
    qos_flow = ue_attach(tc, test_ue, ngap, gtpu);

    /*
     * Second UE: PDU session on 127.0.0.7 as well. The SMF selects the
     * two UPFs in turn for cell 0x40002.
     */
    test_ue2->ran_ue_ngap_id = test_ue->ran_ue_ngap_id;
    ue_register(tc, test_ue2, ngap, false);
    for (psi = 5; psi <= 6; psi++) {
        sess2 = pdu_session_establish(tc, test_ue2, ngap, psi);
        if (sess2->upf_n3_ip.addr == upf_addr)
            break;
    }
    ABTS_INT_EQUAL(tc, upf_addr, sess2->upf_n3_ip.addr);
    qos_flow2 = test_qos_flow_find_by_qfi(sess2, 1);
    ogs_assert(qos_flow2);
    ABTS_TRUE(tc, ping(gtpu, qos_flow2));

    requests = upf_establishment_requests("127.0.0.7");
    ABTS_TRUE(tc, requests > 0);

    test_child_signal_with_name("upf", 0, SIGSTOP);
    start = ogs_get_monotonic_time();

    /* Receive PDUSessionResourceReleaseCommand +
     * DL NAS transport +
     * PDU session release command
     * of the second UE once the heartbeat has given up */
    ABTS_INT_EQUAL(tc,
            NGAP_ProcedureCode_id_PDUSessionResourceRelease,
            ngap_next(test_ue2, ngap));
    ABTS_TRUE(tc, ogs_get_monotonic_time() - start <=
            ogs_time_from_sec(HEARTBEAT_GIVE_UP_MAX + 3));

    pdu_session_release_complete(tc, test_ue2, ngap, sess2);

    /* Let the SMF remove the released session */
    ogs_msleep(3000);

    test_child_signal_with_name("upf", 0, SIGCONT);

    /*
     * The SMF associates again within the association interval (11 s),
     * and the session of the first UE is kept: checked below by the
     * PFCP Session Establishment Requests and ping().
     */
    ogs_msleep(25000);

    /* Deleted by the UPF and restored by the SMF */
    ABTS_INT_EQUAL(tc, (int)(requests + 1),
            (int)upf_establishment_requests("127.0.0.7"));
    ABTS_TRUE(tc, ping(gtpu, qos_flow));

    /* No session left over in the UPF */
    ABTS_INT_EQUAL(tc,
            OGS_GTPU_MSGTYPE_ERR_IND, ping_answer(gtpu, qos_flow2));

    ue_deregister(tc, test_ue2, ngap);
    ue_detach(tc, test_ue, ngap, gtpu);
}

/*
 * As in test2_func, but the PDU session is still being released when the
 * UPF comes back: release_after_peer_failure has expired, and the UE has
 * not answered the PDU Session Release Command yet. A session released on
 * expiry is neither retained nor restored, also while its release is
 * still in progress: the UPF deletes it when the SMF associates again,
 * and the SMF does not set it up again.
 */
static void test6_func(abts_case *tc, void *data)
{
    ogs_socknode_t *ngap;
    ogs_socknode_t *gtpu;
    test_ue_t *test_ue = NULL;
    test_sess_t *sess = NULL;
    test_bearer_t *qos_flow = NULL;
    long requests;
    ogs_time_t start, elapsed;

    test_ue = ue_add("0000203196");

    ngap = testngap_client(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap);
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, gtpu);

    qos_flow = ue_attach(tc, test_ue, ngap, gtpu);
    sess = test_sess_find_by_psi(test_ue, 5);
    ogs_assert(sess);

    requests = upf_establishment_requests("127.0.0.7");
    ABTS_TRUE(tc, requests > 0);

    test_child_signal_with_name("upf", 0, SIGSTOP);
    start = ogs_get_monotonic_time();

    /* Receive PDUSessionResourceReleaseCommand +
     * DL NAS transport +
     * PDU session release command
     * when the timer expires: kept after the heartbeat has given up */
    ABTS_INT_EQUAL(tc,
            NGAP_ProcedureCode_id_PDUSessionResourceRelease,
            ngap_next(test_ue, ngap));
    elapsed = ogs_get_monotonic_time() - start;
    ABTS_TRUE(tc, elapsed >= ogs_time_from_sec(
                HEARTBEAT_GIVE_UP_MIN + SMF_RELEASE_AFTER));
    ABTS_TRUE(tc, elapsed <= ogs_time_from_sec(
                HEARTBEAT_GIVE_UP_MAX + SMF_RELEASE_AFTER + 15));

    test_child_signal_with_name("upf", 0, SIGCONT);

    /* Let the SMF associate with the UPF again (association interval 11 s) */
    ogs_msleep(25000);

    /* Not retained by the UPF */
    ABTS_INT_EQUAL(tc, OGS_GTPU_MSGTYPE_ERR_IND, ping_answer(gtpu, qos_flow));

    pdu_session_release_complete(tc, test_ue, ngap, sess);
    ogs_msleep(1000);

    /* Not restored by the SMF */
    ABTS_INT_EQUAL(tc, OGS_GTPU_MSGTYPE_ERR_IND, ping_answer(gtpu, qos_flow));
    ABTS_INT_EQUAL(tc, (int)requests,
            (int)upf_establishment_requests("127.0.0.7"));

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
static void test5_func(abts_case *tc, void *data)
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
    abts_run_test(suite, test6_func, NULL);
    abts_run_test(suite, test5_func, NULL);

    return suite;
}
