/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "sbi-path.h"
#include "pfcp-path.h"

/* Converts PFCP "Usage Report" "Report Trigger" bitmask to Gy "Reporting-Reason" AVP enum value.
 * PFCP: 3GPP TS 29.244 sec 8.2.41
 * Gy: 3GPP TS 32.299 sec 7.2.175 (OGS_DIAM_GY_REPORTING_REASON_*) */
uint32_t smf_pfcp_urr_usage_report_trigger2diam_gy_reporting_reason(ogs_pfcp_usage_report_trigger_t *rep_trigger)
{

    if (rep_trigger->termination_report ||
        rep_trigger->termination_by_up_function_report)
        return OGS_DIAM_GY_REPORTING_REASON_FINAL;

    if (rep_trigger->time_threshold ||
        rep_trigger->volume_threshold)
        return OGS_DIAM_GY_REPORTING_REASON_THRESHOLD;

    if (rep_trigger->time_quota ||
        rep_trigger->volume_quota ||
        rep_trigger->event_quota)
        return OGS_DIAM_GY_REPORTING_REASON_QUOTA_EXHAUSTED;

    if (rep_trigger->quota_validity_time)
        return OGS_DIAM_GY_REPORTING_REASON_VALIDITY_TIME;

    /* if (rep_trigger->immediate_report ||
        rep_trigger->dropped_dl_traffic_threshold ||
        rep_trigger->stop_of_traffic ||
        rep_trigger->start_of_traffic ||
        rep_trigger->quota_holding_time ||
        rep_trigger->periodic_reporting ||
        rep_trigger->event_threshold ||
        rep_trigger->mac_addresses_reporting ||
        rep_trigger->envelope_closure ||
        rep_trigger->monitoring_time ||
        rep_trigger->linked_usage_reporting ||
        rep_trigger->report_the_end_marker_reception ||
        rep_trigger->ip_multicast_join_leave
        ) */
    return OGS_DIAM_GY_REPORTING_REASON_UNUSED_QUOTA_TIMER;
}

static void pfcp_node_fsm_init(ogs_pfcp_node_t *node, bool try_to_assoicate)
{
    smf_event_t e;

    ogs_assert(node);

    memset(&e, 0, sizeof(e));
    e.pfcp_node = node;

    if (try_to_assoicate == true) {
        node->t_association = ogs_timer_add(ogs_app()->timer_mgr,
                smf_timer_pfcp_association, node);
        ogs_assert(node->t_association);
    }

    ogs_fsm_create(&node->sm, smf_pfcp_state_initial, smf_pfcp_state_final);
    ogs_fsm_init(&node->sm, &e);
}

static void pfcp_node_fsm_fini(ogs_pfcp_node_t *node)
{
    smf_event_t e;

    ogs_assert(node);

    memset(&e, 0, sizeof(e));
    e.pfcp_node = node;

    ogs_fsm_fini(&node->sm, &e);
    ogs_fsm_delete(&node->sm);

    if (node->t_association)
        ogs_timer_delete(node->t_association);
}

static void pfcp_recv_cb(short when, ogs_socket_t fd, void *data)
{
    int rv;

    ssize_t size;
    smf_event_t *e = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    ogs_sockaddr_t from;
    ogs_pfcp_node_t *node = NULL;
    ogs_pfcp_header_t *h = NULL;

    ogs_assert(fd != INVALID_SOCKET);

    pkbuf = ogs_pkbuf_alloc(NULL, OGS_MAX_SDU_LEN);
    ogs_assert(pkbuf);
    ogs_pkbuf_put(pkbuf, OGS_MAX_SDU_LEN);

    size = ogs_recvfrom(fd, pkbuf->data, pkbuf->len, 0, &from);
    if (size <= 0) {
        ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                "ogs_recvfrom() failed");
        ogs_pkbuf_free(pkbuf);
        return;
    }

    ogs_pkbuf_trim(pkbuf, size);

    h = (ogs_pfcp_header_t *)pkbuf->data;
    if (h->version > OGS_PFCP_VERSION) {
        ogs_pfcp_header_t rsp;

        ogs_error("Not supported version[%d]", h->version);

        memset(&rsp, 0, sizeof rsp);
        rsp.flags = (OGS_PFCP_VERSION << 5);
        rsp.type = OGS_PFCP_VERSION_NOT_SUPPORTED_RESPONSE_TYPE;
        rsp.length = htobe16(4);
        rsp.sqn_only = h->sqn_only;
        if (ogs_sendto(fd, &rsp, 8, 0, &from) < 0) {
            ogs_log_message(OGS_LOG_ERROR, ogs_socket_errno,
                    "ogs_sendto() failed");
        }
        ogs_pkbuf_free(pkbuf);

        return;
    }

    e = smf_event_new(SMF_EVT_N4_MESSAGE);
    ogs_assert(e);

    node = ogs_pfcp_node_find(&ogs_pfcp_self()->pfcp_peer_list, &from);
    if (!node) {
        node = ogs_pfcp_node_add(&ogs_pfcp_self()->pfcp_peer_list, &from);
        ogs_assert(node);

        node->sock = data;
        pfcp_node_fsm_init(node, false);
    }
    e->pfcp_node = node;
    e->pkbuf = pkbuf;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        ogs_pkbuf_free(e->pkbuf);
        smf_event_free(e);
    }
}

