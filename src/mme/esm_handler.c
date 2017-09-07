#define TRACE_MODULE _esm_handler

#include "core_debug.h"

#include "nas_message.h"

#include "mme_context.h"
#include "nas_path.h"
#include "mme_gtp_path.h"

#include "esm_build.h"

void esm_handle_pdn_connectivity_request(mme_sess_t *sess, 
        nas_pdn_connectivity_request_t *pdn_connectivity_request)
{
    mme_ue_t *mme_ue = NULL;

    d_assert(sess, return, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return, "Null param");

    if (pdn_connectivity_request->presencemask &
            NAS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT)
    {
        sess->pdn = mme_pdn_find_by_apn(mme_ue, 
                pdn_connectivity_request->access_point_name.apn);
        d_assert(sess->pdn, return, "No PDN Context[APN:%s])", 
            pdn_connectivity_request->access_point_name.apn);
    }

    if (pdn_connectivity_request->presencemask &
            NAS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        nas_protocol_configuration_options_t *protocol_configuration_options = 
            &pdn_connectivity_request->protocol_configuration_options;

        NAS_STORE_DATA(&sess->ue_pco, protocol_configuration_options);
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
        NAS_STORE_DATA(&sess->ue_pco, protocol_configuration_options);
    }
}

void esm_handle_activate_default_bearer_accept(mme_bearer_t *bearer)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    d_assert(bearer, return, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return, "Null param");

    rv = mme_gtp_send_modify_bearer_request(bearer);
    d_assert(rv == CORE_OK, return,
        "mme_gtp_send_modify_bearer_request failed");

    mme_bearer_t *dedicated_bearer = mme_bearer_next(bearer);
    while(dedicated_bearer)
    {
        rv = nas_send_activate_dedicated_bearer_context(
                enb_ue, dedicated_bearer);
        d_assert(rv == CORE_OK, return,
            "nas_send_activate_dedicated_bearer_context failed");

        dedicated_bearer = mme_bearer_next(dedicated_bearer);
    }
}
