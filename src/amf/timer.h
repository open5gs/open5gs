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

#ifndef AMF_TIMER_H
#define AMF_TIMER_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    AMF_TIMER_BASE = 0,

    AMF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL,
    AMF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL,
    AMF_TIMER_NF_INSTANCE_NO_HEARTBEAT,
    AMF_TIMER_NF_INSTANCE_VALIDITY,
    AMF_TIMER_SUBSCRIPTION_VALIDITY,
    AMF_TIMER_SBI_CLIENT_WAIT,

    AMF_TIMER_NG_DELAYED_SEND,
    AMF_TIMER_NG_HOLDING,

    AMF_TIMER_T3513,
    AMF_TIMER_T3522,
    AMF_TIMER_T3550,
    AMF_TIMER_T3555,
    AMF_TIMER_T3560,
    AMF_TIMER_T3570,

    MAX_NUM_OF_AMF_TIMER,

} amf_timer_e;

typedef struct amf_timer_cfg_s {
    int max_count;
    ogs_time_t duration;
} amf_timer_cfg_t;

amf_timer_cfg_t *amf_timer_cfg(amf_timer_e id);

const char *amf_timer_get_name(amf_timer_e id);

void amf_timer_nf_instance_registration_interval(void *data);
void amf_timer_nf_instance_heartbeat_interval(void *data);
void amf_timer_nf_instance_no_heartbeat(void *data);
void amf_timer_nf_instance_validity(void *data);
void amf_timer_subscription_validity(void *data);
void amf_timer_sbi_client_wait_expire(void *data);

void amf_timer_ng_delayed_send(void *data);

void amf_timer_t3513_expire(void *data);
void amf_timer_t3522_expire(void *data);
void amf_timer_t3550_expire(void *data);
void amf_timer_t3555_expire(void *data);
void amf_timer_t3560_expire(void *data);
void amf_timer_t3570_expire(void *data);

void amf_timer_ng_holding_timer_expire(void *data);

#ifdef __cplusplus
}
#endif

#endif /* AMF_TIMER_H */
