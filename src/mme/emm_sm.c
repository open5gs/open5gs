#define TRACE_MODULE _emm_sm

#include "core_debug.h"

#include "nas_message.h"
#include "fd_lib.h"

#include "mme_event.h"
#include "mme_kdf.h"
#include "s1ap_handler.h"
#include "mme_s6a_handler.h"
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
        case MME_EVT_EMM_UE_FROM_S6A:
        {
            index_t index = event_get_param1(e);
            mme_ue_t *mme_ue = NULL;

            d_assert(index, return, "Null param");
            mme_ue = mme_ue_find(index);
            d_assert(mme_ue, return, "Null param");

            switch(event_get_param2(e))
            {
                case S6A_CMD_AUTHENTICATION_INFORMATION:
                {
                    c_uint32_t result_code = event_get_param3(e);
                    if (result_code != ER_DIAMETER_SUCCESS)
                    {
                        /* TODO */
                        /* Send Attach Reject */
                        return;
                    }

                    emm_handle_authentication_request(mme_ue);
                    break;
                }
                case S6A_CMD_UPDATE_LOCATION:
                {
                    mme_sess_t *sess = NULL;
                    mme_bearer_t *bearer = NULL;

                    c_uint32_t result_code = event_get_param3(e);
                    if (result_code != ER_DIAMETER_SUCCESS)
                    {
                        /* TODO */
                        return;
                    }

                    sess = mme_sess_find_by_last_esm_message(mme_ue);
                    d_assert(sess, return, "Null param");
                    bearer = mme_default_bearer_in_sess(sess);
                    d_assert(bearer, return, "Null param");

                    if (MME_SESSION_HAVE_APN(sess))
                    {
                        if (MME_SESSION_IS_VALID(sess))
                        {
                            emm_handle_attach_accept(mme_ue);
                        }
                        else
                        {
                            mme_s11_handle_create_session_request(bearer);
                        }
                    }
                    else
                    {
                        esm_handle_information_request(sess);
                    }

                    break;
                }
            }
            break;
        }
        case MME_EVT_EMM_UE_MSG:
        {
            index_t index = event_get_param1(e);
            mme_ue_t *mme_ue = NULL;
            nas_message_t *message = NULL;

            d_assert(index, return, "Null param");
            mme_ue = mme_ue_find(index);
            d_assert(mme_ue, return, "Null param");

            message = (nas_message_t *)event_get_param4(e);
            d_assert(message, break, "Null param");

            /* Save Last Received NAS-EMM message */
            memcpy(&mme_ue->last_emm_message, message, sizeof(nas_message_t));

            if (message->emm.h.security_header_type
                    == NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE)
            {
                mme_ue_paged(mme_ue);
                emm_handle_service_request(
                        mme_ue, &message->emm.service_request);

                /* Update Kenb */
                if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                    mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, 
                            mme_ue->kenb);

                break;
            }

            switch(message->emm.h.message_type)
            {
                case NAS_ATTACH_REQUEST:
                {
                    mme_ue_paged(mme_ue);
                    emm_handle_attach_request(
                            mme_ue, &message->emm.attach_request);

                    /* Update Kenb */
                    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                        mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, 
                                mme_ue->kenb);
                    break;
                }
                case NAS_IDENTITY_RESPONSE:
                {
                    mme_sess_t *sess = NULL;
                    mme_bearer_t *bearer = NULL;

                    emm_handle_identity_response(mme_ue,
                            &message->emm.identity_response);

                    if (!MME_UE_HAVE_IMSI(mme_ue))
                    {
                        d_error("Cannot find IMSI");
                        break;
                    }
                    
                    sess = mme_sess_find_by_last_esm_message(mme_ue);
                    d_assert(sess, return, "Null param");
                    bearer = mme_default_bearer_in_sess(sess);
                    d_assert(bearer, return, "Null param");

                    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                    {
                        if (MME_SESSION_HAVE_APN(sess))
                        {
                            if (MME_SESSION_IS_VALID(sess))
                            {
                                emm_handle_attach_accept(mme_ue);
                            }
                            else
                            {
                                mme_s11_handle_create_session_request(bearer);
                            }
                        }
                        else
                        {
                            esm_handle_information_request(sess);
                        }

                    }
                    else
                    {
                        if (MME_SESSION_WAS_CREATED(mme_ue))
                        {
                             emm_handle_s11_delete_session_request(mme_ue);
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
                    break;
                }
                case NAS_TRACKING_AREA_UPDATE_REQUEST:
                {
                    mme_ue_paged(mme_ue);
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
        case MME_EVT_EMM_UE_T3413:
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
