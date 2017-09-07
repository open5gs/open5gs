#define TRACE_MODULE _nas_path

#include "core_debug.h"
#include "s1ap_path.h"
#include "s1ap_build.h"
#include "esm_build.h"
#include "emm_build.h"
#include "nas_path.h"
#include "mme_event.h"

status_t nas_send_to_enb(enb_ue_t *enb_ue, pkbuf_t *pkbuf)
{
    mme_enb_t *enb = NULL;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return CORE_ERROR, "Null param");

    return s1ap_send_to_enb(enb, pkbuf);
}

status_t nas_send_emm_to_esm(
        mme_ue_t *mme_ue, nas_esm_message_container_t *esm_message_container)
{
    pkbuf_t *esmbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(esm_message_container, return CORE_ERROR, "Null param");
    d_assert(esm_message_container->length, return CORE_ERROR, "Null param");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    esmbuf = pkbuf_alloc(NAS_HEADROOM, esm_message_container->length);
    d_assert(esmbuf, return CORE_ERROR, "Null param");
    memcpy(esmbuf->payload, 
            esm_message_container->buffer, esm_message_container->length);

    d_assert(s1ap_send_to_esm(mme_ue, esmbuf) == CORE_OK, return CORE_ERROR,
            "s1ap_send_to_esm failed");

    return CORE_OK;
}

status_t nas_send_to_downlink_nas_transport(
    mme_ue_t *mme_ue, pkbuf_t *pkbuf)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;
    enb_ue_t *enb_ue = NULL;

    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    rv = s1ap_build_downlink_nas_transport(&s1apbuf, enb_ue, pkbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(pkbuf); return CORE_ERROR, "s1ap build error");

    return nas_send_to_enb(enb_ue, s1apbuf);
}

status_t nas_send_attach_accept(mme_ue_t *mme_ue)
{
    status_t rv;
    mme_sess_t *sess = NULL;
    enb_ue_t *enb_ue = NULL;
    pkbuf_t *esmbuf = NULL, *emmbuf = NULL, *s1apbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    sess = mme_sess_first(mme_ue);
    d_assert(sess, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    rv = esm_build_activate_default_bearer_context_request(&esmbuf, sess);
    d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR, "esm build error");

    d_trace(3, "[NAS] Activate default bearer context request : "
            "EMM <-- ESM\n");

    rv = emm_build_attach_accept(&emmbuf, mme_ue, esmbuf);
    d_assert(rv == CORE_OK && emmbuf, 
            pkbuf_free(esmbuf); return CORE_ERROR, "emm build error");

    d_trace(3, "[NAS] Attach accept : UE[%s] <-- EMM\n", mme_ue->imsi_bcd);

    rv = s1ap_build_initial_context_setup_request(&s1apbuf, sess, emmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(emmbuf); return CORE_ERROR, "s1ap build error");

    d_assert(nas_send_to_enb(enb_ue, s1apbuf) == CORE_OK,,);

    return CORE_OK;
}

status_t nas_send_attach_reject(mme_ue_t *mme_ue,
    e_S1ap_CauseNas s1ap_cause_nas,
    nas_emm_cause_t emm_cause, nas_esm_cause_t esm_cause)
{
    status_t rv;
    mme_enb_t *enb = NULL;
    enb_ue_t *enb_ue = NULL;
    mme_sess_t *sess = NULL;
    pkbuf_t *s1apbuf = NULL, *esmbuf = NULL, *emmbuf = NULL;
    S1ap_Cause_t cause;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return CORE_ERROR, "Null param");

    sess = mme_sess_first(mme_ue);
    if (sess)
    {
        rv = esm_build_pdn_connectivity_reject(
                &esmbuf, sess->pti, esm_cause);
        d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR,
                "esm build error");
        d_trace(3, "[NAS] PDN Connectivity reject : EMM <-- ESM\n",
                sess->pti);
    }

    rv = emm_build_attach_reject(&emmbuf, emm_cause, esmbuf);
    d_assert(rv == CORE_OK && emmbuf, 
            pkbuf_free(esmbuf); return CORE_ERROR, "emm build error");
    d_assert(nas_send_to_downlink_nas_transport(mme_ue, emmbuf) == CORE_OK,,);
    d_trace(3, "[NAS] Attach reject : UE[%s] <-- EMM\n", mme_ue->imsi_bcd);

    cause.present = S1ap_Cause_PR_nas;
    cause.choice.nas = s1ap_cause_nas;;

    rv = s1ap_build_ue_context_release_commmand(&s1apbuf, enb_ue, &cause);
    d_assert(rv == CORE_OK && s1apbuf,
            return CORE_ERROR, "s1ap build error");
    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,
            return CORE_ERROR, "s1ap send error");

    return CORE_OK;
}

status_t nas_send_activate_dedicated_bearer_context_request(
        enb_ue_t *enb_ue, mme_bearer_t *bearer)
{
    status_t rv;
    pkbuf_t *esmbuf = NULL, *s1apbuf = NULL;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(bearer, return CORE_ERROR, "Null param");

    rv = esm_build_activate_dedicated_bearer_context_request(&esmbuf, bearer);
    d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR, "esm build error");

    d_trace(3, "[NAS] Activate dedicated bearer context request : "
            "EMM <-- ESM\n");

    rv = s1ap_build_e_rab_setup_request(&s1apbuf, bearer, esmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(esmbuf); return CORE_ERROR, "s1ap build error");

    d_assert(nas_send_to_enb(enb_ue, s1apbuf) == CORE_OK,,);

    return CORE_OK;
}

status_t nas_send_deactivate_bearer_context_request(
        enb_ue_t *enb_ue, mme_bearer_t *bearer)
{
    status_t rv;
    pkbuf_t *esmbuf = NULL, *s1apbuf = NULL;
    S1ap_Cause_t cause;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(bearer, return CORE_ERROR, "Null param");

    rv = esm_build_deactivate_bearer_context_request(
            &esmbuf, bearer, ESM_CAUSE_REGULAR_DEACTIVATION);
    d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR, "esm build error");

    d_trace(3, "[NAS] Deactivate bearer context request : EMM <-- ESM\n");

    cause.present = S1ap_Cause_PR_nas;
    cause.choice.nas = S1ap_CauseNas_normal_release;
    rv = s1ap_build_e_rab_release_command(&s1apbuf, bearer, esmbuf, &cause);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(esmbuf); return CORE_ERROR, "s1ap build error");

    d_assert(nas_send_to_enb(enb_ue, s1apbuf) == CORE_OK,,);

    return CORE_OK;
}
