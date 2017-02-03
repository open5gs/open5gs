#include "core.h"
#include "core_errno.h"
#include "core_general.h"
#include "core_debug.h"
#include "core_pkbuf.h"

#include "testutil.h"

static void s1ap_test1(abts_case *tc, void *data)
{
}

abts_suite *test_s1ap(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, s1ap_test1, NULL);

    return suite;
}
