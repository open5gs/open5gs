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

#ifndef AMF_EVENT_H
#define AMF_EVENT_H

#include "ogs-proto.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_nas_5gs_message_s ogs_nas_5gs_message_t;
typedef struct NGAP_NGAP_PDU ogs_ngap_message_t;
typedef long NGAP_ProcedureCode_t;

typedef struct amf_gnb_s amf_gnb_t;
typedef struct ran_ue_s ran_ue_t;
typedef struct amf_ue_s amf_ue_t;
typedef struct amf_sess_s amf_sess_t;
typedef struct amf_bearer_s amf_bearer_t;

typedef enum {
    AMF_EVENT_BASE = OGS_MAX_NUM_OF_PROTO_EVENT,

    AMF_EVENT_NGAP_MESSAGE,
    AMF_EVENT_NGAP_TIMER,
    AMF_EVENT_NGAP_LO_ACCEPT,
    AMF_EVENT_NGAP_LO_SCTP_COMM_UP,
    AMF_EVENT_NGAP_LO_CONNREFUSED,

    AMF_EVENT_5GMM_MESSAGE,
    AMF_EVENT_5GMM_TIMER,
    AMF_EVENT_5GSM_MESSAGE,
    AMF_EVENT_5GSM_TIMER,

    MAX_NUM_OF_AMF_EVENT,

} amf_event_e;

typedef struct amf_event_s {
    ogs_event_t h;

    ogs_pkbuf_t *pkbuf;

    struct {
        ogs_sock_t *sock;
        ogs_sockaddr_t *addr;
        uint16_t max_num_of_istreams;
        uint16_t max_num_of_ostreams;

        NGAP_ProcedureCode_t code;
        ogs_ngap_message_t *message;
    } ngap;

    struct {
        uint8_t type;
        ogs_nas_5gs_message_t *message;
    } nas;

    ogs_pool_id_t gnb_id;
    ogs_pool_id_t ran_ue_id;
    ogs_pool_id_t amf_ue_id;
    ogs_pool_id_t sess_id;

    ogs_timer_t *timer;
} amf_event_t;

OGS_STATIC_ASSERT(OGS_EVENT_SIZE >= sizeof(amf_event_t));

amf_event_t *amf_event_new(int id);

const char *amf_event_get_name(amf_event_t *e);

void amf_sctp_event_push(int id,
        void *sock, ogs_sockaddr_t *addr, ogs_pkbuf_t *pkbuf,
        uint16_t max_num_of_istreams, uint16_t max_num_of_ostreams);

#ifdef __cplusplus
}
#endif

#endif /* AMF_EVENT_H */
