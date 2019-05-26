#include "nas/nas_message.h"

#include "nas_security.h"
#include "mme_kdf.h"
#include "emm_build.h"

#undef OGS_LOG_DOMAIN
#define OGS_LOG_DOMAIN __emm_log_domain

int emm_build_attach_accept(
        ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue, ogs_pkbuf_t *esmbuf)
{
    int rv;
    nas_message_t message;
    nas_attach_accept_t *attach_accept = &message.emm.attach_accept;
    nas_eps_attach_result_t *eps_attach_result = 
        &attach_accept->eps_attach_result;
    nas_gprs_timer_t *t3412_value = &attach_accept->t3412_value;
    int served_tai_index = 0;
    nas_eps_mobile_identity_t *nas_guti = &attach_accept->guti;
    nas_eps_network_feature_support_t *eps_network_feature_support =
        &attach_accept->eps_network_feature_support;

    ogs_assert(mme_ue);
    ogs_assert(esmbuf);

    ogs_debug("[EMM] Attach accept");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_ATTACH_ACCEPT;

    /* Set T3412 */
    eps_attach_result->result = mme_ue->nas_eps.attach.attach_type;
    t3412_value->unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    t3412_value->value = 9;

    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            plmn_id_hexdump(&mme_ue->tai.plmn_id),
            mme_ue->tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            plmn_id_hexdump(&mme_ue->e_cgi.plmn_id),
            mme_ue->e_cgi.cell_id);
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);
    ogs_assert(served_tai_index >= 0 &&
            served_tai_index < MAX_NUM_OF_SERVED_TAI);
    nas_tai_list_build(&attach_accept->tai_list,
            &mme_self()->served_tai[served_tai_index].list0,
            &mme_self()->served_tai[served_tai_index].list2);

    attach_accept->esm_message_container.buffer = esmbuf->data;
    attach_accept->esm_message_container.length = esmbuf->len;

    ogs_debug("    %s GUTI[G:%d,C:%d,M_TMSI:0x%x] IMSI:[%s]",
            mme_ue->guti_present ? "[V]" : "[N]",
            mme_ue->guti.mme_gid, mme_ue->guti.mme_code,
            mme_ue->guti.m_tmsi, mme_ue->imsi_bcd);
    if (mme_ue->guti_present)
    {
        attach_accept->presencemask |= NAS_ATTACH_ACCEPT_GUTI_PRESENT;
        nas_guti->length = sizeof(nas_eps_mobile_identity_guti_t);
        nas_guti->guti.odd_even = NAS_EPS_MOBILE_IDENTITY_EVEN;
        nas_guti->guti.type = NAS_EPS_MOBILE_IDENTITY_GUTI;
        nas_guti->guti.plmn_id = mme_ue->guti.plmn_id;
        nas_guti->guti.mme_gid = mme_ue->guti.mme_gid;
        nas_guti->guti.mme_code = mme_ue->guti.mme_code;
        nas_guti->guti.m_tmsi = mme_ue->guti.m_tmsi;
    }
    mme_ue->guti_present = 0;

#if 0 /* Need not to include T3402 */
    /* Set T3402 */
    attach_accept->presencemask |= NAS_ATTACH_ACCEPT_T3402_VALUE_PRESENT;
    attach_accept->t3402_value.unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_1_MM;
    attach_accept->t3402_value.value = 12;
#endif

    /* Set T3423 */
    attach_accept->presencemask |= NAS_ATTACH_ACCEPT_T3423_VALUE_PRESENT;
    attach_accept->t3423_value.unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    attach_accept->t3423_value.value = 9;
    attach_accept->presencemask |= 
        NAS_ATTACH_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
    eps_network_feature_support->length = 1;
    eps_network_feature_support->ims_vops = 1;

    rv = nas_security_encode(emmbuf, mme_ue, &message);
    ogs_assert(rv == OGS_OK && *emmbuf);
    ogs_pkbuf_free(esmbuf);

    return OGS_OK;
}

int emm_build_attach_reject(
        ogs_pkbuf_t **emmbuf, nas_emm_cause_t emm_cause, ogs_pkbuf_t *esmbuf)
{
    int rv;
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
        attach_reject->esm_message_container.buffer = esmbuf->data;
        attach_reject->esm_message_container.length = esmbuf->len;
    }

    rv = nas_plain_encode(emmbuf, &message);
    ogs_assert(rv == OGS_OK && *emmbuf);

    if (esmbuf)
    {
        ogs_pkbuf_free(esmbuf);
    }

    return rv;
}

int emm_build_identity_request(
        ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue)
{
    int rv;
    nas_message_t message;
    nas_identity_request_t *identity_request = 
        &message.emm.identity_request;

    ogs_assert(mme_ue);

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_IDENTITY_REQUEST;

    /* Request IMSI */
    ogs_debug("[EMM] Identity request");
    ogs_debug("    Identity Type 2 : IMSI");
    identity_request->identity_type.type = NAS_IDENTITY_TYPE_2_IMSI;

    rv = nas_plain_encode(emmbuf, &message);
    ogs_assert(rv == OGS_OK && *emmbuf);

    return rv;
}

