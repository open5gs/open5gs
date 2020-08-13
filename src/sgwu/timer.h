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

#ifndef SGWU_TIMER_H
#define SGWU_TIMER_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    SGWU_TIMER_BASE = 0,

    SGWU_TIMER_ASSOCIATION,
    SGWU_TIMER_NO_HEARTBEAT,

    MAX_NUM_OF_SGWU_TIMER,

} sgwu_timer_e;

const char *sgwu_timer_get_name(sgwu_timer_e id);

void sgwu_timer_association(void *data);
void sgwu_timer_no_heartbeat(void *data);

#ifdef __cplusplus
}
#endif

#endif /* SGWU_TIMER_H */
