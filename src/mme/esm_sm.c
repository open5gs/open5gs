#define TRACE_MODULE _esm_sm

#include "core_debug.h"

#include "nas_message.h"

#include "mme_event.h"
#include "mme_fd_path.h"
#include "emm_handler.h"
#include "esm_build.h"
#include "esm_handler.h"
#include "mme_s11_handler.h"
#include "nas_path.h"

static void esm_state_pdn_connectivity_request(
    fsm_t *s, event_t *e, mme_ue_t *mme_ue, mme_sess_t *sess,
    mme_bearer_t *bearer, nas_message_t *message);

void esm_state_initial(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(3, e);

    FSM_TRAN(s, &esm_state_inactive);
}

void esm_state_final(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(3, e);
}

void esm_state_inactive(fsm_t *s, event_t *e)
{
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    bearer = mme_bearer_find(event_get_param1(e));
    d_assert(bearer, return, "Null param");
    sess = bearer->sess;
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
                    esm_state_pdn_connectivity_request(s, e,
                            mme_ue, sess, bearer, message);
                    break;
                }
                case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT:
                {
                    d_trace(3, "[NAS] Activate default EPS bearer "
                            "context accept : UE[%s] --> ESM[%d]\n", 
                            mme_ue->imsi_bcd, bearer->pti);

                    esm_handle_activate_default_bearer_accept(bearer);
                    FSM_TRAN(s, esm_state_active);
                    break;
                }
                case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT:
                {
                    d_trace(3, "[NAS] Activate dedicated EPS bearer "
                            "context accept : UE[%s] --> ESM[%d]\n", 
                            mme_ue->imsi_bcd, bearer->pti);
                    esm_handle_activate_dedicated_bearer_accept(bearer);
                    FSM_TRAN(s, esm_state_active);
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

void esm_state_information(fsm_t *s, event_t *e)
{
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    bearer = mme_bearer_find(event_get_param1(e));
    d_assert(bearer, return, "Null param");
    sess = bearer->sess;
    d_assert(sess, return, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            status_t rv;
            pkbuf_t *esmbuf = NULL;

            rv = esm_build_information_request(&esmbuf, bearer);
            d_assert(rv == CORE_OK && esmbuf, return,
                    "esm_build failed");
            d_assert(nas_send_to_downlink_nas_transport(
                    mme_ue, esmbuf) == CORE_OK,,);
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
                case NAS_ESM_INFORMATION_RESPONSE:
                {
                    d_trace(3, "[NAS] ESM information response : "
                            "UE[%s] --> ESM[%d]\n", 
                            mme_ue->imsi_bcd, bearer->pti);
                    esm_handle_information_response(
                            sess, &message->esm.esm_information_response);

                    mme_s11_handle_create_session_request(sess);
                    FSM_TRAN(s, esm_state_inactive);
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

void esm_state_active(fsm_t *s, event_t *e)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    bearer = mme_bearer_find(event_get_param1(e));
    d_assert(bearer, return, "Null param");
    sess = bearer->sess;
    d_assert(sess, return, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            enb_ue_t *enb_ue = NULL;
            mme_bearer_t *linked_bearer = mme_linked_bearer(bearer);

            d_assert(linked_bearer, return, "Null param");
            enb_ue = mme_ue->enb_ue;
            d_assert(enb_ue, return, "Null param");

            if (linked_bearer->ebi == bearer->ebi)
            {
                /* Check dedicated bearer */
                mme_bearer_t *dedicated_bearer = mme_bearer_next(bearer);
                while(dedicated_bearer)
                {
                    if (!MME_HAVE_ENB_S1U_PATH(dedicated_bearer))
                    {
                        rv = nas_send_activate_dedicated_bearer_context(
                                enb_ue, dedicated_bearer);
                        d_assert(rv == CORE_OK, return,
                        "nas_send_activate_dedicated_bearer_context failed");
                    }

                    dedicated_bearer = mme_bearer_next(dedicated_bearer);
                }
            }
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
                    esm_state_pdn_connectivity_request(s, e,
                            mme_ue, sess, bearer, message);
                    FSM_TRAN(s, esm_state_inactive);
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

void esm_state_session_exception(fsm_t *s, event_t *e)
{
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

void esm_state_bearer_exception(fsm_t *s, event_t *e)
{
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

static void esm_state_pdn_connectivity_request(
    fsm_t *s, event_t *e, mme_ue_t *mme_ue, mme_sess_t *sess,
    mme_bearer_t *bearer, nas_message_t *message)
{
    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");
    d_assert(mme_ue, return, "Null param");
    d_assert(sess, return, "Null param");
    d_assert(bearer, return, "Null param");
    d_assert(message, return, "Null param");

    d_trace(3, "[NAS] PDN connectivity request : UE[%s] --> ESM[%d]\n", 
            mme_ue->imsi_bcd, bearer->pti);

    d_assert(MME_UE_HAVE_IMSI(mme_ue), return,
        "No IMSI in PDN_CPNNECTIVITY_REQUEST");
    d_assert(SECURITY_CONTEXT_IS_VALID(mme_ue), return,
        "No Security Context in PDN_CPNNECTIVITY_REQUEST");

    esm_handle_pdn_connectivity_request(
            sess, &message->esm.pdn_connectivity_request);

    if (MME_UE_HAVE_APN(mme_ue))
    {
        if (FSM_CHECK(&mme_ue->sm, emm_state_attached))
        {
            mme_s11_handle_create_session_request(sess);
        }
        else
        {
            if (MME_HAVE_SGW_S11_PATH(mme_ue))
            {
                status_t rv;
                rv = nas_send_attach_accept(mme_ue);
                d_assert(rv == CORE_OK, return,
                        "nas_send_attach_accept failed");
            }
            else
            {
                mme_s11_handle_create_session_request(sess);
            }
        }
    }
    else
    {
        FSM_TRAN(s, esm_state_information);
    }
}

