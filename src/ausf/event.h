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

#ifndef AUSF_EVENT_H
#define AUSF_EVENT_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sbi_request_s ogs_sbi_request_t;
typedef struct ogs_sbi_response_s ogs_sbi_response_t;
typedef struct ogs_sbi_message_s ogs_sbi_message_t;
typedef struct ogs_sbi_nf_instance_s ogs_sbi_nf_instance_t;
typedef struct ogs_sbi_subscription_s ogs_sbi_subscription_t;

typedef struct ausf_ue_s ausf_ue_t;

typedef enum {
    AUSF_EVT_BASE = OGS_FSM_USER_SIG,

    AUSF_EVT_SBI_SERVER,
    AUSF_EVT_SBI_CLIENT,
    AUSF_EVT_SBI_TIMER,

    AUSF_EVT_TOP,

} ausf_event_e;

typedef struct ausf_event_s {
    int id;
    int timer_id;

    struct {
        ogs_sbi_request_t *request;
        ogs_sbi_response_t *response;
        void *data;

        ogs_sbi_message_t *message;
    } sbi;

    ausf_ue_t *ausf_ue;

    ogs_timer_t *timer;
} ausf_event_t;

void ausf_event_init(void);
void ausf_event_final(void);

ausf_event_t *ausf_event_new(ausf_event_e id);
void ausf_event_free(ausf_event_t *e);

const char *ausf_event_get_name(ausf_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* AUSF_EVENT_H */
