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

const char *sgwu_timer_get_name(sgwu_timer_e id)
{
    switch (id) {
    case SGWU_TIMER_ASSOCIATION:
        return "SGWU_TIMER_ASSOCIATION";
    case SGWU_TIMER_NO_HEARTBEAT:
        return "SGWU_TIMER_NO_HEARTBEAT";
    default: 
       break;
    }

    return "UNKNOWN_TIMER";
}

static void timer_send_event(int timer_id, void *data)
{
    int rv;
    sgwu_event_t *e = NULL;
    ogs_assert(data);

    e = sgwu_event_new(SGWU_EVT_SXA_TIMER);
    e->timer_id = timer_id;
    e->pfcp_node = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        sgwu_event_free(e);
    }
}

void sgwu_timer_association(void *data)
{
    timer_send_event(SGWU_TIMER_ASSOCIATION, data);
}

void sgwu_timer_no_heartbeat(void *data)
{
    timer_send_event(SGWU_TIMER_NO_HEARTBEAT, data);
}
