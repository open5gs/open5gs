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

#ifndef SMF_NAMF_BUILD_H
#define SMF_NAMF_BUILD_H

#include "context.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct smf_n1_n2_message_transfer_param_s {
#define SMF_N1_N2_MESSAGE_TRANSFER_NO_STATE             0
#define SMF_UE_REQUESTED_PDU_SESSION_ESTABLISHMENT      1
#define SMF_NETWORK_REQUESTED_PDU_SESSION_MODIFICATION  2
#define SMF_NETWORK_REQUESTED_QOS_FLOW_MODIFICATION     3
#define SMF_NETWORK_REQUESTED_PDU_SESSION_RELEASE       4
#define SMF_NETWORK_TRIGGERED_SERVICE_REQUEST           5
#define SMF_ERROR_INDICATON_RECEIVED_FROM_5G_AN         6
    int state;

    ogs_pkbuf_t *n1smbuf;
    ogs_pkbuf_t *n2smbuf;

    bool n1n2_failure_txf_notif_uri;
    bool skip_ind;
} smf_n1_n2_message_transfer_param_t;

ogs_sbi_request_t *smf_namf_comm_build_n1_n2_message_transfer(
        smf_sess_t *sess, smf_n1_n2_message_transfer_param_t *param);

ogs_sbi_request_t *smf_namf_callback_build_sm_context_status(
        smf_sess_t *sess, void *data);


#ifdef __cplusplus
}
#endif

#endif /* SMF_NAMF_BUILD_H */
