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

static OGS_POOL(pool, sgwc_event_t);

void sgwc_event_init(void)
{
    ogs_pool_init(&pool, ogs_app()->pool.event);
}

void sgwc_event_term(void)
{
    ogs_queue_term(ogs_app()->queue);
    ogs_pollset_notify(ogs_app()->pollset);
}

void sgwc_event_final(void)
{
    ogs_pool_final(&pool);
}

sgwc_event_t *sgwc_event_new(sgwc_event_e id)
{
    sgwc_event_t *e = NULL;

    ogs_pool_alloc(&pool, &e);
    ogs_assert(e);
    memset(e, 0, sizeof(*e));

    e->id = id;

    return e;
}

void sgwc_event_free(sgwc_event_t *e)
{
    ogs_assert(e);
    ogs_pool_free(&pool, e);
}

const char *sgwc_event_get_name(sgwc_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG: 
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG: 
        return OGS_FSM_NAME_EXIT_SIG;

    case SGWC_EVT_S11_MESSAGE:
        return "SGWC_EVT_S11_MESSAGE";
    case SGWC_EVT_S5C_MESSAGE:
        return "SGWC_EVT_S5C_MESSAGE";

    case SGWC_EVT_SXA_MESSAGE:
        return "SGWC_EVT_SXA_MESSAGE";
    case SGWC_EVT_SXA_TIMER:
        return "SGWC_EVT_SXA_TIMER";
    case SGWC_EVT_SXA_NO_HEARTBEAT:
        return "SGWC_EVT_SXA_NO_HEARTBEAT";

    default: 
       break;
    }

    return "UNKNOWN_EVENT";
}
