/*
 * Copyright (C) 2023-2024 by Sukchan Lee <acetcom@gmail.com>
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
#include "n32c-handler.h"

void sepp_handshake_fsm_init(sepp_node_t *sepp_node, bool try_to_establish)
{
    sepp_event_t e;

    ogs_assert(sepp_node);

    memset(&e, 0, sizeof(e));
    e.sepp_node = sepp_node;

    if (try_to_establish == true) {
        sepp_node->t_establish_interval = ogs_timer_add(ogs_app()->timer_mgr,
                sepp_timer_peer_establish, sepp_node);
        ogs_assert(sepp_node->t_establish_interval);
    }

    ogs_fsm_init(&sepp_node->sm,
            sepp_handshake_state_initial, sepp_handshake_state_final, &e);
}

void sepp_handshake_fsm_fini(sepp_node_t *sepp_node)
{
    sepp_event_t e;

    ogs_assert(sepp_node);

    memset(&e, 0, sizeof(e));
    e.sepp_node = sepp_node;

    if (OGS_FSM_CHECK(&sepp_node->sm, sepp_handshake_state_established)) {
        ogs_expect(true ==
                sepp_n32c_handshake_send_security_capability_request(
                    sepp_node, true));
    }

    ogs_fsm_fini(&sepp_node->sm, &e);

    if (sepp_node->t_establish_interval)
        ogs_timer_delete(sepp_node->t_establish_interval);
}

void sepp_handshake_state_initial(ogs_fsm_t *s, sepp_event_t *e)
{
    sepp_node_t *sepp_node = NULL;

    ogs_assert(s);
    ogs_assert(e);

    sepp_node = e->sepp_node;
    ogs_assert(sepp_node);

    OGS_FSM_TRAN(s, &sepp_handshake_state_will_establish);
}

void sepp_handshake_state_final(ogs_fsm_t *s, sepp_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    sepp_sm_debug(e);
}

void sepp_handshake_state_will_establish(ogs_fsm_t *s, sepp_event_t *e)
{
    bool handled;
    sepp_node_t *sepp_node = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    sepp_sm_debug(e);

    sepp_node = e->sepp_node;
    ogs_assert(sepp_node);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        if (sepp_node->t_establish_interval) {
            ogs_timer_start(sepp_node->t_establish_interval,
                ogs_local_conf()->time.message.sbi.reconnect_interval);

            ogs_expect(true ==
                    sepp_n32c_handshake_send_security_capability_request(
                        sepp_node, false));
        }
        break;

    case OGS_FSM_EXIT_SIG:
        if (sepp_node->t_establish_interval) {
            ogs_timer_stop(sepp_node->t_establish_interval);
        }
        break;

    case OGS_EVENT_SBI_SERVER:
        message = e->h.sbi.message;
        ogs_assert(message);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY)
                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    handled =
                        sepp_n32c_handshake_handle_security_capability_request(
                            sepp_node, stream, message);
                    if (!handled) {
                        ogs_error("[%s] Cannot handle SBI message",
                                sepp_node->receiver);
                        OGS_FSM_TRAN(s, &sepp_handshake_state_exception);
                        break;
                    }

                    if (sepp_node->negotiated_security_scheme ==
                            OpenAPI_security_capability_TLS) {

                        /* Change to ESTABLISHED */
                        sepp_n32c_handshake_send_security_capability_response(
                                sepp_node, stream);
                        OGS_FSM_TRAN(s, &sepp_handshake_state_established);

                    } else if (sepp_node->negotiated_security_scheme ==
                            OpenAPI_security_capability_PRINS) {

                        ogs_error("[%s] PRINS is not supported",
                                sepp_node->receiver);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                                message, "PRINS is not supported",
                                sepp_node->receiver, NULL));

                    } else if (sepp_node->negotiated_security_scheme ==
                            OpenAPI_security_capability_NONE) {

                        /* No State Change */
                        sepp_n32c_handshake_send_security_capability_response(
                                sepp_node, stream);
                        ogs_warn("[%s] SEPP has not been established",
                                sepp_node->receiver);
                    }
                    break;
                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message->h.method);
                END
                break;

            DEFAULT
                ogs_error("[%s] Invalid resource name [%s]",
                        sepp_node->receiver, message->h.resource.component[0]);
            END
            break;

        DEFAULT
            ogs_error("[%s] Invalid API name [%s]",
                    sepp_node->receiver, message->h.service.name);
        END
        break;

    case OGS_EVENT_SBI_CLIENT:
        message = e->h.sbi.message;
        ogs_assert(message);

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY)
                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)

                    if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                        handled = sepp_n32c_handshake_handle_security_capability_response(
                                sepp_node, message);
                        if (!handled) {
                            ogs_error("[%s] Cannot handle SBI message",
                                    sepp_node->receiver);
                            OGS_FSM_TRAN(s, &sepp_handshake_state_exception);
                            break;
                        }

                        if (sepp_node->negotiated_security_scheme ==
                                OpenAPI_security_capability_TLS) {

                            /* Change to ESTABLISHED */
                            OGS_FSM_TRAN(s, &sepp_handshake_state_established);

                        } else if (sepp_node->negotiated_security_scheme ==
                                OpenAPI_security_capability_PRINS) {

                            ogs_error("[%s] PRINS is not supported",
                                    sepp_node->receiver);

                        } else if (sepp_node->negotiated_security_scheme ==
                                OpenAPI_security_capability_NONE) {

                            /* No State Change */
                            ogs_warn("[%s] SEPP has not been established",
                                    sepp_node->receiver);
                        }
                    } else {
                        ogs_error("[%s] HTTP Response Status Code [%d]",
                                sepp_node->receiver, message->res_status);
                    }
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message->h.method);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("[%s] Invalid resource name [%s]",
                        sepp_node->receiver, message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("[%s] Invalid API name [%s]",
                    sepp_node->receiver, message->h.service.name);
            ogs_assert_if_reached();
        END
        break;

    case OGS_EVENT_SBI_TIMER:
        switch(e->h.timer_id) {
        case SEPP_TIMER_PEER_ESTABLISH:
            ogs_warn("[%s] Retry establishment with Peer SEPP",
                    sepp_node->receiver);

            ogs_assert(sepp_node->t_establish_interval);
            ogs_timer_start(sepp_node->t_establish_interval,
                ogs_local_conf()->time.message.sbi.reconnect_interval);

            ogs_expect(true ==
                sepp_n32c_handshake_send_security_capability_request(
                    sepp_node, false));
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    ogs_timer_get_name(e->h.timer_id), e->h.timer_id);
        }
        break;

    default:
        ogs_error("Unknown event %s", sepp_event_get_name(e));
        break;
    }
}

