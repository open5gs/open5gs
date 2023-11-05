/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-proto.h"

const char *OGS_EVENT_NAME_SBI_SERVER = "OGS_EVENT_NAME_SBI_SERVER";
const char *OGS_EVENT_NAME_SBI_CLIENT = "OGS_EVENT_NAME_SBI_CLIENT";
const char *OGS_EVENT_NAME_SBI_TIMER = "OGS_EVENT_NAME_SBI_TIMER";

void *ogs_event_size(int id, size_t size)
{
    ogs_event_t *e = NULL;

    e = ogs_calloc(1, size);
    ogs_assert(e);

    e->id = id;

    return e;
}

ogs_event_t *ogs_event_new(int id)
{
    return ogs_event_size(id, OGS_EVENT_SIZE);
}

void ogs_event_free(void *e)
{
    ogs_assert(e);
    ogs_free(e);
}

const char *ogs_event_get_name(ogs_event_t *e)
{
    if (e == NULL) {
        return OGS_FSM_NAME_INIT_SIG;
    }

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG:
        return OGS_FSM_NAME_EXIT_SIG;

    case OGS_EVENT_SBI_SERVER:
        return OGS_EVENT_NAME_SBI_SERVER;
    case OGS_EVENT_SBI_CLIENT:
        return OGS_EVENT_NAME_SBI_CLIENT;
    case OGS_EVENT_SBI_TIMER:
        return OGS_EVENT_NAME_SBI_TIMER;

    default:
        break;
    }

    ogs_error("Unknown Event[%d]", e->id);
    return "UNKNOWN_EVENT";
}
