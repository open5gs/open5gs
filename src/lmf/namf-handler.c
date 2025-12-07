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

#include "namf-handler.h"
#include "nrppa-parse.h"
#include "location-determination.h"

#include <string.h>

void lmf_namf_handler_nrppa_measurement_response(
        int status, ogs_sbi_response_t *response, void *data)
{
    int rv;
    ogs_sbi_message_t message;
    lmf_location_request_t *location_request = NULL;
    ogs_pkbuf_t *nrppa_pkbuf = NULL;
    lmf_nrppa_pdu_t nrppa_pdu;

    ogs_assert(response);
    ogs_assert(data);

    location_request = data;

    if (status != OGS_OK) {
        ogs_sbi_stream_t *stream = NULL;
        int http_status = OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR;
        char *error_detail = NULL;

        ogs_error("[%s] NRPPa measurement request failed [%d]",
                location_request->supi ? location_request->supi : "Unknown", status);

        /* Send error response back to client */
        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            /* Extract error details from AMF response if available */
            if (response && response->status >= 400) {
                http_status = response->status;
                error_detail = ogs_msprintf("AMF returned HTTP %d for NRPPa request", response->status);
            } else {
                error_detail = ogs_strdup("AMF NRPPa measurement request failed");
            }

            ogs_assert(true ==
                ogs_sbi_server_send_error(stream, http_status, NULL,
                    "NRPPa measurement request failed", error_detail, NULL));

            if (error_detail)
                ogs_free(error_detail);

            ogs_info("[%s] Sent error response to client for AMF NRPPa failure",
                    location_request->supi ? location_request->supi : "Unknown");
        } else {
            ogs_warn("[%s] Stream [%d] not found for error response",
                    location_request->supi ? location_request->supi : "Unknown",
                    location_request->stream_id);
        }

        ogs_sbi_response_free(response);
        lmf_location_request_remove(location_request);
        return;
    }

    ogs_info("[%s] Received NRPPa measurement response",
            location_request->supi ? location_request->supi : "Unknown");

    /* Parse response message */
    rv = ogs_sbi_parse_response(&message, response);
    if (rv != OGS_OK) {
        ogs_sbi_stream_t *stream = NULL;

        ogs_error("[%s] Failed to parse NRPPa measurement response",
                location_request->supi);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "Failed to parse NRPPa measurement response",
                    "AMF response could not be parsed", NULL));
        }

        lmf_location_request_remove(location_request);
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        return;
    }

    /* Extract NRPPa PDU from multipart message */
    if (message.num_of_part > 0) {
        /* Find NRPPa PDU part */
        int i;
        for (i = 0; i < message.num_of_part; i++) {
            if (message.part[i].pkbuf &&
                message.part[i].content_type &&
                strstr(message.part[i].content_type, "vnd.3gpp.nrppa")) {
                nrppa_pkbuf = ogs_pkbuf_copy(message.part[i].pkbuf);
                break;
            }
        }
    }

    if (!nrppa_pkbuf) {
        ogs_sbi_stream_t *stream = NULL;

        ogs_error("[%s] No NRPPa PDU in measurement response",
                location_request->supi);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "No NRPPa PDU in measurement response",
                    "AMF response does not contain NRPPa PDU", NULL));
        }

        lmf_location_request_remove(location_request);
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        return;
    }

    /* Parse NRPPa PDU */
    rv = lmf_nrppa_parse_pdu(nrppa_pkbuf, &nrppa_pdu);
    ogs_pkbuf_free(nrppa_pkbuf);

    if (rv != OGS_OK) {
        ogs_sbi_stream_t *stream = NULL;

        ogs_error("[%s] Failed to parse NRPPa PDU",
                location_request->supi);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "Failed to parse NRPPa PDU",
                    "NRPPa PDU parsing error", NULL));
        }

        lmf_location_request_remove(location_request);
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        return;
    }

    /* Store parsed NRPPa response in location request */
    if (nrppa_pdu.message_type == NRPPA_MESSAGE_TYPE_ECID_MEASUREMENT_INITIATION_RESPONSE) {
        ogs_info("[%s] ECID measurement response parsed successfully",
                location_request->supi);

        /* Continue with location determination process */
        rv = lmf_location_determine_ecid(location_request, &nrppa_pdu.u.ecid_response);
        if (rv != OGS_OK) {
            ogs_error("[%s] lmf_location_determine_ecid() failed",
                    location_request->supi);
        }
    } else if (nrppa_pdu.message_type == NRPPA_MESSAGE_TYPE_ECID_MEASUREMENT_FAILURE_INDICATION) {
        ogs_sbi_stream_t *stream = NULL;
        char *error_detail = NULL;

        ogs_warn("[%s] gNB rejected NRPPa measurement request (cause=%u)",
                location_request->supi, nrppa_pdu.u.ecid_failure.cause);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            error_detail = ogs_msprintf("gNB rejected NRPPa measurement request (cause=%u)",
                    nrppa_pdu.u.ecid_failure.cause);

            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE, NULL,
                    "NRPPa measurement request rejected by gNB",
                    error_detail, NULL));

            if (error_detail)
                ogs_free(error_detail);
        }

        /* Clean up */
        lmf_location_request_remove(location_request);
    } else {
        ogs_sbi_stream_t *stream = NULL;
        char *error_detail = NULL;

        ogs_error("[%s] Unexpected NRPPa message type: %u",
                location_request->supi, nrppa_pdu.message_type);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            error_detail = ogs_msprintf("Unexpected NRPPa message type: %u",
                    nrppa_pdu.message_type);

            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "Unexpected NRPPa message type",
                    error_detail, NULL));

            if (error_detail)
                ogs_free(error_detail);
        }

        lmf_location_request_remove(location_request);
    }

    ogs_sbi_message_free(&message);
    ogs_sbi_response_free(response);
}

