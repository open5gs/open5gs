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

#ifndef UDM_EVENT_H
#define UDM_EVENT_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sbi_request_s ogs_sbi_request_t;
typedef struct ogs_sbi_response_s ogs_sbi_response_t;
typedef struct ogs_sbi_message_s ogs_sbi_message_t;
typedef struct ogs_sbi_nf_instance_s ogs_sbi_nf_instance_t;
typedef struct ogs_sbi_subscription_s ogs_sbi_subscription_t;

typedef struct udm_ue_s udm_ue_t;

typedef enum {
    UDM_EVT_BASE = OGS_FSM_USER_SIG,

    UDM_EVT_SBI_SERVER,
    UDM_EVT_SBI_CLIENT,
    UDM_EVT_SBI_TIMER,

    UDM_EVT_TOP,

} udm_event_e;

typedef struct udm_event_s {
    int id;
    int timer_id;

    struct {
        ogs_sbi_request_t *request;
        ogs_sbi_response_t *response;
        void *data;

        ogs_sbi_message_t *message;
    } sbi;

    udm_ue_t *udm_ue;

    ogs_timer_t *timer;
} udm_event_t;

void udm_event_init(void);
void udm_event_final(void);

udm_event_t *udm_event_new(udm_event_e id);
void udm_event_free(udm_event_t *e);

const char *udm_event_get_name(udm_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* UDM_EVENT_H */
