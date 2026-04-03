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

#include "pwsiwf_sai-event.h"
#include "context.h"

void pwsiwf_sai_event_term(void)
{
    ogs_queue_term(ogs_app()->queue);
    ogs_pollset_notify(ogs_app()->pollset);
}

pwsiwf_sai_event_t *pwsiwf_sai_event_new(pwsiwf_sai_event_e id)
{
    pwsiwf_sai_event_t *e = NULL;

    e = ogs_calloc(1, sizeof *e);
    ogs_assert(e);
    memset(e, 0, sizeof(*e));

    e->id = id;

    return e;
}

void pwsiwf_sai_event_free(pwsiwf_sai_event_t *e)
{
    ogs_assert(e);
    ogs_free(e);
}

void pwsiwf_sai_event_timeout(void *data)
{
    pwsiwf_sai_event_t *e = data;
    ogs_assert(e);

    ogs_info("PWS-IWF_SAI Event Timeout: %s", pwsiwf_sai_event_get_name(e));

    pwsiwf_sai_event_free(e);
}

const char *pwsiwf_sai_event_get_name(pwsiwf_sai_event_t *e)
{
    if (e == NULL)
        return OGS_FSM_NAME_INIT_SIG;

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG:
        return OGS_FSM_NAME_EXIT_SIG;

    case PWSIWF_SAI_EVENT_SBCAP_MESSAGE:
        return "PWSIWF_SAI_EVENT_SBCAP_MESSAGE";
    case PWSIWF_SAI_EVENT_SBCAP_TIMER:
        return "PWSIWF_SAI_EVENT_SBCAP_TIMER";
    case PWSIWF_SAI_EVENT_SBCAP_LO_ACCEPT:
        return "PWSIWF_SAI_EVENT_SBCAP_LO_ACCEPT";
    case PWSIWF_SAI_EVENT_SBCAP_LO_SCTP_COMM_UP:
        return "PWSIWF_SAI_EVENT_SBCAP_LO_SCTP_COMM_UP";
    case PWSIWF_SAI_EVENT_SBCAP_LO_CONNREFUSED:
        return "PWSIWF_SAI_EVENT_SBCAP_LO_CONNREFUSED";

    case PWSIWF_SAI_EVENT_SBI_MESSAGE:
        return "PWSIWF_SAI_EVENT_SBI_MESSAGE";
    case PWSIWF_SAI_EVENT_SBI_TIMER:
        return "PWSIWF_SAI_EVENT_SBI_TIMER";

    default:
        break;
    }

    return "UNKNOWN_EVENT";
}

void pwsiwf_sai_sctp_event_push(pwsiwf_sai_event_e id,
        void *sock, ogs_sockaddr_t *addr, ogs_pkbuf_t *pkbuf,
        uint16_t max_num_of_istreams, uint16_t max_num_of_ostreams)
{
    pwsiwf_sai_event_t *e = NULL;
    int rv;

    ogs_assert(id);
    ogs_assert(sock);

    e = pwsiwf_sai_event_new(id);
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
        pwsiwf_sai_event_free(e);
    }
#if HAVE_USRSCTP
    else {
        ogs_pollset_notify(ogs_app()->pollset);
    }
#endif
} 