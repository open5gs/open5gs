/*
 * Copyright (C) 2019-2024 by Sukchan Lee <acetcom@gmail.com>
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

#include "npcf-handler.h"
#include "nudr-handler.h"
#include "nbsf-handler.h"

void pcf_sm_state_initial(ogs_fsm_t *s, pcf_event_t *e)
{
    ogs_assert(s);

    OGS_FSM_TRAN(s, &pcf_sm_state_operational);
}

void pcf_sm_state_final(ogs_fsm_t *s, pcf_event_t *e)
{
}

void pcf_sm_state_operational(ogs_fsm_t *s, pcf_event_t *e)
{
    bool handled;
    pcf_ue_t *pcf_ue = NULL;
    pcf_sess_t *sess = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id;
    ogs_sbi_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    pcf_sm_debug(e);

    sess = pcf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);
    pcf_ue = pcf_ue_find_by_id(sess->pcf_ue_id);
    ogs_assert(pcf_ue);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case OGS_EVENT_SBI_SERVER:
        message = e->h.sbi.message;
        ogs_assert(message);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)
            if (!message->h.resource.component[1]) {
                handled = pcf_npcf_smpolicycontrol_handle_create(
                        sess, stream, message);
                if (!handled) {
                    ogs_error("[%s:%d] "
                            "pcf_npcf_smpolicycontrol_handle_create() failed",
                            pcf_ue->supi, sess->psi);
                    OGS_FSM_TRAN(s, pcf_sm_state_exception);
                }
            } else {
                SWITCH(message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_DELETE)
                    handled = pcf_npcf_smpolicycontrol_handle_delete(
                            sess, stream, message);
                    if (!handled) {
                        ogs_error("[%s:%d] "
                            "pcf_npcf_smpolicycontrol_handle_delete() failed",
                            pcf_ue->supi, sess->psi);
                        OGS_FSM_TRAN(s, pcf_sm_state_exception);
                    }
                    break;

                DEFAULT
                    ogs_error("[%s:%d] Invalid HTTP URI [%s]",
                            pcf_ue->supi, sess->psi, message->h.uri);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, message,
                            "Invalid HTTP method", message->h.uri, NULL));
                END
            }
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_POLICYAUTHORIZATION)
            if (message->h.resource.component[1]) {
                if (message->h.resource.component[2]) {
                    SWITCH(message->h.resource.component[2])
                    CASE(OGS_SBI_RESOURCE_NAME_DELETE)
                        handled = pcf_npcf_policyauthorization_handle_delete(
                                sess, e->app, stream, message);
                        break;
                    DEFAULT
                        ogs_error("[%s:%d] Invalid resource name [%s]",
                                pcf_ue->supi, sess->psi,
                                message->h.resource.component[2]);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                                "Invalid resource name", message->h.uri, NULL));
                    END
                } else {
                    SWITCH(message->h.method)
                    CASE(OGS_SBI_HTTP_METHOD_PATCH)
                        handled = pcf_npcf_policyauthorization_handle_update(
                                sess, e->app, stream, message);
                        break;
                    DEFAULT
                        ogs_error("[%s:%d] Unknown method [%s]",
                                pcf_ue->supi, sess->psi, message->h.method);
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, message,
                                "Invalid HTTP method", message->h.uri, NULL));
                    END
                }
            } else {
                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_POST)
                    handled = pcf_npcf_policyauthorization_handle_create(
                            sess, stream, message);
                    break;
                DEFAULT
                    ogs_error("[%s:%d] Unknown method [%s]",
                            pcf_ue->supi, sess->psi, message->h.method);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, message,
                            "Invalid HTTP method", message->h.uri, NULL));
                END
            }
            break;

        DEFAULT
            ogs_error("[%s:%d] Invalid API name [%s]",
                        pcf_ue->supi, sess->psi, message->h.service.name);
            ogs_assert_if_reached();
        END
        break;

    case OGS_EVENT_SBI_CLIENT:
        message = e->h.sbi.message;
        ogs_assert(message);

        stream_id = OGS_POINTER_TO_UINT(e->h.sbi.data);
        ogs_assert(stream_id >= OGS_MIN_POOL_ID &&
                stream_id <= OGS_MAX_POOL_ID);

        stream = ogs_sbi_stream_find_by_id(stream_id);
        if (!stream) {
            ogs_error("STREAM has already been removed [%d]", stream_id);
            break;
        }

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NUDR_DR)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_POLICY_DATA)
                SWITCH(message->h.resource.component[1])
                CASE(OGS_SBI_RESOURCE_NAME_UES)
                    if (message->res_status != OGS_SBI_HTTP_STATUS_OK &&
                        message->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        if (message->res_status ==
                                OGS_SBI_HTTP_STATUS_NOT_FOUND) {
                            ogs_warn("[%s:%d] Cannot find SUPI [%d]",
                                pcf_ue->supi, sess->psi, message->res_status);
                            /*
                             * TS29.512
                             * 4.2.2.2 SM Policy Association establishment
                             *
                             * If the user information received within the "supi"
                             * attribute is unknown, the PCF shall reject the
                             * request with an HTTP "400 Bad Request" response
                             * message including the "cause" attribute of the
                             * ProblemDetails data structure set to "USER_UNKNOWN".
                             */
                            ogs_assert(true ==
                                ogs_sbi_server_send_error(
                                    stream, OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                                    NULL, "End user is unknown to the PCF",
                                    pcf_ue->supi, "USER_UNKNOWN"));
                        } else {
                            ogs_error("[%s:%d] HTTP response error [%d]",
                                pcf_ue->supi, sess->psi, message->res_status);
                            /*
                             * TS29.512
                             * 4.2.2.2 SM Policy Association establishment
                             *
                             * If the PCF, based on local configuration and/or
                             * operator policies, denies the creation of the
                             * Individual SM Policy resource, the PCF may reject
                             * the request and include in an HTTP "403 Forbidden"
                             * response message the "cause" attribute of the
                             * ProblemDetails data structure set to
                             * "POLICY_CONTEXT_DENIED".
                             */
                            ogs_assert(true ==
                                ogs_sbi_server_send_error(
                                    stream, OGS_SBI_HTTP_STATUS_FORBIDDEN,
                                    NULL, "HTTP response error",
                                    pcf_ue->supi, "POLICY_CONTEXT_DENIED"));
                        }
                        break;
                    }

                    pcf_nudr_dr_handle_query_sm_data(sess, stream, message);
                    break;

                DEFAULT
                    ogs_error("[%s:%d] Invalid resource name [%s]",
                            pcf_ue->supi, sess->psi,
                            message->h.resource.component[1]);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("[%s:%d] Invalid resource name [%s]",
                        pcf_ue->supi, sess->psi,
                        message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NBSF_MANAGEMENT)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_PCF_BINDINGS)
                if (message->h.resource.component[1]) {
                    SWITCH(message->h.method)
                    CASE(OGS_SBI_HTTP_METHOD_DELETE)
                        if (message->res_status !=
                                OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                            ogs_warn("[%s:%d] HTTP response error [%d]",
                                pcf_ue->supi, sess->psi, message->res_status);

/*
 * The PCfBindings resource for that UE may not exist in the BSF
 * for reasons such as restarting the BSF.
 *
 * So, session Release continues even if there is no resource in BSF.
 */
#if 0
                            ogs_assert(true ==
                                ogs_sbi_server_send_error(stream,
                                    message->res_status,
                                    NULL, "HTTP response error", pcf_ue->supi));
                            OGS_FSM_TRAN(s, pcf_sm_state_exception);
                            break;
#endif
                        }

                        pcf_nbsf_management_handle_de_register(
                                sess, stream, message);
                        OGS_FSM_TRAN(s, pcf_sm_state_deleted);
                        break;
                    DEFAULT
                        ogs_error("[%s:%d] Unknown method [%s]",
                                pcf_ue->supi, sess->psi, message->h.method);
                        ogs_assert_if_reached();
                    END
                    break;
                } else {
                    SWITCH(message->h.method)
                    CASE(OGS_SBI_HTTP_METHOD_POST)
                        if (message->res_status ==
                                OGS_SBI_HTTP_STATUS_CREATED) {
                            pcf_nbsf_management_handle_register(
                                    sess, stream, message);
                        } else {
                            ogs_error("[%s:%d] HTTP response error [%d]",
                                pcf_ue->supi, sess->psi, message->res_status);

                            /*
                             * Send Response
                             * for SM Policy Association establishment
                             */
                            ogs_expect(true ==
                                pcf_sbi_send_smpolicycontrol_create_response(
                                    sess, stream));
                        }
                        break;
                    DEFAULT
                        ogs_error("[%s:%d] Unknown method [%s]",
                                pcf_ue->supi, sess->psi, message->h.method);
                        ogs_assert_if_reached();
                    END
                }
                break;

            DEFAULT
                ogs_error("[%s:%d] Invalid resource name [%s]",
                        pcf_ue->supi, sess->psi,
                        message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("[%s:%d] Invalid API name [%s]",
                        pcf_ue->supi, sess->psi, message->h.service.name);
            ogs_assert_if_reached();
        END
        break;

    default:
        ogs_error("[%s:%d] Unknown event %s",
                pcf_ue->supi, sess->psi, pcf_event_get_name(e));
        break;
    }
}

