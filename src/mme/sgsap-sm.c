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
#include "mme-timer.h"
#include "mme-sm.h"

#include "sgsap-path.h"
#include "sgsap-handler.h"

void sgsap_state_initial(ogs_fsm_t *s, mme_event_t *e)
{
    mme_vlr_t *vlr = NULL;
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    vlr = e->vlr;
    ogs_assert(vlr);

    vlr->t_conn = ogs_timer_add(ogs_app()->timer_mgr,
            mme_timer_sgs_cli_conn_to_srv, vlr);
    ogs_expect_or_return(vlr->t_conn);

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
    char buf[OGS_ADDRSTRLEN];

    mme_vlr_t *vlr = NULL;
    ogs_sockaddr_t *addr = NULL;
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    vlr = e->vlr;
    ogs_assert(vlr);

    ogs_assert(vlr->t_conn);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        ogs_timer_start(vlr->t_conn,
                mme_timer_cfg(MME_TIMER_SGS_CLI_CONN_TO_SRV)->duration);
        sgsap_client(vlr);
        break;
    case OGS_FSM_EXIT_SIG:
        ogs_timer_stop(vlr->t_conn);
        break;
    case MME_EVT_SGSAP_TIMER:
        switch(e->timer_id) {
        case MME_TIMER_SGS_CLI_CONN_TO_SRV:
            vlr = e->vlr;
            ogs_assert(vlr);
            addr = vlr->sa_list;
            ogs_assert(addr);

            ogs_warn("[SGsAP] Connect to VLR [%s]:%d failed",
                        OGS_ADDR(addr, buf), OGS_PORT(addr));

            ogs_assert(vlr->t_conn);
            ogs_timer_start(vlr->t_conn,
                mme_timer_cfg(MME_TIMER_SGS_CLI_CONN_TO_SRV)->duration);

            mme_vlr_close(vlr);
            sgsap_client(vlr);
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    mme_timer_get_name(e->timer_id), e->timer_id);
            break;
        }
        break;
    case MME_EVT_SGSAP_LO_SCTP_COMM_UP:
        OGS_FSM_TRAN(s, sgsap_state_connected);
        break;
    default:
        ogs_error("Unknown event %s", mme_event_get_name(e));
        break;
    }
}

void sgsap_state_connected(ogs_fsm_t *s, mme_event_t *e)
{
    mme_vlr_t *vlr = NULL;
    ogs_pkbuf_t *pkbuf = NULL;
    uint8_t type;
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    vlr = e->vlr;
    ogs_assert(vlr);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case MME_EVT_SGSAP_LO_CONNREFUSED:
        mme_vlr_close(vlr);
        OGS_FSM_TRAN(s, sgsap_state_will_connect);
        break;
    case MME_EVT_SGSAP_MESSAGE:
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);
        type = *(unsigned char *)(pkbuf->data);
        switch (type) {
        case SGSAP_LOCATION_UPDATE_ACCEPT:
            sgsap_handle_location_update_accept(vlr, pkbuf);
            break;
        case SGSAP_LOCATION_UPDATE_REJECT:
            sgsap_handle_location_update_reject(vlr, pkbuf);
            break;
        case SGSAP_EPS_DETACH_ACK:
        case SGSAP_IMSI_DETACH_ACK:
            sgsap_handle_detach_ack(vlr, pkbuf);
            break;
        case SGSAP_PAGING_REQUEST:
            sgsap_handle_paging_request(vlr, pkbuf);
            break;
        case SGSAP_DOWNLINK_UNITDATA:
            sgsap_handle_downlink_unitdata(vlr, pkbuf);
            break;
        case SGSAP_RESET_INDICATION:
            sgsap_handle_reset_indication(vlr, pkbuf);

            mme_vlr_close(vlr);
            OGS_FSM_TRAN(s, sgsap_state_will_connect);
            break;
        case SGSAP_RELEASE_REQUEST:
            sgsap_handle_release_request(vlr, pkbuf);
            break;
        case SGSAP_MM_INFORMATION_REQUEST:
            sgsap_handle_mm_information_request(vlr, pkbuf);
            break;
        default:
            ogs_warn("Unknown Message Type: [%d]", type);
            break;
        }
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