void sepp_handshake_state_established(ogs_fsm_t *s, sepp_event_t *e)
{
    bool handled;
    sepp_node_t *sepp_node = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    sepp_sm_debug(e);

    sepp_node = e->sepp_node;
    ogs_assert(sepp_node);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        ogs_info("[%s] SEPP established", sepp_node->receiver);
        break;

    case OGS_FSM_EXIT_SIG:
        ogs_info("[%s] SEPP terminated", sepp_node->receiver);
        break;

    case OGS_EVENT_SBI_SERVER:
        message = e->h.sbi.message;
        ogs_assert(message);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY)
                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    handled =
                        sepp_n32c_handshake_handle_security_capability_request(
                            sepp_node, stream, message);
                    if (!handled) {
                        ogs_error("[%s] Cannot handle SBI message",
                                sepp_node->receiver);
                        OGS_FSM_TRAN(s, &sepp_handshake_state_exception);
                        break;
                    }

                    if (sepp_node->negotiated_security_scheme ==
                            OpenAPI_security_capability_TLS) {

                        /* No State Change */
                        sepp_n32c_handshake_send_security_capability_response(
                                sepp_node, stream);
                        ogs_warn("[%s] SEPP has already been established",
                                sepp_node->receiver);

                    } else if (sepp_node->negotiated_security_scheme ==
                            OpenAPI_security_capability_PRINS) {

                        ogs_error("[%s] PRINS is not supported",
                                sepp_node->receiver);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                                message, "PRINS is not supported",
                                sepp_node->receiver, NULL));

                    } else if (sepp_node->negotiated_security_scheme ==
                            OpenAPI_security_capability_NONE) {

                        /* Change to TERMINATED */
                        sepp_n32c_handshake_send_security_capability_response(
                                sepp_node, stream);
                        OGS_FSM_TRAN(s, &sepp_handshake_state_terminated);
                    }
                    break;
                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message->h.method);
                END
                break;

            DEFAULT
                ogs_error("[%s] Invalid resource name [%s]",
                        sepp_node->receiver, message->h.resource.component[0]);
            END
            break;

        DEFAULT
            ogs_error("[%s] Invalid API name [%s]",
                    sepp_node->receiver, message->h.service.name);
        END
        break;

    case OGS_EVENT_SBI_CLIENT:
        message = e->h.sbi.message;
        ogs_assert(message);

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY)
                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                        ogs_warn("[%s] SEPP has already been established",
                                sepp_node->receiver);
                    } else {
                        ogs_error("[%s] HTTP Response Status Code [%d]",
                                sepp_node->receiver, message->res_status);
                    }
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message->h.method);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("[%s] Invalid resource name [%s]",
                        sepp_node->receiver, message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("[%s] Invalid API name [%s]",
                    sepp_node->receiver, message->h.service.name);
            ogs_assert_if_reached();
        END
        break;

    default:
        ogs_error("Unknown event %s", sepp_event_get_name(e));
        break;
    }
}

