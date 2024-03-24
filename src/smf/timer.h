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

#ifndef SMF_TIMER_H
#define SMF_TIMER_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    SMF_TIMER_BASE = OGS_MAX_NUM_OF_PROTO_TIMER,

    SMF_TIMER_PFCP_ASSOCIATION,
    SMF_TIMER_PFCP_NO_HEARTBEAT,
    SMF_TIMER_PFCP_NO_ESTABLISHMENT_RESPONSE,
    SMF_TIMER_PFCP_NO_DELETION_RESPONSE,

    MAX_NUM_OF_SMF_TIMER,

} smf_timer_e;

const char *smf_timer_get_name(int timer_id);

void smf_timer_pfcp_association(void *data);
void smf_timer_pfcp_no_heartbeat(void *data);

#ifdef __cplusplus
}
#endif

#endif /* SMF_TIMER_H */
