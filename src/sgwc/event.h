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

#ifndef SGWC_EVENT_H
#define SGWC_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_gtp_node_s ogs_gtp_node_t;
typedef struct ogs_gtp2_message_s ogs_gtp2_message_t;
typedef struct ogs_pfcp_node_s ogs_pfcp_node_t;
typedef struct ogs_pfcp_xact_s ogs_pfcp_xact_t;
typedef struct ogs_pfcp_message_s ogs_pfcp_message_t;
typedef struct sgwc_bearer_s sgwc_bearer_t;

typedef enum {
    SGWC_EVT_BASE = OGS_MAX_NUM_OF_PROTO_EVENT,

    SGWC_EVT_S11_MESSAGE,
    SGWC_EVT_S5C_MESSAGE,

    SGWC_EVT_SXA_MESSAGE,
    SGWC_EVT_SXA_TIMER,
    SGWC_EVT_SXA_NO_HEARTBEAT,

    SGWC_EVT_TOP,

} sgwc_event_e;

typedef struct sgwc_event_s {
    int id;
    int timer_id;

    ogs_pkbuf_t *pkbuf;

    ogs_gtp_node_t *gnode;
    ogs_gtp2_message_t *gtp_message;

    ogs_pfcp_node_t *pfcp_node;
    ogs_pool_id_t pfcp_xact_id;
    ogs_pfcp_message_t *pfcp_message;
} sgwc_event_t;

OGS_STATIC_ASSERT(OGS_EVENT_SIZE >= sizeof(sgwc_event_t));

void sgwc_event_init(void);
void sgwc_event_term(void);
void sgwc_event_final(void);

sgwc_event_t *sgwc_event_new(sgwc_event_e id);
void sgwc_event_free(sgwc_event_t *e);

const char *sgwc_event_get_name(sgwc_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* SGWC_EVENT_H */
