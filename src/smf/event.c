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

#include "event.h"
#include "context.h"

static OGS_POOL(pool, smf_event_t);
static ogs_thread_mutex_t smf_event_alloc_mutex;

void smf_event_init(void)
{
    ogs_pool_init(&pool, ogs_app()->pool.event);
    ogs_thread_mutex_init(&smf_event_alloc_mutex);
}

void smf_event_final(void)
{
    ogs_pool_final(&pool);
    ogs_thread_mutex_destroy(&smf_event_alloc_mutex);
}

smf_event_t *smf_event_new(smf_event_e id)
{
    smf_event_t *e = NULL;

    ogs_thread_mutex_lock(&smf_event_alloc_mutex);
    ogs_pool_alloc(&pool, &e);
    ogs_thread_mutex_unlock(&smf_event_alloc_mutex);
    ogs_assert(e);
    memset(e, 0, sizeof(*e));

    e->id = id;

    return e;
}

void smf_event_free(smf_event_t *e)
{
    ogs_assert(e);
    ogs_thread_mutex_lock(&smf_event_alloc_mutex);
    ogs_pool_free(&pool, e);
    ogs_thread_mutex_unlock(&smf_event_alloc_mutex);
}

const char *smf_event_get_name(smf_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG:
        return OGS_FSM_NAME_EXIT_SIG;

    case SMF_EVT_S5C_MESSAGE:
        return "SMF_EVT_S5C_MESSAGE";
    case SMF_EVT_S6B_MESSAGE:
        return "SMF_EVT_S6B_MESSAGE";
    case SMF_EVT_GN_MESSAGE:
        return "SMF_EVT_GN_MESSAGE";
    case SMF_EVT_GX_MESSAGE:
        return "SMF_EVT_GX_MESSAGE";
    case SMF_EVT_GY_MESSAGE:
        return "SMF_EVT_GY_MESSAGE";
    case SMF_EVT_N4_MESSAGE:
        return "SMF_EVT_N4_MESSAGE";
    case SMF_EVT_N4_TIMER:
        return "SMF_EVT_N4_TIMER";
    case SMF_EVT_N4_NO_HEARTBEAT:
        return "SMF_EVT_N4_NO_HEARTBEAT";

    case SMF_EVT_SBI_SERVER:
        return "SMF_EVT_SBI_SERVER";
    case SMF_EVT_SBI_CLIENT:
        return "SMF_EVT_SBI_CLIENT";
    case SMF_EVT_SBI_TIMER:
        return "SMF_EVT_SBI_TIMER";

    case SMF_EVT_NGAP_MESSAGE:
        return "SMF_EVT_NGAP_MESSAGE";
    case SMF_EVT_NGAP_TIMER:
        return "SMF_EVT_NGAP_TIMER";

    case SMF_EVT_5GSM_MESSAGE:
        return "SMF_EVT_5GSM_MESSAGE";
    case SMF_EVT_5GSM_TIMER:
        return "SMF_EVT_5GSM_TIMER";

    default:
       break;
    }

    return "UNKNOWN_EVENT";
}
