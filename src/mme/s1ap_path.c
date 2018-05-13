#define TRACE_MODULE _s1ap_path

#include "core_debug.h"
#include "core_thread.h"

#include "mme_event.h"

#include "nas_security.h"
#include "nas_path.h"

#include "s1ap_conv.h"
#include "s1ap_build.h"
#include "s1ap_path.h"

static status_t s1ap_server_list(list_t *list, int type);
static status_t s1ap_delete_list(list_t *list);

status_t s1ap_open(void)
{
    status_t rv;
#if USE_USRSCTP != 1
    int type = SOCK_STREAM;
#else
    int type = SOCK_SEQPACKET;
#endif

    rv = s1ap_server_list(&mme_self()->s1ap_list, type);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    rv = s1ap_server_list(&mme_self()->s1ap_list6, type);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    return CORE_OK;
}

status_t s1ap_close()
{
    s1ap_delete_list(&mme_self()->s1ap_list);
    s1ap_delete_list(&mme_self()->s1ap_list6);

    return CORE_OK;
}

static status_t s1ap_server_list(list_t *list, int type)
{
    status_t rv;
    sock_node_t *snode = NULL;

    d_assert(list, return CORE_ERROR,);

    for (snode = list_first(list); snode; snode = list_next(snode))
    {
        rv = s1ap_server(snode, type);
        d_assert(rv == CORE_OK, return CORE_ERROR,);
    }

    return CORE_OK;
}

static status_t s1ap_delete_list(list_t *list)
{
    sock_node_t *snode = NULL;

    for (snode = list_first(list); snode; snode = list_next(snode))
    {
        s1ap_delete(snode->sock);
    }

    return CORE_OK;
}

status_t s1ap_send_to_enb(mme_enb_t *enb, pkbuf_t *pkbuf, c_uint16_t stream_no)
{
    char buf[CORE_ADDRSTRLEN];
    status_t rv;

    d_assert(enb, return CORE_ERROR,);
    d_assert(pkbuf, return CORE_ERROR,);
    d_assert(enb->sock, return CORE_ERROR,);

    d_trace(5, "    IP[%s] ENB_ID[%d]\n",
            CORE_ADDR(enb->addr, buf), enb->enb_id);

    rv = s1ap_send(enb->sock, pkbuf,
            enb->sock_type == SOCK_STREAM ? NULL : enb->addr,
            stream_no);
    if (rv != CORE_OK)
    {
        d_error("s1_send error");
        pkbuf_free(pkbuf);
    }

    return CORE_OK;;
}

status_t s1ap_send_to_enb_ue(enb_ue_t *enb_ue, pkbuf_t *pkbuf)
{
    mme_enb_t *enb = NULL;
    mme_ue_t *mme_ue = NULL;

    d_assert(enb_ue, return CORE_ERROR,);
    enb = enb_ue->enb;
    d_assert(enb, return CORE_ERROR,);
    mme_ue = enb_ue->mme_ue;
    d_assert(mme_ue, return CORE_ERROR,);

    return s1ap_send_to_enb(enb, pkbuf, mme_ue->ostream_id);
}

status_t s1ap_delayed_send_to_enb_ue(
        enb_ue_t *enb_ue, pkbuf_t *pkbuf, c_uint32_t duration)
{
    tm_block_id timer = 0;

    d_assert(enb_ue, return CORE_ERROR,);
    d_assert(pkbuf, return CORE_ERROR,);
        
    if (duration)
    {
        timer = timer_create(
                &mme_self()->tm_service, MME_EVT_S1AP_DELAYED_SEND, duration);
        d_assert(timer, return CORE_ERROR,);

        timer_set_param1(timer, (c_uintptr_t)enb_ue->index);
        timer_set_param2(timer, (c_uintptr_t)pkbuf);
        timer_set_param3(timer, timer);

        tm_start(timer);

        return CORE_OK;
    }
    else
    {
        mme_enb_t *enb = NULL;
        enb = enb_ue->enb;
        d_assert(enb, return CORE_ERROR,);
        return s1ap_send_to_enb_ue(enb_ue, pkbuf);
    }
}

status_t s1ap_send_to_esm(mme_ue_t *mme_ue, pkbuf_t *esmbuf)
{
    event_t e;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(esmbuf, return CORE_ERROR, "Null param");

    event_set(&e, MME_EVT_ESM_MESSAGE);
    event_set_param1(&e, (c_uintptr_t)mme_ue->index);
    event_set_param2(&e, (c_uintptr_t)esmbuf);
    mme_event_send(&e);

    return CORE_OK;
}
 
