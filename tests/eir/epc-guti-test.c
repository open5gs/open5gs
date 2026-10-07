/* Copyright (C) 2026 by Sukchan Lee <acetcom@gmail.com>
 * SPDX-License-Identifier: AGPL-3.0-or-later */

#include "test-common.h"
#include <poll.h>
#include <sys/time.h>

typedef enum {
    BAD_RES,
    AUTH_FAILURE,
    SMC_REJECT,
    DETACH_DURING_AUTH,
    DETACH_DURING_SMC,
    REATTACH,
    EIR_REJECT,
} scenario_t;

static const struct {
    scenario_t scenario;
    unsigned int pdns;
} cases[] = {
    { BAD_RES, 1 },
    { AUTH_FAILURE, 1 },
    { SMC_REJECT, 1 },
    { DETACH_DURING_AUTH, 1 },
    { DETACH_DURING_SMC, 1 },
    { REATTACH, 1 },
    { EIR_REJECT, 1 },
    { AUTH_FAILURE, 2 },
    { REATTACH, 2 },
};

static bool send_s1ap(abts_case *tc, ogs_socknode_t *s1ap, ogs_pkbuf_t *buf)
{
    int rv;

    ABTS_PTR_NOTNULL(tc, buf);
    if (!buf)
        return false;
    rv = testenb_s1ap_send(s1ap, buf);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    return rv == OGS_OK;
}

static bool send_nas(abts_case *tc, ogs_socknode_t *s1ap,
        test_ue_t *ue, ogs_pkbuf_t *buf)
{
    ABTS_PTR_NOTNULL(tc, buf);
    if (!buf)
        return false;
    return send_s1ap(tc, s1ap, test_s1ap_build_uplink_nas_transport(ue, buf));
}

static bool send_plain_detach(abts_case *tc, ogs_socknode_t *s1ap, test_ue_t *ue)
{
    ogs_nas_eps_message_t message;
    ogs_pkbuf_t *buf, *copy;
    int rv;
    bool valid;

    buf = testemm_build_detach_request(ue, true, false, false);
    ABTS_PTR_NOTNULL(tc, buf);
    if (!buf)
        return false;
    copy = ogs_pkbuf_copy(buf);
    ABTS_PTR_NOTNULL(tc, copy);
    if (!copy) {
        ogs_pkbuf_free(buf);
        return false;
    }
    /* A malformed stimulus would be dropped before the MME's Detach guard,
     * allowing this preservation test to pass without exercising that guard. */
    rv = ogs_nas_emm_decode(&message, copy);
    ogs_pkbuf_free(copy);
    ABTS_INT_EQUAL(tc, OGS_OK, rv);
    valid = rv == OGS_OK &&
        message.emm.h.security_header_type == OGS_NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE &&
        message.emm.h.protocol_discriminator == OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM &&
        message.emm.h.message_type == OGS_NAS_EPS_DETACH_REQUEST &&
        message.emm.detach_request_from_ue.detach_type.switch_off == 1 &&
        message.emm.detach_request_from_ue.eps_mobile_identity.guti.type ==
            OGS_NAS_EPS_MOBILE_IDENTITY_GUTI &&
        message.emm.detach_request_from_ue.eps_mobile_identity.guti.m_tmsi ==
            ue->nas_eps_guti.m_tmsi;
    ABTS_TRUE(tc, valid);
    if (!valid) {
        ogs_pkbuf_free(buf);
        return false;
    }
    return send_nas(tc, s1ap, ue, buf);
}

