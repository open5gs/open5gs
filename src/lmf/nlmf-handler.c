/*
 * Copyright (C) 2025 by Juraj Elias <juraj.elias@gmail.com>
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
#include "nrppa-parse.h"
#include "nrppa-types.h"
#include "namf-path.h"

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
     * - If NCGI/ECGI is provided, use CELLID (basic cell ID with provided info)
     * - If NCGI/ECGI is missing:
     *   * Check LocationQoS: if high accuracy requested (hAccuracy < 100m), use ECID
     *   * Otherwise, default to CELLID and query AMF location-info API for Cell ID
     */
    if (input_data->ncgi || input_data->ecgi) {
        location_request->positioning_method = ogs_strdup("CELLID");
        ogs_info("[%s] Positioning method: CELLID (cell info provided in request)",
                location_request->supi);
    } else {
        /* No cell info provided - check LocationQoS to determine method */
        bool use_ecid = false;
        bool explicit_ecid = false;
        
        if (input_data->location_qo_s && input_data->location_qo_s->is_h_accuracy) {
            float h_accuracy = input_data->location_qo_s->h_accuracy;
            
            /* Explicit ECID request: hAccuracy < 1m (effectively 0 or very low) */
            if (h_accuracy >= 0.0f && h_accuracy < 1.0f) {
                use_ecid = true;
                explicit_ecid = true;
                location_request->ecid_fallback_to_cellid = true;  /* Enable fallback for explicit ECID requests */
                ogs_info("[%s] Explicit ECID request (hAccuracy=%.2fm), enabling CELLID fallback",
                        location_request->supi, h_accuracy);
            }
            /* High accuracy request: hAccuracy < 100m (but >= 1m) - use ECID without fallback */
            else if (h_accuracy >= 1.0f && h_accuracy < 100.0f) {
                use_ecid = true;
                explicit_ecid = false;
                location_request->ecid_fallback_to_cellid = false;  /* No fallback for high-accuracy requests */
                ogs_info("[%s] High accuracy requested (hAccuracy=%.1fm), using ECID positioning",
                        location_request->supi, h_accuracy);
            }
        }
        
        if (use_ecid) {
            location_request->positioning_method = ogs_strdup("ECID");
            if (explicit_ecid) {
                ogs_info("[%s] Positioning method: ECID (explicitly requested, will fallback to CELLID if ECID fails)",
                        location_request->supi);
            } else {
                ogs_info("[%s] Positioning method: ECID (Enhanced Cell ID via NRPPa)",
                        location_request->supi);
            }
        } else {
            location_request->positioning_method = ogs_strdup("CELLID");
            location_request->ecid_fallback_to_cellid = false;  /* Not applicable for CELLID */
            ogs_info("[%s] Positioning method: CELLID (will query AMF location-info API for Cell ID)",
                    location_request->supi);
        }
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
        /* Don't free recvmsg->h here - it's shared with request->h
         * which will be freed by the server framework */
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
        /* Only send error if location_request still exists (error wasn't already sent) */
        if (location_request->input_message) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream, OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                    location_request->input_message, "Location determination failed", NULL, NULL));
            /* Free InputData explicitly before freeing message */
            if (location_request->input_message->InputData) {
                OpenAPI_input_data_free(location_request->input_message->InputData);
                location_request->input_message->InputData = NULL;
            }
            ogs_sbi_message_free(location_request->input_message);
            ogs_free(location_request->input_message);
            location_request->input_message = NULL;
        }
        /* Only remove if it wasn't already removed */
        if (location_request->supi) {
            lmf_location_request_remove(location_request);
        }
        return rv;
    }

    /* Response will be sent asynchronously when location is determined */
    /* input_message will be freed in lmf_location_request_remove() */
    return OGS_OK;
}

