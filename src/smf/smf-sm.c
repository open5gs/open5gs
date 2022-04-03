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

#include "context.h"
#include "gtp-path.h"
#include "fd-path.h"
#include "pfcp-path.h"
#include "sbi-path.h"
#include "s5c-handler.h"
#include "gn-handler.h"
#include "gx-handler.h"
#include "nnrf-handler.h"
#include "namf-handler.h"
#include "npcf-handler.h"

static uint8_t gtp_cause_from_diameter(
        const uint32_t *dia_err, const uint32_t *dia_exp_err)
{
    if (dia_exp_err) {
    }
    if (dia_err) {
        switch (*dia_err) {
        case OGS_DIAM_UNKNOWN_SESSION_ID:
            return OGS_GTP_CAUSE_APN_ACCESS_DENIED_NO_SUBSCRIPTION;
        case ER_DIAMETER_UNABLE_TO_DELIVER:
            return OGS_GTP_CAUSE_REMOTE_PEER_NOT_RESPONDING;
        }
    }

    ogs_error("Unexpected Diameter Result Code %d/%d, defaulting to severe "
              "network failure",
              dia_err ? *dia_err : -1, dia_exp_err ? *dia_exp_err : -1);
    return OGS_GTP_CAUSE_UE_NOT_AUTHORISED_BY_OCS_OR_EXTERNAL_AAA_SERVER;
}

void smf_state_initial(ogs_fsm_t *s, smf_event_t *e)
{
    smf_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &smf_state_operational);
}

void smf_state_final(ogs_fsm_t *s, smf_event_t *e)
{
    smf_sm_debug(e);

    ogs_assert(s);
}

