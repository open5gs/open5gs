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
#include "nsmf-handler.h"
#include "nudm-handler.h"
#include "npcf-handler.h"
#include "gsm-handler.h"
#include "ngap-handler.h"
#include "pfcp-path.h"

void smf_gsm_state_initial(ogs_fsm_t *s, smf_event_t *e)
{
    ogs_assert(s);

    OGS_FSM_TRAN(s, &smf_gsm_state_operational);
}

void smf_gsm_state_final(ogs_fsm_t *s, smf_event_t *e)
{
}

void smf_gsm_state_operational(ogs_fsm_t *s, smf_event_t *e)
{
    int rv;
    char *strerror = NULL;
    smf_ue_t *smf_ue = NULL;
    smf_sess_t *sess = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    ogs_nas_5gs_message_t *nas_message = NULL;

    ogs_sbi_stream_t *stream = NULL;
    ogs_sbi_message_t *sbi_message = NULL;

    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = e->sess;
    ogs_assert(sess);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    case SMF_EVT_SBI_SERVER:
        sbi_message = e->sbi.message;
        ogs_assert(sbi_message);
        stream = e->sbi.data;
        ogs_assert(stream);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NSMF_PDUSESSION)
            SWITCH(sbi_message->h.resource.component[2])
            CASE(OGS_SBI_RESOURCE_NAME_MODIFY)
                smf_nsmf_handle_update_sm_context(sess, stream, sbi_message);
                break;
            CASE(OGS_SBI_RESOURCE_NAME_RELEASE)
                smf_nsmf_handle_release_sm_context(sess, stream, sbi_message);
                break;
            DEFAULT
                smf_nsmf_handle_create_sm_context(sess, stream, sbi_message);
                break;
            END
            break;

        DEFAULT
            ogs_error("Invalid API name [%s]", sbi_message->h.service.name);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, sbi_message,
                    "Invalid API name", sbi_message->h.service.name);
        END
        break;

    case SMF_EVT_SBI_CLIENT:
        sbi_message = e->sbi.message;
        ogs_assert(sbi_message);

        sess = e->sess;
        ogs_assert(sess);
        smf_ue = sess->smf_ue;
        ogs_assert(smf_ue);

        stream = e->sbi.data;
        ogs_assert(stream);

        SWITCH(sbi_message->h.service.name)
        CASE(OGS_SBI_SERVICE_NAME_NUDM_SDM)
            SWITCH(sbi_message->h.resource.component[1])
            CASE(OGS_SBI_RESOURCE_NAME_SM_DATA)
                if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK) {
                    strerror = ogs_msprintf("[%s:%d] HTTP response error [%d]",
                            smf_ue->supi, sess->psi, sbi_message->res_status);
                    ogs_assert(strerror);

                    ogs_error("%s", strerror);
                    ogs_sbi_server_send_error(stream, sbi_message->res_status,
                        sbi_message, strerror, NULL);
                    ogs_free(strerror);
                    break;
                }

                if (smf_nudm_sdm_handle_get(
                            sess, stream, sbi_message) != true) {
                    ogs_sbi_server_send_error(stream,
                            OGS_SBI_HTTP_STATUS_INTERNAL_SERVER_ERROR,
                            sbi_message, "HTTP response error", smf_ue->supi);
                }
                break;

            DEFAULT
                strerror = ogs_msprintf("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[1]);
                ogs_assert(strerror);

                ogs_error("%s", strerror);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        sbi_message, strerror, NULL);
                ogs_free(strerror);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NPCF_SMPOLICYCONTROL)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_SM_POLICIES)
                if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_CREATED) {
                    strerror = ogs_msprintf("[%s:%d] HTTP response error [%d]",
                            smf_ue->supi, sess->psi, sbi_message->res_status);
                    ogs_assert(strerror);

                    ogs_error("%s", strerror);
                    ogs_sbi_server_send_error(stream, sbi_message->res_status,
                            sbi_message, strerror, NULL);
                    ogs_free(strerror);
                    break;
                }

                smf_npcf_smpolicycontrol_handle_create(
                        sess, stream, sbi_message);
                break;

            DEFAULT
                strerror = ogs_msprintf("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[0]);
                ogs_assert(strerror);

                ogs_error("%s", strerror);
                ogs_sbi_server_send_error(stream,
                        OGS_SBI_HTTP_STATUS_BAD_REQUEST,
                        sbi_message, strerror, NULL);
                ogs_free(strerror);
            END
            break;

        CASE(OGS_SBI_SERVICE_NAME_NAMF_COMM)
            SWITCH(sbi_message->h.resource.component[0])
            CASE(OGS_SBI_RESOURCE_NAME_UE_CONTEXTS)
                if (sbi_message->res_status != OGS_SBI_HTTP_STATUS_OK &&
                    sbi_message->res_status != OGS_SBI_HTTP_STATUS_ACCEPTED) {
                    ogs_error("[%s:%d] HTTP response error [%d]",
                        smf_ue->supi, sess->psi, sbi_message->res_status);
                    break;
                }
                break;

            DEFAULT
                ogs_error("[%s:%d] Invalid resource name [%s]",
                        smf_ue->supi, sess->psi,
                        sbi_message->h.resource.component[0]);
                ogs_assert_if_reached();
            END
            break;

        DEFAULT
            ogs_error("[%s:%d] Invalid API name [%s]",
                    smf_ue->supi, sess->psi, sbi_message->h.service.name);

        END
        break;

    case SMF_EVT_5GSM_MESSAGE:
        nas_message = e->nas.message;
        ogs_assert(nas_message);
        sess = e->sess;
        ogs_assert(sess);
        stream = e->sbi.data;
        ogs_assert(stream);
        smf_ue = sess->smf_ue;
        ogs_assert(smf_ue);

        switch (nas_message->gsm.h.message_type) {
        case OGS_NAS_5GS_PDU_SESSION_ESTABLISHMENT_REQUEST:
            rv = gsm_handle_pdu_session_establishment_request(sess, stream,
                    &nas_message->gsm.pdu_session_establishment_request);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Cannot handle NAS message",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            }
            break;

        case OGS_NAS_5GS_PDU_SESSION_RELEASE_REQUEST:
            smf_5gc_pfcp_send_session_deletion_request(
                    sess, stream, OGS_PFCP_DELETE_TRIGGER_UE_REQUESTED);
            break;

        case OGS_NAS_5GS_PDU_SESSION_RELEASE_COMPLETE:
            smf_sbi_send_response(stream, OGS_SBI_HTTP_STATUS_NO_CONTENT);

            /*
             * Race condition for PDU session release complete
             *  - CLIENT : /nsmf-pdusession/v1/sm-contexts/{smContextRef}/modify
             *  - SERVER : /namf-callback/v1/{supi}/sm-context-status/{psi})
             *
             * smf_sbi_send_response(stream, OGS_SBI_HTTP_STATUS_NO_CONTENT);
             * smf_sbi_send_sm_context_status_notify(sess);
             *
             * When executed as above,
             * NOTIFY transmits first, and Modify's Response transmits later.
             *
             * Use the Release Timer to send Notify
             * later than Modify's Response.
             */
            ogs_timer_start(sess->t_release_holding, ogs_time_from_msec(1));
            break;

        default:
            strerror = ogs_msprintf("Unknown message [%d]",
                    nas_message->gsm.h.message_type);
            ogs_assert(strerror);

            ogs_error("%s", strerror);
            ogs_sbi_server_send_error(stream,
                    OGS_SBI_HTTP_STATUS_BAD_REQUEST, NULL, strerror, NULL);
            ogs_free(strerror);
            break;
        }

        break;

    case SMF_EVT_NGAP_MESSAGE:
        sess = e->sess;
        ogs_assert(sess);
        stream = e->sbi.data;
        ogs_assert(stream);
        smf_ue = sess->smf_ue;
        ogs_assert(smf_ue);
        pkbuf = e->pkbuf;
        ogs_assert(pkbuf);
        ogs_assert(e->ngap.type);

        switch (e->ngap.type) {
        case OpenAPI_n2_sm_info_type_PDU_RES_SETUP_RSP:
            rv = ngap_handle_pdu_session_resource_setup_response_transfer(
                    sess, stream, pkbuf);
            if (rv != OGS_OK) {
                ogs_error("[%s:%d] Cannot handle NGAP message",
                        smf_ue->supi, sess->psi);
                OGS_FSM_TRAN(s, smf_gsm_state_exception);
            }
            break;

        case OpenAPI_n2_sm_info_type_PDU_RES_REL_RSP:
            smf_sbi_send_response(stream, OGS_SBI_HTTP_STATUS_NO_CONTENT);
            break;

        default:
            ogs_error("Unknown message[%d]", e->ngap.type);
        }

        break;

    default:
        ogs_error("Unknown event [%s]", smf_event_get_name(e));
        break;
    }
}

void smf_gsm_state_exception(ogs_fsm_t *s, smf_event_t *e)
{
    smf_sess_t *sess = NULL;
    ogs_assert(s);
    ogs_assert(e);

    smf_sm_debug(e);

    sess = e->sess;
    ogs_assert(sess);

    switch (e->id) {
    case OGS_FSM_ENTRY_SIG:
        break;

    case OGS_FSM_EXIT_SIG:
        break;

    default:
        ogs_error("Unknown event %s", smf_event_get_name(e));
        break;
    }
}
