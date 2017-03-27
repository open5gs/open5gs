#define TRACE_MODULE _s1ap_handler

#include "core_debug.h"

#include "event.h"

#include "s1ap_conv.h"
#include "s1ap_build.h"
#include "s1ap_handler.h"
#include "s1ap_path.h"

void s1ap_handle_s1_setup_request(enb_ctx_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    S1ap_S1SetupRequestIEs_t *ies = NULL;
    pkbuf_t *sendbuf = NULL;
    c_uint32_t enb_id;

    d_assert(enb, return, "Null param");
    d_assert(enb->s1ap_sock, return, "Null param");
    d_assert(message, return, "Null param");

    ies = &message->s1ap_S1SetupRequestIEs;
    d_assert(ies, return, "Null param");

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
    d_assert(enb->s1ap_sock, return,);
    d_info("[S1AP] S1SetupRequest : eNB[%s:%d] --> MME",
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb_id);

    enb->enb_id = enb_id;

    d_assert(s1ap_build_setup_rsp(&sendbuf) == CORE_OK, 
            return, "build error");
    d_assert(s1ap_send_to_enb(enb, sendbuf) == CORE_OK, , "send error");

    d_assert(enb->s1ap_sock, return,);
    d_info("[S1AP] S1SetupResponse: eNB[%s:%d] <-- MME",
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb_id);
}

void s1ap_handle_initial_ue_message(enb_ctx_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    ue_ctx_t *ue = NULL;
    S1ap_InitialUEMessage_IEs_t *ies = NULL;

    d_assert(enb, return, "Null param");

    ies = &message->s1ap_InitialUEMessage_IEs;
    d_assert(ies, return, "Null param");

    ue = mme_ctx_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    if (!ue)
    {
        ue = mme_ctx_ue_add(enb);
        d_assert(ue, return, "Null param");

        ue->enb_ue_s1ap_id = ies->eNB_UE_S1AP_ID;
    }
    else
    {
        d_warn("Duplicated: eNB[0x%x] sends "
            "Initial-UE Message[eNB-UE-S1AP-ID(%d)]",
            enb->enb_id, ue->enb_ue_s1ap_id);
    }

    d_assert(enb->s1ap_sock, mme_ctx_ue_remove(ue);return,);
    d_info("[S1AP] InitialUEMessage : UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
        ue->enb_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);

    fsm_create((fsm_t*)&ue->emm_sm, 
            emm_state_initial, emm_state_final);
    ue->emm_sm.ctx = ue;
    fsm_init((fsm_t*)&ue->emm_sm, 0);

    fsm_create((fsm_t*)&ue->esm_sm, 
            esm_state_initial, esm_state_final);
    ue->esm_sm.ctx = ue;
    fsm_init((fsm_t*)&ue->esm_sm, 0);

    /* FIXME */
    {
        s11_ctx_t *s11 = mme_ctx_s11_add(ue);
        d_assert(s11, return, "Null param");
        fsm_create((fsm_t*)&s11->s11_sm, 
                mme_s11_state_initial, mme_s11_state_final);
        s11->s11_sm.ctx = s11;
        fsm_init((fsm_t*)&s11->s11_sm, 0);
    }
    mme_event_s1ap_to_nas(ue, &ies->nas_pdu);
}

void s1ap_handle_uplink_nas_transport(
        enb_ctx_t *enb, s1ap_message_t *message)
{
    char buf[INET_ADDRSTRLEN];

    ue_ctx_t *ue = NULL;
    S1ap_UplinkNASTransport_IEs_t *ies = NULL;

    ies = &message->s1ap_UplinkNASTransport_IEs;
    d_assert(ies, return, "Null param");

    ue = mme_ctx_ue_find_by_enb_ue_s1ap_id(enb, ies->eNB_UE_S1AP_ID);
    d_assert(ue, return, "Null param");

    d_info("[S1AP] uplinkNASTransport : UE[eNB-UE-S1AP-ID(%d)] --> eNB[%s:%d]",
        ue->enb_ue_s1ap_id,
        INET_NTOP(&enb->s1ap_sock->remote.sin_addr.s_addr, buf),
        enb->enb_id);

    mme_event_s1ap_to_nas(ue, &ies->nas_pdu);
}

