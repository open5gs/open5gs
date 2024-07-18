/*
 * Copyright (C) 2019,2024 by Sukchan Lee <acetcom@gmail.com>
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

#include "sbi-path.h"
#include "nnrf-handler.h"

void nrf_nf_fsm_init(ogs_sbi_nf_instance_t *nf_instance)
{
    nrf_event_t e;

    ogs_assert(nf_instance);
    memset(&e, 0, sizeof(e));
    e.nf_instance = nf_instance;

    ogs_fsm_init(&nf_instance->sm,
        nrf_nf_state_initial, nrf_nf_state_final, &e);
}

void nrf_nf_fsm_fini(ogs_sbi_nf_instance_t *nf_instance)
{
    nrf_event_t e;

    ogs_assert(nf_instance);
    memset(&e, 0, sizeof(e));
    e.nf_instance = nf_instance;

    ogs_fsm_fini(&nf_instance->sm, &e);
}

void nrf_nf_state_initial(ogs_fsm_t *s, nrf_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(s);
    ogs_assert(e);

    nrf_sm_debug(e);

    nf_instance = e->nf_instance;
    ogs_assert(nf_instance);

    nf_instance->t_no_heartbeat = ogs_timer_add(ogs_app()->timer_mgr,
            nrf_timer_nf_instance_no_heartbeat, nf_instance);
    ogs_assert(nf_instance->t_no_heartbeat);

    OGS_FSM_TRAN(s, &nrf_nf_state_will_register);
}

void nrf_nf_state_final(ogs_fsm_t *s, nrf_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(s);
    ogs_assert(e);

    nrf_sm_debug(e);

    nf_instance = e->nf_instance;
    ogs_assert(nf_instance);

    ogs_timer_delete(nf_instance->t_no_heartbeat);
}

void nrf_nf_state_will_register(ogs_fsm_t *s, nrf_event_t *e)
{
    bool handled;
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    nrf_sm_debug(e);

    nf_instance = e->nf_instance;
    ogs_assert(nf_instance);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
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
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)

                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_PUT)

                    handled = nrf_nnrf_handle_nf_register(
                            nf_instance, stream, message);
                    if (handled == false)
                        OGS_FSM_TRAN(s, nrf_nf_state_exception);
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid HTTP method [%s]",
                            nf_instance->id, message->h.method);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, message,
                            "Invalid HTTP method", message->h.method, NULL));
                END
                break;

            DEFAULT
                ogs_error("[%s] Invalid resource name [%s]",
                        nf_instance->id, message->h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, message,
                        "Invalid resource name",
                        message->h.resource.component[0], NULL));
            END
            break;

        DEFAULT
            ogs_error("[%s] Invalid API name [%s]",
                    nf_instance->id, message->h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, message,
                    "Invalid resource name", message->h.service.name,
                    NULL));
        END

        OGS_FSM_TRAN(s, nrf_nf_state_registered);
        break;

    default:
        ogs_error("[%s] Unknown event %s",
                nf_instance->id, nrf_event_get_name(e));
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                message, "Unknown event", nrf_event_get_name(e),
                NULL));
        OGS_FSM_TRAN(s, nrf_nf_state_exception);
        break;
    }
}

void nrf_nf_state_registered(ogs_fsm_t *s, nrf_event_t *e)
{
    bool handled;
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *message = NULL;
    ogs_sbi_response_t *response = NULL;

    ogs_assert(s);
    ogs_assert(e);

    nrf_sm_debug(e);

    nf_instance = e->nf_instance;
    ogs_assert(nf_instance);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        ogs_info("[%s] NF registered [Heartbeat:%ds]",
                nf_instance->id, nf_instance->time.heartbeat_interval);
        if (nf_instance->time.heartbeat_interval) {
            ogs_timer_start(nf_instance->t_no_heartbeat,
                    ogs_time_from_sec(
                        nf_instance->time.heartbeat_interval +
                        ogs_local_conf()->time.nf_instance.
                        no_heartbeat_margin));
        }

        ogs_assert(true ==
            nrf_nnrf_nfm_send_nf_status_notify_all(
                OpenAPI_notification_event_type_NF_REGISTERED, nf_instance));
        break;

    case OGS_FSM_EXIT_SIG:
        ogs_info("[%s] NF de-registered", nf_instance->id);
        if (nf_instance->time.heartbeat_interval) {
            ogs_timer_stop(nf_instance->t_no_heartbeat);
        }

        ogs_assert(true ==
            nrf_nnrf_nfm_send_nf_status_notify_all(
                OpenAPI_notification_event_type_NF_DEREGISTERED, nf_instance));
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
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)

                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_PUT)
                CASE(OGS_SBI_HTTP_METHOD_PATCH)
                    if (nf_instance->time.heartbeat_interval) {
                        ogs_timer_start(nf_instance->t_no_heartbeat,
                            ogs_time_from_sec(
                                nf_instance->time.heartbeat_interval +
                                ogs_local_conf()->time.nf_instance.
                                    no_heartbeat_margin));
                    }

                    handled = nrf_nnrf_handle_nf_update(
                            nf_instance, stream, message);
                    if (handled == false)
                        OGS_FSM_TRAN(s, nrf_nf_state_exception);
                    break;

                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    response = ogs_sbi_build_response(
                            message, OGS_SBI_HTTP_STATUS_NO_CONTENT);
                    ogs_assert(response);
                    ogs_assert(true ==
                            ogs_sbi_server_send_response(stream, response));
                    OGS_FSM_TRAN(s, nrf_nf_state_de_registered);
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid HTTP method [%s]",
                            nf_instance->id, message->h.method);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, message,
                            "Invalid HTTP method", message->h.method, NULL));
                END
                break;

            DEFAULT
                ogs_error("[%s] Invalid resource name [%s]",
                        nf_instance->id, message->h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, message,
                        "Invalid resource name",
                        message->h.resource.component[0], NULL));
            END
            break;

        DEFAULT
            ogs_error("[%s] Invalid API name [%s]",
                    nf_instance->id, message->h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, message,
                    "Invalid resource name", message->h.service.name,
                    NULL));
        END
        break;

    default:
        ogs_error("[%s] Unknown event %s",
                nf_instance->id, nrf_event_get_name(e));
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                message, "Unknown event", nrf_event_get_name(e),
                NULL));
        OGS_FSM_TRAN(s, nrf_nf_state_exception);
        break;
    }
}

void nrf_nf_state_de_registered(ogs_fsm_t *s, nrf_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    nrf_sm_debug(e);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", nrf_event_get_name(e));
        break;
    }
}

void nrf_nf_state_exception(ogs_fsm_t *s, nrf_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    nrf_sm_debug(e);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", nrf_event_get_name(e));
        break;
    }
}
