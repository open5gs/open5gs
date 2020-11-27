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

#ifndef NRF_EVENT_H
#define NRF_EVENT_H

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
    NRF_EVT_BASE = OGS_FSM_USER_SIG,

    NRF_EVT_SBI_SERVER,
    NRF_EVT_SBI_CLIENT,
    NRF_EVT_SBI_TIMER,

    NRF_EVT_TOP,

} nrf_event_e;

typedef struct nrf_event_s {
    int id;
    int timer_id;

    struct {
        ogs_sbi_request_t *request;
        ogs_sbi_response_t *response;
        void *data;

        ogs_sbi_message_t *message;
    } sbi;

    ogs_sbi_nf_instance_t *nf_instance;
    ogs_sbi_subscription_t *subscription;
} nrf_event_t;

void nrf_event_init(void);
void nrf_event_final(void);

nrf_event_t *nrf_event_new(nrf_event_e id);
void nrf_event_free(nrf_event_t *e);

const char *nrf_event_get_name(nrf_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* NRF_EVENT_H */
