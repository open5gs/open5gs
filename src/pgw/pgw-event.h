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

#ifndef PGW_EVENT_H
#define PGW_EVENT_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_gtp_node_s ogs_gtp_node_t;
typedef struct ogs_gtp_xact_s ogs_gtp_xact_t;
typedef struct pgw_sess_s pgw_sess_t;

typedef enum {
    PGW_EVT_BASE = OGS_FSM_USER_SIG,

    PGW_EVT_S5C_MESSAGE,
    PGW_EVT_GX_MESSAGE,

    PGW_EVT_TOP,

} pgw_event_e;

typedef struct pgw_event_s {
    int id;
    ogs_pkbuf_t *gtpbuf;
    ogs_pkbuf_t *gxbuf;

    ogs_gtp_node_t *gnode;
    ogs_gtp_xact_t *xact;

    pgw_sess_t *sess;
} pgw_event_t;

void pgw_event_init(void);
void pgw_event_term(void);
void pgw_event_final(void);

pgw_event_t *pgw_event_new(pgw_event_e id);
void pgw_event_free(pgw_event_t *e);

const char *pgw_event_get_name(pgw_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* PGW_EVENT_H */
