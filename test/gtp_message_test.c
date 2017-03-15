#include "core_pkbuf.h"
#include "core_lib.h"
#include "core_debug.h"

#include "gtpv2c_tlv.h"

#include "testutil.h"

static void gtp_message_test1(abts_case *tc, void *data)
{
    status_t rv;
    char *_payload = 
        "0100080055153011 340010f44c000600 9471527600414b00 0800536120009178"
        "840056000d001855 f501102255f50100 019d015300030055 f501520001000657"
        "0009008a80000084 0a32360a57000901 87000000000a3236 254700220005766f"
        "6c7465036e673204 6d6e6574066d6e63 303130066d636335 3535046770727380"
        "000100fc63000100 014f000500010000 00007f0001000048 000800000003e800"
        "0007d04e001a0080 8021100100001081 0600000000830600 000000000d00000a"
        "005d001f00490001 0005500016004505 0000000000000000 0000000000000000"
        "0000000072000200 40005f0002005400";
    gtpv2c_create_session_request_t req;
    c_uint8_t buf[240];
    pkbuf_t *pkbuf = NULL;

    {
        extern int _tlv_msg;
        d_trace_level(&_tlv_msg, 0);
    }

    pkbuf = pkbuf_alloc(0, 240);
    ABTS_PTR_NOTNULL(tc, pkbuf);
    core_ascii_to_hex(_payload, strlen(_payload), pkbuf->payload, 
            pkbuf->len);

    memset(&req, 0, sizeof(req));
    rv = tlv_parse_msg(&req, &gtpv2c_desc_create_session_request, pkbuf,
            TLV_MODE_T1_L2_I1);
    ABTS_INT_EQUAL(tc, CORE_OK, rv);

    pkbuf_free(pkbuf);
}

abts_suite *test_gtp_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, gtp_message_test1, NULL);

    return suite;
}