int lmf_nlmf_handle_nrppa_measurement_notification(
        ogs_sbi_stream_t *stream, ogs_sbi_message_t *recvmsg)
{
    int rv;
    lmf_location_request_t *location_request = NULL;
    ogs_pkbuf_t *nrppa_pkbuf = NULL;
    lmf_nrppa_pdu_t nrppa_pdu;
    uint32_t measurement_id = 0;
    char *measurement_id_str = NULL;

    ogs_assert(stream);
    ogs_assert(recvmsg);

    /* Extract measurement ID from resource component[1] */
    if (!recvmsg->h.resource.component[1]) {
        ogs_error("No measurement ID in NRPPa measurement notification");
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No measurement ID", NULL, NULL));
        /* Don't free recvmsg->h here - it's shared with request->h
         * which will be freed by the server framework */
        ogs_sbi_message_free(recvmsg);
        return OGS_ERROR;
    }

    measurement_id_str = recvmsg->h.resource.component[1];
    measurement_id = (uint32_t)strtoul(measurement_id_str, NULL, 10);
    
    if (measurement_id == 0) {
        ogs_error("Invalid measurement ID in notification: %s", measurement_id_str);
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Invalid measurement ID", NULL, NULL));
        /* Don't free recvmsg->h here - it's shared with request->h
         * which will be freed by the server framework */
        ogs_sbi_message_free(recvmsg);
        return OGS_ERROR;
    }

    ogs_info("Received NRPPa measurement notification for measurement_id=%u", measurement_id);

    /* Find location request by measurement ID */
    location_request = lmf_location_request_find_by_measurement_id(measurement_id);
    if (!location_request) {
        ogs_warn("No location request found for measurement_id=%u. Request may have timed out or been removed.",
                measurement_id);
        /* Send success response anyway - AMF doesn't need to know */
        ogs_sbi_response_t *response = ogs_sbi_response_new();
        if (response) {
            response->status = OGS_SBI_HTTP_STATUS_NO_CONTENT;
            ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        }
        /* Don't free recvmsg->h here - it's shared with request->h
         * which will be freed by the server framework */
        ogs_sbi_message_free(recvmsg);
        return OGS_OK;
    }

    ogs_info("[%s] Found location request for measurement_id=%u, processing NRPPa notification",
            location_request->supi ? location_request->supi : "Unknown", measurement_id);

    /* Extract NRPPa PDU from multipart message */
    if (recvmsg->num_of_part > 0) {
        int i;
        for (i = 0; i < recvmsg->num_of_part; i++) {
            if (recvmsg->part[i].pkbuf &&
                recvmsg->part[i].content_type &&
                strstr(recvmsg->part[i].content_type, "vnd.3gpp.nrppa")) {
                nrppa_pkbuf = ogs_pkbuf_copy(recvmsg->part[i].pkbuf);
                break;
            }
        }
    }

    if (!nrppa_pkbuf) {
        ogs_error("[%s] No NRPPa PDU in measurement notification",
                location_request->supi);
        
        /* Send error response to AMF */
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "No NRPPa PDU in notification", NULL, NULL));
        /* Don't free recvmsg->h here - it's shared with request->h
         * which will be freed by the server framework */
        ogs_sbi_message_free(recvmsg);
        
        /* Send error to client */
        ogs_sbi_stream_t *client_stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (client_stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(client_stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "No NRPPa PDU in callback notification",
                    "AMF callback notification does not contain NRPPa PDU", NULL));
        }
        lmf_location_request_remove(location_request);
        return OGS_ERROR;
    }

    /* Parse NRPPa PDU */
    rv = lmf_nrppa_parse_pdu(nrppa_pkbuf, &nrppa_pdu);
    ogs_pkbuf_free(nrppa_pkbuf);

    if (rv != OGS_OK) {
        ogs_error("[%s] Failed to parse NRPPa PDU from callback notification",
                location_request->supi);
        
        /* Send error response to AMF */
        ogs_assert(true ==
            ogs_sbi_server_send_error(stream,
                OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                recvmsg, "Failed to parse NRPPa PDU", NULL, NULL));
        /* Don't free recvmsg->h here - it's shared with request->h
         * which will be freed by the server framework */
        ogs_sbi_message_free(recvmsg);
        
        /* Send error to client */
        ogs_sbi_stream_t *client_stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (client_stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(client_stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "Failed to parse NRPPa PDU from callback",
                    "NRPPa PDU parsing error in callback notification", NULL));
        }
        lmf_location_request_remove(location_request);
        return OGS_ERROR;
    }

    /* Send success response to AMF */
    ogs_sbi_response_t *response = ogs_sbi_response_new();
    if (response) {
        response->status = OGS_SBI_HTTP_STATUS_NO_CONTENT;
        ogs_assert(true == ogs_sbi_server_send_response(stream, response));
        ogs_info("[%s] Sent HTTP 204 No Content to AMF for callback notification",
                location_request->supi);
    }

    /* Free the request message */
    /* Don't free recvmsg->h here - it's shared with request->h
     * which will be freed by the server framework */
    ogs_sbi_message_free(recvmsg);

    /* Process NRPPa response similar to regular response handler */
    if (nrppa_pdu.message_type == NRPPA_MESSAGE_TYPE_ECID_MEASUREMENT_INITIATION_RESPONSE) {
        ogs_info("[%s] ECID measurement response received via callback, processing",
                location_request->supi);

        /* Continue with location determination process */
        rv = lmf_location_determine_ecid(location_request, &nrppa_pdu.u.ecid_response);
        if (rv != OGS_OK) {
            ogs_error("[%s] lmf_location_determine_ecid() failed",
                    location_request->supi);
        }
        /* Response will be sent to client in lmf_location_determine_ecid */
    } else if (nrppa_pdu.message_type == NRPPA_MESSAGE_TYPE_ECID_MEASUREMENT_FAILURE_INDICATION) {
        ogs_warn("[%s] ECID measurement failed (cause=%d), checking fallback",
                location_request->supi, nrppa_pdu.u.ecid_failure.cause);

        /* Check if we should fallback to CELLID */
        if (location_request->ecid_fallback_to_cellid) {
            ogs_info("[%s] ECID failed, falling back to CELLID positioning",
                    location_request->supi);
            
            /* Switch to CELLID positioning method */
            if (location_request->positioning_method) {
                ogs_free(location_request->positioning_method);
            }
            location_request->positioning_method = ogs_strdup("CELLID");
            
            /* Start CELLID positioning by querying AMF for Cell ID */
            rv = lmf_amf_send_location_info_request(location_request);
            if (rv != OGS_OK) {
                ogs_error("[%s] CELLID fallback failed: unable to query AMF for Cell ID",
                        location_request->supi);
                
                ogs_sbi_stream_t *client_stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
                if (client_stream) {
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(client_stream,
                            OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE, NULL,
                            "ECID failed and CELLID fallback also failed",
                            "Unable to determine location via ECID or CELLID", NULL));
                }
                lmf_location_request_remove(location_request);
            }
            /* Response will be handled asynchronously in lmf_namf_handler_location_info_response */
        } else {
            /* No fallback - send error response to client */
            ogs_sbi_stream_t *client_stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
            if (client_stream) {
                char *error_detail = ogs_msprintf("gNB rejected NRPPa measurement request (cause=%d)",
                        nrppa_pdu.u.ecid_failure.cause);
                ogs_info("[%s] ECID failed with no fallback enabled, sending error to client",
                        location_request->supi);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(client_stream,
                        OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE, NULL,
                        "NRPPa measurement request rejected by gNB",
                        error_detail, NULL));
                if (error_detail)
                    ogs_free(error_detail);
            } else {
                ogs_warn("[%s] Client stream [%d] not found for error response (may have timed out)",
                        location_request->supi, location_request->stream_id);
            }
            lmf_location_request_remove(location_request);
        }
    } else {
        ogs_error("[%s] Unknown NRPPa message type in callback: %d",
                location_request->supi, nrppa_pdu.message_type);
        
        ogs_sbi_stream_t *client_stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (client_stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(client_stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "Unknown NRPPa message type in callback",
                    "Unexpected NRPPa message type", NULL));
        }
        lmf_location_request_remove(location_request);
    }

    return OGS_OK;
}

