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

#ifndef SCP_EVENT_H
#define SCP_EVENT_H

#include "ogs-core.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct scp_sess_s scp_sess_t;
typedef struct ogs_sbi_request_s ogs_sbi_request_t;
typedef struct ogs_sbi_response_s ogs_sbi_response_t;
typedef struct ogs_sbi_message_s ogs_sbi_message_t;
typedef struct ogs_sbi_nf_instance_s ogs_sbi_nf_instance_t;

typedef enum {
    SCP_EVT_BASE = OGS_FSM_USER_SIG,

    SCP_EVT_SBI_SERVER,
    SCP_EVT_SBI_CLIENT,
    SCP_EVT_SBI_TIMER,

    SCP_EVT_TOP,

} scp_event_e;

typedef struct scp_event_s {
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

    ogs_sbi_nf_instance_t *nf_instance;
    scp_sess_t *sess;
} scp_event_t;

void scp_event_init(void);
void scp_event_final(void);

scp_event_t *scp_event_new(scp_event_e id);
void scp_event_free(scp_event_t *e);

const char *scp_event_get_name(scp_event_t *e);

#ifdef __cplusplus
}
#endif

#endif /* SCP_EVENT_H */
