#define TRACE_MODULE _s1msg

#include "context.h"
#include "s1ap_message.h"

status_t s1ap_build_setup_rsp(pkbuf_t **pkbuf)
{
    int erval;
    int i, j;

    s1ap_message message;
    S1ap_S1SetupResponseIEs_t *ies = NULL;
    int num_of_gummei = 0;
    S1ap_ServedGUMMEIsItem_t servedGUMMEI;
    S1ap_PLMNidentity_t plmn_id;
    S1ap_MME_Group_ID_t grp_id;
    S1ap_MME_Code_t code;

    memset(&message, 0, sizeof(s1ap_message));

    ies = &message.msg.s1ap_S1SetupResponseIEs;
    ies->relativeMMECapacity = mme_self()->relative_capacity;

    num_of_gummei = 1;
    for (i = 0; i < num_of_gummei; i++)
    {
        srvd_gummei_t *srvd_gummei = &mme_self()->srvd_gummei;

        for (j = 0; i < srvd_gummei->num_of_plmn_id; j++)
        {
            plmn_id.buf = calloc(3, sizeof(uint8_t));                           \
            plmn_id.buf[0] = 0xf1;
            plmn_id.buf[1] = 0xea;
            plmn_id.buf[2] = 0xaa;
            plmn_id.size = 3;
            ASN_SEQUENCE_ADD(&servedGUMMEI.servedPLMNs.list, &plmn_id);
        }

        for (j = 0; i < srvd_gummei->num_of_grp_id; j++)
        {
            INT16_TO_OCTET_STRING(srvd_gummei->grp_id[j], &grp_id);
            ASN_SEQUENCE_ADD(&servedGUMMEI.servedGroupIDs.list, &grp_id);
        }

        for (j = 0; i < srvd_gummei->num_of_code; j++)
        {
            INT16_TO_OCTET_STRING(srvd_gummei->code[j], &code);
            ASN_SEQUENCE_ADD(&servedGUMMEI.servedMMECs.list, &code);
        }
    }

    ASN_SEQUENCE_ADD(&ies->servedGUMMEIs, &servedGUMMEI);
    message.procedureCode = S1ap_ProcedureCode_id_S1Setup;
    message.direction = S1AP_PDU_PR_successfulOutcome;

    erval = s1ap_encode_pdu(pkbuf, &message);
    if (erval < 0)
        return CORE_ERROR;

    return CORE_OK;
}


