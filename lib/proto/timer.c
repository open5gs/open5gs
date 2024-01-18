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

#include "ogs-proto.h"

const char *OGS_TIMER_NAME_NF_INSTANCE_REGISTRATION_INTERVAL =
    "OGS_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL";
const char *OGS_TIMER_NAME_NF_INSTANCE_HEARTBEAT_INTERVAL =
    "OGS_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL";
const char *OGS_TIMER_NAME_NF_INSTANCE_NO_HEARTBEAT =
    "OGS_TIMER_NF_INSTANCE_NO_HEARTBEAT";
const char *OGS_TIMER_NAME_NF_INSTANCE_VALIDITY =
    "OGS_TIMER_NF_INSTANCE_VALIDITY";
const char *OGS_TIMER_NAME_SUBSCRIPTION_VALIDITY =
    "OGS_TIMER_SUBSCRIPTION_VALIDITY";
const char *OGS_TIMER_NAME_SUBSCRIPTION_PATCH =
    "OGS_TIMER_SUBSCRIPTION_PATCH";
const char *OGS_TIMER_NAME_SBI_CLIENT_WAIT =
    "OGS_TIMER_SBI_CLIENT_WAIT";

const char *ogs_timer_get_name(int timer_id)
{
    switch (timer_id) {
    case OGS_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        return OGS_TIMER_NAME_NF_INSTANCE_REGISTRATION_INTERVAL;
    case OGS_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        return OGS_TIMER_NAME_NF_INSTANCE_HEARTBEAT_INTERVAL;
    case OGS_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        return OGS_TIMER_NAME_NF_INSTANCE_NO_HEARTBEAT;
    case OGS_TIMER_NF_INSTANCE_VALIDITY:
        return OGS_TIMER_NAME_NF_INSTANCE_VALIDITY;
    case OGS_TIMER_SUBSCRIPTION_VALIDITY:
        return OGS_TIMER_NAME_SUBSCRIPTION_VALIDITY;
    case OGS_TIMER_SUBSCRIPTION_PATCH:
        return OGS_TIMER_NAME_SUBSCRIPTION_PATCH;
    case OGS_TIMER_SBI_CLIENT_WAIT:
        return OGS_TIMER_NAME_SBI_CLIENT_WAIT;
    default: 
       break;
    }

    ogs_error("Unknown Timer[%d]", timer_id);
    return "UNKNOWN_TIMER";
}
