#define TRACE_MODULE _nas_path

#include "core_debug.h"
#include "s1ap_path.h"
#include "s1ap_build.h"
#include "esm_build.h"
#include "emm_build.h"
#include "nas_path.h"
#include "mme_event.h"
#include "mme_sm.h"

status_t nas_send_to_enb(mme_ue_t *mme_ue, pkbuf_t *pkbuf)
{
    mme_enb_t *enb = NULL;
    enb_ue_t *enb_ue = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
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

status_t nas_send_to_downlink_nas_transport(mme_ue_t *mme_ue, pkbuf_t *pkbuf)
{
    status_t rv;
    pkbuf_t *s1apbuf = NULL;
    enb_ue_t *enb_ue = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    rv = s1ap_build_downlink_nas_transport(&s1apbuf, enb_ue, pkbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(pkbuf); return CORE_ERROR, "s1ap build error");

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "nas send error");

    return CORE_OK;
}

status_t nas_send_attach_accept(mme_ue_t *mme_ue)
{
    status_t rv;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    pkbuf_t *esmbuf = NULL, *emmbuf = NULL, *s1apbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    sess = mme_sess_first(mme_ue);
    d_assert(sess, return CORE_ERROR, "Null param");
    d_assert(mme_sess_next(sess) == NULL,
            return CORE_ERROR, "there is another session");
    bearer = mme_default_bearer_in_sess(sess);
    d_assert(bearer, return CORE_ERROR, "Null param");
    d_assert(mme_bearer_next(bearer) == NULL,
            return CORE_ERROR, "there is dedicated bearer");

    rv = esm_build_activate_default_bearer_context_request(&esmbuf, sess);
    d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR, "esm build error");

    rv = emm_build_attach_accept(&emmbuf, mme_ue, esmbuf);
    d_assert(rv == CORE_OK && emmbuf, 
            pkbuf_free(esmbuf); return CORE_ERROR, "emm build error");

    rv = s1ap_build_initial_context_setup_request(&s1apbuf, mme_ue, emmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(emmbuf); return CORE_ERROR, "s1ap build error");

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "nas send error");

    return CORE_OK;
}

status_t nas_send_attach_reject(mme_ue_t *mme_ue,
    nas_emm_cause_t emm_cause, nas_esm_cause_t esm_cause)
{
    status_t rv;
    mme_sess_t *sess = NULL;
    pkbuf_t *esmbuf = NULL, *emmbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    d_trace(3, "[EMM] Attach reject\n");
    d_trace(5, "    IMSI[%s] Cause[%d]\n", mme_ue->imsi_bcd, emm_cause);

    sess = mme_sess_first(mme_ue);
    if (sess)
    {
        rv = esm_build_pdn_connectivity_reject(&esmbuf, sess, esm_cause);
        d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR, "esm build error");
    }

    rv = emm_build_attach_reject(&emmbuf, emm_cause, esmbuf);
    d_assert(rv == CORE_OK && emmbuf,
            esmbuf ? pkbuf_free(esmbuf) : 1; return CORE_ERROR,
            "emm build error");
    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    d_assert(rv == CORE_OK,
            esmbuf ? pkbuf_free(esmbuf) : 1; return CORE_ERROR,
            "nas send error");

    return rv;
}

status_t nas_send_identity_request(mme_ue_t *mme_ue)
{
    status_t rv;
    pkbuf_t *emmbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    rv = emm_build_identity_request(&emmbuf, mme_ue);
    d_assert(rv == CORE_OK && emmbuf, return CORE_ERROR,
            "nas_build_detach_accept failed"); 

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    d_assert(rv == CORE_OK,, "nas send failed");

    return rv;
}

status_t nas_send_authentication_request(
        mme_ue_t *mme_ue, e_utran_vector_t *e_utran_vector)
{
    status_t rv;
    pkbuf_t *emmbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(e_utran_vector, return CORE_ERROR, "Null param");

    d_trace(3, "[EMM] Authentication request\n");
    d_trace(5, "    IMSI[%s]\n", mme_ue->imsi_bcd);

    rv = emm_build_authentication_request(&emmbuf, e_utran_vector);
    d_assert(rv == CORE_OK && emmbuf, return CORE_ERROR,
            "nas_build_detach_accept failed"); 

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    d_assert(rv == CORE_OK,, "nas send failed");

    return rv;
}

