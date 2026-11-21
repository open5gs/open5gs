/*
 * Copyright (C) 2026 by Rami Mohamed <ramrode@gmail.com>
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
#include "nnrf-handler.h"
#include "nchf-handler.h"
#include "metrics.h"

/*
 * Request-plane metrics for one Nchf operation.  Call begin() immediately
 * before the handler and record() immediately after, so the in-flight gauge
 * is incremented and decremented in one visible pair.
 */
static int64_t chf_metrics_begin_request(void)
{
    chf_metrics_inst_global_inc(CHF_METR_GAUGE_SBI_INFLIGHT);
    return ogs_get_monotonic_time();
}

/*
 * Decrement the in-flight gauge, observe the handling latency (ms), and count
 * the request by operation + coarse result.  `t0` comes from begin_request().
 */
static void chf_metrics_record_request(const char *op, bool ok, int64_t t0)
{
    int ms = (int)((ogs_get_monotonic_time() - t0) / 1000);
    chf_metrics_inst_global_dec(CHF_METR_GAUGE_SBI_INFLIGHT);
    chf_metrics_inst_labeled_add(CHF_METR_LBL_SBI_REQ_DURATION,
            op, NULL, NULL, ms);
    chf_metrics_inst_labeled_inc(CHF_METR_LBL_SBI_REQUESTS,
            op, ok ? "success" : "failure", NULL);
}

void chf_state_initial(ogs_fsm_t *s, chf_event_t *e)
{
    chf_sm_debug(e);

    ogs_assert(s);

    OGS_FSM_TRAN(s, &chf_state_operational);
}

void chf_state_final(ogs_fsm_t *s, chf_event_t *e)
{
    chf_sm_debug(e);
}

