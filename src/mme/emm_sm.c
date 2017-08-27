#define TRACE_MODULE _emm_sm

#include "core_debug.h"

#include "nas_message.h"
#include "fd_lib.h"
#include "s6a_message.h"

#include "mme_event.h"
#include "mme_kdf.h"
#include "s1ap_handler.h"
#include "mme_fd_path.h"
#include "emm_handler.h"
#include "emm_build.h"
#include "esm_handler.h"
#include "mme_s11_handler.h"

void emm_state_initial(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(3, e);

    FSM_TRAN(s, &emm_state_operational);
}

void emm_state_final(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(3, e);
}

void emm_state_operational(fsm_t *s, event_t *e)
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
        case MME_EVT_EMM_MESSAGE:
        {
            index_t index = event_get_param1(e);
            mme_ue_t *mme_ue = NULL;
            nas_message_t *message = NULL;

            d_assert(index, return, "Null param");
            mme_ue = mme_ue_find(index);
            d_assert(mme_ue, return, "Null param");

            message = (nas_message_t *)event_get_param4(e);
            d_assert(message, break, "Null param");

            if (message->emm.h.security_header_type
                    == NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE)
            {
                /* Update Kenb */
                if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                    mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, 
                            mme_ue->kenb);

                mme_ue_paged(mme_ue);
                emm_handle_service_request(
                        mme_ue, &message->emm.service_request);

                break;
            }

            switch(message->emm.h.message_type)
            {
                case NAS_ATTACH_REQUEST:
                {
                    /* Update Kenb */
                    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                        mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, 
                                mme_ue->kenb);

                    CLEAR_EPS_BEARER_ID(mme_ue);

                    mme_ue_paged(mme_ue);
                    emm_handle_attach_request(
                            mme_ue, &message->emm.attach_request);

                    break;
                }
                case NAS_IDENTITY_RESPONSE:
                {
                    mme_sess_t *sess = NULL;

                    emm_handle_identity_response(mme_ue,
                            &message->emm.identity_response);

                    if (!MME_UE_HAVE_IMSI(mme_ue))
                    {
                        d_error("Cannot find IMSI");
                        break;
                    }
                    
                    sess = mme_sess_first(mme_ue);
                    d_assert(sess, return, "Null param");

                    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                    {
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

                    }
                    else
                    {
                        if (MME_UE_HAVE_SESSION(mme_ue))
                        {
                            mme_s11_handle_delete_all_sessions_request_in_ue(
                                    mme_ue);
                        }
                        else
                        {
                            mme_s6a_send_air(mme_ue);
                        }
                    }

                    break;
                }
                case NAS_AUTHENTICATION_RESPONSE:
                {
                    emm_handle_authentication_response(
                            mme_ue, &message->emm.authentication_response);
                    break;
                }
                case NAS_SECURITY_MODE_COMPLETE:
                {
                    d_trace(3, "[NAS] Security mode complete : "
                            "UE[%s] --> EMM\n", mme_ue->imsi_bcd);

                    /* Update Kenb */
                    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                        mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, 
                                mme_ue->kenb);

                    mme_s6a_send_ulr(mme_ue);
                    break;
                }
                case NAS_ATTACH_COMPLETE:
                {
                    d_trace(3, "[NAS] Attach complete : UE[%s] --> EMM\n",
                            mme_ue->imsi_bcd);
                    emm_handle_attach_complete(
                            mme_ue, &message->emm.attach_complete);
                    break;
                }
                case NAS_EMM_STATUS:
                {
                    emm_handle_emm_status(mme_ue, &message->emm.emm_status);
                    break;
                }
                case NAS_DETACH_REQUEST:
                {
                    emm_handle_detach_request(
                            mme_ue, &message->emm.detach_request_from_ue);
    
                    if (MME_UE_HAVE_SESSION(mme_ue))
                    {
                        mme_s11_handle_delete_all_sessions_request_in_ue(
                            mme_ue);
                    }
                    else
                    {
                        emm_handle_detach_accept(mme_ue);
                    }
                    break;
                }
                case NAS_TRACKING_AREA_UPDATE_REQUEST:
                {
                    mme_ue_paged(mme_ue);
                    emm_handle_tau_request(
                            mme_ue, &message->emm.tracking_area_update_request);
                    break;
                }
                default:
                {
                    d_warn("Not implemented(type:%d)", 
                            message->emm.h.message_type);
                    break;
                }
            }
            break;
        }
        case MME_EVT_EMM_T3413:
        {
            index_t index = event_get_param1(e);
            mme_ue_t *mme_ue = NULL;

            d_assert(index, return, "Null param");
            mme_ue = mme_ue_find(index);
            d_assert(mme_ue, return, "Null param");

            if (mme_ue->max_paging_retry >= MAX_NUM_OF_PAGING)
            {
                /* Paging failed */
                d_warn("Paging to UE(%s) failed. Stop paging",
                        mme_ue->imsi_bcd);
                if (mme_ue->last_paging_msg)
                {
                    pkbuf_free(mme_ue->last_paging_msg);
                    mme_ue->last_paging_msg = NULL;
                }
                break;
            }

            mme_ue->max_paging_retry++;
            s1ap_handle_paging(mme_ue);
            /* Start T3413 */
            tm_start(mme_ue->t3413);

            break;
        }
        default:
        {
            d_error("Unknown event %s", mme_event_get_name(e));
            break;
        }
    }
}

void emm_state_exception(fsm_t *s, event_t *e)
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
