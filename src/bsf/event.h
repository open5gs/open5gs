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

#ifndef BSF_EVENT_H
#define BSF_EVENT_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct bsf_sess_s bsf_sess_t;
typedef struct ogs_sbi_request_s ogs_sbi_request_t;
typedef struct ogs_sbi_response_s ogs_sbi_response_t;
typedef struct ogs_sbi_message_s ogs_sbi_message_t;
typedef struct ogs_sbi_subscription_s ogs_sbi_subscription_t;

typedef enum {
    BSF_EVT_BASE = OGS_FSM_USER_SIG,

    BSF_EVT_SBI_SERVER,
    BSF_EVT_SBI_CLIENT,
    BSF_EVT_SBI_TIMER,

    BSF_EVT_TOP,

} bsf_event_e;

typedef struct bsf_event_s {
    int id;
    ogs_pkbuf_t *pkbuf;
    int timer_id;

    struct {
        ogs_sbi_request_t *request;
        ogs_sbi_response_t *response;
        void *data;
        int state;

        ogs_sbi_message_t *message;
    } sbi;

    bsf_sess_t *sess;
} bsf_event_t;

void bsf_event_init(void);
void bsf_event_final(void);

bsf_event_t *bsf_event_new(bsf_event_e id);
void bsf_event_free(bsf_event_t *e);

const char *bsf_event_get_name(bsf_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* BSF_EVENT_H */
