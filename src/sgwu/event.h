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

#ifndef SGWU_EVENT_H
#define SGWU_EVENT_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_gtp_node_s ogs_gtp_node_t;
typedef struct ogs_pfcp_node_s ogs_pfcp_node_t;
typedef struct ogs_pfcp_xact_s ogs_pfcp_xact_t;
typedef struct ogs_pfcp_message_s ogs_pfcp_message_t;
typedef struct sgwu_bearer_s sgwu_bearer_t;

typedef enum {
    SGWU_EVT_BASE = OGS_FSM_USER_SIG,

    SGWU_EVT_SXA_MESSAGE,
    SGWU_EVT_SXA_TIMER,
    SGWU_EVT_SXA_NO_HEARTBEAT,

    SGWU_EVT_TOP,

} sgwu_event_e;

typedef struct sgwu_event_s {
    int id;
    ogs_pkbuf_t *pkbuf;
    int timer_id;

    ogs_gtp_node_t *gnode;

    ogs_pfcp_node_t *pfcp_node;
    ogs_pfcp_xact_t *pfcp_xact;
    ogs_pfcp_message_t *pfcp_message;

    sgwu_bearer_t *bearer;
} sgwu_event_t;

void sgwu_event_init(void);
void sgwu_event_term(void);
void sgwu_event_final(void);

sgwu_event_t *sgwu_event_new(sgwu_event_e id);
void sgwu_event_free(sgwu_event_t *e);

const char *sgwu_event_get_name(sgwu_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* SGWU_EVENT_H */
