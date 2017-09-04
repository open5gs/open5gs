#define TRACE_MODULE _esm_build

#include "core_debug.h"
#include "core_lib.h"

#include "nas_message.h"

#include "nas_security.h"
#include "esm_build.h"

status_t esm_build_pdn_connectivity_reject(
        pkbuf_t **pkbuf, c_uint8_t pti, nas_esm_cause_t esm_cause)
{
    nas_message_t message;
    nas_pdn_connectivity_reject_t *pdn_connectivity_reject = 
            &message.esm.pdn_connectivity_reject;

    memset(&message, 0, sizeof(message));
    message.esm.h.eps_bearer_identity = 0;
    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = pti;
    message.esm.h.message_type = NAS_PDN_CONNECTIVITY_REJECT;

    pdn_connectivity_reject->esm_cause = esm_cause;

    d_assert(nas_plain_encode(pkbuf, &message) == CORE_OK && *pkbuf,,);

    return CORE_OK;
}

status_t esm_build_information_request(pkbuf_t **pkbuf, mme_bearer_t *bearer)
{
    nas_message_t message;
    mme_ue_t *mme_ue = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = bearer->pti;
    message.esm.h.message_type = NAS_ESM_INFORMATION_REQUEST;

    d_assert(nas_security_encode(pkbuf, mme_ue, &message) == CORE_OK && 
            *pkbuf,,);

    return CORE_OK;
}

status_t esm_build_activate_default_bearer_context(
        pkbuf_t **pkbuf, mme_sess_t *sess)
{
    nas_message_t message;
    nas_activate_default_eps_bearer_context_request_t 
        *activate_default_eps_bearer_context_request = 
            &message.esm.activate_default_eps_bearer_context_request;
    nas_eps_quality_of_service_t *eps_qos =
        &activate_default_eps_bearer_context_request->eps_qos;
    nas_access_point_name_t *access_point_name =
        &activate_default_eps_bearer_context_request->access_point_name;
    nas_pdn_address_t *pdn_address = 
        &activate_default_eps_bearer_context_request->pdn_address;
    nas_apn_aggregate_maximum_bit_rate_t *apn_ambr =
        &activate_default_eps_bearer_context_request->apn_ambr;
    nas_protocol_configuration_options_t *protocol_configuration_options =
        &activate_default_eps_bearer_context_request
            ->protocol_configuration_options;
    
    mme_ue_t *mme_ue = NULL;
    mme_bearer_t *bearer = NULL;
    pdn_t *pdn = NULL;

    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    pdn = sess->pdn;
    d_assert(pdn, return CORE_ERROR, "Null param");
    bearer = mme_default_bearer_in_sess(sess);
    d_assert(bearer, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.esm.h.eps_bearer_identity = bearer->ebi;
    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = bearer->pti;
    message.esm.h.message_type = 
        NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST;

#if 0 /* default bearer is not needed */
    eps_qos->length = 5;
    eps_qos->qci = pdn->qos.qci;
    eps_qos->ul_mbr = 0xff;
    eps_qos->dl_mbr = 0xff;
    eps_qos->ul_gbr = 0xff;
    eps_qos->dl_gbr = 0xff;
#else
    eps_qos->length = 1;
    eps_qos->qci = pdn->qos.qci;
#endif

    access_point_name->length = strlen(pdn->apn);
    core_cpystrn(access_point_name->apn, pdn->apn,
            c_min(access_point_name->length, MAX_APN_LEN) + 1);

    pdn_address->length = PAA_IPV4_LEN;
    memcpy(&pdn_address->paa, &pdn->paa, pdn_address->length);

    if (pdn->ambr.downlink || pdn->ambr.uplink)
    {
        activate_default_eps_bearer_context_request->presencemask |=
            NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_APN_AMBR_PRESENT;
        apn_ambr_build(apn_ambr, pdn->ambr.downlink, pdn->ambr.uplink);
    }

    if (sess->pgw_pco_len)
    {
        activate_default_eps_bearer_context_request->presencemask |=
            NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
        protocol_configuration_options->length = sess->pgw_pco_len;
        memcpy(protocol_configuration_options->buffer, 
                sess->pgw_pco, protocol_configuration_options->length);
    }

    d_assert(nas_plain_encode(pkbuf, &message) == CORE_OK && *pkbuf,,);

    return CORE_OK;
}