static bool receive_s1ap(abts_case *tc, ogs_socknode_t *s1ap,
        test_ue_t *ue, long procedure, int emm, int esm)
{
#if !HAVE_USRSCTP
    struct pollfd fd = { s1ap->sock->fd, POLLIN, 0 };
    int rv;
#endif
    ogs_time_t deadline = ogs_time_now() + ogs_time_from_sec(5);
    ogs_pkbuf_t *buf;

    while (ogs_time_now() < deadline) {
        uint32_t enb_id = ue->enb_ue_s1ap_id;
        uint32_t mme_id = ue->mme_ue_s1ap_id;
#if !HAVE_USRSCTP
        ogs_time_t remaining = deadline - ogs_time_now();
        if (remaining <= 0)
            break;
        rv = poll(&fd, 1, ogs_time_to_msec(remaining) + 1);
        if (rv < 0 && errno == EINTR)
            continue;
        if (rv <= 0 || !(fd.revents & POLLIN))
            break;
#endif
        /* usrsctp sockets have no native fd; use the existing SCTP reader.
         * Meson's suite timeout also bounds that platform's blocking reads. */
        buf = testenb_s1ap_read(s1ap);
        ABTS_PTR_NOTNULL(tc, buf);
        if (!buf)
            return false;
        ue->emm_message_type = ue->esm_message_type = 0;
        tests1ap_recv(ue, buf);
        /* CLEAR_S1_CONTEXT can release the held UE before or after the next
         * NAS message. Acknowledge that UE without overwriting the current IDs. */
        if (procedure != S1AP_ProcedureCode_id_UEContextRelease && enb_id &&
            ue->s1ap_procedure_code == S1AP_ProcedureCode_id_UEContextRelease &&
            ue->enb_ue_s1ap_id != enb_id) {
            bool sent = send_s1ap(tc, s1ap,
                    test_s1ap_build_ue_context_release_complete(ue));
            ue->enb_ue_s1ap_id = enb_id;
            ue->mme_ue_s1ap_id = mme_id;
            if (!sent)
                return false;
            continue;
        }
        ABTS_INT_EQUAL(tc, procedure, ue->s1ap_procedure_code);
        if (emm)
            ABTS_INT_EQUAL(tc, emm, ue->emm_message_type);
        if (esm)
            ABTS_INT_EQUAL(tc, esm, ue->esm_message_type);
        return ue->s1ap_procedure_code == procedure &&
            (!emm || ue->emm_message_type == emm) &&
            (!esm || ue->esm_message_type == esm);
    }
    ABTS_FAIL(tc, "Timed out waiting for the expected S1AP message");
    return false;
}

