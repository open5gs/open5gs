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
#include "namf-handler.h"
#include "ngap-build.h"

bool pwsiws_nonuen2_comm_handle_nonuen2_message_transfer(
        pwsiws_warning_t *warning, int state, ogs_sbi_message_t *recvmsg)
{
    pwsiws_connection_t *connection = NULL;

    ogs_assert(warning);
    connection = pwsiws_connection_find_by_id(warning->connection_id);
    ogs_assert(connection);
    ogs_assert(state);
    ogs_assert(recvmsg);

    switch (state) {
    case PWSIWS_WARNING_MESSAGE_BROADCAST:
        if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {
            /* Warning message broadcast initiated successfully */
            warning->broadcast.broadcast_started = true;
            warning->broadcast.broadcast_count++;
            ogs_info("[Warning ID:%d] Broadcast initiated successfully", warning->warning_id);
        } else {
            ogs_error("[Warning ID:%d] HTTP response error [%d]",
                warning->warning_id, recvmsg->res_status);
        }
        break;

    case PWSIWS_WARNING_MESSAGE_UPDATE:
    case PWSIWS_WARNING_MESSAGE_RETRANSMISSION:
        if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {
            /* Warning message update/retransmission initiated */
            warning->broadcast.broadcast_count++;
            ogs_info("[Warning ID:%d] Update/retransmission successful", warning->warning_id);
        } else if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_ACCEPTED) {
            if (recvmsg->http.location) {
                pwsiws_warning_set_paging_nonuen2message_location(
                        warning, recvmsg->http.location);
                ogs_info("[Warning ID:%d] Message accepted, location: %s", 
                    warning->warning_id, recvmsg->http.location);
            } else {
                ogs_error("No HTTP Location");
            }
        } else {
            ogs_error("[Warning ID:%d] HTTP response error [status:%d]",
                warning->warning_id, recvmsg->res_status);
        }
        break;

    case PWSIWS_WARNING_MESSAGE_STOP:
    case PWSIWS_WARNING_MESSAGE_CANCEL:
    case PWSIWS_WARNING_MESSAGE_ERROR:
        if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_ACCEPTED) {
            /* Warning message stop/cancel initiated */
            warning->broadcast.broadcast_started = false;
            ogs_info("[Warning ID:%d] Stop/cancel initiated", warning->warning_id);
        } else if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_OK) {
            /* Warning message stop/cancel completed */
            warning->broadcast.broadcast_started = false;
            ogs_info("[Warning ID:%d] Stop/cancel completed", warning->warning_id);
        } else if (recvmsg->res_status == OGS_SBI_HTTP_STATUS_NOT_FOUND) {
            /* Message not transferred, retry */
            pwsiws_nonuen2_message_transfer_param_t param;

            memset(&param, 0, sizeof(param));
            param.state = PWSIWS_WARNING_MESSAGE_RETRANSMISSION;
            param.n2smbuf =
                ngap_build_warning_message_broadcast_request_transfer(
                        warning);
            ogs_assert(param.n2smbuf);

            param.nonuen2_failure_txf_notif_uri = true;

            pwsiws_nonuen2_comm_send_nonuen2_message_transfer(warning, &param);
            ogs_info("[Warning ID:%d] Retrying broadcast", warning->warning_id);
        } else {
            ogs_error("[Warning ID:%d] HTTP response error [status:%d]",
                warning->warning_id, recvmsg->res_status);
        }
        break;

    default:
        ogs_fatal("Unexpected state [%d]", state);
        ogs_assert_if_reached();
    }

    return true;
}

bool pwsiws_nonuen2_comm_handle_nonuen2_message_transfer_failure_notify(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    pwsiws_warning_t *warning = NULL;
    char *failure_uri = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    /* Extract failure notification URI from location or custom field */
    if (recvmsg->http.location) {
        failure_uri = recvmsg->http.location;
    } else if (recvmsg->http.custom.callback) {
        failure_uri = recvmsg->http.custom.callback;
    }

    if (!failure_uri) {
        ogs_error("No failure notification URI");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No failure notification URI", NULL, NULL));
        return false;
    }

    warning = pwsiws_warning_find_by_paging_nonuen2message_location(failure_uri);
    if (!warning) {
        ogs_error("Warning not found for URI: %s", failure_uri);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_NOT_FOUND,
                recvmsg, failure_uri, NULL, NULL));
        return false;
    }

    /*
     * TODO:
     *
     * TS23.502 4.2.3.3 Network Triggered Service Request
     *
     * 5. [Conditional] AMF to PWS-IWS:
     * Namf_Communication_NonUeN2Transfer Failure Notification.
     *
     * When a Namf_Communication_NonUeN2Transfer Failure Notification
     * is received, PWS-IWS may retry the warning message broadcast
     * or take appropriate action based on the failure cause.
     */

    ogs_info("[Warning ID:%d] Received failure notification", warning->warning_id);

    ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));
    return true;
}