status_t s1ap_send_to_nas(enb_ue_t *enb_ue,
        S1AP_ProcedureCode_t procedureCode, S1AP_NAS_PDU_t *nasPdu)
{
    nas_security_header_t *sh = NULL;
    nas_security_header_type_t security_header_type;

    nas_emm_header_t *h = NULL;
    pkbuf_t *nasbuf = NULL;
    event_t e;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(nasPdu, return CORE_ERROR, "Null param");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    nasbuf = pkbuf_alloc(NAS_HEADROOM, nasPdu->size);
    d_assert(nasbuf, return CORE_ERROR, "Null param");
    memcpy(nasbuf->payload, nasPdu->buf, nasPdu->size);

    sh = nasbuf->payload;
    d_assert(sh, return CORE_ERROR, "Null param");

    memset(&security_header_type, 0, sizeof(nas_security_header_type_t));
    switch(sh->security_header_type)
    {
        case NAS_SECURITY_HEADER_PLAIN_NAS_MESSAGE:
            break;
        case NAS_SECURITY_HEADER_FOR_SERVICE_REQUEST_MESSAGE:
            security_header_type.service_request = 1;
            break;
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED:
            security_header_type.integrity_protected = 1;
            d_assert(pkbuf_header(nasbuf, -6) == CORE_OK,
                    return CORE_ERROR, "pkbuf_header error");
            break;
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED:
            security_header_type.integrity_protected = 1;
            security_header_type.ciphered = 1;
            d_assert(pkbuf_header(nasbuf, -6) == CORE_OK,
                    return CORE_ERROR, "pkbuf_header error");
            break;
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT:
            security_header_type.integrity_protected = 1;
            security_header_type.new_security_context = 1;
            d_assert(pkbuf_header(nasbuf, -6) == CORE_OK,
                    return CORE_ERROR, "pkbuf_header error");
            break;
        case NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHTERD_WITH_NEW_INTEGRITY_CONTEXT:
            security_header_type.integrity_protected = 1;
            security_header_type.ciphered = 1;
            security_header_type.new_security_context = 1;
            d_assert(pkbuf_header(nasbuf, -6) == CORE_OK,
                    return CORE_ERROR, "pkbuf_header error");
            break;
        default:
            d_error("Not implemented(securiry header type:0x%x)", 
                    sh->security_header_type);
            return CORE_ERROR;
    }

    if (enb_ue->mme_ue)
    {
        d_assert(nas_security_decode(
            enb_ue->mme_ue, security_header_type, nasbuf) == CORE_OK,
            pkbuf_free(nasbuf);return CORE_ERROR, "nas_security_decode failed");
    }

    h = nasbuf->payload;
    d_assert(h, pkbuf_free(nasbuf); return CORE_ERROR, "Null param");
    if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_EMM)
    {
        event_set(&e, MME_EVT_EMM_MESSAGE);
        event_set_param1(&e, (c_uintptr_t)enb_ue->index);
        event_set_param2(&e, (c_uintptr_t)procedureCode);
        event_set_param3(&e, (c_uintptr_t)security_header_type.type);
        event_set_param4(&e, (c_uintptr_t)nasbuf);
        mme_event_send(&e);
    }
    else if (h->protocol_discriminator == NAS_PROTOCOL_DISCRIMINATOR_ESM)
    {
        mme_ue_t *mme_ue = enb_ue->mme_ue;
        d_assert(mme_ue, return CORE_ERROR, "Null param");
        s1ap_send_to_esm(mme_ue, nasbuf);
    }
    else
        d_assert(0, pkbuf_free(nasbuf); return CORE_ERROR,
                "Unknown protocol:%d", h->protocol_discriminator);

    return CORE_OK;
}

status_t s1ap_send_initial_context_setup_request(mme_ue_t *mme_ue)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    rv = s1ap_build_initial_context_setup_request(&s1apbuf, mme_ue, NULL);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "s1ap send error");

    return CORE_OK;
}

status_t s1ap_send_ue_context_release_command(
    enb_ue_t *enb_ue, S1AP_Cause_PR group, long cause,
    c_uint8_t action, c_uint32_t delay)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(action != S1AP_UE_CTX_REL_INVALID_ACTION, return CORE_ERROR,
            "Should give valid action for UE Context Release Command");

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    enb_ue->ue_ctx_rel_action = action;

    d_trace(3, "[MME] UE Context release command\n");
    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);
    d_trace(5, "    Group[%d] Cause[%d] Action[%d] Delay[%d]\n",
            group, cause, action, delay);

    rv = s1ap_build_ue_context_release_command(&s1apbuf, enb_ue, group, cause);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_delayed_send_to_enb_ue(enb_ue, s1apbuf, delay);
    d_assert(rv == CORE_OK,, "s1ap send error");

    return CORE_OK;
}

status_t s1ap_send_mme_configuration_transfer(
        mme_enb_t *target_enb,
        S1AP_SONConfigurationTransfer_t *SONConfigurationTransfer)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(target_enb, return CORE_ERROR,);
    d_assert(SONConfigurationTransfer, return CORE_ERROR,);

    rv = s1ap_build_mme_configuration_transfer(
            &s1apbuf, SONConfigurationTransfer);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb(target_enb, s1apbuf, S1AP_NON_UE_SIGNALLING);
    d_assert(rv == CORE_OK,, "s1ap send error");

    return rv;
}

