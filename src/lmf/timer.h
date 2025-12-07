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

#ifndef LMF_TIMER_H
#define LMF_TIMER_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    LMF_TIMER_BASE = OGS_MAX_NUM_OF_PROTO_TIMER,

    LMF_TIMER_SBI_CLIENT_WAIT,
    LMF_TIMER_LOCATION_REQUEST_TIMEOUT,

    MAX_NUM_OF_LMF_TIMER,

} lmf_timer_e;

const char *lmf_timer_get_name(int timer_id);

void lmf_timer_location_request_timeout(void *data);

#ifdef __cplusplus
}
#endif

#endif /* LMF_TIMER_H */

