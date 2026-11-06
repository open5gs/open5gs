/*
 * Copyright (C) 2026 by Mohit Bidikar <mohitbidikar4@gmail.com>
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <https://www.gnu.org/licenses/>.
 */

#include "amf/context.h"
#include "amf/ngap-handler.h"
#include "amf/nsmf-build.h"
#include "core/abts.h"

/* Real NGAP handlers and SBI builders, without running peer NFs. */
typedef struct redcap_fixture_s {
    amf_gnb_t *gnb;
    int peer_fd;
    ogs_app_global_conf_t saved_global;
    ogs_app_local_conf_t saved_local;
    ogs_sbi_nf_instance_t nf;
    ogs_sbi_nf_instance_t *saved_nf;
    ogs_sbi_server_t server;
    ogs_sockaddr_t sbi_addr;
} redcap_fixture_t;

static void fixture_init(redcap_fixture_t *f)
{
    ogs_sock_t *sock;
    ogs_sockaddr_t *addr;
    int fds[2];

    memset(f, 0, sizeof(*f));
    f->saved_global = *ogs_global_conf();
    f->saved_local = *ogs_local_conf();
    ogs_local_conf()->time.message.duration = ogs_time_from_sec(10);
    ogs_app_context_init();
    ogs_app_global_conf_prepare();
    ogs_global_conf()->max.ue = 8;
    ogs_global_conf()->max.peer = 2;
    ogs_app()->pool.sess = 16;
    ogs_app()->metrics.max_specs = 64;
    ogs_app()->timer_mgr = ogs_timer_mgr_create(128);
    ogs_app()->pollset = ogs_pollset_create(16);
    ogs_app()->queue = ogs_queue_create(16);
    amf_metrics_init();
    amf_context_init();

    ogs_plmn_id_build(&amf_self()->served_guami[0].plmn_id, 999, 70, 2);
    amf_self()->num_of_served_guami = 1;

    ogs_assert(socketpair(AF_UNIX, SOCK_STREAM, 0, fds) == 0);
    sock = ogs_sock_create();
    ogs_assert(sock);
    sock->fd = fds[0];
    f->peer_fd = fds[1];
    addr = ogs_calloc(1, sizeof(*addr));
    ogs_assert(addr);
    addr->ogs_sa_family = AF_INET;
    addr->sin.sin_addr.s_addr = htobe32(INADDR_LOOPBACK);
    f->gnb = amf_gnb_add(sock, addr);
    ogs_assert(f->gnb);
    f->gnb->max_num_of_ostreams = 2;
    f->gnb->gnb_id_length = 28;

    /* Only the URI and NF identity are needed by the request builders. */
    f->saved_nf = ogs_sbi_self()->nf_instance;
    f->nf.id = "00000000-0000-4000-8000-000000000001";
    ogs_sbi_self()->nf_instance = &f->nf;
    f->sbi_addr.ogs_sa_family = AF_INET;
    f->sbi_addr.sin.sin_addr.s_addr = htobe32(INADDR_LOOPBACK);
    f->sbi_addr.ogs_sin_port = htobe16(7777);
    f->server.scheme = OpenAPI_uri_scheme_http;
    f->server.node.addr = &f->sbi_addr;
    ogs_list_add(&ogs_sbi_self()->server_list, &f->server);
}

static void fixture_clear_ues(redcap_fixture_t *f)
{
    amf_event_t *e;
    ran_ue_t *ran, *next;

    while (ogs_queue_trypop(ogs_app()->queue, (void **)&e) == OGS_OK) {
        ogs_pkbuf_free(e->pkbuf);
        ogs_event_free(e);
    }
    ogs_list_for_each_safe(&f->gnb->ran_ue_list, next, ran)
        ran_ue_remove(ran);
    amf_ue_remove_all();
    f->gnb->rat_type = OpenAPI_rat_type_NR;
}

static void fixture_final(redcap_fixture_t *f)
{
    fixture_clear_ues(f);
    amf_context_final();
    amf_metrics_final();
    ogs_closesocket(f->peer_fd);
    ogs_list_remove(&ogs_sbi_self()->server_list, &f->server);
    ogs_sbi_self()->nf_instance = f->saved_nf;
    ogs_app_context_final();
    *ogs_global_conf() = f->saved_global;
    *ogs_local_conf() = f->saved_local;
}

