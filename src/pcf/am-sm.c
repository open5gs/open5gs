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
#include "nnrf-handler.h"

#include "npcf-handler.h"
#include "nudr-handler.h"

void pcf_am_state_initial(ogs_fsm_t *s, pcf_event_t *e)
{
    ogs_assert(s);

    OGS_FSM_TRAN(s, &pcf_am_state_operational);
}

void pcf_am_state_final(ogs_fsm_t *s, pcf_event_t *e)
{
}

void pcf_am_state_operational(ogs_fsm_t *s, pcf_event_t *e)
{
    bool handled;
    pcf_ue_am_t *pcf_ue_am = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id;
    ogs_sbi_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    pcf_sm_debug(e);

    pcf_ue_am = pcf_ue_am_find_by_id(e->pcf_ue_am_id);
    ogs_assert(pcf_ue_am);

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

        SWITCH(message->h.method)
        CASE(OGS_SBI_HTTP_METHOD_POST)
            handled = pcf_npcf_am_policy_control_handle_create(
                    pcf_ue_am, stream, message);
            if (!handled) {
                ogs_error("[%s] Cannot handle SBI message", pcf_ue_am->supi);
                OGS_FSM_TRAN(s, pcf_am_state_exception);
            }
            break;

        CASE(OGS_SBI_HTTP_METHOD_DELETE)
            ogs_assert(true == ogs_sbi_send_http_status_no_content(stream));
            OGS_FSM_TRAN(s, pcf_am_state_deleted);
            break;

        DEFAULT
            ogs_error("[%s] Invalid HTTP method [%s]",
                    pcf_ue_am->supi, message->h.method);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_METHOD_NOT_ALLOWED, message,
                    "Invalid HTTP method", message->h.method, NULL));
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
                            ogs_warn("[%s] Cannot find SUPI [%d]",
                                pcf_ue_am->supi, message->res_status);
                        } else {
                            ogs_error("[%s] HTTP response error [%d]",
                                pcf_ue_am->supi, message->res_status);
                        }
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(
                                stream, message->res_status,
                                NULL, "HTTP response error", pcf_ue_am->supi,
                                NULL));
                        break;
                    }

                    pcf_nudr_dr_handle_query_am_data(
                            pcf_ue_am, stream, message);
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid resource name [%s]",
                            pcf_ue_am->supi, message->h.resource.component[1]);
                    ogs_assert_if_reached();
                END
                break;

            DEFAULT
                ogs_error("[%s] Invalid resource name [%s]",
                        pcf_ue_am->supi, message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("[%s] Invalid API name [%s]",
                    pcf_ue_am->supi, message->h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                    "Invalid API name", message->h.resource.component[0],
                    NULL));
        END
        break;

    default:
        ogs_error("[%s] Unknown event %s",
                pcf_ue_am->supi, pcf_event_get_name(e));
        break;
    }
}

void pcf_am_state_deleted(ogs_fsm_t *s, pcf_event_t *e)
{
    pcf_ue_am_t *pcf_ue_am = NULL;
    ogs_assert(s);
    ogs_assert(e);

    pcf_sm_debug(e);

    pcf_ue_am = pcf_ue_am_find_by_id(e->pcf_ue_am_id);
    ogs_assert(pcf_ue_am);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("[%s] Unknown event %s", pcf_ue_am->supi,
                pcf_event_get_name(e));
        break;
    }
}

void pcf_am_state_exception(ogs_fsm_t *s, pcf_event_t *e)
{
    pcf_ue_am_t *pcf_ue_am = NULL;
    ogs_assert(s);
    ogs_assert(e);

    pcf_sm_debug(e);

    pcf_ue_am = pcf_ue_am_find_by_id(e->pcf_ue_am_id);
    ogs_assert(pcf_ue_am);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("[%s] Unknown event %s", pcf_ue_am->supi,
                pcf_event_get_name(e));
        break;
    }
}
