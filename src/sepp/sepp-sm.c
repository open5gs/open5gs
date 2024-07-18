/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

void sepp_state_initial(ogs_fsm_t *s, sepp_event_t *e)
{
    sepp_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &sepp_state_operational);
}

void sepp_state_final(ogs_fsm_t *s, sepp_event_t *e)
{
    sepp_sm_debug(e);

    ogs_assert(s);
}

void sepp_state_operational(ogs_fsm_t *s, sepp_event_t *e)
{
    int rv;

    sepp_node_t *sepp_node = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_server_t *server = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_subscription_data_t *subscription_data = NULL;
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_message_t message;

    sepp_sm_debug(e);

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

        server = ogs_sbi_server_from_stream(stream);
        ogs_assert(server);

        rv = ogs_sbi_parse_request(&message, request);
        if (rv != OGS_OK) {
            /* 'sbi_message' buffer is released in ogs_sbi_parse_request() */
            ogs_error("cannot parse HTTP sbi_message");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    NULL, "cannot parse HTTP sbi_message", NULL, NULL));
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
            if (server->interface &&
                strcmp(server->interface, OGS_SBI_INTERFACE_NAME_SEPP) == 0) {
                ogs_error("[DROP] Peer SEPP is using the wrong interface[%s]",
                        server->interface);
                break;
            }

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
                        "Invalid resource name",
                        message.h.resource.component[0], NULL));
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE)
            if (!server->interface &&
                ogs_sbi_server_first_by_interface(
                    OGS_SBI_INTERFACE_NAME_SEPP)) {
                ogs_error("[DROP] Peer SEPP is using the wrong interface[sbi]");
                break;
            }

            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY)
                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (message.SecNegotiateReqData &&
                        message.SecNegotiateReqData->sender) {
                        sepp_node = sepp_node_find_by_receiver(
                                message.SecNegotiateReqData->sender);
                        if (!sepp_node) {
                            sepp_node = sepp_node_add(
                                message.SecNegotiateReqData->sender);
                            ogs_assert(sepp_node);

                            sepp_handshake_fsm_init(sepp_node, false);
                        }
                    }
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
                        "Invalid resource name",
                        message.h.resource.component[0], NULL));
            END

            if (!sepp_node) {
                ogs_error("Not found [%s]", message.h.method);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(
                        stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                        &message, "Not found", message.h.method, NULL));
                break;
            }

            ogs_assert(OGS_FSM_STATE(&sepp_node->sm));

            e->sepp_node = sepp_node;
            e->h.sbi.message = &message;
            ogs_fsm_dispatch(&sepp_node->sm, e);
            if (OGS_FSM_CHECK(&sepp_node->sm, sepp_handshake_state_exception))
                ogs_error("[%s] State machine exception", sepp_node->receiver);
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
                ogs_fsm_handler_t old_state;

                nf_instance = e->h.sbi.data;
                ogs_assert(nf_instance);

                old_state = OGS_FSM_STATE(&nf_instance->sm);
                ogs_assert(old_state);

                e->h.sbi.message = &message;
                ogs_fsm_dispatch(&nf_instance->sm, e);

            /*
             * The SEPP on the H-PLMN should send a n32c-handshake message
             * with the Sender PLMN-ID. Otherwise, the UE in the V-PLMN cannot
             * find the SEPP in the H-PLMN during the REGISTRATION.
             *
             * Since SEPP's PLMN-ID can be received when registering with NRF,
             * n32c-handshake messages could be started after that.
             *
             * To confirm that the PLMN-ID has been received from the NRF,
             * we check if the NF state has changed from
             * ogs_sbi_nf_state_will_register to ogs_sbi_nf_state_registered.
             */

                if (old_state ==
                        (ogs_fsm_handler_t)ogs_sbi_nf_state_will_register &&
                    OGS_FSM_CHECK(&nf_instance->sm,
                        ogs_sbi_nf_state_registered)) {

                    /* Initialize SEPP Peer List */
                    ogs_list_for_each(&sepp_self()->peer_list, sepp_node) {

                        /*
                         * Since FSM initialization only needs to be done once
                         * at the beginning, it only performs initialization
                         * when the state variable(OGS_FSM_STATE()) is NULL.
                         */
                        if (!OGS_FSM_STATE(&sepp_node->sm))
                            sepp_handshake_fsm_init(sepp_node, true);
                    }
                }
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
                    if (message.res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        ogs_sbi_subscription_data_remove(subscription_data);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                                subscription_data->id ?
                                    subscription_data->id : "Unknown",
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

        CASE(OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE)

            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY)
                sepp_node = e->h.sbi.data;
                ogs_assert(sepp_node);
                ogs_assert(OGS_FSM_STATE(&sepp_node->sm));

                e->sepp_node = sepp_node;
                e->h.sbi.message = &message;
                ogs_fsm_dispatch(&sepp_node->sm, e);
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

    case OGS_EVENT_SBI_TIMER:
        ogs_assert(e);

        switch(e->h.timer_id) {
        case SEPP_TIMER_PEER_ESTABLISH:
            sepp_node = e->sepp_node;
            ogs_assert(sepp_node);
            ogs_assert(OGS_FSM_STATE(&sepp_node->sm));

            ogs_fsm_dispatch(&sepp_node->sm, e);
            if (OGS_FSM_CHECK(&sepp_node->sm, sepp_handshake_state_exception))
                ogs_error("[%s] State machine exception [%d]",
                        sepp_node->receiver, e->h.timer_id);
            break;

        case OGS_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        case OGS_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        case OGS_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        case OGS_TIMER_NF_INSTANCE_VALIDITY:
            nf_instance = e->h.sbi.data;
            ogs_assert(nf_instance);
            ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

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

        default:
            ogs_error("Unknown timer[%s:%d]",
                    ogs_timer_get_name(e->h.timer_id), e->h.timer_id);
        }
        break;

    default:
        ogs_error("No handler for event %s", sepp_event_get_name(e));
        break;
    }
}
