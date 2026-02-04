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

#include "namf-handler.h"
#include "nrppa-parse.h"
#include "location-determination.h"
#include "namf-path.h"

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
        ogs_error("[%s] NRPPa measurement request failed [%d]",
                location_request->supi ? location_request->supi : "Unknown", status);

        /* Check if we should fallback to CELLID */
        bool should_fallback = location_request->ecid_fallback_to_cellid;
        
        /* Also check HTTP status - 503/404 might indicate UE not reachable.
         * But only fallback if explicitly enabled (hAccuracy < 1m).
         * For high-accuracy requests (hAccuracy >= 1m, < 100m), fallback should NOT occur. */
        if (response) {
            if (response->status == OGS_SBI_HTTP_STATUS_NOT_FOUND ||
                response->status == OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE) {
                if (should_fallback) {
                    ogs_info("[%s] ECID failed with HTTP %d (UE not found/unavailable), attempting CELLID fallback",
                            location_request->supi, response->status);
                } else {
                    ogs_warn("[%s] ECID failed with HTTP %d, but fallback not enabled (high-accuracy request). Returning error.",
                            location_request->supi, response->status);
                }
            }
        }

        if (should_fallback) {
            ogs_info("[%s] ECID failed, falling back to CELLID positioning",
                    location_request->supi);
            
            /* Switch to CELLID positioning method */
            if (location_request->positioning_method) {
                ogs_free(location_request->positioning_method);
            }
            location_request->positioning_method = ogs_strdup("CELLID");
            
            /* Free response - we'll start fresh with CELLID */
            ogs_sbi_response_free(response);
            
            /* Start CELLID positioning by querying AMF for Cell ID */
            rv = lmf_amf_send_location_info_request(location_request);
            if (rv != OGS_OK) {
                ogs_error("[%s] CELLID fallback failed: unable to query AMF for Cell ID",
                        location_request->supi);
                
                ogs_sbi_stream_t *stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
                if (stream) {
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE, NULL,
                            "ECID failed and CELLID fallback also failed",
                            "Unable to determine location via ECID or CELLID", NULL));
                }
                lmf_location_request_remove(location_request);
            }
            /* Response will be handled asynchronously in lmf_namf_handler_location_info_response */
            return;
        }

        /* No fallback - send error response */
        ogs_sbi_stream_t *stream = NULL;
        int http_status = OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR;
        char *error_detail = NULL;

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

    ogs_info("[%s] Received NRPPa measurement response (HTTP %d)",
            location_request->supi ? location_request->supi : "Unknown",
            response->status);

    /* Handle HTTP 202 Accepted - AMF is paging the UE, will send callback notification */
    if (response->status == OGS_SBI_HTTP_STATUS_ACCEPTED) {
        ogs_sbi_message_t location_message;
        char *location_header = NULL;
        
        /* Parse response to extract Location header */
        memset(&location_message, 0, sizeof(location_message));
        if (ogs_sbi_parse_response(&location_message, response) == OGS_OK) {
            location_header = location_message.http.location;
            if (location_header) {
                char *location_dup = ogs_strdup(location_header);
                if (location_dup) {
                    ogs_info("[%s] Received HTTP 202 Accepted with Location: %s. Waiting for callback notification.",
                            location_request->supi, location_dup);
                    /* Store location header for reference (optional - callback will come via separate POST) */
                    if (location_request->callback_reference) {
                        ogs_free(location_request->callback_reference);
                    }
                    location_request->callback_reference = location_dup;
                }
            } else {
                ogs_warn("[%s] Received HTTP 202 Accepted but no Location header. Will wait for callback notification.",
                        location_request->supi);
            }
            /* Free parsed message (but not the response - we'll free that separately) */
            ogs_sbi_message_free(&location_message);
        } else {
            ogs_warn("[%s] Failed to parse HTTP 202 response, but will wait for callback notification anyway.",
                    location_request->supi);
        }
        
        /* Free response - we'll wait for the callback notification */
        ogs_sbi_response_free(response);
        
        /* Note: The callback notification will come as a separate POST request
         * to a callback URI. LMF provides a callback URI when sending the NRPPa request. */
        ogs_info("[%s] Waiting for NRPPa measurement result callback notification",
                location_request->supi);
        return;
    }

    /* Check HTTP status - if it's an error, handle it appropriately */
    if (response->status >= 400) {
        bool should_fallback = location_request->ecid_fallback_to_cellid;
        
        /* Only force fallback for specific errors that indicate NRPPa is truly not available.
         * HTTP 409 (Conflict) might be a temporary issue, so only fallback if explicitly enabled.
         * HTTP 404/503 indicate UE/AMF not available - fallback makes sense ONLY if fallback is enabled.
         * For high-accuracy requests (hAccuracy >= 1m, < 100m), fallback should NOT occur.
         */
        if (response->status == OGS_SBI_HTTP_STATUS_NOT_FOUND ||
            response->status == OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE ||
            response->status == OGS_SBI_HTTP_STATUS_REQUEST_TIMEOUT) {
            /* These errors indicate the service/UE is not available.
             * Only fallback if explicitly enabled (hAccuracy < 1m).
             * For high-accuracy requests without fallback, return error instead. */
            if (should_fallback) {
                ogs_info("[%s] ECID failed with HTTP %d, attempting CELLID fallback",
                        location_request->supi, response->status);
            } else {
                ogs_warn("[%s] ECID failed with HTTP %d, but fallback not enabled (high-accuracy request). Returning error.",
                        location_request->supi, response->status);
            }
        } else if (response->status == OGS_SBI_HTTP_STATUS_CONFLICT) {
            /* HTTP 409 might be temporary - only fallback if explicitly enabled */
            if (should_fallback) {
                ogs_info("[%s] ECID failed with HTTP 409 (Conflict), attempting CELLID fallback",
                        location_request->supi);
            } else {
                ogs_warn("[%s] ECID failed with HTTP 409 (Conflict), but fallback not enabled. Returning error.",
                        location_request->supi);
            }
        }

        if (should_fallback) {
            ogs_info("[%s] ECID failed, falling back to CELLID positioning",
                    location_request->supi);
            
            /* Switch to CELLID positioning method */
            if (location_request->positioning_method) {
                ogs_free(location_request->positioning_method);
            }
            location_request->positioning_method = ogs_strdup("CELLID");
            
            /* Free response - we'll start fresh with CELLID */
            ogs_sbi_response_free(response);
            
            /* Start CELLID positioning by querying AMF for Cell ID */
            rv = lmf_amf_send_location_info_request(location_request);
            if (rv != OGS_OK) {
                ogs_error("[%s] CELLID fallback failed: unable to query AMF for Cell ID",
                        location_request->supi);
                
                ogs_sbi_stream_t *stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
                if (stream) {
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE, NULL,
                            "ECID failed and CELLID fallback also failed",
                            "Unable to determine location via ECID or CELLID", NULL));
                }
                lmf_location_request_remove(location_request);
            }
            /* Response will be handled asynchronously in lmf_namf_handler_location_info_response */
            return;
        }

        /* No fallback - send error response */
        ogs_sbi_stream_t *stream = NULL;
        int http_status = response->status;
        char *error_detail = NULL;

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            error_detail = ogs_msprintf("AMF returned HTTP %d for NRPPa request", response->status);

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

    /* Successful response (HTTP 200-299) - parse as multipart NRPPa message */
    /* Initialize message structure before parsing */
    memset(&message, 0, sizeof(message));
    
    /* Parse response message */
    rv = ogs_sbi_parse_response(&message, response);
    if (rv != OGS_OK) {
        ogs_sbi_stream_t *stream = NULL;

        ogs_error("[%s] Failed to parse NRPPa measurement response",
                location_request->supi);

        /* Check if we should fallback on parse failure
         * Only fallback if explicitly enabled (hAccuracy < 1m)
         * If parse fails and fallback is not enabled, return error
         */
        if (location_request->ecid_fallback_to_cellid) {
            ogs_info("[%s] NRPPa parse failed, falling back to CELLID positioning",
                    location_request->supi);
            
            /* Switch to CELLID positioning method */
            if (location_request->positioning_method) {
                ogs_free(location_request->positioning_method);
            }
            location_request->positioning_method = ogs_strdup("CELLID");
            
            /* Free response and message - we'll start fresh with CELLID */
            ogs_sbi_message_free(&message);
            ogs_sbi_response_free(response);
            
            /* Start CELLID positioning by querying AMF for Cell ID */
            rv = lmf_amf_send_location_info_request(location_request);
            if (rv != OGS_OK) {
                ogs_error("[%s] CELLID fallback failed: unable to query AMF for Cell ID",
                        location_request->supi);
                
                ogs_sbi_stream_t *stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
                if (stream) {
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE, NULL,
                            "ECID failed and CELLID fallback also failed",
                            "Unable to determine location via ECID or CELLID", NULL));
                }
                lmf_location_request_remove(location_request);
            }
            return;
        }

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "Failed to parse NRPPa measurement response",
                    "AMF response could not be parsed", NULL));
        }

        /* NULL out shared pointers, then free message before response */
        {
            int i;
            for (i = 0; i < message.num_of_part; i++) {
                message.part[i].content_id = NULL;
                message.part[i].content_type = NULL;
            }
        }
        /* N1N2MessageTransferRspData is OWNED by message, let ogs_sbi_message_free() free it */
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        lmf_location_request_remove(location_request);
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

        /* NULL out shared pointers, then free message before response */
        {
            int i;
            for (i = 0; i < message.num_of_part; i++) {
                message.part[i].content_id = NULL;
                message.part[i].content_type = NULL;
            }
        }
        /* N1N2MessageTransferRspData is OWNED by message, let ogs_sbi_message_free() free it */
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        lmf_location_request_remove(location_request);
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
        
        /* Clean up: NULL out shared pointers, then free message before response */
        {
            int i;
            for (i = 0; i < message.num_of_part; i++) {
                message.part[i].content_id = NULL;
                message.part[i].content_type = NULL;
            }
        }
        /* N1N2MessageTransferRspData is OWNED by message, let ogs_sbi_message_free() free it */
        
        /* Free message first (frees its owned pkbufs and OpenAPI structures), then response (frees shared strings) */
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
    } else if (nrppa_pdu.message_type == NRPPA_MESSAGE_TYPE_ECID_MEASUREMENT_FAILURE_INDICATION) {
        ogs_warn("[%s] gNB rejected NRPPa measurement request (cause=%u)",
                location_request->supi, nrppa_pdu.u.ecid_failure.cause);

        /* Check if we should fallback to CELLID */
        /* Fallback is enabled for explicit ECID requests or when UE is not reachable */
        bool should_fallback = location_request->ecid_fallback_to_cellid;
        
        /* Check failure cause - fallback for UE not reachable, resource unavailable, etc. */
        uint8_t cause = nrppa_pdu.u.ecid_failure.cause;
        /* Common causes that warrant fallback: 
         * 0 = undefined
         * 1 = radio network - temporary network problem
         * 2 = radio network - UE not reachable
         * 3 = radio network - resource unavailable
         */
        if (cause == 2 || cause == 3) {
            should_fallback = true;
            ogs_info("[%s] ECID failed due to UE not reachable or resource unavailable (cause=%u), attempting CELLID fallback",
                    location_request->supi, cause);
        }

        if (should_fallback) {
            ogs_info("[%s] ECID failed, falling back to CELLID positioning",
                    location_request->supi);
            
            /* Switch to CELLID positioning method */
            if (location_request->positioning_method) {
                ogs_free(location_request->positioning_method);
            }
            location_request->positioning_method = ogs_strdup("CELLID");
            
            /* Free response message - we'll start fresh with CELLID */
            /* NULL out shared pointers, then free message before response */
            {
                int i;
                for (i = 0; i < message.num_of_part; i++) {
                    message.part[i].content_id = NULL;
                    message.part[i].content_type = NULL;
                }
            }
            /* N1N2MessageTransferRspData is OWNED by message, let ogs_sbi_message_free() free it */
            ogs_sbi_message_free(&message);
            ogs_sbi_response_free(response);
            
            /* Start CELLID positioning by querying AMF for Cell ID */
            rv = lmf_amf_send_location_info_request(location_request);
            if (rv != OGS_OK) {
                ogs_error("[%s] CELLID fallback failed: unable to query AMF for Cell ID",
                        location_request->supi);
                
                ogs_sbi_stream_t *stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
                if (stream) {
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE, NULL,
                            "ECID failed and CELLID fallback also failed",
                            "Unable to determine location via ECID or CELLID", NULL));
                }
                lmf_location_request_remove(location_request);
            }
            /* Response will be handled asynchronously in lmf_namf_handler_location_info_response */
            return;
        }

        /* No fallback - send error response */
        ogs_sbi_stream_t *stream = NULL;
        char *error_detail = NULL;

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            error_detail = ogs_msprintf("gNB rejected NRPPa measurement request (cause=%u)",
                    nrppa_pdu.u.ecid_failure.cause);

            /* Try to send error response - stream might be closed already, so don't assert */
            bool sent = ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_SERVICE_UNAVAILABLE, NULL,
                    "NRPPa measurement request rejected by gNB",
                    error_detail, NULL);
            if (!sent) {
                ogs_warn("[%s] Failed to send error response (stream may be closed)",
                        location_request->supi ? location_request->supi : "Unknown");
            }

            if (error_detail)
                ogs_free(error_detail);
        } else {
            ogs_warn("[%s] Stream not found for error response (may already be closed)",
                    location_request->supi ? location_request->supi : "Unknown");
        }

        /* Clean up: NULL out shared pointers, then free message before response
         * message.part[] contains:
         *   - pkbuf: OWNED by message (independent copy), will be freed by ogs_sbi_message_free()
         *   - content_id/content_type: SHARED with response, must be NULLed before freeing response
         * message->N1N2MessageTransferRspData is OWNED by message (allocated by parser), 
         *   let ogs_sbi_message_free() free it.
         * 
         * Order: NULL shared pointers -> free message (frees its pkbufs and OpenAPI structures) -> free response (frees shared strings)
         */
        {
            int i;
            for (i = 0; i < message.num_of_part; i++) {
                /* NULL out shared pointers - don't free pkbuf, let ogs_sbi_message_free() do it */
                message.part[i].content_id = NULL;
                message.part[i].content_type = NULL;
            }
        }
        /* N1N2MessageTransferRspData is OWNED by message, let ogs_sbi_message_free() free it */
        
        /* Free message first (frees its owned pkbufs and OpenAPI structures), then response (frees shared strings) */
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        
        /* Remove location request last - this may trigger async operations */
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

        /* Clean up: NULL out shared pointers, then free message before response */
        {
            int i;
            for (i = 0; i < message.num_of_part; i++) {
                message.part[i].content_id = NULL;
                message.part[i].content_type = NULL;
            }
        }
        /* N1N2MessageTransferRspData is OWNED by message, let ogs_sbi_message_free() free it */
        
        /* Free message first (frees its owned pkbufs and OpenAPI structures), then response (frees shared strings) */
        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        
        lmf_location_request_remove(location_request);
    }
}

