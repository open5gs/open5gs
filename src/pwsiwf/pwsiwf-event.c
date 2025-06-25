/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#include "pwsiws-event.h"
#include "context.h"

void pwsiws_event_term(void)
{
    ogs_queue_term(ogs_app()->queue);
    ogs_pollset_notify(ogs_app()->pollset);
}

pwsiws_event_t *pwsiws_event_new(pwsiws_event_e id)
{
    pwsiws_event_t *e = NULL;

    e = ogs_calloc(1, sizeof *e);
    ogs_assert(e);
    memset(e, 0, sizeof(*e));

    e->id = id;

    return e;
}

void pwsiws_event_free(pwsiws_event_t *e)
{
    ogs_assert(e);
    ogs_free(e);
}

void pwsiws_event_timeout(void *data)
{
    pwsiws_event_t *e = data;
    ogs_assert(e);

    ogs_info("PWS-IWS Event Timeout: %s", pwsiws_event_get_name(e));

    pwsiws_event_free(e);
}

const char *pwsiws_event_get_name(pwsiws_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG:
        return OGS_FSM_NAME_EXIT_SIG;

    case PWSIWS_EVENT_SBCAP_MESSAGE:
        return "PWSIWS_EVENT_SBCAP_MESSAGE";
    case PWSIWS_EVENT_SBCAP_TIMER:
        return "PWSIWS_EVENT_SBCAP_TIMER";
    case PWSIWS_EVENT_SBCAP_LO_ACCEPT:
        return "PWSIWS_EVENT_SBCAP_LO_ACCEPT";
    case PWSIWS_EVENT_SBCAP_LO_SCTP_COMM_UP:
        return "PWSIWS_EVENT_SBCAP_LO_SCTP_COMM_UP";
    case PWSIWS_EVENT_SBCAP_LO_CONNREFUSED:
        return "PWSIWS_EVENT_SBCAP_LO_CONNREFUSED";

    case PWSIWS_EVENT_SBI_MESSAGE:
        return "PWSIWS_EVENT_SBI_MESSAGE";
    case PWSIWS_EVENT_SBI_TIMER:
        return "PWSIWS_EVENT_SBI_TIMER";

    default:
        break;
    }

    return "UNKNOWN_EVENT";
}

void pwsiws_sctp_event_push(pwsiws_event_e id,
        void *sock, ogs_sockaddr_t *addr, ogs_pkbuf_t *pkbuf,
        uint16_t max_num_of_istreams, uint16_t max_num_of_ostreams)
{
    pwsiws_event_t *e = NULL;
    int rv;

    ogs_assert(id);
    ogs_assert(sock);

    e = pwsiws_event_new(id);
    ogs_assert(e);
    e->sock = sock;
    e->addr = addr;
    e->pkbuf = pkbuf;
    e->max_num_of_istreams = max_num_of_istreams;
    e->max_num_of_ostreams = max_num_of_ostreams;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_free(e->addr);
        if (e->pkbuf)
            ogs_pkbuf_free(e->pkbuf);
        pwsiws_event_free(e);
    }
#if HAVE_USRSCTP
    else {
        ogs_pollset_notify(ogs_app()->pollset);
    }
#endif
} 