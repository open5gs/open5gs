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

#include "sbi-path.h"
#include "ngap-path.h"
#include "binding.h"
#include "namf-handler.h"

bool smf_namf_comm_handle_n1_n2_message_transfer(
        smf_sess_t *sess, int state, ogs_sbi_message_t *recvmsg)
{
    smf_ue_t *smf_ue = NULL;
    OpenAPI_n1_n2_message_transfer_rsp_data_t *N1N2MessageTransferRspData;

    ogs_assert(sess);
    smf_ue = sess->smf_ue;
    ogs_assert(smf_ue);
    ogs_assert(state);
    ogs_assert(recvmsg);

    switch (state) {
    case SMF_UE_REQUESTED_PDU_SESSION_ESTABLISHMENT:
        if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {
            smf_qos_flow_binding(sess);
        } else {
            ogs_error("[%s:%d] HTTP response error [%d]",
                smf_ue->supi, sess->psi, recvmsg->res_status);
        }
        break;

    case SMF_NETWORK_TRIGGERED_SERVICE_REQUEST:
    case SMF_NETWORK_REQUESTED_QOS_FLOW_MODIFICATION:
        N1N2MessageTransferRspData = recvmsg->N1N2MessageTransferRspData;
        if (!N1N2MessageTransferRspData) {
            ogs_error("No N1N2MessageTransferRspData [status:%d]",
                    recvmsg->res_status);
            break;
        }

        if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {
            if (N1N2MessageTransferRspData->cause ==
                OpenAPI_n1_n2_message_transfer_cause_N1_N2_TRANSFER_INITIATED) {
                /* Nothing */
            } else {
                ogs_error("Not implemented [cause:%d]",
                        N1N2MessageTransferRspData->cause);
                ogs_assert_if_reached();
            }
        } else if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_ACCEPTED) {
            if (N1N2MessageTransferRspData->cause ==
                OpenAPI_n1_n2_message_transfer_cause_ATTEMPTING_TO_REACH_UE) {
                if (recvmsg->http.location)
                    smf_sess_set_paging_n1n2message_location(
                            sess, recvmsg->http.location);
                else
                    ogs_error("No HTTP Location");
            } else {
                ogs_error("Not implemented [cause:%d]",
                        N1N2MessageTransferRspData->cause);
                ogs_assert_if_reached();
            }
        } else {

    /*
     * TODO:
     *
     * TS23.502 4.2.3.3 Network Triggered Service Request
     *
     * 3c. [Conditional] SMF responds to the UPF
     *
     * If the SMF receives an indication from the AMF that the UE is
     * unreachable or reachable only for regulatory prioritized service
     * and the SMF determines that Extended Buffering does not apply,
     * the SMF may, based on network policies, either:
     *
     * - indicate to the UPF to stop sending Data Notifications;
     * - indicate to the UPF to stop buffering DL data and
     *   discard the buffered data;
     * - indicate to the UPF to stop sending Data Notifications and
     *   stop buffering DL data and discard the buffered data; or
     * - refrains from sending further Namf_Communication_N1N2MessageTransfer
     *   message for DL data to the AMF while the UE is unreachable.
     */

            ogs_error("[%s:%d] HTTP response error [status:%d cause:%d]",
                smf_ue->supi, sess->psi, recvmsg->res_status,
                N1N2MessageTransferRspData->cause);
        }
        break;

    case SMF_NETWORK_REQUESTED_PDU_SESSION_RELEASE:
    case SMF_ERROR_INDICATON_RECEIVED_FROM_5G_AN:
        N1N2MessageTransferRspData = recvmsg->N1N2MessageTransferRspData;
        if (!N1N2MessageTransferRspData) {
            ogs_error("No N1N2MessageTransferRspData [status:%d]",
                    recvmsg->res_status);
            break;
        }

        if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {
            if (N1N2MessageTransferRspData->cause ==
                OpenAPI_n1_n2_message_transfer_cause_N1_MSG_NOT_TRANSFERRED) {
                smf_n1_n2_message_transfer_param_t param;

                memset(&param, 0, sizeof(param));
                param.state = SMF_NETWORK_TRIGGERED_SERVICE_REQUEST;
                param.n2smbuf =
                    ngap_build_pdu_session_resource_setup_request_transfer(
                            sess);
                ogs_assert(param.n2smbuf);

                param.n1n2_failure_txf_notif_uri = true;

                smf_namf_comm_send_n1_n2_message_transfer(sess, &param);
            } else if (N1N2MessageTransferRspData->cause ==
                OpenAPI_n1_n2_message_transfer_cause_N1_N2_TRANSFER_INITIATED) {
                /* Nothing */
            } else {
                ogs_error("Not implemented [cause:%d]",
                        N1N2MessageTransferRspData->cause);
                ogs_assert_if_reached();
            }
        } else {
            ogs_error("[%s:%d] HTTP response error [status:%d cause:%d]",
                smf_ue->supi, sess->psi, recvmsg->res_status,
                N1N2MessageTransferRspData->cause);
        }
        break;

    default:
        ogs_fatal("Unexpected state [%d]", state);
        ogs_assert_if_reached();
    }

    return true;
}

bool smf_namf_comm_handle_n1_n2_message_transfer_failure_notify(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    OpenAPI_n1_n2_msg_txfr_failure_notification_t
        *N1N2MsgTxfrFailureNotification = NULL;

    smf_sess_t *sess = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    N1N2MsgTxfrFailureNotification = recvmsg->N1N2MsgTxfrFailureNotification;
    if (!N1N2MsgTxfrFailureNotification) {
        ogs_error("No N1N2MsgTxfrFailureNotification");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No N1N2MsgTxfrFailureNotification", NULL));
        return false;
    }

    if (!N1N2MsgTxfrFailureNotification->cause) {
        ogs_error("No Cause");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No Cause", NULL));
        return false;
    }

    if (!N1N2MsgTxfrFailureNotification->n1n2_msg_data_uri) {
        ogs_error("No n1n2MsgDataUri");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No n1n2MsgDataUri", NULL));
        return false;
    }

    sess = smf_sess_find_by_paging_n1n2message_location(
        N1N2MsgTxfrFailureNotification->n1n2_msg_data_uri);
    if (!sess) {
        ogs_error("Not found");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                recvmsg, N1N2MsgTxfrFailureNotification->n1n2_msg_data_uri,
                NULL));
        return false;
    }

    /*
     * TODO:
     *
     * TS23.502 4.2.3.3 Network Triggered Service Request
     *
     * 5. [Conditional] AMF to SMF:
     * Namf_Communication_N1N2Transfer Failure Notification.
     *
     * When a Namf_Communication_N1N2Transfer Failure Notification
     * is received, SMF informs the UPF (if applicable).
     *
     * Procedure for pause of charging at SMF is specified in clause 4.4.4.
     */

    ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));
    return true;
}
