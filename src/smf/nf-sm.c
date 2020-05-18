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

#include "sbi-path.h"
#include "nnrf-handler.h"

void smf_nf_fsm_init(ogs_sbi_nf_instance_t *nf_instance)
{
    smf_event_t e;

    ogs_assert(nf_instance);

    e.sbi.data = nf_instance;

    ogs_fsm_create(&nf_instance->sm,
            smf_nf_state_initial, smf_nf_state_final);
    ogs_fsm_init(&nf_instance->sm, &e);
}

void smf_nf_fsm_fini(ogs_sbi_nf_instance_t *nf_instance)
{
    smf_event_t e;

    ogs_assert(nf_instance);
    e.sbi.data = nf_instance;

    ogs_fsm_fini(&nf_instance->sm, &e);
    ogs_fsm_delete(&nf_instance->sm);
}

void smf_nf_state_initial(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    nf_instance = e->sbi.data;
    ogs_assert(nf_instance);
    ogs_assert(nf_instance->id);

    nf_instance->t_registration_interval = ogs_timer_add(smf_self()->timer_mgr,
            smf_timer_nf_instance_registration_interval, nf_instance);
    ogs_assert(nf_instance->t_registration_interval);
    nf_instance->t_heartbeat_interval = ogs_timer_add(smf_self()->timer_mgr,
            smf_timer_nf_instance_heartbeat_interval, nf_instance);
    ogs_assert(nf_instance->t_heartbeat_interval);
    nf_instance->t_heartbeat = ogs_timer_add(smf_self()->timer_mgr,
            smf_timer_nf_instance_heartbeat, nf_instance);
    ogs_assert(nf_instance->t_heartbeat);
    nf_instance->t_validity = ogs_timer_add(smf_self()->timer_mgr,
            smf_timer_nf_instance_validity, nf_instance);
    ogs_assert(nf_instance->t_validity);

    if (NF_INSTANCE_IS_SELF(nf_instance->id)) {
        OGS_FSM_TRAN(s, &smf_nf_state_will_register);
    } else {
        OGS_FSM_TRAN(s, &smf_nf_state_registered);
    }
}

void smf_nf_state_final(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    nf_instance = e->sbi.data;
    ogs_assert(nf_instance);

    ogs_timer_delete(nf_instance->t_registration_interval);
    ogs_timer_delete(nf_instance->t_heartbeat_interval);
    ogs_timer_delete(nf_instance->t_heartbeat);
    ogs_timer_delete(nf_instance->t_validity);
}

void smf_nf_state_will_register(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_client_t *client = NULL;
    ogs_sbi_message_t *message = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    nf_instance = e->sbi.data;
    ogs_assert(nf_instance);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        ogs_timer_start(nf_instance->t_registration_interval,
                smf_timer_cfg(SMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL)->
                    duration);

        smf_sbi_send_nf_register(nf_instance);
        break;

    case OGS_FSM_EXIT_SIG:
        ogs_timer_stop(nf_instance->t_registration_interval);
        break;

    case SMF_EVT_SBI_CLIENT:
        message = e->sbi.message;
        ogs_assert(message);

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NRF_NFM)

            SWITCH(message->h.resource.name)
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)

                if (message->res_status == OGS_SBI_HTTP_STATUS_OK ||
                    message->res_status == OGS_SBI_HTTP_STATUS_CREATED) {
                    smf_nnrf_handle_nf_register(nf_instance, message);
                    OGS_FSM_TRAN(s, &smf_nf_state_registered);
                } else {
                    ogs_error("HTTP Response Status Code [%d]",
                            message->res_status);
                    OGS_FSM_TRAN(s, &smf_nf_state_exception);
                }
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message->h.resource.name);
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", message->h.service.name);
        END
        break;

    case SMF_EVT_SBI_TIMER:
        switch(e->timer_id) {
        case SMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
            client = nf_instance->client;
            ogs_assert(client);
            addr = client->addr;
            ogs_assert(addr);

            ogs_warn("Retry to registration with NRF [%s]", nf_instance->id);

            ogs_timer_start(nf_instance->t_registration_interval,
                smf_timer_cfg(SMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL)->
                    duration);

            smf_sbi_send_nf_register(nf_instance);
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    smf_timer_get_name(e->timer_id), e->timer_id);
        }
        break;

    default:
        ogs_error("Unknown event %s", smf_event_get_name(e));
        break;
    }
}

