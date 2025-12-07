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

#include "nlmf-handler.h"
#include "location-determination.h"
#include "context.h"

int lmf_nlmf_handle_determine_location(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv;
    lmf_location_request_t *location_request = NULL;
    OpenAPI_input_data_t *input_data = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    /* Parse InputData from message */
    input_data = recvmsg->InputData;
    if (!input_data) {
        ogs_error("No InputData in determine location request");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No InputData", NULL, NULL));
        return OGS_ERROR;
    }

    /* Extract SUPI (required) */
    if (!input_data->supi) {
        ogs_error("No SUPI in InputData");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No SUPI in InputData", NULL, NULL));
        return OGS_ERROR;
    }

    /* Create location request context */
    location_request = lmf_location_request_add();
    if (!location_request) {
        ogs_error("lmf_location_request_add() failed");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                recvmsg, "Internal server error", NULL, NULL));
        return OGS_ERROR;
    }

    /* Store stream ID for async response */
    location_request->stream_id = ogs_sbi_id_from_stream(stream);

    /* Extract SUPI */
    location_request->supi = ogs_strdup(input_data->supi);
    ogs_assert(location_request->supi);

    /* Extract AMF ID if present */
    if (input_data->amf_id) {
        location_request->amf_id = ogs_strdup(input_data->amf_id);
        ogs_assert(location_request->amf_id);
    }

    /* Determine positioning method:
     * - If NCGI/ECGI is provided, default to CELLID (basic cell ID)
     * - Otherwise, default to ECID (Enhanced Cell ID via NRPPa)
     * In the future, this could be enhanced to check LocationQoS or LDR type
     */
    if (input_data->ncgi || input_data->ecgi) {
        location_request->positioning_method = ogs_strdup("CELLID");
        ogs_info("[%s] Positioning method: CELLID (cell info provided)",
                location_request->supi);
    } else {
        location_request->positioning_method = ogs_strdup("ECID");
        ogs_info("[%s] Positioning method: ECID (requires NRPPa measurement)",
                location_request->supi);
    }
    ogs_assert(location_request->positioning_method);

    ogs_info("[%s] Location request: SUPI=%s, AMF_ID=%s, Method=%s",
            location_request->supi,
            location_request->supi,
            location_request->amf_id ? location_request->amf_id : "N/A",
            location_request->positioning_method);

    /* Store the input message in location_request for later cleanup */
    /* We need to keep it because it contains allocated OpenAPI objects (InputData) */
    location_request->input_message = ogs_calloc(1, sizeof(ogs_sbi_message_t));
    if (!location_request->input_message) {
        ogs_error("[%s] Failed to allocate input_message",
                location_request->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                recvmsg, "Internal server error", NULL, NULL));
        ogs_sbi_message_free(recvmsg);
        lmf_location_request_remove(location_request);
        return OGS_ERROR;
    }
    /* Copy the message structure - this transfers ownership of allocated members */
    memcpy(location_request->input_message, recvmsg, sizeof(ogs_sbi_message_t));
    /* Clear the original message to prevent double-free when caller frees it */
    memset(recvmsg, 0, sizeof(ogs_sbi_message_t));

    /* Start location determination process */
    rv = lmf_location_determine(location_request);
    if (rv != OGS_OK) {
        ogs_error("[%s] lmf_location_determine() failed",
                location_request->supi);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                location_request->input_message, "Location determination failed", NULL, NULL));
        ogs_sbi_message_free(location_request->input_message);
        ogs_free(location_request->input_message);
        location_request->input_message = NULL;
        lmf_location_request_remove(location_request);
        return rv;
    }

    /* Response will be sent asynchronously when location is determined */
    /* input_message will be freed in lmf_location_request_remove() */
    return OGS_OK;
}