int smf_pfcp_open(void)
{
    ogs_socknode_t *node = NULL;
    ogs_sock_t *sock = NULL;

    /* PFCP Server */
    ogs_list_for_each(&ogs_pfcp_self()->pfcp_list, node) {
        sock = ogs_pfcp_server(node);
        if (!sock) return OGS_ERROR;

        node->poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, sock->fd, pfcp_recv_cb, sock);
        ogs_assert(node->poll);
    }
    ogs_list_for_each(&ogs_pfcp_self()->pfcp_list6, node) {
        sock = ogs_pfcp_server(node);
        if (!sock) return OGS_ERROR;

        node->poll = ogs_pollset_add(ogs_app()->pollset,
                OGS_POLLIN, sock->fd, pfcp_recv_cb, sock);
        ogs_assert(node->poll);
    }

    OGS_SETUP_PFCP_SERVER;

    return OGS_OK;
}

void smf_pfcp_close(void)
{
    ogs_pfcp_node_t *pfcp_node = NULL;

    ogs_list_for_each(&ogs_pfcp_self()->pfcp_peer_list, pfcp_node)
        pfcp_node_fsm_fini(pfcp_node);

    ogs_socknode_remove_all(&ogs_pfcp_self()->pfcp_list);
    ogs_socknode_remove_all(&ogs_pfcp_self()->pfcp_list6);
}

static void sess_5gc_timeout(ogs_pfcp_xact_t *xact, void *data)
{
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;
    ogs_sbi_stream_t *stream = NULL;
    uint8_t type;
    int trigger;
    char *strerror = NULL;

    ogs_assert(xact);
    ogs_assert(data);

    sess = data;
    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    stream = xact->assoc_stream;
    type = xact->seq[0].type;

    switch (type) {
    case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
        ogs_error("No PFCP session establishment response");
        break;
    case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
        strerror = ogs_msprintf("[%s:%d] No PFCP session modification response",
                smf_ue->supi, sess->psi);
        ogs_assert(strerror);

        ogs_error("%s", strerror);
        ogs_assert(stream);
        smf_sbi_send_sm_context_update_error(stream,
                OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT,
                strerror, NULL, NULL, NULL);
        ogs_free(strerror);
        break;
    case OGS_PFCP_SESSION_DELETION_REQUEST_TYPE:
        trigger = xact->delete_trigger;
        ogs_assert(trigger);

        strerror = ogs_msprintf("[%s:%d] No PFCP session deletion response[%d]",
                smf_ue->supi, sess->psi, trigger);
        ogs_assert(strerror);

        ogs_error("%s", strerror);

        if (trigger == OGS_PFCP_DELETE_TRIGGER_LOCAL_INITIATED ||
            trigger == OGS_PFCP_DELETE_TRIGGER_PCF_INITIATED) {

            /* Nothing */

        } else if (trigger == OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED ||
                trigger == OGS_PFCP_DELETE_TRIGGER_AMF_UPDATE_SM_CONTEXT ||
                trigger == OGS_PFCP_DELETE_TRIGGER_AMF_RELEASE_SM_CONTEXT) {

            ogs_assert(stream);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL, strerror, NULL));
        } else {
            ogs_fatal("Unknown trigger [%d]", trigger);
            ogs_assert_if_reached();
        }

        ogs_free(strerror);

        smf_sess_remove(sess);
        break;
    default:
        ogs_error("Not implemented [type:%d]", type);
        break;
    }
}

