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

#include "sbcap-handler.h"
#include "sbcap-path.h"
#include "sbi-path.h"
#include "ngap-build.h"

void pwsiws_sbcap_handle_write_replace_warning_response(
        pwsiws_connection_t *connection, ogs_sbcap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];

    ogs_assert(connection);
    ogs_assert(message);

    ogs_info("Write_Replace_Warning_Response received from [%s]",
            OGS_ADDR(connection->addr, buf));
}

void pwsiws_sbcap_handle_write_replace_warning_request(
        pwsiws_connection_t *connection, ogs_sbcap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;
    pwsiws_warning_t *warning = NULL;
    uint16_t message_id = 0;
    uint16_t serial_number_i = 0;

    SBCAP_InitiatingMessage_t *initiatingMessage = NULL;
    SBCAP_Write_Replace_Warning_Request_t *SbcapWriteReplaceWarningRequest = NULL;
    SBCAP_Write_Replace_Warning_Request_IEs_t *ie = NULL;
    SBCAP_Message_Identifier_t *message_identifier = NULL;
    SBCAP_Serial_Number_t *serial_number = NULL;
    SBCAP_Repetition_Period_t *repetition_period = NULL;
    SBCAP_NumberOfBroadcasts_t *broadcast_requested = NULL;
    SBCAP_Data_Coding_Scheme_t *data_coding_scheme = NULL;
    SBCAP_Warning_Message_Content_t *warning_message_content = NULL;
    SBCAP_Warning_Type_t *warning_type = NULL;
    SBCAP_Warning_Security_Information_t *warning_security_info = NULL;
    SBCAP_Concurrent_Warning_Message_Indicator_t *concurrent_indicator = NULL;
    SBCAP_Send_Write_Replace_Warning_Indication_t *send_indication = NULL;

    ogs_assert(connection);
    ogs_assert(connection->addr);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    SbcapWriteReplaceWarningRequest =
        &initiatingMessage->value.choice.Write_Replace_Warning_Request;
    ogs_assert(SbcapWriteReplaceWarningRequest);

    ogs_info("Write_Replace_Warning_Request received from [%s]",
            OGS_ADDR(connection->addr, buf));

    ogs_info("Number of IEs: %d", SbcapWriteReplaceWarningRequest->protocolIEs.list.count);

    for (i = 0; i < SbcapWriteReplaceWarningRequest->protocolIEs.list.count; i++) {
        ie = SbcapWriteReplaceWarningRequest->protocolIEs.list.array[i];
        switch(ie->id) {
            case SBCAP_ProtocolIE_ID_id_Message_Identifier:
                message_identifier = &ie->value.choice.Message_Identifier;
                memcpy(&message_id, message_identifier->buf, sizeof(uint16_t));
                ogs_info("message_identifier: %d", message_id);
                break;

            case SBCAP_ProtocolIE_ID_id_Serial_Number:
                serial_number = &ie->value.choice.Serial_Number;
                memcpy(&serial_number_i, serial_number->buf, sizeof(uint16_t));
                ogs_info("serial_number: %d", serial_number_i);
                break;

            case SBCAP_ProtocolIE_ID_id_Repetition_Period:
                repetition_period = &ie->value.choice.Repetition_Period;
                ogs_info("repetition_period: %ld", *repetition_period);
                break;

            case SBCAP_ProtocolIE_ID_id_Number_Of_Broadcast_Requested:
                broadcast_requested = &ie->value.choice.Number_of_Broadcasts_Requested;
                ogs_info("broadcast_requested: %ld", *broadcast_requested);
                break;

            case SBCAP_ProtocolIE_ID_id_Data_Coding_Scheme:
                data_coding_scheme = &ie->value.choice.Data_Coding_Scheme;
                uint8_t data_coding_scheme_i = 0;
                memcpy(&data_coding_scheme_i, data_coding_scheme->buf, sizeof(uint8_t));
                ogs_info("data_coding_scheme: %d", data_coding_scheme_i);
                break;

            case SBCAP_ProtocolIE_ID_id_Warning_Message_Content:
                warning_message_content = &ie->value.choice.Warning_Message_Content;
                ogs_info("warning_message_content: size=%ld bytes", warning_message_content->size);
                // Only print hex dump if content is reasonable size and not corrupted
                if (warning_message_content->size > 0 && warning_message_content->size <= 960) {
                    ogs_log_hexdump(OGS_LOG_DEBUG, warning_message_content->buf, warning_message_content->size);
                } else {
                    ogs_warn("Warning message content has invalid size: %ld", warning_message_content->size);
                }
                break;

            case SBCAP_ProtocolIE_ID_id_Warning_Type:
                warning_type = &ie->value.choice.Warning_Type;
                ogs_info("warning_type: size=%ld bytes", warning_type->size);
                if (warning_type->size > 0) {
                    ogs_log_hexdump(OGS_LOG_DEBUG, warning_type->buf, warning_type->size);
                }
                break;

            case SBCAP_ProtocolIE_ID_id_Warning_Security_Information:
                warning_security_info = &ie->value.choice.Warning_Security_Information;
                ogs_info("warning_security_information: size=%ld bytes", warning_security_info->size);
                break;

            case SBCAP_ProtocolIE_ID_id_Concurrent_Warning_Message_Indicator:
                concurrent_indicator = &ie->value.choice.Concurrent_Warning_Message_Indicator;
                ogs_info("concurrent_warning_message_indicator: %ld", *concurrent_indicator);
                break;

            case SBCAP_ProtocolIE_ID_id_Send_Write_Replace_Warning_Indication:
                send_indication = &ie->value.choice.Send_Write_Replace_Warning_Indication;
                ogs_info("send_write_replace_warning_indication: %ld", *send_indication);
                break;

            default:
                ogs_warn("Unhandled IE: %ld", ie->id);
                break;
        }
    }

    warning = pwsiws_warning_add(connection);
    if (!warning) {
        ogs_error("Failed to create a new warning context");
        return;
    }

    warning->message_id = message_id;
    warning->warning_data.serial_number = serial_number_i;

    if (repetition_period)
        warning->warning_data.repetition_period = *repetition_period;
    if (broadcast_requested)
        warning->warning_data.number_of_broadcast = *broadcast_requested;
    if (data_coding_scheme) {
        uint8_t dcs = 0;
        memcpy(&dcs, data_coding_scheme->buf, sizeof(uint8_t));
        warning->warning_data.data_coding_scheme = dcs;
    }
    if (warning_message_content && warning_message_content->size > 0) {
        memcpy(warning->warning_data.message_contents,
                warning_message_content->buf, warning_message_content->size);
        warning->warning_data.message_length = warning_message_content->size;
    }

    pwsiws_nonuen2_message_transfer_param_t param;
    memset(&param, 0, sizeof(param));
    param.state = PWSIWS_WARNING_MESSAGE_BROADCAST;
    param.nonuen2_failure_txf_notif_uri = true;

    param.n2smbuf = ngap_build_warning_message_broadcast_request_transfer(warning);
    if (!param.n2smbuf) {
        ogs_error("Failed to build NGAP warning message broadcast request transfer");
        pwsiws_warning_remove(warning);
        return;
    }

    if (pwsiws_nonuen2_comm_send_nonuen2_message_transfer(warning, &param) != OGS_OK) {
        ogs_error("Failed to send Non-UE N2 message to AMF");
        pwsiws_warning_remove(warning);
        return;
    }

    ogs_info("Forwarded Write-Replace-Warning-Request to AMF");

    ogs_info("Write_Replace_Warning_Request processed from [%s]",
            OGS_ADDR(connection->addr, buf));
}

