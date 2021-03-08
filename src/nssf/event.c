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

#include "event.h"
#include "context.h"

static OGS_POOL(pool, nssf_event_t);

void nssf_event_init(void)
{
    ogs_pool_init(&pool, ogs_app()->pool.event);
}

void nssf_event_final(void)
{
    ogs_pool_final(&pool);
}

nssf_event_t *nssf_event_new(nssf_event_e id)
{
    nssf_event_t *e = NULL;

    ogs_pool_alloc(&pool, &e);
    ogs_assert(e);
    memset(e, 0, sizeof(*e));

    e->id = id;

    return e;
}

void nssf_event_free(nssf_event_t *e)
{
    ogs_assert(e);
    ogs_pool_free(&pool, e);
}

const char *nssf_event_get_name(nssf_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG: 
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG: 
        return OGS_FSM_NAME_EXIT_SIG;

    case NSSF_EVT_SBI_SERVER:
        return "NSSF_EVT_SBI_SERVER";
    case NSSF_EVT_SBI_CLIENT:
        return "NSSF_EVT_SBI_CLIENT";
    case NSSF_EVT_SBI_TIMER:
        return "NSSF_EVT_SBI_TIMER";

    default: 
       break;
    }

    return "UNKNOWN_EVENT";
}
