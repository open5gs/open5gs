/*
 * Copyright (C) 2019 by Sukchan Lee <acetcom@gmail.com>
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

#include "ngap-build.h"
#include "ngap-handler.h"
#include "ngap-path.h"

void ngap_state_initial(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);

    amf_sm_debug(e);

    OGS_FSM_TRAN(s, &ngap_state_operational);
}

void ngap_state_final(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);

    amf_sm_debug(e);
}

void ngap_state_operational(ogs_fsm_t *s, amf_event_t *e)
{
    int r;

    amf_gnb_t *gnb = NULL;
    ogs_pkbuf_t *pkbuf = NULL;

    NGAP_NGAP_PDU_t *pdu = NULL;
    NGAP_InitiatingMessage_t *initiatingMessage = NULL;
    NGAP_SuccessfulOutcome_t *successfulOutcome = NULL;
    NGAP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;

    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    gnb = e->gnb;
    ogs_assert(gnb);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    case AMF_EVENT_NGAP_MESSAGE:
        pdu = e->ngap.message;
        ogs_assert(pdu);
            
        if (!gnb->state.ng_setup_success &&
            !(pdu->present == NGAP_NGAP_PDU_PR_initiatingMessage &&
                pdu->choice.initiatingMessage->procedureCode ==
                    NGAP_ProcedureCode_id_NGSetup)) {
            break;
        }

        switch (pdu->present) {
        case NGAP_NGAP_PDU_PR_initiatingMessage:
            initiatingMessage = pdu->choice.initiatingMessage;
            ogs_assert(initiatingMessage);

            switch (initiatingMessage->procedureCode) {
            case NGAP_ProcedureCode_id_NGSetup:
                ngap_handle_ng_setup_request(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_InitialUEMessage:
                ngap_handle_initial_ue_message(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_UplinkNASTransport:
                ngap_handle_uplink_nas_transport(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_UERadioCapabilityInfoIndication:
                ngap_handle_ue_radio_capability_info_indication(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_UEContextReleaseRequest:
                ngap_handle_ue_context_release_request( gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_PathSwitchRequest:
                ngap_handle_path_switch_request(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_UplinkRANConfigurationTransfer:
                pkbuf = e->pkbuf;
                ogs_assert(pkbuf);

                ngap_handle_uplink_ran_configuration_transfer(gnb, pdu, pkbuf);
                break;
            case NGAP_ProcedureCode_id_HandoverPreparation:
                ngap_handle_handover_required(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_UplinkRANStatusTransfer:
                ngap_handle_uplink_ran_status_transfer(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_HandoverNotification:
                ngap_handle_handover_notification(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_HandoverCancel:
                ngap_handle_handover_cancel(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_RANConfigurationUpdate:
                ngap_handle_ran_configuration_update(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_NGReset:
                ngap_handle_ng_reset(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_ErrorIndication:
                ngap_handle_error_indication(gnb, pdu);
                break;
            default:
                ogs_error("Not implemented(choice:%d, proc:%d)",
                        pdu->present, (int)initiatingMessage->procedureCode);
                break;
            }
            break;
        case NGAP_NGAP_PDU_PR_successfulOutcome :
            successfulOutcome = pdu->choice.successfulOutcome;
            ogs_assert(successfulOutcome);

            switch (successfulOutcome->procedureCode) {
            case NGAP_ProcedureCode_id_InitialContextSetup:
                ngap_handle_initial_context_setup_response(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_PDUSessionResourceSetup:
                ngap_handle_pdu_session_resource_setup_response(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_PDUSessionResourceModify:
                ngap_handle_pdu_session_resource_modify_response(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_PDUSessionResourceRelease:
                ngap_handle_pdu_session_resource_release_response(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_UEContextModification:
                ngap_handle_ue_context_modification_response(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_UEContextRelease:
                ngap_handle_ue_context_release_complete(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_HandoverResourceAllocation:
                ngap_handle_handover_request_ack(gnb, pdu);
                break;
            default:
                ogs_error("Not implemented(choice:%d, proc:%d)",
                        pdu->present, (int)successfulOutcome->procedureCode);
                break;
            }
            break;
        case NGAP_NGAP_PDU_PR_unsuccessfulOutcome :
            unsuccessfulOutcome = pdu->choice.unsuccessfulOutcome;
            ogs_assert(unsuccessfulOutcome);

            switch (unsuccessfulOutcome->procedureCode) {
            case NGAP_ProcedureCode_id_InitialContextSetup :
                ngap_handle_initial_context_setup_failure(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_UEContextModification:
                ngap_handle_ue_context_modification_failure(gnb, pdu);
                break;
            case NGAP_ProcedureCode_id_HandoverResourceAllocation :
                ngap_handle_handover_failure(gnb, pdu);
                break;
            default:
                ogs_error("Not implemented(choice:%d, proc:%d)",
                        pdu->present, (int)unsuccessfulOutcome->procedureCode);
                break;
            }
            break;
        default:
            ogs_error("Not implemented(choice:%d)", pdu->present);
            break;
        }

        break;
    case AMF_EVENT_NGAP_TIMER:
        switch (e->h.timer_id) {
        case AMF_TIMER_NG_DELAYED_SEND:
            ogs_assert(e->ran_ue);
            ogs_assert(e->pkbuf);

            r = ngap_send_to_ran_ue(e->ran_ue, e->pkbuf);
            ogs_expect(r == OGS_OK);
            ogs_assert(r != OGS_ERROR);

            ogs_timer_delete(e->timer);
            break;
        default:
            ogs_error("Unknown timer[%s:%d]",
                    amf_timer_get_name(e->h.timer_id), e->h.timer_id);
            break;
        }
        break;
    default:
        ogs_error("Unknown event %s", amf_event_get_name(e));
        break;
    }
}

void ngap_state_exception(ogs_fsm_t *s, amf_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    amf_sm_debug(e);

    switch (e->h.id) {
    case OGS_FSM_ENTRY_SIG:
        break;
    case OGS_FSM_EXIT_SIG:
        break;
    default:
        ogs_error("Unknown event %s", amf_event_get_name(e));
        break;
    }
}

