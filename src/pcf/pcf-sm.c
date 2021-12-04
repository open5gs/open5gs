/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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
#include "nnrf-handler.h"

void pcf_state_initial(ogs_fsm_t *s, pcf_event_t *e)
{
    pcf_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &pcf_state_operational);
}

void pcf_state_final(ogs_fsm_t *s, pcf_event_t *e)
{
    pcf_sm_debug(e);
}

void pcf_state_operational(ogs_fsm_t *s, pcf_event_t *e)
{
    int rv;

    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *request = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_subscription_t *subscription = NULL;
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_message_t message;

    ogs_sbi_object_t *sbi_object = NULL;
    ogs_sbi_xact_t *sbi_xact = NULL;

    OpenAPI_nf_type_e target_nf_type = OpenAPI_nf_type_NULL;

    pcf_ue_t *pcf_ue = NULL;
    pcf_sess_t *sess = NULL;
    pcf_app_t *app_session = NULL;

    pcf_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case PCF_EVT_SBI_SERVER:
        request = e->sbi.request;
        ogs_assert(request);
        stream = e->sbi.data;
        ogs_assert(stream);

        rv = ogs_sbi_parse_request(&message, request);
        if (rv != OGS_OK) {
            /* 'message' buffer is released in ogs_sbi_parse_request() */
            ogs_error("cannot parse HTTP message");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    NULL, "cannot parse HTTP message", NULL));
            break;
        }

        if (strcmp(message.h.api.version, OGS_SBI_API_V1) != 0) {
            ogs_error("Not supported version [%s]", message.h.api.version);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    &message, "Not supported version", NULL));
            ogs_sbi_message_free(&message);
            break;
        }

        SWITCH(message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY)
                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    pcf_nnrf_handle_nf_status_notify(stream, &message);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message.h.method);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_FORBIDDEN, &message,
                            "Invalid HTTP method", message.h.method));
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &message,
                        "Unknown resource name",
                        message.h.resource.component[0]));
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_AM_POLICY_CONTROL)
            SWITCH(message.h.method)
            CASE(OGS_SBI_HTTP_METHOD_POST)
                if (message.PolicyAssociationRequest &&
                    message.PolicyAssociationRequest->supi) {
                    pcf_ue = pcf_ue_find_by_supi(
                                message.PolicyAssociationRequest->supi);
                    if (!pcf_ue) {
                        pcf_ue = pcf_ue_add(
                            message.PolicyAssociationRequest->supi);
                        ogs_assert(pcf_ue);
                    }
                }
                break;
            CASE(OGS_SBI_HTTP_METHOD_DELETE)
                if (message.h.resource.component[1]) {
                    pcf_ue = pcf_ue_find_by_association_id(
                                message.h.resource.component[1]);
                } else {
                    ogs_error("No Policy Association Id");
                }
                break;
            DEFAULT
            END

            if (!pcf_ue) {
                ogs_error("Not found [%s]", message.h.method);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_NOT_FOUND,
                    &message, "Not found", message.h.method));
                break;
            }

            ogs_assert(OGS_FSM_STATE(&pcf_ue->sm));

            e->pcf_ue = pcf_ue;
            e->sbi.message = &message;
            ogs_fsm_dispatch(&pcf_ue->sm, e);
            if (OGS_FSM_CHECK(&pcf_ue->sm, pcf_am_state_exception)) {
                ogs_error("[%s] State machine exception", pcf_ue->supi);
                pcf_ue_remove(pcf_ue);
            } else if (OGS_FSM_CHECK(&pcf_ue->sm, pcf_am_state_deleted)) {
                ogs_debug("[%s] PCF-AM removed", pcf_ue->supi);
                pcf_ue_remove(pcf_ue);
            }
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)
            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_POLICIES)
                if (!message.h.resource.component[1]) {
                    if (message.SmPolicyContextData &&
                        message.SmPolicyContextData->supi) {
                        pcf_ue = pcf_ue_find_by_supi(
                                    message.SmPolicyContextData->supi);
                        if (pcf_ue) {
                            if (message.SmPolicyContextData->pdu_session_id) {
                                sess = pcf_sess_find_by_psi(pcf_ue, message.
                                        SmPolicyContextData->pdu_session_id);
                                if (!sess) {
                                    sess = pcf_sess_add(pcf_ue, message.
                                        SmPolicyContextData->pdu_session_id);
                                    ogs_assert(sess);
                                    ogs_debug("[%s:%d] PCF session added",
                                                pcf_ue->supi, sess->psi);
                                }
                            }
                        }
                    }
                } else {
                    sess = pcf_sess_find_by_sm_policy_id(
                            message.h.resource.component[1]);
                }
                break;

            DEFAULT
            END

            if (!sess) {
                ogs_error("Not found [%s]", message.h.uri);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_NOT_FOUND,
                        &message, "Not found", message.h.uri));
                break;
            }

            ogs_assert(OGS_FSM_STATE(&sess->sm));

            e->sess = sess;
            e->sbi.message = &message;
            ogs_fsm_dispatch(&sess->sm, e);
            if (OGS_FSM_CHECK(&sess->sm, pcf_sm_state_exception)) {
                ogs_error("[%s:%d] State machine exception",
                        pcf_ue->supi, sess->psi);
                pcf_sess_remove(sess);
            }
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION)
            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_APP_SESSIONS)
                if (!message.h.resource.component[1]) {
                    if (message.AppSessionContext &&
                        message.AppSessionContext->asc_req_data &&
                        (message.AppSessionContext->asc_req_data->ue_ipv4 ||
                         message.AppSessionContext->asc_req_data->ue_ipv6)) {

                        if (!sess &&
                            message.AppSessionContext->asc_req_data->ue_ipv4)
                            sess = pcf_sess_find_by_ipv4addr(message.
                                    AppSessionContext->asc_req_data->ue_ipv4);
                        if (!sess &&
                            message.AppSessionContext->asc_req_data->ue_ipv6)
                            sess = pcf_sess_find_by_ipv6addr(message.
                                    AppSessionContext->asc_req_data->ue_ipv6);
                    }
                } else {
                    app_session = pcf_app_find_by_app_session_id(
                            message.h.resource.component[1]);
                    if (app_session)
                        sess = app_session->sess;
                }
                break;

            DEFAULT
            END

            if (!sess) {
                ogs_error("Not found [%s]", message.h.uri);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_NOT_FOUND,
                        &message, "Not found", message.h.uri));
                break;
            }

            ogs_assert(OGS_FSM_STATE(&sess->sm));

            e->sess = sess;
            e->app = app_session;
            e->sbi.message = &message;
            ogs_fsm_dispatch(&sess->sm, e);
            if (OGS_FSM_CHECK(&sess->sm, pcf_sm_state_exception)) {
                ogs_error("[%s:%d] State machine exception",
                        pcf_ue->supi, sess->psi);
                pcf_sess_remove(sess);
            }
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", message.h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, &message,
                    "Invalid API name", message.h.service.name));
        END

        /* In lib/sbi/server.c, notify_completed() releases 'request' buffer. */
        ogs_sbi_message_free(&message);
        break;

    case PCF_EVT_SBI_CLIENT:
        ogs_assert(e);

        response = e->sbi.response;
        ogs_assert(response);
        rv = ogs_sbi_parse_response(&message, response);
        if (rv != OGS_OK) {
            ogs_error("cannot parse HTTP response");
            ogs_sbi_message_free(&message);
            ogs_sbi_response_free(response);
            break;
        }

        if (strcmp(message.h.api.version, OGS_SBI_API_V1) != 0) {
            ogs_error("Not supported version [%s]", message.h.api.version);
            ogs_sbi_message_free(&message);
            ogs_sbi_response_free(response);
            break;
        }

        SWITCH(message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                nf_instance = e->sbi.data;
                ogs_assert(nf_instance);
                ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

                e->sbi.message = &message;
                ogs_fsm_dispatch(&nf_instance->sm, e);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS)
                subscription = e->sbi.data;
                ogs_assert(subscription);

                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_CREATED ||
                        message.res_status == OGS_SBI_HTTP_STATUS_OK) {
                        pcf_nnrf_handle_nf_status_subscribe(
                                subscription, &message);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                                subscription->id, message.res_status);
                    }
                    break;

                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    if (message.res_status ==
                            OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        ogs_sbi_subscription_remove(subscription);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                                subscription->id, message.res_status);
                    }
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid HTTP method [%s]",
                            subscription->id, message.h.method);
                    ogs_assert_if_reached();
                END
                break;
            
            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NNRF_DISC)
            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                sbi_xact = e->sbi.data;
                ogs_assert(sbi_xact);

                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_GET)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_OK)
                        pcf_nnrf_handle_nf_discover(sbi_xact, &message);
                    else
                        ogs_error("HTTP response error [%d]",
                                message.res_status);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message.h.method);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDR_DR)
            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_POLICY_DATA)
                SWITCH(message.h.resource.component[3])
                CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
                    sbi_xact = e->sbi.data;
                    ogs_assert(sbi_xact);

                    pcf_ue = (pcf_ue_t *)sbi_xact->sbi_object;
                    ogs_assert(pcf_ue);

                    e->sbi.data = sbi_xact->assoc_stream;

                    ogs_sbi_xact_remove(sbi_xact);

                    pcf_ue = pcf_ue_cycle(pcf_ue);
                    ogs_assert(pcf_ue);

                    e->pcf_ue = pcf_ue;
                    e->sbi.message = &message;

                    ogs_fsm_dispatch(&pcf_ue->sm, e);
                    if (OGS_FSM_CHECK(&pcf_ue->sm, pcf_am_state_exception)) {
                        ogs_error("[%s] State machine exception", pcf_ue->supi);
                        pcf_ue_remove(pcf_ue);
                    }
                    break;

                CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
                    sbi_xact = e->sbi.data;
                    ogs_assert(sbi_xact);

                    sess = (pcf_sess_t *)sbi_xact->sbi_object;
                    ogs_assert(sess);

                    e->sbi.data = sbi_xact->assoc_stream;

                    ogs_sbi_xact_remove(sbi_xact);

                    sess = pcf_sess_cycle(sess);
                    ogs_assert(sess);

                    pcf_ue = sess->pcf_ue;
                    ogs_assert(pcf_ue);
                    pcf_ue = pcf_ue_cycle(pcf_ue);
                    ogs_assert(pcf_ue);

                    e->sess = sess;
                    e->sbi.message = &message;

                    ogs_fsm_dispatch(&sess->sm, e);
                    if (OGS_FSM_CHECK(&sess->sm, pcf_sm_state_exception)) {
                        ogs_error("[%s:%d] State machine exception",
                                    pcf_ue->supi, sess->psi);
                        pcf_sess_remove(sess);
                    }
                    break;

                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                            message.h.resource.component[3]);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT)

            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_PCF_BINDINGS)

                sbi_xact = e->sbi.data;
                ogs_assert(sbi_xact);

                sess = (pcf_sess_t *)sbi_xact->sbi_object;
                ogs_assert(sess);

                e->sbi.data = sbi_xact->assoc_stream;

                ogs_sbi_xact_remove(sbi_xact);

                sess = pcf_sess_cycle(sess);
                ogs_assert(sess);

                pcf_ue = sess->pcf_ue;
                ogs_assert(pcf_ue);
                pcf_ue = pcf_ue_cycle(pcf_ue);
                ogs_assert(pcf_ue);

                e->sess = sess;
                e->sbi.message = &message;

                ogs_fsm_dispatch(&sess->sm, e);
                if (OGS_FSM_CHECK(&sess->sm, pcf_sm_state_exception)) {
                    ogs_error("[%s:%d] State machine exception",
                                pcf_ue->supi, sess->psi);
                    pcf_sess_remove(sess);
                } else if (OGS_FSM_CHECK(&sess->sm, pcf_sm_state_deleted)) {
                    ogs_debug("[%s:%d] PCF session removed",
                                pcf_ue->supi, sess->psi);
                    pcf_sess_remove(sess);
                }
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", message.h.service.name);
            ogs_assert_if_reached();
        END

        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        break;

    case PCF_EVT_SBI_TIMER:
        ogs_assert(e);

        switch(e->timer_id) {
        case PCF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        case PCF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        case PCF_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        case PCF_TIMER_NF_INSTANCE_VALIDITY:
            nf_instance = e->sbi.data;
            ogs_assert(nf_instance);
            ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

            ogs_fsm_dispatch(&nf_instance->sm, e);
            if (OGS_FSM_CHECK(&nf_instance->sm, pcf_nf_state_exception))
                ogs_error("[%s] State machine exception [%d]",
                        nf_instance->id, e->timer_id);
            break;

        case PCF_TIMER_SUBSCRIPTION_VALIDITY:
            subscription = e->sbi.data;
            ogs_assert(subscription);

            ogs_assert(true ==
                ogs_nnrf_nfm_send_nf_status_subscribe(subscription->client,
                    pcf_self()->nf_type, subscription->req_nf_instance_id,
                    subscription->subscr_cond.nf_type));

            ogs_info("[%s] Subscription validity expired", subscription->id);
            ogs_sbi_subscription_remove(subscription);
            break;

        case PCF_TIMER_SBI_CLIENT_WAIT:
            sbi_xact = e->sbi.data;
            ogs_assert(sbi_xact);

            sbi_object = sbi_xact->sbi_object;
            ogs_assert(sbi_object);

            stream = sbi_xact->assoc_stream;
            ogs_assert(stream);

            target_nf_type = sbi_xact->target_nf_type;

            ogs_sbi_xact_remove(sbi_xact);

            ogs_assert(sbi_object->type > OGS_SBI_OBJ_BASE &&
                        sbi_object->type < OGS_SBI_OBJ_TOP);

            switch(sbi_object->type) {
            case OGS_SBI_OBJ_UE_TYPE:
                pcf_ue = (pcf_ue_t *)sbi_object;
                ogs_assert(pcf_ue);
                ogs_error("[%s] Cannot receive SBI message", pcf_ue->supi);
                break;

            case OGS_SBI_OBJ_SESS_TYPE:
                sess = (pcf_sess_t *)sbi_object;
                ogs_assert(sess);
                ogs_error("[%d] Cannot receive SBI message", sess->psi);
                break;

            default:
                ogs_fatal("Not implemented [%s:%d]",
                    OpenAPI_nf_type_ToString(target_nf_type), sbi_object->type);
                ogs_assert_if_reached();
            }

            ogs_error("Cannot receive SBI message");
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                    "Cannot receive SBI message", NULL));
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    pcf_timer_get_name(e->timer_id), e->timer_id);
        }
        break;

    default:
        ogs_error("No handler for event %s", pcf_event_get_name(e));
        break;
    }
}
