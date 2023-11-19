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

#ifndef SCP_EVENT_H
#define SCP_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct scp_sess_s scp_sess_t;
typedef struct ogs_sbi_nf_instance_s ogs_sbi_nf_instance_t;

typedef struct scp_event_s {
    ogs_event_t h;
} scp_event_t;

OGS_STATIC_ASSERT(OGS_EVENT_SIZE >= sizeof(scp_event_t));

scp_event_t *scp_event_new(int id);

const char *scp_event_get_name(scp_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* SCP_EVENT_H */
