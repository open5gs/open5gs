/*
 * Copyright (C) 2019,2020 by Sukchan Lee <acetcom@gmail.com>
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
    ogs_sbi_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    pcf_sm_debug(e);

    sess = e->sess;
    ogs_assert(sess);
    pcf_ue = sess->pcf_ue;
    ogs_assert(pcf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case PCF_EVT_SBI_SERVER:
        message = e->sbi.message;
        ogs_assert(message);
        stream = e->sbi.data;
        ogs_assert(stream);

        SWITCH(message->h.method)
        CASE(OGS_SBI_HTTP_METHOD_POST)
            handled = pcf_npcf_smpolicycontrtol_handle_create(
                    sess, stream, message);
            if (!handled) {
                ogs_error("[%s:%d] Cannot handle SBI message",
                        pcf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, pcf_sm_state_exception);
            }
            break;

        DEFAULT
            ogs_error("[%s:%d] Invalid HTTP method [%s]",
                    pcf_ue->supi, sess->psi, message->h.method);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_FORBIDDEN, message,
                    "Invalid HTTP method", message->h.method);
        END
        break;

    case PCF_EVT_SBI_CLIENT:
        message = e->sbi.message;
        ogs_assert(message);
        stream = e->sbi.data;
        ogs_assert(stream);

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
                        } else {
                            ogs_error("[%s:%d] HTTP response error [%d]",
                                pcf_ue->supi, sess->psi, message->res_status);
                        }
                        ogs_sbi_server_send_error(stream, message->res_status,
                            NULL, "HTTP response error", pcf_ue->supi);
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

        DEFAULT
            ogs_error("[%s:%d] Invalid API name [%s]",
                        pcf_ue->supi, sess->psi, message->h.service.name);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                    "Invalid API name", message->h.resource.component[0]);
        END
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

    sess = e->sess;
    ogs_assert(sess);
    pcf_ue = sess->pcf_ue;
    ogs_assert(pcf_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("[%s:%d] Unknown event %s",
                pcf_ue->supi, sess->psi, pcf_event_get_name(e));
        break;
    }
}
