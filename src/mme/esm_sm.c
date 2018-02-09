#define TRACE_MODULE _esm_sm

#include "core_debug.h"

#include "nas/nas_message.h"

#include "mme_event.h"
#include "mme_sm.h"
#include "mme_fd_path.h"
#include "emm_handler.h"
#include "esm_build.h"
#include "esm_handler.h"
#include "mme_s11_handler.h"
#include "nas_path.h"
#include "mme_gtp_path.h"

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
                    d_trace(3, "[ESM] PDN Connectivity request\n");
                    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
                            mme_ue->imsi_bcd, sess->pti, bearer->ebi);
                    rv = esm_handle_pdn_connectivity_request(
                            bearer, &message->esm.pdn_connectivity_request);
                    if (rv != CORE_OK)
                    {
                        FSM_TRAN(s, esm_state_exception);
                        break;
                    }
                    break;
                }
                case NAS_ESM_INFORMATION_RESPONSE:
                {
                    d_trace(3, "[ESM] ESM information response\n");
                    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
                            mme_ue->imsi_bcd, sess->pti, bearer->ebi);
                    rv = esm_handle_information_response(
                            sess, &message->esm.esm_information_response);
                    if (rv != CORE_OK)
                    {
                        FSM_TRAN(s, esm_state_exception);
                        break;
                    }
                    break;
                }
                case NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_ACCEPT:
                {
                    d_trace(3, "[ESM] Activate default EPS bearer "
                            "context accept\n");
                    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
                            mme_ue->imsi_bcd, sess->pti, bearer->ebi);
                    /* Check if Initial Context Setup Response or 
                     *          E-RAB Setup Response is received */
                    if (MME_HAVE_ENB_S1U_PATH(bearer))
                    {
                        rv = mme_gtp_send_modify_bearer_request(bearer, 0);
                        d_assert(rv == CORE_OK,, "gtp send failed");
                    }

                    rv = nas_send_activate_all_dedicated_bearers(bearer);
                    d_assert(rv == CORE_OK,, "nas send failed");

                    FSM_TRAN(s, esm_state_active);
                    break;
                }
                case NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_ACCEPT:
                {
                    d_trace(3, "[ESM] Activate dedicated EPS bearer "
                            "context accept\n");
                    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
                            mme_ue->imsi_bcd, sess->pti, bearer->ebi);
                    /* Check if Initial Context Setup Response or 
                     *          E-RAB Setup Response is received */
                    if (MME_HAVE_ENB_S1U_PATH(bearer))
                    {
                        rv = mme_gtp_send_create_bearer_response(bearer);
                        d_assert(rv == CORE_OK,, "gtp send failed");
                    }

                    FSM_TRAN(s, esm_state_active);
                    break;
                }
                default:
                {
                    d_error("Unknown message(type:%d)", 
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
                    d_trace(3, "[ESM] PDN Connectivity request\n");
                    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
                            mme_ue->imsi_bcd, sess->pti, bearer->ebi);
                    rv = esm_handle_pdn_connectivity_request(
                            bearer, &message->esm.pdn_connectivity_request);
                    if (rv != CORE_OK)
                    {
                        FSM_TRAN(s, esm_state_exception);
                        break;
                    }

                    FSM_TRAN(s, esm_state_inactive);
                    break;
                }
                case NAS_PDN_DISCONNECT_REQUEST:
                {
                    d_trace(3, "[ESM] PDN disconnect request\n");
                    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
                            mme_ue->imsi_bcd, sess->pti, bearer->ebi);
                    if (MME_HAVE_SGW_S1U_PATH(sess))
                    {
                        rv = mme_gtp_send_delete_session_request(sess);
                        d_assert(rv == CORE_OK, return,
                                "mme_gtp_send_delete_session_request error");
                    }
                    else
                    {
                        rv = nas_send_deactivate_bearer_context_request(bearer);
                        d_assert(rv == CORE_OK, return,
                        "nas_send_deactivate_bearer_context_request failed");
                    }
                    FSM_TRAN(s, esm_state_pdn_will_disconnect);
                    break;
                }
                case NAS_MODIFY_EPS_BEARER_CONTEXT_ACCEPT:
                {
                    d_trace(3, "[ESM] Modify EPS bearer context accept\n");
                    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
                            mme_ue->imsi_bcd, sess->pti, bearer->ebi);

                    rv = mme_gtp_send_update_bearer_response(bearer);
                    d_assert(rv == CORE_OK, return,
                            "mme_gtp_send_update_session_request error");
                    break;
                }
                case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT:
                {
                    d_trace(3, "[ESM] [A] Deactivate EPS bearer "
                            "context accept\n");
                    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
                            mme_ue->imsi_bcd, sess->pti, bearer->ebi);
                    rv = mme_gtp_send_delete_bearer_response(bearer);
                    d_assert(rv == CORE_OK, return,
                            "mme_gtp_send_delete_session_request error");
                    FSM_TRAN(s, esm_state_bearer_deactivated);
                    break;
                }
                default:
                {
                    d_error("Unknown message(type:%d)", 
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

void esm_state_pdn_will_disconnect(fsm_t *s, event_t *e)
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
                case NAS_DEACTIVATE_EPS_BEARER_CONTEXT_ACCEPT:
                {
                    d_trace(3, "[ESM] [D] Deactivate EPS bearer "
                            "context accept\n");
                    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
                            mme_ue->imsi_bcd, sess->pti, bearer->ebi);
                    FSM_TRAN(s, esm_state_pdn_did_disconnect);
                    break;
                }
                default:
                {
                    d_error("Unknown message(type:%d)", 
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

void esm_state_pdn_did_disconnect(fsm_t *s, event_t *e)
{
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

void esm_state_bearer_deactivated(fsm_t *s, event_t *e)
{
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

void esm_state_exception(fsm_t *s, event_t *e)
{
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
