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

#ifndef SMF_EVENT_H
#define SMF_EVENT_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_gtp_node_s ogs_gtp_node_t;
typedef struct ogs_gtp_xact_s ogs_gtp_xact_t;
typedef struct ogs_pfcp_node_s ogs_pfcp_node_t;
typedef struct ogs_pfcp_xact_s ogs_pfcp_xact_t;
typedef struct ogs_pfcp_message_s ogs_pfcp_message_t;
typedef struct smf_sess_s smf_sess_t;
typedef struct smf_upf_s smf_upf_t;

typedef enum {
    SMF_EVT_BASE = OGS_FSM_USER_SIG,

    SMF_EVT_S5C_MESSAGE,
    SMF_EVT_GX_MESSAGE,

    SMF_EVT_N4_MESSAGE,
    SMF_EVT_N4_TIMER,
    SMF_EVT_N4_NO_HEARTBEAT,

    SMF_EVT_TOP,

} smf_event_e;

typedef struct smf_event_s {
    int id;
    ogs_pkbuf_t *pkbuf;
    int timer_id;

    ogs_gtp_node_t *gnode;
    ogs_gtp_xact_t *gtp_xact;

    ogs_pfcp_node_t *pfcp_node;
    ogs_pfcp_xact_t *pfcp_xact;
    ogs_pfcp_message_t *pfcp_message;

    smf_sess_t *sess;
} smf_event_t;

void smf_event_init(void);
void smf_event_term(void);
void smf_event_final(void);

smf_event_t *smf_event_new(smf_event_e id);
void smf_event_free(smf_event_t *e);

const char *smf_event_get_name(smf_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* SMF_EVENT_H */
