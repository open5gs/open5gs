/*
 * Copyright (C) 2019-2026 by Arthur SC Chan <arthur.chan@adalogics.com>
 *
 * This file is part of Open5GS.
 *
 * Threat model: N4 PFCP from a peer SMF/UPF. pfcp_message_fuzz only decodes
 * TLVs. xact.c (651), handler.c (unlinked), and build.c (850) are 0-covered
 * in the 20261114 report. This target drives transaction matching and
 * session-rule installation/re-encode. It does not commit transactions
 * (that would send on a socket).
 */

#include <stdint.h>
#include <string.h>

#include "fuzzing.h"
#include "ogs-app.h"
#include "ogs-pfcp.h"

#define kMinInputLength 5
#define kMaxInputLength 2048

static ogs_pfcp_node_t *peer;

static void init_once(void)
{
    ogs_sockaddr_t *addr = NULL;

    if (initialized)
        return;
    initialize();
    ogs_log_install_domain(&__ogs_pfcp_domain, "pfcp", OGS_LOG_NONE);
    ogs_log_install_domain(&__ogs_tlv_domain, "tlv", OGS_LOG_NONE);
    ogs_app_context_init();
    ogs_app()->timer_mgr = ogs_timer_mgr_create(64);
    ogs_app()->pool.nf = 8;
    ogs_app()->pool.sess = 4;
    ogs_app()->pool.xact = 32;
    /* Production loads these from YAML. Zero durations abort in ogs_timer_start. */
    ogs_local_conf()->time.message.duration = ogs_time_from_sec(4);
    ogs_local_conf()->time.message.pfcp.n1_response_rcount = 3;
    ogs_local_conf()->time.message.pfcp.t1_response_duration = ogs_time_from_sec(1);
    ogs_local_conf()->time.message.pfcp.n1_holding_rcount = 1;
    ogs_local_conf()->time.message.pfcp.t1_holding_duration = ogs_time_from_sec(3);
    ogs_local_conf()->time.message.pfcp.association_interval = ogs_time_from_sec(3);
    ogs_local_conf()->time.message.pfcp.no_heartbeat_duration = ogs_time_from_sec(10);
    ogs_pfcp_context_init();
    ogs_pfcp_xact_init();
    ogs_pfcp_pdrbuf_init();
    ogs_list_init(&ogs_pfcp_self()->pfcp_peer_list);
    if (ogs_getaddrinfo(&addr, AF_INET, "127.0.0.1", 8805, 0) == OGS_OK)
        peer = ogs_pfcp_node_new(addr);
    if (peer)
        ogs_list_add(&ogs_pfcp_self()->pfcp_peer_list, peer);
}

static void install_rules(ogs_pfcp_message_t *message)
{
    ogs_pfcp_tlv_create_pdr_t *pdr = NULL;
    ogs_pfcp_tlv_create_far_t *far = NULL;
    ogs_pfcp_tlv_create_qer_t *qer = NULL;
    ogs_pfcp_tlv_create_urr_t *urr = NULL;
    size_t i, pdr_count = 0, far_count = 0, qer_count = 0, urr_count = 0;
    ogs_pfcp_sess_t sess;
    uint8_t cause = 0, offending = 0;

    if (message->h.type == OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE) {
        ogs_pfcp_session_establishment_request_t *req =
            &message->pfcp_session_establishment_request;
        pdr = req->create_pdr;
        pdr_count = OGS_ARRAY_SIZE(req->create_pdr);
        far = req->create_far;
        far_count = OGS_ARRAY_SIZE(req->create_far);
        qer = req->create_qer;
        qer_count = OGS_ARRAY_SIZE(req->create_qer);
        urr = req->create_urr;
        urr_count = OGS_ARRAY_SIZE(req->create_urr);
    } else if (message->h.type == OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE) {
        ogs_pfcp_session_modification_request_t *req =
            &message->pfcp_session_modification_request;
        pdr = req->create_pdr;
        pdr_count = OGS_ARRAY_SIZE(req->create_pdr);
        far = req->create_far;
        far_count = OGS_ARRAY_SIZE(req->create_far);
        qer = req->create_qer;
        qer_count = OGS_ARRAY_SIZE(req->create_qer);
        urr = req->create_urr;
        urr_count = OGS_ARRAY_SIZE(req->create_urr);
    } else {
        return;
    }

    memset(&sess, 0, sizeof(sess));
    ogs_pfcp_pool_init(&sess);
    for (i = 0; i < far_count; i++)
        ogs_pfcp_handle_create_far(&sess, &far[i], &cause, &offending);
    for (i = 0; i < qer_count; i++)
        ogs_pfcp_handle_create_qer(&sess, &qer[i], &cause, &offending);
    for (i = 0; i < urr_count; i++)
        ogs_pfcp_handle_create_urr(&sess, &urr[i], &cause, &offending);
    for (i = 0; i < pdr_count; i++) {
        ogs_pfcp_pdr_t *created;
        ogs_pfcp_tlv_create_pdr_t rebuilt;

        created = ogs_pfcp_handle_create_pdr(
                &sess, &pdr[i], NULL, &cause, &offending);
        if (created && created->far) {
            memset(&rebuilt, 0, sizeof(rebuilt));
            ogs_pfcp_build_create_pdr(&rebuilt, (int)i, created);
        }
    }
    ogs_pfcp_sess_clear(&sess);
    ogs_pfcp_pool_final(&sess);
}

extern int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size)
{
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_pkbuf_t *built = NULL;
    ogs_pfcp_message_t *message = NULL;
    ogs_pfcp_xact_t *xact = NULL;

    if (Size < kMinInputLength || Size > kMaxInputLength)
        return 0;

    init_once();
    if (!ogs_pfcp_self())
        return 0;

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    if (!pkbuf)
        return 0;
    ogs_pkbuf_put_data(pkbuf, Data, Size);

    message = ogs_pfcp_parse_msg(pkbuf);
    if (message) {
        if (peer) {
            xact = NULL;
            if (ogs_pfcp_xact_receive(peer, &message->h, &xact) == OGS_OK &&
                    xact)
                ogs_pfcp_xact_delete(xact);
        }
        install_rules(message);
        ogs_pfcp_message_free(message);
    }

    built = ogs_pfcp_build_heartbeat_request(OGS_PFCP_HEARTBEAT_REQUEST_TYPE);
    if (built)
        ogs_pkbuf_free(built);
    built = ogs_pfcp_build_heartbeat_response(OGS_PFCP_HEARTBEAT_RESPONSE_TYPE);
    if (built)
        ogs_pkbuf_free(built);
    built = ogs_pfcp_build_session_report_response(
            OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE,
            OGS_PFCP_CAUSE_REQUEST_ACCEPTED);
    if (built)
        ogs_pkbuf_free(built);

    ogs_pkbuf_free(pkbuf);
    return 0;
}
