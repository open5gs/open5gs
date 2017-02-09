#define TRACE_MODULE _s1msg

#include "core_debug.h"
#include "context.h"
#include "s1ap_message.h"

status_t s1ap_build_setup_rsp(pkbuf_t **pkbuf)
{
    int erval;
    int i, j;

    s1ap_message message;
    S1ap_S1SetupResponseIEs_t *ies = NULL;
    int numServedGUMMEI = 0;
    S1ap_ServedGUMMEIsItem_t *servedGUMMEI;
    S1ap_PLMNidentity_t *plmnIdentity;
    S1ap_MME_Group_ID_t *mmeGroupId;
    S1ap_MME_Code_t *mmeCode;

    uint16_t mcc = 0x1234;
    uint16_t mnc = 0x5678;
    uint16_t mnc_digit_len = 2;

    memset(&message, 0, sizeof(s1ap_message));

    ies = &message.msg.s1ap_S1SetupResponseIEs;
    ies->relativeMMECapacity = mme_self()->relative_capacity;

    numServedGUMMEI = 1;

    servedGUMMEI = (S1ap_ServedGUMMEIsItem_t *)
        CALLOC(numServedGUMMEI, sizeof(S1ap_ServedGUMMEIsItem_t));
    for (i = 0; i < numServedGUMMEI; i++)
    {
        srvd_gummei_t *srvd_gummei = &mme_self()->srvd_gummei;

        for (j = 0; j < srvd_gummei->num_of_plmn_id; j++)
        {
            plmnIdentity = (S1ap_PLMNidentity_t *)
                CALLOC(srvd_gummei->num_of_plmn_id, 
                    sizeof(S1ap_PLMNidentity_t));
            MCC_MNC_TO_TBCD(mcc, mnc, mnc_digit_len, plmnIdentity);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedPLMNs, plmnIdentity);
        }

        for (j = 0; j < srvd_gummei->num_of_grp_id; j++)
        {
            mmeGroupId = (S1ap_MME_Group_ID_t *)
                CALLOC(srvd_gummei->num_of_grp_id, 
                        sizeof(S1ap_MME_Group_ID_t));
            INT16_TO_OCTET_STRING(srvd_gummei->grp_id[j], mmeGroupId);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedGroupIDs, mmeGroupId);
        }

        for (j = 0; j < srvd_gummei->num_of_code; j++)
        {
            mmeCode = (S1ap_MME_Code_t *)
                CALLOC(srvd_gummei->num_of_grp_id, 
                        sizeof(S1ap_MME_Code_t));
            INT8_TO_OCTET_STRING(srvd_gummei->code[j], mmeCode);
            ASN_SEQUENCE_ADD(&servedGUMMEI->servedMMECs, mmeCode);
        }
    }

    ASN_SEQUENCE_ADD(&ies->servedGUMMEIs, servedGUMMEI);
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


