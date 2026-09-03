/*
 * Copyright (C) 2019-2023 by Sukchan Lee <acetcom@gmail.com>
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
#include "local-path.h"
#include "namf-handler.h"

bool smf_namf_comm_handle_n1_n2_message_transfer(
        smf_sess_t *sess, ogs_sbi_stream_t *stream,
        int state, ogs_sbi_message_t *recvmsg)
{
    smf_ue_t *smf_ue = NULL;
    OpenAPI_n1_n2_message_transfer_rsp_data_t *N1N2MessageTransferRspData;

    ogs_assert(sess);
    smf_ue = smf_ue_find_by_id(sess->smf_ue_id);
    ogs_assert(smf_ue);
    ogs_assert(state);
    ogs_assert(recvmsg);

    switch (state) {
    case SMF_UE_REQUESTED_PDU_SESSION_ESTABLISHMENT:
        if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {
/*
 * Non-roaming/LBO: start network-triggered PDU Session Modification at step 11
 * after N1N2 transfer (Establishment Accept) and N2/N4 context sync, ensuring
 * the session is active on UE, RAN, and SMF before applying QoS updates.
 *
 * Home-Routed Roaming: trigger PDU Session Modification at step 13
 * immediately after H-SMF’s CreateSMContext response and H-UPF N4 setup
 * to apply QoS updates without waiting for V-SMF or RAN setup.
 */
            smf_qos_flow_binding(sess);
        } else if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_ACCEPTED) {
/*
 * TS29.518 Namf_Communication_N1N2MessageTransfer
 *
 * The AMF has accepted the transfer and is still trying to deliver it -
 * it may be paging the UE. This is not a failure, so keep the session.
 *
 * The AMF in Open5GS cannot answer an Establishment Accept this way: its
 * 202 Accepted paths require either no N1 message or a different NGAP IE
 * type. A peer AMF may still do so.
 */
            ogs_warn("[%s:%d] N1 N2 transfer is still in progress [%d]",
                smf_ue->supi, sess->psi, recvmsg->res_status);
        } else {
            ogs_error("[%s:%d] HTTP response error [%d]",
                smf_ue->supi, sess->psi, recvmsg->res_status);

/*
 * TS23.502 4.3.2.2.1 UE Requested PDU Session Establishment
 *
 * The SM context is created and returned to the AMF at step 5, long before
 * this N1N2 transfer at step 11. If the AMF cannot accept the PDU Session
 * Establishment Accept - the RAN-UE context was released while the session
 * was being established, so the AMF has already dropped its own session
 * context - nothing else will ever release the SM context.
 *
 * Release it locally. Otherwise the session, its UE IP address, its PFCP
 * session and its PCF/UDM associations are orphaned in the SMF until the
 * same UE happens to request the same PDU Session ID again.
 */
            if (HOME_ROUTED_ROAMING_IN_VSMF(sess) ||
                HOME_ROUTED_ROAMING_IN_HSMF(sess)) {
                ogs_error("[%s:%d] Home-routed roaming session is not "
                        "released locally", smf_ue->supi, sess->psi);
            } else {
                smf_trigger_session_release(
                        sess, NULL, OGS_PFCP_DELETE_TRIGGER_LOCAL_INITIATED);
            }
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
                if (stream) {
                    if (sess->vsmf_to_hsmf_modify_stream_id >=
                            OGS_MIN_POOL_ID &&
                        sess->vsmf_to_hsmf_modify_stream_id <=
                            OGS_MAX_POOL_ID)
                        ogs_error("N1 N2 modified stream ID [%d]"
                                "has not been used yet",
                                sess->vsmf_to_hsmf_modify_stream_id);
                    sess->vsmf_to_hsmf_modify_stream_id =
                        ogs_sbi_id_from_stream(stream);
                }
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

                if (stream) {
                    if (sess->vsmf_to_hsmf_modify_stream_id >=
                            OGS_MIN_POOL_ID &&
                        sess->vsmf_to_hsmf_modify_stream_id <=
                            OGS_MAX_POOL_ID)
                        ogs_error("N1 N2 modified stream ID [%d]"
                                "has not been used yet",
                                sess->vsmf_to_hsmf_modify_stream_id);
                    sess->vsmf_to_hsmf_modify_stream_id =
                        ogs_sbi_id_from_stream(stream);
                }
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

    case SMF_UE_OR_NETWORK_REQUESTED_PDU_SESSION_RELEASE:
    case SMF_ERROR_INDICATON_RECEIVED_FROM_5G_AN:

        N1N2MessageTransferRspData = recvmsg->N1N2MessageTransferRspData;
        if (!N1N2MessageTransferRspData) {
            ogs_error("No N1N2MessageTransferRspData [status:%d]",
                    recvmsg->res_status);
            break;
        }

        if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_ACCEPTED) {
    /*
     * OpenAPI_n1_n2_message_transfer_cause_ATTEMPTING_TO_REACH_UE and
     * HTTP_STATUS_ACCEPTED should be handled here when removing PDU session
     * due to the change of PDU Session Anchor.
     *
     * TS23.502
     * 4.3.4 PDU Session Release
     * 4.3.4.2 UE or network requested PDU Session Release for Non-Roaming
     * and Roaming with Local Breakout
     *
     * 3b. ...
     *
     * The "skip indicator" tells the AMF whether it may skip sending
     * the N1 SM container to the UE (e.g. when the UE is in CM-IDLE state).
     * SMF includes the "skip indicator"
     * in the Namf_Communication_N1N2MessageTransfer
     * except when the procedure is triggered to change PDU Session Anchor
     * of a PDU Session with SSC mode 2.
     *
     * Related Issue #2396
     */
            if (N1N2MessageTransferRspData->cause ==
                OpenAPI_n1_n2_message_transfer_cause_ATTEMPTING_TO_REACH_UE) {
                /* Nothing */
            } else {
                ogs_error("Not implemented [cause:%d]",
                        N1N2MessageTransferRspData->cause);
                ogs_assert_if_reached();
            }
        } else if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {
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

                smf_namf_comm_send_n1_n2_message_transfer(sess, NULL, &param);
            } else if (N1N2MessageTransferRspData->cause ==
                OpenAPI_n1_n2_message_transfer_cause_N1_N2_TRANSFER_INITIATED) {
                /*
                 * H-SMF stream was stored when the N1N2 message was
                 * sent. N1/N2 released updates may already have
                 * consumed it before this response arrives.
                 */
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
                recvmsg, "No N1N2MsgTxfrFailureNotification", NULL, NULL));
        return false;
    }

    if (!N1N2MsgTxfrFailureNotification->cause) {
        ogs_error("No Cause");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No Cause", NULL, NULL));
        return false;
    }

    if (!N1N2MsgTxfrFailureNotification->n1n2_msg_data_uri) {
        ogs_error("No n1n2MsgDataUri");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No n1n2MsgDataUri", NULL, NULL));
        return false;
    }

    sess = smf_sess_find_by_paging_n1n2message_location(
        N1N2MsgTxfrFailureNotification->n1n2_msg_data_uri);
    if (!sess) {
        ogs_error("Not found");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                recvmsg, N1N2MsgTxfrFailureNotification->n1n2_msg_data_uri,
                NULL, NULL));
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
