#define TRACE_MODULE _s1ap_build

#include "core_debug.h"
#include "context.h"
#include "s1ap_build.h"
#include "s1ap_conv.h"
#include "nas_message.h"

void s1ap_send_nas_to_emm(ue_ctx_t *ue, S1ap_NAS_PDU_t *nasPdu)
{
    pkbuf_t *sendbuf = NULL;
    event_t e;

    d_assert(nasPdu, return, "Null param");

    /* The Packet Buffer(pkbuf_t) for NAS message MUST make a HEADROOM. 
     * When calculating AES_CMAC, we need to use the headroom of the packet. */
    sendbuf = pkbuf_alloc(NAS_HEADROOM, nasPdu->size);
    d_assert(sendbuf, return, "Null param");
    memcpy(sendbuf->payload, nasPdu->buf, nasPdu->size);

    event_set(&e, EVT_MSG_UE_EMM);
    event_set_param1(&e, (c_uintptr_t)ue);
    event_set_param2(&e, (c_uintptr_t)sendbuf);

    event_send(mme_self()->queue_id, &e);
}

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
