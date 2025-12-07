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

#include "sbi-path.h"
#include "context.h"
#include "event.h"
#include "nlmf-handler.h"
#include "namf-handler.h"

void lmf_state_initial(ogs_fsm_t *s, lmf_event_t *e)
{
    lmf_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &lmf_state_operational);
}

void lmf_state_final(ogs_fsm_t *s, lmf_event_t *e)
{
    lmf_sm_debug(e);

    ogs_assert(s);
}

void lmf_state_operational(ogs_fsm_t *s, lmf_event_t *e)
{
    int rv;
    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_request_t *request = NULL;
    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_xact_t *sbi_xact = NULL;
    ogs_pool_id_t sbi_xact_id = OGS_INVALID_POOL_ID;
    ogs_pool_id_t location_request_id = OGS_INVALID_POOL_ID;
    lmf_location_request_t *location_request = NULL;

    ogs_sbi_message_t message;

    ogs_assert(e);
    ogs_assert(s);

    lmf_sm_debug(e);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case OGS_EVENT_SBI_SERVER:
        request = e->h.sbi.request;
        ogs_assert(request);
        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        rv = ogs_sbi_parse_request(&message, request);
        if (rv != OGS_OK) {
            ogs_error("cannot parse HTTP message");
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    NULL, "cannot parse HTTP message", NULL, NULL));
            break;
        }

        if (strcmp(message.h.api.version, OGS_SBI_API_V1) != 0) {
            ogs_error("Not supported version [%s]", message.h.api.version);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    &message, "Not supported version", NULL, NULL));
            ogs_sbi_message_free(&message);
            break;
        }

        SWITCH(message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NLMF_LOC)
            SWITCH(message.h.resource.component[0])
            CASE("determine-location")
                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    rv = lmf_nlmf_handle_determine_location(stream, &message);
                    if (rv != OGS_OK) {
                        ogs_error("lmf_nlmf_handle_determine_location() failed");
                    }
                    /* Always free the message after handling - it contains allocated OpenAPI objects */
                    ogs_sbi_message_free(&message);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message.h.method);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_FORBIDDEN, &message,
                            "Invalid HTTP method", message.h.method, NULL));
                    ogs_sbi_message_free(&message);
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &message,
                        "Invalid resource name",
                        message.h.resource.component[0], NULL));
                ogs_sbi_message_free(&message);
            END
            break;

        DEFAULT
            ogs_error("Unknown service name [%s]", message.h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, &message,
                    "Unknown service name", message.h.service.name, NULL));
            ogs_sbi_message_free(&message);
        END
        break;

    case OGS_EVENT_SBI_CLIENT:
        ogs_assert(e->h.sbi.response);
        ogs_assert(e->h.sbi.data);

        /* Try to identify service type from transaction ID first */
        /* For transaction-based responses, e->h.sbi.data is the transaction ID */
        /* For NF instance responses (nnrf-nfm), e->h.sbi.data is the nf_instance pointer */
        sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        if (sbi_xact_id >= OGS_MIN_POOL_ID && sbi_xact_id <= OGS_MAX_POOL_ID) {
            sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
            if (sbi_xact) {
                /* This is a transaction-based response */
                if (sbi_xact->service_type == OGS_SBI_SERVICE_TYPE_NAMF_COMM) {
                    /* Handle AMF Communication Service response */
                    location_request_id = sbi_xact->sbi_object_id;

                    if (location_request_id > 0) {
                        location_request =
                            lmf_location_request_try_find_by_id(location_request_id);
                    }
                    if (!location_request) {
                        ogs_error("Location request has already been removed [%d]",
                                location_request_id);
                        ogs_sbi_xact_remove(sbi_xact);
                        ogs_sbi_response_free(e->h.sbi.response);
                        break;
                    }

                    /* Remove transaction for all responses */
                    ogs_sbi_xact_remove(sbi_xact);

                    if (e->h.sbi.response->status >= 200 && e->h.sbi.response->status < 300) {
                        /* HTTP 200 OK - contains the actual NRPPa response */
                        lmf_namf_handler_nrppa_measurement_response(
                                OGS_OK, e->h.sbi.response, location_request);
                    } else {
                        ogs_error("[%s] AMF NRPPa request failed with HTTP status %d",
                                location_request->supi ? location_request->supi : "Unknown",
                                e->h.sbi.response->status);
                        lmf_namf_handler_nrppa_measurement_response(
                                OGS_ERROR, e->h.sbi.response, location_request);
                    }
                    /* Handler function will free response */
                    break;
                }
                /* For other transaction types (like NRF_DISC), let library handle it */
                /* Fall through to parse response */
            }
        }

        /* Parse response for NRF NFM and other services */
        rv = ogs_sbi_parse_response(&message, e->h.sbi.response);
        if (rv != OGS_OK) {
            ogs_error("cannot parse HTTP response");
            ogs_sbi_message_free(&message);
            ogs_sbi_response_free(e->h.sbi.response);
            break;
        }

        if (strcmp(message.h.api.version, OGS_SBI_API_V1) != 0) {
            ogs_error("Not supported version [%s]", message.h.api.version);
            ogs_sbi_message_free(&message);
            ogs_sbi_response_free(e->h.sbi.response);
            break;
        }

        SWITCH(message.h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NNRF_NFM)

            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                /* For NRF NFM responses, e->h.sbi.data is the nf_instance pointer */
                nf_instance = e->h.sbi.data;
                ogs_assert(nf_instance);

    /*
     * Guard against dispatching to an FSM that may have been finalized
     * by an asynchronous shutdown triggered by SIGTERM.
     *
     * In init.c's event_termination(), which can be invoked asynchronously
     * when the process receives SIGTERM, we iterate over all NF instances:
     *     ogs_list_for_each(&ogs_sbi_self()->nf_instance_list, nf_instance)
     *         ogs_sbi_nf_fsm_fini(nf_instance);
     * and call ogs_fsm_fini() on each instance's FSM. That finalizes the FSM
     * and its state is reset to zero.
     *
     * After event_termination(), any incoming SBI response—such as an NRF
     * client callback arriving after deregistration—would otherwise be
     * dispatched into a dead FSM and trigger an assertion failure.
     *
     * To avoid this, we check OGS_FSM_STATE(&nf_instance->sm):
     *   - If non-zero, the FSM is still active and can safely handle the event.
     *   - If zero, the FSM has already been finalized by event_termination(),
     *     so we log and drop the event to allow graceful shutdown.
     */
                if (OGS_FSM_STATE(&nf_instance->sm)) {
                    e->h.sbi.message = &message;
                    ogs_fsm_dispatch(&nf_instance->sm, e);
                    /* FSM processes message but doesn't free it - we must free both */
                    ogs_sbi_message_free(&message);
                    ogs_sbi_response_free(e->h.sbi.response);
                } else {
                    ogs_error("NF instance FSM has been finalized");
                    ogs_sbi_message_free(&message);
                    ogs_sbi_response_free(e->h.sbi.response);
                }

                break;

            DEFAULT
                ogs_error("Unknown NRF resource [%s]",
                        message.h.resource.component[0] ?
                            message.h.resource.component[0] : "Unknown");
                ogs_sbi_message_free(&message);
                ogs_sbi_response_free(e->h.sbi.response);
            END
            break;

        DEFAULT
            ogs_error("Unknown service name [%s]", message.h.service.name);
            ogs_sbi_message_free(&message);
            ogs_sbi_response_free(e->h.sbi.response);
        END
        break;

    case OGS_EVENT_SBI_TIMER:
        switch (e->h.timer_id) {
        case OGS_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        case OGS_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        case OGS_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        case OGS_TIMER_NF_INSTANCE_VALIDITY:
            nf_instance = e->h.sbi.data;
            ogs_assert(nf_instance);
            ogs_assert(OGS_FSM_STATE(&nf_instance->sm));

            /* Update load for self instance if this is a heartbeat for self */
            if (nf_instance == ogs_sbi_self()->nf_instance) {
                ogs_sbi_self()->nf_instance->load =
                    ogs_list_count(&lmf_self()->location_request_list);
            }

            ogs_fsm_dispatch(&nf_instance->sm, e);
            if (OGS_FSM_CHECK(&nf_instance->sm, ogs_sbi_nf_state_exception))
                ogs_error("[%s:%s] State machine exception [%d]",
                        OpenAPI_nf_type_ToString(nf_instance->nf_type),
                        nf_instance->id, e->h.timer_id);
            break;

        case OGS_TIMER_SBI_CLIENT_WAIT:
            sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
            ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                    sbi_xact_id <= OGS_MAX_POOL_ID);

            sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
            if (!sbi_xact) {
                ogs_error("SBI transaction has already been removed [%d]",
                        sbi_xact_id);
                break;
            }

            location_request_id = sbi_xact->sbi_object_id;
            ogs_sbi_xact_remove(sbi_xact);

            if (location_request_id > 0) {
                location_request =
                    lmf_location_request_try_find_by_id(location_request_id);
            }
            if (!location_request) {
                ogs_error("Location request has already been removed [%d]",
                        location_request_id);
                break;
            }

            stream = ogs_sbi_stream_find_by_id(location_request->stream_id);
            ogs_error("[%s] SBI request timed out while waiting for AMF",
                    location_request->supi ? location_request->supi : "Unknown");
            if (stream) {
                ogs_assert(true == ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_GATEWAY_TIMEOUT,
                        NULL, "AMF discovery timed out",
                        "Unable to reach AMF via NRF/SCP", NULL));
            } else {
                ogs_error("STREAM has already been removed [%d]",
                        location_request->stream_id);
            }

            location_request->xact = NULL;
            lmf_location_request_remove(location_request);
            break;

        default:
            ogs_error("Unknown timer event [%d]", e->h.timer_id);
            break;
        }
        break;

    default:
        ogs_error("Unknown event %s", lmf_event_get_name(e));
        break;
    }
}

