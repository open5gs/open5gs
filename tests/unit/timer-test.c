
#include "smf/timer.h"
#include "core/abts.h"

static void smf_timer_test1(abts_case *tc, void *data)
{
    const char *name;

    // Example timer IDs and expected names
    struct {
        int timer_id;
        const char *expected_name;
    } test_cases[] = {
        {OGS_TIMER_NF_INSTANCE_REGISTRATION_INTERVAL, OGS_TIMER_NAME_NF_INSTANCE_REGISTRATION_INTERVAL},
        {OGS_TIMER_NF_INSTANCE_HEARTBEAT_INTERVAL, OGS_TIMER_NAME_NF_INSTANCE_HEARTBEAT_INTERVAL},
        {OGS_TIMER_NF_INSTANCE_NO_HEARTBEAT, OGS_TIMER_NAME_NF_INSTANCE_NO_HEARTBEAT},
        {OGS_TIMER_NF_INSTANCE_VALIDITY, OGS_TIMER_NAME_NF_INSTANCE_VALIDITY},
        {OGS_TIMER_SUBSCRIPTION_VALIDITY, OGS_TIMER_NAME_SUBSCRIPTION_VALIDITY},
        {OGS_TIMER_SUBSCRIPTION_PATCH, OGS_TIMER_NAME_SUBSCRIPTION_PATCH},
        {OGS_TIMER_SBI_CLIENT_WAIT, OGS_TIMER_NAME_SBI_CLIENT_WAIT},
        {SMF_TIMER_PFCP_ASSOCIATION, "SMF_TIMER_PFCP_ASSOCIATION"},
        {SMF_TIMER_PFCP_NO_HEARTBEAT, "SMF_TIMER_PFCP_NO_HEARTBEAT"},
        {SMF_TIMER_PFCP_NO_ESTABLISHMENT_RESPONSE, "SMF_TIMER_PFCP_NO_ESTABLISHMENT_RESPONSE"},
        {SMF_TIMER_PFCP_NO_DELETION_RESPONSE, "SMF_TIMER_PFCP_NO_DELETION_RESPONSE"},
        {-1, "UNKNOWN_TIMER"} // Unknown timer ID to test default case
    };

    for (int i = 0; test_cases[i].timer_id != -1; i++) {
        name = smf_timer_get_name(test_cases[i].timer_id);
        ABTS_STR_EQUAL(tc, test_cases[i].expected_name, name);
    }
}

abts_suite *test_smf_timer(abts_suite *suite)
{
    suite = ADD_SUITE(suite);

    abts_run_test(suite, smf_timer_test1, NULL);

    return suite;
}