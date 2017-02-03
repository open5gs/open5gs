#include "core.h"
#include "core_errno.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pkbuf.h"

#include "testutil.h"

#include "s1ap_common.h"
#include "s1ap_ies_defs.h"
#include "s1ap_mme_decoder.h"

static void s1ap_test1(abts_case *tc, void *data)
{
    char *s1setup_msg[] = {
        "\x00\x11\x00\x2d\x00\x00\x04\x00\x3b\x00\x09\x00\x00\xf1\x10\x40\x54\xf6\x40\x10\x00\x3c\x40\x09\x03\x00\x4a\x4c\x54\x2d\x36\x32\x31\x00\x40\x00\x07\x00\x0c\x0e\x40\x00\xf1\x10\x00\x89\x40\x01\x00",
    };
    s1ap_message message;
    int result;

    result = s1ap_mme_decode_pdu(&message, (unsigned char*)s1setup_msg[0], 49);
    ABTS_INT_EQUAL(tc, 0, result);
}

abts_suite *test_s1ap(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, s1ap_test1, NULL);

    return suite;
}
