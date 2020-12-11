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

#ifndef PCF_EVENT_H
#define PCF_EVENT_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ogs_sbi_request_s ogs_sbi_request_t;
typedef struct ogs_sbi_response_s ogs_sbi_response_t;
typedef struct ogs_sbi_message_s ogs_sbi_message_t;
typedef struct ogs_sbi_nf_instance_s ogs_sbi_nf_instance_t;
typedef struct ogs_sbi_subscription_s ogs_sbi_subscription_t;

typedef struct pcf_ue_s pcf_ue_t;
typedef struct pcf_sess_s pcf_sess_t;

typedef enum {
    PCF_EVT_BASE = OGS_FSM_USER_SIG,

    PCF_EVT_SBI_SERVER,
    PCF_EVT_SBI_CLIENT,
    PCF_EVT_SBI_TIMER,

    PCF_EVT_TOP,

} pcf_event_e;

typedef struct pcf_event_s {
    int id;
    int timer_id;

    struct {
        ogs_sbi_request_t *request;
        ogs_sbi_response_t *response;
        void *data;

        ogs_sbi_message_t *message;
    } sbi;

    pcf_ue_t *pcf_ue;
    pcf_sess_t *sess;

    ogs_timer_t *timer;
} pcf_event_t;

void pcf_event_init(void);
void pcf_event_final(void);

pcf_event_t *pcf_event_new(pcf_event_e id);
void pcf_event_free(pcf_event_t *e);

const char *pcf_event_get_name(pcf_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* PCF_EVENT_H */
