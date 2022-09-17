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

#include "ogs-dbi.h"

static void timer_send_event(int timer_id, void *data)
{
    int rv;
    ogs_event_t *e = NULL;

    e = ogs_event_new(OGS_EVENT_DBI_POLL_TIMER);
    ogs_assert(e);
    e->timer_id = timer_id;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_event_free(e);
    }
}

void ogs_timer_dbi_poll_change_stream(void *data)
{
    timer_send_event(OGS_TIMER_DBI_POLL_CHANGE_STREAM, data);
}
