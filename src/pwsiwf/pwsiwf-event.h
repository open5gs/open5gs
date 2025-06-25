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

#ifndef PWSIWS_EVENT_H
#define PWSIWS_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    PWSIWS_EVENT_BASE = OGS_MAX_NUM_OF_PROTO_EVENT,

    PWSIWS_EVENT_SBCAP_MESSAGE,
    PWSIWS_EVENT_SBCAP_TIMER,
    PWSIWS_EVENT_SBCAP_LO_ACCEPT,
    PWSIWS_EVENT_SBCAP_LO_SCTP_COMM_UP,
    PWSIWS_EVENT_SBCAP_LO_CONNREFUSED,

    PWSIWS_EVENT_SBI_MESSAGE,
    PWSIWS_EVENT_SBI_TIMER,

    MAX_NUM_OF_PWSIWS_EVENT,

} pwsiws_event_e;

typedef struct SBCAP_SBC_AP_PDU ogs_sbcap_message_t;
typedef struct pwsiws_connection_s pwsiws_connection_t;

typedef struct pwsiws_event_s {
    int id;
    int timer_id;

    ogs_pkbuf_t *pkbuf;

    ogs_sock_t *sock;
    ogs_sockaddr_t *addr;

    uint16_t max_num_of_istreams;
    uint16_t max_num_of_ostreams;

    ogs_sbcap_message_t *sbcap_message;

    pwsiws_connection_t *connection;

    ogs_timer_t *timer;
} pwsiws_event_t;

OGS_STATIC_ASSERT(OGS_EVENT_SIZE >= sizeof(pwsiws_event_t));

void pwsiws_event_term(void);

pwsiws_event_t *pwsiws_event_new(pwsiws_event_e id);
void pwsiws_event_free(pwsiws_event_t *e);

void pwsiws_event_timeout(void *data);

const char *pwsiws_event_get_name(pwsiws_event_t *e);

void pwsiws_sctp_event_push(pwsiws_event_e id,
        void *sock, ogs_sockaddr_t *addr, ogs_pkbuf_t *pkbuf,
        uint16_t max_num_of_istreams, uint16_t max_num_of_ostreams);

#ifdef __cplusplus
}
#endif

#endif /* PWSIWS_EVENT_H */ 