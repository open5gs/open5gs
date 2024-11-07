/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_request_t *request = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_subscription_data_t *subscription_data = NULL;
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_message_t message;

    ogs_sbi_object_t *sbi_object = NULL;
    ogs_sbi_xact_t *sbi_xact = NULL;
    ogs_pool_id_t sbi_xact_id = OGS_INVALID_POOL_ID;

    ogs_sbi_service_type_e service_type = OGS_SBI_SERVICE_TYPE_NULL;

    pcf_ue_t *pcf_ue = NULL;
    ogs_pool_id_t pcf_ue_id = OGS_INVALID_POOL_ID;
    pcf_sess_t *sess = NULL;
    ogs_pool_id_t sess_id = OGS_INVALID_POOL_ID;
    pcf_app_t *app_session = NULL;

    pcf_sm_debug(e);

    ogs_assert(s);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case OGS_EVENT_SBI_SERVER:
        request = e->h.sbi.request;
        ogs_assert(request);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        rv = ogs_sbi_parse_request(&message, request);
        if (rv != OGS_OK) {
            /* 'message' buffer is released in ogs_sbi_parse_request() */
            ogs_error("cannot parse HTTP message");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    NULL, "cannot parse HTTP message", NULL, NULL));
            break;
        }

        if (strcmp(message.h.api.version, OGS_SBI_API_V1) != 0) {
            ogs_error("Not supported version [%s]", message.h.api.version);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    &message, "Not supported version", NULL, NULL));
            ogs_sbi_message_free(&message);
            break;
        }

        SWITCH(message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY)
                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    ogs_nnrf_nfm_handle_nf_status_notify(stream, &message);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message.h.method);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_FORBIDDEN, &message,
                            "Invalid HTTP method", message.h.method, NULL));
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &message,
                        "Unknown resource name",
                        message.h.resource.component[0], NULL));
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
                    &message, "Not found", message.h.method, NULL));
                break;
            }

            ogs_assert(OGS_FSM_STATE(&pcf_ue->sm));

            e->pcf_ue_id = pcf_ue->id;
            e->h.sbi.message = &message;
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
                        message.SmPolicyContextData->supi &&
                        message.SmPolicyContextData->pdu_session_id) {

                        pcf_ue = pcf_ue_find_by_supi(
                                    message.SmPolicyContextData->supi);
                        if (!pcf_ue) {
                            if (!strcmp(message.h.method,
                                        OGS_SBI_HTTP_METHOD_POST)) {
                                pcf_ue = pcf_ue_add(
                                            message.SmPolicyContextData->supi);
                                if (!pcf_ue) {
                                    ogs_error("[%s:%d] Invalid Request [%s]",
                                            message.SmPolicyContextData->supi,
                                            message.SmPolicyContextData->
                                                pdu_session_id,
                                            message.h.method);
                                } else
                                    ogs_debug("[%s:%d] PCF UE added",
                                        message.SmPolicyContextData->supi,
                                        message.SmPolicyContextData->
                                            pdu_session_id);
                            } else {
                                ogs_error("[%s:%d] Invalid HTTP method [%s]",
                                        message.SmPolicyContextData->supi,
                                        message.SmPolicyContextData->
                                            pdu_session_id,
                                        message.h.method);
                            }
                        }

                        if (pcf_ue) {
                            sess = pcf_sess_find_by_psi(pcf_ue, message.
                                    SmPolicyContextData->pdu_session_id);
                            if (!sess) {
                                if (!strcmp(message.h.method,
                                            OGS_SBI_HTTP_METHOD_POST)) {
                                    sess = pcf_sess_add(pcf_ue, message.
                                        SmPolicyContextData->pdu_session_id);
                                    if (!sess) {
                                        ogs_error("[%s:%d] "
                                                "Invalid Request [%s]",
                                                message.SmPolicyContextData->
                                                    supi,
                                                message.SmPolicyContextData->
                                                    pdu_session_id,
                                                message.h.method);
                                    } else
                                        ogs_debug("[%s:%d] PCF session added",
                                                    pcf_ue->supi, sess->psi);
                                } else {
                                    ogs_error("[%s:%d] "
                                            "Invalid HTTP method [%s]",
                                            message.SmPolicyContextData->supi,
                                            message.SmPolicyContextData->
                                                pdu_session_id,
                                            message.h.method);
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
                /*
                 * TS29.512
                 * 4.2.2.2 SM Policy Association establishment
                 *
                 * If the user information received within the "supi" attribute is
                 * unknown, the PCF shall reject the request with an HTTP "400 Bad
                 * Request" response message including the "cause" attribute
                 * of the ProblemDetails data structure set to "USER_UNKNOWN".
                 */
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        &message, "Not found", message.h.uri, "USER_UNKNOWN"));
                break;
            }

            ogs_assert(OGS_FSM_STATE(&sess->sm));

            e->sess_id = sess->id;
            e->h.sbi.message = &message;
            ogs_fsm_dispatch(&sess->sm, e);
            if (OGS_FSM_CHECK(&sess->sm, pcf_sm_state_exception)) {
                /* Clang scan-build SA: NULL pointer dereference: pcf_ue=NULL, remove logging of pcf_ue->supi. */
                ogs_error("[%s:%d] State machine exception",
                        pcf_ue ? pcf_ue->supi : "Unknown", sess->psi);
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
                        &message, "Not found", message.h.uri, NULL));
                break;
            }

            ogs_assert(OGS_FSM_STATE(&sess->sm));

            e->sess_id = sess->id;
            e->app = app_session;
            e->h.sbi.message = &message;
            ogs_fsm_dispatch(&sess->sm, e);
            if (OGS_FSM_CHECK(&sess->sm, pcf_sm_state_exception)) {
                /* Clang scan-build SA: NULL pointer dereference: pcf_ue=NULL, remove logging of pcf_ue->supi. */
                ogs_error("[%s:%d] State machine exception",
                        pcf_ue ? pcf_ue->supi : "Unknown", sess->psi);
                pcf_sess_remove(sess);
            }
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", message.h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, &message,
                    "Invalid API name", message.h.service.name, NULL));
        END

        /* In lib/sbi/server.c, notify_completed() releases 'request' buffer. */
        ogs_sbi_message_free(&message);
        break;

    case OGS_EVENT_SBI_CLIENT:
        ogs_assert(e);

        response = e->h.sbi.response;
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
                nf_instance = e->h.sbi.data;
                ogs_assert(nf_instance);
                ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

                e->h.sbi.message = &message;
                ogs_fsm_dispatch(&nf_instance->sm, e);
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS)
                subscription_data = e->h.sbi.data;
                ogs_assert(subscription_data);

                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_CREATED ||
                        message.res_status == OGS_SBI_HTTP_STATUS_OK) {
                        ogs_nnrf_nfm_handle_nf_status_subscribe(
                                subscription_data, &message);
                    } else {
                        ogs_error("HTTP response error : %d",
                                message.res_status);
                    }
                    break;

                CASE(OGS_SBI_HTTP_METHOD_PATCH)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_OK ||
                        message.res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        ogs_nnrf_nfm_handle_nf_status_update(
                                subscription_data, &message);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                                subscription_data->id ?
                                    subscription_data->id : "Unknown",
                                message.res_status);
                    }
                    break;

                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    if (message.res_status ==
                            OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        ogs_sbi_subscription_data_remove(subscription_data);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                                subscription_data->id ?
                                    subscription_data->id : "Unknown",
                                message.res_status);
                    }
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid HTTP method [%s]",
                            subscription_data->id, message.h.method);
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
                sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
                ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                        sbi_xact_id <= OGS_MAX_POOL_ID);

                sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
                if (!sbi_xact) {
                    /* CLIENT_WAIT timer could remove SBI transaction
                     * before receiving SBI message */
                    ogs_error("SBI transaction has already been removed [%d]",
                            sbi_xact_id);
                    break;
                }

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
                    sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
                    ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                            sbi_xact_id <= OGS_MAX_POOL_ID);

                    sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
                    if (!sbi_xact) {
                        /* CLIENT_WAIT timer could remove SBI transaction
                         * before receiving SBI message */
                        ogs_error(
                                "SBI transaction has already been removed [%d]",
                                sbi_xact_id);
                        break;
                    }

                    pcf_ue_id = sbi_xact->sbi_object_id;
                    ogs_assert(pcf_ue_id >= OGS_MIN_POOL_ID &&
                            pcf_ue_id <= OGS_MAX_POOL_ID);

                    if (sbi_xact->assoc_stream_id >= OGS_MIN_POOL_ID &&
                        sbi_xact->assoc_stream_id <= OGS_MAX_POOL_ID)
                        e->h.sbi.data =
                            OGS_UINT_TO_POINTER(sbi_xact->assoc_stream_id);

                    ogs_sbi_xact_remove(sbi_xact);

                    pcf_ue = pcf_ue_find_by_id(pcf_ue_id);
                    if (!pcf_ue) {
                        ogs_error("UE(pcf_ue) Context "
                                    "has already been removed");
                        break;
                    }

                    e->pcf_ue_id = pcf_ue->id;
                    e->h.sbi.message = &message;

                    ogs_fsm_dispatch(&pcf_ue->sm, e);
                    if (OGS_FSM_CHECK(&pcf_ue->sm, pcf_am_state_exception)) {
                        ogs_error("[%s] State machine exception", pcf_ue->supi);
                        pcf_ue_remove(pcf_ue);
                    }
                    break;

                CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
                    sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
                    ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                            sbi_xact_id <= OGS_MAX_POOL_ID);

                    sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
                    if (!sbi_xact) {
                        /* CLIENT_WAIT timer could remove SBI transaction
                         * before receiving SBI message */
                        ogs_error(
                                "SBI transaction has already been removed [%d]",
                                sbi_xact_id);
                        break;
                    }

                    sess_id = sbi_xact->sbi_object_id;
                    ogs_assert(sess_id >= OGS_MIN_POOL_ID &&
                            sess_id <= OGS_MAX_POOL_ID);

                    if (sbi_xact->assoc_stream_id >= OGS_MIN_POOL_ID &&
                        sbi_xact->assoc_stream_id <= OGS_MAX_POOL_ID)
                        e->h.sbi.data =
                            OGS_UINT_TO_POINTER(sbi_xact->assoc_stream_id);

                    ogs_sbi_xact_remove(sbi_xact);

                    sess = pcf_sess_find_by_id(sess_id);
                    if (!sess) {
                        ogs_error("Session has already been removed");
                        break;
                    }

                    pcf_ue = pcf_ue_find_by_id(sess->pcf_ue_id);
                    ogs_assert(pcf_ue);

                    e->sess_id = sess->id;
                    e->h.sbi.message = &message;

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
                sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
                ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                        sbi_xact_id <= OGS_MAX_POOL_ID);

                sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
                if (!sbi_xact) {
                    /* CLIENT_WAIT timer could remove SBI transaction
                     * before receiving SBI message */
                    ogs_error("SBI transaction has already been removed [%d]",
                            sbi_xact_id);
                    break;
                }

                sess_id = sbi_xact->sbi_object_id;
                ogs_assert(sess_id >= OGS_MIN_POOL_ID &&
                        sess_id <= OGS_MAX_POOL_ID);

                if (sbi_xact->assoc_stream_id >= OGS_MIN_POOL_ID &&
                    sbi_xact->assoc_stream_id <= OGS_MAX_POOL_ID)
                    e->h.sbi.data = OGS_UINT_TO_POINTER(sbi_xact->assoc_stream_id);

                ogs_sbi_xact_remove(sbi_xact);

                sess = pcf_sess_find_by_id(sess_id);
                if (!sess) {
                    ogs_error("Session has already been removed");
                    break;
                }

                pcf_ue = pcf_ue_find_by_id(sess->pcf_ue_id);
                ogs_assert(pcf_ue);

                e->sess_id = sess->id;
                e->h.sbi.message = &message;

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

    case OGS_EVENT_SBI_TIMER:
        ogs_assert(e);

        switch(e->h.timer_id) {
        case OGS_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        case OGS_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        case OGS_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        case OGS_TIMER_NF_INSTANCE_VALIDITY:
            nf_instance = e->h.sbi.data;
            ogs_assert(nf_instance);
            ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

            ogs_sbi_self()->nf_instance->load = pcf_instance_get_load();

            ogs_fsm_dispatch(&nf_instance->sm, e);
            if (OGS_FSM_CHECK(&nf_instance->sm, ogs_sbi_nf_state_exception))
                ogs_error("[%s:%s] State machine exception [%d]",
                        OpenAPI_nf_type_ToString(nf_instance->nf_type),
                        nf_instance->id, e->h.timer_id);
            break;

        case OGS_TIMER_SUBSCRIPTION_VALIDITY:
            subscription_data = e->h.sbi.data;
            ogs_assert(subscription_data);

            ogs_assert(true ==
                ogs_nnrf_nfm_send_nf_status_subscribe(
                    ogs_sbi_self()->nf_instance->nf_type,
                    subscription_data->req_nf_instance_id,
                    subscription_data->subscr_cond.nf_type,
                    subscription_data->subscr_cond.service_name));

            ogs_error("[%s] Subscription validity expired",
                subscription_data->id);
            ogs_sbi_subscription_data_remove(subscription_data);
            break;

        case OGS_TIMER_SUBSCRIPTION_PATCH:
            subscription_data = e->h.sbi.data;
            ogs_assert(subscription_data);

            ogs_assert(true ==
                ogs_nnrf_nfm_send_nf_status_update(subscription_data));

            ogs_info("[%s] Need to update Subscription",
                    subscription_data->id);
            break;

        case OGS_TIMER_SBI_CLIENT_WAIT:
            /*
             * ogs_pollset_poll() receives the time of the expiration
             * of next timer as an argument. If this timeout is
             * in very near future (1 millisecond), and if there are
             * multiple events that need to be processed by ogs_pollset_poll(),
             * these could take more than 1 millisecond for processing,
             * resulting in the timer already passed the expiration.
             *
             * In case that another NF is under heavy load and responds
             * to an SBI request with some delay of a few seconds,
             * it can happen that ogs_pollset_poll() adds SBI responses
             * to the event list for further processing,
             * then ogs_timer_mgr_expire() is called which will add
             * an additional event for timer expiration. When all events are
             * processed one-by-one, the SBI xact would get deleted twice
             * in a row, resulting in a crash.
             *
             * 1. ogs_pollset_poll()
             *    message was received and put into an event list,
             * 2. ogs_timer_mgr_expire()
             *    add an additional event for timer expiration
             * 3. message event is processed. (free SBI xact)
             * 4. timer expiration event is processed. (double-free SBI xact)
             *
             * To avoid double-free SBI xact,
             * we need to check ogs_sbi_xact_find_by_id()
             */
            sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                    sbi_xact_id <= OGS_MAX_POOL_ID);

            sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
            if (!sbi_xact) {
                ogs_error("SBI transaction has already been removed [%d]",
                        sbi_xact_id);
                break;
            }

            sbi_object = sbi_xact->sbi_object;
            ogs_assert(sbi_object);

            ogs_assert(sbi_xact->assoc_stream_id >= OGS_MIN_POOL_ID &&
                    sbi_xact->assoc_stream_id <= OGS_MAX_POOL_ID);
            stream = ogs_sbi_stream_find_by_id(sbi_xact->assoc_stream_id);

            service_type = sbi_xact->service_type;

            ogs_sbi_xact_remove(sbi_xact);

            ogs_assert(sbi_object->type > OGS_SBI_OBJ_BASE &&
                        sbi_object->type < OGS_SBI_OBJ_TOP);

            switch(sbi_object->type) {
            case OGS_SBI_OBJ_UE_TYPE:
                pcf_ue_id = sbi_xact->sbi_object_id;
                ogs_assert(pcf_ue_id >= OGS_MIN_POOL_ID &&
                        pcf_ue_id <= OGS_MAX_POOL_ID);

                pcf_ue = pcf_ue_find_by_id(pcf_ue_id);
                if (!pcf_ue) {
                    ogs_error("UE(pcf_ue) has already been removed");
                    break;
                }
                ogs_error("[%s] Cannot receive SBI message", pcf_ue->supi);
                break;

            case OGS_SBI_OBJ_SESS_TYPE:
                sess_id = sbi_xact->sbi_object_id;
                ogs_assert(sess_id >= OGS_MIN_POOL_ID &&
                        sess_id <= OGS_MAX_POOL_ID);

                sess = pcf_sess_find_by_id(sess_id);
                if (!sess) {
                    ogs_error("Session has already been removed");
                    break;
                }
                ogs_error("[%d] Cannot receive SBI message", sess->psi);
                break;

            default:
                ogs_fatal("Not implemented [%s:%d]",
                    ogs_sbi_service_type_to_name(service_type),
                    sbi_object->type);
                ogs_assert_if_reached();
            }

            ogs_error("Cannot receive SBI message");

            if (!stream) {
                ogs_error("STREAM has alreadt been removed [%d]",
                        sbi_xact->assoc_stream_id);
                break;
            }
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                    "Cannot receive SBI message", NULL, NULL));
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    ogs_timer_get_name(e->h.timer_id), e->h.timer_id);
        }
        break;

    default:
        ogs_error("No handler for event %s", pcf_event_get_name(e));
        break;
    }
}
