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

#ifndef PWSIWF_SAI_EVENT_H
#define PWSIWF_SAI_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

/* forward declaration */
typedef enum {
    PWSIWF_SAI_EVENT_BASE = OGS_MAX_NUM_OF_PROTO_EVENT,

    PWSIWF_SAI_EVENT_SBCAP_MESSAGE,
    PWSIWF_SAI_EVENT_SBCAP_TIMER,
    PWSIWF_SAI_EVENT_SBCAP_LO_ACCEPT,
    PWSIWF_SAI_EVENT_SBCAP_LO_SCTP_COMM_UP,
    PWSIWF_SAI_EVENT_SBCAP_LO_CONNREFUSED,

    PWSIWF_SAI_EVENT_SBI_MESSAGE,
    PWSIWF_SAI_EVENT_SBI_TIMER,

    MAX_NUM_OF_PWSIWF_SAI_EVENT,

} pwsiwf_sai_event_e;

typedef struct SBCAP_SBC_AP_PDU ogs_sbcap_message_t;
typedef struct pwsiwf_sai_connection_s pwsiwf_sai_connection_t;

typedef struct pwsiwf_sai_event_s {
    int id;
    int timer_id;

    ogs_pkbuf_t *pkbuf;

    ogs_sock_t *sock;
    ogs_sockaddr_t *addr;

    uint16_t max_num_of_istreams;
    uint16_t max_num_of_ostreams;

    ogs_sbcap_message_t *sbcap_message;

    pwsiwf_sai_connection_t *connection;

    ogs_timer_t *timer;
} pwsiwf_sai_event_t;

OGS_STATIC_ASSERT(OGS_EVENT_SIZE >= sizeof(pwsiwf_sai_event_t));

void pwsiwf_sai_event_term(void);

pwsiwf_sai_event_t *pwsiwf_sai_event_new(pwsiwf_sai_event_e id);
void pwsiwf_sai_event_free(pwsiwf_sai_event_t *e);

void pwsiwf_sai_event_timeout(void *data);

const char *pwsiwf_sai_event_get_name(pwsiwf_sai_event_t *e);

void pwsiwf_sai_sctp_event_push(pwsiwf_sai_event_e id,
        void *sock, ogs_sockaddr_t *addr, ogs_pkbuf_t *pkbuf,
        uint16_t max_num_of_istreams, uint16_t max_num_of_ostreams);

#ifdef __cplusplus
}
#endif

#endif /* PWSIWF_SAI_EVENT_H */ 