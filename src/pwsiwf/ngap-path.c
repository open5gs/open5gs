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

#include "ngap-path.h"
#include "ngap-build.h"
#include "sbi-path.h"

int pwsiws_ngap_send_write_replace_warning_request(sbc_pws_data_t *sbc_pws)
{
    ogs_pkbuf_t *ngapbuf = NULL;
    pwsiws_warning_t *warning = NULL;
    pwsiws_connection_t *connection = NULL;
    pwsiws_nonuen2_message_transfer_param_t param;

    ogs_assert(sbc_pws);

    /* Create a warning message from SBC PWS data */
    /* For now, we'll use the first connection or create a dummy one */
    connection = ogs_list_first(&pwsiws_self()->connection_list);
    if (!connection) {
        ogs_error("No PWS-IWS connection available");
        return OGS_ERROR;
    }

    warning = pwsiws_warning_add(connection);
    if (!warning) {
        ogs_error("Failed to create PWS warning");
        return OGS_ERROR;
    }

    /* Fill warning data from SBC PWS data */
    warning->warning_data.message_type = 1; /* Write Replace Warning */
    warning->warning_data.serial_number = sbc_pws->serial_number;
    warning->warning_data.repetition_period = sbc_pws->repetition_period;
    warning->warning_data.number_of_broadcast = sbc_pws->number_of_broadcast;
    warning->warning_data.data_coding_scheme = sbc_pws->data_coding_scheme;
    
    if (sbc_pws->message_length > 0 && sbc_pws->message_length <= sizeof(warning->warning_data.message_contents)) {
        warning->warning_data.message_length = sbc_pws->message_length;
        memcpy(warning->warning_data.message_contents, sbc_pws->message_contents, sbc_pws->message_length);
    }

    /* Build NGAP message */
    ngapbuf = ngap_build_warning_message_broadcast_request_transfer(warning);
    if (!ngapbuf) {
        ogs_error("Failed to build NGAP warning message");
        pwsiws_warning_remove(warning);
        return OGS_ERROR;
    }

    /* Set up transfer parameters */
    memset(&param, 0, sizeof(param));
    param.state = PWSIWS_WARNING_MESSAGE_BROADCAST;
    param.n2smbuf = ngapbuf;
    param.nonuen2_failure_txf_notif_uri = true;

    /* Send to AMF via SBI */
    if (pwsiws_nonuen2_comm_send_nonuen2_message_transfer(warning, &param) != OGS_OK) {
        ogs_error("Failed to send Non-UE N2 message transfer");
        pwsiws_warning_remove(warning);
        return OGS_ERROR;
    }

    ogs_info("PWS warning message sent to AMF: WarningID=%d, MessageID=%d", 
            warning->warning_id, warning->message_id);

    return OGS_OK;
}

int pwsiws_ngap_send_kill_request(sbc_pws_data_t *sbc_pws)
{
    ogs_pkbuf_t *ngapbuf = NULL;
    pwsiws_warning_t *warning = NULL;
    pwsiws_connection_t *connection = NULL;
    pwsiws_nonuen2_message_transfer_param_t param;

    ogs_assert(sbc_pws);

    /* Find existing warning by message ID */
    warning = pwsiws_warning_find_by_warning_id(sbc_pws->message_id);
    if (!warning) {
        ogs_error("Warning not found for message ID: %d", sbc_pws->message_id);
        return OGS_ERROR;
    }

    /* Build NGAP cancel message */
    ngapbuf = ngap_build_warning_message_cancel_request_transfer(warning);
    if (!ngapbuf) {
        ogs_error("Failed to build NGAP cancel message");
        return OGS_ERROR;
    }

    /* Set up transfer parameters */
    memset(&param, 0, sizeof(param));
    param.state = PWSIWS_WARNING_MESSAGE_CANCEL;
    param.n2smbuf = ngapbuf;
    param.nonuen2_failure_txf_notif_uri = true;

    /* Send to AMF via SBI */
    if (pwsiws_nonuen2_comm_send_nonuen2_message_transfer(warning, &param) != OGS_OK) {
        ogs_error("Failed to send Non-UE N2 cancel message transfer");
        return OGS_ERROR;
    }

    ogs_info("PWS cancel message sent to AMF: WarningID=%d, MessageID=%d", 
            warning->warning_id, warning->message_id);

    return OGS_OK;
}

int pwsiws_ngap_send_to_amf(ogs_pkbuf_t *buf)
{
    if (!pwsiws_self()->amf_sbi) {
        ogs_error("AMF SBI client not configured");
        ogs_pkbuf_free(buf);
        return OGS_ERROR;
    }

    /* This function is used by the legacy SBC handler */
    /* For now, we'll just log and free the buffer */
    ogs_info("Legacy NGAP send to AMF called (stub implementation)");
    ogs_pkbuf_free(buf);
    return OGS_OK;
} 