void pwsiws_sbcap_handle_stop_warning_request(
        pwsiws_connection_t *connection, ogs_sbcap_message_t *message)
{
    char buf[OGS_ADDRSTRLEN];
    int i;

    SBCAP_InitiatingMessage_t *initiatingMessage = NULL;
    SBCAP_Stop_Warning_Request_t *SbcapStopWarningRequest = NULL;
    SBCAP_Stop_Warning_Request_IEs_t *ie = NULL;
    SBCAP_Message_Identifier_t *message_identifier = NULL;

    ogs_assert(connection);
    ogs_assert(connection->addr);
    ogs_assert(message);

    initiatingMessage = message->choice.initiatingMessage;
    ogs_assert(initiatingMessage);
    SbcapStopWarningRequest =
        &initiatingMessage->value.choice.Stop_Warning_Request;
    ogs_assert(SbcapStopWarningRequest);

    ogs_info("Stop_Warning_Request received from [%s]",
            OGS_ADDR(connection->addr, buf));

    for (i = 0; i < SbcapStopWarningRequest->protocolIEs.list.count; i++) {
        ie = SbcapStopWarningRequest->protocolIEs.list.array[i];
        switch (ie->id) {
            case SBCAP_ProtocolIE_ID_id_Message_Identifier:
                message_identifier = &ie->value.choice.Message_Identifier;
                int message = 0;
                memcpy(&message, message_identifier->buf, sizeof(uint16_t));
                ogs_debug("message_identifier: %ld", message_identifier->size);
                ogs_info("message_identifier: %d", message);
                ogs_log_hexdump(OGS_LOG_INFO, message_identifier->buf, message_identifier->size);
                break;

            default:
                ogs_warn("Unhandled IE: %ld", ie->id);
                break;
        }
    }

    ogs_info("Stop_Warning_Request processed from [%s]",
            OGS_ADDR(connection->addr, buf));
} 