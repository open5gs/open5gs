#define TRACE_MODULE _s1enc

#include "core_debug.h"
#include "s1ap_codecs.h"

static inline int s1ap_encode_initiating(
    s1ap_message *message_p, pkbuf_t *pkbuf);
static inline int s1ap_encode_successfull_outcome(
    s1ap_message *message_p, pkbuf_t *pkbuf);
static inline int s1ap_encode_unsuccessfull_outcome(
    s1ap_message *message_p, pkbuf_t *pkbuf);

static inline int s1ap_encode_initial_context_setup_request(
    s1ap_message *message_p, pkbuf_t *pkbuf);
static inline int s1ap_encode_s1setupresponse(
    s1ap_message *message_p, pkbuf_t *pkbuf);
static inline int s1ap_encode_s1setupfailure(
    s1ap_message *message_p, pkbuf_t *pkbuf);
static inline int s1ap_encode_downlink_nas_transport(
  s1ap_message *message_p, pkbuf_t *pkbuf);
static inline int s1ap_encode_ue_context_release_command (
  s1ap_message *message_p, pkbuf_t *pkbuf);

ssize_t s1ap_generate_initiating_message(pkbuf_t *pkbuf,
    e_S1ap_ProcedureCode procedureCode, S1ap_Criticality_t criticality,
    asn_TYPE_descriptor_t * td, void *sptr);
ssize_t s1ap_generate_successfull_outcome(pkbuf_t *pkbuf,
    e_S1ap_ProcedureCode procedureCode, S1ap_Criticality_t criticality,
    asn_TYPE_descriptor_t * td, void *sptr);
ssize_t s1ap_generate_unsuccessfull_outcome(pkbuf_t *pkbuf,
    e_S1ap_ProcedureCode procedureCode, S1ap_Criticality_t criticality,
    asn_TYPE_descriptor_t * td, void *sptr);

int s1ap_encode_pdu(pkbuf_t **pkb, s1ap_message *message_p)
{
    int encoded = -1;

    d_assert (message_p, return -1, "Null param");

    *pkb = pkbuf_alloc(0, S1AP_SDU_SIZE);
    d_assert(*pkb, return -1, "Null Param");

    switch (message_p->direction) 
    {
        case S1AP_PDU_PR_initiatingMessage:
            encoded = s1ap_encode_initiating(message_p, *pkb);
            break;

        case S1AP_PDU_PR_successfulOutcome:
            encoded = s1ap_encode_successfull_outcome(message_p, *pkb);
            break;

        case S1AP_PDU_PR_unsuccessfulOutcome:
            encoded = s1ap_encode_unsuccessfull_outcome(message_p, *pkb);
            break;

        default:
            d_warn("Unknown message outcome (%d) or not implemented", 
                    (int)message_p->direction);
            pkbuf_free(*pkb);
            return -1;
    }

    if (encoded < 0)
    {
        pkbuf_free(*pkb);
        return -1;
    }

    (*pkb)->len = encoded;
    return encoded;
}

static inline int s1ap_encode_initiating(
    s1ap_message *message_p, pkbuf_t *pkbuf)
{
    switch (message_p->procedureCode) 
    {
        case S1ap_ProcedureCode_id_downlinkNASTransport:
            return s1ap_encode_downlink_nas_transport(message_p, pkbuf);

        case S1ap_ProcedureCode_id_InitialContextSetup:
            return s1ap_encode_initial_context_setup_request(message_p, pkbuf);

        case S1ap_ProcedureCode_id_UEContextRelease:
            return s1ap_encode_ue_context_release_command(message_p, pkbuf);

        default:
            d_warn("Unknown procedure ID (%d) for initiating message_p\n", 
                    (int)message_p->procedureCode);
            break;
    }

    return -1;
}

static inline int s1ap_encode_successfull_outcome(
    s1ap_message *message_p, pkbuf_t *pkbuf)
{
    switch (message_p->procedureCode) 
    {
        case S1ap_ProcedureCode_id_S1Setup:
            return s1ap_encode_s1setupresponse(message_p, pkbuf);

        default:
            d_warn("Unknown procedure ID (%d) for successfull "
                    "outcome message\n", (int)message_p->procedureCode);
        break;
    }

    return -1;
}

