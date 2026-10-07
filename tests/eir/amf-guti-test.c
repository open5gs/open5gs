/* Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
 * SPDX-License-Identifier: AGPL-3.0-or-later */

#include "integration.h"
#include <poll.h>
#include <sys/time.h>

#define AMF_GUTI_TEST_PEI "imeisv-8665070400405307"

typedef enum {
    BAD_RES,
    AUTH_FAILURE,
    SMC_REJECT,
    DEREG_DURING_AUTH,
    DEREG_DURING_SMC,
    REREGISTER,
    EIR_REJECT,
} scenario_t;

static const struct {
    scenario_t scenario;
    unsigned int sessions;
} cases[] = {
    { BAD_RES, 1 },
    { AUTH_FAILURE, 1 },
    { SMC_REJECT, 1 },
    { DEREG_DURING_AUTH, 1 },
    { DEREG_DURING_SMC, 1 },
    { REREGISTER, 1 },
    { EIR_REJECT, 1 },
    { AUTH_FAILURE, 2 },
    { REREGISTER, 2 },
};

static bool send_ngap(abts_case *tc, ogs_socknode_t *ngap, ogs_pkbuf_t *buf)
{
    int rv;

    ABTS_PTR_NOTNULL(tc, buf);
    if (!buf)
        return false;
    rv = testgnb_ngap_send(ngap, buf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    return rv == OGS_OK;
}

static bool send_nas(abts_case *tc, ogs_socknode_t *ngap,
        test_ue_t *ue, ogs_pkbuf_t *buf)
{
    ABTS_PTR_NOTNULL(tc, buf);
    if (!buf)
        return false;
    return send_ngap(tc, ngap, testngap_build_uplink_nas_transport(ue, buf));
}

static bool send_plain_deregistration(abts_case *tc,
        ogs_socknode_t *ngap, test_ue_t *ue)
{
    ogs_nas_5gs_message_t message;
    ogs_nas_5gs_mobile_identity_guti_t *identity;
    ogs_pkbuf_t *buf, *copy;
    bool valid;
    int rv;

    buf = testgmm_build_de_registration_request(ue, true, false, false);
    ABTS_PTR_NOTNULL(tc, buf);
    if (!buf)
        return false;
    copy = ogs_pkbuf_copy(buf);
    ABTS_PTR_NOTNULL(tc, copy);
    if (!copy) {
        ogs_pkbuf_free(buf);
        return false;
    }
    memset(&message, 0, sizeof(message));
    rv = ogs_nas_5gmm_decode(&message, copy);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    identity = message.gmm.deregistration_request_from_ue.mobile_identity.buffer;
    /* Ensure this stimulus reaches the AMF's guard instead of failing decode. */
    valid = rv == OGS_OK &&
        message.gmm.h.security_header_type == OGS_NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE &&
        message.gmm.h.extended_protocol_discriminator ==
            OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM &&
        message.gmm.h.message_type == OGS_NAS_5GS_DEREGISTRATION_REQUEST_FROM_UE &&
        message.gmm.deregistration_request_from_ue.de_registration_type.switch_off == 1 &&
        message.gmm.deregistration_request_from_ue.mobile_identity.length ==
            sizeof(*identity) && identity &&
        identity->h.type == OGS_NAS_5GS_MOBILE_IDENTITY_GUTI &&
        be32toh(identity->m_tmsi) == ue->nas_5gs_guti.m_tmsi;
    ABTS_TRUE(tc, valid);
    ogs_pkbuf_free(copy);
    if (!valid) {
        ogs_pkbuf_free(buf);
        return false;
    }
    return send_nas(tc, ngap, ue, buf);
}

static bool receive_ngap(abts_case *tc, ogs_socknode_t *ngap,
        test_ue_t *ue, long procedure, int gmm, int gsm)
{
#if !HAVE_USRSCTP
    struct pollfd fd = { ngap->sock->fd, POLLIN, 0 };
    int rv;
#endif
    ogs_time_t deadline = ogs_time_now() + ogs_time_from_sec(5);
    ogs_pkbuf_t *buf;

    while (ogs_time_now() < deadline) {
        uint64_t ran_id = ue->ran_ue_ngap_id;
        uint64_t amf_id = ue->amf_ue_ngap_id;
#if !HAVE_USRSCTP
        ogs_time_t remaining = deadline - ogs_time_now();
        if (remaining <= 0)
            break;
        rv = poll(&fd, 1, ogs_time_to_msec(remaining) + 1);
        if (rv < 0 && errno == EINTR)
            continue;
        if (rv < 0 || (rv > 0 &&
            (fd.revents & (POLLERR | POLLHUP | POLLNVAL)))) {
            ABTS_FAIL(tc, "Cannot poll the NGAP socket");
            return false;
        }
        if (rv <= 0 || !(fd.revents & POLLIN))
            break;
#endif
        /* usrsctp has no native fd; Meson's timeout bounds its SCTP reader. */
        buf = testgnb_ngap_read(ngap);
        ABTS_PTR_NOTNULL(tc, buf);
        if (!buf)
            return false;
        ue->gmm_message_type = ue->gsm_message_type = 0;
        testngap_recv(ue, buf);
        /* The held RAN context can be released before or after the next NAS
         * message. Acknowledge it without replacing the current RAN IDs. */
        if (procedure != NGAP_ProcedureCode_id_UEContextRelease && ran_id &&
            ue->ngap_procedure_code == NGAP_ProcedureCode_id_UEContextRelease &&
            ue->ran_ue_ngap_id != ran_id) {
            bool sent = send_ngap(tc, ngap,
                    testngap_build_ue_context_release_complete(ue));
            ue->ran_ue_ngap_id = ran_id;
            ue->amf_ue_ngap_id = amf_id;
            if (!sent)
                return false;
            continue;
        }
        ABTS_INT_EQUAL(tc, procedure, ue->ngap_procedure_code);
        if (gmm)
            ABTS_INT_EQUAL(tc, gmm, ue->gmm_message_type);
        if (gsm)
            ABTS_INT_EQUAL(tc, gsm, ue->gsm_message_type);
        return ue->ngap_procedure_code == procedure &&
            (!gmm || ue->gmm_message_type == gmm) &&
            (!gsm || ue->gsm_message_type == gsm);
    }
    ABTS_FAIL(tc, "Timed out waiting for the expected NGAP message");
    return false;
}

static bool matching_qfi(ogs_pkbuf_t *buf, int header_len, uint8_t qfi)
{
    ogs_gtp2_header_t *header = (ogs_gtp2_header_t *)buf->data;
    int offset = OGS_GTPV1U_HEADER_LEN + OGS_GTPV1U_EXTENSION_HEADER_LEN;
    uint8_t type;
    bool matched = false;

    if (!(header->flags & OGS_GTPU_FLAGS_E) || header_len < offset + 4)
        return false;
    type = buf->data[offset - 1];
    while (type) {
        uint8_t *ext = buf->data + offset;
        int len;

        if (offset + 4 > header_len)
            return false;
        len = ext[0] * 4;
        if (len < 4 || offset + len > header_len)
            return false;
        if (type == OGS_GTP2_EXTENSION_HEADER_TYPE_PDU_SESSION_CONTAINER) {
            if (matched || (ext[1] >> 4) !=
                OGS_GTP2_EXTENSION_HEADER_PDU_TYPE_DL_PDU_SESSION_INFORMATION ||
                (ext[2] & 0x3f) != qfi)
                return false;
            matched = true;
        }
        type = ext[len - 1];
        offset += len;
    }
    return matched;
}

/* Match a fresh Echo Reply on the expected N3 tunnel and QFI. In the unfixed
 * AMF, Authentication Request follows old-session deletion, so the original
 * tunnel probe at that point fails without waiting for an AKA timeout. */

static bool check_bearer(abts_case *tc, ogs_socknode_t *gtpu,
        test_bearer_t *bearer, bool alive)
{
    static uint16_t sequence;
    ogs_gtp2_header_desc_t desc;
    ogs_pkbuf_t *buf;
    struct pollfd fd = { gtpu->sock->fd, POLLIN, 0 };
    uint16_t packet_words[18] = { 0 };
    uint8_t *packet = (uint8_t *)packet_words;
    uint16_t value, seq = ++sequence;
    uint32_t dst;
    int round;
    bool reply = false;

    ogs_assert(inet_pton(AF_INET, TEST_PING_IPV4, &dst) == 1);
    packet[0] = 0x45;
    packet[2] = 0; packet[3] = sizeof(packet_words);
    packet[8] = 64; packet[9] = IPPROTO_ICMP;
    memcpy(packet + 12, &bearer->sess->ue_ip.addr, 4);
    memcpy(packet + 16, &dst, 4);
    packet[20] = 8; /* Echo Request */
    packet[24] = 0x45; packet[25] = 0x77;
    packet[26] = seq >> 8; packet[27] = seq;
    memcpy(packet + 28, "guti-amf", 8);
    value = ogs_in_cksum((uint16_t *)(packet + 20), 16);
    memcpy(packet + 22, &value, 2);
    value = ogs_in_cksum((uint16_t *)packet, 20);
    memcpy(packet + 10, &value, 2);

    memset(&desc, 0, sizeof(desc));
    desc.type = OGS_GTPU_MSGTYPE_GPDU;
    desc.teid = bearer->sess->upf_n3_teid;
    desc.pdu_type =
        OGS_GTP2_EXTENSION_HEADER_PDU_TYPE_UL_PDU_SESSION_INFORMATION;
    desc.qos_flow_identifier = bearer->qfi;
    for (round = 0; round < (alive ? 3 : 1) && !reply; round++) {
        ogs_time_t deadline = ogs_time_now() + ogs_time_from_sec(1);

        buf = ogs_pkbuf_alloc(NULL, 128);
        ogs_assert(buf);
        ogs_pkbuf_reserve(buf, OGS_GTPV1U_5GC_HEADER_LEN);
        ogs_pkbuf_put_data(buf, packet, sizeof(packet_words));
        ABTS_INT_EQUAL(tc, OGS_OK, test_gtpu_send(gtpu, bearer, &desc, buf));
        while (ogs_time_now() < deadline) {
            ogs_gtp2_header_t *gtp;
            ogs_gtp2_header_desc_t received;
            uint8_t *ip;
            int rv, header_len, ip_len;
            ogs_time_t remaining = deadline - ogs_time_now();

            if (remaining <= 0)
                break;
            rv = poll(&fd, 1, ogs_time_to_msec(remaining) + 1);
            if (rv < 0 && errno == EINTR)
                continue;
            if (rv < 0 || (rv > 0 &&
                (fd.revents & (POLLERR | POLLHUP | POLLNVAL)))) {
                ABTS_FAIL(tc, "Cannot poll the GTP-U socket");
                return false;
            }
            if (rv <= 0 || !(fd.revents & POLLIN))
                break;
            buf = test_gtpu_read(gtpu);
            ABTS_PTR_NOTNULL(tc, buf);
            if (!buf)
                return false;
            if (!buf->len ||
                buf->len > (unsigned int)(buf->tail - buf->data)) {
                ogs_pkbuf_free(buf);
                ABTS_FAIL(tc, "Cannot receive the GTP-U packet");
                return false;
            }
            if (buf->len < OGS_GTPV1U_HEADER_LEN) {
                ogs_pkbuf_free(buf);
                continue;
            }
            gtp = (ogs_gtp2_header_t *)buf->data;
            if (gtp->type == OGS_GTPU_MSGTYPE_ERR_IND) {
                ogs_pkbuf_free(buf);
                /* Another tunnel's delayed Error Indication may share this
                 * socket. Only this probe's Echo Reply proves it is alive. */
                continue;
            }
            header_len = ogs_gtpu_parse_header(&received, buf);
            if (gtp->version == OGS_GTP1_VERSION_1 &&
                gtp->type == OGS_GTPU_MSGTYPE_GPDU &&
                be32toh(gtp->teid) == bearer->sess->gnb_n3_teid &&
                header_len >= OGS_GTPV1U_HEADER_LEN &&
                buf->len >= header_len + sizeof(packet_words) &&
                matching_qfi(buf, header_len, bearer->qfi)) {
                ip = buf->data + header_len;
                ip_len = (ip[0] & 0x0f) * 4;
                if ((ip[0] >> 4) == 4 && ip_len >= 20 &&
                    buf->len >= header_len + ip_len + 16 &&
                    ip[9] == IPPROTO_ICMP &&
                    !memcmp(ip + 12, &dst, 4) &&
                    !memcmp(ip + 16, &bearer->sess->ue_ip.addr, 4) &&
                    ip[ip_len] == 0 && ip[ip_len + 1] == 0 &&
                    !memcmp(ip + ip_len + 4, packet + 24, 12))
                    reply = true;
            }
            ogs_pkbuf_free(buf);
            if (reply)
                break;
        }
    }
    if (reply != alive)
        ABTS_FAIL(tc, alive ? "No ICMP Echo Reply on the original bearer" :
                "The deleted bearer still forwards ICMP Echo Replies");
    return reply == alive;
}

static bool check_bearers(abts_case *tc, ogs_socknode_t *gtpu,
        test_bearer_t *bearers[], unsigned int count, bool alive)
{
    unsigned int i;

    for (i = 0; i < count; i++) {
        if (!check_bearer(tc, gtpu, bearers[i], alive))
            return false;
    }
    return true;
}

static test_ue_t *new_ue(const char *msin)
{
    ogs_nas_5gs_mobile_identity_suci_t suci;
    test_ue_t *ue;

    memset(&suci, 0, sizeof(suci));
    suci.h.supi_format = OGS_NAS_5GS_SUPI_FORMAT_IMSI;
    suci.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;
    suci.routing_indicator2 = suci.routing_indicator3 =
        suci.routing_indicator4 = 0xf;
    ue = test_ue_add_by_suci(&suci, msin);
    ogs_assert(ue);
    ue->nr_cgi.cell_id = 0x40001;
    ue->nas.registration.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    ue->nas.registration.follow_on_request = 1;
    ue->nas.registration.value = OGS_NAS_5GS_REGISTRATION_TYPE_INITIAL;
    ue->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    ue->opc_string = "e8ed289deba952e4283b54e88e6183ca";
    ogs_assert(ogs_hex_from_string_checked(
                ue->k_string, ue->k, sizeof(ue->k)) == OGS_OK);
    ogs_assert(ogs_hex_from_string_checked(
                ue->opc_string, ue->opc, sizeof(ue->opc)) == OGS_OK);
    ue->mobile_identity_imeisv.digit16 = 7;
    return ue;
}

static ogs_pkbuf_t *registration_request(test_ue_t *ue,
        bool guti, bool container)
{
    memset(&ue->registration_request_param, 0,
            sizeof(ue->registration_request_param));
    ue->registration_request_param.guti = guti;
    if (container) {
        ue->registration_request_param.gmm_capability = 1;
        ue->registration_request_param.s1_ue_network_capability = 1;
        ue->registration_request_param.requested_nssai = 1;
        ue->registration_request_param.last_visited_registered_tai = 1;
        ue->registration_request_param.ue_usage_setting = 1;
    }
    return testgmm_build_registration_request(ue, NULL, false, false);
}

static bool finish_registration(abts_case *tc,
        ogs_socknode_t *ngap, test_ue_t *ue)
{
    if (!receive_ngap(tc, ngap, ue, NGAP_ProcedureCode_id_InitialContextSetup,
                OGS_NAS_5GS_REGISTRATION_ACCEPT, 0) ||
        !send_ngap(tc, ngap,
                testngap_build_ue_radio_capability_info_indication(ue)) ||
        !send_ngap(tc, ngap,
                testngap_build_initial_context_setup_response(ue, false)) ||
        !send_nas(tc, ngap, ue, testgmm_build_registration_complete(ue)))
        return false;
    return receive_ngap(tc, ngap, ue, NGAP_ProcedureCode_id_DownlinkNASTransport,
            OGS_NAS_5GS_CONFIGURATION_UPDATE_COMMAND, 0);
}

static bool establish_session(abts_case *tc,
        ogs_socknode_t *ngap, test_ue_t *ue, char *dnn, uint8_t psi)
{
    test_sess_t *sess = test_sess_add_by_dnn_and_psi(ue, dnn, psi);
    test_bearer_t *flow;
    ogs_pkbuf_t *gsm;

    ogs_assert(sess);
    sess->ul_nas_transport_param.request_type = OGS_NAS_5GS_REQUEST_TYPE_INITIAL;
    sess->ul_nas_transport_param.dnn = 1;
    sess->ul_nas_transport_param.s_nssai = 1;
    sess->pdu_session_establishment_param.ssc_mode = 1;
    sess->pdu_session_establishment_param.epco = 1;
    gsm = testgsm_build_pdu_session_establishment_request(sess);
    if (!send_nas(tc, ngap, ue, testgmm_build_ul_nas_transport(sess,
                    OGS_NAS_PAYLOAD_CONTAINER_N1_SM_INFORMATION, gsm)) ||
        !receive_ngap(tc, ngap, ue, NGAP_ProcedureCode_id_PDUSessionResourceSetup,
                    OGS_NAS_5GS_DL_NAS_TRANSPORT,
                    OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_ACCEPT))
        return false;
    flow = test_qos_flow_find_by_qfi(sess, 1);
    ABTS_PTR_NOTNULL(tc, flow);
    ABTS_TRUE(tc, sess->ue_ip.ipv4 && sess->ue_ip.addr);
    if (!flow)
        return false;
    if (!send_ngap(tc, ngap,
                testngap_sess_build_pdu_session_resource_setup_response(sess)))
        return false;

    /* Setup Response starts an asynchronous AMF-to-SMF update with no
     * NAS/NGAP completion acknowledgement. An N3 Echo Reply can arrive before
     * the AMF receives its SBI response. Allow the initial setup to settle
     * before starting the GUTI scenario, as in the registration idle tests. */
    ogs_msleep(100);
    return true;
}

static bool release_ue(abts_case *tc, ogs_socknode_t *ngap, test_ue_t *ue)
{
    uint64_t current_id = ue->ran_ue_ngap_id;
    int i;

    for (i = 0; i < 2; i++) {
        bool current;
        if (!receive_ngap(tc, ngap, ue,
                    NGAP_ProcedureCode_id_UEContextRelease, 0, 0))
            return false;
        current = ue->ran_ue_ngap_id == current_id;
        if (!send_ngap(tc, ngap, testngap_build_ue_context_release_complete(ue)))
            return false;
        if (current)
            return true;
    }
    ABTS_FAIL(tc, "The current RAN UE context was not released");
    return false;
}

static bool equipment_begin(abts_case *tc)
{
    bson_error_t error;
    bson_t *query = BCON_NEW("pei", BCON_UTF8(AMF_GUTI_TEST_PEI));
    bool rv;

    /* Reset only this test's identity, including records from interrupted runs. */
    rv = mongoc_collection_delete_many(ogs_mongoc()->collection.eir,
            query, NULL, NULL, &error);
    if (!rv)
        ABTS_FAIL(tc, error.message);
    bson_destroy(query);
    return rv;
}

static bool equipment_record(abts_case *tc, bson_oid_t *id, const char *status)
{
    bson_error_t error;
    bson_t *query = BCON_NEW("_id", BCON_OID(id));
    bson_t *doc = BCON_NEW("$set", "{",
            "pei", BCON_UTF8(AMF_GUTI_TEST_PEI),
            "status", BCON_UTF8(status), "}");
    bson_t *opts = BCON_NEW("upsert", BCON_BOOL(true));
    bool rv = mongoc_collection_update_one(ogs_mongoc()->collection.eir,
            query, doc, opts, NULL, &error);

    if (!rv)
        ABTS_FAIL(tc, error.message);
    bson_destroy(opts);
    bson_destroy(doc);
    bson_destroy(query);
    return rv;
}

static ogs_pkbuf_t *bad_authentication_response(abts_case *tc, test_ue_t *ue)
{
    ogs_nas_5gs_message_t message;
    ogs_pkbuf_t *buf = testgmm_build_authentication_response(ue);
    ogs_pkbuf_t *result = NULL;
    int rv;

    ABTS_PTR_NOTNULL(tc, buf);
    if (!buf)
        return NULL;
    rv = ogs_nas_5gmm_decode(&message, buf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    if (rv == OGS_OK) {
        ogs_nas_authentication_response_parameter_t *response =
            &message.gmm.authentication_response.authentication_response_parameter;
        ABTS_INT_EQUAL(tc, OGS_AUTN_LEN, response->length);
        response->res[0] ^= 0xff;
        result = ogs_nas_5gs_plain_encode(&message);
    }
    ogs_pkbuf_free(buf);
    return result;
}

static ogs_pkbuf_t *security_mode_reject(void)
{
    ogs_nas_5gs_message_t message;

    memset(&message, 0, sizeof(message));
    message.gmm.h.extended_protocol_discriminator =
        OGS_NAS_EXTENDED_PROTOCOL_DISCRIMINATOR_5GMM;
    message.gmm.h.message_type = OGS_NAS_5GS_SECURITY_MODE_REJECT;
    message.gmm.security_mode_reject.gmm_cause =
        OGS_5GMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED;
    return ogs_nas_5gs_plain_encode(&message);
}

static void test_amf_case(abts_case *tc, void *data)
{
    const unsigned int index = *(const unsigned int *)data;
    scenario_t scenario = cases[index].scenario;
    unsigned int sessions = cases[index].sessions, i;
    bool subscriber = false, equipment = false;
    test_ue_t *victim = NULL, *peer = NULL, *cleanup_ue;
    test_bearer_t *old_flows[2] = { NULL, NULL }, *new_flow;
    ogs_socknode_t *ngap = NULL, *gtpu = NULL;
    ogs_pkbuf_t *container = NULL;
    bson_oid_t equipment_id;
    char msin[11];
#if !HAVE_USRSCTP
    struct timeval timeout = { 5, 0 };
#endif

    ogs_snprintf(msin, sizeof(msin), "00004577%02u", index);
    victim = new_ue(msin);
    if (test_db_insert_ue(victim, sessions == 2 ?
                test_db_new_ims(victim) : test_db_new_simple(victim)) != OGS_OK) {
        ABTS_FAIL(tc, "Cannot insert AMF GUTI test subscriber");
        goto cleanup;
    }
    subscriber = true;
    if (!equipment_begin(tc))
        goto cleanup;
    bson_oid_init(&equipment_id, NULL);
    equipment = true;
    if (!equipment_record(tc, &equipment_id, "WHITELISTED"))
        goto cleanup;
    ngap = testngap_client(1, AF_INET);
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, ngap);
    ABTS_PTR_NOTNULL(tc, gtpu);
    if (!ngap || !gtpu)
        goto cleanup;
#if !HAVE_USRSCTP
    ABTS_INT_EQUAL(tc, 0, setsockopt(ngap->sock->fd, SOL_SOCKET, SO_RCVTIMEO,
                &timeout, sizeof(timeout)));
#endif
    if (!send_ngap(tc, ngap, testngap_build_ng_setup_request(0x4577 + index, 22)) ||
        !receive_ngap(tc, ngap, victim, NGAP_ProcedureCode_id_NGSetup, 0, 0) ||
        !send_ngap(tc, ngap, testngap_build_initial_ue_message(victim,
                    registration_request(victim, false, false),
                    NGAP_RRCEstablishmentCause_mo_Signalling, false, true)) ||
        !receive_ngap(tc, ngap, victim, NGAP_ProcedureCode_id_DownlinkNASTransport,
                    OGS_NAS_5GS_AUTHENTICATION_REQUEST, 0) ||
        !send_nas(tc, ngap, victim, testgmm_build_authentication_response(victim)) ||
        !receive_ngap(tc, ngap, victim, NGAP_ProcedureCode_id_DownlinkNASTransport,
                    OGS_NAS_5GS_SECURITY_MODE_COMMAND, 0) ||
        !send_nas(tc, ngap, victim, testgmm_build_security_mode_complete(victim,
                    registration_request(victim, false, true))) ||
        !finish_registration(tc, ngap, victim))
        goto cleanup;
    for (i = 0; i < sessions; i++) {
        test_sess_t *sess;
        if (!establish_session(tc, ngap, victim, i ? "ims" : "internet", 5 + i))
            goto cleanup;
        sess = test_sess_find_by_psi(victim, 5 + i);
        ogs_assert(sess);
        old_flows[i] = test_qos_flow_find_by_qfi(sess, 1);
        ogs_assert(old_flows[i]);
    }
    if (sessions == 2) {
        ABTS_TRUE(tc, old_flows[0]->sess->upf_n3_teid != old_flows[1]->sess->upf_n3_teid);
        ABTS_TRUE(tc, old_flows[0]->sess->gnb_n3_teid != old_flows[1]->sess->gnb_n3_teid);
        ABTS_TRUE(tc, old_flows[0]->sess->ue_ip.addr != old_flows[1]->sess->ue_ip.addr);
    }
    if (!check_bearers(tc, gtpu, old_flows, sessions, true))
        goto cleanup;

    /* Copy only the assigned GUTI. The victim keeps the original NAS keys,
     * counters, PDU sessions and N3 tunnels for preservation probes. */
    peer = new_ue(msin);
    peer->nas_5gs_guti = victim->nas_5gs_guti;
    peer->ran_ue_ngap_id = victim->ran_ue_ngap_id;
    if (scenario == EIR_REJECT &&
        !equipment_record(tc, &equipment_id, "BLACKLISTED"))
        goto cleanup;
    if (!send_ngap(tc, ngap, testngap_build_initial_ue_message(peer,
                    registration_request(peer, true, false),
                    NGAP_RRCEstablishmentCause_mo_Signalling, false, true)) ||
        !receive_ngap(tc, ngap, peer, NGAP_ProcedureCode_id_DownlinkNASTransport,
                    OGS_NAS_5GS_AUTHENTICATION_REQUEST, 0) ||
        !check_bearers(tc, gtpu, old_flows, sessions, true))
        goto cleanup;
    ABTS_TRUE(tc, peer->ran_ue_ngap_id != victim->ran_ue_ngap_id);

    if (scenario == DEREG_DURING_AUTH &&
        !send_plain_deregistration(tc, ngap, peer))
        goto cleanup;
    if (scenario == BAD_RES) {
        if (!send_nas(tc, ngap, peer, bad_authentication_response(tc, peer)))
            goto cleanup;
    } else if (scenario == AUTH_FAILURE || scenario == DEREG_DURING_AUTH) {
        if (!send_nas(tc, ngap, peer, testgmm_build_authentication_failure(
                        peer, OGS_5GMM_CAUSE_MAC_FAILURE, 0)))
            goto cleanup;
    } else {
        if (!send_nas(tc, ngap, peer, testgmm_build_authentication_response(peer)) ||
            !receive_ngap(tc, ngap, peer, NGAP_ProcedureCode_id_DownlinkNASTransport,
                        OGS_NAS_5GS_SECURITY_MODE_COMMAND, 0) ||
            !check_bearers(tc, gtpu, old_flows, sessions, true))
            goto cleanup;
        if (scenario == DEREG_DURING_SMC &&
            !send_plain_deregistration(tc, ngap, peer))
            goto cleanup;
        if (scenario == SMC_REJECT || scenario == DEREG_DURING_SMC) {
            if (!send_nas(tc, ngap, peer, security_mode_reject()))
                goto cleanup;
        } else {
            container = registration_request(peer, true, true);
            if (!send_nas(tc, ngap, peer,
                        testgmm_build_security_mode_complete(peer, container))) {
                container = NULL;
                goto cleanup;
            }
            container = NULL;
            if (scenario == EIR_REJECT) {
                if (!receive_ngap(tc, ngap, peer, NGAP_ProcedureCode_id_DownlinkNASTransport,
                            OGS_NAS_5GS_REGISTRATION_REJECT, 0))
                    goto cleanup;
                ABTS_INT_EQUAL(tc, OGS_5GMM_CAUSE_ILLEGAL_ME, peer->registration_reject_cause);
                if (!release_ue(tc, ngap, peer) ||
                    !check_bearers(tc, gtpu, old_flows, sessions, false))
                    goto cleanup;
                goto cleanup;
            }
            if (!finish_registration(tc, ngap, peer) ||
                !check_bearers(tc, gtpu, old_flows, sessions, false) ||
                !establish_session(tc, ngap, peer, "internet", 5))
                goto cleanup;
            new_flow = test_qos_flow_find_by_qfi(test_sess_find_by_psi(peer, 5), 1);
            ogs_assert(new_flow);
            for (i = 0; i < sessions; i++)
                ABTS_TRUE(tc, new_flow->sess->upf_n3_teid != old_flows[i]->sess->upf_n3_teid);
            if (!check_bearer(tc, gtpu, new_flow, true))
                goto cleanup;
        }
    }

    if (scenario == BAD_RES || scenario == AUTH_FAILURE ||
        scenario == DEREG_DURING_AUTH) {
        if (!receive_ngap(tc, ngap, peer, NGAP_ProcedureCode_id_DownlinkNASTransport,
                    OGS_NAS_5GS_AUTHENTICATION_REJECT, 0))
            goto cleanup;
    }
    if (scenario != REREGISTER &&
        !check_bearers(tc, gtpu, old_flows, sessions, true))
        goto cleanup;
    /* A protected Deregistration with the original keys verifies rollback.
     * SMC Reject has no reply, so this follows it on the same SCTP stream.
     * This stage retains the peer's NG association after failure; use current
     * IDs here and revise cleanup when NG association restoration is added. */
    cleanup_ue = scenario == REREGISTER ? peer : victim;
    cleanup_ue->ran_ue_ngap_id = peer->ran_ue_ngap_id;
    cleanup_ue->amf_ue_ngap_id = peer->amf_ue_ngap_id;
    if (!send_nas(tc, ngap, cleanup_ue,
                testgmm_build_de_registration_request(cleanup_ue, false, true, false)) ||
        !receive_ngap(tc, ngap, cleanup_ue, NGAP_ProcedureCode_id_DownlinkNASTransport,
                    OGS_NAS_5GS_DEREGISTRATION_ACCEPT_FROM_UE, 0))
        goto cleanup;
    ABTS_INT_EQUAL(tc, 0, cleanup_ue->mac_failed);
    if (!release_ue(tc, ngap, cleanup_ue))
        goto cleanup;

cleanup:
    if (container)
        ogs_pkbuf_free(container);
    if (ngap)
        testgnb_ngap_close(ngap);
    if (gtpu)
        test_gtpu_close(gtpu);
    if (equipment) {
        bson_error_t error;
        bson_t *query = BCON_NEW("_id", BCON_OID(&equipment_id));
        if (!mongoc_collection_delete_one(ogs_mongoc()->collection.eir,
                    query, NULL, NULL, &error))
            ABTS_FAIL(tc, error.message);
        bson_destroy(query);
    }
    if (subscriber)
        ABTS_INT_EQUAL(tc, OGS_OK, test_db_remove_ue(victim));
    if (peer)
        test_ue_remove(peer);
    if (victim)
        test_ue_remove(victim);
}

abts_suite *test_guti_amf(abts_suite *suite)
{
    unsigned int i;
    static unsigned int indexes[OGS_ARRAY_SIZE(cases)];

    suite = ADD_SUITE(suite);
    for (i = 0; i < OGS_ARRAY_SIZE(cases); i++) {
        indexes[i] = i;
        abts_run_test(suite, test_amf_case, &indexes[i]);
    }
    return suite;
}
