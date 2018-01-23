#define TRACE_MODULE _esm_build

#include "core_debug.h"
#include "core_lib.h"

#include "nas/nas_message.h"

#include "nas_security.h"
#include "esm_build.h"
#include "mme_sm.h"

status_t esm_build_pdn_connectivity_reject(
        pkbuf_t **pkbuf, mme_sess_t *sess, nas_esm_cause_t esm_cause)
{
    mme_ue_t *mme_ue = NULL;
    nas_message_t message;
    nas_pdn_connectivity_reject_t *pdn_connectivity_reject = 
            &message.esm.pdn_connectivity_reject;

    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = sess->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    d_trace(3, "[ESM] PDN connectivity reject\n");
    d_trace(5, "    IMSI[%s] PTI[%d] Cause[%d]\n",
            mme_ue->imsi_bcd, sess->pti, esm_cause);

    memset(&message, 0, sizeof(message));
    if (FSM_CHECK(&mme_ue->sm, emm_state_registered))
    {
        message.h.security_header_type = 
           NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
        message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    }
    message.esm.h.eps_bearer_identity = 0;
    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = sess->pti;
    message.esm.h.message_type = NAS_PDN_CONNECTIVITY_REJECT;

    pdn_connectivity_reject->esm_cause = esm_cause;

    if (FSM_CHECK(&mme_ue->sm, emm_state_registered))
    {
        d_assert(nas_security_encode(pkbuf, mme_ue, &message) == CORE_OK && 
                *pkbuf, return CORE_ERROR,);
    }
    else
    {
        d_assert(nas_plain_encode(pkbuf, &message) == CORE_OK && *pkbuf,
                return CORE_ERROR,);
    }

    return CORE_OK;
}

status_t esm_build_information_request(pkbuf_t **pkbuf, mme_bearer_t *bearer)
{
    nas_message_t message;
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;

    d_assert(bearer, return CORE_ERROR, "Null param");
    sess = bearer->sess;
    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    d_trace(3, "[ESM] ESM information request\n");
    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
            mme_ue->imsi_bcd, sess->pti, bearer->ebi);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = sess->pti;
    message.esm.h.message_type = NAS_ESM_INFORMATION_REQUEST;

    d_assert(nas_security_encode(pkbuf, mme_ue, &message) == CORE_OK && 
            *pkbuf, return CORE_ERROR,);

    return CORE_OK;
}

status_t esm_build_activate_default_bearer_context_request(
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
    d_assert(mme_bearer_next(bearer) == NULL,
            return CORE_ERROR, "there is dedicated bearer");

    d_trace(3, "[ESM] Activate default bearer context request\n");
    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
            mme_ue->imsi_bcd, sess->pti, bearer->ebi);

    memset(&message, 0, sizeof(message));
    if (FSM_CHECK(&mme_ue->sm, emm_state_registered))
    {
        message.h.security_header_type = 
           NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
        message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    }
    message.esm.h.eps_bearer_identity = bearer->ebi;
    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = sess->pti;
    message.esm.h.message_type = 
        NAS_ACTIVATE_DEFAULT_EPS_BEARER_CONTEXT_REQUEST;

    memcpy(&bearer->qos, &pdn->qos, sizeof(qos_t));

    eps_qos_build(eps_qos, bearer->qos.qci,
            bearer->qos.mbr.downlink, bearer->qos.mbr.uplink,
            bearer->qos.gbr.downlink, bearer->qos.gbr.uplink);

    access_point_name->length = strlen(pdn->apn);
    core_cpystrn(access_point_name->apn, pdn->apn,
            c_min(access_point_name->length, MAX_APN_LEN) + 1);
    d_trace(5, "    APN[%s]\n", pdn->apn);

    pdn_address->pdn_type = pdn->paa.pdn_type;
    if (pdn_address->pdn_type == GTP_PDN_TYPE_IPV4)
    {
        pdn_address->addr = pdn->paa.addr;
        pdn_address->length = NAS_PDN_ADDRESS_IPV4_LEN;
        d_trace(5, "    IPv4\n");
    }
    else if (pdn_address->pdn_type == GTP_PDN_TYPE_IPV6)
    {
        memcpy(pdn_address->addr6, pdn->paa.addr6+(IPV6_LEN>>1), IPV6_LEN>>1);
        pdn_address->length = NAS_PDN_ADDRESS_IPV6_LEN;
        d_trace(5, "    IPv6\n");
    }
    else if (pdn_address->pdn_type == GTP_PDN_TYPE_IPV4V6)
    {
        pdn_address->both.addr = pdn->paa.both.addr;
        memcpy(pdn_address->both.addr6,
                pdn->paa.both.addr6+(IPV6_LEN>>1), IPV6_LEN>>1);
        pdn_address->length = NAS_PDN_ADDRESS_IPV4V6_LEN;
        d_trace(5, "    IPv4v6\n");
    }
    else
        d_assert(0, return CORE_ERROR,
                "Invalid PDN_TYPE(%d)", pdn->paa.pdn_type);

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
    }

    if (FSM_CHECK(&mme_ue->sm, emm_state_registered))
    {
        d_assert(nas_security_encode(pkbuf, mme_ue, &message) == CORE_OK && 
                *pkbuf, return CORE_ERROR,);
    }
    else
    {
        d_assert(nas_plain_encode(pkbuf, &message) == CORE_OK && *pkbuf,
                return CORE_ERROR,);
    }

    return CORE_OK;
}