void lmf_namf_handler_location_info_response(
        int status, ogs_sbi_response_t *response, void *data)
{
    int rv;
    lmf_location_request_t *location_request = NULL;
    cJSON *json = NULL;
    cJSON *ncgi_json = NULL;
    cJSON *plmn_json = NULL;
    cJSON *item = NULL;
    ogs_sbi_stream_t *stream = NULL;

    ogs_assert(response);
    ogs_assert(data);

    location_request = data;

    if (status != OGS_OK) {
        ogs_error("[%s] AMF location info request failed [%d]",
                location_request->supi ? location_request->supi : "Unknown", status);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            int http_status = OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR;
            char *error_detail = NULL;

            if (response && response->status >= 400) {
                http_status = response->status;
                error_detail = ogs_msprintf("AMF returned HTTP %d for location info request", response->status);
            } else {
                error_detail = ogs_strdup("AMF location info request failed");
            }

            ogs_assert(true ==
                ogs_sbi_server_send_error(stream, http_status, NULL,
                    "Failed to get Cell ID from AMF", error_detail, NULL));

            if (error_detail)
                ogs_free(error_detail);
        }

        ogs_sbi_response_free(response);
        lmf_location_request_remove(location_request);
        return;
    }

    ogs_info("[%s] Received location info response from AMF",
            location_request->supi ? location_request->supi : "Unknown");

    /* Check response status */
    if (response->status != OGS_SBI_HTTP_STATUS_OK) {
        ogs_error("[%s] AMF returned HTTP %d for location info request",
                location_request->supi, response->status);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream, response->status, NULL,
                    "AMF location info request failed",
                    "AMF returned error status", NULL));
        }

        ogs_sbi_response_free(response);
        lmf_location_request_remove(location_request);
        return;
    }

    /* Parse JSON response */
    if (!response->http.content) {
        ogs_error("[%s] No content in AMF location info response",
                location_request->supi);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "Invalid AMF response",
                    "AMF location info response has no content", NULL));
        }

        ogs_sbi_response_free(response);
        lmf_location_request_remove(location_request);
        return;
    }

    json = cJSON_Parse(response->http.content);
    if (!json) {
        ogs_error("[%s] Failed to parse AMF location info JSON",
                location_request->supi);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "Failed to parse AMF response",
                    "AMF location info response is not valid JSON", NULL));
        }

        ogs_sbi_response_free(response);
        lmf_location_request_remove(location_request);
        return;
    }

    /* Extract NCGI from JSON */
    ncgi_json = cJSON_GetObjectItem(json, "ncgi");
    if (!ncgi_json || !cJSON_IsObject(ncgi_json)) {
        ogs_error("[%s] No 'ncgi' object in AMF location info response",
                location_request->supi);

        cJSON_Delete(json);
        ogs_sbi_response_free(response);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "Invalid AMF response",
                    "AMF location info response missing 'ncgi' field", NULL));
        }

        lmf_location_request_remove(location_request);
        return;
    }

    /* Parse NCGI */
    location_request->ncgi_from_amf = ogs_calloc(1, sizeof(OpenAPI_ncgi_t));
    ogs_assert(location_request->ncgi_from_amf);

    /* Parse PLMN ID */
    plmn_json = cJSON_GetObjectItem(ncgi_json, "plmnId");
    if (plmn_json && cJSON_IsObject(plmn_json)) {
        OpenAPI_plmn_id_t *plmn_id = ogs_calloc(1, sizeof(OpenAPI_plmn_id_t));
        ogs_assert(plmn_id);

        item = cJSON_GetObjectItem(plmn_json, "mcc");
        if (item && cJSON_IsString(item)) {
            plmn_id->mcc = ogs_strdup(cJSON_GetStringValue(item));
        }

        item = cJSON_GetObjectItem(plmn_json, "mnc");
        if (item && cJSON_IsString(item)) {
            plmn_id->mnc = ogs_strdup(cJSON_GetStringValue(item));
        }

        location_request->ncgi_from_amf->plmn_id = plmn_id;
    }

    /* Parse Cell ID */
    item = cJSON_GetObjectItem(ncgi_json, "nrCellId");
    if (item && cJSON_IsString(item)) {
        location_request->ncgi_from_amf->nr_cell_id = ogs_strdup(cJSON_GetStringValue(item));
    }

    if (!location_request->ncgi_from_amf->nr_cell_id) {
        ogs_error("[%s] No 'nrCellId' in AMF location info response",
                location_request->supi);

        if (location_request->ncgi_from_amf->plmn_id) {
            if (location_request->ncgi_from_amf->plmn_id->mcc)
                ogs_free(location_request->ncgi_from_amf->plmn_id->mcc);
            if (location_request->ncgi_from_amf->plmn_id->mnc)
                ogs_free(location_request->ncgi_from_amf->plmn_id->mnc);
            ogs_free(location_request->ncgi_from_amf->plmn_id);
        }
        ogs_free(location_request->ncgi_from_amf);
        location_request->ncgi_from_amf = NULL;

        cJSON_Delete(json);
        ogs_sbi_response_free(response);

        stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
        if (stream) {
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR, NULL,
                    "Invalid AMF response",
                    "AMF location info response missing 'nrCellId' field", NULL));
        }

        lmf_location_request_remove(location_request);
        return;
    }

    ogs_info("[%s] Retrieved Cell ID from AMF: PLMN=%s/%s, CellID=%s",
            location_request->supi,
            location_request->ncgi_from_amf->plmn_id ?
                (location_request->ncgi_from_amf->plmn_id->mcc ?
                    location_request->ncgi_from_amf->plmn_id->mcc : "N/A") : "N/A",
            location_request->ncgi_from_amf->plmn_id ?
                (location_request->ncgi_from_amf->plmn_id->mnc ?
                    location_request->ncgi_from_amf->plmn_id->mnc : "N/A") : "N/A",
            location_request->ncgi_from_amf->nr_cell_id);

    cJSON_Delete(json);
    ogs_sbi_response_free(response);

    /* Now continue with CELLID positioning using the retrieved NCGI */
    rv = lmf_location_determine_cellid(location_request);
    if (rv != OGS_OK) {
        ogs_error("[%s] lmf_location_determine_cellid() failed after getting NCGI from AMF",
                location_request->supi);
        /* Error response already sent by lmf_location_determine_cellid */
    }
}

