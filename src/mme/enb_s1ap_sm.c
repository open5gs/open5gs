#define TRACE_MODULE _enb_s1_sm

#include "core_debug.h"

#include "s1ap_build.h"
#include "s1ap_conv.h"
#include "s1ap_path.h"

#include "sm.h"
#include "context.h"
#include "event.h"

static status_t enb_s1ap_handle_s1setuprequest(
        enb_ctx_t *enb, s1ap_message *message);
static status_t enb_s1ap_handle_initialuemessage(
        enb_ctx_t *enb, s1ap_message *message);

void enb_s1ap_state_initial(enb_s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    sm_trace(1, e);

    FSM_TRAN(s, &enb_s1ap_state_operational);
}

void enb_s1ap_state_final(enb_s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    sm_trace(1, e);
}

void enb_s1ap_state_operational(enb_s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    enb_ctx_t *enb = s->ctx;
    d_assert(enb, return, "Null param");

    sm_trace(1, e);

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
        case EVT_ENB_S1AP_INF:
        {
            s1ap_message message;
            status_t rv;
            pkbuf_t *recvbuf = event_get_msg(e);

            rv = s1ap_decode_pdu(&message, recvbuf);
            if (rv != CORE_OK) 
            {
                d_error("Can't parse S1AP_PDU in EVT_ENB_S1APAP_INF");
                break;
            }

            switch(message.direction)
            {
                case S1AP_PDU_PR_initiatingMessage :
                {
                    switch(message.procedureCode)
                    {
                        case S1ap_ProcedureCode_id_S1Setup :
                        {
                            enb_s1ap_handle_s1setuprequest(enb, &message);
                            break;
                        }
                        case S1ap_ProcedureCode_id_initialUEMessage :
                        {
                            enb_s1ap_handle_initialuemessage(enb, &message);
                            break;
                        }
                        default:
                        {
                            d_warn("Not implemented(choice:%d, proc:%d",
                                    message.direction, message.procedureCode);
                            break;
                        }
                    }
                    break;
                }
                case S1AP_PDU_PR_successfulOutcome :
                {
                    switch(message.procedureCode)
                    {
                        default:
                        {
                            d_warn("Not implemented(choice:%d, proc:%d",
                                    message.direction, message.procedureCode);
                            break;
                        }
                    }
                    break;
                }
                case S1AP_PDU_PR_unsuccessfulOutcome :
                default:
                {
                    d_warn("Not implemented(choice:%d, proc:%d",
                        message.direction, message.procedureCode);
                    break;
                }
            }

            s1ap_free_pdu(&message);
            break;
        }

        default:
        {
            d_error("Unknown event %s", event_get_name(e));
            break;
        }
    }
}

void enb_s1ap_state_exception(enb_s1ap_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    sm_trace(1, e);

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
            d_error("Unknown event %s", event_get_name(e));
            break;
        }
    }
}

status_t enb_s1ap_handle_s1setuprequest(enb_ctx_t *enb, s1ap_message *message)
{
    char buf[INET_ADDRSTRLEN];

    S1ap_S1SetupRequestIEs_t *ies = NULL;
    pkbuf_t *sendbuf = NULL;
    c_uint32_t enb_id;

    d_assert(enb, return CORE_ERROR, "Null param");
    d_assert(enb->s1ap_sock, return CORE_ERROR, "Null param");
    d_assert(message, return CORE_ERROR, "Null param");

    ies = &message->s1ap_S1SetupRequestIEs;
    d_assert(ies, return CORE_ERROR, "Null param");

    s1ap_ENB_ID_to_uint32(&ies->global_ENB_ID.eNB_ID, &enb_id);

#if 0 /* FIXME : does it needed? */
    if (mme_ctx_enb_find_by_enb_id(enb_id))
    {
        S1ap_Cause_t cause;
        d_error("eNB-id[0x%x] duplicated from [%s]", enb_id,
                INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf));

        cause.present = S1ap_Cause_PR_protocol;
        cause.choice.protocol = 
            S1ap_CauseProtocol_message_not_compatible_with_receiver_state;
        rv = s1ap_build_setup_failure(&sendbuf, cause);
    }
#endif

    d_info("eNB[0x%x] sends S1-Setup-Request from [%s]", enb_id,
            INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf));

    enb->enb_id = enb_id;

    d_assert(s1ap_build_setup_rsp(&sendbuf) == CORE_OK, 
            return CORE_ERROR, "build error");
    d_assert(s1ap_send_to_enb(enb, sendbuf) == CORE_OK, 
            return CORE_ERROR, "send error");

    pkbuf_free(sendbuf);

    return CORE_OK;
}

status_t enb_s1ap_handle_initialuemessage(
        enb_ctx_t *enb, s1ap_message *message)
{
    S1ap_InitialUEMessage_IEs_t *ies = NULL;
    ue_ctx_t *ue = NULL;

    d_assert(enb, return CORE_ERROR, "Null param");
    d_assert(message, return CORE_ERROR, "Null param");

    ies = &message->s1ap_InitialUEMessage_IEs;
    d_assert(ies, return CORE_ERROR, "Null param");

    ue = mme_ctx_ue_add(enb);
    d_assert(ue, return CORE_ERROR, "Null param");

    ue->enb_ue_s1ap_id = ies->eNB_UE_S1AP_ID;

    d_info("eNB[0x%x] sends Initial-UE Message[eNB-UE-S1AP-ID(%d)]",
            enb->enb_id, ue->enb_ue_s1ap_id);
#if 0
    fsm_create((fsm_t*)&enb->s1ap_sm, 
            enb_s1ap_state_initial, enb_s1ap_state_final);
    enb->s1ap_sm.ctx = enb;
    enb->s1ap_sm.queue_id = s->queue_id;
    enb->s1ap_sm.tm_service = s->tm_service;
    fsm_init((fsm_t*)&enb->s1ap_sm, 0);
#endif

    return CORE_OK;
}