status_t esm_build_activate_dedicated_bearer_context_request(
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

    d_trace(3, "[ESM] Activate dedicated bearer context request\n");
    d_trace(5, "    IMSI[%s] EBI[%d] Linked-EBI[%d]\n",
            mme_ue->imsi_bcd, bearer->ebi, linked_bearer->ebi);

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

    d_assert(nas_security_encode(pkbuf, mme_ue, &message) == CORE_OK && 
            *pkbuf, return CORE_ERROR,);

    return CORE_OK;
}

status_t esm_build_modify_bearer_context_request(
        pkbuf_t **pkbuf, mme_bearer_t *bearer, int qos_presence, int tft_presence)
{
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;

    nas_message_t message;
    nas_modify_eps_bearer_context_request_t 
        *modify_eps_bearer_context_request = 
            &message.esm.modify_eps_bearer_context_request;
    nas_eps_quality_of_service_t *new_eps_qos =
        &modify_eps_bearer_context_request->new_eps_qos;
    nas_traffic_flow_template_t *tft = 
        &modify_eps_bearer_context_request->tft;

    d_assert(bearer, return CORE_ERROR, "Null param");
    sess = bearer->sess;
    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    d_trace(3, "[ESM] Modify bearer context request\n");
    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
            mme_ue->imsi_bcd, sess->pti, bearer->ebi);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.esm.h.eps_bearer_identity = bearer->ebi;
    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = sess->pti;
    message.esm.h.message_type = NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST;

    if (qos_presence == 1)
    {
        modify_eps_bearer_context_request->presencemask |=
            NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_NEW_EPS_QOS_PRESENT;
        eps_qos_build(new_eps_qos, bearer->qos.qci,
                bearer->qos.mbr.downlink, bearer->qos.mbr.uplink,
                bearer->qos.gbr.downlink, bearer->qos.gbr.uplink);
    }

    if (tft_presence == 1)
    {
        modify_eps_bearer_context_request->presencemask |=
            NAS_MODIFY_EPS_BEARER_CONTEXT_REQUEST_TFT_PRESENT;
        tft->length = bearer->tft.len;
        d_assert(tft->length, return CORE_ERROR, "No TFT Len");
        d_assert(bearer->tft.data, return CORE_ERROR, "Null param");
        memcpy(tft->buffer, bearer->tft.data, tft->length);
    }

    d_assert(nas_security_encode(pkbuf, mme_ue, &message) == CORE_OK && 
            *pkbuf, return CORE_ERROR,);

    return CORE_OK;
}

status_t esm_build_deactivate_bearer_context_request(
        pkbuf_t **pkbuf, mme_bearer_t *bearer, nas_esm_cause_t esm_cause)
{
    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;

    nas_message_t message;
    nas_deactivate_eps_bearer_context_request_t 
        *deactivate_eps_bearer_context_request = 
            &message.esm.deactivate_eps_bearer_context_request;
    
    d_assert(bearer, return CORE_ERROR, "Null param");
    sess = bearer->sess;
    d_assert(sess, return CORE_ERROR, "Null param");
    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");

    d_trace(3, "[ESM] Deactivate bearer context request\n");
    d_trace(5, "    IMSI[%s] PTI[%d] EBI[%d]\n",
            mme_ue->imsi_bcd, sess->pti, bearer->ebi);
    d_trace(5, "    Cause[%d]\n", esm_cause);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.esm.h.eps_bearer_identity = bearer->ebi;
    message.esm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_ESM;
    message.esm.h.procedure_transaction_identity = sess->pti;
    message.esm.h.message_type = NAS_DEACTIVATE_EPS_BEARER_CONTEXT_REQUEST;

    deactivate_eps_bearer_context_request->esm_cause = esm_cause;

    d_assert(nas_security_encode(pkbuf, mme_ue, &message) == CORE_OK && 
            *pkbuf, return CORE_ERROR,);

    return CORE_OK;
}
