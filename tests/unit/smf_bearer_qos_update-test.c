#include "test-common.h"
#include "smf_context.h"


smf_bearer_t *bearer = NULL;
smf_sess_t *sess = NULL;
ogs_pfcp_qer_t *qer = NULL;
ogs_pfcp_pdr_t *dl_pdr = NULL;
ogs_pfcp_pdr_t *ul_pdr = NULL;

void test_smf_bearer_qos_update_setup(void)
{
    bearer = malloc(sizeof(smf_bearer_t));
    sess = malloc(sizeof(smf_sess_t));
    qer = malloc(sizeof(ogs_pfcp_qer_t));
    dl_pdr = malloc(sizeof(ogs_pfcp_pdr_t));
    ul_pdr = malloc(sizeof(ogs_pfcp_pdr_t));
    memset(bearer, 0, sizeof(smf_bearer_t));
    memset(sess, 0, sizeof(smf_sess_t));
    memset(qer, 0, sizeof(ogs_pfcp_qer_t));
    memset(dl_pdr, 0, sizeof(ogs_pfcp_pdr_t));
    memset(ul_pdr, 0, sizeof(ogs_pfcp_pdr_t));

    // Set some random values for the bearer
    bearer->sess_id = 1;
    bearer->dl_pdr = dl_pdr;
    bearer->ul_pdr = ul_pdr;
    bearer->qos.mbr.uplink = 666;
    bearer->qos.mbr.downlink = 9000;
    bearer->qos.gbr.uplink = 3000;
    bearer->qos.gbr.downlink = 4000;

    sess->pfcp.per = qer;
    bearer->qer = qer;
}

void test_smf_bearer_qos_update_teardown(void)
{
    free(bearer);
    free(sess);
    free(qer);
    free(dl_pdr);
    free(ul_pdr);
}

void test_smf_bearer_qos_update(abts_case *tc, void *data)
{
    smf_bearer_qos_update(bearer);

    ABTS_ASSERT(tc, bearer->qer != NULL);
    ABTS_ASSERT(tc, bearer->qer->mbr.uplink == 666);
    ABTS_ASSERT(tc, bearer->qer->mbr.downlink == 9000);
    ABTS_ASSERT(tc, bearer->qer->gbr.uplink == 3000);
    ABTS_ASSERT(tc, bearer->qer->gbr.downlink == 4000);    
}

abts_suite *test_smf_bearer_qos_update(abts_suite *suite)
{
    suite = ADD_SUITE(suite);
    abts_add_test(suite, test_smf_bearer_qos_update_setup, NULL, NULL);
    abts_add_test(suite, test_smf_bearer_qos_update, test_smf_bearer_qos_update_setup, test_smf_bearer_qos_update_teardown);
    return suite;
}
