#include "nas/nas_message.h"
#include "gtp/gtp_message.h"

#include "s1ap_build.h"
#include "s1ap_handler.h"

#include "mme_event.h"
#include "mme_sm.h"

void s1ap_state_initial(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);

    mme_sm_debug(e);

    OGS_FSM_TRAN(s, &s1ap_state_operational);
}

void s1ap_state_final(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);

    mme_sm_debug(e);
}

void s1ap_state_operational(ogs_fsm_t *s, mme_event_t *e)
{
    mme_enb_t *enb = NULL;

    ogs_assert(s);
    ogs_assert(e);

    ogs_assert(e);
    mme_sm_debug(e);

    enb = e->enb;
    ogs_assert(enb);

    switch (e->id)
    {
        case OGS_FSM_ENTRY_SIG:
        {
            break;
        }
        case OGS_FSM_EXIT_SIG:
        {
            break;
        }
        case MME_EVT_S1AP_MESSAGE:
        {
            S1AP_S1AP_PDU_t *pdu = e->s1ap_message;
            ogs_assert(pdu);

            switch(pdu->present)
            {
                case S1AP_S1AP_PDU_PR_initiatingMessage :
                {
                    S1AP_InitiatingMessage_t *initiatingMessage =
                        pdu->choice.initiatingMessage;
                    ogs_assert(initiatingMessage);

                    switch(initiatingMessage->procedureCode)
                    {
                        case S1AP_ProcedureCode_id_S1Setup :
                        {
                            s1ap_handle_s1_setup_request(enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_initialUEMessage :
                        {
                            s1ap_handle_initial_ue_message(enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_uplinkNASTransport :
                        {
                            s1ap_handle_uplink_nas_transport(enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_UECapabilityInfoIndication :
                        {
                            s1ap_handle_ue_capability_info_indication(
                                    enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_UEContextReleaseRequest:
                        {
                            s1ap_handle_ue_context_release_request(
                                    enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_PathSwitchRequest:
                        {
                            s1ap_handle_path_switch_request(enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_eNBConfigurationTransfer:
                        {
                            ogs_pkbuf_t *pkbuf = e->pkbuf;
                            ogs_assert(pkbuf);

                            s1ap_handle_enb_configuration_transfer(
                                    enb, pdu, pkbuf);
                            break;
                        }
                        case S1AP_ProcedureCode_id_HandoverPreparation:
                        {
                            s1ap_handle_handover_required(enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_HandoverCancel:
                        {
                            s1ap_handle_handover_cancel(enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_eNBStatusTransfer:
                        {
                            s1ap_handle_enb_status_transfer(enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_HandoverNotification:
                        {
                            s1ap_handle_handover_notification(enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_Reset:
                        {
                            s1ap_handle_s1_reset(enb, pdu);
                            break;
                        }
                        default:
                        {
                            ogs_warn("Not implemented(choice:%d, proc:%d)",
                                    pdu->present,
                                    (int)initiatingMessage->procedureCode);
                            break;
                        }
                    }

                    break;
                }
                case S1AP_S1AP_PDU_PR_successfulOutcome :
                {
                    S1AP_SuccessfulOutcome_t *successfulOutcome =
                        pdu->choice.successfulOutcome;
                    ogs_assert(successfulOutcome);

                    switch(successfulOutcome->procedureCode)
                    {
                        case S1AP_ProcedureCode_id_InitialContextSetup :
                        {
                            s1ap_handle_initial_context_setup_response(
                                    enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_E_RABSetup :
                        {
                            s1ap_handle_e_rab_setup_response(enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_E_RABModify :
                        {
                            break;
                        }
                        case S1AP_ProcedureCode_id_E_RABRelease :
                        {
                            break;
                        }
                        case S1AP_ProcedureCode_id_UEContextRelease :
                        {
                            s1ap_handle_ue_context_release_complete(
                                    enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_HandoverResourceAllocation:
                        {
                            s1ap_handle_handover_request_ack(enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_WriteReplaceWarning:
                        {
                            s1ap_handle_write_replace_warning_response(enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_Kill:
                        {
                            s1ap_handle_kill_response(enb, pdu);
                            break;
                        }
                        default:
                        {
                            ogs_warn("Not implemented(choice:%d, proc:%d)",
                                    pdu->present,
                                    (int)successfulOutcome->procedureCode);
                            break;
                        }
                    }
                    break;
                }
                case S1AP_S1AP_PDU_PR_unsuccessfulOutcome :
                {
                    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = 
                        pdu->choice.unsuccessfulOutcome;
                    ogs_assert(unsuccessfulOutcome);

                    switch(unsuccessfulOutcome->procedureCode)
                    {
                        case S1AP_ProcedureCode_id_InitialContextSetup :
                        {
                            s1ap_handle_initial_context_setup_failure(
                                    enb, pdu);
                            break;
                        }
                        case S1AP_ProcedureCode_id_HandoverResourceAllocation :
                        {
                            s1ap_handle_handover_failure(enb, pdu);
                            break;
                        }
                        default:
                        {
                            ogs_warn("Not implemented(choice:%d, proc:%d)",
                                    pdu->present,
                                    (int)unsuccessfulOutcome->procedureCode);
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    ogs_warn("Not implemented(choice:%d)", pdu->present);
                    break;
                }
            }

            break;
        }
        default:
        {
            ogs_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}

void s1ap_state_exception(ogs_fsm_t *s, mme_event_t *e)
{
    ogs_assert(s);
    ogs_assert(e);

    mme_sm_debug(e);

    switch (e->id)
    {
        case OGS_FSM_ENTRY_SIG:
        {
            break;
        }
        case OGS_FSM_EXIT_SIG:
        {
            break;
        }
        default:
        {
            ogs_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}

