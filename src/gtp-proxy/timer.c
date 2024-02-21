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

#include "timer.h"
#include "event.h"
#include "context.h"

const char *sgwc_timer_get_name(sgwc_timer_e id)
{
    switch (id) {
    case SGWC_TIMER_PFCP_ASSOCIATION:
        return "SGWC_TIMER_PFCP_ASSOCIATION";
    case SGWC_TIMER_PFCP_NO_HEARTBEAT:
        return "SGWC_TIMER_PFCP_NO_HEARTBEAT";
    default: 
       break;
    }

    return "UNKNOWN_TIMER";
}

static void timer_send_event(int timer_id, void *data)
{
    int rv;
    sgwc_event_t *e = NULL;
    ogs_assert(data);

    switch (timer_id) {
    case SGWC_TIMER_PFCP_ASSOCIATION:
    case SGWC_TIMER_PFCP_NO_HEARTBEAT:
        e = sgwc_event_new(SGWC_EVT_SXA_TIMER);
        ogs_assert(e);
        e->timer_id = timer_id;
        e->pfcp_node = data;
        break;
    default:
        ogs_fatal("Unknown timer id[%d]", timer_id);
        ogs_assert_if_reached();
        break;
    }

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed [%d] in %s",
                (int)rv, sgwc_timer_get_name(e->timer_id));
        sgwc_event_free(e);
    }
}

void sgwc_timer_pfcp_association(void *data)
{
    timer_send_event(SGWC_TIMER_PFCP_ASSOCIATION, data);
}

void sgwc_timer_pfcp_no_heartbeat(void *data)
{
    timer_send_event(SGWC_TIMER_PFCP_NO_HEARTBEAT, data);
}
