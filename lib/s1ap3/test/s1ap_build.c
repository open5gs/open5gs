#define TRACE_MODULE _s1ap_build

#include "core_debug.h"

#if 0
#include "fd/s6a/s6a_message.h"

#include "mme_context.h"

#include "mme_kdf.h"
#endif
#include "s1ap_build.h"
#include "s1ap_conv.h"

#if 0
status_t s1ap_build_setup_rsp(pkbuf_t **pkbuf)
{
    int erval;
    int i, j;

    s1ap_message_t message;
    S1ap_S1SetupResponseIEs_t *ies = NULL;
    S1ap_ServedGUMMEIsItem_t *servedGUMMEI;
    S1ap_PLMNidentity_t *plmnIdentity;
    S1ap_MME_Group_ID_t *mmeGroupId;
    S1ap_MME_Code_t *mmeCode;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies = &message.s1ap_S1SetupResponseIEs;

    for (i = 0; i < mme_self()->max_num_of_served_gummei; i++)
    {
        servedGUMMEI = (S1ap_ServedGUMMEIsItem_t *)
            core_calloc(1, sizeof(S1ap_ServedGUMMEIsItem_t));
        served_gummei_t *served_gummei = &mme_self()->served_gummei[i];

        for (j = 0; j < served_gummei->num_of_plmn_id; j++)
        {
            plmnIdentity = (S1ap_PLMNidentity_t *)
                core_calloc(1, sizeof(S1ap_PLMNidentity_t));
            s1ap_buffer_to_OCTET_STRING(
                    &served_gummei->plmn_id[j], PLMN_ID_LEN, plmnIdentity);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedPLMNs, plmnIdentity);
            d_trace(5, "    PLMN_ID[MCC:%d MNC:%d]\n",
                plmn_id_mcc(&served_gummei->plmn_id[j]),
                plmn_id_mnc(&served_gummei->plmn_id[j]));
        }

        for (j = 0; j < served_gummei->num_of_mme_gid; j++)
        {
            mmeGroupId = (S1ap_MME_Group_ID_t *)
                core_calloc(1, sizeof(S1ap_MME_Group_ID_t));
            s1ap_uint16_to_OCTET_STRING(
                served_gummei->mme_gid[j], mmeGroupId);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedGroupIDs, mmeGroupId);
            d_trace(5, "    MME Group[%d]\n", served_gummei->mme_gid[j]);
        }

        for (j = 0; j < served_gummei->num_of_mme_code; j++)
        {
            mmeCode = (S1ap_MME_Code_t *)
                core_calloc(1, sizeof(S1ap_MME_Code_t));
            s1ap_uint8_to_OCTET_STRING(
                served_gummei->mme_code[j], mmeCode);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedMMECs, mmeCode);
            d_trace(5, "    MME Code[%d]\n", served_gummei->mme_code[j]);
        }
        ASN_SEQUENCE_ADD(&ies->servedGUMMEIs, servedGUMMEI);
    }

    ies->relativeMMECapacity = mme_self()->relative_capacity;

    message.procedureCode = S1ap_ProcedureCode_id_S1Setup;
    message.direction = S1AP_PDU_PR_successfulOutcome;

    erval = s1ap_encode_pdu(pkbuf, &message);
    s1ap_free_pdu(&message);

    if (erval < 0)
    {
        d_error("s1ap_encode_error : (%d)", erval);
        return CORE_ERROR;
    }

    return CORE_OK;
}
#endif

status_t s1ap_build_setup_failure(
        pkbuf_t **pkbuf, S1AP_Cause_PR group, long cause, long time_to_wait)
{
    status_t rv;

    S1AP_S1AP_PDU_t pdu;
    S1AP_UnsuccessfulOutcome_t *unsuccessfulOutcome = NULL;
    S1AP_S1SetupFailure_t *S1SetupFailure = NULL;

    S1AP_S1SetupFailureIEs_t *ie = NULL;
    S1AP_Cause_t *Cause = NULL;
    S1AP_TimeToWait_t *TimeToWait = NULL;
    
    d_trace(5, "    Group[%d] Cause[%d] TimeToWait[%ld]\n",
            group, cause, time_to_wait);

    memset(&pdu, 0, sizeof (S1AP_S1AP_PDU_t));
    pdu.present = S1AP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.unsuccessfulOutcome = 
        core_calloc(1, sizeof(S1AP_UnsuccessfulOutcome_t));

    unsuccessfulOutcome = pdu.choice.unsuccessfulOutcome;
    unsuccessfulOutcome->procedureCode = S1AP_ProcedureCode_S1AP_id_S1Setup;
    unsuccessfulOutcome->criticality = S1AP_Criticality_reject;
    unsuccessfulOutcome->value.present =
        UnsuccessfulOutcome__value_PR_S1SetupFailure;

    S1SetupFailure = &unsuccessfulOutcome->value.choice.S1SetupFailure;

    ie = core_calloc(1, sizeof(S1AP_S1SetupFailureIEs_t));
    ASN_SEQUENCE_ADD(&S1SetupFailure->protocolIEs, ie);

    ie->id = S1AP_ProtocolIE_ID_S1AP_id_Cause;
    ie->criticality = S1AP_Criticality_ignore;
    ie->value.present = S1SetupFailureIEs__value_PR_Cause;

    Cause = &ie->value.choice.Cause;

    if (time_to_wait > -1)
    {
        ie = core_calloc(1, sizeof(S1AP_S1SetupFailureIEs_t));
        ASN_SEQUENCE_ADD(&S1SetupFailure->protocolIEs, ie);

        ie->id = S1AP_ProtocolIE_ID_S1AP_id_TimeToWait;
        ie->criticality = S1AP_Criticality_ignore;
        ie->value.present = S1SetupFailureIEs__value_PR_TimeToWait;

        TimeToWait = &ie->value.choice.TimeToWait;
    }

    Cause->present = group;
    Cause->choice.radioNetwork = cause;

    if (TimeToWait)
        *TimeToWait = time_to_wait;

    rv = s1ap_encode_pdu(pkbuf, &pdu);
    s1ap_free_pdu(&pdu);

    if (rv != CORE_OK)
    {
        d_error("s1ap_encode_pdu() failed");
        return CORE_ERROR;
    }

    return CORE_OK;
}

