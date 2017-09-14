#define TRACE_MODULE _s1ap_build

#include "core_debug.h"

#include "s6a_message.h"

#include "mme_context.h"

#include "mme_kdf.h"
#include "s1ap_build.h"
#include "s1ap_conv.h"

static void s1ap_build_cause(S1ap_Cause_t *dst, S1ap_Cause_t *src);

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
        }

        for (j = 0; j < served_gummei->num_of_mme_gid; j++)
        {
            mmeGroupId = (S1ap_MME_Group_ID_t *)
                core_calloc(1, sizeof(S1ap_MME_Group_ID_t));
            s1ap_uint16_to_OCTET_STRING(
                served_gummei->mme_gid[j], mmeGroupId);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedGroupIDs, mmeGroupId);
        }

        for (j = 0; j < served_gummei->num_of_mme_code; j++)
        {
            mmeCode = (S1ap_MME_Code_t *)
                core_calloc(1, sizeof(S1ap_MME_Code_t));
            s1ap_uint8_to_OCTET_STRING(
                served_gummei->mme_code[j], mmeCode);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedMMECs, mmeCode);
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


status_t s1ap_build_setup_failure(pkbuf_t **pkbuf, S1ap_Cause_t cause)
{
    int erval;

    s1ap_message_t message;
    S1ap_S1SetupFailureIEs_t *ies = NULL;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies = &message.s1ap_S1SetupFailureIEs;
    ies->cause = cause;

    message.procedureCode = S1ap_ProcedureCode_id_S1Setup;
    message.direction = S1AP_PDU_PR_unsuccessfulOutcome;

    erval = s1ap_encode_pdu(pkbuf, &message);
    s1ap_free_pdu(&message);

    if (erval < 0)
    {
        d_error("s1ap_encode_error : (%d)", erval);
        return CORE_ERROR;
    }

    return CORE_OK;
}

