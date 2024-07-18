/*
 * Copyright (C) 2019-2022 by Sukchan Lee <acetcom@gmail.com>
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

#include "nudm-handler.h"
#include "nudr-handler.h"

void udm_sess_state_initial(ogs_fsm_t *s, udm_event_t *e)
{
    ogs_assert(s);

    OGS_FSM_TRAN(s, &udm_sess_state_operational);
}

void udm_sess_state_final(ogs_fsm_t *s, udm_event_t *e)
{
}

void udm_sess_state_operational(ogs_fsm_t *s, udm_event_t *e)
{
    udm_ue_t *udm_ue = NULL;
    udm_sess_t *sess = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    udm_sm_debug(e);

    sess = udm_sess_find_by_id(e->sess_id);
    ogs_assert(sess);
    udm_ue = udm_ue_find_by_id(sess->udm_ue_id);
    ogs_assert(udm_ue);

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
        CASE(OGS_SBI_SERVICE_NAME_NUDM_UECM)
            SWITCH(message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
                SWITCH(message->h.method)
                CASE(OGS_SBI_HTTP_METHOD_PUT)
                    udm_nudm_uecm_handle_smf_registration(
                            sess, stream, message);
                    break;
                CASE(OGS_SBI_HTTP_METHOD_DELETE)
                    udm_nudm_uecm_handle_smf_deregistration(
                            sess, stream, message);
                    break;

                DEFAULT
                    ogs_error("[%s:%d] Invalid HTTP method [%s]",
                            udm_ue->suci, sess->psi, message->h.method);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_FORBIDDEN, message,
                            "Invalid HTTP method", message->h.method,
                            NULL));
                END
                break;
            DEFAULT
                ogs_error("[%s:%d] Invalid resource name [%s]",
                        udm_ue->suci, sess->psi,
                        message->h.resource.component[1]);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                        "Invalid HTTP method", message->h.method, NULL));
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", message->h.service.name);
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                    "Invalid API name", message->h.service.name, NULL));
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
            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA)
                SWITCH(message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_CONTEXT_DATA)
                    udm_nudr_dr_handle_smf_registration(
                            sess, stream, message);
                    break;

                DEFAULT
                    ogs_error("Invalid resource name [%s]",
                            message->h.resource.component[2]);
                    ogs_assert_if_reached();
                END
                break;
            DEFAULT
                ogs_error("Invalid resource name [%s]",
                        message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("[%s:%d] Invalid API name [%s]",
                        udm_ue->supi, sess->psi, message->h.service.name);
            ogs_assert_if_reached();
        END
        break;

    default:
        ogs_error("[%s:%d] Unknown event %s",
                udm_ue->supi, sess->psi, udm_event_get_name(e));
        break;
    }
}

void udm_sess_state_exception(ogs_fsm_t *s, udm_event_t *e)
{
    udm_ue_t *udm_ue = NULL;
    udm_sess_t *sess = NULL;

    ogs_assert(s);
    ogs_assert(e);

    udm_sm_debug(e);

    sess = udm_sess_find_by_id(e->sess_id);
    ogs_assert(sess);
    udm_ue = udm_ue_find_by_id(sess->udm_ue_id);
    ogs_assert(udm_ue);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("[%s:%d] Unknown event %s",
                udm_ue->supi, sess->psi, udm_event_get_name(e));
        break;
    }
}