#if 0
status_t s1ap_build_downlink_nas_transport(
            pkbuf_t **s1apbuf, enb_ue_t *enb_ue, pkbuf_t *emmbuf)
{
    int encoded;
    s1ap_message_t message;
    S1ap_DownlinkNASTransport_IEs_t *ies = 
        &message.s1ap_DownlinkNASTransport_IEs;
    S1ap_NAS_PDU_t *nasPdu = &ies->nas_pdu;

    d_assert(emmbuf, return CORE_ERROR, "Null param");
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(s1ap_message_t));
    
    d_trace(3, "[MME] Downlink NAS transport\n");
    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    ies->mme_ue_s1ap_id = enb_ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    nasPdu->size = emmbuf->len;
    nasPdu->buf = core_calloc(nasPdu->size, sizeof(c_uint8_t));
    memcpy(nasPdu->buf, emmbuf->payload, nasPdu->size);

    message.procedureCode = S1ap_ProcedureCode_id_downlinkNASTransport;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);
    pkbuf_free(emmbuf);

    return CORE_OK;
}

status_t s1ap_build_initial_context_setup_request(
            pkbuf_t **s1apbuf, mme_ue_t *mme_ue, pkbuf_t *emmbuf)
{
    status_t rv;

    int encoded;
    s1ap_message_t message;
    S1ap_InitialContextSetupRequestIEs_t *ies =
            &message.s1ap_InitialContextSetupRequestIEs;
    S1ap_E_RABToBeSetupItemCtxtSUReq_t *e_rab = NULL;
	struct S1ap_GBR_QosInformation *gbrQosInformation = NULL; /* OPTIONAL */
    S1ap_NAS_PDU_t *nasPdu = NULL;
    enb_ue_t *enb_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    s6a_subscription_data_t *subscription_data = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");
    subscription_data = &mme_ue->subscription_data;
    d_assert(subscription_data, return CORE_ERROR, "Null param");

    d_trace(3, "[MME] Initial context setup request\n");
    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = enb_ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    asn_uint642INTEGER(
            &ies->uEaggregateMaximumBitrate.uEaggregateMaximumBitRateUL, 
            subscription_data->ambr.uplink);
    asn_uint642INTEGER(
            &ies->uEaggregateMaximumBitrate.uEaggregateMaximumBitRateDL, 
            subscription_data->ambr.downlink);

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        bearer = mme_bearer_first(sess);
        while(bearer)
        {
            e_rab = (S1ap_E_RABToBeSetupItemCtxtSUReq_t *)
                core_calloc(1, sizeof(S1ap_E_RABToBeSetupItemCtxtSUReq_t));
            e_rab->e_RAB_ID = bearer->ebi;
            e_rab->e_RABlevelQoSParameters.qCI = bearer->qos.qci;

            d_trace(5, "    EBI[%d] QCI[%d] SGW-S1U-TEID[%d]\n",
                    bearer->ebi, bearer->qos.qci, bearer->sgw_s1u_teid);

            e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
                priorityLevel = bearer->qos.arp.priority_level;
            e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
                pre_emptionCapability =
                    !(bearer->qos.arp.pre_emption_capability);
            e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
                pre_emptionVulnerability =
                    !(bearer->qos.arp.pre_emption_vulnerability);

            if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
                bearer->qos.gbr.downlink || bearer->qos.gbr.uplink)
            {
                if (bearer->qos.mbr.downlink == 0)
                    bearer->qos.mbr.downlink = MAX_BIT_RATE;
                if (bearer->qos.mbr.uplink == 0)
                    bearer->qos.mbr.uplink = MAX_BIT_RATE;
                if (bearer->qos.gbr.downlink == 0)
                    bearer->qos.gbr.downlink = MAX_BIT_RATE;
                if (bearer->qos.gbr.uplink == 0)
                    bearer->qos.gbr.uplink = MAX_BIT_RATE;

                gbrQosInformation = 
                        core_calloc(1, sizeof(struct S1ap_GBR_QosInformation));
                asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateDL,
                        bearer->qos.mbr.downlink);
                asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateUL,
                        bearer->qos.mbr.uplink);
                asn_uint642INTEGER(&gbrQosInformation->
                        e_RAB_GuaranteedBitrateDL, bearer->qos.gbr.downlink);
                asn_uint642INTEGER(&gbrQosInformation->
                        e_RAB_GuaranteedBitrateUL, bearer->qos.gbr.uplink);
                e_rab->e_RABlevelQoSParameters.gbrQosInformation =
                        gbrQosInformation;
            }

            rv = s1ap_ip_to_BIT_STRING(
                    &bearer->sgw_s1u_ip, &e_rab->transportLayerAddress);
            d_assert(rv == CORE_OK, return CORE_ERROR,);
            s1ap_uint32_to_OCTET_STRING(bearer->sgw_s1u_teid, &e_rab->gTP_TEID);

            if (emmbuf && emmbuf->len)
            {
                nasPdu = (S1ap_NAS_PDU_t *)core_calloc(
                        1, sizeof(S1ap_NAS_PDU_t));
                nasPdu->size = emmbuf->len;
                nasPdu->buf = core_calloc(nasPdu->size, sizeof(c_uint8_t));
                memcpy(nasPdu->buf, emmbuf->payload, nasPdu->size);
                e_rab->nAS_PDU = nasPdu;
            }

            ASN_SEQUENCE_ADD(&ies->e_RABToBeSetupListCtxtSUReq, e_rab);

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }

    ies->ueSecurityCapabilities.encryptionAlgorithms.size = 2;
    ies->ueSecurityCapabilities.encryptionAlgorithms.buf = 
        core_calloc(ies->ueSecurityCapabilities.encryptionAlgorithms.size, 
                    sizeof(c_uint8_t));
    ies->ueSecurityCapabilities.encryptionAlgorithms.bits_unused = 0;
    ies->ueSecurityCapabilities.encryptionAlgorithms.buf[0] = 
        (mme_ue->ue_network_capability.eea << 1);

    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.size = 2;
    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.buf =
        core_calloc(ies->ueSecurityCapabilities.
                        integrityProtectionAlgorithms.size, sizeof(c_uint8_t));
    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.bits_unused = 0;
    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.buf[0] =
        (mme_ue->ue_network_capability.eia << 1);

    ies->securityKey.size = SHA256_DIGEST_SIZE;
    ies->securityKey.buf = 
        core_calloc(ies->securityKey.size, sizeof(c_uint8_t));
    ies->securityKey.bits_unused = 0;
    memcpy(ies->securityKey.buf, mme_ue->kenb, ies->securityKey.size);

    /* Set UeRadioCapability if exists */
    if (mme_ue->radio_capa)
    {
        S1ap_UERadioCapability_t *radio_capa = 
            (S1ap_UERadioCapability_t *)mme_ue->radio_capa;

        ies->presenceMask |= 
               S1AP_INITIALCONTEXTSETUPREQUESTIES_UERADIOCAPABILITY_PRESENT;

        ies->ueRadioCapability.size = radio_capa->size;
        ies->ueRadioCapability.buf = 
            core_calloc(ies->ueRadioCapability.size, sizeof(c_uint8_t));
        memcpy(ies->ueRadioCapability.buf, radio_capa->buf, radio_capa->size);
    }

    message.procedureCode = S1ap_ProcedureCode_id_InitialContextSetup;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    if (emmbuf && emmbuf->len)
    {
        pkbuf_free(emmbuf);
    }

    return CORE_OK;
}