static inline int s1ap_encode_unsuccessfull_outcome(
    s1ap_message *message_p, pkbuf_t *pkbuf)
{
    switch (message_p->procedureCode) 
    {
        case S1ap_ProcedureCode_id_S1Setup:
            return s1ap_encode_s1setupfailure(message_p, pkbuf);

        default:
            d_warn("Unknown procedure ID (%d) for unsuccessfull "
                    "outcome message\n", (int)message_p->procedureCode);
        break;
    }

    return -1;
}

static inline int s1ap_encode_initial_context_setup_request(
    s1ap_message *message_p, pkbuf_t *pkbuf)
{
    S1ap_InitialContextSetupRequest_t initialContextSetupRequest;
    S1ap_InitialContextSetupRequest_t *initialContextSetupRequest_p = 
                                        &initialContextSetupRequest;

    memset(initialContextSetupRequest_p, 0, 
            sizeof(S1ap_InitialContextSetupRequest_t));

    if (s1ap_encode_s1ap_initialcontextsetuprequesties(
            initialContextSetupRequest_p, 
            &message_p->msg.s1ap_InitialContextSetupRequestIEs) < 0) 
    {
        return -1;
    }

    return s1ap_generate_initiating_message(pkbuf,
            S1ap_ProcedureCode_id_InitialContextSetup, 
            S1ap_Criticality_reject, &asn_DEF_S1ap_InitialContextSetupRequest, 
            initialContextSetupRequest_p);
}

static inline int s1ap_encode_s1setupresponse(
    s1ap_message *message_p, pkbuf_t *pkbuf)
{
    S1ap_S1SetupResponse_t s1SetupResponse;
    S1ap_S1SetupResponse_t *s1SetupResponse_p = &s1SetupResponse;

    memset(s1SetupResponse_p, 0, sizeof (S1ap_S1SetupResponse_t));

    if (s1ap_encode_s1ap_s1setupresponseies(
            s1SetupResponse_p, &message_p->msg.s1ap_S1SetupResponseIEs) < 0) 
    {
        return -1;
    }

    return s1ap_generate_successfull_outcome(pkbuf,
            S1ap_ProcedureCode_id_S1Setup, message_p->criticality, 
            &asn_DEF_S1ap_S1SetupResponse, s1SetupResponse_p);
}

static inline int s1ap_encode_s1setupfailure(
    s1ap_message *message_p, pkbuf_t *pkbuf)
{
    S1ap_S1SetupFailure_t s1SetupFailure;
    S1ap_S1SetupFailure_t *s1SetupFailure_p = &s1SetupFailure;

    memset(s1SetupFailure_p, 0, sizeof (S1ap_S1SetupFailure_t));

    if (s1ap_encode_s1ap_s1setupfailureies(
            s1SetupFailure_p, &message_p->msg.s1ap_S1SetupFailureIEs) < 0) 
    {
        return -1;
    }

    return s1ap_generate_unsuccessfull_outcome(pkbuf,
            S1ap_ProcedureCode_id_S1Setup, message_p->criticality, 
            &asn_DEF_S1ap_S1SetupFailure, s1SetupFailure_p);
}

static inline int s1ap_encode_downlink_nas_transport(
  s1ap_message *message_p, pkbuf_t *pkbuf)
{
    S1ap_DownlinkNASTransport_t downlinkNasTransport;
    S1ap_DownlinkNASTransport_t *downlinkNasTransport_p = &downlinkNasTransport;

    memset(downlinkNasTransport_p, 0, sizeof(S1ap_DownlinkNASTransport_t));

    /*
    * Convert IE structure into asn1 message_p
    */
    if (s1ap_encode_s1ap_downlinknastransport_ies(downlinkNasTransport_p, 
            &message_p->msg.s1ap_DownlinkNASTransport_IEs) < 0) 
    {
        return -1;
    }

    /*
    * Generate Initiating message_p for the list of IEs
    */
    return s1ap_generate_initiating_message(pkbuf,
            S1ap_ProcedureCode_id_downlinkNASTransport, message_p->criticality,
            &asn_DEF_S1ap_DownlinkNASTransport, downlinkNasTransport_p);
}

