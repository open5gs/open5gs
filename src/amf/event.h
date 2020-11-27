/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sbi_request_s ogs_sbi_request_t;
typedef struct ogs_sbi_response_s ogs_sbi_response_t;
typedef struct ogs_sbi_message_s ogs_sbi_message_t;
typedef struct ogs_sbi_nf_instance_s ogs_sbi_nf_instance_t;
typedef struct ogs_sbi_subscription_s ogs_sbi_subscription_t;

typedef struct ogs_nas_5gs_message_s ogs_nas_5gs_message_t;
typedef struct NGAP_NGAP_PDU ogs_ngap_message_t;
typedef long NGAP_ProcedureCode_t;

typedef struct amf_gnb_s amf_gnb_t;
typedef struct ran_ue_s ran_ue_t;
typedef struct amf_ue_s amf_ue_t;
typedef struct amf_sess_s amf_sess_t;
typedef struct amf_bearer_s amf_bearer_t;

typedef enum {
    AMF_EVT_BASE = OGS_FSM_USER_SIG,

    AMF_EVT_SBI_SERVER,
    AMF_EVT_SBI_CLIENT,
    AMF_EVT_SBI_TIMER,

    AMF_EVT_NGAP_MESSAGE,
    AMF_EVT_NGAP_TIMER,
    AMF_EVT_NGAP_LO_ACCEPT,
    AMF_EVT_NGAP_LO_SCTP_COMM_UP,
    AMF_EVT_NGAP_LO_CONNREFUSED,

    AMF_EVT_5GMM_MESSAGE,
    AMF_EVT_5GMM_TIMER,
    AMF_EVT_5GSM_MESSAGE,
    AMF_EVT_5GSM_TIMER,

    AMF_EVT_TOP,

} amf_event_e;

typedef struct amf_event_s {
    int id;
    ogs_pkbuf_t *pkbuf;
    int timer_id;

    struct {
        ogs_sbi_request_t *request;
        ogs_sbi_response_t *response;
        void *data;

        ogs_sbi_message_t *message;
    } sbi;

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

    amf_gnb_t *gnb;
    ran_ue_t *ran_ue;
    amf_ue_t *amf_ue;
    amf_sess_t *sess;
    amf_bearer_t *bearer;

    ogs_timer_t *timer;
} amf_event_t;

void amf_event_init(void);
void amf_event_final(void);

amf_event_t *amf_event_new(amf_event_e id);
void amf_event_free(amf_event_t *e);

const char *amf_event_get_name(amf_event_t *e);

void amf_sctp_event_push(amf_event_e id,
        void *sock, ogs_sockaddr_t *addr, ogs_pkbuf_t *pkbuf,
        uint16_t max_num_of_istreams, uint16_t max_num_of_ostreams);

#ifdef __cplusplus
}
#endif

#endif /* AMF_EVENT_H */