status_t s1ap_build_e_rab_setup_request(
            pkbuf_t **s1apbuf, mme_bearer_t *bearer, pkbuf_t *esmbuf)
{
    status_t rv;
    int encoded;
    s1ap_message_t message;
    S1ap_E_RABSetupRequestIEs_t *ies = &message.s1ap_E_RABSetupRequestIEs;
    S1ap_E_RABToBeSetupItemBearerSUReq_t *e_rab = NULL;
	struct S1ap_GBR_QosInformation *gbrQosInformation = NULL; /* OPTIONAL */
    S1ap_NAS_PDU_t *nasPdu = NULL;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    d_assert(esmbuf, return CORE_ERROR, "Null param");
    d_assert(bearer, return CORE_ERROR, "Null param");

    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    d_trace(3, "[MME] E-RAB setup request\n");
    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = enb_ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    e_rab = (S1ap_E_RABToBeSetupItemBearerSUReq_t *)
        core_calloc(1, sizeof(S1ap_E_RABToBeSetupItemBearerSUReq_t));
    e_rab->e_RAB_ID = bearer->ebi;
    e_rab->e_RABlevelQoSParameters.qCI = bearer->qos.qci;

    d_trace(5, "    EBI[%d] QCI[%d]\n", bearer->ebi, bearer->qos.qci);

    e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
        priorityLevel = bearer->qos.arp.priority_level;
    e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
        pre_emptionCapability = !(bearer->qos.arp.pre_emption_capability);
    e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
        pre_emptionVulnerability = !(bearer->qos.arp.pre_emption_vulnerability);

    if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
        bearer->qos.gbr.downlink || bearer->qos.gbr.uplink)
    {
        if (bearer->qos.mbr.downlink == 0)
            bearer->qos.mbr.downlink = MAX_BIT_RATE;
        if (bearer->qos.mbr.uplink == 0)
            bearer->qos.mbr.uplink = MAX_BIT_RATE;
        if (bearer->qos.gbr.downlink == 0)
            bearer->qos.gbr.downlink = MAX_BIT_RATE;
        if (bearer->qos.gbr.uplink == 0)
            bearer->qos.gbr.uplink = MAX_BIT_RATE;

        gbrQosInformation = 
                core_calloc(1, sizeof(struct S1ap_GBR_QosInformation));
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateDL,
                bearer->qos.mbr.downlink);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateUL,
                bearer->qos.mbr.uplink);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_GuaranteedBitrateDL,
                bearer->qos.gbr.downlink);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_GuaranteedBitrateUL,
                bearer->qos.gbr.uplink);
        e_rab->e_RABlevelQoSParameters.gbrQosInformation = gbrQosInformation;
    }

    rv = s1ap_ip_to_BIT_STRING(
            &bearer->sgw_s1u_ip, &e_rab->transportLayerAddress);
    d_assert(rv == CORE_OK, return CORE_ERROR,);
    s1ap_uint32_to_OCTET_STRING(bearer->sgw_s1u_teid, &e_rab->gTP_TEID);
    d_trace(5, "    SGW-S1U-TEID[%d]\n", bearer->sgw_s1u_teid);

    nasPdu = &e_rab->nAS_PDU;
    nasPdu->size = esmbuf->len;
    nasPdu->buf = core_calloc(nasPdu->size, sizeof(c_uint8_t));
    memcpy(nasPdu->buf, esmbuf->payload, nasPdu->size);

    ASN_SEQUENCE_ADD(&ies->e_RABToBeSetupListBearerSUReq, e_rab);

    message.procedureCode = S1ap_ProcedureCode_id_E_RABSetup;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    pkbuf_free(esmbuf);

    return CORE_OK;
}