static ran_ue_t *initial_ue(abts_case *tc, redcap_fixture_t *f,
        uint64_t id, bool redcap, bool unknown_value,
        const ogs_nas_5gs_guti_t *guti)
{
    ogs_ngap_message_t pdu, decoded;
    NGAP_InitialUEMessage_t *initial;
    NGAP_InitialUEMessage_IEs_t *ie;
    NGAP_UserLocationInformationNR_t *nr;
    ogs_pkbuf_t *pkbuf;
    ogs_5gs_tai_t tai;
    ogs_nr_cgi_t cgi;
    uint8_t nas[] = { 0x7e, 0x00, OGS_NAS_5GS_REGISTRATION_REQUEST };
    ran_ue_t *ran;

    memset(&pdu, 0, sizeof(pdu));
    pdu.present = NGAP_NGAP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage = CALLOC(1, sizeof(NGAP_InitiatingMessage_t));
    pdu.choice.initiatingMessage->procedureCode = NGAP_ProcedureCode_id_InitialUEMessage;
    pdu.choice.initiatingMessage->criticality = NGAP_Criticality_ignore;
    pdu.choice.initiatingMessage->value.present = NGAP_InitiatingMessage__value_PR_InitialUEMessage;
    initial = CALLOC(1, sizeof(*initial));
    pdu.choice.initiatingMessage->value.choice.InitialUEMessage = initial;
    initial->protocolIEs = ogs_asn_calloc_protocol_ies(&asn_DEF_NGAP_InitialUEMessage);

#define ADD_INITIAL_IE(name, crit) \
    ie = CALLOC(1, sizeof(*ie)); \
    ASN_SEQUENCE_ADD(initial->protocolIEs, ie); \
    ie->id = NGAP_ProtocolIE_ID_id_ ## name; \
    ie->criticality = NGAP_Criticality_ ## crit; \
    ie->value.present = NGAP_InitialUEMessage_IEs__value_PR_ ## name; \
    ie->value.choice.name = CALLOC(1, sizeof(*ie->value.choice.name))

    ADD_INITIAL_IE(RAN_UE_NGAP_ID, reject);
    *ie->value.choice.RAN_UE_NGAP_ID = id;
    ADD_INITIAL_IE(NAS_PDU, reject);
    ogs_asn_buffer_to_OCTET_STRING(nas, sizeof(nas), ie->value.choice.NAS_PDU);
    ADD_INITIAL_IE(UserLocationInformation, reject);
    ie->value.choice.UserLocationInformation->present =
        NGAP_UserLocationInformation_PR_userLocationInformationNR;
    nr = CALLOC(1, sizeof(*nr));
    ie->value.choice.UserLocationInformation->choice.userLocationInformationNR = nr;
    nr->nR_CGI = CALLOC(1, sizeof(*nr->nR_CGI));
    nr->tAI = CALLOC(1, sizeof(*nr->tAI));
    memset(&tai, 0, sizeof(tai));
    memset(&cgi, 0, sizeof(cgi));
    ogs_plmn_id_build(&tai.plmn_id, 999, 70, 2);
    cgi.plmn_id = tai.plmn_id;
    cgi.cell_id = 1;
    tai.tac.v = 1;
    ogs_ngap_nr_cgi_to_ASN(&cgi, nr->nR_CGI);
    ogs_ngap_5gs_tai_to_ASN(&tai, nr->tAI);
    ADD_INITIAL_IE(RRCEstablishmentCause, ignore);
    *ie->value.choice.RRCEstablishmentCause = NGAP_RRCEstablishmentCause_mo_Signalling;
    if (guti) {
        NGAP_FiveG_S_TMSI_t *tmsi;
        ADD_INITIAL_IE(FiveG_S_TMSI, reject);
        tmsi = ie->value.choice.FiveG_S_TMSI;
        ogs_ngap_uint16_to_AMFSetID(
                ogs_amf_set_id(&guti->amf_id), &tmsi->aMFSetID);
        ogs_ngap_uint8_to_AMFPointer(
                ogs_amf_pointer(&guti->amf_id), &tmsi->aMFPointer);
        ogs_asn_uint32_to_OCTET_STRING(guti->m_tmsi, &tmsi->fiveG_TMSI);
    }
    if (redcap) {
        ADD_INITIAL_IE(RedCapIndication, ignore);
        *ie->value.choice.RedCapIndication = NGAP_RedCapIndication_redcap;
    }
#undef ADD_INITIAL_IE

    pkbuf = ogs_ngap_encode(&pdu);
    ogs_assert(pkbuf);
    memset(&decoded, 0, sizeof(decoded));
    ABTS_INT_EQUAL(tc, OGS_OK, ogs_ngap_decode(&decoded, pkbuf));
    if (unknown_value) {
        initial = decoded.choice.initiatingMessage->value.choice.InitialUEMessage;
        ie = OGS_ASN_LIST_GET(initial->protocolIEs,
                OGS_ASN_LIST_COUNT(initial->protocolIEs) - 1);
        *ie->value.choice.RedCapIndication = NGAP_RedCapIndication_redcap + 1;
    }
    ngap_handle_initial_ue_message(f->gnb, &decoded);
    ogs_ngap_free(&decoded);
    ogs_pkbuf_free(pkbuf);
    ran = ran_ue_find_by_ran_ue_ngap_id(f->gnb, id);
    ABTS_PTR_NOTNULL(tc, ran);
    return ran;
}

