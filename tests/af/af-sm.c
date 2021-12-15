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
#include "nnrf-handler.h"
#include "nbsf-handler.h"
#include "npcf-handler.h"

void af_state_initial(ogs_fsm_t *s, af_event_t *e)
{
    af_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &af_state_operational);
}

void af_state_final(ogs_fsm_t *s, af_event_t *e)
{
    af_sm_debug(e);

    ogs_assert(s);
}

void af_state_operational(ogs_fsm_t *s, af_event_t *e)
{
    int rv;

    af_sess_t *sess = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_request_t *request = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_subscription_t *subscription = NULL;
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_message_t message;
    ogs_sbi_xact_t *sbi_xact = NULL;

    af_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case AF_EVT_SBI_SERVER:
        request = e->sbi.request;
        ogs_assert(request);
        stream = e->sbi.data;
        ogs_assert(stream);

        rv = ogs_sbi_parse_request(&message, request);
        if (rv != OGS_OK) {
            /* 'sbi_message' buffer is released in ogs_sbi_parse_request() */
            ogs_error("cannot parse HTTP sbi_message");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    NULL, "cannot parse HTTP sbi_message", NULL));
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
                    af_nnrf_handle_nf_status_notify(stream, &message);
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
                        "Invalid resource name",
                        message.h.resource.component[0]));
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION)
            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_APP_SESSIONS)
                af_sess_t *app_session = NULL;

                if (!message.h.resource.component[1]) {
                    ogs_error("[%s] Unknown app session id",
                            message.h.resource.component[1]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_NOT_FOUND,
                            &message, "Not found",
                            message.h.resource.component[1]));
                    break;
                }

                app_session = af_sess_find_by_af_app_session_id(
                            message.h.resource.component[1]);
                if (!app_session) {
                    ogs_error("[%s] Unknown app session id",
                            message.h.resource.component[1]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_NOT_FOUND,
                            &message, "Not found",
                            message.h.resource.component[1]));
                    break;
                }

                SWITCH(message.h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_TERMINATE)
                    ogs_assert(true ==
                            ogs_sbi_send_http_status_no_content(stream));
                    af_sess_remove(app_session);
                    break;
                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                            message.h.resource.component[2]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, &message,
                            "Invalid resource name",
                            message.h.resource.component[2]));
                END
                break;
            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &message,
                        "Invalid resource name",
                        message.h.resource.component[0]));
            END
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

    case AF_EVT_SBI_CLIENT:
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
                        af_nnrf_handle_nf_status_subscribe(
                                subscription, &message);
                    } else {
                        ogs_error("HTTP response error : %d",
                                message.res_status);
                    }
                    break;

                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        ogs_sbi_subscription_remove(subscription);
                    } else {
                        ogs_error("HTTP response error : %d",
                                message.res_status);
                    }
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

        CASE(OGS_SBI_SERVICE_NAME_NNRF_DISC)
            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                sbi_xact = e->sbi.data;
                ogs_assert(sbi_xact);

                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_GET)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_OK)
                        af_nnrf_handle_nf_discover(sbi_xact, &message);
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

        CASE(OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT)
            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_PCF_BINDINGS)
                sbi_xact = e->sbi.data;
                ogs_assert(sbi_xact);

                sess = (af_sess_t *)sbi_xact->sbi_object;
                ogs_assert(sess);

                ogs_sbi_xact_remove(sbi_xact);

                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_GET)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_OK)
                        af_nbsf_management_handle_pcf_binding(sess, &message);
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

        CASE(OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION)
            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_APP_SESSIONS)
                sess = e->sbi.data;
                ogs_assert(sess);

                if (message.h.resource.component[1]) {
                    if (message.h.resource.component[2]) {
                        SWITCH(message.h.resource.component[2])
                        CASE(OGS_SBI_RESOURCE_NAME_DELETE)
                            af_sess_remove(sess);
                            break;
                        DEFAULT
                            ogs_error("Invalid resource name [%s]",
                                    message.h.resource.component[2]);
                            ogs_assert_if_reached();
                        END
                    } else {
                        SWITCH(message.h.method)
                        CASE(OGS_SBI_HTTP_METHOD_PATCH)
                            if (message.res_status == OGS_SBI_HTTP_STATUS_OK)
                                af_npcf_policyauthorization_handle_update(
                                        sess, &message);
                            else
                                ogs_error("HTTP response error [%d]",
                                        message.res_status);
                            break;
                        DEFAULT
                            ogs_error("Invalid HTTP method [%s]",
                                    message.h.method);
                            ogs_assert_if_reached();
                        END
                    }
                } else {
                    SWITCH(message.h.method)
                    CASE(OGS_SBI_HTTP_METHOD_POST)
                        if (message.res_status == OGS_SBI_HTTP_STATUS_CREATED)
                            af_npcf_policyauthorization_handle_create(
                                    sess, &message);
                        else
                            ogs_error("HTTP response error [%d]",
                                    message.res_status);
                        break;
                    DEFAULT
                        ogs_error("Invalid HTTP method [%s]", message.h.method);
                        ogs_assert_if_reached();
                    END
                }
                break;
            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("Invalid service name [%s]", message.h.service.name);
            ogs_assert_if_reached();
        END

        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        break;

    case AF_EVT_SBI_TIMER:
        ogs_assert(e);

        switch(e->timer_id) {
        case AF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        case AF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        case AF_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        case AF_TIMER_NF_INSTANCE_VALIDITY:
            nf_instance = e->sbi.data;
            ogs_assert(nf_instance);
            ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

            ogs_fsm_dispatch(&nf_instance->sm, e);
            if (OGS_FSM_CHECK(&nf_instance->sm, af_nf_state_exception))
                ogs_error("State machine exception [%d]", e->timer_id);
            break;

        case AF_TIMER_SUBSCRIPTION_VALIDITY:
            subscription = e->sbi.data;
            ogs_assert(subscription);

            ogs_assert(true ==
                ogs_nnrf_nfm_send_nf_status_subscribe(subscription->client,
                    af_self()->nf_type, subscription->req_nf_instance_id,
                    subscription->subscr_cond.nf_type));

            ogs_info("Subscription validity expired [%s]", subscription->id);
            ogs_sbi_subscription_remove(subscription);
            break;

        case AF_TIMER_SBI_CLIENT_WAIT:
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
                    af_timer_get_name(e->timer_id), e->timer_id);
        }
        break;

    case AF_EVT_SBI_LOCAL:
        ogs_assert(e);

        switch(e->local_id) {
        case AF_LOCAL_DISCOVER_AND_SEND:
            af_sbi_discover_and_send(e->local.target_nf_type,
                    e->sess, e->local.data, e->local.build);
            break;
        case AF_LOCAL_SEND_TO_PCF:
            af_sbi_send_to_pcf(e->sess, e->local.data, e->local.build);
            break;
        default:
            ogs_error("Unknown local[%s:%d]",
                    af_local_get_name(e->local_id), e->local_id);
        }
        break;

    default:
        ogs_error("No handler for event %s", af_event_get_name(e));
        break;
    }
}
