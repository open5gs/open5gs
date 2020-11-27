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

#include "context.h"

#include "sbi-path.h"
#include "nnrf-handler.h"

void udm_nf_fsm_init(ogs_sbi_nf_instance_t *nf_instance)
{
    udm_event_t e;

    ogs_assert(nf_instance);

    memset(&e, 0, sizeof(e));
    e.sbi.data = nf_instance;

    ogs_fsm_create(&nf_instance->sm,
            udm_nf_state_initial, udm_nf_state_final);
    ogs_fsm_init(&nf_instance->sm, &e);
}

void udm_nf_fsm_fini(ogs_sbi_nf_instance_t *nf_instance)
{
    udm_event_t e;

    ogs_assert(nf_instance);

    memset(&e, 0, sizeof(e));
    e.sbi.data = nf_instance;

    ogs_fsm_fini(&nf_instance->sm, &e);
    ogs_fsm_delete(&nf_instance->sm);
}

void udm_nf_state_initial(ogs_fsm_t *s, udm_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(s);
    ogs_assert(e);

    udm_sm_debug(e);

    nf_instance = e->sbi.data;
    ogs_assert(nf_instance);
    ogs_assert(nf_instance->id);

    ogs_assert(nf_instance->t_registration_interval);
    nf_instance->t_registration_interval->cb =
            udm_timer_nf_instance_registration_interval;
    ogs_assert(nf_instance->t_heartbeat_interval);
    nf_instance->t_heartbeat_interval->cb =
            udm_timer_nf_instance_heartbeat_interval;
    ogs_assert(nf_instance->t_no_heartbeat);
    nf_instance->t_no_heartbeat->cb = udm_timer_nf_instance_no_heartbeat;
    ogs_assert(nf_instance->t_validity);
    nf_instance->t_validity->cb = udm_timer_nf_instance_validity;

    if (NF_INSTANCE_IS_SELF(nf_instance->id)) {
        OGS_FSM_TRAN(s, &udm_nf_state_will_register);
    } else {
        OGS_FSM_TRAN(s, &udm_nf_state_registered);
    }
}

void udm_nf_state_final(ogs_fsm_t *s, udm_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    udm_sm_debug(e);
}

void udm_nf_state_will_register(ogs_fsm_t *s, udm_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_client_t *client = NULL;
    ogs_sbi_message_t *message = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(s);
    ogs_assert(e);

    udm_sm_debug(e);

    nf_instance = e->sbi.data;
    ogs_assert(nf_instance);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        if (NF_INSTANCE_IS_SELF(nf_instance->id))
            ogs_timer_start(nf_instance->t_registration_interval,
                ogs_app()->time.message.sbi.nf_register_interval);

        ogs_nnrf_nfm_send_nf_register(nf_instance);
        break;

    case OGS_FSM_EXIT_SIG:
        if (NF_INSTANCE_IS_SELF(nf_instance->id))
            ogs_timer_stop(nf_instance->t_registration_interval);
        break;

    case UDM_EVT_SBI_CLIENT:
        message = e->sbi.message;
        ogs_assert(message);

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)

                if (message->res_status == OGS_SBI_HTTP_STATUS_OK ||
                    message->res_status == OGS_SBI_HTTP_STATUS_CREATED) {
                    udm_nnrf_handle_nf_register(nf_instance, message);
                    OGS_FSM_TRAN(s, &udm_nf_state_registered);
                } else {
                    ogs_error("[%s] HTTP response error [%d]",
                            nf_instance->id, message->res_status);
                    OGS_FSM_TRAN(s, &udm_nf_state_exception);
                }
                break;

            DEFAULT
                ogs_error("[%s] Invalid resource name [%s]",
                        nf_instance->id, message->h.resource.component[0]);
            END
            break;

        DEFAULT
            ogs_error("[%s] Invalid API name [%s]",
                    nf_instance->id, message->h.service.name);
        END
        break;

    case UDM_EVT_SBI_TIMER:
        switch(e->timer_id) {
        case UDM_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
            client = nf_instance->client;
            ogs_assert(client);
            addr = client->node.addr;
            ogs_assert(addr);

            ogs_warn("[%s] Retry to registration with NRF", nf_instance->id);

            if (NF_INSTANCE_IS_SELF(nf_instance->id))
                ogs_timer_start(nf_instance->t_registration_interval,
                    ogs_app()->time.message.sbi.nf_register_interval);

            ogs_nnrf_nfm_send_nf_register(nf_instance);
            break;

        default:
            ogs_error("[%s] Unknown timer[%s:%d]", nf_instance->id,
                    udm_timer_get_name(e->timer_id), e->timer_id);
        }
        break;

    default:
        ogs_error("Unknown event %s", udm_event_get_name(e));
        break;
    }
}

