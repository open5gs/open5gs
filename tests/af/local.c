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

const char *af_local_get_name(af_local_e id)
{
    switch (id) {
    case AF_LOCAL_DISCOVER_AND_SEND:
        return "AF_LOCAL_DISCOVER_AND_SEND";
    default: 
       break;
    }

    return "UNKNOWN_LOCAL";
}

void af_local_discover_and_send(OpenAPI_nf_type_e target_nf_type,
        af_sess_t *sess, void *data,
        ogs_sbi_request_t *(*build)(af_sess_t *sess, void *data))
{
    int rv;
    af_event_t *e = NULL;

    e = af_event_new(AF_EVT_SBI_LOCAL);
    ogs_assert(e);

    e->local_id = AF_LOCAL_DISCOVER_AND_SEND;
    e->sess = sess;

    e->local.target_nf_type = target_nf_type;
    e->local.data = data;
    e->local.build = build;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed [%d] in %s",
                (int)rv, af_timer_get_name(e->timer_id));
        af_event_free(e);
    } else {
        ogs_pollset_notify(ogs_app()->pollset);
    }
}

void af_local_send_to_pcf(
        af_sess_t *sess, void *data,
        ogs_sbi_request_t *(*build)(af_sess_t *sess, void *data))
{
    int rv;
    af_event_t *e = NULL;

    e = af_event_new(AF_EVT_SBI_LOCAL);
    ogs_assert(e);

    e->local_id = AF_LOCAL_SEND_TO_PCF;
    e->sess = sess;

    e->local.data = data;
    e->local.build = build;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed [%d] in %s",
                (int)rv, af_timer_get_name(e->timer_id));
        af_event_free(e);
    } else {
        ogs_pollset_notify(ogs_app()->pollset);
    }
}
