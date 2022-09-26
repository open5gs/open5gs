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

#include "ogs-dbi.h"

int ogs_dbi_process_change_stream(const bson_t *document)
{
    int rv;

    ogs_event_t *e = NULL;

    e = ogs_event_new(OGS_EVENT_DBI_MESSAGE);
    ogs_assert(e);
    e->dbi.document = bson_copy(document);
    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        bson_destroy((bson_t*)e->dbi.document);
        ogs_event_free(e);
    } else {
        ogs_pollset_notify(ogs_app()->pollset);
    }

    return OGS_OK;
}