static void sess_epc_timeout(ogs_pfcp_xact_t *xact, void *data)
{
    uint8_t type;

    ogs_assert(xact);
    type = xact->seq[0].type;

    switch (type) {
    case OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE:
        ogs_warn("No PFCP session establishment response");
        break;
    case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
        ogs_error("No PFCP session modification response");
        break;
    case OGS_PFCP_SESSION_DELETION_REQUEST_TYPE:
        ogs_error("No PFCP session deletion response");
        break;
    default:
        ogs_error("Not implemented [type:%d]", type);
        break;
    }
}

static void bearer_epc_timeout(ogs_pfcp_xact_t *xact, void *data)
{
    uint8_t type;

    ogs_assert(xact);
    type = xact->seq[0].type;

    switch (type) {
    case OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE:
        ogs_error("No PFCP session modification response");
        break;
    default:
        ogs_error("Not implemented [type:%d]", type);
        break;
    }
}

int smf_pfcp_send_modify_list(
        smf_sess_t *sess,
        ogs_pkbuf_t *(*modify_list)(
            uint8_t type, smf_sess_t *sess, ogs_pfcp_xact_t *xact),
        ogs_pfcp_xact_t *xact, ogs_time_t duration)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;

    ogs_assert(sess);
    ogs_assert(xact);

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_MODIFICATION_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = (*modify_list)(h.type, sess, xact);
    ogs_expect_or_return_val(n4buf, OGS_ERROR);

    rv = ogs_pfcp_xact_update_tx(xact, &h, n4buf);
    ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);

    if (duration) {
        ogs_pfcp_xact_delayed_commit(xact, duration);

        return OGS_OK;
    } else {
        rv = ogs_pfcp_xact_commit(xact);
        ogs_expect(rv == OGS_OK);

        return rv;
    }
}

