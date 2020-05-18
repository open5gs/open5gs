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
static OGS_POOL(pool, nrf_event_t);

void nrf_event_init(void)
{
    ogs_pool_init(&pool, EVENT_POOL);

    nrf_self()->queue = ogs_queue_create(EVENT_POOL);
    ogs_assert(nrf_self()->queue);
    nrf_self()->timer_mgr = ogs_timer_mgr_create();
    ogs_assert(nrf_self()->timer_mgr);
    nrf_self()->pollset = ogs_pollset_create();
    ogs_assert(nrf_self()->pollset);
}

void nrf_event_final(void)
{
    if (nrf_self()->pollset)
        ogs_pollset_destroy(nrf_self()->pollset);
    if (nrf_self()->timer_mgr)
        ogs_timer_mgr_destroy(nrf_self()->timer_mgr);
    if (nrf_self()->queue)
        ogs_queue_destroy(nrf_self()->queue);

    ogs_pool_final(&pool);
}

nrf_event_t *nrf_event_new(nrf_event_e id)
{
    nrf_event_t *e = NULL;

    ogs_pool_alloc(&pool, &e);
    ogs_assert(e);
    memset(e, 0, sizeof(*e));

    e->id = id;

    return e;
}

void nrf_event_free(nrf_event_t *e)
{
    ogs_assert(e);
    ogs_pool_free(&pool, e);
}

const char *nrf_event_get_name(nrf_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG: 
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG: 
        return OGS_FSM_NAME_EXIT_SIG;

    case NRF_EVT_SBI_SERVER:
        return "NRF_EVT_SBI_SERVER";
    case NRF_EVT_SBI_CLIENT:
        return "NRF_EVT_SBI_CLIENT";
    case NRF_EVT_SBI_TIMER:
        return "NRF_EVT_SBI_TIMER";

    default: 
       break;
    }

    return "UNKNOWN_EVENT";
}
