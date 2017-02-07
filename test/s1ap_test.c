#include "core.h"
#include "core_errno.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pkbuf.h"

#include "testutil.h"

#include "s1ap_message.h"

static void s1ap_test1(abts_case *tc, void *data)
{
    /* S1SetupRequest */
    char *payload[] = {
        "\x00\x11\x00\x2d\x00\x00\x04\x00\x3b\x00\x09\x00\x00\xf1\x10\x40\x54\xf6\x40\x10\x00\x3c\x40\x09\x03\x00\x4a\x4c\x54\x2d\x36\x32\x31\x00\x40\x00\x07\x00\x0c\x0e\x40\x00\xf1\x10\x00\x89\x40\x01\x00",
    };

    s1ap_message message;
    pkbuf_t *pkb;
    int result;

    pkb = pkbuf_alloc(0, S1AP_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkb);
    pkb->len = 49;
    memcpy(pkb->payload, payload[0], pkb->len);

    result = s1ap_decode_pdu(&message, pkb);
    ABTS_INT_EQUAL(tc, 0, result);
}

static void s1ap_test2(abts_case *tc, void *data)
{
    /* InitialUE(Attach Request) */
    char *payload[] = {
        "\x00\x0c\x40\x6f\x00\x00\x06\x00\x08\x00\x02\x00\x01\x00\x1a\x00\x3c\x3b\x17\xdf\x67\x5a\xa8\x05\x07\x41\x02\x0b\xf6\x00\xf1\x10\x00\x02\x01\x03\x00\x03\xe6\x05\xf0\x70\x00\x00\x10\x00\x05\x02\x15\xd0\x11\xd1\x52\x00\xf1\x10\x30\x39\x5c\x0a\x00\x31\x03\xe5\xe0\x34\x90\x11\x03\x57\x58\xa6\x5d\x01\x00\xe0\xc1\x00\x43\x00\x06\x00\x00\xf1\x10\x30\x39\x00\x64\x40\x08\x00\x00\xf1\x10\x8c\x33\x78\x20\x00\x86\x40\x01\x30\x00\x4b\x00\x07\x00\x00\xf1\x10\x00\x02\x01"
    };

    s1ap_message message;
    pkbuf_t *pkb;
    int result;

    pkb = pkbuf_alloc(0, S1AP_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkb);
    pkb->len = 115;
    memcpy(pkb->payload, payload[0], pkb->len);

    result = s1ap_decode_pdu(&message, pkb);
    ABTS_INT_EQUAL(tc, 0, result);
}

static void s1ap_test3(abts_case *tc, void *data)
{
    /* initial context setup response */
    char *payload[] = {
        "\x20\x09\x00\x25\x00\x00\x03\x00\x00\x40\x05\xc0\x02\x00\x00\xbf\x00\x08\x40\x02\x00\x01\x00\x33\x40\x0f\x00\x00\x32\x40\x0a\x0a\x1f\x0a\x01\x23\xc6\x01\x00\x09\x08"
    };

    s1ap_message message;
    pkbuf_t *pkb;
    int result;

    pkb = pkbuf_alloc(0, S1AP_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkb);
    pkb->len = 41;
    memcpy(pkb->payload, payload[0], pkb->len);

    result = s1ap_decode_pdu(&message, pkb);
    ABTS_INT_EQUAL(tc, 0, result);
}

static void s1ap_test4(abts_case *tc, void *data)
{
    status_t rv;
    pkbuf_t *pkbuf;

    rv = s1ap_build_setup_rsp(&pkbuf);

    ABTS_INT_EQUAL(tc, CORE_OK, rv);
#if  0
    ABTS_PTR_NOTNULL(tc, pkbuf);
    ABTS_PTR_NOTNULL(tc, pkbuf->payload);
    ABTS_INT_EQUAL(tc, 216, pkbuf->len);
#endif
}

static void s1ap_test5(abts_case *tc, void *data)
{
    int erval = -1;
    pkbuf_t *pkbuf;

    s1ap_message message;
    S1ap_S1SetupRequestIEs_t *s1SetupRequestIEs;
    S1ap_PLMNidentity_t *plmnIdentity;
    S1ap_SupportedTAs_Item_t *supportedTA;

    uint16_t mcc = 0x1234;
    uint16_t mnc = 0x5678;
    uint16_t mnc_digit_len = 2;
    uint32_t enb_id = 0x5f123;
    uint32_t tac = 0x1234;

    memset(&message, 0, sizeof(s1ap_message));

    s1SetupRequestIEs = &message.msg.s1ap_S1SetupRequestIEs;

    s1SetupRequestIEs->global_ENB_ID.eNB_ID.present = 
        S1ap_ENB_ID_PR_macroENB_ID;
    MACRO_ENB_ID_TO_BIT_STRING(enb_id,
         &s1SetupRequestIEs->global_ENB_ID.eNB_ID.choice.macroENB_ID);
    MCC_MNC_TO_PLMNID(mcc, mnc, mnc_digit_len,
        &s1SetupRequestIEs->global_ENB_ID.pLMNidentity);

    supportedTA = (S1ap_SupportedTAs_Item_t *)
        calloc(1, sizeof(S1ap_SupportedTAs_Item_t));
    INT16_TO_OCTET_STRING(tac, &supportedTA->tAC);
    plmnIdentity = (S1ap_PLMNidentity_t *)
        calloc(1, sizeof(S1ap_PLMNidentity_t));
    MCC_MNC_TO_TBCD(mcc, mnc, mnc_digit_len, plmnIdentity);
    ASN_SEQUENCE_ADD(&supportedTA->broadcastPLMNs, plmnIdentity);

    ASN_SEQUENCE_ADD(&s1SetupRequestIEs->supportedTAs, supportedTA);

    message.direction = S1AP_PDU_PR_initiatingMessage;
    message.procedureCode = S1ap_ProcedureCode_id_S1Setup;
    message.criticality = S1ap_Criticality_reject;

    erval = s1ap_encode_pdu(&pkbuf, &message);

    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_Global_ENB_ID, 
            &s1SetupRequestIEs->global_ENB_ID);
    ASN_STRUCT_FREE_CONTENTS_ONLY(asn_DEF_S1ap_SupportedTAs, 
            &s1SetupRequestIEs->supportedTAs);

    ABTS_INT_EQUAL(tc, 280, erval);
}

abts_suite *test_s1ap(abts_suite *suite)
{
    suite = ADD_SUITE(suite)
    {
        extern int _s1dec;
        d_trace_level(&_s1dec, 100);
    }

    abts_run_test(suite, s1ap_test1, NULL);
    abts_run_test(suite, s1ap_test2, NULL);
    abts_run_test(suite, s1ap_test3, NULL);
    abts_run_test(suite, s1ap_test4, NULL);
    abts_run_test(suite, s1ap_test5, NULL);

    return suite;
}
