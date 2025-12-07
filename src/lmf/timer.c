/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "timer.h"
#include "context.h"

const char *lmf_timer_get_name(int timer_id)
{
    switch(timer_id) {
    case LMF_TIMER_SBI_CLIENT_WAIT:
        return "LMF_TIMER_SBI_CLIENT_WAIT";
    case LMF_TIMER_LOCATION_REQUEST_TIMEOUT:
        return "LMF_TIMER_LOCATION_REQUEST_TIMEOUT";
    default:
        break;
    }

    return "UNKNOWN_TIMER";
}

void lmf_timer_location_request_timeout(void *data)
{
    lmf_location_request_t *location_request = data;

    ogs_assert(location_request);
    
    ogs_warn("[%s] Location request timeout", 
            location_request->supi ? location_request->supi : "Unknown");

    /* Cleanup location request */
    lmf_location_request_remove(location_request);
}



