#define TRACE_MODULE _emm_sm

#include "core_debug.h"

#include "nas/nas_message.h"
#include "fd/fd_lib.h"
#include "fd/s6a/s6a_message.h"

#include "mme_event.h"
#include "mme_kdf.h"
#include "s1ap_handler.h"
#include "mme_fd_path.h"
#include "emm_handler.h"
#include "emm_build.h"
#include "esm_handler.h"
#include "nas_path.h"
#include "s1ap_path.h"
#include "mme_gtp_path.h"
#include "mme_sm.h"

void emm_state_initial(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(3, e);

    FSM_TRAN(s, &emm_state_detached);
}

void emm_state_final(fsm_t *s, event_t *e)
{
    d_assert(s, return, "Null param");

    mme_sm_trace(3, e);
}

void emm_state_detached(fsm_t *s, event_t *e)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;

    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    mme_ue = mme_ue_find(event_get_param1(e));
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
        case MME_EVT_EMM_MESSAGE:
        {
            nas_message_t *message = (nas_message_t *)event_get_param4(e);
            d_assert(message, break, "Null param");

            if (message->emm.h.security_header_type
                    == NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE)
            {
                rv = emm_handle_service_request(
                        mme_ue, &message->emm.service_request);
                if (rv != CORE_OK)
                {
                    d_error("emm_handle_service_request() failed "
                            "in emm_state_detached");
                    FSM_TRAN(s, emm_state_exception);
                    break;
                }

                if (MME_UE_HAVE_IMSI(mme_ue))
                {
                    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                    {
                        rv = s1ap_send_initial_context_setup_request(mme_ue);
                        d_assert(rv == CORE_OK,, "s1ap send error");
                    }
                    else
                    {
                        d_error("No Security Context");
                        nas_send_service_reject(mme_ue, EMM_CAUSE_MAC_FAILURE);
                        FSM_TRAN(s, &emm_state_exception);
                    }
                }
                else
                {
                    d_error("Unknown UE");
                    nas_send_service_reject(mme_ue, 
                        EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                    FSM_TRAN(s, emm_state_exception);
                }
                break;
            }

            switch(message->emm.h.message_type)
            {
                case NAS_ATTACH_REQUEST:
                {
                    rv = emm_handle_attach_request(
                            mme_ue, &message->emm.attach_request);
                    if (rv != CORE_OK)
                    {
                        d_error("emm_handle_attach_request() failed "
                                "in emm_state_detached");
                        FSM_TRAN(s, emm_state_exception);
                        break;
                    }

                    if (MME_UE_HAVE_IMSI(mme_ue))
                    {
                        if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                        {
                            rv = nas_send_emm_to_esm(
                                    mme_ue, &mme_ue->pdn_connectivity_request);
                            d_assert(rv == CORE_OK,,
                                    "nas_send_emm_to_esm failed");
                            FSM_TRAN(s, &emm_state_initial_context_setup);
                        }
                        else
                        {
                            if (MME_HAVE_SGW_S11_PATH(mme_ue))
                            {
                                rv = mme_gtp_send_delete_all_sessions(mme_ue);
                                d_assert(rv == CORE_OK, ,, "gtp send failed");
                            }
                            else
                            {
                                mme_s6a_send_air(mme_ue, NULL);
                            }
                            FSM_TRAN(s, &emm_state_authentication);
                        }
                    }
                    else
                    {
                        FSM_TRAN(s, &emm_state_identity);
                    }
    
                    break;
                }

                case NAS_TRACKING_AREA_UPDATE_REQUEST:
                {
                    rv = emm_handle_tau_request(
                            mme_ue, &message->emm.tracking_area_update_request);
                    if (rv != CORE_OK)
                    {
                        d_error("emm_handle_tau_request() failed"
                                "in emm_state_detached");
                        FSM_TRAN(s, emm_state_exception);
                        break;
                    }

                    if (MME_UE_HAVE_IMSI(mme_ue))
                    {
                        if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                        {
                            /* Send TAU Accept */
                            rv = nas_send_tau_accept(mme_ue);
                            d_assert(rv == CORE_OK,, "send send failed");
                        }
                        else
                        {
                            /* Send TAU reject */
                            d_error("No Security Context");
                            nas_send_tau_reject(mme_ue, EMM_CAUSE_MAC_FAILURE);
                            FSM_TRAN(s, emm_state_exception);
                        }
                    }
                    else
                    {
                        /* Send TAU reject */
                        d_error("Unknown UE");
                        nas_send_tau_reject(mme_ue, 
                            EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                        FSM_TRAN(s, emm_state_exception);
                    }
                    break;
                }

                default:
                {
                    d_warn("Unknown message type = %d", 
                            message->emm.h.message_type);
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

void emm_state_identity(fsm_t *s, event_t *e)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;

    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    mme_ue = mme_ue_find(event_get_param1(e));
    d_assert(mme_ue, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            pkbuf_t *emmbuf = NULL;

            rv = emm_build_identity_request(&emmbuf, mme_ue);
            d_assert(rv == CORE_OK && emmbuf, break, "emm build error");

            rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
            d_assert(rv == CORE_OK && emmbuf, break, "emm send error");

            break;
        }
        case FSM_EXIT_SIG:
        {
            break;
        }
        case MME_EVT_EMM_MESSAGE:
        {
            nas_message_t *message = (nas_message_t *)event_get_param4(e);
            d_assert(message, break, "Null param");

            switch(message->emm.h.message_type)
            {
                case NAS_IDENTITY_RESPONSE:
                {
                    rv = emm_handle_identity_response(mme_ue,
                            &message->emm.identity_response);
                    if (rv != CORE_OK)
                    {
                        d_error("emm_handle_identity_response failed() "
                                "in emm_state_identity");
                        FSM_TRAN(s, emm_state_exception);
                        break;
                    }

                    if (mme_ue->nas_eps.type == MME_EPS_TYPE_ATTACH_REQUEST)
                    {
                        if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                        {
                            FSM_TRAN(&mme_ue->sm,
                                    &emm_state_initial_context_setup);
                        }
                        else
                        {
                            FSM_TRAN(&mme_ue->sm, &emm_state_authentication);
                        }
                    }
                    else if (mme_ue->nas_eps.type ==
                                MME_EPS_TYPE_SERVICE_REQUEST ||
                            mme_ue->nas_eps.type == MME_EPS_TYPE_TAU_REQUEST)
                    {
                        d_assert(0,,
                                "Invalid EPS Type : %d", mme_ue->nas_eps.type);
                        FSM_TRAN(s, &emm_state_exception);
                    }
                    break;
                }
                case NAS_EMM_STATUS:
                {
                    d_warn("[NAS] EMM STATUS [IMSI:%s,Cause:%d] "
                            "in emm_state_identity",
                            mme_ue->imsi_bcd,
                            message->emm.emm_status.emm_cause);
                    FSM_TRAN(s, &emm_state_exception);
                    break;
                }
                default:
                {
                    d_warn("Unknown message(type:%d)", 
                            message->emm.h.message_type);
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

void emm_state_authentication(fsm_t *s, event_t *e)
{
    mme_ue_t *mme_ue = NULL;

    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    mme_ue = mme_ue_find(event_get_param1(e));
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
        case MME_EVT_EMM_MESSAGE:
        {
            nas_message_t *message = (nas_message_t *)event_get_param4(e);
            d_assert(message, break, "Null param");

            switch(message->emm.h.message_type)
            {
                case NAS_AUTHENTICATION_RESPONSE:
                {
                    nas_authentication_response_t *authentication_response =
                        &message->emm.authentication_response;
                    nas_authentication_response_parameter_t
                        *authentication_response_parameter =
                            &authentication_response->
                                authentication_response_parameter;

                    if (authentication_response_parameter->length != 
                            mme_ue->xres_len ||
                        memcmp(authentication_response_parameter->res,
                            mme_ue->xres, mme_ue->xres_len) != 0)
                    {
                        status_t rv;

                        d_warn("[NAS] Authentication failure [IMSI:%s]",
                                mme_ue->imsi_bcd);

                        rv = nas_send_authentication_reject(mme_ue);
                        d_assert(rv == CORE_OK,, "nas send error");
                        FSM_TRAN(&mme_ue->sm, &emm_state_exception);
                    }
                    else
                    {
                        FSM_TRAN(&mme_ue->sm, &emm_state_security_mode);
                    }
                    break;
                }
                case NAS_AUTHENTICATION_FAILURE:
                {
                    nas_authentication_failure_t *authentication_failure =
                        &message->emm.authentication_failure;
                    nas_authentication_failure_parameter_t
                        *authentication_failure_parameter = 
                            &authentication_failure->
                                authentication_failure_parameter;

                    mme_s6a_send_air(mme_ue, authentication_failure_parameter);
                    break;
                }
                case NAS_EMM_STATUS:
                {
                    d_warn("[NAS] EMM STATUS [IMSI:%s,Cause:%d] "
                            "in emm_state_authentication",
                            mme_ue->imsi_bcd,
                            message->emm.emm_status.emm_cause);
                    FSM_TRAN(s, &emm_state_exception);
                    break;
                }
                default:
                {
                    d_warn("Unknown message(type:%d)", 
                            message->emm.h.message_type);
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

void emm_state_security_mode(fsm_t *s, event_t *e)
{
    mme_ue_t *mme_ue = NULL;

    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    mme_ue = mme_ue_find(event_get_param1(e));
    d_assert(mme_ue, return, "Null param");

    switch (event_get(e))
    {
        case FSM_ENTRY_SIG:
        {
            status_t rv;
            pkbuf_t *emmbuf = NULL;

            rv = emm_build_security_mode_command(&emmbuf, mme_ue);
            d_assert(rv == CORE_OK && emmbuf, break, "emm build error");

            rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
            d_assert(rv == CORE_OK && emmbuf, break, "emm send error");

            d_trace(3, "[NAS] Security mode command : UE[%s] <-- EMM\n", 
                    mme_ue->imsi_bcd);
            break;
        }
        case FSM_EXIT_SIG:
        {
            break;
        }
        case MME_EVT_EMM_MESSAGE:
        {
            nas_message_t *message = (nas_message_t *)event_get_param4(e);
            d_assert(message, break, "Null param");

            switch(message->emm.h.message_type)
            {
                case NAS_SECURITY_MODE_COMPLETE:
                {
                    d_trace(3, "[NAS] Security mode complete : "
                            "UE[%s] --> EMM\n", mme_ue->imsi_bcd);

                    /* Update Kenb */
                    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                    {
                        mme_kdf_enb(mme_ue->kasme, mme_ue->ul_count.i32, 
                                mme_ue->kenb);
                        mme_kdf_nh(mme_ue->kasme, mme_ue->kenb, mme_ue->nh);
                        mme_ue->nhcc = 1;
                    }

                    mme_s6a_send_ulr(mme_ue);
                    FSM_TRAN(s, &emm_state_initial_context_setup);
                    break;
                }
                case NAS_SECURITY_MODE_REJECT:
                {
                    d_warn("[NAS] Security mode reject [IMSI:%s,Cause:%d] "
                            "UE[%s] --> EMM\n", mme_ue->imsi_bcd,
                            message->emm.security_mode_reject.emm_cause);
                    FSM_TRAN(s, &emm_state_exception);
                    break;
                }
                case NAS_EMM_STATUS:
                {
                    d_warn("[NAS] EMM STATUS [IMSI:%s,Cause:%d] "
                            "in emm_state_security_mode",
                            mme_ue->imsi_bcd,
                            message->emm.emm_status.emm_cause);
                    FSM_TRAN(s, &emm_state_exception);
                    break;
                }
                default:
                {
                    d_warn("Unknown message(type:%d)", 
                            message->emm.h.message_type);
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

void emm_state_initial_context_setup(fsm_t *s, event_t *e)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;

    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    mme_ue = mme_ue_find(event_get_param1(e));
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
        case MME_EVT_EMM_MESSAGE:
        {
            nas_message_t *message = (nas_message_t *)event_get_param4(e);
            d_assert(message, break, "Null param");

            switch(message->emm.h.message_type)
            {
                case NAS_ATTACH_COMPLETE:
                {
                    d_trace(3, "[NAS] Attach complete : UE[%s] --> EMM\n",
                            mme_ue->imsi_bcd);
                    rv = emm_handle_attach_complete(
                            mme_ue, &message->emm.attach_complete);
                    if (rv != CORE_OK)
                    {
                        d_error("emm_handle_attach_complete() failed "
                                "in emm_state_initial_context_setup");
                        FSM_TRAN(s, emm_state_exception);
                        break;
                    }
                    FSM_TRAN(s, &emm_state_attached);
                    break;
                }
#if 0 /* This should be removed */
                case NAS_TRACKING_AREA_UPDATE_COMPLETE:
                {
                    status_t rv;
                    S1ap_Cause_t cause;
                    enb_ue_t *enb_ue = mme_ue->enb_ue;

                    d_trace(3, "[NAS] Tracking area update complete : "
                            "UE[%s] --> EMM\n",
                            mme_ue->imsi_bcd);
                    d_assert(enb_ue, return, "Null param");

                    cause.present = S1ap_Cause_PR_nas;
                    cause.choice.nas = S1ap_CauseNas_normal_release;

                    rv = s1ap_send_ue_context_release_commmand(
                            enb_ue, &cause, S1AP_UE_CTX_REL_NO_ACTION, 0);
                    d_assert(rv == CORE_OK, return, "s1ap send error");
                    FSM_TRAN(s, &emm_state_attached);
                    break;
                }
#endif
                case NAS_EMM_STATUS:
                {
                    d_warn("[NAS] EMM STATUS [IMSI:%s,Cause:%d] "
                            "in emm_state_initial_context_setup",
                            mme_ue->imsi_bcd,
                            message->emm.emm_status.emm_cause);
                    FSM_TRAN(s, &emm_state_exception);
                    break;
                }
                default:
                {
                    d_warn("Unknown message(type:%d)", 
                            message->emm.h.message_type);
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

void emm_state_attached(fsm_t *s, event_t *e)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;

    d_assert(s, return, "Null param");
    d_assert(e, return, "Null param");

    mme_sm_trace(3, e);

    mme_ue = mme_ue_find(event_get_param1(e));
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
        case MME_EVT_EMM_MESSAGE:
        {
            nas_message_t *message = (nas_message_t *)event_get_param4(e);
            d_assert(message, break, "Null param");

            if (message->emm.h.security_header_type
                    == NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE)
            {
                rv = emm_handle_service_request(
                        mme_ue, &message->emm.service_request);
                if (rv != CORE_OK)
                {
                    d_error("emm_handle_service_request() failed "
                            "in emm_state_attached");
                    FSM_TRAN(s, emm_state_exception);
                    break;
                }

                if (MME_UE_HAVE_IMSI(mme_ue))
                {
                    if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                    {
                        rv = s1ap_send_initial_context_setup_request(mme_ue);
                        d_assert(rv == CORE_OK,, "s1ap send error");
                    }
                    else
                    {
                        d_error("No Secuity Context");
                        nas_send_service_reject(mme_ue, EMM_CAUSE_MAC_FAILURE);
                        FSM_TRAN(&mme_ue->sm, &emm_state_exception);
                    }
                }
                else
                {
                    d_error("Unknown UE");
                    nas_send_service_reject(mme_ue, 
                        EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                    FSM_TRAN(s, emm_state_exception);
                }
                break;
            }

            switch(message->emm.h.message_type)
            {
                case NAS_ATTACH_REQUEST:
                {
                    rv = emm_handle_attach_request(
                            mme_ue, &message->emm.attach_request);
                    if (rv != CORE_OK)
                    {
                        d_error("emm_handle_attach_request() failed "
                                "in emm_state_attached");
                        FSM_TRAN(s, emm_state_exception);
                        break;
                    }

                    if (MME_UE_HAVE_IMSI(mme_ue))
                    {
                        if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                        {
                            rv = nas_send_emm_to_esm(
                                    mme_ue, &mme_ue->pdn_connectivity_request);
                            d_assert(rv == CORE_OK,,
                                    "nas_send_emm_to_esm failed");
                            FSM_TRAN(s, &emm_state_initial_context_setup);
                        }
                        else
                        {
                            if (MME_HAVE_SGW_S11_PATH(mme_ue))
                            {
                                rv = mme_gtp_send_delete_all_sessions(mme_ue);
                                d_assert(rv == CORE_OK, ,, "gtp send failed");
                            }
                            else
                            {
                                mme_s6a_send_air(mme_ue, NULL);
                            }
                            FSM_TRAN(s, &emm_state_authentication);
                        }
                    }
                    else
                    {
                        FSM_TRAN(s, &emm_state_identity);
                    }
                    break;
                }
                case NAS_EMM_STATUS:
                {
                    d_warn("[NAS] EMM STATUS [IMSI:%s,Cause:%d] "
                            "in emm_state_attached",
                            mme_ue->imsi_bcd,
                            message->emm.emm_status.emm_cause);
                    FSM_TRAN(s, &emm_state_exception);
                    break;
                }
                case NAS_DETACH_REQUEST:
                {
                    emm_handle_detach_request(
                            mme_ue, &message->emm.detach_request_from_ue);
                    FSM_TRAN(s, &emm_state_detached);
                    break;
                }
                case NAS_TRACKING_AREA_UPDATE_REQUEST:
                {
                    rv = emm_handle_tau_request(
                            mme_ue, &message->emm.tracking_area_update_request);
                    if (rv != CORE_OK)
                    {
                        d_error("emm_handle_tau_request() failed "
                                "in emm_state_attached");
                        FSM_TRAN(s, emm_state_exception);
                        break;
                    }

                    if (MME_UE_HAVE_IMSI(mme_ue))
                    {
                        if (SECURITY_CONTEXT_IS_VALID(mme_ue))
                        {
                            /* Send TAU Accept */
                            rv = nas_send_tau_accept(mme_ue);
                            d_assert(rv == CORE_OK,, "send send failed");
                        }
                        else
                        {
                            /* Send TAU reject */
                            d_error("No Security Context");
                            nas_send_tau_reject(mme_ue, EMM_CAUSE_MAC_FAILURE);
                            FSM_TRAN(s, emm_state_exception);
                        }
                    }
                    else
                    {
                        /* Send TAU reject */
                        d_error("Unknown UE");
                        nas_send_tau_reject(mme_ue, 
                            EMM_CAUSE_UE_IDENTITY_CANNOT_BE_DERIVED_BY_THE_NETWORK);
                        FSM_TRAN(s, emm_state_exception);
                    }
                    break;
                }
                default:
                {
                    d_warn("Unknown message(type:%d)", 
                            message->emm.h.message_type);
                    break;
                }
            }
            break;
        }
        case MME_EVT_EMM_T3413:
        {
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
