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

#ifndef SMF_EVENT_H
#define SMF_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_gtp_node_s ogs_gtp_node_t;
typedef struct ogs_gtp_xact_s ogs_gtp_xact_t;
typedef struct ogs_gtp1_message_s ogs_gtp1_message_t;
typedef struct ogs_gtp2_message_s ogs_gtp2_message_t;
typedef struct ogs_pfcp_node_s ogs_pfcp_node_t;
typedef struct ogs_pfcp_xact_s ogs_pfcp_xact_t;
typedef struct ogs_pfcp_message_s ogs_pfcp_message_t;
typedef struct ogs_diam_gx_message_s ogs_diam_gx_message_t;
typedef struct ogs_diam_gy_message_s ogs_diam_gy_message_t;
typedef struct ogs_diam_s6b_message_s ogs_diam_s6b_message_t;
typedef struct smf_sess_s smf_sess_t;
typedef struct smf_upf_s smf_upf_t;
typedef struct smf_gtp_node_s smf_gtp_node_t;
typedef struct ogs_nas_5gs_message_s ogs_nas_5gs_message_t;
typedef struct NGAP_NGAP_PDU ogs_ngap_message_t;
typedef long NGAP_ProcedureCode_t;

typedef enum {
    SMF_EVT_BASE = OGS_MAX_NUM_OF_PROTO_EVENT,

    SMF_EVT_S5C_MESSAGE,
    SMF_EVT_S6B_MESSAGE,
    SMF_EVT_GN_MESSAGE,
    SMF_EVT_GX_MESSAGE,
    SMF_EVT_GY_MESSAGE,

    SMF_EVT_N4_MESSAGE,
    SMF_EVT_N4_TIMER,
    SMF_EVT_N4_NO_HEARTBEAT,

    SMF_EVT_NGAP_MESSAGE,
    SMF_EVT_NGAP_TIMER,

    SMF_EVT_5GSM_MESSAGE,
    SMF_EVT_5GSM_TIMER,

    SMF_EVT_TOP,

} smf_event_e;

typedef struct smf_event_s {
    ogs_event_t h;

    ogs_pkbuf_t *pkbuf;

    smf_gtp_node_t *gnode;
    ogs_pool_id_t gtp_xact_id;

    ogs_pfcp_node_t *pfcp_node;
    ogs_pool_id_t pfcp_xact_id;
    ogs_pfcp_message_t *pfcp_message;

    union {
        ogs_gtp1_message_t *gtp1_message;
        ogs_gtp2_message_t *gtp2_message;
    };

    union {
        ogs_diam_gx_message_t *gx_message;
        ogs_diam_gy_message_t *gy_message;
        ogs_diam_s6b_message_t *s6b_message;
    };

    struct {
        int type;
        ogs_ngap_message_t *message;
    } ngap;

    struct {
        uint8_t type;
        ogs_nas_5gs_message_t *message;
    } nas;

    ogs_pool_id_t sess_id;
} smf_event_t;

OGS_STATIC_ASSERT(OGS_EVENT_SIZE >= sizeof(smf_event_t));

smf_event_t *smf_event_new(int id);

const char *smf_event_get_name(smf_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* SMF_EVENT_H */
