/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    AMF_TIMER_BASE = OGS_MAX_NUM_OF_PROTO_TIMER,

    AMF_TIMER_NG_DELAYED_SEND,
    AMF_TIMER_NG_HOLDING,

    AMF_TIMER_T3513,
    AMF_TIMER_T3522,
    AMF_TIMER_T3550,
    AMF_TIMER_T3555,
    AMF_TIMER_T3560,
    AMF_TIMER_T3570,
    AMF_TIMER_MOBILE_REACHABLE,
    AMF_TIMER_IMPLICIT_DEREGISTRATION,

    MAX_NUM_OF_AMF_TIMER,

} amf_timer_e;

typedef struct amf_timer_cfg_s {
    bool have;
    int max_count;
    ogs_time_t duration;
} amf_timer_cfg_t;

amf_timer_cfg_t *amf_timer_cfg(amf_timer_e id);

const char *amf_timer_get_name(int timer_id);

void amf_timer_ng_delayed_send(void *data);

void amf_timer_t3513_expire(void *data);
void amf_timer_t3522_expire(void *data);
void amf_timer_t3550_expire(void *data);
void amf_timer_t3555_expire(void *data);
void amf_timer_t3560_expire(void *data);
void amf_timer_t3570_expire(void *data);

void amf_timer_ng_holding_timer_expire(void *data);

void amf_timer_mobile_reachable_expire(void *data);
void amf_timer_implicit_deregistration_expire(void *data);

#ifdef __cplusplus
}
#endif

#endif /* AMF_TIMER_H */
