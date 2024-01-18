/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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

#if !defined(OGS_PROTO_INSIDE) && !defined(OGS_PROTO_COMPILATION)
#error "This header cannot be included directly."
#endif

#ifndef OGS_PROTO_TIMER_H
#define OGS_PROTO_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

extern const char *OGS_TIMER_NAME_NF_INSTANCE_REGISTRATION_INTERVAL;
extern const char *OGS_TIMER_NAME_NF_INSTANCE_HEARTBEAT_INTERVAL;
extern const char *OGS_TIMER_NAME_NF_INSTANCE_NO_HEARTBEAT;
extern const char *OGS_TIMER_NAME_NF_INSTANCE_VALIDITY;
extern const char *OGS_TIMER_NAME_SUBSCRIPTION_VALIDITY;
extern const char *OGS_TIMER_NAME_SUBSCRIPTION_PATCH;
extern const char *OGS_TIMER_NAME_SBI_CLIENT_WAIT;

/* forward declaration */
typedef enum {
    OGS_TIMER_BASE = 0,

    OGS_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL,
    OGS_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL,
    OGS_TIMER_NF_INSTANCE_NO_HEARTBEAT,
    OGS_TIMER_NF_INSTANCE_VALIDITY,
    OGS_TIMER_SUBSCRIPTION_VALIDITY,
    OGS_TIMER_SUBSCRIPTION_PATCH,
    OGS_TIMER_SBI_CLIENT_WAIT,

    OGS_MAX_NUM_OF_PROTO_TIMER,

} ogs_timer_e;

const char *ogs_timer_get_name(int timer_id);

#ifdef __cplusplus
}
#endif

#endif /* OGS_PROTO_TIMER_H */
