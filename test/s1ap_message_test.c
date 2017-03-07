#include "core_debug.h"
#include "core_pkbuf.h"
#include "core_lib.h"

#include "s1ap_build.h"
#include "s1ap_conv.h"

#include "testutil.h"
#include "tests1ap.h"

static void s1ap_message_test1(abts_case *tc, void *data)
{
    /* S1SetupRequest */
    char *payload = 
        "0011002d000004003b00090000f11040"
        "54f64010003c400903004a4c542d3632"
        "3100400007000c0e4000f11000894001"
        "00";

    s1ap_message_t message;
    pkbuf_t *pkbuf;
    int result;

    pkbuf = pkbuf_alloc(0, MESSAGE_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    core_ascii_to_hex(payload, strlen(payload), pkbuf->payload);
    pkbuf->len = 49;

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    s1ap_free_pdu(&message);

    pkbuf_free(pkbuf);
}

static void s1ap_message_test2(abts_case *tc, void *data)
{
    /* InitialUE(Attach Request) */
    char *payload = 
        "000c406f000006000800020001001a00"
        "3c3b17df675aa8050741020bf600f110"
        "000201030003e605f070000010000502"
        "15d011d15200f11030395c0a003103e5"
        "e0349011035758a65d0100e0c1004300"
        "060000f1103039006440080000f1108c"
        "3378200086400130004b00070000f110"
        "000201";

    s1ap_message_t message;
    pkbuf_t *pkbuf;
    int result;

    pkbuf = pkbuf_alloc(0, MESSAGE_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    core_ascii_to_hex(payload, strlen(payload), pkbuf->payload);
    pkbuf->len = 115;

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    s1ap_free_pdu(&message);

    pkbuf_free(pkbuf);
}

static void s1ap_message_test3(abts_case *tc, void *data)
{
    /* initial context setup response */
    char *payload = 
        "2009002500000300004005c0020000bf"
        "0008400200010033400f000032400a0a"
        "1f0a0123c601000908";

    s1ap_message_t message;
    pkbuf_t *pkbuf;
    int result;

    pkbuf = pkbuf_alloc(0, MESSAGE_SDU_SIZE);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    core_ascii_to_hex(payload, strlen(payload), pkbuf->payload);
    pkbuf->len = 41;

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);
    s1ap_free_pdu(&message);

    pkbuf_free(pkbuf);
}

static void s1ap_message_test4(abts_case *tc, void *data)
{
    s1ap_message_t message;
    status_t rv;
    pkbuf_t *pkbuf;
    int result;

    rv = s1ap_build_setup_rsp(&pkbuf);

    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    ABTS_PTR_NOTNULL(tc, pkbuf->payload);
    ABTS_INT_EQUAL(tc, 27, pkbuf->len);

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);

    s1ap_free_pdu(&message);
    pkbuf_free(pkbuf);
}

static void s1ap_message_test5(abts_case *tc, void *data)
{
    s1ap_message_t message;
    status_t rv;
    pkbuf_t *pkbuf;
    int result;

    rv = tests1ap_build_setup_req(&pkbuf, 0x54f64);

    ABTS_INT_EQUAL(tc, CORE_OK, rv);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    ABTS_PTR_NOTNULL(tc, pkbuf->payload);
    ABTS_INT_EQUAL(tc, 35, pkbuf->len);

    result = s1ap_decode_pdu(&message, pkbuf);
    ABTS_INT_EQUAL(tc, 0, result);

    s1ap_free_pdu(&message);
    pkbuf_free(pkbuf);
}

abts_suite *test_s1ap_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, s1ap_message_test1, NULL);
    abts_run_test(suite, s1ap_message_test2, NULL);
    abts_run_test(suite, s1ap_message_test3, NULL);
    abts_run_test(suite, s1ap_message_test4, NULL);
    abts_run_test(suite, s1ap_message_test5, NULL);

    return suite;
}