void sepp_handshake_state_terminated(ogs_fsm_t *s, sepp_event_t *e)
{
    bool handled;
    sepp_node_t *sepp_node = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    sepp_sm_debug(e);

    sepp_node = e->sepp_node;
    ogs_assert(sepp_node);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        if (sepp_node->t_establish_interval) {
            ogs_timer_start(sepp_node->t_establish_interval,
                ogs_local_conf()->time.message.sbi.reconnect_interval);
        }
        break;

    case OGS_FSM_EXIT_SIG:
        if (sepp_node->t_establish_interval) {
            ogs_timer_stop(sepp_node->t_establish_interval);
        }
        break;

    case OGS_EVENT_SBI_SERVER:
        message = e->h.sbi.message;
        ogs_assert(message);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY)
                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    handled =
                        sepp_n32c_handshake_handle_security_capability_request(
                            sepp_node, stream, message);
                    if (!handled) {
                        ogs_error("[%s] Cannot handle SBI message",
                                sepp_node->receiver);
                        OGS_FSM_TRAN(s, &sepp_handshake_state_exception);
                        break;
                    }

                    if (sepp_node->negotiated_security_scheme ==
                            OpenAPI_security_capability_TLS) {

                        /* Change to ESTABLISHED */
                        sepp_n32c_handshake_send_security_capability_response(
                                sepp_node, stream);
                        OGS_FSM_TRAN(s, &sepp_handshake_state_established);

                    } else if (sepp_node->negotiated_security_scheme ==
                            OpenAPI_security_capability_PRINS) {

                        ogs_error("[%s] PRINS is not supported",
                                sepp_node->receiver);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                                message, "PRINS is not supported",
                                sepp_node->receiver, NULL));

                    } else if (sepp_node->negotiated_security_scheme ==
                            OpenAPI_security_capability_NONE) {

                        /* No State Change */
                        sepp_n32c_handshake_send_security_capability_response(
                                sepp_node, stream);
                        ogs_warn("[%s] SEPP has not been established",
                                sepp_node->receiver);
                    }
                    break;
                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message->h.method);
                END
                break;

            DEFAULT
                ogs_error("[%s] Invalid resource name [%s]",
                        sepp_node->receiver, message->h.resource.component[0]);
            END
            break;

        DEFAULT
            ogs_error("[%s] Invalid API name [%s]",
                    sepp_node->receiver, message->h.service.name);
        END
        break;

    case OGS_EVENT_SBI_CLIENT:
        message = e->h.sbi.message;
        ogs_assert(message);

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY)
                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                        ogs_warn("[%s] SEPP was terminated", sepp_node->receiver);
                    } else {
                        ogs_error("[%s] HTTP Response Status Code [%d]",
                                sepp_node->receiver, message->res_status);
                    }
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message->h.method);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("[%s] Invalid resource name [%s]",
                        sepp_node->receiver, message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("[%s] Invalid API name [%s]",
                    sepp_node->receiver, message->h.service.name);
            ogs_assert_if_reached();
        END
        break;

    case OGS_EVENT_SBI_TIMER:
        switch(e->h.timer_id) {
        case SEPP_TIMER_PEER_ESTABLISH:
            ogs_warn("[%s] Retry establishment with Peer SEPP",
                    sepp_node->receiver);

            OGS_FSM_TRAN(s, &sepp_handshake_state_will_establish);
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    ogs_timer_get_name(e->h.timer_id), e->h.timer_id);
        }
        break;

    default:
        ogs_error("Unknown event %s", sepp_event_get_name(e));
        break;
    }
}

void sepp_handshake_state_exception(ogs_fsm_t *s, sepp_event_t *e)
{
    sepp_node_t *sepp_node = NULL;

    ogs_sbi_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    sepp_sm_debug(e);

    sepp_node = e->sepp_node;
    ogs_assert(sepp_node);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        if (sepp_node->t_establish_interval) {
            ogs_timer_start(sepp_node->t_establish_interval,
                ogs_local_conf()->time.message.sbi.
                reconnect_interval_in_exception);
        }
        break;

    case OGS_FSM_EXIT_SIG:
        if (sepp_node->t_establish_interval) {
            ogs_timer_stop(sepp_node->t_establish_interval);
        }
        break;

    case OGS_EVENT_SBI_TIMER:
        switch(e->h.timer_id) {
        case SEPP_TIMER_PEER_ESTABLISH:
            ogs_warn("[%s] Retry establishment with Peer SEPP",
                    sepp_node->receiver);

            OGS_FSM_TRAN(s, &sepp_handshake_state_will_establish);
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    ogs_timer_get_name(e->h.timer_id), e->h.timer_id);
        }
        break;

    case OGS_EVENT_SBI_SERVER:
    case OGS_EVENT_SBI_CLIENT:
        message = e->h.sbi.message;
        ogs_assert(message);

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_N32C_HANDSHAKE)

            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_EXCHANGE_CAPABILITY)
                ogs_error("[%s] SEPP exception state", sepp_node->receiver);
                break;
            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message->h.resource.component[0]);
            END
            break;
        DEFAULT
            ogs_error("Invalid API name [%s]", message->h.service.name);
        END
        break;

    default:
        ogs_error("Unknown event %s", sepp_event_get_name(e));
        break;
    }
}
