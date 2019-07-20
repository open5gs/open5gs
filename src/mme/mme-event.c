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

#include "mme-event.h"
#include "mme-context.h"

#include "s1ap-path.h"

#define EVENT_POOL 32 /* FIXME : 32 */
void mme_event_init(void)
{
    mme_self()->queue = ogs_queue_create(EVENT_POOL);
    ogs_assert(mme_self()->queue);
    mme_self()->timer_mgr = ogs_timer_mgr_create();
    ogs_assert(mme_self()->timer_mgr);
    mme_self()->pollset = ogs_pollset_create();
    ogs_assert(mme_self()->pollset);
}

void mme_event_term(void)
{
    ogs_queue_term(mme_self()->queue);
    ogs_pollset_notify(mme_self()->pollset);
}

void mme_event_final(void)
{
    if (mme_self()->pollset)
        ogs_pollset_destroy(mme_self()->pollset);
    if (mme_self()->timer_mgr)
        ogs_timer_mgr_destroy(mme_self()->timer_mgr);
    if (mme_self()->queue)
        ogs_queue_destroy(mme_self()->queue);
}

mme_event_t *mme_event_new(mme_event_e id)
{
    mme_event_t *e = NULL;

    e = ogs_calloc(1, sizeof *e);
    ogs_assert(e);
    e->id = id;

    return e;
}

void mme_event_free(mme_event_t *e)
{
    ogs_assert(e);
    ogs_free(e);
}

const char *mme_event_get_name(mme_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG: 
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG: 
        return OGS_FSM_NAME_EXIT_SIG;

    case MME_EVT_S1AP_MESSAGE:
        return "MME_EVT_S1AP_MESSAGE";
    case MME_EVT_S1AP_TIMER:
        return "MME_EVT_S1AP_TIMER";
    case MME_EVT_S1AP_LO_ACCEPT:
        return "MME_EVT_S1AP_LO_ACCEPT";
    case MME_EVT_S1AP_LO_SCTP_COMM_UP:
        return "MME_EVT_S1AP_LO_SCTP_COMM_UP";
    case MME_EVT_S1AP_LO_CONNREFUSED:
        return "MME_EVT_S1AP_LO_CONNREFUSED";

    case MME_EVT_EMM_MESSAGE:
        return "MME_EVT_EMM_MESSAGE";
    case MME_EVT_EMM_TIMER:
        return "MME_EVT_EMM_TIMER";
    case MME_EVT_ESM_MESSAGE:
        return "MME_EVT_ESM_MESSAGE";
    case MME_EVT_ESM_TIMER:
        return "MME_EVT_ESM_TIMER";
    case MME_EVT_S11_MESSAGE:
        return "MME_EVT_S11_MESSAGE";
    case MME_EVT_S11_TIMER:
        return "MME_EVT_S11_TIMER";
    case MME_EVT_S6A_MESSAGE:
        return "MME_EVT_S6A_MESSAGE";
    case MME_EVT_S6A_TIMER:
        return "MME_EVT_S6A_TIMER";

    case MME_EVT_SGSAP_MESSAGE:
        return "MME_EVT_SGSAP_MESSAGE";
    case MME_EVT_SGSAP_TIMER:
        return "MME_EVT_SGSAP_TIMER";
    case MME_EVT_SGSAP_LO_SCTP_COMM_UP:
        return "MME_EVT_SGSAP_LO_SCTP_COMM_UP";
    case MME_EVT_SGSAP_LO_CONNREFUSED:
        return "MME_EVT_SGSAP_LO_CONNREFUSED";

    default: 
       break;
    }

    return "UNKNOWN_EVENT";
}

void mme_sctp_event_push(mme_event_e id,
        void *sock, ogs_sockaddr_t *addr, ogs_pkbuf_t *pkbuf,
        uint16_t max_num_of_istreams, uint16_t max_num_of_ostreams)
{
    mme_event_t *e = NULL;
    int rv;

    ogs_assert(id);
    ogs_assert(sock);
    ogs_assert(addr);

    e = mme_event_new(id);
    ogs_assert(e);
    e->sctp_sock = sock;
    e->sctp_addr = addr;
    e->pkbuf = pkbuf;
    e->max_num_of_istreams = max_num_of_istreams;
    e->max_num_of_ostreams = max_num_of_ostreams;

    rv = ogs_queue_push(mme_self()->queue, e);
    if (rv != OGS_OK) {
        ogs_warn("ogs_queue_push() failed:%d", (int)rv);
        ogs_free(e->sctp_addr);
        if (e->pkbuf)
            ogs_pkbuf_free(e->pkbuf);
        mme_event_free(e);
    }
#if HAVE_USRSCTP
    else {
        ogs_pollset_notify(mme_self()->pollset);
    }
#endif
}
