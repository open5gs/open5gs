#define TRACE_MODULE _s1ap_sm

#include "core_debug.h"

#include "nas/nas_message.h"
#include "gtp/gtp_message.h"

#include "s1ap_build.h"
#include "s1ap_handler.h"

#include "mme_event.h"
#include "mme_sm.h"

void s1ap_state_initial(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(3, e);

    FSM_TRAN(s, &s1ap_state_operational);
}

void s1ap_state_final(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(3, e);
}

void s1ap_state_operational(fsm_t *s, event_t *e)
{
    mme_enb_t *enb = NULL;

    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    enb = mme_enb_find(event_get_param1(e));
    d_assert(enb, return,);

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            break;
        }
        case FSM_EXIT_SIG:
        {
            break;
        }
        case MME_EVT_S1AP_MESSAGE:
        {
            s1ap_message_t *message = (s1ap_message_t *)event_get_param4(e);
            d_assert(message, break, "Null param");

            switch(message->direction)
            {
                case S1AP_PDU_PR_initiatingMessage :
                {
                    switch(message->procedureCode)
                    {
                        case S1ap_ProcedureCode_id_S1Setup :
                        {
                            s1ap_handle_s1_setup_request(enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_initialUEMessage :
                        {
                            s1ap_handle_initial_ue_message(enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_uplinkNASTransport :
                        {
                            s1ap_handle_uplink_nas_transport(enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_UECapabilityInfoIndication :
                        {
                            s1ap_handle_ue_capability_info_indication(
                                    enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_UEContextReleaseRequest:
                        {
                            s1ap_handle_ue_context_release_request(
                                    enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_PathSwitchRequest:
                        {
                            s1ap_handle_path_switch_request(enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_HandoverPreparation:
                        {
                            s1ap_handle_handover_required(enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_HandoverCancel:
                        {
                            s1ap_handle_handover_cancel(enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_eNBStatusTransfer:
                        {
                            s1ap_handle_enb_status_transfer(enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_eNBConfigurationTransfer:
                        {
                            s1ap_handle_enb_configuration_transfer(
                                    enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_HandoverNotification:
                        {
                            s1ap_handle_handover_notification(enb, message);
                            break;
                        }
                        default:
                        {
                            d_warn("Not implemented(choice:%d, proc:%d)",
                                    message->direction, message->procedureCode);
                            break;
                        }
                    }

                    break;
                }
                case S1AP_PDU_PR_successfulOutcome :
                {
                    switch(message->procedureCode)
                    {
                        case S1ap_ProcedureCode_id_InitialContextSetup :
                        {
                            s1ap_handle_initial_context_setup_response(
                                    enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_E_RABSetup :
                        {
                            s1ap_handle_e_rab_setup_response(enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_E_RABModify :
                        {
                            break;
                        }
                        case S1ap_ProcedureCode_id_E_RABRelease :
                        {
                            break;
                        }
                        case S1ap_ProcedureCode_id_UEContextRelease :
                        {
                            s1ap_handle_ue_context_release_complete(
                                    enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_HandoverResourceAllocation:
                        {
                            s1ap_handle_handover_request_ack(enb, message);
                            break;
                        }
                        default:
                        {
                            d_warn("Not implemented(choice:%d, proc:%d)",
                                    message->direction, message->procedureCode);
                            break;
                        }
                    }
                    break;
                }
                case S1AP_PDU_PR_unsuccessfulOutcome :
                {
                    switch(message->procedureCode)
                    {
                        case S1ap_ProcedureCode_id_InitialContextSetup :
                        {
                            s1ap_handle_initial_context_setup_failure(
                                    enb, message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_HandoverResourceAllocation :
                        {
                            s1ap_handle_handover_failure(enb, message);
                            break;
                        }
                        default:
                        {
                            d_warn("Not implemented(choice:%d, proc:%d)",
                                    message->direction, message->procedureCode);
                            break;
                        }
                    }
                    break;
                }
                default:
                {
                    d_warn("Not implemented(choice:%d, proc:%d)",
                            message->direction, message->procedureCode);
                    break;
                }
            }

            break;
        }
        default:
        {
            d_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}

void s1ap_state_exception(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            break;
        }
        case FSM_EXIT_SIG:
        {
            break;
        }
        default:
        {
            d_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}