status_t s1ap_build_downlink_nas_transport(
            pkbuf_t **s1apbuf, enb_ue_t *enb_ue, pkbuf_t *emmbuf)
{
    char buf[INET_ADDRSTRLEN];
    
    int encoded;
    s1ap_message_t message;
    S1ap_DownlinkNASTransport_IEs_t *ies = 
        &message.s1ap_DownlinkNASTransport_IEs;
    S1ap_NAS_PDU_t *nasPdu = &ies->nas_pdu;

    d_assert(emmbuf, return CORE_ERROR, "Null param");
    d_assert(enb_ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(s1ap_message_t));

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

    d_trace(3, "[S1AP] downlinkNASTransport : "
            "UE[eNB-UE-S1AP-ID(%d)] <-- eNB[%s:%d]\n",
            enb_ue->enb_ue_s1ap_id,
            INET_NTOP(&enb_ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb_ue->enb->enb_id);

    return CORE_OK;
}

status_t s1ap_build_initial_context_setup_request(
            pkbuf_t **s1apbuf, mme_ue_t *mme_ue, pkbuf_t *emmbuf)
{
    char buf[INET_ADDRSTRLEN];

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

            e_rab->transportLayerAddress.size = 4;
            e_rab->transportLayerAddress.buf = core_calloc(
                    e_rab->transportLayerAddress.size, sizeof(c_uint8_t));
            memcpy(e_rab->transportLayerAddress.buf, &bearer->sgw_s1u_addr,
                    e_rab->transportLayerAddress.size);

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

    d_trace(3, "[S1AP] Initial Context Setup Request : "
            "UE[eNB-UE-S1AP-ID(%d)] <-- eNB[%s:%d]\n",
            enb_ue->enb_ue_s1ap_id,
            INET_NTOP(&enb_ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb_ue->enb->enb_id);

    if (emmbuf && emmbuf->len)
    {
        pkbuf_free(emmbuf);
    }

    return CORE_OK;
}

status_t s1ap_build_e_rab_setup_request(
            pkbuf_t **s1apbuf, mme_bearer_t *bearer, pkbuf_t *esmbuf)
{
    char buf[INET_ADDRSTRLEN];

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

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = enb_ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;

    e_rab = (S1ap_E_RABToBeSetupItemBearerSUReq_t *)
        core_calloc(1, sizeof(S1ap_E_RABToBeSetupItemBearerSUReq_t));
    e_rab->e_RAB_ID = bearer->ebi;
    e_rab->e_RABlevelQoSParameters.qCI = bearer->qos.qci;

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

    e_rab->transportLayerAddress.size = 4;
    e_rab->transportLayerAddress.buf = 
        core_calloc(e_rab->transportLayerAddress.size, sizeof(c_uint8_t));
    memcpy(e_rab->transportLayerAddress.buf, &bearer->sgw_s1u_addr,
            e_rab->transportLayerAddress.size);

    s1ap_uint32_to_OCTET_STRING(bearer->sgw_s1u_teid, &e_rab->gTP_TEID);

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

    d_trace(3, "[S1AP] E-RAB Setup Request : "
            "UE[eNB-UE-S1AP-ID(%d)] <-- eNB[%s:%d]\n",
            enb_ue->enb_ue_s1ap_id,
            INET_NTOP(&enb_ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb_ue->enb->enb_id);

    pkbuf_free(esmbuf);

    return CORE_OK;
}

status_t s1ap_build_e_rab_release_command(pkbuf_t **s1apbuf,
        mme_bearer_t *bearer, pkbuf_t *esmbuf, S1ap_Cause_t *cause)
{
    char buf[INET_ADDRSTRLEN];

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
    s1ap_build_cause(&e_rab->cause, cause);

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

    d_trace(3, "[S1AP] E-RAB Release Command : "
            "UE[eNB-UE-S1AP-ID(%d)] <-- eNB[%s:%d]\n",
            enb_ue->enb_ue_s1ap_id,
            INET_NTOP(&enb_ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb_ue->enb->enb_id);

    pkbuf_free(esmbuf);

    return CORE_OK;
}

status_t s1ap_build_ue_context_release_commmand(
            pkbuf_t **s1apbuf, enb_ue_t *enb_ue, S1ap_Cause_t *cause)
{
    char buf[INET_ADDRSTRLEN];

    int encoded;
    s1ap_message_t message;
    S1ap_UEContextReleaseCommand_IEs_t *ies =
            &message.s1ap_UEContextReleaseCommand_IEs;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(cause, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(s1ap_message_t));

    if (enb_ue->mme_ue_s1ap_id == 0)
    {
        d_error("invalid mme ue s1ap id (idx: %d)", enb_ue->index);
        return CORE_ERROR;
    }

    if (enb_ue->enb_ue_s1ap_id)
    {
        ies->uE_S1AP_IDs.present = S1ap_UE_S1AP_IDs_PR_uE_S1AP_ID_pair;
        ies->uE_S1AP_IDs.choice.uE_S1AP_ID_pair.mME_UE_S1AP_ID = 
            enb_ue->mme_ue_s1ap_id;
        ies->uE_S1AP_IDs.choice.uE_S1AP_ID_pair.eNB_UE_S1AP_ID = 
            enb_ue->enb_ue_s1ap_id;
        ies->uE_S1AP_IDs.choice.uE_S1AP_ID_pair.iE_Extensions = NULL;
    }
    else
    {
        ies->uE_S1AP_IDs.present = S1ap_UE_S1AP_IDs_PR_mME_UE_S1AP_ID;
        ies->uE_S1AP_IDs.choice.mME_UE_S1AP_ID = enb_ue->mme_ue_s1ap_id;
    }

    s1ap_build_cause(&ies->cause, cause);

    message.procedureCode = S1ap_ProcedureCode_id_UEContextRelease;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0, return CORE_ERROR,);

    d_trace(3, "[S1AP] UE Context Release Command : "
            "UE[mME-UE-S1AP-ID(%d)] <-- eNB[%s:%d]\n",
            enb_ue->mme_ue_s1ap_id,
            INET_NTOP(&enb_ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb_ue->enb->enb_id);

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

    d_trace(3, "[S1AP] Paging to UE[m_tmsi:0x%x]\n", mme_ue->guti.m_tmsi);

    return CORE_OK;
}

status_t s1ap_build_path_switch_ack(pkbuf_t **s1apbuf, mme_ue_t *mme_ue)
{
    char buf[INET_ADDRSTRLEN];

    int encoded;
    s1ap_message_t message;
    S1ap_PathSwitchRequestAcknowledgeIEs_t *ies = 
        &message.s1ap_PathSwitchRequestAcknowledgeIEs;
    enb_ue_t *enb_ue = NULL;

    d_assert(mme_ue, return CORE_ERROR, "Null param");
    enb_ue = mme_ue->enb_ue;
    d_assert(enb_ue, return CORE_ERROR, "Null param");

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

    d_trace(3, "[S1AP] Path Switch Ack : "
            "UE[mME-UE-S1AP-ID(%d)] <-- eNB[%s:%d]\n",
            enb_ue->mme_ue_s1ap_id,
            INET_NTOP(&enb_ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb_ue->enb->enb_id);

    return CORE_OK;
}

status_t s1ap_build_path_switch_failure(pkbuf_t **s1apbuf,
    c_uint32_t enb_ue_s1ap_id, c_uint32_t mme_ue_s1ap_id, S1ap_Cause_t *cause)
{
    int encoded;
    s1ap_message_t message;
    S1ap_PathSwitchRequestFailureIEs_t *ies = 
        &message.s1ap_PathSwitchRequestFailureIEs;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue_s1ap_id;

    s1ap_build_cause(&ies->cause, cause);

    message.procedureCode = S1ap_ProcedureCode_id_PathSwitchRequest;
    message.direction = S1AP_PDU_PR_unsuccessfulOutcome;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    d_trace(3, "[S1AP] PathSwitchFailure : "
            "UE[eNB-UE-S1AP-ID(%d), mME-UE-S1AP-ID(%d)]",
            enb_ue_s1ap_id, mme_ue_s1ap_id);

    return CORE_OK;
}

status_t s1ap_build_handover_request(
        pkbuf_t **s1apbuf, mme_ue_t *mme_ue, enb_ue_t *enb_ue,
        S1ap_HandoverRequiredIEs_t *required)
{
    char buf[INET_ADDRSTRLEN];

    int encoded;
    s1ap_message_t message;
    S1ap_HandoverRequestIEs_t *ies = &message.s1ap_HandoverRequestIEs;
    S1ap_E_RABToBeSetupItemHOReq_t *e_rab = NULL;
	struct S1ap_GBR_QosInformation *gbrQosInformation = NULL; /* OPTIONAL */

    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;
    s6a_subscription_data_t *subscription_data = NULL;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(mme_ue, return CORE_ERROR, "Null param");
    d_assert(required, return CORE_ERROR, "Null param");
    subscription_data = &mme_ue->subscription_data;
    d_assert(subscription_data, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = enb_ue->mme_ue_s1ap_id;
    ies->handoverType = required->handoverType;
    s1ap_build_cause(&ies->cause, &required->cause);

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

            e_rab->transportLayerAddress.size = 4;
            e_rab->transportLayerAddress.buf = core_calloc(
                    e_rab->transportLayerAddress.size, sizeof(c_uint8_t));
            memcpy(e_rab->transportLayerAddress.buf, &bearer->sgw_s1u_addr,
                    e_rab->transportLayerAddress.size);

            s1ap_uint32_to_OCTET_STRING(bearer->sgw_s1u_teid, &e_rab->gTP_TEID);

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

    d_trace(3, "[S1AP] Handover Request : ",
            "UE[mME-UE-S1AP-ID(%d)] <-- eNB[%s:%d]\n",
            enb_ue->mme_ue_s1ap_id,
            INET_NTOP(&enb_ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb_ue->enb->enb_id);

    return CORE_OK;
}

status_t s1ap_build_handover_command(pkbuf_t **s1apbuf, enb_ue_t *enb_ue)
{
    char buf[INET_ADDRSTRLEN];

    int encoded;
    s1ap_message_t message;
    S1ap_HandoverCommandIEs_t *ies = &message.s1ap_HandoverCommandIEs;
    S1ap_E_RABDataForwardingItem_t *e_rab = NULL;

    mme_ue_t *mme_ue = NULL;
    mme_sess_t *sess = NULL;
    mme_bearer_t *bearer = NULL;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    mme_ue = enb_ue->mme_ue;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = enb_ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;
    ies->handoverType = enb_ue->handover_type;

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
                e_rab->dL_transportLayerAddress->size = 4;
                e_rab->dL_transportLayerAddress->buf = core_calloc(
                        e_rab->dL_transportLayerAddress->size, sizeof(c_uint8_t));
                memcpy(e_rab->dL_transportLayerAddress->buf,
                        &bearer->sgw_dl_addr,
                        e_rab->dL_transportLayerAddress->size);

                e_rab->dL_gTP_TEID = (S1ap_GTP_TEID_t *)
                    core_calloc(1, sizeof(S1ap_GTP_TEID_t));
                s1ap_uint32_to_OCTET_STRING(
                        bearer->sgw_dl_teid, e_rab->dL_gTP_TEID);
            }

            if (MME_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer))
            {
                e_rab->uL_S1ap_TransportLayerAddress =
                    (S1ap_TransportLayerAddress_t *)
                    core_calloc(1, sizeof(S1ap_TransportLayerAddress_t));
                e_rab->uL_S1ap_TransportLayerAddress->size = 4;
                e_rab->uL_S1ap_TransportLayerAddress->buf = core_calloc(
                        e_rab->uL_S1ap_TransportLayerAddress->size,
                        sizeof(c_uint8_t));
                memcpy(e_rab->uL_S1ap_TransportLayerAddress->buf,
                        &bearer->sgw_ul_addr,
                        e_rab->uL_S1ap_TransportLayerAddress->size);

                e_rab->uL_S1ap_GTP_TEID = (S1ap_GTP_TEID_t *)
                    core_calloc(1, sizeof(S1ap_GTP_TEID_t));
                s1ap_uint32_to_OCTET_STRING(
                        bearer->sgw_ul_teid, e_rab->uL_S1ap_GTP_TEID);
            }

            if (MME_HAVE_SGW_DL_INDIRECT_TUNNEL(bearer) ||
                MME_HAVE_SGW_UL_INDIRECT_TUNNEL(bearer))
            {
                ASN_SEQUENCE_ADD(&ies->e_RABDataForwardingList, e_rab);
            }

            bearer = mme_bearer_next(bearer);
        }
        sess = mme_sess_next(sess);
    }

    if (ies->e_RABDataForwardingList.s1ap_E_RABDataForwardingItem.count)
    {
        ies->presenceMask |=
            S1AP_HANDOVERCOMMANDIES_E_RABDATAFORWARDINGLIST_PRESENT;
    }

    s1ap_buffer_to_OCTET_STRING(mme_ue->container.buf, mme_ue->container.size, 
            &ies->target_ToSource_TransparentContainer);
    S1AP_CLEAR_DATA(&mme_ue->container);

    message.procedureCode = S1ap_ProcedureCode_id_HandoverPreparation;
    message.direction = S1AP_PDU_PR_successfulOutcome;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    d_trace(3, "[S1AP] Handover Command : ",
            "UE[mME-UE-S1AP-ID(%d)] <-- eNB[%s:%d]\n",
            enb_ue->mme_ue_s1ap_id,
            INET_NTOP(&enb_ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb_ue->enb->enb_id);

    return CORE_OK;
}

status_t s1ap_build_mme_status_transfer(pkbuf_t **s1apbuf,
        enb_ue_t *enb_ue, S1ap_ENBStatusTransferIEs_t *enb_ies)
{
    char buf[INET_ADDRSTRLEN];

    int encoded;
    s1ap_message_t message;
    S1ap_MMEStatusTransferIEs_t *mme_ies = &message.s1ap_MMEStatusTransferIEs;

    d_assert(enb_ue, return CORE_ERROR, "Null param");
    d_assert(enb_ies, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(s1ap_message_t));

    mme_ies->mme_ue_s1ap_id = enb_ue->mme_ue_s1ap_id;
    mme_ies->eNB_UE_S1AP_ID = enb_ue->enb_ue_s1ap_id;
    memcpy(&mme_ies->eNB_StatusTransfer_TransparentContainer,
            &enb_ies->eNB_StatusTransfer_TransparentContainer,
            sizeof(S1ap_ENB_StatusTransfer_TransparentContainer_t));

    message.procedureCode = S1ap_ProcedureCode_id_MMEStatusTransfer;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);

    d_trace(3, "[S1AP] MME Status Transfer : ",
            "UE[mME-UE-S1AP-ID(%d)] <-- eNB[%s:%d]\n",
            enb_ue->mme_ue_s1ap_id,
            INET_NTOP(&enb_ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            enb_ue->enb->enb_id);

    return CORE_OK;
}

static void s1ap_build_cause(S1ap_Cause_t *dst, S1ap_Cause_t *src)
{
    d_assert(src, return, "Null param");
    d_assert(dst, return, "Null param");

    dst->present = src->present;
    switch(dst->present)
    {
        case S1ap_Cause_PR_radioNetwork:
            dst->choice.radioNetwork = src->choice.radioNetwork;
            break;
        case S1ap_Cause_PR_transport:
            dst->choice.transport = src->choice.transport;
            break;
        case S1ap_Cause_PR_nas:
            dst->choice.nas = src->choice.nas;
            break;
        case S1ap_Cause_PR_protocol:
            dst->choice.protocol = src->choice.protocol;
            break;
        case S1ap_Cause_PR_misc:
            dst->choice.misc = src->choice.misc;
            break;
        default:
            d_error("Invalid src type : %d", dst->present);
            break;
    }
}

