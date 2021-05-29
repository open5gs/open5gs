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

#ifndef AF_EVENT_H
#define AF_EVENT_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct af_sess_s af_sess_t;
typedef struct ogs_sbi_request_s ogs_sbi_request_t;
typedef struct ogs_sbi_response_s ogs_sbi_response_t;
typedef struct ogs_sbi_message_s ogs_sbi_message_t;
typedef struct ogs_sbi_subscription_s ogs_sbi_subscription_t;

typedef enum {
    AF_EVT_BASE = OGS_FSM_USER_SIG,

    AF_EVT_SBI_SERVER,
    AF_EVT_SBI_CLIENT,
    AF_EVT_SBI_TIMER,
    AF_EVT_SBI_LOCAL,

    AF_EVT_TOP,

} af_event_e;

typedef struct af_event_s {
    int id;
    ogs_pkbuf_t *pkbuf;
    int timer_id;
    int local_id;

    struct {
        OpenAPI_nf_type_e target_nf_type;
        void *data;
        ogs_sbi_request_t *(*build)(af_sess_t *sess, void *data);
    } local;

    struct {
        ogs_sbi_request_t *request;
        ogs_sbi_response_t *response;
        void *data;
        int state;

        ogs_sbi_message_t *message;
    } sbi;

    af_sess_t *sess;
} af_event_t;

void af_event_init(void);
void af_event_final(void);

af_event_t *af_event_new(af_event_e id);
void af_event_free(af_event_t *e);

const char *af_event_get_name(af_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* AF_EVENT_H */