void pcf_sm_state_deleted(ogs_fsm_t *s, pcf_event_t *e)
{
    pcf_ue_t *pcf_ue = NULL;
    pcf_sess_t *sess = NULL;

    ogs_assert(s);
    ogs_assert(e);

    pcf_sm_debug(e);

    sess = pcf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);
    pcf_ue = pcf_ue_find_by_id(sess->pcf_ue_id);
    ogs_assert(pcf_ue);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        pcf_metrics_inst_by_slice_add(&pcf_ue->guami.plmn_id,
                &sess->s_nssai, PCF_METR_GAUGE_PA_SESSIONNBR, -1);
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("[%s:%d] Unknown event %s",
                pcf_ue->supi, sess->psi, pcf_event_get_name(e));
        break;
    }
}

void pcf_sm_state_exception(ogs_fsm_t *s, pcf_event_t *e)
{
    pcf_ue_t *pcf_ue = NULL;
    pcf_sess_t *sess = NULL;

    ogs_assert(s);
    ogs_assert(e);

    pcf_sm_debug(e);

    sess = pcf_sess_find_by_id(e->sess_id);
    ogs_assert(sess);
    pcf_ue = pcf_ue_find_by_id(sess->pcf_ue_id);
    ogs_assert(pcf_ue);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        pcf_metrics_inst_by_slice_add(&pcf_ue->guami.plmn_id,
                &sess->s_nssai, PCF_METR_GAUGE_PA_SESSIONNBR, -1);
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("[%s:%d] Unknown event %s",
                pcf_ue->supi, sess->psi, pcf_event_get_name(e));
        break;
    }
}