int emm_build_authentication_request(
        ogs_pkbuf_t **emmbuf, e_utran_vector_t *e_utran_vector)
{
    int rv;
    nas_message_t message;
    nas_authentication_request_t *authentication_request = 
        &message.emm.authentication_request;

    ogs_assert(e_utran_vector);

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
    ogs_assert(rv == OGS_OK && *emmbuf);

    return rv;
}

int emm_build_authentication_reject(ogs_pkbuf_t **emmbuf)
{
    int rv;
    nas_message_t message;

    memset(&message, 0, sizeof(message));

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_AUTHENTICATION_REJECT;

    rv = nas_plain_encode(emmbuf, &message);
    ogs_assert(rv == OGS_OK && *emmbuf);

    return rv;
}

int emm_build_security_mode_command(
        ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue)
{
    int rv;
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

    ogs_assert(mme_ue);

    ogs_debug("[EMM] Security mode command");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

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

    replayed_ue_security_capabilities->eea = mme_ue->ue_network_capability.eea;
    replayed_ue_security_capabilities->eia = mme_ue->ue_network_capability.eia;
    replayed_ue_security_capabilities->uea = mme_ue->ue_network_capability.uea;
    replayed_ue_security_capabilities->uia = 
        mme_ue->ue_network_capability.uia & 0x7f;
    replayed_ue_security_capabilities->gea = 
        (mme_ue->ms_network_capability.gea1 << 6) | 
        mme_ue->ms_network_capability.extended_gea;

    replayed_ue_security_capabilities->length =
        sizeof(replayed_ue_security_capabilities->eea) +
        sizeof(replayed_ue_security_capabilities->eia);
    if (replayed_ue_security_capabilities->uea ||
        replayed_ue_security_capabilities->uia)
        replayed_ue_security_capabilities->length =
            sizeof(replayed_ue_security_capabilities->eea) +
            sizeof(replayed_ue_security_capabilities->eia) +
            sizeof(replayed_ue_security_capabilities->uea) +
            sizeof(replayed_ue_security_capabilities->uia);
    if (replayed_ue_security_capabilities->gea)
        replayed_ue_security_capabilities->length =
            sizeof(replayed_ue_security_capabilities->eea) +
            sizeof(replayed_ue_security_capabilities->eia) +
            sizeof(replayed_ue_security_capabilities->uea) +
            sizeof(replayed_ue_security_capabilities->uia) +
            sizeof(replayed_ue_security_capabilities->gea);
    ogs_debug("    Replayed UE SEC[LEN:%d EEA:0x%x EIA:0x%x UEA:0x%x UIA:0x%x GEA:0x%x]",
            replayed_ue_security_capabilities->length,
            replayed_ue_security_capabilities->eea,
            replayed_ue_security_capabilities->eia,
            replayed_ue_security_capabilities->uea,
            replayed_ue_security_capabilities->uia,
            replayed_ue_security_capabilities->gea);
    ogs_debug("    Selected[Integrity:0x%x Encrypt:0x%x]",
            mme_ue->selected_int_algorithm, mme_ue->selected_enc_algorithm);
    if (mme_ue->selected_int_algorithm == NAS_SECURITY_ALGORITHMS_EIA0)
    {
        ogs_fatal("Encrypt[0x%x] can be skipped with EEA0, "
            "but Integrity[0x%x] cannot be bypassed with EIA0",
            mme_ue->selected_enc_algorithm, mme_ue->selected_int_algorithm);
        ogs_assert_if_reached();
        return OGS_ERROR;
    }


    mme_kdf_nas(MME_KDF_NAS_INT_ALG, mme_ue->selected_int_algorithm,
            mme_ue->kasme, mme_ue->knas_int);
    mme_kdf_nas(MME_KDF_NAS_ENC_ALG, mme_ue->selected_enc_algorithm,
            mme_ue->kasme, mme_ue->knas_enc);

    rv = nas_security_encode(emmbuf, mme_ue, &message);
    ogs_assert(rv == OGS_OK && *emmbuf);

    return OGS_OK;
}

