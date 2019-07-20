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
#include "mme-context.h"

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
    case MME_TIMER_CLI_CONN_TO_SRV:
        return "MME_TIMER_CLI_CONN_TO_SRV";
    case MME_TIMER_T3413:
        return "MME_TIMER_T3413";
    default: 
       break;
    }

    return "UNKNOWN_TIMER";
}
