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

#include "context.h"

static amf_timer_cfg_t g_amf_timer_cfg[MAX_NUM_OF_AMF_TIMER] = {
    [AMF_TIMER_SBI_CLIENT_WAIT] =
        { .duration = ogs_time_from_msec(500) },

    /* Paging procedure for EPS services initiated */
    [AMF_TIMER_T3513] =
        { .max_count = 2, .duration = ogs_time_from_sec(2) },

    /* DEREGISTRATION REQUEST sent */
    [AMF_TIMER_T3522] =
        { .max_count = 4, .duration = ogs_time_from_sec(3) },

    /* REGISTRATION ACCEPT sent */
    [AMF_TIMER_T3550] =
        { .max_count = 4, .duration = ogs_time_from_sec(6) },

    /* CONFIGURATION UPDATE COMMAND sent */
    [AMF_TIMER_T3555] =
        { .max_count = 4, .duration = ogs_time_from_sec(6) },

    /* AUTHENTICATION REQUEST sent
     * SECURITY MODE COMMAND sent */
    [AMF_TIMER_T3560] =
        { .max_count = 4, .duration = ogs_time_from_sec(6) },

    /* IDENTITY REQUEST sent */
    [AMF_TIMER_T3570] =
        { .max_count = 4, .duration = ogs_time_from_sec(3) },

    [AMF_TIMER_NG_HOLDING] =
        { .duration = ogs_time_from_sec(30) },
};

static void gmm_timer_event_send(
        amf_timer_e timer_id, amf_ue_t *amf_ue);

amf_timer_cfg_t *amf_timer_cfg(amf_timer_e id)
{
    ogs_assert(id < MAX_NUM_OF_AMF_TIMER);
    return &g_amf_timer_cfg[id];
}

const char *amf_timer_get_name(amf_timer_e id)
{
    switch (id) {
    case AMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        return "AMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL";
    case AMF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        return "AMF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL";
    case AMF_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        return "AMF_TIMER_NF_INSTANCE_NO_HEARTBEAT";
    case AMF_TIMER_NF_INSTANCE_VALIDITY:
        return "AMF_TIMER_NF_INSTANCE_VALIDITY";
    case AMF_TIMER_SUBSCRIPTION_VALIDITY:
        return "AMF_TIMER_SUBSCRIPTION_VALIDITY";
    case AMF_TIMER_SBI_CLIENT_WAIT:
        return "AMF_TIMER_SBI_CLIENT_WAIT";
    case AMF_TIMER_NG_DELAYED_SEND:
        return "AMF_TIMER_NG_DELAYED_SEND";
    case AMF_TIMER_T3513:
        return "AMF_TIMER_T3513";
    case AMF_TIMER_T3522:
        return "AMF_TIMER_T3522";
    case AMF_TIMER_T3550:
        return "AMF_TIMER_T3550";
    case AMF_TIMER_T3555:
        return "AMF_TIMER_T3555";
    case AMF_TIMER_T3560:
        return "AMF_TIMER_T3560";
    case AMF_TIMER_T3570:
        return "AMF_TIMER_T3570";
    case AMF_TIMER_NG_HOLDING:
        return "AMF_TIMER_NG_HOLDING";
    default: 
       break;
    }

    return "UNKNOWN_TIMER";
}

void amf_timer_ng_delayed_send(void *data)
{
    int rv;
    amf_event_t *e = data;
    ogs_assert(e);

    e->timer_id = AMF_TIMER_NG_DELAYED_SEND;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        ogs_timer_delete(e->timer);
        amf_event_free(e);
    }
}

static void sbi_timer_send_event(int timer_id, void *data)
{
    int rv;
    amf_event_t *e = NULL;
    ogs_assert(data);

    switch (timer_id) {
    case AMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
    case AMF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
    case AMF_TIMER_NF_INSTANCE_NO_HEARTBEAT:
    case AMF_TIMER_NF_INSTANCE_VALIDITY:
    case AMF_TIMER_SUBSCRIPTION_VALIDITY:
        e = amf_event_new(AMF_EVT_SBI_TIMER);
        ogs_assert(e);
        e->timer_id = timer_id;
        e->sbi.data = data;
        break;
    case AMF_TIMER_SBI_CLIENT_WAIT:
        e = amf_event_new(AMF_EVT_SBI_TIMER);
        if (!e) {
            ogs_sbi_xact_t *sbi_xact = data;
            ogs_assert(sbi_xact);

            ogs_error("sbi_timer_send_event() failed");
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
                (int)rv, amf_timer_get_name(e->timer_id));
        amf_event_free(e);
    }
}

void amf_timer_nf_instance_registration_interval(void *data)
{
    sbi_timer_send_event(AMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL, data);
}

void amf_timer_nf_instance_heartbeat_interval(void *data)
{
    sbi_timer_send_event(AMF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL, data);
}

void amf_timer_nf_instance_no_heartbeat(void *data)
{
    sbi_timer_send_event(AMF_TIMER_NF_INSTANCE_NO_HEARTBEAT, data);
}

void amf_timer_nf_instance_validity(void *data)
{
    sbi_timer_send_event(AMF_TIMER_NF_INSTANCE_VALIDITY, data);
}

void amf_timer_subscription_validity(void *data)
{
    sbi_timer_send_event(AMF_TIMER_SUBSCRIPTION_VALIDITY, data);
}

void amf_timer_sbi_client_wait_expire(void *data)
{
    sbi_timer_send_event(AMF_TIMER_SBI_CLIENT_WAIT, data);
}

static void gmm_timer_event_send(
        amf_timer_e timer_id, amf_ue_t *amf_ue)
{
    int rv;
    amf_event_t *e = NULL;
    ogs_assert(amf_ue);

    e = amf_event_new(AMF_EVT_5GMM_TIMER);
    ogs_assert(e);
    e->timer_id = timer_id;
    e->amf_ue = amf_ue;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        amf_event_free(e);
    }
}

void amf_timer_t3513_expire(void *data)
{
    gmm_timer_event_send(AMF_TIMER_T3513, data);
}
void amf_timer_t3522_expire(void *data)
{
    gmm_timer_event_send(AMF_TIMER_T3522, data);
}
void amf_timer_t3550_expire(void *data)
{
    gmm_timer_event_send(AMF_TIMER_T3550, data);
}
void amf_timer_t3555_expire(void *data)
{
    gmm_timer_event_send(AMF_TIMER_T3555, data);
}
void amf_timer_t3560_expire(void *data)
{
    gmm_timer_event_send(AMF_TIMER_T3560, data);
}
void amf_timer_t3570_expire(void *data)
{
    gmm_timer_event_send(AMF_TIMER_T3570, data);
}

void amf_timer_ng_holding_timer_expire(void *data)
{
    int rv;
    amf_event_t *e = NULL;
    ran_ue_t *ran_ue = NULL;

    ogs_assert(data);
    ran_ue = data;

    e = amf_event_new(AMF_EVT_NGAP_TIMER);
    ogs_assert(e);

    e->timer_id = AMF_TIMER_NG_HOLDING;
    e->ran_ue = ran_ue;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        amf_event_free(e);
    }
}
