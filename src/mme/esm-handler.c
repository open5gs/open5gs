#include "nas/nas-message.h"

#include "mme-context.h"
#include "nas-path.h"
#include "mme-gtp-path.h"

#include "esm-build.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __esm_log_domain

int esm_handle_pdn_connectivity_request(mme_bearer_t *bearer, 
        nas_pdn_connectivity_request_t *pdn_connectivity_request)
{
    int rv;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    uint8_t security_protected_required = 0;

    ogs_assert(bearer);
    sess = bearer->sess;
    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

    ogs_assert(pdn_connectivity_request);

    ogs_assert(MME_UE_HAVE_IMSI(mme_ue));
    ogs_assert(SECURITY_CONTEXT_IS_VALID(mme_ue));

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
        ogs_debug("    EIT(ESM information transfer)[%d]",
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
            ogs_assert(rv == OGS_OK);

            return OGS_ERROR;
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
        ogs_assert(rv == OGS_OK);

        return OGS_OK;
    }

    if (!sess->pdn)
    {
        /* Default APN */
        sess->pdn = mme_default_pdn(mme_ue);
    }

    if (sess->pdn)
    {
        ogs_debug("    APN[%s]", sess->pdn->apn);
        rv = mme_gtp_send_create_session_request(sess);
        ogs_assert(rv == OGS_OK);
    }
    else
    {
        rv = nas_send_pdn_connectivity_reject(
                sess, ESM_CAUSE_MISSING_OR_UNKNOWN_APN);
        ogs_assert(rv == OGS_OK);

        return OGS_ERROR;
    }

    return OGS_OK;
}

int esm_handle_information_response(mme_sess_t *sess, 
        nas_esm_information_response_t *esm_information_response)
{
    int rv;
    mme_ue_t *mme_ue = NULL;

    ogs_assert(sess);
    mme_ue = sess->mme_ue;
    ogs_assert(mme_ue);

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
        ogs_debug("    APN[%s]", sess->pdn->apn);
        if (SESSION_CONTEXT_IS_AVAILABLE(mme_ue))
        {
            rv = nas_send_attach_accept(mme_ue);
            ogs_assert(rv == OGS_OK);
        }
        else
        {
            rv = mme_gtp_send_create_session_request(sess);
            ogs_assert(rv == OGS_OK);
        }
    }
    else
    {
        rv = nas_send_pdn_connectivity_reject(
                sess, ESM_CAUSE_MISSING_OR_UNKNOWN_APN);
        ogs_assert(rv == OGS_OK);

        return OGS_ERROR;
    }

    return OGS_OK;
}
