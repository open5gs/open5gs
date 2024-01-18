/*
 * Copyright (C) 2023 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef SEPP_TIMER_H
#define SEPP_TIMER_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    SEPP_TIMER_BASE = OGS_MAX_NUM_OF_PROTO_TIMER,

    SEPP_TIMER_PEER_ESTABLISH,

    MAX_NUM_OF_SEPP_TIMER,

} sepp_timer_e;

const char *sepp_timer_get_name(int timer_id);

void sepp_timer_peer_establish(void *data);

#ifdef __cplusplus
}
#endif

#endif /* SEPP_TIMER_H */
