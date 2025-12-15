/*
 * Copyright (C) 2019-2025 by Sukchan Lee <acetcom@gmail.com>
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

#ifndef PWSIWF_SAI_SBI_PATH_H
#define PWSIWF_SAI_SBI_PATH_H

#include "ogs-sbi.h"
#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

/* PWS-IWF_SAI SBI Server Functions */
int pwsiwf_sai_sbi_open(void);
void pwsiwf_sai_sbi_close(void);

/* PWS-IWF_SAI SBI Server Callback Functions */
int pwsiwf_sai_sbi_server_callback(ogs_sbi_request_t *request, void *data);

/* PWS-IWF_SAI SBI Path Functions */
int pwsiwf_sai_sbi_discover_and_send(ogs_sbi_service_type_e service_type,
        ogs_sbi_request_t *(*build)(pwsiwf_sai_warning_t *warning, void *data),
        pwsiwf_sai_warning_t *warning, ogs_sbi_xact_t *xact, void *data);

/* PWS-IWF_SAI NONUEN2 Message Transfer Functions */
typedef struct pwsiwf_sai_nonuen2_message_transfer_param_s {
#define PWSIWF_SAI_NONUEN2_MESSAGE_TRANSFER_NO_STATE             0
#define PWSIWF_SAI_WARNING_MESSAGE_BROADCAST                     1
#define PWSIWF_SAI_WARNING_MESSAGE_STOP                          2
#define PWSIWF_SAI_WARNING_MESSAGE_UPDATE                        3
#define PWSIWF_SAI_WARNING_MESSAGE_CANCEL                        4
#define PWSIWF_SAI_WARNING_MESSAGE_RETRANSMISSION                5
#define PWSIWF_SAI_WARNING_MESSAGE_ERROR                         6
    int state;

    ogs_pkbuf_t *n2smbuf;

    bool nonuen2_failure_txf_notif_uri;
    bool skip_ind;
} pwsiwf_sai_nonuen2_message_transfer_param_t;

int pwsiwf_sai_nonuen2_comm_send_nonuen2_message_transfer(
        pwsiwf_sai_warning_t *warning, pwsiwf_sai_nonuen2_message_transfer_param_t *param);

/* PWS-IWF_SAI SBI Client Functions */
int pwsiwf_sai_sbi_client_open(void);
void pwsiwf_sai_sbi_client_close(void);

/* PWS-IWF_SAI SBI Client Callback Functions */
int pwsiwf_sai_sbi_client_callback(int status, ogs_sbi_response_t *response, void *data);

#ifdef __cplusplus
}
#endif

#endif /* PWSIWF_SAI_SBI_PATH_H */ 