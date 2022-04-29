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
    /* Paging procedure for EPS services initiated */
    [MME_TIMER_T3413] =
        { .max_count = 2, .duration = ogs_time_from_sec(2) },

    /* DETACH REQUEST sent */
    [MME_TIMER_T3422] =
        { .max_count = 4, .duration = ogs_time_from_sec(3) },

    /* ATTACH ACCEPT sent
     * TRACKING AREA UPDATE ACCEPT sent with GUTI
     * TRACKING AREA UPDATE ACCEPT sent with TMSI
     * GUTI REALLOCATION COMMAND sent */
    [MME_TIMER_T3450] =
        { .max_count = 4, .duration = ogs_time_from_sec(6) },

    /* AUTHENTICATION REQUEST sent
     * SECURITY MODE COMMAND sent */
    [MME_TIMER_T3460] =
        { .max_count = 4, .duration = ogs_time_from_sec(3) },

    /* IDENTITY REQUEST sent */
    [MME_TIMER_T3470] =
        { .max_count = 4, .duration = ogs_time_from_sec(3) },

    /* ESM INFORMATION REQUEST sent */
    [MME_TIMER_T3489] =
        { .max_count = 2, .duration = ogs_time_from_sec(4) },

    [MME_TIMER_SGS_CLI_CONN_TO_SRV] =
        { .duration = ogs_time_from_sec(3) },

    [MME_TIMER_S1_HOLDING] =
        { .duration = ogs_time_from_sec(30) },

    [MME_TIMER_S11_HOLDING] =
        { .duration = ogs_time_from_msec(300) },
};

static void emm_timer_event_send(
        mme_timer_e timer_id, mme_ue_t *mme_ue);
static void esm_timer_event_send(
        mme_timer_e timer_id, mme_bearer_t *bearer);

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
    case MME_TIMER_T3422:
        return "MME_TIMER_T3422";
    case MME_TIMER_T3450:
        return "MME_TIMER_T3450";
    case MME_TIMER_T3460:
        return "MME_TIMER_T3460";
    case MME_TIMER_T3470:
        return "MME_TIMER_T3470";
    case MME_TIMER_T3489:
        return "MME_TIMER_T3489";
    case MME_TIMER_SGS_CLI_CONN_TO_SRV:
        return "MME_TIMER_SGS_CLI_CONN_TO_SRV";
    case MME_TIMER_S1_HOLDING:
        return "MME_TIMER_S1_HOLDING";
    case MME_TIMER_S11_HOLDING:
        return "MME_TIMER_S11_HOLDING";
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

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        ogs_timer_delete(e->timer);
        mme_event_free(e);
    }
}


static void emm_timer_event_send(
        mme_timer_e timer_id, mme_ue_t *mme_ue)
{
    int rv;
    mme_event_t *e = NULL;
    ogs_assert(mme_ue);

    e = mme_event_new(MME_EVT_EMM_TIMER);
    e->timer_id = timer_id;
    e->mme_ue = mme_ue;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        mme_event_free(e);
    }
}

void mme_timer_t3413_expire(void *data)
{
    emm_timer_event_send(MME_TIMER_T3413, data);
}
void mme_timer_t3422_expire(void *data)
{
    emm_timer_event_send(MME_TIMER_T3422, data);
}
void mme_timer_t3450_expire(void *data)
{
    emm_timer_event_send(MME_TIMER_T3450, data);
}
void mme_timer_t3460_expire(void *data)
{
    emm_timer_event_send(MME_TIMER_T3460, data);
}
void mme_timer_t3470_expire(void *data)
{
    emm_timer_event_send(MME_TIMER_T3470, data);
}

static void esm_timer_event_send(
        mme_timer_e timer_id, mme_bearer_t *bearer)
{
    int rv;
    mme_event_t *e = NULL;
    mme_ue_t *mme_ue = NULL;
    ogs_assert(bearer);
    mme_ue = bearer->mme_ue;
    ogs_assert(bearer);

    e = mme_event_new(MME_EVT_ESM_TIMER);
    e->timer_id = timer_id;
    e->mme_ue = mme_ue;
    e->bearer = bearer;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        mme_event_free(e);
    }
}

void mme_timer_t3489_expire(void *data)
{
    esm_timer_event_send(MME_TIMER_T3489, data);
}

void mme_timer_sgs_cli_conn_to_srv(void *data)
{
    int rv;
    mme_event_t *e = NULL;
    ogs_assert(data);

    e = mme_event_new(MME_EVT_SGSAP_TIMER);
    e->timer_id = MME_TIMER_SGS_CLI_CONN_TO_SRV;
    e->vlr = data;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        mme_event_free(e);
    }
}

void mme_timer_s1_holding_timer_expire(void *data)
{
    int rv;
    mme_event_t *e = NULL;
    enb_ue_t *enb_ue = NULL;

    ogs_assert(data);
    enb_ue = data;

    e = mme_event_new(MME_EVT_S1AP_TIMER);

    e->timer_id = MME_TIMER_S1_HOLDING;
    e->enb_ue = enb_ue;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        mme_event_free(e);
    }
}

void mme_timer_s11_holding_timer_expire(void *data)
{
    int rv;
    mme_event_t *e = NULL;
    sgw_ue_t *sgw_ue = NULL;

    ogs_assert(data);
    sgw_ue = data;

    e = mme_event_new(MME_EVT_S11_TIMER);

    e->timer_id = MME_TIMER_S11_HOLDING;
    e->sgw_ue = sgw_ue;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        mme_event_free(e);
    }
}
