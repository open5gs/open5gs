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
    if (FSM_CHECK(&mme_ue->sm, emm_state_attached))
    {
        message.h.security_header_type = 
           NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
        message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    }
    message.esm.h.eps_bearer_identity = bearer->ebi;
    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    if (!FSM_CHECK(&mme_ue->sm, emm_state_attached))
        message.esm.h.procedure_transaction_identity = bearer->pti;
    message.esm.h.message_type = 
        NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST;

    eps_qos_build(eps_qos, pdn->qos.qci,
            pdn->qos.mbr.downlink, pdn->qos.mbr.uplink,
            pdn->qos.gbr.downlink, pdn->qos.gbr.uplink);

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

    if (sess->pgw_pco.presence && sess->pgw_pco.len && sess->pgw_pco.data)
    {
        activate_default_eps_bearer_context_request->presencemask |=
            NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST_PROTOCOL_CONFIGURATION_OPTIONS_PRESENT;
        protocol_configuration_options->length = sess->pgw_pco.len;
        memcpy(protocol_configuration_options->buffer, 
                sess->pgw_pco.data, protocol_configuration_options->length);

        TLV_CLEAR_DATA(&sess->pgw_pco);
    }

    if (FSM_CHECK(&mme_ue->sm, emm_state_attached))
    {
        d_assert(nas_security_encode(pkbuf, mme_ue, &message) == CORE_OK && 
                *pkbuf,,);
    }
    else
    {
        d_assert(nas_plain_encode(pkbuf, &message) == CORE_OK && *pkbuf,,);
    }

    return CORE_OK;
}

status_t esm_build_activate_dedicated_bearer_context(
        pkbuf_t **pkbuf, mme_bearer_t *bearer)
{
    mme_ue_t *mme_ue = NULL;
    mme_bearer_t *linked_bearer = NULL;

    nas_message_t message;
    nas_activate_dedicated_eps_bearer_context_request_t 
        *activate_dedicated_eps_bearer_context_request = 
            &message.esm.activate_dedicated_eps_bearer_context_request;
    nas_linked_eps_bearer_identity_t *linked_ebi =
        &activate_dedicated_eps_bearer_context_request->
            linked_eps_bearer_identity;
    nas_eps_quality_of_service_t *eps_qos =
        &activate_dedicated_eps_bearer_context_request->eps_qos;
    nas_traffic_flow_template_t *tft = 
        &activate_dedicated_eps_bearer_context_request->tft;
    
    d_assert(bearer, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    linked_bearer = mme_linked_bearer(bearer); 
    d_assert(linked_bearer, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.esm.h.eps_bearer_identity = bearer->ebi;
    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = 0;
    message.esm.h.message_type = 
        NAS_ACTIVATE_DEDICATED_EPS_BEARER_CONTEXT_REQUEST;

    linked_ebi->eps_bearer_identity = linked_bearer->ebi;
    eps_qos_build(eps_qos, bearer->qos.qci,
            bearer->qos.mbr.downlink, bearer->qos.mbr.uplink,
            bearer->qos.gbr.downlink, bearer->qos.gbr.uplink);

    tft->length = bearer->tft.len;
    d_assert(tft->length, return CORE_ERROR, "No TFT Len");
    d_assert(bearer->tft.data, return CORE_ERROR, "Null param");
    memcpy(tft->buffer, bearer->tft.data, tft->length);

    TLV_CLEAR_DATA(&bearer->tft);

    d_assert(nas_security_encode(pkbuf, mme_ue, &message) == CORE_OK && 
            *pkbuf,,);

    return CORE_OK;
}