status_t s1ap_build_e_rab_modify_request(
            pkbuf_t **s1apbuf, mme_bearer_t *bearer, pkbuf_t *esmbuf)
{
    int encoded;
    s1ap_message_t message;
    S1ap_E_RABModifyRequestIEs_t *ies = &message.s1ap_E_RABModifyRequestIEs;
    S1ap_E_RABToBeModifiedItemBearerModReq_t *e_rab = NULL;
	struct S1ap_GBR_QosInformation *gbrQosInformation = NULL; /* OPTIONAL */
    S1ap_NAS_PDU_t *nasPdu = NULL;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;

    d_assert(esmbuf, return CORE_ERROR, "Null param");
    d_assert(bearer, return CORE_ERROR, "Null param");

    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    d_trace(3, "[MME] E-RAB modify request\n");
    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = enb_ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    e_rab = (S1ap_E_RABToBeModifiedItemBearerModReq_t *)
        core_calloc(1, sizeof(S1ap_E_RABToBeModifiedItemBearerModReq_t));
    e_rab->e_RAB_ID = bearer->ebi;
    e_rab->e_RABLevelQoSParameters.qCI = bearer->qos.qci;

    d_trace(5, "    EBI[%d] QCI[%d]\n", bearer->ebi, bearer->qos.qci);

    e_rab->e_RABLevelQoSParameters.allocationRetentionPriority.
        priorityLevel = bearer->qos.arp.priority_level;
    e_rab->e_RABLevelQoSParameters.allocationRetentionPriority.
        pre_emptionCapability = !(bearer->qos.arp.pre_emption_capability);
    e_rab->e_RABLevelQoSParameters.allocationRetentionPriority.
        pre_emptionVulnerability = !(bearer->qos.arp.pre_emption_vulnerability);

    if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
        bearer->qos.gbr.downlink || bearer->qos.gbr.uplink)
    {
        if (bearer->qos.mbr.downlink == 0)
            bearer->qos.mbr.downlink = MAX_BIT_RATE;
        if (bearer->qos.mbr.uplink == 0)
            bearer->qos.mbr.uplink = MAX_BIT_RATE;
        if (bearer->qos.gbr.downlink == 0)
            bearer->qos.gbr.downlink = MAX_BIT_RATE;
        if (bearer->qos.gbr.uplink == 0)
            bearer->qos.gbr.uplink = MAX_BIT_RATE;

        gbrQosInformation = 
                core_calloc(1, sizeof(struct S1ap_GBR_QosInformation));
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateDL,
                bearer->qos.mbr.downlink);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateUL,
                bearer->qos.mbr.uplink);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_GuaranteedBitrateDL,
                bearer->qos.gbr.downlink);
        asn_uint642INTEGER(&gbrQosInformation->e_RAB_GuaranteedBitrateUL,
                bearer->qos.gbr.uplink);
        e_rab->e_RABLevelQoSParameters.gbrQosInformation = gbrQosInformation;
    }

    nasPdu = &e_rab->nAS_PDU;
    nasPdu->size = esmbuf->len;
    nasPdu->buf = core_calloc(nasPdu->size, sizeof(c_uint8_t));
    memcpy(nasPdu->buf, esmbuf->payload, nasPdu->size);

    ASN_SEQUENCE_ADD(&ies->e_RABToBeModifiedListBearerModReq, e_rab);

    message.procedureCode = S1ap_ProcedureCode_id_E_RABModify;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    pkbuf_free(esmbuf);

    return CORE_OK;
}

status_t s1ap_build_e_rab_release_command(pkbuf_t **s1apbuf,
        mme_bearer_t *bearer, pkbuf_t *esmbuf, 
        S1ap_Cause_PR group, long cause)
{
    int encoded;
    s1ap_message_t message;
    S1ap_E_RABReleaseCommandIEs_t *ies = &message.s1ap_E_RABReleaseCommandIEs;
    S1ap_E_RABItem_t *e_rab = NULL;
    S1ap_NAS_PDU_t *nasPdu = NULL;
    mme_ue_t *mme_ue = NULL;
    enb_ue_t *enb_ue = NULL;
    s6a_subscription_data_t *subscription_data = NULL;

    d_assert(esmbuf, return CORE_ERROR, "Null param");
    d_assert(bearer, return CORE_ERROR, "Null param");

    mme_ue = bearer->mme_ue;
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");
    subscription_data = &mme_ue->subscription_data;
    d_assert(subscription_data, return CORE_ERROR, "Null param");

    d_trace(3, "[MME] E-RAB release command\n");
    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = enb_ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    ies->presenceMask |= 
           S1AP_E_RABRELEASECOMMANDIES_UEAGGREGATEMAXIMUMBITRATE_PRESENT;
    asn_uint642INTEGER(
            &ies->uEaggregateMaximumBitrate.uEaggregateMaximumBitRateUL, 
            subscription_data->ambr.uplink);
    asn_uint642INTEGER(
            &ies->uEaggregateMaximumBitrate.uEaggregateMaximumBitRateDL, 
            subscription_data->ambr.downlink);

    e_rab = (S1ap_E_RABItem_t *)core_calloc(1, sizeof(S1ap_E_RABItem_t));
    e_rab->e_RAB_ID = bearer->ebi;
    e_rab->cause.present = group;
    e_rab->cause.choice.radioNetwork = cause;

    d_trace(5, "    EBI[%d] Gruop[%d] Cause[%d]\n", bearer->ebi, group, cause);

    ies->presenceMask |= S1AP_E_RABRELEASECOMMANDIES_NAS_PDU_PRESENT;
    nasPdu = &ies->nas_pdu;
    nasPdu->size = esmbuf->len;
    nasPdu->buf = core_calloc(nasPdu->size, sizeof(c_uint8_t));
    memcpy(nasPdu->buf, esmbuf->payload, nasPdu->size);

    ASN_SEQUENCE_ADD(&ies->e_RABToBeReleasedList, e_rab);

    message.procedureCode = S1ap_ProcedureCode_id_E_RABRelease;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    pkbuf_free(esmbuf);

    return CORE_OK;
}