void udm_nf_state_registered(ogs_fsm_t *s, udm_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_client_t *client = NULL;
    ogs_sbi_message_t *message = NULL;
    ogs_assert(s);
    ogs_assert(e);

    udm_sm_debug(e);

    nf_instance = e->sbi.data;
    ogs_assert(nf_instance);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        if (NF_INSTANCE_IS_SELF(nf_instance->id)) {
            ogs_info("[%s] NF registered", nf_instance->id);

            client = nf_instance->client;
            ogs_assert(client);

            if (nf_instance->time.heartbeat_interval) {
                ogs_timer_start(nf_instance->t_heartbeat_interval,
                    ogs_time_from_sec(nf_instance->time.heartbeat_interval));
                ogs_timer_start(nf_instance->t_no_heartbeat,
                    ogs_time_from_sec(
                        nf_instance->time.heartbeat_interval +
                        ogs_app()->time.nf_instance.no_heartbeat_margin));
            }

            ogs_nnrf_nfm_send_nf_status_subscribe(client,
                    udm_self()->nf_type, nf_instance->id);
        }

        break;

    case OGS_FSM_EXIT_SIG:
        if (NF_INSTANCE_IS_SELF(nf_instance->id)) {
            ogs_info("[%s] NF de-registered", nf_instance->id);

            if (nf_instance->time.heartbeat_interval) {
                ogs_timer_stop(nf_instance->t_heartbeat_interval);
                ogs_timer_stop(nf_instance->t_no_heartbeat);
            }

            if (!OGS_FSM_CHECK(&nf_instance->sm, udm_nf_state_exception)) {
                ogs_nnrf_nfm_send_nf_de_register(nf_instance);
            }
        }
        break;

    case UDM_EVT_SBI_CLIENT:
        message = e->sbi.message;
        ogs_assert(message);

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)

                if (message->res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT ||
                    message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                    if (nf_instance->time.heartbeat_interval)
                        ogs_timer_start(nf_instance->t_no_heartbeat,
                            ogs_time_from_sec(
                                nf_instance->time.heartbeat_interval +
                                ogs_app()->time.nf_instance.
                                    no_heartbeat_margin));
                } else {
                    ogs_warn("[%s] HTTP response error [%d]",
                            nf_instance->id, message->res_status);
                    OGS_FSM_TRAN(s, &udm_nf_state_exception);
                }

                break;

            DEFAULT
                ogs_error("[%s] Invalid resource name [%s]",
                        nf_instance->id, message->h.resource.component[0]);
            END
            break;

        DEFAULT
            ogs_error("[%s] Invalid API name [%s]",
                    nf_instance->id, message->h.service.name);
        END
        break;

    case UDM_EVT_SBI_TIMER:
        switch(e->timer_id) {
        case UDM_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
            if (nf_instance->time.heartbeat_interval)
                ogs_timer_start(nf_instance->t_heartbeat_interval,
                    ogs_time_from_sec(nf_instance->time.heartbeat_interval));

            ogs_nnrf_nfm_send_nf_update(nf_instance);
            break;

        case UDM_TIMER_NF_INSTANCE_NO_HEARTBEAT:
            ogs_error("[%s] No heartbeat", nf_instance->id);
            OGS_FSM_TRAN(s, &udm_nf_state_will_register);
            break;

        case UDM_TIMER_NF_INSTANCE_VALIDITY:
            if (NF_INSTANCE_IS_OTHERS(nf_instance->id)) {
                ogs_info("[%s] NF expired", nf_instance->id);
                OGS_FSM_TRAN(s, &udm_nf_state_de_registered);
            }
            break;

        default:
            ogs_error("[%s] Unknown timer[%s:%d]", nf_instance->id,
                    udm_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;

    default:
        ogs_error("[%s] Unknown event %s",
                nf_instance->id, udm_event_get_name(e));
        break;
    }
}

void udm_nf_state_de_registered(ogs_fsm_t *s, udm_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_assert(s);
    ogs_assert(e);

    udm_sm_debug(e);

    nf_instance = e->sbi.data;
    ogs_assert(nf_instance);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        if (NF_INSTANCE_IS_SELF(nf_instance->id)) {
            ogs_info("[%s] NF de-registered", nf_instance->id);
        }
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("[%s] Unknown event %s",
                nf_instance->id, udm_event_get_name(e));
        break;
    }
}

void udm_nf_state_exception(ogs_fsm_t *s, udm_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_client_t *client = NULL;
    ogs_sbi_message_t *message = NULL;
    ogs_sockaddr_t *addr = NULL;
    ogs_assert(s);
    ogs_assert(e);

    udm_sm_debug(e);

    nf_instance = e->sbi.data;
    ogs_assert(nf_instance);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        if (NF_INSTANCE_IS_SELF(nf_instance->id))
            ogs_timer_start(nf_instance->t_registration_interval,
                ogs_app()->time.message.sbi.
                    nf_register_interval_in_exception);
        break;

    case OGS_FSM_EXIT_SIG:
        if (NF_INSTANCE_IS_SELF(nf_instance->id))
            ogs_timer_stop(nf_instance->t_registration_interval);
        break;

    case UDM_EVT_SBI_TIMER:
        switch(e->timer_id) {
        case UDM_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
            client = nf_instance->client;
            ogs_assert(client);
            addr = client->node.addr;
            ogs_assert(addr);

            ogs_warn("[%s] Retry to registration with NRF", nf_instance->id);

            OGS_FSM_TRAN(s, &udm_nf_state_will_register);
            break;

        default:
            ogs_error("[%s] Unknown timer[%s:%d]", nf_instance->id,
                    udm_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;

    case UDM_EVT_SBI_CLIENT:
        message = e->sbi.message;
        ogs_assert(message);

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
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
        ogs_error("[%s] Unknown event %s",
                nf_instance->id, udm_event_get_name(e));
        break;
    }
}