int emm_build_detach_accept(ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue)
{
    int rv;
    nas_message_t message;

    ogs_assert(mme_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
        NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    ogs_debug("[EMM] Detach accept");
    ogs_debug("    IMSI[%s]", mme_ue->imsi_bcd);

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_DETACH_ACCEPT;

    rv = nas_security_encode(emmbuf, mme_ue, &message);
    ogs_assert(rv == OGS_OK && emmbuf);

    return OGS_OK;
}

int emm_build_tau_accept(ogs_pkbuf_t **emmbuf, mme_ue_t *mme_ue)
{
    nas_message_t message;
    nas_tracking_area_update_accept_t *tau_accept = 
        &message.emm.tracking_area_update_accept;
    int served_tai_index = 0;

    mme_sess_t *sess = NULL;

    ogs_assert(mme_ue);

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
        NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_TRACKING_AREA_UPDATE_ACCEPT;

    tau_accept->eps_update_result.result = mme_ue->nas_eps.update.update_type;

    /* Set T3412 */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_T3412_VALUE_PRESENT ;
    tau_accept->t3412_value.unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    tau_accept->t3412_value.value = 9;

    /* Set TAI */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_TAI_LIST_PRESENT;

    ogs_debug("    TAI[PLMN_ID:%06x,TAC:%d]",
            plmn_id_hexdump(&mme_ue->tai.plmn_id),
            mme_ue->tai.tac);
    ogs_debug("    E_CGI[PLMN_ID:%06x,CELL_ID:%d]",
            plmn_id_hexdump(&mme_ue->e_cgi.plmn_id),
            mme_ue->e_cgi.cell_id);
    served_tai_index = mme_find_served_tai(&mme_ue->tai);
    ogs_debug("    SERVED_TAI_INDEX[%d]", served_tai_index);
    ogs_assert(served_tai_index >= 0 &&
            served_tai_index < MAX_NUM_OF_SERVED_TAI);
    nas_tai_list_build(&tau_accept->tai_list,
            &mme_self()->served_tai[served_tai_index].list0,
            &mme_self()->served_tai[served_tai_index].list2);

    /* Set EPS bearer context status */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_BEARER_CONTEXT_STATUS_PRESENT;
    tau_accept->eps_bearer_context_status.length = 2;
    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        mme_bearer_t *bearer = mme_bearer_first(sess);
        while(bearer)
        {
            switch(bearer->ebi)
            {
                case 5: tau_accept->eps_bearer_context_status.ebi5 = 1; break;
                case 6: tau_accept->eps_bearer_context_status.ebi6 = 1; break;
                case 7: tau_accept->eps_bearer_context_status.ebi7 = 1; break;
                case 8: tau_accept->eps_bearer_context_status.ebi8 = 1; break;
                case 9: tau_accept->eps_bearer_context_status.ebi9 = 1; break;
                case 10: tau_accept->eps_bearer_context_status.ebi10 = 1; break;
                case 11: tau_accept->eps_bearer_context_status.ebi11 = 1; break;
                case 12: tau_accept->eps_bearer_context_status.ebi12 = 1; break;
                case 13: tau_accept->eps_bearer_context_status.ebi13 = 1; break;
                case 14: tau_accept->eps_bearer_context_status.ebi14 = 1; break;
                case 15: tau_accept->eps_bearer_context_status.ebi15 = 1; break;
                default: break;
            }

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }

#if 0 /* Need not to include T3402 */
    /* Set T3402 */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_T3402_VALUE_PRESENT;
    tau_accept->t3402_value.unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_1_MM;
    tau_accept->t3402_value.value = 12;
#endif

    /* Set T3423 */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_T3423_VALUE_PRESENT;
    tau_accept->t3423_value.unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    tau_accept->t3423_value.value = 9;

    /* Set EPS network feature support */
    tau_accept->presencemask |= 
        NAS_TRACKING_AREA_UPDATE_ACCEPT_EPS_NETWORK_FEATURE_SUPPORT_PRESENT;
    tau_accept->eps_network_feature_support.length = 1;
    tau_accept->eps_network_feature_support.ims_vops = 1;

    ogs_assert(nas_security_encode(emmbuf, mme_ue, &message) == OGS_OK && 
            *emmbuf);

    return OGS_OK;
}

int emm_build_tau_reject(ogs_pkbuf_t **emmbuf, nas_emm_cause_t emm_cause,
        mme_ue_t *mme_ue)
{
    nas_message_t message;
    nas_tracking_area_update_reject_t *tau_reject = 
        &message.emm.tracking_area_update_reject;

    ogs_assert(mme_ue);

    ogs_debug("[EMM] Tracking area update reject");
    ogs_debug("    IMSI[%s] Cause[%d]",
            MME_UE_HAVE_IMSI(mme_ue) ? mme_ue->imsi_bcd : "Unknown", emm_cause);

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_TRACKING_AREA_UPDATE_REJECT;

    tau_reject->emm_cause = emm_cause;

    ogs_assert(nas_plain_encode(emmbuf, &message) == OGS_OK && *emmbuf);

    return OGS_OK;
}

int emm_build_service_reject(ogs_pkbuf_t **emmbuf, nas_emm_cause_t emm_cause, 
        mme_ue_t *mme_ue)
{
    nas_message_t message;
    nas_service_reject_t *service_reject = &message.emm.service_reject;

    ogs_assert(mme_ue);

    ogs_debug("[EMM] Service reject");
    ogs_debug("    Cause[%d]", emm_cause);

    memset(&message, 0, sizeof(message));
    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_SERVICE_REJECT;

    service_reject->emm_cause = emm_cause;

    ogs_assert(nas_plain_encode(emmbuf, &message) == OGS_OK && *emmbuf);

    return OGS_OK;
}