status_t nas_send_authentication_reject(mme_ue_t *mme_ue)
{
    status_t rv;
    pkbuf_t *emmbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    d_trace(3, "[EMM] Authentication reject\n");
    d_trace(5, "    IMSI[%s]\n", mme_ue->imsi_bcd);

    rv = emm_build_authentication_reject(&emmbuf);
    d_assert(rv == CORE_OK && emmbuf, return CORE_ERROR,
            "nas_build_detach_accept failed"); 

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    d_assert(rv == CORE_OK,, "nas send failed");

    return CORE_OK;
}

status_t nas_send_detach_accept(mme_ue_t *mme_ue)
{
    status_t rv;
    enb_ue_t *enb_ue = NULL;
    pkbuf_t *emmbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    /* reply with detach accept */
    if (mme_ue->nas_eps.detach.switch_off == 0)
    {
        rv = emm_build_detach_accept(&emmbuf, mme_ue);
        d_assert(rv == CORE_OK && emmbuf, return CORE_ERROR,
                "nas_build_detach_accept failed"); 

        rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
        d_assert(rv == CORE_OK, return CORE_ERROR, "nas send failed");
    }

    rv = s1ap_send_ue_context_release_command(enb_ue,
            S1AP_Cause_PR_nas, S1AP_CauseNas_detach,
            S1AP_UE_CTX_REL_S1_NORMAL_RELEASE, 0);
    d_assert(rv == CORE_OK, return CORE_ERROR, "s1ap send error");

    return CORE_OK;
}


status_t nas_send_pdn_connectivity_reject(
    mme_sess_t *sess, nas_esm_cause_t esm_cause)
{
    status_t rv;
    mme_ue_t *mme_ue;
    pkbuf_t *esmbuf = NULL;

    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    rv = esm_build_pdn_connectivity_reject(&esmbuf, sess, esm_cause);
    d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR, "esm build error");

    rv = nas_send_to_downlink_nas_transport(mme_ue, esmbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR,
            "nas_send_to_downlink_nas_transport");

    return CORE_OK;
}

status_t nas_send_esm_information_request(mme_bearer_t *bearer)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;
    pkbuf_t *esmbuf = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    rv = esm_build_information_request(&esmbuf, bearer);
    d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR, "esm build error");

    rv = nas_send_to_downlink_nas_transport(mme_ue, esmbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR,
            "nas_send_to_downlink_nas_transport");

    return CORE_OK;
}

status_t nas_send_activate_default_bearer_context_request(mme_bearer_t *bearer)
{
    status_t rv;
    pkbuf_t *esmbuf = NULL, *s1apbuf = NULL;
    mme_sess_t *sess = NULL;
    mme_ue_t *mme_ue = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    sess = bearer->sess;
    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    rv = esm_build_activate_default_bearer_context_request(&esmbuf, sess);
    d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR, "esm build error");

    rv = s1ap_build_e_rab_setup_request(&s1apbuf, bearer, esmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(esmbuf); return CORE_ERROR, "s1ap build error");

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "nas send error");

    return CORE_OK;
}

status_t nas_send_activate_dedicated_bearer_context_request(
        mme_bearer_t *bearer)
{
    status_t rv;
    pkbuf_t *esmbuf = NULL, *s1apbuf = NULL;
    mme_ue_t *mme_ue = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    rv = esm_build_activate_dedicated_bearer_context_request(&esmbuf, bearer);
    d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR, "esm build error");

    rv = s1ap_build_e_rab_setup_request(&s1apbuf, bearer, esmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(esmbuf); return CORE_ERROR, "s1ap build error");

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "nas send error");

    return CORE_OK;
}

status_t nas_send_activate_all_dedicated_bearers(mme_bearer_t *default_bearer)
{
    status_t rv;

    d_assert(default_bearer, return CORE_ERROR, "Null param");

    mme_bearer_t *dedicated_bearer = mme_bearer_next(default_bearer);
    while(dedicated_bearer)
    {
        rv = nas_send_activate_dedicated_bearer_context_request(
                dedicated_bearer);
        d_assert(rv == CORE_OK, return CORE_ERROR,
            "nas_send_activate_dedicated_bearer_context failed");

        dedicated_bearer = mme_bearer_next(dedicated_bearer);
    }

    return CORE_OK;
}

