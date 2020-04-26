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

#define EVENT_POOL 32 /* FIXME : 32 */
static OGS_POOL(pool, upf_event_t);

void upf_event_init(void)
{
    ogs_pool_init(&pool, EVENT_POOL);

    upf_self()->queue = ogs_queue_create(EVENT_POOL);
    ogs_assert(upf_self()->queue);
    upf_self()->timer_mgr = ogs_timer_mgr_create();
    ogs_assert(upf_self()->timer_mgr);
    upf_self()->pollset = ogs_pollset_create();
    ogs_assert(upf_self()->pollset);
}

void upf_event_term(void)
{
    ogs_queue_term(upf_self()->queue);
    ogs_pollset_notify(upf_self()->pollset);
}

void upf_event_final(void)
{
    if (upf_self()->pollset)
        ogs_pollset_destroy(upf_self()->pollset);
    if (upf_self()->timer_mgr)
        ogs_timer_mgr_destroy(upf_self()->timer_mgr);
    if (upf_self()->queue)
        ogs_queue_destroy(upf_self()->queue);

    ogs_pool_final(&pool);
}

upf_event_t *upf_event_new(upf_event_e id)
{
    upf_event_t *e = NULL;

    ogs_pool_alloc(&pool, &e);
    ogs_assert(e);
    e->id = id;

    return e;
}

void upf_event_free(upf_event_t *e)
{
    ogs_assert(e);
    ogs_pool_free(&pool, e);
}

const char *upf_event_get_name(upf_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG: 
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG: 
        return OGS_FSM_NAME_EXIT_SIG;

    case UPF_EVT_N4_MESSAGE:
        return "UPF_EVT_N4_MESSAGE";
    case UPF_EVT_N4_TIMER:
        return "UPF_EVT_N4_TIMER";
    case UPF_EVT_N4_NO_HEARTBEAT:
        return "UPF_EVT_N4_NO_HEARTBEAT";

    default: 
       break;
    }

    return "UNKNOWN_EVENT";
}
