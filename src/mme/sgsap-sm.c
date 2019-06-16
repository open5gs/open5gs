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

#include "ogs-sctp.h"

#include "mme-context.h"
#include "mme-event.h"
#include "mme-sm.h"

#include "sgsap-path.h"

static void sgsap_connect_timeout(void *data);

void sgsap_state_initial(ogs_fsm_t *s, mme_event_t *e)
{
    mme_vlr_t *vlr = NULL;
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    vlr = e->vlr;
    ogs_assert(vlr);

    vlr->t_conn = ogs_timer_add(mme_self()->timer_mgr,
            sgsap_connect_timeout, vlr);
    ogs_assert(vlr->t_conn);

    OGS_FSM_TRAN(s, &sgsap_state_will_connect);
}

void sgsap_state_final(ogs_fsm_t *s, mme_event_t *e)
{
    mme_vlr_t *vlr = NULL;
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    vlr = e->vlr;
    ogs_assert(vlr);

    ogs_timer_delete(vlr->t_conn);
}

void sgsap_state_will_connect(ogs_fsm_t *s, mme_event_t *e)
{
    mme_vlr_t *vlr = NULL;
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    vlr = e->vlr;
    ogs_assert(vlr);

    ogs_assert(vlr->t_conn);
    ogs_assert(vlr->node);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        ogs_timer_start(vlr->t_conn, mme_self()->t_conn_value);
        sgsap_client(vlr->node);
        break;
    case OGS_FSM_EXIT_SIG:
        ogs_timer_stop(vlr->t_conn);
        break;
    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}

void sgsap_state_connected(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_SGSAP_MESSAGE:
        break;
    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}

void sgsap_state_exception(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}

static void sgsap_connect_timeout(void *data)
{
    char buf[OGS_ADDRSTRLEN];

    mme_vlr_t *vlr = data;
    ogs_socknode_t *node = NULL;
    ogs_sockaddr_t *addr = NULL;

    ogs_assert(vlr);
    node = vlr->node;
    ogs_assert(node);
    addr = node->addr;
    ogs_assert(addr);

    ogs_warn("[SGsAP] Connect to VLR [%s]:%d failed",
                OGS_ADDR(addr, buf), OGS_PORT(addr));

    ogs_assert(vlr->t_conn);
    ogs_timer_start(vlr->t_conn, mme_self()->t_conn_value);

    ogs_assert(vlr->node->sock);
    ogs_sctp_destroy(vlr->node->sock);

    ogs_assert(vlr->node);
    sgsap_client(vlr->node);
}

