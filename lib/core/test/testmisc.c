#include "core_lib.h"
#include "core_debug.h"

#include "testutil.h"

static void misc_test1(abts_case *tc, void *data)
{
    unsigned char c[42];
    ABTS_INT_EQUAL(tc, CORE_OK, core_generate_random_bytes(c, sizeof c));
}

abts_suite *testmisc(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, misc_test1, NULL);

    return suite;
}
