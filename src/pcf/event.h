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

#ifndef PCF_EVENT_H
#define PCF_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct pcf_ue_am_s pcf_ue_am_t;
typedef struct pcf_ue_sm_s pcf_ue_sm_t;
typedef struct pcf_sess_s pcf_sess_t;
typedef struct pcf_app_s pcf_app_t;

typedef struct pcf_event_s {
    ogs_event_t h;

    ogs_pool_id_t pcf_ue_am_id;
    ogs_pool_id_t pcf_ue_sm_id;
    ogs_pool_id_t sess_id;
    pcf_app_t *app;
} pcf_event_t;

OGS_STATIC_ASSERT(OGS_EVENT_SIZE >= sizeof(pcf_event_t));

pcf_event_t *pcf_event_new(int id);

const char *pcf_event_get_name(pcf_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* PCF_EVENT_H */
