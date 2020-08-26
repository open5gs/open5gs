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

static nrf_timer_cfg_t g_nrf_timer_cfg[MAX_NUM_OF_NRF_TIMER] = {
    /* Nothing */
};

nrf_timer_cfg_t *nrf_timer_cfg(nrf_timer_e id)
{
    ogs_assert(id < MAX_NUM_OF_NRF_TIMER);
    return &g_nrf_timer_cfg[id];
}

const char *nrf_timer_get_name(nrf_timer_e id)
{
    switch (id) {
    case NRF_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        return "NRF_TIMER_NF_INSTANCE_NO_HEARTBEAT";
    case NRF_TIMER_SUBSCRIPTION_VALIDITY:
        return "NRF_TIMER_SUBSCRIPTION_VALIDITY";
    default: 
       break;
    }

    return "UNKNOWN_TIMER";
}

static void timer_send_event(int timer_id, void *data)
{
    int rv;
    nrf_event_t *e = NULL;
    ogs_assert(data);

    switch (timer_id) {
    case NRF_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        e = nrf_event_new(NRF_EVT_SBI_TIMER);
        e->timer_id = timer_id;
        e->nf_instance = data;
        break;
    case NRF_TIMER_SUBSCRIPTION_VALIDITY:
        e = nrf_event_new(NRF_EVT_SBI_TIMER);
        e->timer_id = timer_id;
        e->subscription = data;
        break;
    default:
        ogs_fatal("Unknown timer id[%d]", timer_id);
        ogs_assert_if_reached();
        break;
    }

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        nrf_event_free(e);
    }
}

void nrf_timer_nf_instance_no_heartbeat(void *data)
{
    timer_send_event(NRF_TIMER_NF_INSTANCE_NO_HEARTBEAT, data);
}

void nrf_timer_subscription_validity(void *data)
{
    timer_send_event(NRF_TIMER_SUBSCRIPTION_VALIDITY, data);
}
