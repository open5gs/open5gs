#define TRACE_MODULE _esm_handler

#include "core_debug.h"

#include "nas_message.h"

#include "mme_context.h"
#include "mme_event.h"
#include "esm_build.h"
#include "s1ap_build.h"
#include "s1ap_path.h"
#include "mme_s11_build.h"
#include "mme_s11_path.h"

void esm_handle_pdn_connectivity_request(mme_bearer_t *bearer, 
        nas_pdn_connectivity_request_t *pdn_connectivity_request)
{
    if (pdn_connectivity_request->presencemask &
            NAS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        nas_protocol_configuration_options_t *protocol_configuration_options = 
            &pdn_connectivity_request->protocol_configuration_options;
        bearer->ue_pco_len = protocol_configuration_options->length;
        d_assert(bearer->ue_pco_len <= MAX_PCO_LEN, return, 
                "length(%d) exceeds MAX:%d", bearer->ue_pco_len, MAX_PCO_LEN);
        memcpy(bearer->ue_pco, protocol_configuration_options->buffer, 
                bearer->ue_pco_len);
    }
}

void esm_handle_s6a_update_location(mme_bearer_t *bearer)
{
    status_t rv;
    mme_ue_t *ue = NULL;
    enb_ue_t *enb_ue = NULL;
    mme_enb_t *enb = NULL;
    pkbuf_t *esmbuf = NULL, *s1apbuf = NULL;

    d_assert(bearer, return, "Null param");
    ue = bearer->ue;
    d_assert(ue, return, "Null param");
    enb_ue = ue->enb_ue;
    d_assert(enb_ue, return, "Null param");
    enb = enb_ue->enb;
    d_assert(enb, return, "Null param");

    rv = esm_build_information_request(&esmbuf, bearer);
    d_assert(rv == CORE_OK && esmbuf, return, "esm_build failed");

    rv = s1ap_build_downlink_nas_transport(&s1apbuf, enb_ue, esmbuf);
    d_assert(rv == CORE_OK && s1apbuf, 
            pkbuf_free(esmbuf); return, "s1ap build error");

    d_assert(s1ap_send_to_enb(enb, s1apbuf) == CORE_OK,, "s1ap send error");
}

void esm_handle_modify_bearer_request(mme_bearer_t *bearer)
{
    status_t rv;
    mme_sess_t *sess = NULL;
    pkbuf_t *pkbuf = NULL;

    d_assert(bearer, return, "Null param");
    sess = bearer->sess;
    d_assert(sess, return, "Null param");

    rv = mme_s11_build_modify_bearer_request(&pkbuf, bearer);
    d_assert(rv == CORE_OK, return, "S11 build error");

    rv = mme_s11_send_to_sgw(bearer->sgw, 
            GTP_MODIFY_BEARER_REQUEST_TYPE, sess->sgw_s11_teid, pkbuf);
    d_assert(rv == CORE_OK, return, "S11 send error");
}

void esm_handle_information_response(mme_bearer_t *bearer, 
        nas_esm_information_response_t *esm_information_response)
{
    mme_ue_t *ue = NULL;
    pkbuf_t *pkbuf = NULL;
    status_t rv;

    d_assert(bearer, return, "Null param");
    ue = bearer->ue;
    d_assert(ue, return, "Null param");

    if (esm_information_response->presencemask &
            NAS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT)
    {
        bearer->pdn = mme_pdn_find_by_apn(ue, 
                esm_information_response->access_point_name.apn);
        d_assert(bearer->pdn, return, "No PDN Context[APN:%s])", 
            esm_information_response->access_point_name.apn);
    }

    if (esm_information_response->presencemask &
            NAS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        nas_protocol_configuration_options_t *protocol_configuration_options = 
            &esm_information_response->protocol_configuration_options;
        bearer->ue_pco_len = protocol_configuration_options->length;
        d_assert(bearer->ue_pco_len <= MAX_PCO_LEN, return, 
                "length(%d) exceeds MAX:%d", bearer->ue_pco_len, MAX_PCO_LEN);
        memcpy(bearer->ue_pco, protocol_configuration_options->buffer, 
                bearer->ue_pco_len);
    }

    rv = mme_s11_build_create_session_request(&pkbuf, bearer);
    d_assert(rv == CORE_OK, return, "S11 build error");

    rv = mme_s11_send_to_sgw(bearer->sgw, 
            GTP_CREATE_SESSION_REQUEST_TYPE, 0, pkbuf);
    d_assert(rv == CORE_OK, return, "S11 send error");
}