void smf_state_operational(ogs_fsm_t *s, smf_event_t *e)
{
    int rv;
    const char *api_version = NULL;

    ogs_pkbuf_t *recvbuf = NULL;
    smf_sess_t *sess = NULL;
    smf_ue_t *smf_ue = NULL;

    ogs_gtp_node_t *gnode = NULL;
    ogs_gtp_xact_t *gtp_xact = NULL;
    ogs_gtp_message_t gtp_message;
    ogs_gtp1_message_t gtp1_message;

    ogs_diam_gx_message_t *gx_message = NULL;

    ogs_pfcp_node_t *pfcp_node = NULL;
    ogs_pfcp_xact_t *pfcp_xact = NULL;
    ogs_pfcp_message_t pfcp_message;

    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *sbi_request = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_subscription_t *subscription = NULL;
    ogs_sbi_response_t *sbi_response = NULL;
    ogs_sbi_message_t sbi_message;
    ogs_sbi_xact_t *sbi_xact = NULL;

    ogs_nas_5gs_message_t nas_message;
    ogs_pkbuf_t *pkbuf = NULL;

    smf_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case SMF_EVT_S5C_MESSAGE:
        ogs_assert(e);
        recvbuf = e->pkbuf;
        ogs_assert(recvbuf);

        if (ogs_gtp_parse_msg(&gtp_message, recvbuf) != OGS_OK) {
            ogs_error("ogs_gtp_parse_msg() failed");
            ogs_pkbuf_free(recvbuf);
            break;
        }

        if (gtp_message.h.teid != 0) {
            sess = smf_sess_find_by_teid(gtp_message.h.teid);
        }

        if (sess) {
            gnode = sess->gnode;
            ogs_assert(gnode);
        } else {
            gnode = e->gnode;
            ogs_assert(gnode);
        }

        rv = ogs_gtp_xact_receive(gnode, &gtp_message.h, &gtp_xact);
        if (rv != OGS_OK) {
            ogs_pkbuf_free(recvbuf);
            break;
        }

        switch(gtp_message.h.type) {
        case OGS_GTP_ECHO_REQUEST_TYPE:
            smf_s5c_handle_echo_request(gtp_xact, &gtp_message.echo_request);
            break;
        case OGS_GTP_ECHO_RESPONSE_TYPE:
            smf_s5c_handle_echo_response(gtp_xact, &gtp_message.echo_response);
            break;
        case OGS_GTP_CREATE_SESSION_REQUEST_TYPE:
            if (gtp_message.h.teid == 0) {
                ogs_expect(!sess);
                sess = smf_sess_add_by_gtp_message(&gtp_message);
                if (sess)
                    OGS_SETUP_GTP_NODE(sess, gnode);
            }
            smf_s5c_handle_create_session_request(
                sess, gtp_xact, &gtp_message.create_session_request);
            break;
        case OGS_GTP_DELETE_SESSION_REQUEST_TYPE:
            smf_s5c_handle_delete_session_request(
                sess, gtp_xact, &gtp_message.delete_session_request);
            break;
        case OGS_GTP_MODIFY_BEARER_REQUEST_TYPE:
            smf_s5c_handle_modify_bearer_request(
                sess, gtp_xact, &gtp_message.modify_bearer_request);
            break;
        case OGS_GTP_CREATE_BEARER_RESPONSE_TYPE:
            smf_s5c_handle_create_bearer_response(
                sess, gtp_xact, &gtp_message.create_bearer_response);
            break;
        case OGS_GTP_UPDATE_BEARER_RESPONSE_TYPE:
            smf_s5c_handle_update_bearer_response(
                sess, gtp_xact, &gtp_message.update_bearer_response);
            break;
        case OGS_GTP_DELETE_BEARER_RESPONSE_TYPE:
            smf_s5c_handle_delete_bearer_response(
                sess, gtp_xact, &gtp_message.delete_bearer_response);
            break;
        case OGS_GTP_BEARER_RESOURCE_COMMAND_TYPE:
            smf_s5c_handle_bearer_resource_command(
                sess, gtp_xact, &gtp_message.bearer_resource_command);
            break;
        default:
            ogs_warn("Not implmeneted(type:%d)", gtp_message.h.type);
            break;
        }
        ogs_pkbuf_free(recvbuf);
        break;

    case SMF_EVT_GN_MESSAGE:
        ogs_assert(e);
        recvbuf = e->pkbuf;
        ogs_assert(recvbuf);

        if (ogs_gtp1_parse_msg(&gtp1_message, recvbuf) != OGS_OK) {
            ogs_error("ogs_gtp_parse_msg() failed");
            ogs_pkbuf_free(recvbuf);
            break;
        }

        if (gtp1_message.h.teid != 0) {
            sess = smf_sess_find_by_teid(gtp1_message.h.teid);
        }

        if (sess) {
            gnode = sess->gnode;
            ogs_assert(gnode);
        } else {
            gnode = e->gnode;
            ogs_assert(gnode);
        }

        rv = ogs_gtp1_xact_receive(gnode, &gtp1_message.h, &gtp_xact);
        if (rv != OGS_OK) {
            ogs_pkbuf_free(recvbuf);
            break;
        }

        switch(gtp1_message.h.type) {
        case OGS_GTP1_ECHO_REQUEST_TYPE:
            smf_gn_handle_echo_request(gtp_xact, &gtp1_message.echo_request);
            break;
        case OGS_GTP1_ECHO_RESPONSE_TYPE:
            smf_gn_handle_echo_response(gtp_xact, &gtp1_message.echo_response);
            break;
        case OGS_GTP1_CREATE_PDP_CONTEXT_REQUEST_TYPE:
            if (gtp1_message.h.teid == 0) {
                ogs_expect(!sess);
                sess = smf_sess_add_by_gtp1_message(&gtp1_message);
                if (sess)
                    OGS_SETUP_GTP_NODE(sess, gnode);
            }
            smf_gn_handle_create_pdp_context_request(
                sess, gtp_xact, &gtp1_message.create_pdp_context_request);
            break;
        case OGS_GTP1_DELETE_PDP_CONTEXT_REQUEST_TYPE:
            smf_gn_handle_delete_pdp_context_request(
                sess, gtp_xact, &gtp1_message.delete_pdp_context_request);
            break;
        case OGS_GTP1_UPDATE_PDP_CONTEXT_REQUEST_TYPE:
            smf_gn_handle_update_pdp_context_request(
                sess, gtp_xact, &gtp1_message.update_pdp_context_request);
            break;
        case OGS_GTP1_ERROR_INDICATION_TYPE:
            /* TS 29.060 10.1.1.4 dst port shall be the user plane port (2152) */
            ogs_error("Rx unexpected Error Indication in GTPC port");
            break;
        default:
            ogs_warn("Not implmeneted(type:%d)", gtp1_message.h.type);
            break;
        }
        ogs_pkbuf_free(recvbuf);
        break;

    case SMF_EVT_GX_MESSAGE:
        ogs_assert(e);
        gx_message = e->gx_message;
        ogs_assert(gx_message);

        sess = e->sess;
        ogs_assert(sess);

        switch(gx_message->cmd_code) {
        case OGS_DIAM_GX_CMD_CODE_CREDIT_CONTROL:
            gtp_xact = e->gtp_xact;
            ogs_assert(gtp_xact);

            if (gx_message->result_code != ER_DIAMETER_SUCCESS) {
                uint8_t cause_value = gtp_cause_from_diameter(
                    gx_message->err, gx_message->exp_err);

                if (gtp_xact->gtp_version == 1)
                    ogs_gtp1_send_error_message(
                        gtp_xact, sess ? sess->sgw_s5c_teid : 0,
                        OGS_GTP1_CREATE_PDP_CONTEXT_RESPONSE_TYPE, cause_value);
                else
                    ogs_gtp_send_error_message(
                        gtp_xact, sess ? sess->sgw_s5c_teid : 0,
                        OGS_GTP_CREATE_SESSION_RESPONSE_TYPE, cause_value);

                break;
            }

            switch(gx_message->cc_request_type) {
            case OGS_DIAM_GX_CC_REQUEST_TYPE_INITIAL_REQUEST:
                smf_gx_handle_cca_initial_request(
                        sess, gx_message, gtp_xact);
                break;
            case OGS_DIAM_GX_CC_REQUEST_TYPE_TERMINATION_REQUEST:
                smf_gx_handle_cca_termination_request(
                        sess, gx_message, gtp_xact);
                break;
            default:
                ogs_error("Not implemented(%d)", gx_message->cc_request_type);
                break;
            }

            break;
        case OGS_DIAM_GX_CMD_RE_AUTH:
            smf_gx_handle_re_auth_request(sess, gx_message);
            break;
        default:
            ogs_error("Invalid type(%d)", gx_message->cmd_code);
            break;
        }

        ogs_session_data_free(&gx_message->session_data);
        ogs_free(gx_message);
        break;
    case SMF_EVT_N4_MESSAGE:
        ogs_assert(e);
        recvbuf = e->pkbuf;
        ogs_assert(recvbuf);
        pfcp_node = e->pfcp_node;
        ogs_assert(pfcp_node);
        ogs_assert(OGS_FSM_STATE(&pfcp_node->sm));

        if (ogs_pfcp_parse_msg(&pfcp_message, recvbuf) != OGS_OK) {
            ogs_error("ogs_pfcp_parse_msg() failed");
            ogs_pkbuf_free(recvbuf);
            break;
        }

        rv = ogs_pfcp_xact_receive(pfcp_node, &pfcp_message.h, &pfcp_xact);
        if (rv != OGS_OK) {
            ogs_pkbuf_free(recvbuf);
            break;
        }

        e->pfcp_message = &pfcp_message;
        e->pfcp_xact = pfcp_xact;
        ogs_fsm_dispatch(&pfcp_node->sm, e);
        if (OGS_FSM_CHECK(&pfcp_node->sm, smf_pfcp_state_exception)) {
            ogs_error("PFCP state machine exception");
            break;
        }

        ogs_pkbuf_free(recvbuf);
        break;
    case SMF_EVT_N4_TIMER:
    case SMF_EVT_N4_NO_HEARTBEAT:
        ogs_assert(e);
        pfcp_node = e->pfcp_node;
        ogs_assert(pfcp_node);
        ogs_assert(OGS_FSM_STATE(&pfcp_node->sm));

        ogs_fsm_dispatch(&pfcp_node->sm, e);
        break;

    case SMF_EVT_SBI_SERVER:
        sbi_request = e->sbi.request;
        ogs_assert(sbi_request);
        stream = e->sbi.data;
        ogs_assert(stream);

        rv = ogs_sbi_parse_request(&sbi_message, sbi_request);
        if (rv != OGS_OK) {
            /* 'sbi_message' buffer is released in ogs_sbi_parse_request() */
            ogs_error("cannot parse HTTP sbi_message");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    NULL, "cannot parse HTTP sbi_message", NULL));
            break;
        }

        SWITCH(sbi_message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            api_version = OGS_SBI_API_V2;
            break;
        DEFAULT
            api_version = OGS_SBI_API_V1;
        END

        ogs_assert(api_version);
        if (strcmp(sbi_message.h.api.version, api_version) != 0) {
            ogs_error("Not supported version [%s]", sbi_message.h.api.version);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    &sbi_message, "Not supported version", NULL));
            ogs_sbi_message_free(&sbi_message);
            break;
        }

        SWITCH(sbi_message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(sbi_message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY)
                SWITCH(sbi_message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    smf_nnrf_handle_nf_status_notify(stream, &sbi_message);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", sbi_message.h.method);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_FORBIDDEN, &sbi_message,
                            "Invalid HTTP method", sbi_message.h.method));
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                        "Invalid resource name",
                        sbi_message.h.resource.component[0]));
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            SWITCH(sbi_message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_CONTEXTS)
                SWITCH(sbi_message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    SWITCH(sbi_message.h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                    CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                        if (!sbi_message.h.resource.component[1]) {
                            ogs_error("No smContextRef [%s]",
                                    sbi_message.h.resource.component[1]);
                            smf_sbi_send_sm_context_update_error(stream,
                                    OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                                    "No smContextRef",
                                    sbi_message.h.resource.component[1],
                                    NULL, NULL);
                            break;
                        }

                        sess = smf_sess_find_by_sm_context_ref(
                                sbi_message.h.resource.component[1]);

                        if (!sess) {
                            ogs_warn("Not found [%s]", sbi_message.h.uri);
                            smf_sbi_send_sm_context_update_error(stream,
                                    OGS_SBI_HTTP_STATUS_NOT_FOUND, "Not found",
                                    sbi_message.h.uri, NULL, NULL);
                        }
                        break;

                    DEFAULT
                        sess = smf_sess_add_by_sbi_message(&sbi_message);
                        ogs_assert(sess);
                    END
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", sbi_message.h.method);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                            "Invalid HTTP method", sbi_message.h.method));
                    break;
                END

                if (sess) {
                    smf_ue = sess->smf_ue;
                    ogs_assert(smf_ue);
                    ogs_assert(OGS_FSM_STATE(&sess->sm));

                    e->sess = sess;
                    e->sbi.message = &sbi_message;
                    ogs_fsm_dispatch(&sess->sm, e);
                    if (OGS_FSM_CHECK(&sess->sm, smf_gsm_state_exception)) {
                        ogs_error("[%s] State machine exception", smf_ue->supi);
                        SMF_SESS_CLEAR(sess);
                    }
                }
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                        "Invalid resource name",
                        sbi_message.h.resource.component[0]));
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NSMF_CALLBACK)
            SWITCH(sbi_message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_N1_N2_FAILURE_NOTIFY)
                smf_namf_comm_handle_n1_n2_message_transfer_failure_notify(
                        stream, &sbi_message);
                break;
            CASE(OGS_SBI_RESOURCE_NAME_SM_POLICY_NOTIFY)
                if (!sbi_message.h.resource.component[1]) {
                    ogs_error("No smContextRef [%s]",
                            sbi_message.h.resource.component[1]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                            "No smContextRef",
                            sbi_message.h.resource.component[1]));
                    break;
                }

                sess = smf_sess_find_by_sm_context_ref(
                        sbi_message.h.resource.component[1]);

                if (!sess) {
                    ogs_warn("Not found [%s]", sbi_message.h.uri);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_NOT_FOUND, &sbi_message,
                            "Not found",
                            sbi_message.h.resource.component[1]));
                    break;
                }

                SWITCH(sbi_message.h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_UPDATE)
                    smf_npcf_smpolicycontrol_handle_update_notify(
                            sess, stream, &sbi_message);
                    break;
                CASE(OGS_SBI_RESOURCE_NAME_TERMINATE)
                    smf_npcf_smpolicycontrol_handle_terminate_notify(
                            sess, stream, &sbi_message);
                    break;
                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                            sbi_message.h.resource.component[0]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                            "Invalid resource name",
                            sbi_message.h.resource.component[0]));
                END
                break;
            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                        "Invalid resource name",
                        sbi_message.h.resource.component[0]));
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", sbi_message.h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, &sbi_message,
                    "Invalid API name", sbi_message.h.service.name));
        END

        /* In lib/sbi/server.c, notify_completed() releases 'request' buffer. */
        ogs_sbi_message_free(&sbi_message);
        break;

    case SMF_EVT_SBI_CLIENT:
        ogs_assert(e);

        sbi_response = e->sbi.response;
        ogs_assert(sbi_response);
        rv = ogs_sbi_parse_response(&sbi_message, sbi_response);
        if (rv != OGS_OK) {
            ogs_error("cannot parse HTTP response");
            ogs_sbi_message_free(&sbi_message);
            ogs_sbi_response_free(sbi_response);
            break;
        }

        SWITCH(sbi_message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            api_version = OGS_SBI_API_V2;
            break;
        DEFAULT
            api_version = OGS_SBI_API_V1;
        END

        ogs_assert(api_version);
        if (strcmp(sbi_message.h.api.version, api_version) != 0) {
            ogs_error("Not supported version [%s]", sbi_message.h.api.version);
            ogs_sbi_message_free(&sbi_message);
            ogs_sbi_response_free(sbi_response);
            break;
        }

        SWITCH(sbi_message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(sbi_message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                nf_instance = e->sbi.data;
                ogs_assert(nf_instance);
                ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

                e->sbi.message = &sbi_message;
                ogs_fsm_dispatch(&nf_instance->sm, e);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS)
                subscription = e->sbi.data;
                ogs_assert(subscription);

                SWITCH(sbi_message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (sbi_message.res_status == OGS_SBI_HTTP_STATUS_CREATED ||
                        sbi_message.res_status == OGS_SBI_HTTP_STATUS_OK) {
                        smf_nnrf_handle_nf_status_subscribe(
                                subscription, &sbi_message);
                    } else {
                        ogs_error("HTTP response error : %d",
                                sbi_message.res_status);
                    }
                    break;

                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    if (sbi_message.res_status ==
                            OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        ogs_sbi_subscription_remove(subscription);
                    } else {
                        ogs_error("HTTP response error : %d",
                                sbi_message.res_status);
                    }
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", sbi_message.h.method);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NNRF_DISC)
            SWITCH(sbi_message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                sbi_xact = e->sbi.data;
                ogs_assert(sbi_xact);

                SWITCH(sbi_message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_GET)
                    if (sbi_message.res_status == OGS_SBI_HTTP_STATUS_OK)
                        smf_nnrf_handle_nf_discover(sbi_xact, &sbi_message);
                    else
                        ogs_error("HTTP response error [%d]",
                                sbi_message.res_status);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", sbi_message.h.method);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        sbi_message.h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
        CASE(OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)
        CASE(OGS_SBI_SERVICE_NAME_NAMF_COMM)
            sbi_xact = e->sbi.data;
            ogs_assert(sbi_xact);

            sess = (smf_sess_t *)sbi_xact->sbi_object;
            ogs_assert(sess);

            e->sbi.data = sbi_xact->assoc_stream;
            e->sbi.state = sbi_xact->state;

            ogs_sbi_xact_remove(sbi_xact);

            sess = smf_sess_cycle(sess);
            ogs_assert(sess);
            smf_ue = sess->smf_ue;
            ogs_assert(smf_ue);
            smf_ue = smf_ue_cycle(smf_ue);
            ogs_assert(smf_ue);
            ogs_assert(OGS_FSM_STATE(&sess->sm));

            e->sess = sess;
            e->sbi.message = &sbi_message;

            ogs_fsm_dispatch(&sess->sm, e);
            if (OGS_FSM_CHECK(&sess->sm, smf_gsm_state_exception)) {
                ogs_error("[%s] State machine exception", smf_ue->supi);
                SMF_SESS_CLEAR(sess);
            } else if (OGS_FSM_CHECK(
                        &sess->sm, smf_gsm_state_session_will_release)) {
                SMF_SESS_CLEAR(sess);
            }
            break;

        DEFAULT
            ogs_error("Invalid service name [%s]", sbi_message.h.service.name);
            ogs_assert_if_reached();
        END

        ogs_sbi_message_free(&sbi_message);
        ogs_sbi_response_free(sbi_response);
        break;

    case SMF_EVT_SBI_TIMER:
        ogs_assert(e);

        switch(e->timer_id) {
        case SMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        case SMF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        case SMF_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        case SMF_TIMER_NF_INSTANCE_VALIDITY:
            nf_instance = e->sbi.data;
            ogs_assert(nf_instance);
            ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

            ogs_fsm_dispatch(&nf_instance->sm, e);
            if (OGS_FSM_CHECK(&nf_instance->sm, smf_nf_state_exception))
                ogs_error("State machine exception [%d]", e->timer_id);
            break;

        case SMF_TIMER_SUBSCRIPTION_VALIDITY:
            subscription = e->sbi.data;
            ogs_assert(subscription);

            ogs_assert(true ==
                ogs_nnrf_nfm_send_nf_status_subscribe(subscription->client,
                    smf_self()->nf_type, subscription->req_nf_instance_id,
                    subscription->subscr_cond.nf_type));

            ogs_info("Subscription validity expired [%s]", subscription->id);
            ogs_sbi_subscription_remove(subscription);
            break;

        case SMF_TIMER_SBI_CLIENT_WAIT:
            sbi_xact = e->sbi.data;
            ogs_assert(sbi_xact);

            stream = sbi_xact->assoc_stream;
            /* Here, we should not use ogs_assert(stream)
             * since 'namf-comm' service has no an associated stream. */

            ogs_sbi_xact_remove(sbi_xact);

            ogs_error("Cannot receive SBI message");
            if (stream) {
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                        "Cannot receive SBI message", NULL));
            }
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    smf_timer_get_name(e->timer_id), e->timer_id);
        }
        break;

    case SMF_EVT_5GSM_MESSAGE:
        sess = e->sess;
        ogs_assert(sess);
        stream = e->sbi.data;
        ogs_assert(stream);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);

        if (ogs_nas_5gsm_decode(&nas_message, pkbuf) != OGS_OK) {
            ogs_error("ogs_nas_5gsm_decode() failed");
            ogs_pkbuf_free(pkbuf);
            return;
        }

        ogs_assert(sess);
        ogs_assert(OGS_FSM_STATE(&sess->sm));

        sess->pti = nas_message.gsm.h.procedure_transaction_identity;

        switch (nas_message.gsm.h.message_type) {
        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
            ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));
            ogs_assert(true == smf_sbi_send_sm_context_status_notify(sess));
            SMF_SESS_CLEAR(sess);
            break;
        default:
            e->nas.message = &nas_message;
            ogs_fsm_dispatch(&sess->sm, e);
            if (OGS_FSM_CHECK(&sess->sm, smf_gsm_state_exception)) {
                ogs_error("State machine exception");
                SMF_SESS_CLEAR(sess);
            }
        }

        ogs_pkbuf_free(pkbuf);
        break;

    case SMF_EVT_NGAP_MESSAGE:
        sess = e->sess;
        ogs_assert(sess);
        stream = e->sbi.data;
        ogs_assert(stream);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);
        ogs_assert(e->ngap.type);

        ogs_assert(sess);
        ogs_assert(OGS_FSM_STATE(&sess->sm));

        ogs_fsm_dispatch(&sess->sm, e);
        if (OGS_FSM_CHECK(&sess->sm, smf_gsm_state_exception)) {
            ogs_error("State machine exception");
            SMF_SESS_CLEAR(sess);
        }

        ogs_pkbuf_free(pkbuf);
        break;

    default:
        ogs_error("No handler for event %s", smf_event_get_name(e));
        break;
    }
}
