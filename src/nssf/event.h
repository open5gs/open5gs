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

#ifndef NSSF_EVENT_H
#define NSSF_EVENT_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sbi_request_s ogs_sbi_request_t;
typedef struct ogs_sbi_response_s ogs_sbi_response_t;
typedef struct ogs_sbi_message_s ogs_sbi_message_t;
typedef struct ogs_sbi_nf_instance_s ogs_sbi_nf_instance_t;
typedef struct ogs_sbi_subscription_s ogs_sbi_subscription_t;

typedef enum {
    NSSF_EVT_BASE = OGS_FSM_USER_SIG,

    NSSF_EVT_SBI_SERVER,
    NSSF_EVT_SBI_CLIENT,
    NSSF_EVT_SBI_TIMER,

    NSSF_EVT_TOP,

} nssf_event_e;

typedef struct nssf_event_s {
    int id;
    int timer_id;

    struct {
        ogs_sbi_request_t *request;
        ogs_sbi_response_t *response;
        void *data;

        ogs_sbi_message_t *message;
    } sbi;

    ogs_timer_t *timer;
} nssf_event_t;

void nssf_event_init(void);
void nssf_event_final(void);

nssf_event_t *nssf_event_new(nssf_event_e id);
void nssf_event_free(nssf_event_t *e);

const char *nssf_event_get_name(nssf_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* NSSF_EVENT_H */
