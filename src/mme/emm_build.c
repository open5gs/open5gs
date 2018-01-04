#define TRACE_MODULE _emm_build

#include "core_debug.h"

#include "nas/nas_message.h"

#include "nas_security.h"
#include "mme_kdf.h"
#include "emm_build.h"

status_t emm_build_attach_accept(
        pkbuf_t **emmbuf, mme_ue_t *mme_ue, pkbuf_t *esmbuf)
{
    status_t rv;
    nas_message_t message;
    nas_attach_accept_t *attach_accept = &message.emm.attach_accept;
    nas_eps_attach_result_t *eps_attach_result = 
        &attach_accept->eps_attach_result;
    nas_gprs_timer_t *t3412_value = &attach_accept->t3412_value;
    int served_tai_index = 0;
    nas_eps_mobile_identity_t *guti = &attach_accept->guti;
    nas_gprs_timer_t *t3402_value = &attach_accept->t3402_value;
    nas_gprs_timer_t *t3423_value = &attach_accept->t3423_value;
    nas_eps_network_feature_support_t *eps_network_feature_support =
        &attach_accept->eps_network_feature_support;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(mme_ue->enb_ue, return CORE_ERROR, "Null param");
    d_assert(esmbuf, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_ATTACH_ACCEPT;

    eps_attach_result->result = NAS_ATTACH_RESULT_COMBINED_EPS_IMSI_ATTACH;
    t3412_value->unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    t3412_value->value = 9;

    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    d_assert(served_tai_index >= 0 &&
            served_tai_index < MAX_NUM_OF_SERVED_TAI, return CORE_ERROR,
            "Cannot find Served TAI. Check 'mme.tai' configuration");
    nas_tai_list_build(&attach_accept->tai_list,
            &mme_self()->served_tai[served_tai_index].list0,
            &mme_self()->served_tai[served_tai_index].list2);

    attach_accept->esm_message_container.buffer = esmbuf->payload;
    attach_accept->esm_message_container.length = esmbuf->len;

    attach_accept->presencemask |= NAS_ATTACH_ACCEPT_GUTI_PRESENT;
    guti->length = sizeof(nas_eps_mobile_identity_guti_t);
    guti->guti.odd_even = NAS_EPS_MOBILE_IDENTITY_EVEN;
    guti->guti.type = NAS_EPS_MOBILE_IDENTITY_GUTI;
    memcpy(&guti->guti.plmn_id, &mme_ue->guti.plmn_id, PLMN_ID_LEN);
    guti->guti.mme_gid = mme_ue->guti.mme_gid;
    guti->guti.mme_code = mme_ue->guti.mme_code;
    guti->guti.m_tmsi = mme_ue->guti.m_tmsi;

    attach_accept->presencemask |= NAS_ATTACH_ACCEPT_EMM_CAUSE_PRESENT;
    attach_accept->emm_cause = EMM_CAUSE_CS_DOMAIN_NOT_AVAILABLE;
    attach_accept->presencemask |= NAS_ATTACH_ACCEPT_T3402_VALUE_PRESENT;
    t3402_value->unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_1_MM;
    t3402_value->value = 12;
    attach_accept->presencemask |= NAS_ATTACH_ACCEPT_T3423_VALUE_PRESENT;
    t3423_value->unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    t3423_value->value = 9;
    attach_accept->presencemask |= 
        NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
    eps_network_feature_support->length = 1;
    eps_network_feature_support->esr_ps = 1;
    eps_network_feature_support->epc_lcs = 1;
    eps_network_feature_support->ims_vops = 1;

    rv = nas_security_encode(emmbuf, mme_ue, &message);
    d_assert(rv == CORE_OK && *emmbuf,, "nas_security_encode error");
    pkbuf_free(esmbuf);

    return CORE_OK;
}

status_t emm_build_attach_reject(
        pkbuf_t **emmbuf, nas_emm_cause_t emm_cause, pkbuf_t *esmbuf)
{
    status_t rv;
    nas_message_t message;
    nas_attach_reject_t *attach_reject = &message.emm.attach_reject;

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_ATTACH_REJECT;

    attach_reject->emm_cause = emm_cause;

    if (esmbuf)
    {
        attach_reject->presencemask |= 
            NAS_ATTACH_REJECT_ESM_MESSAGE_CONTAINER_PRESENT;
        attach_reject->esm_message_container.buffer = esmbuf->payload;
        attach_reject->esm_message_container.length = esmbuf->len;
    }

    rv = nas_plain_encode(emmbuf, &message);
    d_assert(rv == CORE_OK && *emmbuf,, "nas_plain_encode error");

    if (esmbuf)
    {
        pkbuf_free(esmbuf);
    }

    return rv;
}

status_t emm_build_identity_request(
        pkbuf_t **emmbuf, mme_ue_t *mme_ue)
{
    status_t rv;
    nas_message_t message;
    nas_identity_request_t *identity_request = 
        &message.emm.identity_request;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_IDENTITY_REQUEST;

    /* Request IMSI */
    identity_request->identity_type.type = NAS_IDENTITY_TYPE_2_IMSI;

    rv = nas_plain_encode(emmbuf, &message);
    d_assert(rv == CORE_OK && *emmbuf,, "nas_plain_encode error");

    return rv;
}

status_t emm_build_authentication_request(
        pkbuf_t **emmbuf, e_utran_vector_t *e_utran_vector)
{
    status_t rv;
    nas_message_t message;
    nas_authentication_request_t *authentication_request = 
        &message.emm.authentication_request;

    d_assert(e_utran_vector, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_AUTHENTICATION_REQUEST;

    memcpy(authentication_request->authentication_parameter_rand.rand,
            e_utran_vector->rand, RAND_LEN);
    memcpy(authentication_request->authentication_parameter_autn.autn,
            e_utran_vector->autn, AUTN_LEN);
    authentication_request->authentication_parameter_autn.length = 
            AUTN_LEN;

    rv = nas_plain_encode(emmbuf, &message);
    d_assert(rv == CORE_OK && *emmbuf, , "nas encode error");

    return rv;
}

status_t emm_build_authentication_reject(pkbuf_t **emmbuf)
{
    status_t rv;
    nas_message_t message;

    memset(&message, 0, sizeof(message));

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_AUTHENTICATION_REJECT;

    rv = nas_plain_encode(emmbuf, &message);
    d_assert(rv == CORE_OK && *emmbuf,, "nas_plain_encode error");

    return rv;
}

status_t emm_build_security_mode_command(
        pkbuf_t **emmbuf, mme_ue_t *mme_ue)
{
    status_t rv;
    int i;

    nas_message_t message;
    nas_security_mode_command_t *security_mode_command = 
        &message.emm.security_mode_command;
    nas_security_algorithms_t *selected_nas_security_algorithms =
        &security_mode_command->selected_nas_security_algorithms;
    nas_key_set_identifier_t *nas_key_set_identifier =
        &security_mode_command->nas_key_set_identifier;
    nas_ue_security_capability_t *replayed_ue_security_capabilities = 
        &security_mode_command->replayed_ue_security_capabilities;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_NEW_SECURITY_CONTEXT;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_SECURITY_MODE_COMMAND;

    for (i = 0; i < mme_self()->num_of_integrity_order; i++)
    {
        if (mme_ue->ue_network_capability.eia & 
                (0x80 >> mme_self()->integrity_order[i]))
        {
            mme_ue->selected_int_algorithm = mme_self()->integrity_order[i];
            break;
        }
    }
    for (i = 0; i < mme_self()->num_of_ciphering_order; i++)
    {
        if (mme_ue->ue_network_capability.eea & 
                (0x80 >> mme_self()->ciphering_order[i]))
        {
            mme_ue->selected_enc_algorithm = mme_self()->ciphering_order[i];
            break;
        }
    }

    selected_nas_security_algorithms->type_of_integrity_protection_algorithm =
        mme_ue->selected_int_algorithm;
    selected_nas_security_algorithms->type_of_ciphering_algorithm =
        mme_ue->selected_enc_algorithm;

    nas_key_set_identifier->tsc = 0;
    nas_key_set_identifier->nas_key_set_identifier = 0;

    replayed_ue_security_capabilities->length =
        sizeof(replayed_ue_security_capabilities->eea) +
        sizeof(replayed_ue_security_capabilities->eia) +
        sizeof(replayed_ue_security_capabilities->uea) +
        sizeof(replayed_ue_security_capabilities->uia) +
        sizeof(replayed_ue_security_capabilities->gea);
    replayed_ue_security_capabilities->eea = mme_ue->ue_network_capability.eea;
    replayed_ue_security_capabilities->eia = mme_ue->ue_network_capability.eia;
    replayed_ue_security_capabilities->uea = mme_ue->ue_network_capability.uea;
    replayed_ue_security_capabilities->uia = mme_ue->ue_network_capability.uia;
    replayed_ue_security_capabilities->gea = 
        (mme_ue->ms_network_capability.gea1 << 6) | 
        mme_ue->ms_network_capability.extended_gea;

    mme_kdf_nas(MME_KDF_NAS_INT_ALG, mme_ue->selected_int_algorithm,
            mme_ue->kasme, mme_ue->knas_int);
    mme_kdf_nas(MME_KDF_NAS_ENC_ALG, mme_ue->selected_enc_algorithm,
            mme_ue->kasme, mme_ue->knas_enc);

    rv = nas_security_encode(emmbuf, mme_ue, &message);
    d_assert(rv == CORE_OK && *emmbuf, return CORE_ERROR, "emm build error");

    return CORE_OK;
}

status_t emm_build_detach_accept(pkbuf_t **emmbuf, mme_ue_t *mme_ue)
{
    status_t rv;
    nas_message_t message;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
        NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_DETACH_ACCEPT;

    rv = nas_security_encode(emmbuf, mme_ue, &message);
    d_assert(rv == CORE_OK && emmbuf, return CORE_ERROR, "emm build error");

    return CORE_OK;
}

status_t emm_build_tau_accept(pkbuf_t **emmbuf, mme_ue_t *mme_ue)
{
    nas_message_t message;
    nas_tracking_area_update_accept_t *tau_accept = 
        &message.emm.tracking_area_update_accept;
    int served_tai_index = 0;

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_TRACKING_AREA_UPDATE_ACCEPT;

    tau_accept->eps_update_result.result = NAS_EPS_UPDATE_RESULT_TA_UPDATED;

    /* Set T3412 */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_PRESENT ;

    /* FIXME: Use the  value from  configuration */
    tau_accept->t3412_value.unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    tau_accept->t3412_value.value = 9;

    /* Set TAI */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_PRESENT;

    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    d_assert(served_tai_index >= 0 &&
            served_tai_index < MAX_NUM_OF_SERVED_TAI, return CORE_ERROR,
            "Cannot find Served TAI. Check 'mme.tai' configuration");
    nas_tai_list_build(&tau_accept->tai_list,
            &mme_self()->served_tai[served_tai_index].list0,
            &mme_self()->served_tai[served_tai_index].list2);

    /* Set EPS bearer context status */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT;

    tau_accept->eps_bearer_context_status.length = 2;
    tau_accept->eps_bearer_context_status.ebi5 = 
        (mme_bearer_find_by_ue_ebi(mme_ue, 5) ? 1 : 0);
    tau_accept->eps_bearer_context_status.ebi6 = 
        (mme_bearer_find_by_ue_ebi(mme_ue, 6) ? 1 : 0);
    tau_accept->eps_bearer_context_status.ebi7 = 
        (mme_bearer_find_by_ue_ebi(mme_ue, 7) ? 1 : 0);
    /* FIXME : Need to set other ebi */

    /* Set T3402 */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_PRESENT;

    /* FIXME: Use the  value from  configuration */
    tau_accept->t3402_value.unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_1_MM;
    tau_accept->t3402_value.value = 12;

    /* Set T3423 */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_PRESENT;

    /* FIXME: Use the  value from  configuration */
    tau_accept->t3423_value.unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    tau_accept->t3423_value.value = 9;

    /* Set EPS network feature support */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;

    tau_accept->eps_network_feature_support.length = 1;
    tau_accept->eps_network_feature_support.esr_ps = 1;
    tau_accept->eps_network_feature_support.epc_lcs = 1;
    tau_accept->eps_network_feature_support.ims_vops = 1;

    d_assert(nas_security_encode(emmbuf, mme_ue, &message) == CORE_OK && 
            *emmbuf,,);

    return CORE_OK;
}

status_t emm_build_tau_reject(pkbuf_t **emmbuf, nas_emm_cause_t emm_cause,
        mme_ue_t *mme_ue)
{
    nas_message_t message;
    nas_tracking_area_update_reject_t *tau_reject = 
        &message.emm.tracking_area_update_reject;

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_TRACKING_AREA_UPDATE_REJECT;

    tau_reject->emm_cause = emm_cause;

    d_assert(nas_plain_encode(emmbuf, &message) == CORE_OK && *emmbuf,,);

    return CORE_OK;
}

status_t emm_build_service_reject(pkbuf_t **emmbuf, nas_emm_cause_t emm_cause, 
        mme_ue_t *mme_ue)
{
    nas_message_t message;
    nas_service_reject_t *service_reject = &message.emm.service_reject;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_SERVICE_REJECT;

    service_reject->emm_cause = emm_cause;

    d_assert(nas_plain_encode(emmbuf, &message) == CORE_OK && *emmbuf,,);

    return CORE_OK;
}