status_t s1ap_build_ue_context_release_command(
    pkbuf_t **s1apbuf, enb_ue_t *enb_ue, S1ap_Cause_PR group, long cause)
{
    int encoded;
    s1ap_message_t message;
    S1ap_UEContextReleaseCommand_IEs_t *ies =
            &message.s1ap_UEContextReleaseCommand_IEs;

    d_assert(enb_ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(s1ap_message_t));

    if (enb_ue->mme_ue_s1ap_id == 0)
    {
        d_error("invalid mme ue s1ap id (idx: %d)", enb_ue->index);
        return CORE_ERROR;
    }

#if 0 /* ENB_UE_S1AP_ID could be allocated with 0 from eNodeB */
    if (enb_ue->enb_ue_s1ap_id)
#endif
    {
        ies->uE_S1AP_IDs.present = S1ap_UE_S1AP_IDs_PR_uE_S1AP_ID_pair;
        ies->uE_S1AP_IDs.choice.uE_S1AP_ID_pair.mME_UE_S1AP_ID = 
            enb_ue->mme_ue_s1ap_id;
        ies->uE_S1AP_IDs.choice.uE_S1AP_ID_pair.eNB_UE_S1AP_ID = 
            enb_ue->enb_ue_s1ap_id;
        ies->uE_S1AP_IDs.choice.uE_S1AP_ID_pair.iE_Extensions = NULL;
    }
#if 0
    else
    {
        ies->uE_S1AP_IDs.present = S1ap_UE_S1AP_IDs_PR_mME_UE_S1AP_ID;
        ies->uE_S1AP_IDs.choice.mME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
    }
#endif

    ies->cause.present = group;
    ies->cause.choice.radioNetwork = cause;

    message.procedureCode = S1ap_ProcedureCode_id_UEContextRelease;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0, return CORE_ERROR,);

    return CORE_OK;
}

status_t s1ap_build_paging(pkbuf_t **s1apbuf, mme_ue_t *mme_ue)
{
    int encoded;
    s1ap_message_t message;
    S1ap_PagingIEs_t *ies = &message.s1ap_PagingIEs;
    S1ap_TAIItem_t *tai_item = NULL;
    c_uint16_t index_value;
    c_uint64_t ue_imsi_value = 0;
    int i = 0;

    d_assert(mme_ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(s1ap_message_t));

    d_trace(3, "[MME] Paging\n");

    /* Set UE Identity Index value : IMSI mod 4096 */
    ies->ueIdentityIndexValue.size = 2;
    ies->ueIdentityIndexValue.buf = 
        core_calloc(ies->ueIdentityIndexValue.size, sizeof(c_uint8_t));

    /* Conver string to value */
    for (i = 0; i < strlen(mme_ue->imsi_bcd); i++)
    {
        ue_imsi_value = ue_imsi_value*10 + (mme_ue->imsi_bcd[i] - '0');
    }

    /* index(10bit) = ue_imsi_value mod 1024 */
    index_value = ue_imsi_value % 1024;
    ies->ueIdentityIndexValue.buf[0] = index_value >> 2;
    ies->ueIdentityIndexValue.buf[1] = (index_value & 0x3f) << 6;
    ies->ueIdentityIndexValue.bits_unused = 6;

    /* Set Paging Identity */
    ies->uePagingID.present = S1ap_UEPagingID_PR_s_TMSI;
    s1ap_uint8_to_OCTET_STRING(mme_ue->guti.mme_code, 
            &ies->uePagingID.choice.s_TMSI.mMEC);

    s1ap_uint32_to_OCTET_STRING(mme_ue->guti.m_tmsi, 
            &ies->uePagingID.choice.s_TMSI.m_TMSI);

    d_trace(5, "    MME_CODE[%d] M_TMSI[0x%x]\n",
            mme_ue->guti.mme_code, mme_ue->guti.m_tmsi);

    /* FIXME : fixed to ps */
    ies->cnDomain = S1ap_CNDomain_ps;


    tai_item  = core_calloc(1, sizeof(S1ap_TAIItem_t));
    s1ap_buffer_to_OCTET_STRING(&mme_ue->tai.plmn_id, sizeof(plmn_id_t),
            &tai_item->tAI.pLMNidentity);
    s1ap_uint16_to_OCTET_STRING(mme_ue->tai.tac, &tai_item->tAI.tAC);

    ASN_SEQUENCE_ADD(&ies->taiList, tai_item);

    message.procedureCode = S1ap_ProcedureCode_id_Paging;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0, return CORE_ERROR,);

    return CORE_OK;
}

