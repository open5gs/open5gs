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

    ogs_sbi_stream_t *stream = NULL;
    ogs_pool_id_t stream_id = OGS_INVALID_POOL_ID;
    ogs_sbi_message_t *message = NULL;
    int r;

    ogs_assert(s);
    ogs_assert(e);

    udm_sm_debug(e);

    udm_ue = udm_ue_find_by_id(e->udm_ue_id);
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
        CASE(OGS_SBI_SERVICE_NAME_NUDM_UEAU)
            SWITCH(message->h.method)
            CASE(OGS_SBI_HTTP_METHOD_POST)
                SWITCH(message->h.resource.component[1])
                CASE(OGS_SBI_RESOURCE_NAME_SECURITY_INFORMATION)
                    udm_nudm_ueau_handle_get(udm_ue, stream, message);
                    break;
                CASE(OGS_SBI_RESOURCE_NAME_AUTH_EVENTS)
                    udm_nudm_ueau_handle_result_confirmation_inform(
                            udm_ue, stream, message);
                    break;
                DEFAULT
                    ogs_error("[%s] Invalid resource name [%s]",
                            udm_ue->suci, message->h.resource.component[1]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                            "Invalid resource name", message->h.method, NULL));
                END
                break;

            CASE(OGS_SBI_HTTP_METHOD_PUT)
                SWITCH(message->h.resource.component[1])
                CASE(OGS_SBI_RESOURCE_NAME_AUTH_EVENTS)
                    udm_nudm_ueau_handle_result_confirmation_inform(
                            udm_ue, stream, message);
                    break;
                DEFAULT
                    ogs_error("[%s] Invalid resource name [%s]",
                            udm_ue->suci, message->h.resource.component[1]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                            "Invalid resource name", message->h.method, NULL));
                END
                break;

            DEFAULT
                ogs_error("[%s] Invalid HTTP method [%s]",
                        udm_ue->suci, message->h.method);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_FORBIDDEN, message,
                        "Invalid HTTP method", message->h.method, NULL));
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_UECM)
            SWITCH(message->h.method)
            CASE(OGS_SBI_HTTP_METHOD_PUT)
                SWITCH(message->h.resource.component[1])
                CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
                    udm_nudm_uecm_handle_amf_registration(
                            udm_ue, stream, message);
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid resource name [%s]",
                            udm_ue->suci, message->h.resource.component[1]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                            "Invalid HTTP method", message->h.method, NULL));
                END
                break;
            CASE(OGS_SBI_HTTP_METHOD_PATCH)
                SWITCH(message->h.resource.component[1])
                CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
                    udm_nudm_uecm_handle_amf_registration_update(
                            udm_ue, stream, message);
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid resource name [%s]",
                            udm_ue->suci, message->h.resource.component[1]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                            "Invalid HTTP method", message->h.method, NULL));
                END
                break;
            CASE(OGS_SBI_HTTP_METHOD_GET)
                SWITCH(message->h.resource.component[1])
                CASE(OGS_SBI_RESOURCE_NAME_REGISTRATIONS)
                    r = udm_nudm_uecm_handle_amf_registration_get(
                            udm_ue, stream, message);
                    if (!r)
                    {
                        ogs_assert(true ==
                            ogs_sbi_server_send_error(stream,
                                OGS_SBI_HTTP_STATUS_FORBIDDEN, message,
                                "Invalid UE Identifier", message->h.method, NULL));
                    }
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid resource name [%s]",
                            udm_ue->suci, message->h.resource.component[1]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                            "Invalid resource name", message->h.method, NULL));
                END
                break;
            DEFAULT
                ogs_error("[%s] Invalid HTTP method [%s]",
                        udm_ue->suci, message->h.method);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_FORBIDDEN, message,
                        "Invalid HTTP method", message->h.method, NULL));
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            SWITCH(message->h.method)
            CASE(OGS_SBI_HTTP_METHOD_GET)
                SWITCH(message->h.resource.component[1])
                CASE(OGS_SBI_RESOURCE_NAME_AM_DATA)
                CASE(OGS_SBI_RESOURCE_NAME_SMF_SELECT_DATA)
                CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
                    r = udm_ue_sbi_discover_and_send(
                            OGS_SBI_SERVICE_TYPE_NUDR_DR, NULL,
                            udm_nudr_dr_build_query_subscription_provisioned,
                            udm_ue, stream, message);
                    ogs_expect(r == OGS_OK);
                    ogs_assert(r != OGS_ERROR);
                    break;

                CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXT_IN_SMF_DATA)
                    udm_nudm_sdm_handle_subscription_provisioned(
                            udm_ue, stream, message);
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid resource name [%s]",
                            udm_ue->suci, message->h.resource.component[1]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                            "Invalid resource name", message->h.method, NULL));
                END
                break;

            CASE(OGS_SBI_HTTP_METHOD_POST)
                SWITCH(message->h.resource.component[1])
                CASE(OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS)
                    udm_nudm_sdm_handle_subscription_create(
                            udm_ue, stream, message);
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid resource name [%s]",
                            udm_ue->suci, message->h.resource.component[1]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                            "Invalid resource name", message->h.method, NULL));
                END
                break;

            CASE(OGS_SBI_HTTP_METHOD_DELETE)
                SWITCH(message->h.resource.component[1])
                CASE(OGS_SBI_RESOURCE_NAME_SDM_SUBSCRIPTIONS)
                    udm_nudm_sdm_handle_subscription_delete(
                            udm_ue, stream, message);
                    break;

                DEFAULT
                    ogs_error("[%s] Invalid resource name [%s]",
                            udm_ue->suci, message->h.resource.component[1]);
                    ogs_assert(true ==
                        ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                            "Invalid resource name", message->h.method, NULL));
                END
                break;
            DEFAULT
                ogs_error("[%s] Invalid HTTP method [%s]",
                        udm_ue->supi, message->h.method);
                ogs_assert(true ==
                    ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_NOT_FOUND, message,
                        "Invalid HTTP method", message->h.method,
                        NULL));
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

        udm_ue = udm_ue_find_by_id(e->udm_ue_id);
        ogs_assert(udm_ue);

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
                CASE(OGS_SBI_RESOURCE_NAME_AUTHENTICATION_DATA)
                    udm_nudr_dr_handle_subscription_authentication(
                            udm_ue, stream, message);
                    break;

                CASE(OGS_SBI_RESOURCE_NAME_CONTEXT_DATA)
                    udm_nudr_dr_handle_subscription_context(
                            udm_ue, stream, message);
                    break;

                DEFAULT
                    SWITCH(message->h.resource.component[3])
                    CASE(OGS_SBI_RESOURCE_NAME_PROVISIONED_DATA)
                        udm_nudr_dr_handle_subscription_provisioned(
                                udm_ue, stream, message);
                        break;

                    DEFAULT
                        ogs_error("Invalid resource name [%s]",
                                message->h.resource.component[2]);
                        ogs_assert_if_reached();
                    END
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
            ogs_assert(true ==
                ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, message,
                    "Invalid API name", message->h.resource.component[0],
                    NULL));
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

    udm_ue = udm_ue_find_by_id(e->udm_ue_id);
    ogs_assert(udm_ue);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("[%s] Unknown event %s", udm_ue->suci, udm_event_get_name(e));
        break;
    }
}
