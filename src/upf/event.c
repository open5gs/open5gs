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

static OGS_POOL(pool, upf_event_t);

void upf_event_init(void)
{
    ogs_pool_init(&pool, ogs_app()->pool.event);

#if defined(HAVE_KQUEUE)
    ogs_assert(ogs_app()->pollset);
    ogs_pollset_destroy(ogs_app()->pollset);

    pollset_action_setup();

    ogs_app()->pollset = ogs_pollset_create(ogs_app()->pool.socket);
    ogs_assert(ogs_app()->pollset);
#endif
}

void upf_event_term(void)
{
    ogs_queue_term(ogs_app()->queue);
    ogs_pollset_notify(ogs_app()->pollset);
}

void upf_event_final(void)
{
    ogs_pool_final(&pool);
}

upf_event_t *upf_event_new(upf_event_e id)
{
    upf_event_t *e = NULL;

    ogs_pool_alloc(&pool, &e);
    ogs_assert(e);
    memset(e, 0, sizeof(*e));

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