int smf_5gc_pfcp_send_session_establishment_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);
    ogs_assert(stream);

    xact = ogs_pfcp_xact_local_create(sess->pfcp_node, sess_5gc_timeout, sess);
    ogs_expect_or_return_val(xact, OGS_ERROR);

    xact->assoc_stream = stream;
    xact->local_seid = sess->smf_n4_seid;

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = smf_n4_build_session_establishment_request(h.type, sess);
    ogs_expect_or_return_val(n4buf, OGS_ERROR);

    rv = ogs_pfcp_xact_update_tx(xact, &h, n4buf);
    ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_5gc_pfcp_send_all_pdr_modification_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        uint64_t flags, ogs_time_t duration)
{
    int rv;
    ogs_pfcp_xact_t *xact = NULL;
    ogs_pfcp_pdr_t *pdr = NULL;

    ogs_assert(sess);
    if ((flags & OGS_PFCP_MODIFY_ERROR_INDICATION) == 0)
        ogs_assert(stream);

    xact = ogs_pfcp_xact_local_create(sess->pfcp_node, sess_5gc_timeout, sess);
    ogs_expect_or_return_val(xact, OGS_ERROR);

    xact->assoc_stream = stream;
    xact->local_seid = sess->smf_n4_seid;
    xact->modify_flags = flags | OGS_PFCP_MODIFY_SESSION;

    ogs_list_init(&sess->pdr_to_modify_list);
    ogs_list_for_each(&sess->pfcp.pdr_list, pdr)
        ogs_list_add(&sess->pdr_to_modify_list, &pdr->to_modify_node);

    rv = smf_pfcp_send_modify_list(
            sess, smf_n4_build_pdr_to_modify_list, xact, duration);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_5gc_pfcp_send_qos_flow_list_modification_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        uint64_t flags, ogs_time_t duration)
{
    int rv;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);

    xact = ogs_pfcp_xact_local_create(sess->pfcp_node, sess_5gc_timeout, sess);
    ogs_expect_or_return_val(xact, OGS_ERROR);

    xact->assoc_stream = stream;
    xact->local_seid = sess->smf_n4_seid;
    xact->modify_flags = flags | OGS_PFCP_MODIFY_SESSION;

    rv = smf_pfcp_send_modify_list(
            sess, smf_n4_build_qos_flow_to_modify_list, xact, duration);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_5gc_pfcp_send_session_deletion_request(
        smf_sess_t *sess, ogs_sbi_stream_t *stream, int trigger)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);
    ogs_assert(trigger);

    xact = ogs_pfcp_xact_local_create(sess->pfcp_node, sess_5gc_timeout, sess);
    ogs_expect_or_return_val(xact, OGS_ERROR);

    xact->assoc_stream = stream;
    xact->delete_trigger = trigger;
    xact->local_seid = sess->smf_n4_seid;

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_DELETION_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = smf_n4_build_session_deletion_request(h.type, sess);
    ogs_expect_or_return_val(n4buf, OGS_ERROR);

    rv = ogs_pfcp_xact_update_tx(xact, &h, n4buf);
    ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_epc_pfcp_send_session_establishment_request(
        smf_sess_t *sess, void *gtp_xact)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);

    xact = ogs_pfcp_xact_local_create(sess->pfcp_node, sess_epc_timeout, sess);
    ogs_expect_or_return_val(xact, OGS_ERROR);

    xact->epc = true; /* EPC PFCP transaction */
    xact->assoc_xact = gtp_xact;
    xact->local_seid = sess->smf_n4_seid;

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_ESTABLISHMENT_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = smf_n4_build_session_establishment_request(h.type, sess);
    ogs_expect_or_return_val(n4buf, OGS_ERROR);

    rv = ogs_pfcp_xact_update_tx(xact, &h, n4buf);
    ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_epc_pfcp_send_all_pdr_modification_request(
        smf_sess_t *sess, void *gtp_xact, ogs_pkbuf_t *gtpbuf,
        uint64_t flags, uint8_t gtp_pti, uint8_t gtp_cause)
{
    int rv;
    ogs_pfcp_xact_t *xact = NULL;
    ogs_pfcp_pdr_t *pdr = NULL;

    ogs_assert(sess);

    xact = ogs_pfcp_xact_local_create(sess->pfcp_node, sess_epc_timeout, sess);
    ogs_expect_or_return_val(xact, OGS_ERROR);

    xact->epc = true; /* EPC PFCP transaction */
    xact->assoc_xact = gtp_xact;
    xact->local_seid = sess->smf_n4_seid;
    xact->modify_flags = flags | OGS_PFCP_MODIFY_SESSION;

    xact->gtp_pti = gtp_pti;
    xact->gtp_cause = gtp_cause;
    if (gtpbuf) {
        xact->gtpbuf = ogs_pkbuf_copy(gtpbuf);
        ogs_expect_or_return_val(xact->gtpbuf, OGS_ERROR);
    }

    ogs_list_init(&sess->pdr_to_modify_list);
    ogs_list_for_each(&sess->pfcp.pdr_list, pdr)
        ogs_list_add(&sess->pdr_to_modify_list, &pdr->to_modify_node);

    rv = smf_pfcp_send_modify_list(
            sess, smf_n4_build_pdr_to_modify_list, xact, 0);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_epc_pfcp_send_one_bearer_modification_request(
        smf_bearer_t *bearer, void *gtp_xact,
        uint64_t flags, uint8_t gtp_pti, uint8_t gtp_cause)
{
    int rv;
    ogs_pfcp_xact_t *xact = NULL;
    smf_sess_t *sess = NULL;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);

    xact = ogs_pfcp_xact_local_create(
            sess->pfcp_node, bearer_epc_timeout, bearer);
    ogs_expect_or_return_val(xact, OGS_ERROR);

    xact->epc = true; /* EPC PFCP transaction */
    xact->assoc_xact = gtp_xact;
    xact->local_seid = sess->smf_n4_seid;
    xact->modify_flags = flags;

    xact->gtp_pti = gtp_pti;
    xact->gtp_cause = gtp_cause;

    ogs_list_init(&sess->qos_flow_to_modify_list);
    ogs_list_add(&sess->qos_flow_to_modify_list, &bearer->to_modify_node);

    rv = smf_pfcp_send_modify_list(
            sess, smf_n4_build_qos_flow_to_modify_list, xact, 0);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_epc_pfcp_send_session_deletion_request(
        smf_sess_t *sess, void *gtp_xact)
{
    int rv;
    ogs_pkbuf_t *n4buf = NULL;
    ogs_pfcp_header_t h;
    ogs_pfcp_xact_t *xact = NULL;

    ogs_assert(sess);

    xact = ogs_pfcp_xact_local_create(sess->pfcp_node, sess_epc_timeout, sess);
    ogs_expect_or_return_val(xact, OGS_ERROR);

    xact->epc = true; /* EPC PFCP transaction */

    /*
     * << 'gtp_xact' is NOT NULL >>
     *
     * 1. MME sends Delete Session Request to SGW/SMF.
     * 2. SMF sends Delete Session Response to SGW/MME.
     *
     *
     * << 'gtp_xact' should be NULL >>
     *
     * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to SGW/MME.
     * 2. MME sends Delete Bearer Response to SGW/SMF.
     *
     * OR
     *
     * 1. SMF sends Delete Bearer Request(DEFAULT BEARER) to ePDG.
     * 2. ePDG sends Delete Bearer Response(DEFAULT BEARER) to SMF.
     *
     * Note that the following messages are not processed here.
     * - Bearer Resource Command
     * - Delete Bearer Request/Response with DEDICATED BEARER.
     */
    xact->assoc_xact = gtp_xact;
    xact->local_seid = sess->smf_n4_seid;

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_DELETION_REQUEST_TYPE;
    h.seid = sess->upf_n4_seid;

    n4buf = smf_n4_build_session_deletion_request(h.type, sess);
    ogs_expect_or_return_val(n4buf, OGS_ERROR);

    rv = ogs_pfcp_xact_update_tx(xact, &h, n4buf);
    ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}

int smf_epc_pfcp_send_deactivation(smf_sess_t *sess, uint8_t gtp_cause)
{
    int rv;
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *eutran_sess = NULL, *wlan_sess = NULL;

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);

    switch (gtp_cause) {
    case OGS_GTP2_CAUSE_ACCESS_CHANGED_FROM_NON_3GPP_TO_3GPP:
        /* Handover from Non-3GPP to 3GPP */
        wlan_sess = smf_sess_find_by_apn(
                smf_ue, sess->session.name, OGS_GTP2_RAT_TYPE_WLAN);
        ogs_expect_or_return_val(wlan_sess, OGS_ERROR);
        ogs_expect_or_return_val(
                ogs_list_first(&wlan_sess->bearer_list), OGS_ERROR);

        /* Deactivate WLAN Session */
        rv = smf_epc_pfcp_send_all_pdr_modification_request(
                wlan_sess, NULL, NULL,
                OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_DEACTIVATE,
                OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
                OGS_GTP2_CAUSE_ACCESS_CHANGED_FROM_NON_3GPP_TO_3GPP);
        ogs_expect_or_return_val(rv == OGS_OK, rv);
        break;

    case OGS_GTP2_CAUSE_RAT_CHANGED_FROM_3GPP_TO_NON_3GPP:
        /* Handover from 3GPP to Non-3GPP */
        eutran_sess = smf_sess_find_by_apn(
                smf_ue, sess->session.name, OGS_GTP2_RAT_TYPE_EUTRAN);
        if (eutran_sess) {
            ogs_expect_or_return_val(
                    ogs_list_first(&eutran_sess->bearer_list), OGS_ERROR);

            /* Deactivate EUTRAN Session */
            rv = smf_epc_pfcp_send_all_pdr_modification_request(
                    eutran_sess, NULL, NULL,
                    OGS_PFCP_MODIFY_DL_ONLY|OGS_PFCP_MODIFY_DEACTIVATE,
                    OGS_NAS_PROCEDURE_TRANSACTION_IDENTITY_UNASSIGNED,
                    OGS_GTP2_CAUSE_RAT_CHANGED_FROM_3GPP_TO_NON_3GPP);
            ogs_expect_or_return_val(rv == OGS_OK, rv);
        }
        break;

    default:
        ogs_fatal("Invalid GTP-Cause[%d]", gtp_cause);
        ogs_assert_if_reached();
    }

    return OGS_OK;
}

int smf_pfcp_send_session_report_response(
        ogs_pfcp_xact_t *xact, smf_sess_t *sess, uint8_t cause)
{
    int rv;
    ogs_pkbuf_t *sxabuf = NULL;
    ogs_pfcp_header_t h;

    ogs_assert(xact);
    xact->local_seid = sess->smf_n4_seid;

    memset(&h, 0, sizeof(ogs_pfcp_header_t));
    h.type = OGS_PFCP_SESSION_REPORT_RESPONSE_TYPE;
    h.seid = sess->upf_n4_seid;

    sxabuf = ogs_pfcp_build_session_report_response(h.type, cause);
    ogs_expect_or_return_val(sxabuf, OGS_ERROR);

    rv = ogs_pfcp_xact_update_tx(xact, &h, sxabuf);
    ogs_expect_or_return_val(rv == OGS_OK, OGS_ERROR);

    rv = ogs_pfcp_xact_commit(xact);
    ogs_expect(rv == OGS_OK);

    return rv;
}
