/*
 * Copyright (C) 2024 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef PCRF_EVENT_H
#define PCRF_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    PCRF_EVT_BASE = OGS_MAX_NUM_OF_PROTO_EVENT,

    PCRF_EVT_TOP,
} pcrf_event_e;

typedef struct pcrf_event_s {
    int id;
    int timer_id;

    struct {
        void *document;
    } dbi;
} pcrf_event_t;

void pcrf_event_init(void);
void pcrf_event_term(void);
void pcrf_event_final(void);

pcrf_event_t *pcrf_event_new(pcrf_event_e id);
void pcrf_event_free(pcrf_event_t *e);

const char *pcrf_event_get_name(pcrf_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* PCRF_EVENT_H */
