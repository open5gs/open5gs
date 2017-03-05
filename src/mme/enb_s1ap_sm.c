/**
 * @file enb_s1_state.c
 */

/* Server */
#include "sm.h"
#include "context.h"
#include "event.h"

/* Core libaray */
#define TRACE_MODULE _enb_s1_sm
#include "core_debug.h"

#include "s1ap_build.h"
#include "s1ap_conv.h"
#include "s1ap_path.h"

status_t enb_s1_handle_s1setuprequest(enb_ctx_t *enb, s1ap_message *message);

void enb_s1_state_initial(enb_s1_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    sm_trace(1, e);

    FSM_TRAN(s, &enb_s1_state_operational);
}

void enb_s1_state_final(enb_s1_sm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    sm_trace(1, e);
}

void enb_s1_state_operational(enb_s1_sm_t *s, event_t *e)
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
                            d_info("S1-Setup-Request is received");
                            enb_s1_handle_s1setuprequest(enb, &message);
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

void enb_s1_state_exception(enb_s1_sm_t *s, event_t *e)
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

status_t enb_s1_handle_s1setuprequest(enb_ctx_t *enb, s1ap_message *message)
{
    char buf[INET_ADDRSTRLEN];

    S1ap_S1SetupRequestIEs_t *ies = NULL;
    status_t rv;
    pkbuf_t *sendbuf = NULL;
    c_uint32_t enb_id;

    d_assert(enb, return CORE_ERROR, "Null param");
    d_assert(enb->s1_sock, return CORE_ERROR, "Null param");
    d_assert(message, return CORE_ERROR, "Null param");

    ies = &message->msg.s1ap_S1SetupRequestIEs;
    d_assert(ies, return CORE_ERROR, "Null param");

    s1ap_ENB_ID_to_uint32(&ies->global_ENB_ID.eNB_ID, &enb_id);
    if (mme_ctx_enb_find_by_id(enb_id))
    {
        S1ap_Cause_t cause;
        d_error("eNB-id[0x%x] duplicated from [%s]", enb_id,
                INET_NTOP(&enb->s1_sock->remote.sin_addr.s_addr, buf));

        cause.present = S1ap_Cause_PR_protocol;
        cause.choice.protocol = 
            S1ap_CauseProtocol_message_not_compatible_with_receiver_state;
        rv = s1ap_build_setup_failure(&sendbuf, cause);
    }
    else
    {
        d_info("eNB-id[0x%x] sends S1-Setup-Request from [%s]", enb_id,
                INET_NTOP(&enb->s1_sock->remote.sin_addr.s_addr, buf));

        enb->id = enb_id;
        rv = s1ap_build_setup_rsp(&sendbuf);
    }

    if (rv != CORE_OK) 
    {
        d_error("Can't build S1-Setup-Response/Failure");
        return CORE_ERROR;
    }

    rv = s1ap_send_to_enb(enb, sendbuf);
    if (rv != CORE_OK) 
    {
        d_error("Can't send S1 Setup Response");
    }

    pkbuf_free(sendbuf);

    return CORE_OK;
}

