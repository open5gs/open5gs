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

#ifndef PCF_TIMER_H
#define PCF_TIMER_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    PCF_TIMER_BASE = 0,

    PCF_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL,
    PCF_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL,
    PCF_TIMER_NF_INSTANCE_NO_HEARTBEAT,
    PCF_TIMER_NF_INSTANCE_VALIDITY,
    PCF_TIMER_SUBSCRIPTION_VALIDITY,
    PCF_TIMER_SBI_CLIENT_WAIT,

    MAX_NUM_OF_PCF_TIMER,

} pcf_timer_e;

const char *pcf_timer_get_name(pcf_timer_e id);

void pcf_timer_nf_instance_registration_interval(void *data);
void pcf_timer_nf_instance_heartbeat_interval(void *data);
void pcf_timer_nf_instance_no_heartbeat(void *data);
void pcf_timer_nf_instance_validity(void *data);
void pcf_timer_subscription_validity(void *data);
void pcf_timer_sbi_client_wait_expire(void *data);

#ifdef __cplusplus
}
#endif

#endif /* PCF_TIMER_H */
