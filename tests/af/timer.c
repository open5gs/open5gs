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

const char *af_timer_get_name(af_timer_e id)
{
    switch (id) {
    case AF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        return "AF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL";
    case AF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        return "AF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL";
    case AF_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        return "AF_TIMER_NF_INSTANCE_NO_HEARTBEAT";
    case AF_TIMER_NF_INSTANCE_VALIDITY:
        return "AF_TIMER_NF_INSTANCE_VALIDITY";
    case AF_TIMER_SUBSCRIPTION_VALIDITY:
        return "AF_TIMER_SUBSCRIPTION_VALIDITY";
    case AF_TIMER_SBI_CLIENT_WAIT:
        return "AF_TIMER_SBI_CLIENT_WAIT";
    default: 
       break;
    }

    return "UNKNOWN_TIMER";
}

static void timer_send_event(int timer_id, void *data)
{
    int rv;
    af_event_t *e = NULL;
    ogs_assert(data);

    switch (timer_id) {
    case AF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
    case AF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
    case AF_TIMER_NF_INSTANCE_NO_HEARTBEAT:
    case AF_TIMER_NF_INSTANCE_VALIDITY:
    case AF_TIMER_SUBSCRIPTION_VALIDITY:
        e = af_event_new(AF_EVT_SBI_TIMER);
        ogs_assert(e);
        e->timer_id = timer_id;
        e->sbi.data = data;
        break;
    case AF_TIMER_SBI_CLIENT_WAIT:
        e = af_event_new(AF_EVT_SBI_TIMER);
        if (!e) {
            ogs_sbi_xact_t *sbi_xact = data;
            ogs_assert(sbi_xact);

            ogs_error("timer_send_event() failed");
            ogs_sbi_xact_remove(sbi_xact);
            return;
        }
        e->timer_id = timer_id;
        e->sbi.data = data;
        break;
    default:
        ogs_fatal("Unknown timer id[%d]", timer_id);
        ogs_assert_if_reached();
        break;
    }

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed [%d] in %s",
                (int)rv, af_timer_get_name(e->timer_id));
        af_event_free(e);
    }
}

void af_timer_nf_instance_registration_interval(void *data)
{
    timer_send_event(AF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL, data);
}

void af_timer_nf_instance_heartbeat_interval(void *data)
{
    timer_send_event(AF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL, data);
}

void af_timer_nf_instance_no_heartbeat(void *data)
{
    timer_send_event(AF_TIMER_NF_INSTANCE_NO_HEARTBEAT, data);
}

void af_timer_nf_instance_validity(void *data)
{
    timer_send_event(AF_TIMER_NF_INSTANCE_VALIDITY, data);
}

void af_timer_subscription_validity(void *data)
{
    timer_send_event(AF_TIMER_SUBSCRIPTION_VALIDITY, data);
}

void af_timer_sbi_client_wait_expire(void *data)
{
    timer_send_event(AF_TIMER_SBI_CLIENT_WAIT, data);
}
