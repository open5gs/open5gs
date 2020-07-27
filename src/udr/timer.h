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

#ifndef UDR_TIMER_H
#define UDR_TIMER_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    UDR_TIMER_BASE = 0,

    UDR_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL,
    UDR_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL,
    UDR_TIMER_NF_INSTANCE_NO_HEARTBEAT,
    UDR_TIMER_NF_INSTANCE_VALIDITY,
    UDR_TIMER_SUBSCRIPTION_VALIDITY,

    MAX_NUM_OF_UDR_TIMER,

} udr_timer_e;

const char *udr_timer_get_name(udr_timer_e id);

void udr_timer_nf_instance_registration_interval(void *data);
void udr_timer_nf_instance_heartbeat_interval(void *data);
void udr_timer_nf_instance_no_heartbeat(void *data);
void udr_timer_nf_instance_validity(void *data);
void udr_timer_subscription_validity(void *data);

#ifdef __cplusplus
}
#endif

#endif /* UDR_TIMER_H */
