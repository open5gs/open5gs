#define TRACE_MODULE _s1ap_build

#include "core_debug.h"

#include "mme_context.h"

#include "mme_kdf.h"
#include "s1ap_build.h"
#include "s1ap_conv.h"

status_t s1ap_build_setup_rsp(pkbuf_t **pkbuf)
{
    int erval;
    int i, j;

    s1ap_message_t message;
    S1ap_S1SetupResponseIEs_t *ies = NULL;
    int numServedGUMMEI = 0;
    S1ap_ServedGUMMEIsItem_t *servedGUMMEI;
    S1ap_PLMNidentity_t *plmnIdentity;
    S1ap_MME_Group_ID_t *mmeGroupId;
    S1ap_MME_Code_t *mmeCode;

    memset(&message, 0, sizeof(s1ap_message_t));

    ies = &message.s1ap_S1SetupResponseIEs;

    numServedGUMMEI = 1;
    servedGUMMEI = (S1ap_ServedGUMMEIsItem_t *)
        core_calloc(numServedGUMMEI, sizeof(S1ap_ServedGUMMEIsItem_t));
    for (i = 0; i < numServedGUMMEI; i++)
    {
        srvd_gummei_t *srvd_gummei = &mme_self()->srvd_gummei;

        for (j = 0; j < srvd_gummei->num_of_plmn_id; j++)
        {
            plmnIdentity = (S1ap_PLMNidentity_t *)
                core_calloc(1, sizeof(S1ap_PLMNidentity_t));
            s1ap_buffer_to_OCTET_STRING(
                    &srvd_gummei->plmn_id[j], PLMN_ID_LEN, plmnIdentity);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedPLMNs, plmnIdentity);
        }

        for (j = 0; j < srvd_gummei->num_of_mme_gid; j++)
        {
            mmeGroupId = (S1ap_MME_Group_ID_t *)
                core_calloc(1, sizeof(S1ap_MME_Group_ID_t));
            s1ap_uint16_to_OCTET_STRING(
                srvd_gummei->mme_gid[j], mmeGroupId);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedGroupIDs, mmeGroupId);
        }

        for (j = 0; j < srvd_gummei->num_of_mme_code; j++)
        {
            mmeCode = (S1ap_MME_Code_t *)
                core_calloc(1, sizeof(S1ap_MME_Code_t));
            s1ap_uint8_to_OCTET_STRING(
                srvd_gummei->mme_code[j], mmeCode);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedMMECs, mmeCode);
        }
    }
    ASN_SEQUENCE_ADD(&ies->servedGUMMEIs, servedGUMMEI);

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
            pkbuf_t **s1apbuf, mme_ue_t *ue, pkbuf_t *emmbuf)
{
    char buf[INET_ADDRSTRLEN];
    
    int encoded;
    s1ap_message_t message;
    S1ap_DownlinkNASTransport_IEs_t *ies = 
        &message.s1ap_DownlinkNASTransport_IEs;
    S1ap_NAS_PDU_t *nasPdu = &ies->nas_pdu;

    d_assert(emmbuf, return CORE_ERROR, "Null param");
    d_assert(ue, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = ue->enb_ue_s1ap_id;

    nasPdu->size = emmbuf->len;
    nasPdu->buf = core_calloc(nasPdu->size, sizeof(c_uint8_t));
    memcpy(nasPdu->buf, emmbuf->payload, nasPdu->size);

    message.procedureCode = S1ap_ProcedureCode_id_downlinkNASTransport;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);
    pkbuf_free(emmbuf);

    d_info("[S1AP] downlinkNASTransport : "
            "UE[eNB-UE-S1AP-ID(%d)] <-- eNB[%s:%d]",
            ue->enb_ue_s1ap_id,
            INET_NTOP(&ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            ue->enb->enb_id);

    return CORE_OK;
}

status_t s1ap_build_initial_context_setup_request(
            pkbuf_t **s1apbuf, mme_bearer_t *bearer, pkbuf_t *emmbuf)
{
    char buf[INET_ADDRSTRLEN];

    int encoded;
    s1ap_message_t message;
    S1ap_InitialContextSetupRequestIEs_t *ies =
            &message.s1ap_InitialContextSetupRequestIEs;
    S1ap_E_RABToBeSetupItemCtxtSUReq_t *e_rab = NULL;
    S1ap_NAS_PDU_t *nasPdu = NULL;
    mme_ue_t *ue = NULL;
    pdn_t *pdn = NULL;

    d_assert(emmbuf, return CORE_ERROR, "Null param");
    d_assert(bearer, return CORE_ERROR, "Null param");
    ue = bearer->ue;
    d_assert(ue, return CORE_ERROR, "Null param");
    pdn = bearer->pdn;
    d_assert(pdn, return CORE_ERROR, "Null param");

    memset(&message, 0, sizeof(s1ap_message_t));

    ies->mme_ue_s1ap_id = ue->mme_ue_s1ap_id;
    ies->eNB_UE_S1AP_ID = ue->enb_ue_s1ap_id;

    asn_uint642INTEGER(
            &ies->uEaggregateMaximumBitrate.uEaggregateMaximumBitRateUL, 
            ue->max_bandwidth_ul);
    asn_uint642INTEGER(
            &ies->uEaggregateMaximumBitrate.uEaggregateMaximumBitRateDL, 
            ue->max_bandwidth_dl);

    e_rab = (S1ap_E_RABToBeSetupItemCtxtSUReq_t *)
        core_calloc(1, sizeof(S1ap_E_RABToBeSetupItemCtxtSUReq_t));
    e_rab->e_RAB_ID = bearer->ebi;
    e_rab->e_RABlevelQoSParameters.qCI = pdn->qci;

    e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
        priorityLevel = pdn->priority_level;
    e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
        pre_emptionCapability = 
        S1ap_Pre_emptionCapability_shall_not_trigger_pre_emption;
    e_rab->e_RABlevelQoSParameters.allocationRetentionPriority.
        pre_emptionVulnerability = 
            S1ap_Pre_emptionVulnerability_not_pre_emptable;
    e_rab->transportLayerAddress.size = 4;
    e_rab->transportLayerAddress.buf = 
        core_calloc(e_rab->transportLayerAddress.size, sizeof(c_uint8_t));
    memcpy(e_rab->transportLayerAddress.buf, &bearer->sgw_s1u_addr,
            e_rab->transportLayerAddress.size);

    s1ap_uint32_to_OCTET_STRING(bearer->sgw_s1u_teid, &e_rab->gTP_TEID);

    nasPdu = (S1ap_NAS_PDU_t *)core_calloc(1, sizeof(S1ap_NAS_PDU_t));
    nasPdu->size = emmbuf->len;
    nasPdu->buf = core_calloc(nasPdu->size, sizeof(c_uint8_t));
    memcpy(nasPdu->buf, emmbuf->payload, nasPdu->size);
    e_rab->nAS_PDU = nasPdu;

    ASN_SEQUENCE_ADD(&ies->e_RABToBeSetupListCtxtSUReq, e_rab);

    ies->securityKey.size = SHA256_DIGEST_SIZE;
    ies->securityKey.buf = 
        core_calloc(ies->securityKey.size, sizeof(c_uint8_t));
    memcpy(ies->securityKey.buf, ue->kenb, ies->securityKey.size);

    message.procedureCode = S1ap_ProcedureCode_id_InitialContextSetup;
    message.direction = S1AP_PDU_PR_initiatingMessage;

    encoded = s1ap_encode_pdu(s1apbuf, &message);
    s1ap_free_pdu(&message);

    d_assert(s1apbuf && encoded >= 0,return CORE_ERROR,);
    pkbuf_free(emmbuf);

    d_info("[S1AP] Initial Context Setup Request : "
            "UE[eNB-UE-S1AP-ID(%d)] <-- eNB[%s:%d]",
            ue->enb_ue_s1ap_id,
            INET_NTOP(&ue->enb->s1ap_sock->remote.sin_addr.s_addr, buf),
            ue->enb->enb_id);

    return CORE_OK;
}