static amf_ue_t *associate_ue(ran_ue_t *ran)
{
    char supi[32];
    amf_ue_t *ue = amf_ue_add(ran);
    ogs_assert(ue);
    amf_ue_associate_ran_ue(ue, ran);
    ue->home_plmn_id = ue->nr_tai.plmn_id;
    ogs_snprintf(supi, sizeof(supi), "imsi-999700000000%03u", ue->id);
    amf_ue_set_supi(ue, supi);
    return ue;
}

static void check_sm_requests(abts_case *tc, amf_ue_t *ue,
        OpenAPI_rat_type_e expected)
{
    amf_sess_t *sess;
    amf_nsmf_pdusession_sm_context_param_t param;
    ogs_sbi_request_t *request;
    cJSON *json;
    OpenAPI_sm_context_create_data_t *create;
    OpenAPI_sm_context_update_data_t *update;

    sess = amf_sess_add(ue, 1);
    ogs_assert(sess);
    sess->dnn = ogs_strdup("internet");
    sess->s_nssai.sst = 1;
    sess->s_nssai.sd.v = OGS_S_NSSAI_NO_SD_VALUE;
    sess->lbo_roaming_allowed = true;
    sess->sm_context_resource_uri = ogs_strdup(
            "http://127.0.0.4:7777/nsmf-pdusession/v1/sm-contexts/1");
    request = amf_nsmf_pdusession_build_create_sm_context(sess, NULL);
    ABTS_PTR_NOTNULL(tc, request);
    json = cJSON_Parse(request->http.content);
    ABTS_PTR_NOTNULL(tc, json);
    create = OpenAPI_sm_context_create_data_parseFromJSON(json);
    ABTS_PTR_NOTNULL(tc, create);
    if (create)
        ABTS_INT_EQUAL(tc, expected, create->rat_type);
    OpenAPI_sm_context_create_data_free(create);
    cJSON_Delete(json);
    ogs_sbi_request_free(request);

    memset(&param, 0, sizeof(param));
    param.upCnxState = OpenAPI_up_cnx_state_ACTIVATING;
    request = amf_nsmf_pdusession_build_update_sm_context(sess, &param);
    ABTS_PTR_NOTNULL(tc, request);
    json = cJSON_Parse(request->http.content);
    ABTS_PTR_NOTNULL(tc, json);
    update = OpenAPI_sm_context_update_data_parseFromJSON(json);
    ABTS_PTR_NOTNULL(tc, update);
    if (update)
        ABTS_INT_EQUAL(tc, expected, update->rat_type);
    OpenAPI_sm_context_update_data_free(update);
    cJSON_Delete(json);
    ogs_sbi_request_free(request);
    amf_sess_remove(sess);
}