void chf_state_operational(ogs_fsm_t *s, chf_event_t *e)
{
    int rv;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_request_t *request = NULL;

    ogs_sbi_nf_instance_t *nf_instance = NULL;
    ogs_sbi_subscription_data_t *subscription_data = NULL;
    ogs_sbi_response_t *response = NULL;
    ogs_sbi_message_t message;

    ogs_sbi_xact_t *sbi_xact = NULL;
    ogs_pool_id_t sbi_xact_id = OGS_INVALID_POOL_ID;

    chf_sess_t *sess = NULL;

    int64_t t0 = 0;   /* Nchf handler timing, see chf_metrics_begin_request */
    bool ok = false;

    chf_sm_debug(e);

    ogs_assert(s);

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
            /*
             * parse_content() in lib/sbi/message.c returns OGS_OK
             * immediately for nchf-convergedcharging (early return before
             * cJSON_Parse, since the service parses the body in its own
             * handler).  Therefore rv != OGS_OK cannot occur for our own
             * service name under normal operation — not even for malformed
             * request bodies.
             *
             * If rv != OGS_OK does somehow fire (e.g., parse_header() itself
             * failed, or an unexpected code path was reached), reject the
             * request with 400 for unrecognised services, or let our handlers
             * read request->http.content directly for our own services.
             * The handler will call cJSON_Parse itself and return 400 to the
             * client if the body is invalid.
             */
            if (!message.h.service.name ||
                strcmp(message.h.service.name,
                       OGS_SBI_SERVICE_NAME_NCHF_CONVERGEDCHARGING) != 0) {
                ogs_error("cannot parse HTTP message");
                ogs_assert(true ==
                    ogs_sbi_server_send_error(
                        stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        NULL, "cannot parse HTTP message", NULL, NULL));
                ogs_sbi_message_free(&message);
                break;
            }
            ogs_debug("parse_content() returned error for [%s]"
                      " (unexpected; handler will read request->http.content"
                      " directly and return 400 if body is invalid)",
                      message.h.service.name);
        }

        if (strcmp(message.h.api.version, CHF_API_VERSION) != 0 &&
            strcmp(message.h.api.version, OGS_SBI_API_V1) != 0) {
            ogs_error("Not supported version [%s]", message.h.api.version);
            ogs_assert(true ==
                ogs_sbi_server_send_error(
                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                    &message, "Not supported version", NULL, NULL));
            ogs_sbi_message_free(&message);
            break;
        }

        SWITCH(message.h.service.name)
        CASE("nnrf-nfm")

            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_STATUS_NOTIFY)
                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    ogs_nnrf_nfm_handle_nf_status_notify(stream, &message);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message.h.method);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_FORBIDDEN, &message,
                            "Invalid HTTP method", message.h.method, NULL));
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &message,
                        "Unknown resource name",
                        message.h.resource.component[0], NULL));
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NCHF_CONVERGEDCHARGING)

            SWITCH(message.h.resource.component[0])
            CASE(CHF_RESOURCE_NAME_CHARGINGDATA)

                if (!message.h.resource.component[1]) {
                    /* POST /chargingdata → Create */
                    SWITCH(message.h.method)
                    CASE(OGS_SBI_HTTP_METHOD_POST)
                        t0 = chf_metrics_begin_request();
                        ok = chf_nchf_convergedcharging_handle_create(
                                stream, request, &message);
                        chf_metrics_record_request("create", ok, t0);
                        break;

                    DEFAULT
                        ogs_error("Invalid HTTP method [%s]",
                                message.h.method);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED,
                                &message, "Invalid HTTP method",
                                message.h.method, NULL));
                    END
                } else {
                    /* /chargingdata/{ChargingDataRef}/update|release */
                    sess = chf_sess_find_by_charging_data_ref(
                            message.h.resource.component[1]);

                    if (!sess) {
                        ogs_error("Session not found [%s]",
                                message.h.resource.component[1]);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_NOT_FOUND,
                                &message, "Session not found",
                                message.h.resource.component[1], NULL));
                        break;
                    }

                    SWITCH(message.h.resource.component[2])
                    CASE(CHF_RESOURCE_NAME_UPDATE)
                        t0 = chf_metrics_begin_request();
                        ok = chf_nchf_convergedcharging_handle_update(
                                sess, stream, request, &message);
                        chf_metrics_record_request("update", ok, t0);
                        break;

                    CASE(CHF_RESOURCE_NAME_RELEASE)
                        t0 = chf_metrics_begin_request();
                        ok = chf_nchf_convergedcharging_handle_release(
                                sess, stream, request, &message);
                        chf_metrics_record_request("release", ok, t0);
                        break;

                    DEFAULT
                        ogs_error("Invalid resource [%s]",
                                message.h.resource.component[2] ?
                                message.h.resource.component[2] : "NULL");
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_NOT_FOUND,
                                &message, "Invalid resource",
                                message.h.resource.component[2], NULL));
                    END
                }
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, &message,
                        "Unknown resource name",
                        message.h.resource.component[0], NULL));
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", message.h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, &message,
                    "Invalid API name", message.h.service.name, NULL));
        END

        ogs_sbi_message_free(&message);
        break;

    case OGS_EVENT_SBI_CLIENT:
        ogs_assert(e);

        response = e->h.sbi.response;
        ogs_assert(response);
        rv = ogs_sbi_parse_response(&message, response);
        if (rv != OGS_OK) {
            ogs_error("cannot parse HTTP response");
            ogs_sbi_message_free(&message);
            ogs_sbi_response_free(response);
            break;
        }

        SWITCH(message.h.service.name)
        CASE("nnrf-nfm")

            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                nf_instance = e->h.sbi.data;
                ogs_assert(nf_instance);

                if (OGS_FSM_STATE(&nf_instance->sm)) {
                    e->h.sbi.message = &message;
                    ogs_fsm_dispatch(&nf_instance->sm, e);
                } else
                    ogs_error("NF instance FSM has been finalized");

                break;

            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTIONS)
                subscription_data = e->h.sbi.data;
                ogs_assert(subscription_data);

                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_CREATED ||
                        message.res_status == OGS_SBI_HTTP_STATUS_OK) {
                        ogs_nnrf_nfm_handle_nf_status_subscribe(
                                subscription_data, &message);
                    } else {
                        ogs_error("HTTP response error : %d",
                                message.res_status);
                    }
                    break;

                CASE(OGS_SBI_HTTP_METHOD_PATCH)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_OK ||
                        message.res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        ogs_nnrf_nfm_handle_nf_status_update(
                                subscription_data, &message);
                    } else {
                        ogs_error("[%s] HTTP response error [%d]",
                                subscription_data->id ?
                                    subscription_data->id : "Unknown",
                                message.res_status);
                    }
                    break;

                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_NO_CONTENT)
                        ogs_info("[%s] Subscription deleted",
                                subscription_data->id ?
                                    subscription_data->id : "Unknown");
                    else
                        ogs_error("[%s] HTTP response error [%d]",
                                subscription_data->id ?
                                    subscription_data->id : "Unknown",
                                message.res_status);

                    ogs_sbi_subscription_data_remove(subscription_data);
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid HTTP method [%s]",
                            subscription_data->id, message.h.method);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE("nnrf-disc")
            SWITCH(message.h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_NF_INSTANCES)
                sbi_xact_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
                ogs_assert(sbi_xact_id >= OGS_MIN_POOL_ID &&
                        sbi_xact_id <= OGS_MAX_POOL_ID);

                sbi_xact = ogs_sbi_xact_find_by_id(sbi_xact_id);
                if (!sbi_xact) {
                    ogs_error("SBI transaction has already been removed [%d]",
                            sbi_xact_id);
                    break;
                }

                SWITCH(message.h.method)
                CASE(OGS_SBI_HTTP_METHOD_GET)
                    if (message.res_status == OGS_SBI_HTTP_STATUS_OK)
                        chf_nnrf_handle_nf_discover(sbi_xact, &message);
                    else
                        ogs_error("HTTP response error [%d]",
                                message.res_status);
                    break;

                DEFAULT
                    ogs_error("Invalid HTTP method [%s]", message.h.method);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message.h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", message.h.service.name);
            ogs_assert_if_reached();
        END

        ogs_sbi_message_free(&message);
        ogs_sbi_response_free(response);
        break;

    case OGS_EVENT_SBI_TIMER:
        ogs_assert(e);

        switch(e->h.timer_id) {
        case OGS_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL:
        case OGS_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL:
        case OGS_TIMER_NF_INSTANCE_NO_HEARTBEAT:
        case OGS_TIMER_NF_INSTANCE_VALIDITY:
            /*
             * An SBI timer carries a pool id, not a pointer.  Upstream
             * changed this in 310fea94e ("avoid stale pointers in NF
             * timers and subscription responses") so that a timer firing
             * after its NF instance was removed cannot dereference freed
             * memory; the instance is looked up and may legitimately be
             * gone.  Treating the id as a pointer segfaults immediately,
             * which is what it did here until the replant onto that
             * commit caught it.
             *
             * The FSM handler still expects a pointer, so put one back
             * before dispatching.
             */
            nf_instance = ogs_sbi_nf_instance_find_by_pool_id(
                    OGS_POINTER_TO_UINT(e->h.sbi.data));
            if (!nf_instance) {
                ogs_error("NF Instance has already been removed [%d]",
                        (int)OGS_POINTER_TO_UINT(e->h.sbi.data));
                break;
            }
            ogs_assert(OGS_FSM_STATE(&nf_instance->sm));
            e->h.sbi.data = nf_instance;

            ogs_fsm_dispatch(&nf_instance->sm, e);
            if (OGS_FSM_CHECK(&nf_instance->sm, ogs_sbi_nf_state_exception))
                ogs_error("[%s:%s] State machine exception [%d]",
                        OpenAPI_nf_type_ToString(nf_instance->nf_type),
                        nf_instance->id, e->h.timer_id);
            break;

        case OGS_TIMER_SUBSCRIPTION_VALIDITY:
            /* A pool id, as above (310fea94e). */
            subscription_data = ogs_sbi_subscription_data_find_by_pool_id(
                    OGS_POINTER_TO_UINT(e->h.sbi.data));
            if (!subscription_data) {
                ogs_error("Subscription has already been removed [%d]",
                        (int)OGS_POINTER_TO_UINT(e->h.sbi.data));
                break;
            }

            ogs_assert(true ==
                ogs_nnrf_nfm_send_nf_status_subscribe(
                    ogs_sbi_self()->nf_instance->nf_type,
                    subscription_data->req_nf_instance_id,
                    subscription_data->subscr_cond.nf_type,
                    subscription_data->subscr_cond.service_name));

            ogs_error("[%s] Subscription validity expired",
                subscription_data->id);
            ogs_sbi_subscription_data_remove(subscription_data);
            break;

        case OGS_TIMER_SUBSCRIPTION_PATCH:
            /* A pool id, as above (310fea94e). */
            subscription_data = ogs_sbi_subscription_data_find_by_pool_id(
                    OGS_POINTER_TO_UINT(e->h.sbi.data));
            if (!subscription_data) {
                ogs_error("Subscription has already been removed [%d]",
                        (int)OGS_POINTER_TO_UINT(e->h.sbi.data));
                break;
            }

            ogs_assert(true ==
                ogs_nnrf_nfm_send_nf_status_update(subscription_data));

            ogs_info("[%s] Need to update Subscription",
                    subscription_data->id);
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

            ogs_sbi_xact_remove(sbi_xact);

            ogs_error("Cannot receive SBI message");
            break;

        default:
            ogs_error("Unknown timer[%s:%d]",
                    ogs_timer_get_name(e->h.timer_id), e->h.timer_id);
        }
        break;

    default:
        ogs_error("No handler for event %s", chf_event_get_name(e));
        break;
    }
}
