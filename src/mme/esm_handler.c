#define TRACE_MODULE _esm_handler

#include "core_debug.h"

#include "nas_message.h"

#include "mme_context.h"
#include "mme_event.h"
#include "esm_build.h"
#include "s1ap_build.h"
#include "s1ap_path.h"
#include "nas_path.h"

void esm_handle_pdn_connectivity_request(mme_sess_t *sess, 
        nas_pdn_connectivity_request_t *pdn_connectivity_request)
{
    d_assert(sess, return, "Null param");

    if (pdn_connectivity_request->presencemask &
            NAS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        nas_protocol_configuration_options_t *protocol_configuration_options = 
            &pdn_connectivity_request->protocol_configuration_options;
        sess->ue_pco_len = protocol_configuration_options->length;
        d_assert(sess->ue_pco_len <= MAX_PCO_LEN, return, 
                "length(%d) exceeds MAX:%d", sess->ue_pco_len, MAX_PCO_LEN);
        memcpy(sess->ue_pco, protocol_configuration_options->buffer, 
                sess->ue_pco_len);
    }
}

void esm_handle_information_response(mme_sess_t *sess, 
        nas_esm_information_response_t *esm_information_response)
{
    mme_ue_t *mme_ue = NULL;

    d_assert(sess, return, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return, "Null param");

    if (esm_information_response->presencemask &
            NAS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT)
    {
        sess->pdn = mme_pdn_find_by_apn(mme_ue, 
                esm_information_response->access_point_name.apn);
        d_assert(sess->pdn, return, "No PDN Context[APN:%s])", 
            esm_information_response->access_point_name.apn);
    }

    if (esm_information_response->presencemask &
            NAS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        nas_protocol_configuration_options_t *protocol_configuration_options = 
            &esm_information_response->protocol_configuration_options;
        sess->ue_pco_len = protocol_configuration_options->length;
        d_assert(sess->ue_pco_len <= MAX_PCO_LEN, return, 
                "length(%d) exceeds MAX:%d", sess->ue_pco_len, MAX_PCO_LEN);
        memcpy(sess->ue_pco, protocol_configuration_options->buffer, 
                sess->ue_pco_len);
    }
}

void esm_handle_activate_dedicated_bearer_request(mme_bearer_t *bearer)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    pkbuf_t *esmbuf = NULL, *s1apbuf = NULL;

    d_assert(bearer, return, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");

    rv = esm_build_activate_dedicated_bearer_context(&esmbuf, bearer);
    d_assert(rv == CORE_OK && esmbuf, return, "esm build error");

    d_trace(3, "[NAS] Activate dedicated bearer context request : "
            "EMM <-- ESM\n");

    rv = s1ap_build_e_rab_setup_request(&s1apbuf, bearer, esmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(esmbuf); return, "s1ap build error");

    d_assert(nas_send_to_enb(enb_ue, s1apbuf) == CORE_OK,,);
}
