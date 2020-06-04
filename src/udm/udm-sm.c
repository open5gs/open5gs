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

void udm_state_initial(ogs_fsm_t *s, udm_event_t *e)
{
    udm_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &udm_state_operational);
}

void udm_state_final(ogs_fsm_t *s, udm_event_t *e)
{
    udm_sm_debug(e);
}

void udm_state_operational(ogs_fsm_t *s, udm_event_t *e)
{
    int rv;

    ogs_sbi_session_t *session = NULL;
    ogs_sbi_request_t *request = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_subscription_t *subscription = NULL;
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_message_t message;

    udm_ue_t *udm_ue = NULL;

    udm_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        rv = udm_sbi_open();
        if (rv != OGS_OK) {
            ogs_fatal("Can't establish SBI path");
        }

        break;

    case OGS_FSM_EXIT_SIG:
        udm_sbi_close();
        break;

    case UDM_EVT_SBI_SERVER:
        request = e->sbi.request;
        ogs_assert(request);
        session = e->sbi.session;
        ogs_assert(session);

        rv = ogs_sbi_parse_request(&message, request);
        if (rv != OGS_OK) {
            /* 'message' buffer is released in ogs_sbi_parse_request() */
            ogs_error("cannot parse HTTP message");
            ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    NULL, "cannot parse HTTP message", NULL);
            break;
        }

        if (strcmp(message.h.api.version, OGS_SBI_API_VERSION) != 0) {
            ogs_error("Not supported version [%s]", message.h.api.version);
            ogs_sbi_server_send_error(session, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    &message, "Not supported version", NULL);
            ogs_sbi_message_free(&message);
            break;
        }

        SWITCH(message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY)
                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    udm_nnrf_handle_nf_status_notify(session, &message);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]",
                            message.h.method);
                    ogs_sbi_server_send_error(session,
                            OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED,
                            &message,
                            "Invalid HTTP method", message.h.method);
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_sbi_server_send_error(session,
                        OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED, &message,
                        "Unknown resource name",
                        message.h.resource.component[0]);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_UEAU)
            if (!message.h.resource.component[0]) {
                ogs_error("Not found [%s]", message.h.method);
                ogs_sbi_server_send_error(session,
                    OGS_SBI_HTTP_STATUS_NOT_FOUND,
                    &message, "Not found", message.h.method);
                break;
            }

            if (!message.h.resource.component[1]) {
                ogs_error("Invalid resource name [%s]", message.h.method);
                ogs_sbi_server_send_error(session,
                    OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED,
                    &message, "Invalid resource name", message.h.method);
                break;
            }

            SWITCH(message.h.resource.component[2])
            CASE(OGS_SBI_RESOURCE_NAME_AUTH_EVENTS)
                udm_ue = udm_ue_find_by_ctx_id(
                        message.h.resource.component[2]);
                break;

            DEFAULT
                udm_ue = udm_ue_find_by_suci_or_supi(
                        message.h.resource.component[0]);
                if (!udm_ue) {
                    udm_ue = udm_ue_add(message.h.resource.component[0]);
                    ogs_assert(udm_ue);
                }
            END

            if (!udm_ue) {
                ogs_error("Not found [%s]", message.h.method);
                ogs_sbi_server_send_error(session,
                    OGS_SBI_HTTP_STATUS_NOT_FOUND,
                    &message, "Not found", message.h.method);
                break;
            }

            ogs_assert(OGS_FSM_STATE(&udm_ue->sm));

            OGS_SETUP_SBI_SESSION(udm_ue, session);

            if (udm_ue->state.component1)
                ogs_free(udm_ue->state.component1);
            udm_ue->state.component1 = ogs_strdup(
                    message.h.resource.component[1]);

            e->udm_ue = udm_ue;
            e->sbi.message = &message;
            ogs_fsm_dispatch(&udm_ue->sm, e);
            if (OGS_FSM_CHECK(&udm_ue->sm, udm_ue_state_exception)) {
                ogs_error("[%s] State machine exception", udm_ue->suci);
                udm_ue_remove(udm_ue);
            }
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", message.h.service.name);
            ogs_sbi_server_send_error(session,
                    OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED, &message,
                    "Invalid API name", message.h.resource.component[0]);
        END

        /* In lib/sbi/server.c, notify_completed() releases 'request' buffer. */
        ogs_sbi_message_free(&message);
        break;

    case UDM_EVT_SBI_CLIENT:
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

        if (strcmp(message.h.api.version, OGS_SBI_API_VERSION) != 0) {
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

                if (OGS_FSM_CHECK(&nf_instance->sm, udm_nf_state_exception)) {
                    ogs_error("[%s] State machine exception", nf_instance->id);
                }
                break;

            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS)
                subscription = e->sbi.data;
                ogs_assert(subscription);

                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_CREATED ||
                        message.res_status == OGS_SBI_HTTP_STATUS_OK) {
                        udm_nnrf_handle_nf_status_subscribe(
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
                udm_ue = e->sbi.data;
                ogs_assert(udm_ue);

                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_GET)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_OK) {
                        ogs_timer_stop(udm_ue->sbi_client_wait.timer);

                        udm_nnrf_handle_nf_discover(udm_ue, &message);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                                udm_ue->suci, message.res_status);
                    }
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid HTTP method [%s]",
                            udm_ue->suci, message.h.method);
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
            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA)
                udm_ue = e->sbi.data;
                ogs_assert(udm_ue);

                e->udm_ue = udm_ue;
                e->sbi.message = &message;
                ogs_fsm_dispatch(&udm_ue->sm, e);
                if (OGS_FSM_CHECK(&udm_ue->sm, udm_ue_state_exception)) {
                    ogs_error("[%s] State machine exception", udm_ue->suci);
                    udm_ue_remove(udm_ue);
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

    case UDM_EVT_SBI_TIMER:
        ogs_assert(e);

        switch(e->timer_id) {
        case UDM_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        case UDM_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        case UDM_TIMER_NF_INSTANCE_HEARTBEAT:
        case UDM_TIMER_NF_INSTANCE_VALIDITY:
            nf_instance = e->sbi.data;
            ogs_assert(nf_instance);
            ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

            ogs_fsm_dispatch(&nf_instance->sm, e);
            if (OGS_FSM_CHECK(&nf_instance->sm, udm_nf_state_exception))
                ogs_error("[%s] State machine exception [%d]",
                        nf_instance->id, e->timer_id);
            break;

        case UDM_TIMER_SUBSCRIPTION_VALIDITY:
            subscription = e->sbi.data;
            ogs_assert(subscription);

            ogs_info("[%s] Subscription validity expired", subscription->id);
            ogs_sbi_subscription_remove(subscription);

            ogs_nnrf_nfm_send_nf_status_subscribe(subscription->client,
                    udm_self()->nf_type, subscription->nf_instance_id);
            break;

        case UDM_TIMER_SBI_CLIENT_WAIT:
            udm_ue = e->sbi.data;
            ogs_assert(udm_ue);
            session = udm_ue->session;
            ogs_assert(session);

            ogs_error("[%s] Cannot receive SBI message", udm_ue->suci);
            ogs_sbi_server_send_error(session,
                    OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT, NULL,
                    "Cannot receive SBI message", udm_ue->suci);
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    udm_timer_get_name(e->timer_id), e->timer_id);
        }
        break;

    default:
        ogs_error("No handler for event %s", udm_event_get_name(e));
        break;
    }
}
