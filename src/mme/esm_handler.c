#define TRACE_MODULE _esm_handler

#include "core_debug.h"

#include "nas/nas_message.h"

#include "mme_context.h"
#include "nas_path.h"
#include "mme_gtp_path.h"

#include "esm_build.h"

status_t esm_handle_pdn_connectivity_request(mme_bearer_t *bearer, 
        nas_pdn_connectivity_request_t *pdn_connectivity_request)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    c_uint8_t security_protected_required = 0;

    d_assert(bearer, return CORE_ERROR,);
    sess = bearer->sess;
    d_assert(sess, return CORE_ERROR,);
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return CORE_ERROR,);

    d_assert(pdn_connectivity_request, return CORE_ERROR,);

    d_assert(MME_UE_HAVE_IMSI(mme_ue), return CORE_ERROR,
        "No IMSI in PDN_CPNNECTIVITY_REQUEST");
    d_assert(SECURITY_CONTEXT_IS_VALID(mme_ue), return CORE_ERROR,
        "No Security Context in PDN_CPNNECTIVITY_REQUEST");

    memcpy(&sess->request_type, &pdn_connectivity_request->request_type,
            sizeof(sess->request_type));

    security_protected_required = 0;
    if (pdn_connectivity_request->presencemask &
            NAS_PDN_CONNECTIVITY_REQUEST_ESM_INFORMATION_TRANSFER_FLAG_PRESENT)
    {
        nas_esm_information_transfer_flag_t *esm_information_transfer_flag =
            &pdn_connectivity_request->esm_information_transfer_flag;
        security_protected_required = 
                esm_information_transfer_flag->security_protected_required;
        d_trace(5, "    EIT(ESM information transfer)[%d]\n",
                security_protected_required);
    }

    if (pdn_connectivity_request->presencemask &
            NAS_PDN_CONNECTIVITY_REQUEST_ACCESS_POINT_NAME_PRESENT)
    {
        sess->pdn = mme_pdn_find_by_apn(mme_ue, 
            pdn_connectivity_request->access_point_name.apn);
        if (!sess->pdn)
        {
            /* Invalid APN */
            rv = nas_send_pdn_connectivity_reject(
                    sess, ESM_CAUSE_MISSING_OR_UNKNOWN_APN);
            d_assert(rv == CORE_OK,, "nas send failed");

            return CORE_ERROR;
        }
    }

    if (pdn_connectivity_request->presencemask &
            NAS_PDN_CONNECTIVITY_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        nas_protocol_configuration_options_t *protocol_configuration_options = 
            &pdn_connectivity_request->protocol_configuration_options;

        NAS_STORE_DATA(&sess->ue_pco, protocol_configuration_options);
    }

    if (security_protected_required)
    {
        rv = nas_send_esm_information_request(bearer);
        d_assert(rv == CORE_OK, return CORE_ERROR, "nas send failed");

        return CORE_OK;
    }

    if (!sess->pdn)
    {
        /* Default APN */
        sess->pdn = mme_default_pdn(mme_ue);
    }

    if (sess->pdn)
    {
        d_trace(5, "    APN[%s]\n", sess->pdn->apn);
        rv = mme_gtp_send_create_session_request(sess);
        d_assert(rv == CORE_OK, return CORE_ERROR, "gtp send failed");
    }
    else
    {
        rv = nas_send_pdn_connectivity_reject(
                sess, ESM_CAUSE_MISSING_OR_UNKNOWN_APN);
        d_assert(rv == CORE_OK,, "nas send failed");

        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t esm_handle_information_response(mme_sess_t *sess, 
        nas_esm_information_response_t *esm_information_response)
{
    status_t rv;
    mme_ue_t *mme_ue = NULL;

    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    if (esm_information_response->presencemask &
            NAS_ESM_INFORMATION_RESPONSE_ACCESS_POINT_NAME_PRESENT)
    {
        sess->pdn = mme_pdn_find_by_apn(mme_ue, 
                esm_information_response->access_point_name.apn);
    }

    if (esm_information_response->presencemask &
            NAS_ESM_INFORMATION_RESPONSE_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT)
    {
        nas_protocol_configuration_options_t *protocol_configuration_options = 
            &esm_information_response->protocol_configuration_options;
        NAS_STORE_DATA(&sess->ue_pco, protocol_configuration_options);
    }

    if (sess->pdn)
    {
        d_trace(5, "    APN[%s]\n", sess->pdn->apn);
        if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue))
        {
            rv = nas_send_attach_accept(mme_ue);
            d_assert(rv == CORE_OK, return CORE_ERROR, "nas send failed");
        }
        else
        {
            rv = mme_gtp_send_create_session_request(sess);
            d_assert(rv == CORE_OK, return CORE_ERROR, "gtp send failed");
        }
    }
    else
    {
        rv = nas_send_pdn_connectivity_reject(
                sess, ESM_CAUSE_MISSING_OR_UNKNOWN_APN);
        d_assert(rv == CORE_OK,, "nas send failed");

        return CORE_ERROR;
    }

    return CORE_OK;
}