status_t s1ap_build_path_switch_ack(pkbuf_t **s1apbuf, mme_ue_t *mme_ue)
{
    int encoded;
    s1ap_message_t message;
    S1ap_PathSwitchRequestAcknowledgeIEs_t *ies = 
        &message.s1ap_PathSwitchRequestAcknowledgeIEs;
    enb_ue_t *enb_ue = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    d_trace(3, "[MME] Path switch acknowledge\n");
    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue->enb_ue_s1ap_id, enb_ue->mme_ue_s1ap_id);

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = enb_ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    ies->securityContext.nextHopChainingCount = mme_ue->nhcc;
    ies->securityContext.nextHopParameter.size = SHA256_DIGEST_SIZE;
    ies->securityContext.nextHopParameter.buf = 
        core_calloc(ies->securityContext.nextHopParameter.size,
        sizeof(c_uint8_t));
    ies->securityContext.nextHopParameter.bits_unused = 0;
    memcpy(ies->securityContext.nextHopParameter.buf,
            mme_ue->nh, ies->securityContext.nextHopParameter.size);

    message.procedureCode = S1ap_ProcedureCode_id_PathSwitchRequest;
    message.direction = S1AP_PDU_PR_successfulOutcome;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    return CORE_OK;
}

status_t s1ap_build_path_switch_failure(pkbuf_t **s1apbuf,
    c_uint32_t enb_ue_s1ap_id, c_uint32_t mme_ue_s1ap_id,
    S1ap_Cause_PR group, long cause)
{
    int encoded;
    s1ap_message_t message;
    S1ap_PathSwitchRequestFailureIEs_t *ies = 
        &message.s1ap_PathSwitchRequestFailureIEs;

    memset(&message, 0, sizeof(s1ap_message_t));

    d_trace(3, "[MME] Path switch failure\n");
    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue_s1ap_id, mme_ue_s1ap_id);
    d_trace(5, "    Group[%d] Cause[%d]\n", group, cause);

    ies->mme_ue_s1ap_id = mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue_s1ap_id;
    ies->cause.present = group;
    ies->cause.choice.radioNetwork = cause;

    message.procedureCode = S1ap_ProcedureCode_id_PathSwitchRequest;
    message.direction = S1AP_PDU_PR_unsuccessfulOutcome;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    return CORE_OK;
}

status_t s1ap_build_handover_command(pkbuf_t **s1apbuf, enb_ue_t *source_ue)
{
    status_t rv;

    int encoded;
    s1ap_message_t message;
    S1ap_HandoverCommandIEs_t *ies = &message.s1ap_HandoverCommandIEs;
    S1ap_E_RABDataForwardingItem_t *e_rab = NULL;

    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    d_assert(source_ue, return CORE_ERROR, "Null param");
    mme_ue = source_ue->mme_ue;

    d_trace(3, "[MME] Handover command\n");

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = source_ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = source_ue->enb_ue_s1ap_id;
    ies->handoverType = source_ue->handover_type;

    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        bearer = mme_bearer_first(sess);
        while(bearer)
        {
            if (MME_HAVE_SGW_DL_INDIRECT_TUNNEL(bearer) ||
                MME_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer))
            {
                e_rab = (S1ap_E_RABDataForwardingItem_t *)
                    core_calloc(1, sizeof(S1ap_E_RABDataForwardingItem_t));
                e_rab->e_RAB_ID = bearer->ebi;
            }

            if (MME_HAVE_SGW_DL_INDIRECT_TUNNEL(bearer))
            {
                e_rab->dL_transportLayerAddress =
                    (S1ap_TransportLayerAddress_t *)
                    core_calloc(1, sizeof(S1ap_TransportLayerAddress_t));
                rv = s1ap_ip_to_BIT_STRING(
                        &bearer->sgw_dl_ip, e_rab->dL_transportLayerAddress);
                d_assert(rv == CORE_OK, return CORE_ERROR,);

                e_rab->dL_gTP_TEID = (S1ap_GTP_TEID_t *)
                    core_calloc(1, sizeof(S1ap_GTP_TEID_t));
                s1ap_uint32_to_OCTET_STRING(
                        bearer->sgw_dl_teid, e_rab->dL_gTP_TEID);
                d_trace(5, "    SGW-DL-TEID[%d]\n", bearer->sgw_dl_teid);
            }

            if (MME_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer))
            {
                e_rab->uL_S1ap_TransportLayerAddress =
                    (S1ap_TransportLayerAddress_t *)
                    core_calloc(1, sizeof(S1ap_TransportLayerAddress_t));
                rv = s1ap_ip_to_BIT_STRING(
                    &bearer->sgw_ul_ip, e_rab->uL_S1ap_TransportLayerAddress);
                d_assert(rv == CORE_OK, return CORE_ERROR,);

                e_rab->uL_S1ap_GTP_TEID = (S1ap_GTP_TEID_t *)
                    core_calloc(1, sizeof(S1ap_GTP_TEID_t));
                s1ap_uint32_to_OCTET_STRING(
                        bearer->sgw_ul_teid, e_rab->uL_S1ap_GTP_TEID);
                d_trace(5, "    SGW-UL-TEID[%d]\n", bearer->sgw_dl_teid);
            }

            if (MME_HAVE_SGW_DL_INDIRECT_TUNNEL(bearer) ||
                MME_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer))
            {
                ASN_SEQUENCE_ADD(&ies->e_RABSubjecttoDataForwardingList, e_rab);
            }

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }

    if (ies->e_RABSubjecttoDataForwardingList.
            s1ap_E_RABDataForwardingItem.count)
    {
        ies->presenceMask |=
            S1AP_HANDOVERCOMMANDIES_E_RABSUBJECTTODATAFORWARDINGLIST_PRESENT;
    }

    s1ap_buffer_to_OCTET_STRING(mme_ue->container.buf, mme_ue->container.size, 
            &ies->target_ToSource_TransparentContainer);

    message.procedureCode = S1ap_ProcedureCode_id_HandoverPreparation;
    message.direction = S1AP_PDU_PR_successfulOutcome;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    return CORE_OK;
}

