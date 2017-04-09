#define TRACE_MODULE _s1ap_sm

#include "core_debug.h"

#include "mme_event.h"

#include "s1ap_build.h"
#include "s1ap_handler.h"

void s1ap_state_initial(s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);

    FSM_TRAN(s, &s1ap_state_operational);
}

void s1ap_state_final(s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(1, e);
}

void s1ap_state_operational(s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_enb_t *enb = s->ctx;
    d_assert(enb, return, "Null param");

    mme_sm_trace(1, e);

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
        case EVT_MSG_MME_S1AP:
        {
            s1ap_message_t *message = (s1ap_message_t *)event_get_param3(e);
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
                        default:
                        {
                            d_warn("Not implemented(choice:%d, proc:%d",
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
                        default:
                        {
                            d_warn("Not implemented(choice:%d, proc:%d",
                                    message->direction, message->procedureCode);
                            break;
                        }
                    }
                    break;
                }
                case S1AP_PDU_PR_unsuccessfulOutcome :
                default:
                {
                    d_warn("Not implemented(choice:%d, proc:%d",
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

void s1ap_state_exception(s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(1, e);

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