static inline int s1ap_encode_ue_context_release_command (
  s1ap_message *message_p, pkbuf_t *pkbuf)
{
    S1ap_UEContextReleaseCommand_t ueContextReleaseCommand;
    S1ap_UEContextReleaseCommand_t *ueContextReleaseCommand_p = 
                                    &ueContextReleaseCommand;

    memset(ueContextReleaseCommand_p, 0, 
            sizeof(S1ap_UEContextReleaseCommand_t));

    /*
    * Convert IE structure into asn1 message_p
    */
    if (s1ap_encode_s1ap_uecontextreleasecommand_ies(ueContextReleaseCommand_p,
            &message_p->msg.s1ap_UEContextReleaseCommand_IEs) < 0) 
    {
        return -1;
    }

    return s1ap_generate_initiating_message(pkbuf,
            S1ap_ProcedureCode_id_UEContextRelease, message_p->criticality, 
            &asn_DEF_S1ap_UEContextReleaseCommand, ueContextReleaseCommand_p);
}


ssize_t s1ap_generate_initiating_message(pkbuf_t *pkbuf,
    e_S1ap_ProcedureCode procedureCode, S1ap_Criticality_t criticality,
    asn_TYPE_descriptor_t * td, void *sptr)
{
    S1AP_PDU_t pdu;
    asn_enc_rval_t enc_ret = {0};

    memset(&pdu, 0, sizeof (S1AP_PDU_t));
    pdu.present = S1AP_PDU_PR_initiatingMessage;
    pdu.choice.initiatingMessage.procedureCode = procedureCode;
    pdu.choice.initiatingMessage.criticality = criticality;
    ANY_fromType_aper(&pdu.choice.initiatingMessage.value, td, sptr);

    /*
    * We can safely free list of IE from sptr
    */
    ASN_STRUCT_FREE_CONTENTS_ONLY (*td, sptr);

    enc_ret = aper_encode_to_buffer(&asn_DEF_S1AP_PDU, 
                    &pdu, pkbuf->payload, S1AP_SDU_SIZE);
    if (enc_ret.encoded < 0)
    {
        d_error("Encoding of %s failed", td->name);
        return -1;
    }

    return enc_ret.encoded;
}

ssize_t s1ap_generate_successfull_outcome(pkbuf_t *pkbuf,
    e_S1ap_ProcedureCode procedureCode, S1ap_Criticality_t criticality,
    asn_TYPE_descriptor_t * td, void *sptr)
{
    S1AP_PDU_t pdu;
    asn_enc_rval_t enc_ret = {0};

    memset(&pdu, 0, sizeof (S1AP_PDU_t));
    pdu.present = S1AP_PDU_PR_successfulOutcome;
    pdu.choice.successfulOutcome.procedureCode = procedureCode;
    pdu.choice.successfulOutcome.criticality = criticality;
    ANY_fromType_aper(&pdu.choice.successfulOutcome.value, td, sptr);

    /*
    * We can safely free list of IE from sptr
    */
    ASN_STRUCT_FREE_CONTENTS_ONLY (*td, sptr);

    enc_ret = aper_encode_to_buffer(&asn_DEF_S1AP_PDU, 
                    &pdu, pkbuf->payload, S1AP_SDU_SIZE);
    if (enc_ret.encoded < 0)
    {
        d_error("Encoding of %s failed", td->name);
        return -1;
    }

    return enc_ret.encoded;
}

ssize_t s1ap_generate_unsuccessfull_outcome(pkbuf_t *pkbuf,
    e_S1ap_ProcedureCode procedureCode, S1ap_Criticality_t criticality,
    asn_TYPE_descriptor_t * td, void *sptr)
{
    S1AP_PDU_t pdu;
    asn_enc_rval_t enc_ret = {0};

    memset(&pdu, 0, sizeof (S1AP_PDU_t));
    pdu.present = S1AP_PDU_PR_unsuccessfulOutcome;
    pdu.choice.successfulOutcome.procedureCode = procedureCode;
    pdu.choice.successfulOutcome.criticality = criticality;
    ANY_fromType_aper(&pdu.choice.successfulOutcome.value, td, sptr);

    /*
    * We can safely free list of IE from sptr
    */
    ASN_STRUCT_FREE_CONTENTS_ONLY (*td, sptr);

    enc_ret = aper_encode_to_buffer(&asn_DEF_S1AP_PDU, 
                    &pdu, pkbuf->payload, S1AP_SDU_SIZE);
    if (enc_ret.encoded < 0)
    {
        d_error("Encoding of %s failed", td->name);
        return -1;
    }

    return enc_ret.encoded;
}