status_t s1ap_send_path_switch_ack(mme_ue_t *mme_ue)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    rv = s1ap_build_path_switch_ack(&s1apbuf, mme_ue);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "s1ap send error");
    
    return CORE_OK;
}

status_t s1ap_send_handover_command(enb_ue_t *source_ue)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(source_ue, return CORE_ERROR,);

    rv = s1ap_build_handover_command(&s1apbuf, source_ue);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb_ue(source_ue, s1apbuf);
    d_assert(rv == CORE_OK,, "s1ap send error");

    return rv;
}

status_t s1ap_send_handover_preparation_failure(
        enb_ue_t *source_ue, S1AP_Cause_t *cause)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(source_ue, return CORE_ERROR,);
    d_assert(cause, return CORE_ERROR,);

    rv = s1ap_build_handover_preparation_failure(&s1apbuf, source_ue, cause);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb_ue(source_ue, s1apbuf);
    d_assert(rv == CORE_OK,, "s1ap send error");

    return rv;
}

status_t s1ap_send_handover_cancel_ack(enb_ue_t *source_ue)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(source_ue, return CORE_ERROR,);

    rv = s1ap_build_handover_cancel_ack(&s1apbuf, source_ue);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb_ue(source_ue, s1apbuf);
    d_assert(rv == CORE_OK,, "s1ap send error");

    return rv;
}


status_t s1ap_send_handover_request(
        mme_ue_t *mme_ue,
        mme_enb_t *target_enb,
        S1AP_ENB_UE_S1AP_ID_t *enb_ue_s1ap_id,
        S1AP_MME_UE_S1AP_ID_t *mme_ue_s1ap_id,
        S1AP_HandoverType_t *handovertype,
        S1AP_Cause_t *cause,
        S1AP_Source_ToTarget_TransparentContainer_t
            *source_totarget_transparentContainer)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    enb_ue_t *source_ue = NULL, *target_ue = NULL;

    d_trace(3, "[MME] Handover request\n");
    
    d_assert(target_enb, return CORE_ERROR, "Cannot find target eNB");

    d_assert(mme_ue, return CORE_ERROR,);
    source_ue = mme_ue->enb_ue;
    d_assert(source_ue, return CORE_ERROR,);
    d_assert(source_ue->target_ue == NULL, return CORE_ERROR,
            "Handover Required Duplicated");

    target_ue = enb_ue_add(target_enb);
    d_assert(target_ue, return CORE_ERROR,);

    d_trace(5, "    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    d_trace(5, "    Target : ENB_UE_S1AP_ID[Unknown] MME_UE_S1AP_ID[%d]\n",
            target_ue->mme_ue_s1ap_id);

    rv = source_ue_associate_target_ue(source_ue, target_ue);
    d_assert(rv == CORE_OK, return CORE_ERROR,);

    rv = s1ap_build_handover_request(&s1apbuf, mme_ue, target_ue,
            enb_ue_s1ap_id, mme_ue_s1ap_id,
            handovertype, cause,
            source_totarget_transparentContainer);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb_ue(target_ue, s1apbuf);
    d_assert(rv == CORE_OK,, "s1ap send error");

    return rv;
}

status_t s1ap_send_mme_status_transfer(
        enb_ue_t *target_ue,
        S1AP_ENB_StatusTransfer_TransparentContainer_t
            *enb_statustransfer_transparentContainer)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(target_ue, return CORE_ERROR,);

    rv = s1ap_build_mme_status_transfer(&s1apbuf, target_ue,
            enb_statustransfer_transparentContainer);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb_ue(target_ue, s1apbuf);
    d_assert(rv == CORE_OK,, "s1ap send error");

    return rv;
}

status_t s1ap_send_error_indication(
        mme_enb_t *enb,
        S1AP_MME_UE_S1AP_ID_t *mme_ue_s1ap_id,
        S1AP_ENB_UE_S1AP_ID_t *enb_ue_s1ap_id,
        S1AP_Cause_PR group, long cause)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(enb, return CORE_ERROR,);

    rv = s1ap_build_error_indication(&s1apbuf,
            mme_ue_s1ap_id, enb_ue_s1ap_id, group, cause);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb(enb, s1apbuf, S1AP_NON_UE_SIGNALLING);
    d_assert(rv == CORE_OK,, "s1ap send error");

    return rv;
}

status_t s1ap_send_s1_reset_ack(
        mme_enb_t *enb,
        S1AP_UE_associatedLogicalS1_ConnectionListRes_t *partOfS1_Interface)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;

    d_assert(enb, return CORE_ERROR,);

    rv = s1ap_build_s1_reset_ack(&s1apbuf, partOfS1_Interface);
    d_assert(rv == CORE_OK && s1apbuf, return CORE_ERROR, "s1ap build error");

    rv = s1ap_send_to_enb(enb, s1apbuf, S1AP_NON_UE_SIGNALLING);
    d_assert(rv == CORE_OK,, "s1ap send error");

    return rv;
}
