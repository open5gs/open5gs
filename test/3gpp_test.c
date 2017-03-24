#include "core_lib.h"
#include "core_debug.h"

#include "3gpp_defs.h"
#include "3gpp_types.h"
#include "3gpp_conv.h"

#include "testutil.h"

static void _3gpp_test1(abts_case *tc, void *data)
{
    status_t rv;
    plmn_id_t plmn_id;
    c_uint8_t *buffer = (c_uint8_t *)&plmn_id;

    plmn_id_build(&plmn_id, 1, 1, 2);
    ABTS_TRUE(tc, memcmp(buffer, "\x00\xf1\x10", PLMN_ID_LEN) == 0);

    ABTS_INT_EQUAL(tc, 1, plmn_id_mcc(&plmn_id));
    ABTS_INT_EQUAL(tc, 1, plmn_id_mnc(&plmn_id));
    ABTS_INT_EQUAL(tc, 2, plmn_id_mnc_len(&plmn_id));

    plmn_id_build(&plmn_id, 555, 10, 2);
    ABTS_TRUE(tc, memcmp(buffer, "\x55\xf5\x01", PLMN_ID_LEN) == 0);

    ABTS_INT_EQUAL(tc, 555, plmn_id_mcc(&plmn_id));
    ABTS_INT_EQUAL(tc, 10, plmn_id_mnc(&plmn_id));
    ABTS_INT_EQUAL(tc, 2, plmn_id_mnc_len(&plmn_id));
}

abts_suite *test_3gpp(abts_suite *suite)
{
    suite = ADD_SUITE(suite)

    abts_run_test(suite, _3gpp_test1, NULL);

    return suite;
}
