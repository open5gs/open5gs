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

#include "pgw-event.h"
#include "pgw-context.h"

#if defined(HAVE_KQUEUE)
/*
 * kqueue does not support TUN/TAP character device
 * So, PGW should use select action in I/O multiplexing
 */
extern const ogs_pollset_actions_t ogs_select_actions;

extern ogs_pollset_actions_t ogs_pollset_actions;
extern bool ogs_pollset_actions_initialized;

static void pollset_action_setup(void)
{
    ogs_pollset_actions = ogs_select_actions;
    ogs_pollset_actions_initialized = true;
}
#endif

#define EVENT_POOL 32 /* FIXME : 32 */
static OGS_POOL(pool, pgw_event_t);

void pgw_event_init(void)
{
    ogs_pool_init(&pool, EVENT_POOL);

#if defined(HAVE_KQUEUE)
    pollset_action_setup();
#endif

    pgw_self()->queue = ogs_queue_create(EVENT_POOL);
    ogs_assert(pgw_self()->queue);
    pgw_self()->timer_mgr = ogs_timer_mgr_create();
    ogs_assert(pgw_self()->timer_mgr);
    pgw_self()->pollset = ogs_pollset_create();
    ogs_assert(pgw_self()->pollset);
}

void pgw_event_term(void)
{
    ogs_queue_term(pgw_self()->queue);
    ogs_pollset_notify(pgw_self()->pollset);
}

void pgw_event_final(void)
{
    if (pgw_self()->pollset)
        ogs_pollset_destroy(pgw_self()->pollset);
    if (pgw_self()->timer_mgr)
        ogs_timer_mgr_destroy(pgw_self()->timer_mgr);
    if (pgw_self()->queue)
        ogs_queue_destroy(pgw_self()->queue);

    ogs_pool_final(&pool);
}

pgw_event_t *pgw_event_new(pgw_event_e id)
{
    pgw_event_t *e = NULL;

    ogs_pool_alloc(&pool, &e);
    ogs_assert(e);
    memset(e, 0, sizeof(*e));

    e->id = id;

    return e;
}

void pgw_event_free(pgw_event_t *e)
{
    ogs_assert(e);
    ogs_pool_free(&pool, e);
}

const char *pgw_event_get_name(pgw_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG: 
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG: 
        return OGS_FSM_NAME_EXIT_SIG;

    case PGW_EVT_S5C_MESSAGE:
        return "PGW_EVT_S5C_MESSAGE";
    case PGW_EVT_GX_MESSAGE:
        return "PGW_EVT_GX_MESSAGE";

    default: 
       break;
    }

    return "UNKNOWN_EVENT";
}
