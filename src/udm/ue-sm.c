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
#include "nudm-handler.h"
#include "nudr-handler.h"

void udm_ue_state_initial(ogs_fsm_t *s, udm_event_t *e)
{
    ogs_assert(s);

    OGS_FSM_TRAN(s, &udm_ue_state_operational);
}

void udm_ue_state_final(ogs_fsm_t *s, udm_event_t *e)
{
}

void udm_ue_state_operational(ogs_fsm_t *s, udm_event_t *e)
{
    udm_ue_t *udm_ue = NULL;

    ogs_sbi_session_t *session = NULL;
    ogs_sbi_message_t *message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    udm_sm_debug(e);

    udm_ue = e->udm_ue;
    ogs_assert(udm_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case UDM_EVT_SBI_SERVER:
        message = e->sbi.message;
        ogs_assert(message);
        session = e->sbi.session;
        ogs_assert(session);

        SWITCH(message->h.method)
        CASE(OGS_SBI_HTTP_METHOD_POST)
            SWITCH(message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_SECURITY_INFORMATION)
                udm_nudm_ueau_handle_get(udm_ue, message);
                break;
            CASE(OGS_SBI_RESOURCE_NAME_AUTH_EVENTS)
                udm_nudm_ueau_handle_result_confirmation_inform(
                        udm_ue, message);
                break;
            DEFAULT
                ogs_error("[%s] Invalid resource name  [%s]",
                        udm_ue->suci, message->h.resource.component[1]);
                ogs_sbi_server_send_error(session,
                        OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED, message,
                        "Invalid HTTP method", message->h.method);
            END
            break;
        DEFAULT
            ogs_error("[%s] Invalid HTTP method [%s]",
                    udm_ue->suci, message->h.method);
            ogs_sbi_server_send_error(session,
                    OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED, message,
                    "Invalid HTTP method", message->h.method);
        END
        break;

    case UDM_EVT_SBI_CLIENT:
        message = e->sbi.message;
        ogs_assert(message);
        udm_ue = e->sbi.data;
        ogs_assert(udm_ue);
        session = udm_ue->session;
        ogs_assert(session);

        SWITCH(message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NUDR_DR)
            SWITCH(message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SUBSCRIPTION_DATA)
                SWITCH(message->h.resource.component[2])
                CASE(OGS_SBI_RESOURCE_NAME_AUTHENTICATION_DATA)
                    ogs_timer_stop(udm_ue->sbi_client_wait.timer);

                    if (message->res_status != OGS_SBI_HTTP_STATUS_OK &&
                        message->res_status != OGS_SBI_HTTP_STATUS_NO_CONTENT) {
                        ogs_error("[%s] HTTP response error [%d]",
                            udm_ue->suci, message->res_status);
                        ogs_sbi_server_send_error(
                            session, message->res_status,
                            NULL, "HTTP response error", udm_ue->suci);
                        break;
                    }

                    udm_nudr_dr_handle_subscription_authentication(
                            udm_ue, message);
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
            ogs_error("Invalid API name [%s]", message->h.service.name);
            ogs_sbi_server_send_error(session,
                    OGS_SBI_HTTP_STATUS_MEHTOD_NOT_ALLOWED, message,
                    "Invalid API name", message->h.resource.component[0]);
        END
        break;

    default:
        ogs_error("[%s] Unknown event %s", udm_ue->suci, udm_event_get_name(e));
        break;
    }
}

void udm_ue_state_exception(ogs_fsm_t *s, udm_event_t *e)
{
    udm_ue_t *udm_ue = NULL;
    ogs_assert(s);
    ogs_assert(e);

    udm_sm_debug(e);

    udm_ue = e->udm_ue;
    ogs_assert(udm_ue);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("[%s] Unknown event %s", udm_ue->suci, udm_event_get_name(e));
        break;
    }
}
