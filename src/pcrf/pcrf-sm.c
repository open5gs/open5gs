/*
 * Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>
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

#include "pcrf-sm.h"
#include "pcrf-context.h"
#include "pcrf-event.h"

void pcrf_state_initial(ogs_fsm_t *s, pcrf_event_t *e)
{
    pcrf_sm_debug(e);

    ogs_assert(s);
}

void pcrf_state_final(ogs_fsm_t *s, pcrf_event_t *e)
{
    pcrf_sm_debug(e);

    ogs_assert(s);
}

void pcrf_state_operational(ogs_fsm_t *s, pcrf_event_t *e)
{
    pcrf_sm_debug(e);

    ogs_assert(s);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("No handler for event %s", pcrf_event_get_name(e));
        break;
    }
}