void smf_nf_state_registered(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_client_t *client = NULL;
    ogs_sbi_message_t *message = NULL;
    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    nf_instance = e->sbi.data;
    ogs_assert(nf_instance);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        client = nf_instance->client;
        ogs_assert(client);
        if (NF_INSTANCE_IS_SELF(nf_instance->id)) {
            ogs_info("NF registered [%s]", nf_instance->id);

            if (nf_instance->time.heartbeat) {
                ogs_timer_start(nf_instance->t_heartbeat_interval,
                        ogs_time_from_sec(nf_instance->time.heartbeat));
                ogs_timer_start(nf_instance->t_heartbeat,
                        ogs_time_from_sec(nf_instance->time.heartbeat *
                            OGS_SBI_HEARTBEAT_RETRYCOUNT));
            }

            smf_sbi_send_nf_status_subscribe(client,
                    smf_self()->nf_type, nf_instance->id);
        }

        break;

    case OGS_FSM_EXIT_SIG:
        if (NF_INSTANCE_IS_SELF(nf_instance->id)) {
            ogs_info("NF de-registered [%s]", nf_instance->id);

            if (nf_instance->time.heartbeat) {
                ogs_timer_stop(nf_instance->t_heartbeat_interval);
                ogs_timer_stop(nf_instance->t_heartbeat);
            }

            smf_sbi_send_nf_de_register(nf_instance);
        }
        break;

    case SMF_EVT_SBI_CLIENT:
        message = e->sbi.message;
        ogs_assert(message);

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NRF_NFM)

            SWITCH(message->h.resource.name)
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)

                if (message->res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT ||
                    message->res_status == OGS_SBI_HTTP_STATUS_OK) {
                    if (nf_instance->time.heartbeat)
                        ogs_timer_start(nf_instance->t_heartbeat,
                                ogs_time_from_sec(nf_instance->time.heartbeat *
                                    OGS_SBI_HEARTBEAT_RETRYCOUNT));
                } else {
                    ogs_error("HTTP response error : %d", message->res_status);
                }

                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message->h.resource.name);
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", message->h.service.name);
        END
        break;

    case SMF_EVT_SBI_TIMER:
        switch(e->timer_id) {
        case SMF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
            if (nf_instance->time.heartbeat) {
                ogs_timer_start(nf_instance->t_heartbeat_interval,
                        ogs_time_from_sec(nf_instance->time.heartbeat));
            }

            smf_sbi_send_nf_update(nf_instance);
            break;

        case SMF_TIMER_NF_INSTANCE_HEARTBEAT:
            OGS_FSM_TRAN(s, &smf_nf_state_will_register);
            break;

        case SMF_TIMER_NF_INSTANCE_VALIDITY:
            if (NF_INSTANCE_IS_OTHERS(nf_instance->id)) {
                ogs_info("NF expired [%s]", nf_instance->id);
                OGS_FSM_TRAN(s, &smf_nf_state_de_registered);
            }
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    smf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;

    default:
        ogs_error("Unknown event %s", smf_event_get_name(e));
        break;
    }
}

void smf_nf_state_de_registered(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    nf_instance = e->sbi.data;
    ogs_assert(nf_instance);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        if (NF_INSTANCE_IS_SELF(nf_instance->id)) {
            ogs_info("NF de-registered [%s]", nf_instance->id);
        }
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("Unknown event %s", smf_event_get_name(e));
        break;
    }
}

void smf_nf_state_exception(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_client_t *client = NULL;
    ogs_sockaddr_t *addr = NULL;
    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    nf_instance = e->sbi.data;
    ogs_assert(nf_instance);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        if (NF_INSTANCE_IS_SELF(nf_instance->id)) {
            ogs_timer_start(nf_instance->t_registration_interval,
                smf_timer_cfg(SMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL)->
                    duration);
        }
        break;

    case OGS_FSM_EXIT_SIG:
        if (NF_INSTANCE_IS_SELF(nf_instance->id)) {
            ogs_timer_stop(nf_instance->t_registration_interval);
        }
        break;

    case SMF_EVT_SBI_TIMER:
        switch(e->timer_id) {
        case SMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
            client = nf_instance->client;
            ogs_assert(client);
            addr = client->addr;
            ogs_assert(addr);

            ogs_warn("Retry to registration with NRF [%s]", nf_instance->id);

            OGS_FSM_TRAN(s, &smf_nf_state_will_register);
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    smf_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;

    default:
        ogs_error("Unknown event %s", smf_event_get_name(e));
        break;
    }
}
