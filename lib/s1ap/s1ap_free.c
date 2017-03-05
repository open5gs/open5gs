#define TRACE_MODULE _s1free

#include "core_debug.h"
#include "s1ap_message.h"

static inline int s1ap_free_initiating_message(s1ap_message *message);
static inline int s1ap_free_successfull_outcome(s1ap_message *message);
static inline int s1ap_free_unsuccessfull_outcome(s1ap_message *message);

int s1ap_free_pdu(s1ap_message *message)
{
    d_assert (message, return -1, "Null param");

    switch (message->direction) 
    {
        case S1AP_PDU_PR_initiatingMessage:
            return s1ap_free_initiating_message(message);

        case S1AP_PDU_PR_successfulOutcome:
            return s1ap_free_successfull_outcome(message);

        case S1AP_PDU_PR_unsuccessfulOutcome:
            return s1ap_free_unsuccessfull_outcome(message);

        default:
            d_warn("Unknown message outcome (%d) or not implemented", 
                    (int)message->direction);
            break;
    }

    return -1;
}

static inline int s1ap_free_initiating_message(s1ap_message *message)
{
    switch (message->procedureCode) 
    {
        case S1ap_ProcedureCode_id_S1Setup:
            s1ap_free_s1ap_s1setuprequesties(
                    &message->s1ap_S1SetupRequestIEs);
            break;

        case S1ap_ProcedureCode_id_initialUEMessage: 
            s1ap_free_s1ap_initialuemessage_ies(
                    &message->s1ap_InitialUEMessage_IEs);
            break;

        case S1ap_ProcedureCode_id_UECapabilityInfoIndication: 
            s1ap_free_s1ap_uecapabilityinfoindicationies(
                    &message->s1ap_UECapabilityInfoIndicationIEs);
            break;

        case S1ap_ProcedureCode_id_InitialContextSetup:
            s1ap_free_s1ap_initialcontextsetuprequesties(
                    &message->s1ap_InitialContextSetupRequestIEs);
            break;

        case S1ap_ProcedureCode_id_downlinkNASTransport:
            s1ap_free_s1ap_downlinknastransport_ies(
                    &message->s1ap_DownlinkNASTransport_IEs);
            break;

        case S1ap_ProcedureCode_id_uplinkNASTransport: 
            s1ap_free_s1ap_uplinknastransport_ies(
                    &message->s1ap_UplinkNASTransport_IEs);
            break;

        case S1ap_ProcedureCode_id_NASNonDeliveryIndication: 
            s1ap_free_s1ap_nasnondeliveryindication_ies(
                    &message->s1ap_NASNonDeliveryIndication_IEs);
            break;

        case S1ap_ProcedureCode_id_UEContextRelease:
            s1ap_free_s1ap_uecontextreleasecommand_ies(
                    &message->s1ap_UEContextReleaseCommand_IEs);
            break;

        case S1ap_ProcedureCode_id_UEContextReleaseRequest: 
            s1ap_free_s1ap_uecontextreleaserequest_ies(
                    &message->s1ap_UEContextReleaseRequest_IEs);
            break;

        default:
            d_warn("Unknown procedure ID (%d) for initiating message\n", 
                    (int)message->procedureCode);
            return -1;
    }

    return 0;
}

static inline int s1ap_free_successfull_outcome(s1ap_message *message)
{
    switch (message->procedureCode) 
    {
        case S1ap_ProcedureCode_id_S1Setup:
            s1ap_free_s1ap_s1setupresponseies(
                    &message->s1ap_S1SetupResponseIEs);
            break;

        case S1ap_ProcedureCode_id_InitialContextSetup: 
            s1ap_free_s1ap_initialcontextsetupresponseies(
                &message->s1ap_InitialContextSetupResponseIEs);
            break;

        case S1ap_ProcedureCode_id_UEContextRelease: 
            s1ap_free_s1ap_uecontextreleasecomplete_ies(
                    &message->s1ap_UEContextReleaseComplete_IEs);
            break;

        default:
            d_warn("Unknown procedure ID (%d) for successfull "
                    "outcome message\n", (int)message->procedureCode);
            return -1;
    }

    return 0;
}

static inline int s1ap_free_unsuccessfull_outcome(s1ap_message *message)
{
    switch (message->procedureCode) 
    {
        case S1ap_ProcedureCode_id_S1Setup:
            s1ap_free_s1ap_s1setupfailureies(
                    &message->s1ap_S1SetupFailureIEs);
            break;

        case S1ap_ProcedureCode_id_InitialContextSetup: 
            s1ap_free_s1ap_initialcontextsetupfailureies(
                &message->s1ap_InitialContextSetupFailureIEs);
            break;

        default:
            d_warn("Unknown procedure ID (%d) for unsuccessfull "
                    "outcome message\n", (int)message->procedureCode);
            return -1;
    }

    return 0;
}
