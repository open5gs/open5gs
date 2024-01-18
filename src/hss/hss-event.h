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

#ifndef HSS_EVENT_H
#define HSS_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    HSS_EVT_BASE = OGS_MAX_NUM_OF_PROTO_EVENT,

    HSS_EVENT_DBI_POLL_TIMER,
    HSS_EVENT_DBI_MESSAGE,

    HSS_EVT_TOP,
} hss_event_e;

typedef struct hss_event_s {
    int id;
    int timer_id;

    struct {
        void *document;
    } dbi;
} hss_event_t;

void hss_event_init(void);
void hss_event_term(void);
void hss_event_final(void);

hss_event_t *hss_event_new(hss_event_e id);
void hss_event_free(hss_event_t *e);

const char *hss_event_get_name(hss_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* HSS_EVENT_H */