status_t nas_send_modify_bearer_context_request(
        mme_bearer_t *bearer, int qos_presence, int tft_presence)
{
    status_t rv;
    pkbuf_t *esmbuf = NULL, *s1apbuf = NULL;
    mme_ue_t *mme_ue = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    rv = esm_build_modify_bearer_context_request(
            &esmbuf, bearer, qos_presence, tft_presence);
    d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR, "esm build error");

    if (qos_presence == 1)
    {
        rv = s1ap_build_e_rab_modify_request(&s1apbuf, bearer, esmbuf);
        d_assert(rv == CORE_OK && s1apbuf, 
                pkbuf_free(esmbuf); return CORE_ERROR, "s1ap build error");

        rv = nas_send_to_enb(mme_ue, s1apbuf);
        d_assert(rv == CORE_OK, return CORE_ERROR, "nas send error");
    }
    else
    {
        rv = nas_send_to_downlink_nas_transport(mme_ue, esmbuf);
        d_assert(rv == CORE_OK, return CORE_ERROR, "nas send failed");
    }

    return CORE_OK;
}

status_t nas_send_deactivate_bearer_context_request(mme_bearer_t *bearer)
{
    status_t rv;
    pkbuf_t *esmbuf = NULL, *s1apbuf = NULL;
    mme_ue_t *mme_ue = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    rv = esm_build_deactivate_bearer_context_request(
            &esmbuf, bearer, ESM_CAUSE_REGULAR_DEACTIVATION);
    d_assert(rv == CORE_OK && esmbuf, return CORE_ERROR, "esm build error");

    rv = s1ap_build_e_rab_release_command(&s1apbuf, bearer, esmbuf,
            S1AP_Cause_PR_nas, S1AP_CauseNas_normal_release);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(esmbuf); return CORE_ERROR, "s1ap build error");

    rv = nas_send_to_enb(mme_ue, s1apbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "nas send error");

    return CORE_OK;
}

status_t nas_send_tau_accept(
        mme_ue_t *mme_ue, S1AP_ProcedureCode_t procedureCode)
{
    status_t rv;
    pkbuf_t *emmbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    d_trace(3, "[EMM] Tracking area update accept\n");
    d_trace(5, "    IMSI[%s]\n", mme_ue->imsi_bcd);

    rv = emm_build_tau_accept(&emmbuf, mme_ue);
    d_assert(rv == CORE_OK, return CORE_ERROR, "emm build error");

    if (procedureCode == S1AP_ProcedureCode_id_InitialContextSetup)
    {
        pkbuf_t *s1apbuf = NULL;
        rv = s1ap_build_initial_context_setup_request(&s1apbuf, mme_ue, emmbuf);
        d_assert(rv == CORE_OK && s1apbuf,
            pkbuf_free(emmbuf); return CORE_ERROR, "s1ap build error");

        rv = nas_send_to_enb(mme_ue, s1apbuf);
        d_assert(rv == CORE_OK,, "nas send error");
    }
    else if (procedureCode == S1AP_ProcedureCode_id_downlinkNASTransport)
    {
        rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
        d_assert(rv == CORE_OK,, "nas_send_to_downlink_nas_transport");
    }
    else
        d_assert(0, pkbuf_free(emmbuf); return CORE_ERROR,
                "Invalid Procedure Code[%d]", procedureCode);

    return rv;
}

status_t nas_send_tau_reject(mme_ue_t *mme_ue, nas_emm_cause_t emm_cause)
{
    status_t rv;
    pkbuf_t *emmbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    /* Build TAU reject */
    rv = emm_build_tau_reject(&emmbuf, emm_cause, mme_ue);
    d_assert(rv == CORE_OK, return CORE_ERROR, "emm build error");

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "nas dl send error");

    return CORE_OK;
}

status_t nas_send_service_reject(mme_ue_t *mme_ue, nas_emm_cause_t emm_cause)
{
    status_t rv;
    pkbuf_t *emmbuf = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    /* Build Service Reject */
    rv = emm_build_service_reject(&emmbuf, emm_cause, mme_ue);
    d_assert(rv == CORE_OK, return CORE_ERROR, "emm build error");

    rv = nas_send_to_downlink_nas_transport(mme_ue, emmbuf);
    d_assert(rv == CORE_OK, return CORE_ERROR, "nas dl send error");

    return CORE_OK;
}