/* Match this probe, not an old reply, Router Advertisement or Error Indication.
 * In unpatched main, Authentication Request follows Delete Session Response,
 * so probing the original tunnel at that point detects the regression. */
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
    memcpy(packet + 28, "guti-epc", 8);
    value = ogs_in_cksum((uint16_t *)(packet + 20), 16);
    memcpy(packet + 22, &value, 2);
    value = ogs_in_cksum((uint16_t *)packet, 20);
    memcpy(packet + 10, &value, 2);

    memset(&desc, 0, sizeof(desc));
    desc.type = OGS_GTPU_MSGTYPE_GPDU;
    desc.teid = bearer->sgw_s1u_teid;
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
                be32toh(gtp->teid) == bearer->enb_s1u_teid &&
                header_len >= OGS_GTPV1U_HEADER_LEN &&
                buf->len >= header_len + sizeof(packet_words)) {
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

static test_ue_t *new_ue(const char *imsi)
{
    ogs_nas_5gs_mobile_identity_suci_t suci;
    test_ue_t *ue;

    memset(&suci, 0, sizeof(suci));
    suci.h.supi_format = OGS_NAS_5GS_SUPI_FORMAT_IMSI;
    suci.h.type = OGS_NAS_5GS_MOBILE_IDENTITY_SUCI;
    suci.routing_indicator2 = suci.routing_indicator3 =
        suci.routing_indicator4 = 0xf;
    ue = test_ue_add_by_suci(&suci, imsi);
    ogs_assert(ue);
    ue->e_cgi.cell_id = 0x1079baf0;
    ue->nas.ksi = OGS_NAS_KSI_NO_KEY_IS_AVAILABLE;
    ue->nas.value = OGS_NAS_ATTACH_TYPE_EPS_ATTACH;
    ue->k_string = "465b5ce8b199b49faa5f0a2ee238a6bc";
    ue->opc_string = "e8ed289deba952e4283b54e88e6183ca";
    ue->mobile_identity_imeisv.digit16 = 8;
    ue->mobile_identity_imeisv_presence = true;
    ogs_assert(test_sess_add_by_apn(ue, "internet", OGS_GTP2_RAT_TYPE_EUTRAN));
    return ue;
}

static ogs_pkbuf_t *plain_attach(test_ue_t *ue, bool guti)
{
    test_sess_t *sess = ogs_list_first(&ue->sess_list);
    ogs_pkbuf_t *esm;

    sess->pti = 1;
    memset(&sess->pdn_connectivity_param, 0, sizeof(sess->pdn_connectivity_param));
    sess->pdn_connectivity_param.eit = 1;
    sess->pdn_connectivity_param.request_type = OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    esm = testesm_build_pdn_connectivity_request(sess, false, OGS_NAS_EPS_PDN_TYPE_IPV4);
    ogs_assert(esm);
    memset(&ue->attach_request_param, 0, sizeof(ue->attach_request_param));
    ue->attach_request_param.guti = guti;
    return testemm_build_attach_request(ue, esm, false, false);
}

static bool finish_attach(abts_case *tc, ogs_socknode_t *s1ap, test_ue_t *ue)
{
    test_sess_t *sess = ogs_list_first(&ue->sess_list);
    test_bearer_t *bearer;

    if (!receive_s1ap(tc, s1ap, ue, S1AP_ProcedureCode_id_downlinkNASTransport,
                0, OGS_NAS_EPS_ESM_INFORMATION_REQUEST) ||
        !send_nas(tc, s1ap, ue, testesm_build_esm_information_response(sess)) ||
        !receive_s1ap(tc, s1ap, ue, S1AP_ProcedureCode_id_InitialContextSetup,
                OGS_NAS_EPS_ATTACH_ACCEPT,
                OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST))
        return false;
    bearer = test_bearer_find_by_ue_ebi(ue, 5);
    ABTS_PTR_NOTNULL(tc, bearer);
    if (!bearer ||
        !send_s1ap(tc, s1ap, test_s1ap_build_initial_context_setup_response(ue)) ||
        !send_nas(tc, s1ap, ue, testemm_build_attach_complete(ue,
            testesm_build_activate_default_eps_bearer_context_accept(bearer, false))))
        return false;
    return receive_s1ap(tc, s1ap, ue, S1AP_ProcedureCode_id_downlinkNASTransport,
            OGS_NAS_EPS_EMM_INFORMATION, 0);
}

static bool add_ims_pdn(abts_case *tc, ogs_socknode_t *s1ap, test_ue_t *ue)
{
    test_sess_t *sess;
    test_bearer_t *bearer;

    sess = test_sess_add_by_apn(ue, "ims", OGS_GTP2_RAT_TYPE_EUTRAN);
    ogs_assert(sess);
    sess->pti = 2;
    /* Use the standalone IMS PDN setup exercised by the VoLTE tests. */
    sess->pdn_connectivity_param.apn = 1;
    sess->pdn_connectivity_param.pco = 1;
    sess->pdn_connectivity_param.request_type = OGS_NAS_EPS_REQUEST_TYPE_INITIAL;
    if (!send_nas(tc, s1ap, ue, testesm_build_pdn_connectivity_request(
                    sess, true, OGS_NAS_EPS_PDN_TYPE_IPV4)) ||
        !receive_s1ap(tc, s1ap, ue, S1AP_ProcedureCode_id_E_RABSetup,
                    0, OGS_NAS_EPS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST))
        return false;
    bearer = test_bearer_find_by_ue_ebi(ue, 6);
    ABTS_PTR_NOTNULL(tc, bearer);
    if (!bearer)
        return false;
    ABTS_PTR_EQUAL(tc, sess, bearer->sess);
    ABTS_TRUE(tc, sess->ue_ip.ipv4 && sess->ue_ip.addr);
    /* The fixture has no installed PCC flows, so both PDNs have only their
     * default bearer. No dedicated-bearer or Rx procedure is part of this test. */
    ABTS_PTR_EQUAL(tc, NULL, test_bearer_find_by_ue_ebi(ue, 7));
    return send_s1ap(tc, s1ap, test_s1ap_build_e_rab_setup_response(bearer)) &&
        send_nas(tc, s1ap, ue,
                testesm_build_activate_default_eps_bearer_context_accept(bearer, true));
}

/* The old, held S1 context can be released before the current one. */
static bool release_ue(abts_case *tc, ogs_socknode_t *s1ap, test_ue_t *ue)
{
    uint32_t current_id = ue->enb_ue_s1ap_id;
    int i;

    for (i = 0; i < 2; i++) {
        bool current;
        if (!receive_s1ap(tc, s1ap, ue, S1AP_ProcedureCode_id_UEContextRelease, 0, 0))
            return false;
        current = ue->enb_ue_s1ap_id == current_id;
        if (!send_s1ap(tc, s1ap, test_s1ap_build_ue_context_release_complete(ue)))
            return false;
        if (current)
            return true;
    }
    ABTS_FAIL(tc, "The current S1 UE context was not released");
    return false;
}

static bool equipment_record(abts_case *tc, bson_oid_t *id, const char *status)
{
    bson_error_t error;
    bson_t *query = BCON_NEW("_id", BCON_OID(id));
    bson_t *doc = BCON_NEW("$set", "{", "pei", BCON_UTF8("imeisv-8665070400405308"),
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

static void test_epc_case(abts_case *tc, void *data)
{
    const unsigned int index = *(const unsigned int *)data;
    scenario_t scenario = cases[index].scenario;
    const unsigned int pdns = cases[index].pdns;
    unsigned int i;
    bool subscriber = false, equipment = false;
    test_ue_t *victim = NULL, *peer = NULL, *cleanup_ue;
    test_bearer_t *old_bearers[2] = { NULL, NULL }, *new_bearer;
    ogs_socknode_t *s1ap = NULL, *gtpu = NULL;
    ogs_pkbuf_t *nas;
    ogs_nas_eps_message_t message;
    bson_oid_t equipment_id;
    char imsi[16];
#if !HAVE_USRSCTP
    struct timeval timeout = { 5, 0 };
#endif

    ogs_snprintf(imsi, sizeof(imsi), "374600009%u", index);
    victim = new_ue(imsi);
    if (test_db_insert_ue(victim, pdns == 2 ?
                test_db_new_ims(victim) : test_db_new_simple(victim)) != OGS_OK) {
        ABTS_FAIL(tc, "Cannot insert GUTI test subscriber");
        goto cleanup;
    }
    subscriber = true;
    bson_oid_init(&equipment_id, NULL);
    if (!equipment_record(tc, &equipment_id, "WHITELISTED"))
        goto cleanup;
    equipment = true;
    s1ap = tests1ap_client(AF_INET);
    gtpu = test_gtpu_server(1, AF_INET);
    ABTS_PTR_NOTNULL(tc, s1ap);
    ABTS_PTR_NOTNULL(tc, gtpu);
    if (!s1ap || !gtpu)
        goto cleanup;
    /* A readable SCTP notification must not leave the following read unbounded. */
#if !HAVE_USRSCTP
    ABTS_INT_EQUAL(tc, 0, setsockopt(s1ap->sock->fd, SOL_SOCKET, SO_RCVTIMEO,
                &timeout, sizeof(timeout)));
#endif
    if (!send_s1ap(tc, s1ap, test_s1ap_build_s1_setup_request(
                    S1AP_ENB_ID_PR_macroENB_ID, 0x54f64)) ||
        !receive_s1ap(tc, s1ap, victim, S1AP_ProcedureCode_id_S1Setup, 0, 0) ||
        !send_s1ap(tc, s1ap, test_s1ap_build_initial_ue_message(victim,
                    plain_attach(victim, false),
                    S1AP_RRC_Establishment_Cause_mo_Signalling, false)) ||
        !receive_s1ap(tc, s1ap, victim, S1AP_ProcedureCode_id_downlinkNASTransport,
                    OGS_NAS_EPS_AUTHENTICATION_REQUEST, 0) ||
        !send_nas(tc, s1ap, victim, testemm_build_authentication_response(victim)) ||
        !receive_s1ap(tc, s1ap, victim, S1AP_ProcedureCode_id_downlinkNASTransport,
                    OGS_NAS_EPS_SECURITY_MODE_COMMAND, 0) ||
        !send_nas(tc, s1ap, victim, testemm_build_security_mode_complete(victim)) ||
        !finish_attach(tc, s1ap, victim))
        goto cleanup;
    if (pdns == 2 && !add_ims_pdn(tc, s1ap, victim))
        goto cleanup;
    for (i = 0; i < pdns; i++) {
        old_bearers[i] = test_bearer_find_by_ue_ebi(victim, 5 + i);
        ogs_assert(old_bearers[i]);
    }
    if (pdns == 2) {
        ABTS_TRUE(tc, old_bearers[0]->sgw_s1u_teid != old_bearers[1]->sgw_s1u_teid);
        ABTS_TRUE(tc, old_bearers[0]->sess->ue_ip.addr != old_bearers[1]->sess->ue_ip.addr);
    }
    if (!check_bearers(tc, gtpu, old_bearers, pdns, true))
        goto cleanup;

    /* Separate UE state: only the assigned GUTI is copied. The victim's NAS
     * keys/counts and original GTP-U bearer must survive the failed attempt.
     * This does not assert restoration of the pre-authentication S1 association. */
    peer = new_ue(imsi);
    peer->nas_eps_guti = victim->nas_eps_guti;
    /* InitialUEMessage increments this ID. Use a fresh UE on this eNB. */
    peer->enb_ue_s1ap_id = victim->enb_ue_s1ap_id;
    if (scenario == EIR_REJECT &&
        !equipment_record(tc, &equipment_id, "BLACKLISTED"))
        goto cleanup;
    if (!send_s1ap(tc, s1ap, test_s1ap_build_initial_ue_message(peer,
                    plain_attach(peer, true),
                    S1AP_RRC_Establishment_Cause_mo_Signalling, false)) ||
        !receive_s1ap(tc, s1ap, peer, S1AP_ProcedureCode_id_downlinkNASTransport,
                    OGS_NAS_EPS_AUTHENTICATION_REQUEST, 0) ||
        !check_bearers(tc, gtpu, old_bearers, pdns, true))
        goto cleanup;
    ABTS_TRUE(tc, peer->enb_ue_s1ap_id != victim->enb_ue_s1ap_id);

    if (scenario == DETACH_DURING_AUTH &&
        !send_plain_detach(tc, s1ap, peer))
        goto cleanup;
    if (scenario == BAD_RES) {
        nas = testemm_build_authentication_response(peer);
        ogs_assert(nas && nas->len > 3);
        nas->data[3] ^= 0xff; /* Corrupt one RES octet, keep NAS well formed. */
        if (!send_nas(tc, s1ap, peer, nas))
            goto cleanup;
    } else if (scenario == AUTH_FAILURE || scenario == DETACH_DURING_AUTH) {
        if (!send_nas(tc, s1ap, peer, testemm_build_authentication_failure(
                        peer, OGS_NAS_EMM_CAUSE_MAC_FAILURE, 0)))
            goto cleanup;
    } else {
        if (!send_nas(tc, s1ap, peer, testemm_build_authentication_response(peer)) ||
            !receive_s1ap(tc, s1ap, peer, S1AP_ProcedureCode_id_downlinkNASTransport,
                        OGS_NAS_EPS_SECURITY_MODE_COMMAND, 0) ||
            !check_bearers(tc, gtpu, old_bearers, pdns, true))
            goto cleanup;
        if (scenario == DETACH_DURING_SMC &&
            !send_plain_detach(tc, s1ap, peer))
            goto cleanup;
        if (scenario == SMC_REJECT || scenario == DETACH_DURING_SMC) {
            memset(&message, 0, sizeof(message));
            message.emm.h.protocol_discriminator = OGS_NAS_PROTOCOL_DISCRIMINATOR_EMM;
            message.emm.h.message_type = OGS_NAS_EPS_SECURITY_MODE_REJECT;
            message.emm.security_mode_reject.emm_cause =
                OGS_NAS_EMM_CAUSE_SECURITY_MODE_REJECTED_UNSPECIFIED;
            if (!send_nas(tc, s1ap, peer, ogs_nas_eps_plain_encode(&message)))
                goto cleanup;
        } else {
            if (!send_nas(tc, s1ap, peer, testemm_build_security_mode_complete(peer)))
                goto cleanup;
            if (scenario == EIR_REJECT) {
                if (!receive_s1ap(tc, s1ap, peer, S1AP_ProcedureCode_id_downlinkNASTransport,
                            OGS_NAS_EPS_ATTACH_REJECT, 0))
                    goto cleanup;
                ABTS_INT_EQUAL(tc, OGS_NAS_EMM_CAUSE_ILLEGAL_ME, peer->attach_reject_cause);
                if (!release_ue(tc, s1ap, peer) ||
                    !check_bearers(tc, gtpu, old_bearers, pdns, false))
                    goto cleanup;
                goto cleanup;
            }
            if (!finish_attach(tc, s1ap, peer))
                goto cleanup;
            new_bearer = test_bearer_find_by_ue_ebi(peer, 5);
            ogs_assert(new_bearer);
            for (i = 0; i < pdns; i++)
                ABTS_TRUE(tc, new_bearer->sgw_s1u_teid != old_bearers[i]->sgw_s1u_teid);
            if (!check_bearer(tc, gtpu, new_bearer, true) ||
                !check_bearers(tc, gtpu, old_bearers, pdns, false))
                goto cleanup;
        }
    }

    if (scenario == BAD_RES || scenario == AUTH_FAILURE ||
        scenario == DETACH_DURING_AUTH) {
        if (!receive_s1ap(tc, s1ap, peer, S1AP_ProcedureCode_id_downlinkNASTransport,
                    OGS_NAS_EPS_AUTHENTICATION_REJECT, 0))
            goto cleanup;
    }
    if (scenario != REATTACH && !check_bearers(tc, gtpu, old_bearers, pdns, true))
        goto cleanup;
    /* A protected Detach with the original keys also checks security rollback.
     * It follows SMC Reject on the same SCTP stream, since SMC Reject has no reply.
     * Stage 1 leaves the S1 association on the peer after failure. Use those
     * current IDs; adjust this check when S1 association restoration is added. */
    cleanup_ue = scenario == REATTACH ? peer : victim;
    nas = testemm_build_detach_request(cleanup_ue, false, true, false);
    cleanup_ue->enb_ue_s1ap_id = peer->enb_ue_s1ap_id;
    cleanup_ue->mme_ue_s1ap_id = peer->mme_ue_s1ap_id;
    if (!send_nas(tc, s1ap, peer, nas) ||
        !receive_s1ap(tc, s1ap, cleanup_ue, S1AP_ProcedureCode_id_downlinkNASTransport,
                    OGS_NAS_EPS_DETACH_ACCEPT, 0))
        goto cleanup;
    ABTS_INT_EQUAL(tc, 0, cleanup_ue->mac_failed);
    if (!release_ue(tc, s1ap, cleanup_ue))
        goto cleanup;

cleanup:
    if (s1ap)
        testenb_s1ap_close(s1ap);
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

abts_suite *test_guti_epc(abts_suite *suite)
{
    unsigned int i;
    static unsigned int indexes[OGS_ARRAY_SIZE(cases)];

    suite = ADD_SUITE(suite);
    for (i = 0; i < OGS_ARRAY_SIZE(cases); i++) {
        indexes[i] = i;
        abts_run_test(suite, test_epc_case, &indexes[i]);
    }
    return suite;
}
