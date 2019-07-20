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

#include "mme-timer.h"
#include "mme-event.h"
#include "mme-context.h"

static mme_timer_cfg_t g_mme_timer_cfg[MAX_NUM_OF_MME_TIMER] = {
    [MME_TIMER_T3413] = 
        { .max_count = 2, .duration = ogs_time_from_sec(2) },
    [MME_TIMER_SGS_CLI_CONN_TO_SRV] = 
        { .duration = ogs_time_from_sec(3) },
};

static void mme_ue_timer_event(
        mme_timer_e timer_id, mme_ue_t *mme_ue);

mme_timer_cfg_t *mme_timer_cfg(mme_timer_e id)
{
    ogs_assert(id < MAX_NUM_OF_MME_TIMER);
    return &g_mme_timer_cfg[id];
}

const char *mme_timer_get_name(mme_timer_e id)
{
    switch (id) {
    case MME_TIMER_S1_DELAYED_SEND:
        return "MME_TIMER_S1_DELAYED_SEND";
    case MME_TIMER_T3413:
        return "MME_TIMER_T3413";
    case MME_TIMER_SGS_CLI_CONN_TO_SRV:
        return "MME_TIMER_SGS_CLI_CONN_TO_SRV";
    default: 
       break;
    }

    return "UNKNOWN_TIMER";
}

void mme_timer_s1_delayed_send(void *data)
{
    int rv;
    mme_event_t *e = data;
    ogs_assert(e);

    e->timer_id = MME_TIMER_S1_DELAYED_SEND;

    rv = ogs_queue_push(mme_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        ogs_timer_delete(e->timer);
        mme_event_free(e);
    }
}

void mme_timer_t3413_expire(void *data)
{
    mme_ue_timer_event(MME_TIMER_T3413, data);
}

static void mme_ue_timer_event(
        mme_timer_e timer_id, mme_ue_t *mme_ue)
{
    int rv;
    mme_event_t *e = NULL;
    ogs_assert(mme_ue);

    e = mme_event_new(MME_EVT_EMM_TIMER);
    e->timer_id = timer_id;
    e->mme_ue = mme_ue;

    rv = ogs_queue_push(mme_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        mme_event_free(e);
    }
}

void mme_timer_sgs_cli_conn_to_srv(void *data)
{
    int rv;
    mme_event_t *e = NULL;
    ogs_assert(data);

    e = mme_event_new(MME_EVT_SGSAP_TIMER);
    e->timer_id = MME_TIMER_SGS_CLI_CONN_TO_SRV;
    e->vlr = data;

    rv = ogs_queue_push(mme_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        mme_event_free(e);
    }
}
