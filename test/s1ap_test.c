#include "core.h"
#include "core_errno.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pkbuf.h"

#include "testutil.h"

#include "s1ap_message.h"
#include "s1ap_conv.h"

static void s1ap_test1(abts_case *tc, void *data)
{
    /* S1SetupRequest */
    char *payload[] = {
        "\x00\x11\x00\x2d\x00\x00\x04\x00\x3b\x00\x09\x00\x00\xf1\x10\x40\x54\xf6\x40\x10\x00\x3c\x40\x09\x03\x00\x4a\x4c\x54\x2d\x36\x32\x31\x00\x40\x00\x07\x00\x0c\x0e\x40\x00\xf1\x10\x00\x89\x40\x01\x00",
    };

    s1ap_message message;
    pkbuf_t *pkbuf;
    int result;

    pkbuf = pkbuf_alloc(0, S1AP_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 49;
    memcpy(pkbuf->payload, payload[0], pkbuf->len);

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    s1ap_free_pdu(&message);

    pkbuf_free(pkbuf);
}

static void s1ap_test2(abts_case *tc, void *data)
{
    /* InitialUE(Attach Request) */
    char *payload[] = {
        "\x00\x0c\x40\x6f\x00\x00\x06\x00\x08\x00\x02\x00\x01\x00\x1a\x00\x3c\x3b\x17\xdf\x67\x5a\xa8\x05\x07\x41\x02\x0b\xf6\x00\xf1\x10\x00\x02\x01\x03\x00\x03\xe6\x05\xf0\x70\x00\x00\x10\x00\x05\x02\x15\xd0\x11\xd1\x52\x00\xf1\x10\x30\x39\x5c\x0a\x00\x31\x03\xe5\xe0\x34\x90\x11\x03\x57\x58\xa6\x5d\x01\x00\xe0\xc1\x00\x43\x00\x06\x00\x00\xf1\x10\x30\x39\x00\x64\x40\x08\x00\x00\xf1\x10\x8c\x33\x78\x20\x00\x86\x40\x01\x30\x00\x4b\x00\x07\x00\x00\xf1\x10\x00\x02\x01"
    };

    s1ap_message message;
    pkbuf_t *pkbuf;
    int result;

    pkbuf = pkbuf_alloc(0, S1AP_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 115;
    memcpy(pkbuf->payload, payload[0], pkbuf->len);

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    s1ap_free_pdu(&message);

    pkbuf_free(pkbuf);
}

static void s1ap_test3(abts_case *tc, void *data)
{
    /* initial context setup response */
    char *payload[] = {
        "\x20\x09\x00\x25\x00\x00\x03\x00\x00\x40\x05\xc0\x02\x00\x00\xbf\x00\x08\x40\x02\x00\x01\x00\x33\x40\x0f\x00\x00\x32\x40\x0a\x0a\x1f\x0a\x01\x23\xc6\x01\x00\x09\x08"
    };

    s1ap_message message;
    pkbuf_t *pkbuf;
    int result;

    pkbuf = pkbuf_alloc(0, S1AP_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    pkbuf->len = 41;
    memcpy(pkbuf->payload, payload[0], pkbuf->len);

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    s1ap_free_pdu(&message);

    pkbuf_free(pkbuf);
}

static void s1ap_test4(abts_case *tc, void *data)
{
    status_t rv;
    pkbuf_t *pkbuf;

    rv = s1ap_build_setup_rsp(&pkbuf);

    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    ABTS_PTR_NOTNULL(tc, pkbuf->payload);
    ABTS_INT_EQUAL(tc, 216, pkbuf->len);

    pkbuf_free(pkbuf);
}

static void s1ap_test5(abts_case *tc, void *data)
{
    int erval = -1;
    pkbuf_t *pkbuf;

    s1ap_message message;
    S1ap_S1SetupRequestIEs_t *ies;
    S1ap_PLMNidentity_t *plmnIdentity;
    S1ap_SupportedTAs_Item_t *supportedTA;

    memset(&message, 0, sizeof(s1ap_message));

    ies = &message.msg.s1ap_S1SetupRequestIEs;

    ies->global_ENB_ID.eNB_ID.present = S1ap_ENB_ID_PR_macroENB_ID;
    s1ap_conv_macro_enb_id_to_bit_string(0x5f123, 
         &ies->global_ENB_ID.eNB_ID.choice.macroENB_ID);
    s1ap_conv_plmn_id_to_tbcd_string(
        &mme_self()->plmn_id, &ies->global_ENB_ID.pLMNidentity);

    supportedTA = (S1ap_SupportedTAs_Item_t *)
        core_calloc(1, sizeof(S1ap_SupportedTAs_Item_t));
    s1ap_conv_uint16_to_octet_string(mme_self()->tac, &supportedTA->tAC);
    plmnIdentity = (S1ap_PLMNidentity_t *)
        core_calloc(1, sizeof(S1ap_PLMNidentity_t));
    s1ap_conv_plmn_id_to_tbcd_string(
        &mme_self()->plmn_id, plmnIdentity);
    ASN_SEQUENCE_ADD(&supportedTA->broadcastPLMNs, plmnIdentity);

    ASN_SEQUENCE_ADD(&ies->supportedTAs, supportedTA);

    message.direction = S1AP_PDU_PR_initiatingMessage;
    message.procedureCode = S1ap_ProcedureCode_id_S1Setup;

    erval = s1ap_encode_pdu(&pkbuf, &message);
    ABTS_INT_EQUAL(tc, 280, erval);

    s1ap_free_pdu(&message);

    pkbuf_free(pkbuf);
}

abts_suite *test_s1ap(abts_suite *suite)
{
    suite = ADD_SUITE(suite)
    {
        extern int _s1enc;
        extern int _s1dec;
        d_trace_level(&_s1enc, 0);
        d_trace_level(&_s1dec, 0);
    }

    abts_run_test(suite, s1ap_test1, NULL);
    abts_run_test(suite, s1ap_test2, NULL);
    abts_run_test(suite, s1ap_test3, NULL);
    abts_run_test(suite, s1ap_test4, NULL);
    abts_run_test(suite, s1ap_test5, NULL);

    return suite;
}
