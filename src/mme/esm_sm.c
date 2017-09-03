#define TRACE_MODULE _esm_sm

#include "core_debug.h"

#include "nas_message.h"

#include "mme_event.h"
#include "mme_fd_path.h"
#include "emm_handler.h"
#include "esm_build.h"
#include "esm_handler.h"
#include "mme_s11_handler.h"

void esm_state_initial(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(3, e);

    FSM_TRAN(s, &esm_state_operational);
}

void esm_state_final(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(3, e);
}

void esm_state_operational(fsm_t *s, event_t *e)
{
    mme_sess_t *sess = NULL;
    mme_ue_t *mme_ue = NULL;

    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    sess = mme_sess_find(event_get_param1(e));
    d_assert(sess, return, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return, "Null param");

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
        case MME_EVT_ESM_MESSAGE:
        {
            nas_message_t *message = (nas_message_t *)event_get_param3(e);
            d_assert(message, break, "Null param");

            switch(message->esm.h.message_type)
            {
                case NAS_PDN_CONNECTIVITY_REQUEST:
                {
                    esm_handle_pdn_connectivity_request(
                            sess, &message->esm.pdn_connectivity_request);
                    d_trace(3, "[NAS] PDN connectivity request : "
                            "UE[%s] --> ESM[%d]\n", 
                            mme_ue->imsi_bcd, sess->pti);

                    d_assert(MME_UE_HAVE_IMSI(mme_ue), break,
                        "No IMSI in PDN_CPNNECTIVITY_REQUEST");
                    d_assert(SECURITY_CONTEXT_IS_VALID(mme_ue), break,
                        "No Security Context in PDN_CPNNECTIVITY_REQUEST");

                    if (MME_UE_HAVE_APN(mme_ue))
                    {
                        if (MME_UE_HAVE_SESSION(mme_ue))
                        {
                            emm_handle_attach_accept(mme_ue);
                        }
                        else
                        {
                            mme_s11_handle_create_session_request(sess);
                        }
                    }
                    else
                    {
                        esm_handle_information_request(sess);
                    }
                    break;
                }
                case NAS_ESM_INFORMATION_RESPONSE:
                {
                    d_trace(3, "[NAS] ESM information response : "
                            "UE[%s] --> ESM[%d]\n", 
                            mme_ue->imsi_bcd, sess->pti);
                    esm_handle_information_response(
                            sess, &message->esm.esm_information_response);

                    mme_s11_handle_create_session_request(sess);
                    break;
                }
                case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT:
                {
                    d_trace(3, "[NAS] Activate default eps bearer "
                            "context accept : UE[%s] --> ESM[%d]\n", 
                            mme_ue->imsi_bcd, sess->pti);
                    break;
                }
                default:
                {
                    d_warn("Not implemented(type:%d)", 
                            message->esm.h.message_type);
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

void esm_state_exception(fsm_t *s, event_t *e)
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
