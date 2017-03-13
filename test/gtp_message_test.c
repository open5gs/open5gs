#include "core_pkbuf.h"
#include "core_lib.h"
#include "core_debug.h"

#include "testutil.h"
#include "gtpv2c_tlv.h"

static void gtp_message_test1(abts_case *tc, void *data)
{
}

abts_suite *test_gtp_message(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, gtp_message_test1, NULL);

    return suite;
}