static void initial_and_sbi_func(abts_case *tc, void *data)
{
    redcap_fixture_t *f = data;
    ran_ue_t *redcap, *ordinary, *unknown;
    amf_ue_t *redcap_ue, *ordinary_ue, *unknown_ue;

    redcap = initial_ue(tc, f, 1, true, false, NULL);
    ordinary = initial_ue(tc, f, 2, false, false, NULL);
    unknown = initial_ue(tc, f, 3, true, true, NULL);
    redcap_ue = associate_ue(redcap);
    ordinary_ue = associate_ue(ordinary);
    unknown_ue = associate_ue(unknown);
    ABTS_TRUE(tc, redcap_ue->redcap_indication);
    ABTS_TRUE(tc, !ordinary_ue->redcap_indication);
    ABTS_TRUE(tc, !unknown_ue->redcap_indication);
    ABTS_INT_EQUAL(tc, OpenAPI_rat_type_NR, f->gnb->rat_type);
    check_sm_requests(tc, redcap_ue, OpenAPI_rat_type_NR_REDCAP);
    check_sm_requests(tc, ordinary_ue, OpenAPI_rat_type_NR);
    check_sm_requests(tc, unknown_ue, OpenAPI_rat_type_NR);
    fixture_clear_ues(f);
}

static void reconnect_and_handover_func(abts_case *tc, void *data)
{
    redcap_fixture_t *f = data;
    ran_ue_t *source, *target, *reconnected;
    amf_ue_t *ue;

    source = initial_ue(tc, f, 1, true, false, NULL);
    ue = associate_ue(source);
    target = ran_ue_add(f->gnb, 2);
    ogs_assert(target);
    source_ue_associate_target_ue(source, target);
    ABTS_TRUE(tc, target->redcap_indication);
    amf_ue_associate_ran_ue(ue, target);
    ABTS_INT_EQUAL(tc, OpenAPI_rat_type_NR_REDCAP, amf_ue_rat_type(ue));
    check_sm_requests(tc, ue, OpenAPI_rat_type_NR_REDCAP);

    /* An indication is per access: a fresh InitialUEMessage replaces it. */
    reconnected = initial_ue(tc, f, 3, false, false, NULL);
    amf_ue_associate_ran_ue(ue, reconnected);
    ABTS_TRUE(tc, !ue->redcap_indication);
    check_sm_requests(tc, ue, OpenAPI_rat_type_NR);
    amf_ue_associate_ran_ue(ue, source);
    ABTS_INT_EQUAL(tc, OpenAPI_rat_type_NR_REDCAP, amf_ue_rat_type(ue));

    /* A stored RedCap capability must not override a non-NR access. */
    f->gnb->rat_type = OpenAPI_rat_type_EUTRA;
    ABTS_INT_EQUAL(tc, OpenAPI_rat_type_EUTRA, amf_ue_rat_type(ue));
    fixture_clear_ues(f);
}

static void cached_context_func(abts_case *tc, void *data)
{
    redcap_fixture_t *f = data;
    ran_ue_t *ran;
    amf_ue_t *ue;

    ran = initial_ue(tc, f, 1, false, false, NULL);
    ue = associate_ue(ran);
    amf_ue_new_guti(ue);
    amf_ue_confirm_guti(ue);

    /* Resolve the existing AMF UE through the actual 5G-S-TMSI path. */
    ran = initial_ue(tc, f, 2, true, false, &ue->current.guti);
    ABTS_INT_EQUAL(tc, ue->id, ran->amf_ue_id);
    ABTS_TRUE(tc, ue->redcap_indication);
    check_sm_requests(tc, ue, OpenAPI_rat_type_NR_REDCAP);

    ran = initial_ue(tc, f, 3, false, false, &ue->current.guti);
    ABTS_INT_EQUAL(tc, ue->id, ran->amf_ue_id);
    ABTS_TRUE(tc, !ue->redcap_indication);
    check_sm_requests(tc, ue, OpenAPI_rat_type_NR);
    fixture_clear_ues(f);
}

abts_suite *test_amf_redcap(abts_suite *suite)
{
    redcap_fixture_t f;

    suite = ADD_SUITE(suite)
    if (suite->tail->not_run)
        return suite;
    fixture_init(&f);
    abts_run_test(suite, initial_and_sbi_func, &f);
    abts_run_test(suite, reconnect_and_handover_func, &f);
    abts_run_test(suite, cached_context_func, &f);
    fixture_final(&f);
    return suite;
}
