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

static void mme_ue_timer_event(
        mme_timer_e timer_id, mme_ue_t *mme_ue);

void mme_timer_init(void)
{
    /* Paging retry timer: 2 secs */
    mme_self()->t3413_value = ogs_time_from_sec(2); 
    /* Client timer to connect to server: 3 secs */
    mme_self()->t_conn_value = ogs_time_from_sec(3);
}

void mme_timer_final(void)
{
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

#include "s1ap-path.h"

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
