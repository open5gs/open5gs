#define TRACE_MODULE _emm_build

#include "core_debug.h"

#include "nas_message.h"

#include "nas_security.h"
#include "emm_build.h"

status_t emm_build_attach_accept(
        pkbuf_t **emmbuf, mme_ue_t *mme_ue, pkbuf_t *esmbuf)
{
    nas_message_t message;
    nas_attach_accept_t *attach_accept = &message.emm.attach_accept;
    nas_eps_attach_result_t *eps_attach_result = 
        &attach_accept->eps_attach_result;
    nas_gprs_timer_t *t3412_value = &attach_accept->t3412_value;
    nas_tracking_area_identity_list_t *tai_list = &attach_accept->tai_list;
    nas_eps_mobile_identity_t *guti = &attach_accept->guti;
    nas_gprs_timer_t *t3402_value = &attach_accept->t3402_value;
    nas_gprs_timer_t *t3423_value = &attach_accept->t3423_value;
    nas_eps_network_feature_support_t *eps_network_feature_support =
        &attach_accept->eps_network_feature_support;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(mme_ue->enb_ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(message));
    message.h.security_header_type = 
       NAS_SECURITY_HEADER_INTEGRITY_PROTECTED_AND_CIPHERED;
    message.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;

    message.emm.h.protocol_discriminator = NAS_PROTOCOL_DISCRIMINATOR_EMM;
    message.emm.h.message_type = NAS_ATTACH_ACCEPT;

    eps_attach_result->result = NAS_ATTACH_RESULT_COMBINED_EPS_IMSI_ATTACH;
    t3412_value->unit = NAS_GRPS_TIMER_UNIT_MULTIPLES_OF_DECI_HH;
    t3412_value->value = 9;

    tai_list->length = 6;
    tai_list->type = 2;
    tai_list->num = 0; /* +1 = 1 elements */
    memcpy(&tai_list->type2.tai[0], &mme_ue->enb_ue->tai, sizeof(tai_t));

    attach_accept->esm_message_container.data = esmbuf->payload;
    attach_accept->esm_message_container.len = esmbuf->len;

    d_assert(mme_ue_new_guti(mme_ue) == CORE_OK,
            return CORE_ERROR, "GUTI error");
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

    d_assert(nas_security_encode(emmbuf, mme_ue, &message) == CORE_OK && 
            *emmbuf,,);
    pkbuf_free(esmbuf);

    return CORE_OK;
}

