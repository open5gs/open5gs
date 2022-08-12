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

#include "event.h"
#include "context.h"

amf_event_t *amf_event_new(int id)
{
    amf_event_t *e = NULL;

    e = ogs_event_size(id, sizeof(amf_event_t));
    ogs_assert(e);

    e->h.id = id;

    return e;
}

const char *amf_event_get_name(amf_event_t *e)
{
    if (e == NULL) {
        return OGS_FSM_NAME_INIT_SIG;
    }

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        return OGS_FSM_NAME_ENTRY_SIG;
    case OGS_FSM_EXIT_SIG:
        return OGS_FSM_NAME_EXIT_SIG;

    case OGS_EVENT_SBI_SERVER:
        return OGS_EVENT_NAME_SBI_SERVER;
    case OGS_EVENT_SBI_CLIENT:
        return OGS_EVENT_NAME_SBI_CLIENT;
    case OGS_EVENT_SBI_TIMER:
        return OGS_EVENT_NAME_SBI_TIMER;

    case AMF_EVENT_NGAP_MESSAGE:
        return "AMF_EVENT_NGAP_MESSAGE";
    case AMF_EVENT_NGAP_TIMER:
        return "AMF_EVENT_NGAP_TIMER";
    case AMF_EVENT_NGAP_LO_ACCEPT:
        return "AMF_EVENT_NGAP_LO_ACCEPT";
    case AMF_EVENT_NGAP_LO_SCTP_COMM_UP:
        return "AMF_EVENT_NGAP_LO_SCTP_COMM_UP";
    case AMF_EVENT_NGAP_LO_CONNREFUSED:
        return "AMF_EVENT_NGAP_LO_CONNREFUSED";

    case AMF_EVENT_5GMM_MESSAGE:
        return "AMF_EVENT_5GMM_MESSAGE";
    case AMF_EVENT_5GMM_TIMER:
        return "AMF_EVENT_5GMM_TIMER";
    case AMF_EVENT_5GSM_MESSAGE:
        return "AMF_EVENT_5GSM_MESSAGE";
    case AMF_EVENT_5GSM_TIMER:
        return "AMF_EVENT_5GSM_TIMER";

    default:
        break;
    }

    ogs_error("Unknown Event[%d]", e->h.id);
    return "UNKNOWN_EVENT";
}

void amf_sctp_event_push(int id,
        void *sock, ogs_sockaddr_t *addr, ogs_pkbuf_t *pkbuf,
        uint16_t max_num_of_istreams, uint16_t max_num_of_ostreams)
{
    amf_event_t *e = NULL;
    int rv;

    ogs_assert(id);
    ogs_assert(sock);
    ogs_assert(addr);

    e = amf_event_new(id);
    ogs_assert(e);

    e->pkbuf = pkbuf;

    e->ngap.sock = sock;
    e->ngap.addr = addr;
    e->ngap.max_num_of_istreams = max_num_of_istreams;
    e->ngap.max_num_of_ostreams = max_num_of_ostreams;

    rv = ogs_queue_push(ogs_app()->queue, e);
    if (rv != OGS_OK) {
        ogs_error("ogs_queue_push() failed:%d", (int)rv);
        ogs_free(e->ngap.addr);
        if (e->pkbuf)
            ogs_pkbuf_free(e->pkbuf);
        ogs_event_free(e);
    }
#if HAVE_USRSCTP
    else {
        ogs_pollset_notify(ogs_app()->pollset);
    }
#endif
}