status_t s1ap_build_handover_preparation_failure(
        pkbuf_t **s1apbuf, enb_ue_t *source_ue, S1ap_Cause_t *cause)
{
    int encoded;
    s1ap_message_t message;
    S1ap_HandoverPreparationFailureIEs_t *ies =
        &message.s1ap_HandoverPreparationFailureIEs;

    d_assert(s1apbuf, return CORE_ERROR,);
    d_assert(source_ue, return CORE_ERROR,);
    d_assert(cause, return CORE_ERROR,);

    memset(&message, 0, sizeof(s1ap_message_t));

    d_trace(3, "[MME] Handover preparation failure\n");

    ies->mme_ue_s1ap_id = source_ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = source_ue->enb_ue_s1ap_id;
    ies->cause.present = cause->present;
    ies->cause.choice.radioNetwork = cause->choice.radioNetwork;

    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);
    d_trace(5, "    Group[%d] Cause[%d]\n",
            cause->present, cause->choice.radioNetwork);

    message.procedureCode = S1ap_ProcedureCode_id_HandoverPreparation;
    message.direction = S1AP_PDU_PR_unsuccessfulOutcome;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    return CORE_OK;
}

status_t s1ap_build_handover_request(
        pkbuf_t **s1apbuf, mme_ue_t *mme_ue, enb_ue_t *target_ue,
        S1ap_HandoverRequiredIEs_t *required)
{
    status_t rv;

    int encoded;
    s1ap_message_t message;
    S1ap_HandoverRequestIEs_t *ies = &message.s1ap_HandoverRequestIEs;
    S1ap_E_RABToBeSetupItemHOReq_t *e_rab = NULL;
	struct S1ap_GBR_QosInformation *gbrQosInformation = NULL; /* OPTIONAL */

    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    s6a_subscription_data_t *subscription_data = NULL;

    d_assert(target_ue, return CORE_ERROR, "Null param");
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(required, return CORE_ERROR, "Null param");
    subscription_data = &mme_ue->subscription_data;
    d_assert(subscription_data, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = target_ue->mme_ue_s1ap_id;
    ies->handoverType = required->handoverType;
    ies->cause.present = required->cause.present;
    ies->cause.choice.radioNetwork = required->cause.choice.radioNetwork;

    asn_uint642INTEGER(
            &ies->uEaggregateMaximumBitrate.uEaggregateMaximumBitRateUL, 
            subscription_data->ambr.uplink);
    asn_uint642INTEGER(
            &ies->uEaggregateMaximumBitrate.uEaggregateMaximumBitRateDL, 
            subscription_data->ambr.downlink);

    sess = mme_sess_first(mme_ue);
    while(sess)
    {
        bearer = mme_bearer_first(sess);
        while(bearer)
        {
            e_rab = (S1ap_E_RABToBeSetupItemHOReq_t *)
                core_calloc(1, sizeof(S1ap_E_RABToBeSetupItemHOReq_t));
            e_rab->e_RAB_ID = bearer->ebi;
            e_rab->e_RABlevelQosParameters.qCI = bearer->qos.qci;

            e_rab->e_RABlevelQosParameters.allocationRetentionPriority.
                priorityLevel = bearer->qos.arp.priority_level;
            e_rab->e_RABlevelQosParameters.allocationRetentionPriority.
                pre_emptionCapability =
                    !(bearer->qos.arp.pre_emption_capability);
            e_rab->e_RABlevelQosParameters.allocationRetentionPriority.
                pre_emptionVulnerability =
                    !(bearer->qos.arp.pre_emption_vulnerability);

            if (bearer->qos.mbr.downlink || bearer->qos.mbr.uplink ||
                bearer->qos.gbr.downlink || bearer->qos.gbr.uplink)
            {
                if (bearer->qos.mbr.downlink == 0)
                    bearer->qos.mbr.downlink = MAX_BIT_RATE;
                if (bearer->qos.mbr.uplink == 0)
                    bearer->qos.mbr.uplink = MAX_BIT_RATE;
                if (bearer->qos.gbr.downlink == 0)
                    bearer->qos.gbr.downlink = MAX_BIT_RATE;
                if (bearer->qos.gbr.uplink == 0)
                    bearer->qos.gbr.uplink = MAX_BIT_RATE;

                gbrQosInformation = 
                        core_calloc(1, sizeof(struct S1ap_GBR_QosInformation));
                asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateDL,
                        bearer->qos.mbr.downlink);
                asn_uint642INTEGER(&gbrQosInformation->e_RAB_MaximumBitrateUL,
                        bearer->qos.mbr.uplink);
                asn_uint642INTEGER(&gbrQosInformation->
                        e_RAB_GuaranteedBitrateDL, bearer->qos.gbr.downlink);
                asn_uint642INTEGER(&gbrQosInformation->
                        e_RAB_GuaranteedBitrateUL, bearer->qos.gbr.uplink);
                e_rab->e_RABlevelQosParameters.gbrQosInformation =
                        gbrQosInformation;
            }

            rv = s1ap_ip_to_BIT_STRING(
                    &bearer->sgw_s1u_ip, &e_rab->transportLayerAddress);
            d_assert(rv == CORE_OK, return CORE_ERROR,);
            s1ap_uint32_to_OCTET_STRING(bearer->sgw_s1u_teid, &e_rab->gTP_TEID);
            d_trace(5, "    SGW-S1U-TEID[%d]\n", bearer->sgw_s1u_teid);

            ASN_SEQUENCE_ADD(&ies->e_RABToBeSetupListHOReq, e_rab);

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }

    s1ap_buffer_to_OCTET_STRING(
            required->source_ToTarget_TransparentContainer.buf, 
            required->source_ToTarget_TransparentContainer.size, 
            &ies->source_ToTarget_TransparentContainer);

    ies->ueSecurityCapabilities.encryptionAlgorithms.size = 2;
    ies->ueSecurityCapabilities.encryptionAlgorithms.buf = 
        core_calloc(ies->ueSecurityCapabilities.encryptionAlgorithms.size, 
                    sizeof(c_uint8_t));
    ies->ueSecurityCapabilities.encryptionAlgorithms.bits_unused = 0;
    ies->ueSecurityCapabilities.encryptionAlgorithms.buf[0] = 
        (mme_ue->ue_network_capability.eea << 1);

    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.size = 2;
    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.buf =
        core_calloc(ies->ueSecurityCapabilities.
                        integrityProtectionAlgorithms.size, sizeof(c_uint8_t));
    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.bits_unused = 0;
    ies->ueSecurityCapabilities.integrityProtectionAlgorithms.buf[0] =
        (mme_ue->ue_network_capability.eia << 1);

    ies->securityContext.nextHopChainingCount = mme_ue->nhcc;
    ies->securityContext.nextHopParameter.size = SHA256_DIGEST_SIZE;
    ies->securityContext.nextHopParameter.buf = 
        core_calloc(ies->securityContext.nextHopParameter.size,
        sizeof(c_uint8_t));
    ies->securityContext.nextHopParameter.bits_unused = 0;
    memcpy(ies->securityContext.nextHopParameter.buf,
            mme_ue->nh, ies->securityContext.nextHopParameter.size);

    message.procedureCode = S1ap_ProcedureCode_id_HandoverResourceAllocation;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    return CORE_OK;
}

status_t s1ap_build_handover_cancel_ack(pkbuf_t **s1apbuf, enb_ue_t *source_ue)
{
    int encoded;
    s1ap_message_t message;
    S1ap_HandoverCancelAcknowledgeIEs_t *ies =
        &message.s1ap_HandoverCancelAcknowledgeIEs;

    d_assert(source_ue, return CORE_ERROR, "Null param");

    d_trace(3, "[MME] Handover cancel acknowledge\n");

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = source_ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = source_ue->enb_ue_s1ap_id;

    d_trace(5, "    Source : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            source_ue->enb_ue_s1ap_id, source_ue->mme_ue_s1ap_id);

    message.procedureCode = S1ap_ProcedureCode_id_HandoverCancel;
    message.direction = S1AP_PDU_PR_successfulOutcome;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    return CORE_OK;
}

status_t s1ap_build_mme_status_transfer(pkbuf_t **s1apbuf,
        enb_ue_t *target_ue, S1ap_ENBStatusTransferIEs_t *enb_ies)
{
    int i;

    int encoded;
    s1ap_message_t message;
    S1ap_MMEStatusTransferIEs_t *mme_ies = &message.s1ap_MMEStatusTransferIEs;

    d_assert(target_ue, return CORE_ERROR, "Null param");
    d_assert(enb_ies, return CORE_ERROR, "Null param");
    
    d_trace(3, "[MME] MME status transfer\n");

    memset(&message, 0, sizeof(s1ap_message_t));

    mme_ies->mme_ue_s1ap_id = target_ue->mme_ue_s1ap_id;
    mme_ies->eNB_UE_S1AP_ID = target_ue->enb_ue_s1ap_id;

    d_trace(5, "    Target : ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            target_ue->enb_ue_s1ap_id, target_ue->mme_ue_s1ap_id);

    for (i = 0; i < enb_ies->eNB_StatusTransfer_TransparentContainer.
            bearers_SubjectToStatusTransferList.list.count; i++)
    {
        S1ap_IE_t *enb_ie = NULL, *mme_ie = NULL;

        enb_ie = (S1ap_IE_t *)enb_ies->
            eNB_StatusTransfer_TransparentContainer.
            bearers_SubjectToStatusTransferList.list.array[i];
        d_assert(enb_ie, return CORE_ERROR, "Null param");

        mme_ie = s1ap_copy_ie(enb_ie);
        d_assert(mme_ie, return CORE_ERROR, "Null param");

        ASN_SEQUENCE_ADD(&mme_ies->
                eNB_StatusTransfer_TransparentContainer.
                bearers_SubjectToStatusTransferList, mme_ie);
    }

    message.procedureCode = S1ap_ProcedureCode_id_MMEStatusTransfer;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    return CORE_OK;
}

status_t s1ap_build_error_indication(
        pkbuf_t **s1apbuf, c_uint16_t presenceMask,
        c_uint32_t enb_ue_s1ap_id, c_uint32_t mme_ue_s1ap_id,
        S1ap_Cause_PR group, long cause)
{
    int encoded;
    s1ap_message_t message;
    S1ap_ErrorIndicationIEs_t *ies = &message.s1ap_ErrorIndicationIEs;

    d_assert(presenceMask, return CORE_ERROR,
            "Invalid PresenceMask[0x%x]", presenceMask);
    
    d_trace(3, "[MME] Error Indication\n");

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->presenceMask = presenceMask;
    ies->mme_ue_s1ap_id = mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue_s1ap_id;
    ies->cause.present = group;
    ies->cause.choice.radioNetwork = cause;

    d_trace(5, "    PresenceMask[0x%x]\n", presenceMask);
    d_trace(5, "    ENB_UE_S1AP_ID[%d] MME_UE_S1AP_ID[%d]\n",
            enb_ue_s1ap_id, mme_ue_s1ap_id);
    d_trace(5, "    Group[%d] Cause[%d]\n", group, cause);

    message.procedureCode = S1ap_ProcedureCode_id_ErrorIndication;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    return CORE_OK;
}
#endif
